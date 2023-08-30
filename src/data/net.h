//
// Created by 15258 on 2023/8/27.
//

#ifndef FLUENTCHAT_NET_H
#define FLUENTCHAT_NET_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QWebSocket>
#include <QJSValue>
#include "model/group.h"

class Net : public QObject {
Q_OBJECT

public:
    explicit Net(QObject *parent = nullptr);

    static Net *instance();

    void setCookie(const QString &cookie);

    void get(const QString &url, QMap<QString, QString> params, const std::function<void(QJsonDocument)> &callback);

    void post(const QString &url, const QJsonDocument &json, const std::function<void(QJsonDocument)> &callback);

    void loadUserFromJson(const QJsonObject &json, UserModel *user);

    void loadGroupFromJson(const QJsonObject &json, GroupModel *group);

    void loadMessageFromJson(const QJsonObject &json, MessageModel *message);

    void loadUsers(const QList<UserModel *> &users); // 加载用户到Users Map并保存到数据库

    void loadGroups(); // 加载群组到chatList并保存到数据库

    void getMessages(int gid, int start, int end,
                     const std::function<void(QList<MessageModel *>)> &callback); // 加载群组消息并保存到数据库 -1为未设置

    void getGroupUsers(int gid, const std::function<void(QList<UserModel *>)> &callback); // 加载群组用户

    void login(const QString &username, const QString &password, const std::function<void(UserModel *)> &callback);

    void resgisterUser(const QString &username, const QString &password, const QString &nickname, const QString &color,
                       const QString &avatar, const std::function<void(UserModel *)> &callback);

    void requestUser(QString username, const std::function<void()> &callback);

    void requestGroup(int gid, const std::function<void()> &callback);

    void createGroup(const QString &name, const QString &avatar, const QString &color,
                     const std::function<void()> &callback);

    void getOnlineStatus(const QList<int> &uids, const std::function<void(QList<bool>)> &callback);

    void sendMessage(int gid, QString type, QString content, const std::function<void()> &callback);

    void getUserAddress(int uid, const std::function<void(QString,quint16)> &callback);

    QList<QString> getIPs();

    quint16 getPort();

private:

    QString baseUrl();

    QNetworkAccessManager *manager;
};

class Ws : public QObject {
Q_OBJECT

public:
    explicit Ws(QObject *parent = nullptr);

    static Ws *instance();

    void init();

private:
    QWebSocket *socket;
};


#endif //FLUENTCHAT_NET_H
