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

    void setUserId(int userId);

    void saveUsers(QList<UserModel *> users);

    QList<UserModel *> loadUsers(QList<UserModel *> users); // 加载用户 返回加载成功的

    void saveGroups(QList<GroupModel *> groups);

    QList<GroupModel *> getGroups(); // 加载所有群组

    void saveMessages(QList<MessageModel *> messages);

    MessageModel *getMessage(int id); // 加载单条消息

    QList<MessageModel *> getMessages(int gid, int start, int end); // 加载群组消息

    void saveRead(int gid, int mid); // 保存已读消息

    void loadRead(QList<GroupModel *> groups); // 加载已读消息

    void saveFileHash(QString filePath, QString hash); // 保存文件hash

    QMap<QString, QString> getFileHashes(); // 获取所有文件hash

private:
    QSqlDatabase db;

    QString uid;
};


#endif //FLUENTCHAT_DATABASE_H
