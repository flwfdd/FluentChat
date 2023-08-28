//
// Created by flwfdd on 2023/8/27.
//

#ifndef FLUENTCHAT_CONTROL_H
#define FLUENTCHAT_CONTROL_H


#include <QObject>
#include "model/group.h"
#include "model/user.h"
#include "data/net.h"
#include "data/database.h"

class Control : public QObject {
Q_OBJECT

public:
    explicit Control(QObject *parent = nullptr);

    static Control *instance();

    void initConnect();

    Q_INVOKABLE void sendMessage(int gid, const QString &type, const QString &content);

    Q_INVOKABLE void openGroup(GroupModel *item);

    Q_INVOKABLE void loadMessages();

    QList<UserModel *> getUsers(QList<int> ids); // 加载用户到Users Map 返回的可能会延迟加载

    void init();

public slots:

    void initMessageList();

private:
    Net *net;
    Database *db;
    int pageSize = 20;
};


#endif //FLUENTCHAT_CONTROL_H
