#include "group.h"
#include <QDebug>

GroupModel::GroupModel(QObject *parent)
        : QObject(parent) {
    m_id = 0;
    m_type = "";
    m_name = "";
    m_avatar = "";
    m_color = "";
    m_remark = "";
    m_owner = nullptr;
    m_last = nullptr;
    m_read = 0;

    m_waitForDeleteLast = nullptr;
}

GroupModel::GroupModel(int id, const QString &type, const QString &name, const QString &avatar, const QString &color,
                       const QString &remark, UserModel *owner, MessageModel *last, int read, QObject *parent)
        : QObject(parent), m_id(id), m_type(type), m_name(name), m_avatar(avatar), m_color(color), m_remark(remark),
          m_owner(owner), m_last(last), m_read(read) {
}

int GroupModel::id() const {
    return m_id;
}

void GroupModel::setId(int id) {
    if (m_id != id) {
        m_id = id;
        emit idChanged();
    }
}

QString GroupModel::type() const {
    return m_type;
}

void GroupModel::setType(const QString &type) {
    if (m_type != type) {
        m_type = type;
        emit typeChanged();
    }
}

QString GroupModel::name() const {
    return m_name;
}

void GroupModel::setName(const QString &name) {
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
    }
}

QString GroupModel::avatar() const {
    return m_avatar;
}

void GroupModel::setAvatar(const QString &avatar) {
    if (m_avatar != avatar) {
        m_avatar = avatar;
        emit avatarChanged();
    }
}

QString GroupModel::color() const {
    return m_color;
}

void GroupModel::setColor(const QString &color) {
    if (m_color != color) {
        m_color = color;
        emit colorChanged();
    }
}

QString GroupModel::remark() const {
    return m_remark;
}

void GroupModel::setRemark(const QString &remark) {
    if (m_remark != remark) {
        m_remark = remark;
        emit remarkChanged();
    }
}

UserModel *GroupModel::owner() const {
    return m_owner;
}

void GroupModel::setOwner(UserModel *owner) {
    if (m_owner != owner) {
        m_owner = owner;
        emit ownerChanged();
    }
}


MessageModel *GroupModel::last() const {
    return m_last;
}

void GroupModel::setLast(MessageModel *last) {
    if (m_last != last) {
        delete m_waitForDeleteLast;
        m_waitForDeleteLast = m_last;
        m_last = last;
        emit lastChanged();
        emit unreadNumChanged();
    }
}

int GroupModel::read() const {
    return m_read;
}

void GroupModel::setRead(int read) {
    if (m_read != read) {
        m_read = read;
        emit readChanged();
        emit unreadNumChanged();
    }
}

int GroupModel::unreadNum() const {
    if (!m_last)return 0;
    return m_last->mid() - m_read;
}


GroupListModel::GroupListModel(QObject *parent) : QObject(parent) {
    m_items = QList<GroupModel *>();
    m_currentItem = nullptr;
    m_waitForDeleteItems = QList<GroupModel *>();
}

QList<GroupModel *> GroupListModel::items() const {
    return m_items;
}

void GroupListModel::setItems(QList<GroupModel *> items) {
    if (m_items != items) {
        for (auto item: m_waitForDeleteItems) {
            delete item;
        }
        m_waitForDeleteItems = m_items;
        m_items = items;
        sortItems();
    }
}

void GroupListModel::sortItems() {
    // 按最后一条消息时间排序
    std::sort(m_items.begin(), m_items.end(), [](GroupModel *a, GroupModel *b) {
        if (a->last() == nullptr)return false;
        if (b->last() == nullptr)return true;
        return a->last()->time() > b->last()->time();
    });
    emit itemsChanged();
}

GroupModel *GroupListModel::currentItem() const {
    return m_currentItem;
}

void GroupListModel::setCurrentItem(GroupModel *currentItem) {
    if (m_currentItem != currentItem) {
        m_currentItem = currentItem;
        emit currentItemChanged();
    }
}
