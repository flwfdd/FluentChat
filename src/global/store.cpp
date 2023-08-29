#include <QCoreApplication>
#include "store.h"

Store::Store(QObject *parent) : QObject(parent) {
    m_users = new QMap<int, UserModel *>();
    m_groupList = new GroupListModel();
    m_messageList = new MessageListModel();
    m_currentUser = nullptr;
    m_currentGroup = nullptr;
    m_currentGroupUsers = QList<UserModel *>();
    m_errorMsg = "";
    m_successMsg = "";
    m_isLogin = false;
    m_control = Control::instance();
    m_settings = new QSettings("fluentchat.ini", QSettings::IniFormat);
    m_config = QMap<QString, QString>();
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
    if (user)Database::instance()->setUserId(user->id());
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

QString Store::successMsg() const {
    return m_successMsg;
}

void Store::setSuccessMsg(const QString &msg) {
    m_successMsg = msg;
    emit successMsgChanged();
}

QString Store::errorMsg() const {
    return m_errorMsg;
}

void Store::setErrorMsg(const QString &msg) {
    m_errorMsg = msg;
    emit errorMsgChanged();
}

bool Store::isLogin() const {
    return m_currentUser != nullptr;
}

void Store::setIsLogin(bool isLogin) {
    if (isLogin != m_isLogin) {
        m_isLogin = isLogin;
    }
    emit isLoginChanged();
}

Control *Store::control() const {
    return m_control;
}

void Store::setConfig(const QString &key, const QString &value) {
    m_config.insert(key, value);
    m_settings->setValue(key, value);
}

QString Store::getConfig(const QString &key, const QString &defaultValue) {
    if (m_config.contains(key))return m_config.value(key);
    return m_settings->value(key, defaultValue).toString();
}



