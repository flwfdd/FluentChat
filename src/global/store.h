#ifndef STORE_H
#define STORE_H


#include "model/group.h"
#include "model/message.h"
#include "model/user.h"
#include "control.h"
#include <QMap>

class Store : public QObject {
Q_OBJECT

    Q_PROPERTY(QMap<int, UserModel *> *users READ users CONSTANT) //缓存用户的Map
    Q_PROPERTY(GroupListModel *groupList READ groupList CONSTANT) //会话列表
    Q_PROPERTY(MessageListModel *messageList READ messageList CONSTANT) //消息列表
    Q_PROPERTY(UserModel *currentUser READ currentUser WRITE setCurrentUser NOTIFY currentUserChanged FINAL) //当前用户
    Q_PROPERTY(GroupModel *currentGroup READ currentGroup WRITE setCurrentGroup NOTIFY currentGroupChanged FINAL) //当前会话
    Q_PROPERTY(QList<UserModel *> currentGroupUsers READ currentGroupUsers WRITE setCurrentGroupUsers NOTIFY currentGroupUsersChanged FINAL) //当前会话的用户列表

    Q_PROPERTY(Control *control READ control CONSTANT)

public:
    static Store *instance();

    QMap<int, UserModel *> *users() const;

    GroupListModel *groupList() const;

    MessageListModel *messageList() const;

    UserModel *currentUser() const;

    void setCurrentUser(UserModel *user);

    GroupModel *currentGroup() const;

    void setCurrentGroup(GroupModel *group);

    QList<UserModel *> currentGroupUsers() const;

    void setCurrentGroupUsers(QList<UserModel *> users);

    Control *control() const;


signals:

    void currentUserChanged();

    void currentGroupChanged();

    void currentGroupUsersChanged();


private:
    explicit Store(QObject *parent = nullptr);

    QMap<int, UserModel *> *m_users;
    GroupListModel *m_groupList;
    MessageListModel *m_messageList;
    UserModel *m_currentUser;
    GroupModel *m_currentGroup;
    QList<UserModel *> m_currentGroupUsers;
    Control *m_control;
};

#endif // STORE_H
