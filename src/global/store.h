#ifndef STORE_H
#define STORE_H


#include "model/chat_list.h"
#include "model/message_list.h"
#include "model/user.h"
#include <QMap>
class Store: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QMap<int,UserModel*>* users READ users CONSTANT)
    Q_PROPERTY(ChatListModel* chatList READ chatList CONSTANT)
    Q_PROPERTY(MessageListModel* messageList READ messageList CONSTANT)
    Q_PROPERTY(UserModel* currentUser READ currentUser WRITE setCurrentUser NOTIFY currentUserChanged FINAL)

public:
    static Store* instance();

    QMap<int,UserModel*>* users() const;
    ChatListModel* chatList() const;
    MessageListModel* messageList() const;

    UserModel* currentUser() const;
    void setCurrentUser(UserModel *user);

signals:
    void currentUserChanged();


private:
    explicit Store(QObject *parent = nullptr);

    QMap<int,UserModel*>* m_users;
    UserModel* m_currentUser;
    ChatListModel* m_chatList;
    MessageListModel* m_messageList;
};

#endif // STORE_H
