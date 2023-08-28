//
// Created by flwfdd on 2023/8/27.
//

#ifndef FLUENTCHAT_DATABASE_H
#define FLUENTCHAT_DATABASE_H


#include "model/user.h"
#include "model/group.h"
#include <QSqlDatabase>

class Database {
    Database();

public:
    static Database *instance();

    void saveUsers(QList<UserModel *> users);

    QList<UserModel *> loadUsers(QList<UserModel *> users); // 加载用户 返回加载成功的

    void saveGroups(QList<GroupModel *> groups);

    QList<GroupModel *> getGroups(); // 加载所有群组

    void saveMessages(QList<MessageModel *> messages);

    MessageModel *getMessage(int id); // 加载单条消息

    QList<MessageModel *> loadMessages(int gid, int start, int end); // 加载群组消息

private:
    QSqlDatabase db;
};


#endif //FLUENTCHAT_DATABASE_H
