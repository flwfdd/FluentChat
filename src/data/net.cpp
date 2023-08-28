//
// Created by 15258 on 2023/8/27.
//

#include <QUrlQuery>
#include "net.h"
#include "global/store.h"

Net::Net(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
}

Net *Net::instance() {
    static Net *net = new Net();
    return net;
}

void Net::setCookie(const QString &cookie_) {
    this->cookie = cookie_;
}

void Net::get(const QString &url, QMap<QString, QString> params, const std::function<void(QJsonDocument)> &callback) {
    QNetworkRequest request;
    QString urlStr = baseUrl + url;
    QUrlQuery query;
    for (auto it = params.begin(); it != params.end(); it++) {
        query.addQueryItem(it.key(), it.value());
    }
    urlStr += "?" + query.toString();
    request.setUrl(QUrl(urlStr));
    request.setRawHeader("Cookie", cookie.toUtf8());
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray bytes = reply->readAll();
            auto doc = QJsonDocument(QJsonDocument::fromJson(bytes));
            callback(doc);
        } else {
            qDebug() << reply->errorString();
        }
    });

}

void Net::post(const QString &url, const QJsonDocument &json, const std::function<void(QJsonDocument)> &callback) {
    QNetworkRequest request;
    QString urlStr = baseUrl + url;
    request.setUrl(QUrl(urlStr));
    request.setRawHeader("Cookie", cookie.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = manager->post(request, json.toJson());
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray bytes = reply->readAll();
            auto doc = QJsonDocument(QJsonDocument::fromJson(bytes));
            callback(doc);
        } else {
            qDebug() << reply->errorString();
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
    group->setOwner(Control::instance()->getUsers(QList<int>() << json["owner"].toInt())[0]);
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
    message->setUser(Control::instance()->getUsers(QList<int>() << json["uid"].toInt())[0]);
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
        auto list = QList<UserModel *>();
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
        auto groups = QList<GroupModel *>();
        auto uids = QList<int>();
        auto groupJsonList = QList<QJsonObject>();
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
        // 保存到数据库
        Database::instance()->saveGroups(groups);
        // 加载到groupList
        Store::instance()->groupList()->setItems(groups);
    });
}

void Net::getMessages(int gid, int start, int end, const std::function<void(QList<MessageModel *>)> &callback) {
    QMap<QString, QString> params;
    params["gid"] = QString::number(gid);
    if (start != -1)params["start"] = QString::number(start);
    if (end != -1)params["end"] = QString::number(end);
    get("/message/history", params, [=](const QJsonDocument &doc) {
        auto jsonList = doc.array();
        auto messageJsonList = QList<QJsonObject>();
        auto messages = QList<MessageModel *>();
        auto uids = QList<int>();
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
