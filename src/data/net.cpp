//
// Created by 15258 on 2023/8/27.
//

#include <QUrlQuery>
#include <QNetworkInterface>
#include <QTcpServer>
#include "net.h"
#include "global/store.h"

Net::Net(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
}

Net *Net::instance() {
    static Net *net = new Net();
    return net;
}

QString Net::baseUrl() {
    //    const QString baseUrl = "http://127.0.0.1:4523/m1/3184744-0-default";
//    const QString baseUrl = "http://10.179.243.187:1235";
    auto url = Store::instance()->getConfig("httpURL");
    if (url.isEmpty()) {
        url = "http://127.0.0.1:1235";
        Store::instance()->setConfig("httpURL", url);
    }
    return url;
}

void Net::get(const QString &url, QMap<QString, QString> params, const std::function<void(QJsonDocument)> &callback) {
    QNetworkRequest request;
    QString urlStr = baseUrl() + url;
    QUrlQuery query;
    for (auto it = params.begin(); it != params.end(); it++) {
        query.addQueryItem(it.key(), it.value());
    }
    urlStr += "?" + query.toString();
    request.setUrl(QUrl(urlStr));
    request.setRawHeader("Cookie", Store::instance()->getConfig("cookie").toUtf8());
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray bytes = reply->readAll();
            auto doc = QJsonDocument(QJsonDocument::fromJson(bytes));
            callback(doc);
        } else {
            qDebug() << reply->errorString();
            if (reply->error() == QNetworkReply::AuthenticationRequiredError) {
                Store::instance()->setConfig("cookie", "");
            }
            try {
                auto doc = QJsonDocument(QJsonDocument::fromJson(reply->readAll()));
                auto json = doc.object();
                if (!json["msg"].isNull())Control::instance()->showError(json["msg"].toString());
            } catch (...) {}
        }
    });

}

void Net::post(const QString &url, const QJsonDocument &json, const std::function<void(QJsonDocument)> &callback) {
    QNetworkRequest request;
    QString urlStr = baseUrl() + url;
    request.setUrl(QUrl(urlStr));
    request.setRawHeader("Cookie", Store::instance()->getConfig("cookie").toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = manager->post(request, json.toJson());
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray bytes = reply->readAll();
            auto doc = QJsonDocument(QJsonDocument::fromJson(bytes));
            callback(doc);
        } else {
            qDebug() << reply->errorString();
            if (reply->error() == QNetworkReply::AuthenticationRequiredError) {
                Store::instance()->setConfig("cookie", "");
            }
            try {
                auto doc = QJsonDocument(QJsonDocument::fromJson(reply->readAll()));
                auto json = doc.object();
                if (!json["msg"].isNull())Control::instance()->showError(json["msg"].toString());
            } catch (...) {}
        }
    });

}

void Net::loadUserFromJson(const QJsonObject &json, UserModel *user) {
    user->setId(json["id"].toInt());
    user->setUsername(json["username"].toString());
    user->setNickname(json["nickname"].toString());
    user->setColor(json["color"].toString());
    user->setAvatar(json["avatar"].toString());
}

void Net::loadGroupFromJson(const QJsonObject &json, GroupModel *group) {
    group->setId(json["id"].toInt());
    group->setType(json["type"].toString());
    group->setName(json["name"].toString());
    group->setAvatar(json["avatar"].toString());
    group->setColor(json["color"].toString());
    if (json["type"] == "twin") {
        auto uids = json["name"].toString().split(",");
        auto uid0 = uids[0].toInt();
        auto uid1 = uids[1].toInt();
        if (uid0 == Store::instance()->currentUser()->id()) {
            group->setOwner(Control::instance()->getUsers(QList < int > () << uid1)[0]);
        } else {
            group->setOwner(Control::instance()->getUsers(QList < int > () << uid0)[0]);
        }
    } else group->setOwner(Control::instance()->getUsers(QList < int > () << json["owner"].toInt())[0]);
    if (json["last"].isNull())group->setLast(nullptr);
    else {
        auto last = new MessageModel();
        loadMessageFromJson(json["last"].toObject(), last);
        group->setLast(last);
    }
}

void Net::loadMessageFromJson(const QJsonObject &json, MessageModel *message) {
    message->setId(json["id"].toInt());
    message->setType(json["type"].toString());
    message->setContent(json["content"].toString());
    message->setTime(json["time"].toInteger());
    message->setUser(Control::instance()->getUsers(QList < int > () << json["uid"].toInt())[0]);
    message->setGid(json["gid"].toInt());
    message->setMid(json["mid"].toInt());
}

