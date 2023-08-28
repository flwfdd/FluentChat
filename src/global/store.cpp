#include "store.h"

Store::Store(QObject *parent) : QObject(parent) {
    m_users = new QMap<int, UserModel *>();
    m_groupList = new GroupListModel();
    m_messageList = new MessageListModel();
    m_currentUser = nullptr;
    m_currentGroup = nullptr;
    m_currentGroupUsers = QList<UserModel *>();
    m_control = Control::instance();
}

Store *Store::instance() {
    static Store *store = new Store();
    return store;
}

QMap<int, UserModel *> *Store::users() const {
    return m_users;
}

GroupListModel *Store::groupList() const {
    return m_groupList;
}

MessageListModel *Store::messageList() const {
    return m_messageList;
}

UserModel *Store::currentUser() const {
    return m_currentUser;
}

void Store::setCurrentUser(UserModel *user) {
    if (m_currentUser != user) {
        m_currentUser = user;
        emit currentUserChanged();
    }
}

GroupModel *Store::currentGroup() const {
    return m_currentGroup;
}

void Store::setCurrentGroup(GroupModel *group) {
    if (m_currentGroup != group) {
        m_currentGroup = group;
        m_currentGroupUsers.clear();
        // TODO: load group users
        emit currentGroupChanged();
        emit currentGroupUsersChanged();
    }
}

QList<UserModel *> Store::currentGroupUsers() const {
    return m_currentGroupUsers;
}

void Store::setCurrentGroupUsers(QList<UserModel *> users) {
    if (m_currentGroupUsers != users) {
        m_currentGroupUsers = users;
        emit currentGroupUsersChanged();
    }
}

Control *Store::control() const {
    return m_control;
}



