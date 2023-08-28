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
#include "model/group.h"

class Net : public QObject {
Q_OBJECT

public:
    explicit Net(QObject *parent = nullptr);

    static Net *instance();

    void setCookie(const QString &cookie);

    void get(const QString &url, QMap<QString, QString> params, const std::function<void(QJsonDocument)>& callback);

    void post(const QString &url, const QJsonDocument &json, const std::function<void(QJsonDocument)>& callback);

    void loadUserFromJson(const QJsonObject &json, UserModel *user);

    void loadGroupFromJson(const QJsonObject &json, GroupModel *group);

    void loadMessageFromJson(const QJsonObject &json, MessageModel *message);

    void loadUsers(const QList<UserModel*>& users); // 加载用户到Users Map并保存到数据库

    void loadGroups(); // 加载群组到chatList并保存到数据库

    void getMessages(int gid, int start, int end, const std::function<void(QList<MessageModel*>)> &callback); // 加载群组消息并保存到数据库 -1为未设置

private:
    const QString baseUrl = "http://127.0.0.1:4523/m1/3184744-0-default";
    QString cookie;
    QNetworkAccessManager *manager;
};


#endif //FLUENTCHAT_NET_H