void Net::loadUsers(const QList<UserModel *> &users) {
    if (users.isEmpty())return;
    QJsonObject requestJson;
    auto uids = QString();
    for (auto user: users) {
        uids += QString::number(user->id()) + ",";
    }
    uids = uids.left(uids.length() - 1);
    requestJson["uids"] = uids;
    post("/user/infos", QJsonDocument(requestJson), [=](const QJsonDocument &doc) {
        auto jsonList = doc.array();
        auto users = Store::instance()->users();
        auto list = QList < UserModel * > ();
        for (auto &&it: jsonList) {
            auto json = it.toObject();
            auto user = users->value(json["id"].toInt());
            loadUserFromJson(json, user);
            list.append(user);
        }
        // 保存到数据库
        Database::instance()->saveUsers(list);
    });
}

void Net::loadGroups() {
    get("/group/list", QMap<QString, QString>(), [=](const QJsonDocument &doc) {
        auto jsonList = doc.array();
        auto groups = QList < GroupModel * > ();
        auto uids = QList < int > ();
        auto groupJsonList = QList < QJsonObject > ();
        for (auto &&it: jsonList) {
            auto json = it.toObject();
            groupJsonList.append(json);
            uids.append(json["owner"].toInt());
        }
        // 加载用户
        Control::instance()->getUsers(uids);
        // 加载群组
        for (auto &&json: groupJsonList) {
            auto group = new GroupModel();
            loadGroupFromJson(json, group);
            groups.append(group);
        }
        auto messages = QList < MessageModel * > ();
        for (auto group: groups) {
            if (group->last() != nullptr)
                messages.append(group->last());
        }
        //加载已读
        Database::instance()->loadRead(groups);
        // 保存到数据库
        Database::instance()->saveMessages(messages);
        Database::instance()->saveGroups(groups);
        // 加载到groupList
        Store::instance()->groupList()->setItems(groups);
    });
}

void Net::getMessages(int gid, int start, int end, const std::function<void(QList < MessageModel * > )> &callback) {
    QMap<QString, QString> params;
    params["gid"] = QString::number(gid);
    if (start != -1)params["start"] = QString::number(start);
    if (end != -1)params["end"] = QString::number(end);
    get("/message/history", params, [=](const QJsonDocument &doc) {
        auto jsonList = doc.array();
        auto messageJsonList = QList < QJsonObject > ();
        auto messages = QList < MessageModel * > ();
        auto uids = QList < int > ();
        for (auto &&it: jsonList) {
            auto json = it.toObject();
            messageJsonList.append(json);
            uids.append(json["uid"].toInt());
        }
        // 加载用户
        Control::instance()->getUsers(uids);
        // 加载消息
        for (auto &&json: messageJsonList) {
            auto message = new MessageModel();
            loadMessageFromJson(json, message);
            messages.append(message);
        }
        // 保存到数据库
        Database::instance()->saveMessages(messages);
        callback(messages);
    });
}

void Net::getGroupUsers(int gid, const std::function<void(QList < UserModel * > )> &callback) {
    QMap<QString, QString> params;
    params["gid"] = QString::number(gid);
    get("/group/users", params, [=](const QJsonDocument &doc) {
        auto jsonList = doc.array();
        auto users = QList < UserModel * > ();
        auto uids = QList < int > ();
        for (auto &&it: jsonList) {
            uids.append(it.toInt());
        }
        callback(Control::instance()->getUsers(uids));
    });
}

void Net::login(const QString &username, const QString &password, const std::function<void(UserModel *)> &callback) {
    QJsonObject requestJson;
    requestJson["username"] = username;
    requestJson["password"] = password;
    post("/user/login", QJsonDocument(requestJson), [=](const QJsonDocument &doc) {
        auto json = doc.object();
        auto uid = json["user"].toObject()["id"].toInt();
        auto user = Control::instance()->getUsers(QList < int > () << uid)[0];  // 加载用户
        Store::instance()->setConfig("cookie", json["cookie"].toString());
        Store::instance()->setConfig("loginUid", QString::number(uid));
        Store::instance()->setConfig("loginUsername", username);
        callback(user);
    });
}

void Net::resgisterUser(const QString &username, const QString &password, const QString &nickname,
                        const QString &color, const QString &avatar, const std::function<void(UserModel *)> &callback) {
    QJsonObject requestJson;
    requestJson["username"] = username;
    requestJson["password"] = password;
    requestJson["nickname"] = nickname;
    requestJson["color"] = color;
    requestJson["avatar"] = avatar;
    post("/user/register", QJsonDocument(requestJson), [=](const QJsonDocument &doc) {
        auto json = doc.object();
        auto uid = json["user"].toObject()["id"].toInt();
        auto user = Control::instance()->getUsers(QList < int > () << uid)[0];  // 加载用户
        Store::instance()->setConfig("cookie", json["cookie"].toString());
        Store::instance()->setConfig("loginUid", QString::number(uid));
        Store::instance()->setConfig("loginUsername", username);
        callback(user);
    });
}

