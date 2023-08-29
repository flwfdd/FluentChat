#ifndef STORE_H
#define STORE_H


#include "model/group.h"
#include "model/message.h"
#include "model/user.h"
#include "control.h"
#include <QMap>
#include <QSettings>

class Store : public QObject {
Q_OBJECT

    Q_PROPERTY(QMap<int, UserModel *> *users READ users CONSTANT) //缓存用户的Map
    Q_PROPERTY(GroupListModel *groupList READ groupList CONSTANT) //会话列表
    Q_PROPERTY(MessageListModel *messageList READ messageList CONSTANT) //消息列表
    Q_PROPERTY(UserModel *currentUser READ currentUser WRITE setCurrentUser NOTIFY currentUserChanged FINAL) //当前用户
    Q_PROPERTY(GroupModel *currentGroup READ currentGroup WRITE setCurrentGroup NOTIFY currentGroupChanged FINAL) //当前会话
    Q_PROPERTY(QList<UserModel *> currentGroupUsers READ currentGroupUsers WRITE setCurrentGroupUsers NOTIFY currentGroupUsersChanged FINAL) //当前会话的用户列表

    Q_PROPERTY(QString successMsg READ successMsg WRITE setSuccessMsg NOTIFY successMsgChanged FINAL)
    Q_PROPERTY(QString errorMsg READ errorMsg WRITE setErrorMsg NOTIFY errorMsgChanged FINAL)
    Q_PROPERTY(bool isLogin READ isLogin WRITE setIsLogin NOTIFY isLoginChanged FINAL)


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

    QString successMsg() const;

    void setSuccessMsg(const QString &msg);

    QString errorMsg() const;

    void setErrorMsg(const QString &msg);

    bool isLogin() const;

    void setIsLogin(bool isLogin);

    Control *control() const;

    Q_INVOKABLE void setConfig(const QString &key, const QString &value);
    Q_INVOKABLE QString getConfig(const QString &key, const QString &defaultValue = "");


signals:

    void currentUserChanged();

    void currentGroupChanged();

    void currentGroupUsersChanged();

    void successMsgChanged();

    void errorMsgChanged();

    void isLoginChanged();


private:
    explicit Store(QObject *parent = nullptr);

    QMap<int, UserModel *> *m_users;
    GroupListModel *m_groupList;
    MessageListModel *m_messageList;
    UserModel *m_currentUser;
    GroupModel *m_currentGroup;
    QList<UserModel *> m_currentGroupUsers;
    Control *m_control;
    QString m_successMsg;
    QString m_errorMsg;
    QSettings* m_settings;
    QMap<QString, QString> m_config;
    bool m_isLogin;
};

#endif // STORE_H
