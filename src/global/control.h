//
// Created by flwfdd on 2023/8/27.
//

#ifndef FLUENTCHAT_CONTROL_H
#define FLUENTCHAT_CONTROL_H


#include <QObject>
#include <QTimer>
#include <QJSValue>
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

    Q_INVOKABLE void sendMessage(int gid, QString type, QString content);

    Q_INVOKABLE void sendImage(int gid, QString filePath);

    Q_INVOKABLE void sendFile(int gid, QString filePath, QString fileName);

    Q_INVOKABLE void openGroup(GroupModel *item);

    Q_INVOKABLE void loadMessages();

    Q_INVOKABLE void init();

    Q_INVOKABLE void login(const QString &username, const QString &password);

    Q_INVOKABLE void
    registerUser(const QString &username, const QString &password, const QString &nickname, const QString &color,
                 const QString &avatar);

    Q_INVOKABLE void requestUser(QString username);

    Q_INVOKABLE void requestGroup(int gid);

    Q_INVOKABLE void createGroup(const QString &name, const QString &avatar, const QString &color);

    Q_INVOKABLE void setGroupRemark(QString remark);

    Q_INVOKABLE void saveBase64File(QString filePath,QString base64);

    QList<UserModel *> getUsers(QList<int> ids); // 加载用户到Users Map 返回的可能会延迟加载

    void showSuccess(const QString &message);

    void showError(const QString &message);



public
    slots:

    void initMessageList();

    void receiveMessage(MessageModel *message);

    void updateOnlineStatus();

private:
    Net *net;
    Database *db;
    int pageSize = 24;

    void mergeMessageList(QList<MessageModel *> list, bool front, bool replace = false);

    QSet<UserModel *> checkOnlineUids;
    int onlineStatusTimerInterval = 1000 * 30; // 30s
};


#endif //FLUENTCHAT_CONTROL_H