void Net::requestUser(QString username, const std::function<void()> &callback) {
    QJsonObject requestJson;
    requestJson["username"] = username;
    requestJson["text"] = "";
    post("/friend/request", QJsonDocument(requestJson), [=](const QJsonDocument &doc) {
        callback();
    });
}

void Net::requestGroup(int gid, const std::function<void()> &callback) {
    QJsonObject requestJson;
    requestJson["gid"] = gid;
    post("/group/request", QJsonDocument(requestJson), [=](const QJsonDocument &doc) {
        callback();
    });
}

void Net::createGroup(const QString &name, const QString &avatar, const QString &color,
                      const std::function<void()> &callback) {
    QJsonObject requestJson;
    requestJson["name"] = name;
    requestJson["avatar"] = avatar;
    requestJson["color"] = color;
    post("/group/new", QJsonDocument(requestJson), [=](const QJsonDocument &doc) {
        callback();
    });
}

void Net::getOnlineStatus(const QList<int> &uids, const std::function<void(QList < bool > )> &callback) {
    QJsonObject requestJson;
    auto uidsStr = QString();
    for (auto uid: uids) {
        uidsStr += QString::number(uid) + ",";
    }
    uidsStr = uidsStr.left(uidsStr.length() - 1);
    requestJson["uids"] = uidsStr;
    post("/user/onlines", QJsonDocument(requestJson), [=](const QJsonDocument &doc) {
        auto jsonList = doc.array();
        auto list = QList < bool > ();
        for (auto &&it: jsonList) {
            list.append(it.toBool());
        }
        callback(list);
    });
}

void Net::sendMessage(int gid, QString type, QString content, const std::function<void()> &callback) {
    QJsonObject requestJson;
    requestJson["gid"] = gid;
    requestJson["type"] = type;
    requestJson["content"] = content;
    post("/message/send", QJsonDocument(requestJson), [=](const QJsonDocument &doc) {
        callback();
    });
}


QList<QString> Net::getIPs() {
    QList < QString > ips;
    QList < QNetworkInterface > nets = QNetworkInterface::allInterfaces();
    for (int i = 0; i < nets.count(); i++) {
        if (nets[i].flags().testFlag(QNetworkInterface::IsUp) &&
            nets[i].flags().testFlag(QNetworkInterface::IsRunning) &&
            !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack)) {
            QList < QNetworkAddressEntry > addrs = nets[i].addressEntries();
            for (int j = 0; j < addrs.count(); j++) {
                if (addrs[j].ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    ips.append(addrs[j].ip().toString());
                }
            }
        }
    }
    return ips;
}

quint16 Net::getPort() {
    QTcpServer tcpServer;
    tcpServer.listen(QHostAddress::Any, 0);
    quint16 port = tcpServer.serverPort();
    tcpServer.close();
    return port;
}

void Net::getUserAddress(int uid, const std::function<void(QString, quint16)> &callback) {
    QMap<QString, QString> params;
    params["uid"] = QString::number(uid);
    get("/user/ip", params, [=](const QJsonDocument &doc) {
        auto json = doc.object();
        callback(json["ip"].toString(), json["port"].toString().toInt());
    });
}



Ws::Ws(QObject *parent) : QObject(parent) {
    socket = new QWebSocket();

    connect(socket, &QWebSocket::disconnected, [=]() {
        qDebug() << "disconnected";
        Control::instance()->showError("连接服务器失败");
    });
}

Ws *Ws::instance() {
    static Ws *ws = new Ws();
    return ws;
}

void Ws::init() {
    if (Store::instance()->getConfig("cookie").isEmpty())return;

    QString urlStr = Store::instance()->getConfig("wsURL");
    if (urlStr.isEmpty()) {
        urlStr = "ws://127.0.0.1:1234";
        Store::instance()->setConfig("wsURL", urlStr);
    }
    socket->open(QUrl(urlStr));

    connect(socket, &QWebSocket::textMessageReceived, [=](const QString &message) {
        qDebug() << "receive:" << message;
        auto messageModel = new MessageModel();
        auto json = QJsonDocument::fromJson(message.toUtf8()).object();
        if (json["type"] == "message") {
            Net::instance()->loadMessageFromJson(json["data"].toObject(), messageModel);
            Control::instance()->receiveMessage(messageModel);
        }
    });

    connect(socket, &QWebSocket::connected, [=]() {
        qDebug() << "connected";

        QJsonObject requestJson;
        requestJson["cookie"] = Store::instance()->getConfig("cookie");
        requestJson["ip"] = Store::instance()->IP();
        requestJson["port"] = QString::number(Store::instance()->Port());

        qDebug() << "send:" << QJsonDocument(requestJson).toJson();
        socket->sendTextMessage(QJsonDocument(requestJson).toJson());
    });
}

