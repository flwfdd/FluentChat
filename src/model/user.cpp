#include "user.h"

UserModel::UserModel(QObject *parent)
        : QObject(parent) {
    m_id = 0;
    m_username = "";
    m_nickname = "";
    m_color = "";
    m_avatar = "";
    m_remark = "";
    m_online = false;
}

UserModel::UserModel(int id, const QString &username, const QString &nickname, const QString &color,
                     const QString &avatar, const QString &remark, bool online, QObject *parent)
        : QObject(parent), m_id(id), m_username(username), m_nickname(nickname), m_color(color), m_avatar(avatar),
          m_remark(remark), m_online(online) {
}

int UserModel::id() const {
    return m_id;
}

void UserModel::setId(int id) {
    if (m_id != id) {
        m_id = id;
        emit idChanged();
    }
}

QString UserModel::username() const {
    return m_username;
}

void UserModel::setUsername(const QString &username) {
    if (m_username != username) {
        m_username = username;
        emit usernameChanged();
    }
}

QString UserModel::nickname() const {
    return m_nickname;
}

void UserModel::setNickname(const QString &nickname) {
    if (m_nickname != nickname) {
        m_nickname = nickname;
        emit nicknameChanged();
    }
}

QString UserModel::color() const {
    return m_color;
}

void UserModel::setColor(const QString &color) {
    if (m_color != color) {
        m_color = color;
        emit colorChanged();
    }
}

QString UserModel::avatar() const {
    return m_avatar;
}

void UserModel::setAvatar(const QString &avatar) {
    if (m_avatar != avatar) {
        m_avatar = avatar;
        emit avatarChanged();
    }
}

QString UserModel::remark() const {
    return m_remark;
}

void UserModel::setRemark(const QString &remark) {
    if (m_remark != remark) {
        m_remark = remark;
        emit remarkChanged();
    }
}

bool UserModel::online() const {
    return m_online;
}

void UserModel::setOnline(bool online) {
    if (m_online != online) {
        m_online = online;
        emit onlineChanged();
    }
}
