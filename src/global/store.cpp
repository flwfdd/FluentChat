#include "store.h"

Store::Store(QObject *parent) : QObject(parent)
{
    m_users = new QMap<int,UserModel*>();
    m_chatList = new ChatListModel();
    m_messageList = new MessageListModel();
    m_currentUser = nullptr;
}

Store* Store::instance()
{
    static Store* store = new Store();
    return store;
}

QMap<int,UserModel*>* Store::users() const
{
    return m_users;
}

UserModel* Store::currentUser() const
{
    return m_currentUser;
}

void Store::setCurrentUser(UserModel *user)
{
    if (m_currentUser != user) {
        m_currentUser = user;
        emit currentUserChanged();
    }
}

ChatListModel* Store::chatList() const
{
    return m_chatList;
}

MessageListModel* Store::messageList() const
{
    return m_messageList;
}

