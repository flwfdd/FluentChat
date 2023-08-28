#include "message.h"
#include "global/store.h"
#include <QDateTime>
#include <QDebug>

MessageModel::MessageModel(QObject *parent)
        : QObject(parent) {
    m_id = 0;
    m_type = "";
    m_content = "";
    m_time = 0;
    m_user = nullptr;
    m_gid = 0;
    m_mid = 0;
    m_recall = false;
}

MessageModel::MessageModel(int id, const QString &type, const QString &content, qint64 time, UserModel *user, int gid,
                           int mid, bool recall, QObject *parent)
        : QObject(parent), m_id(id), m_type(type), m_content(content), m_time(time), m_user(user), m_gid(gid),
          m_mid(mid),
          m_recall(recall) {
}

int MessageModel::id() const {
    return m_id;
}

void MessageModel::setId(int id) {
    if (m_id != id) {
        m_id = id;
        emit idChanged();
    }
}

QString MessageModel::type() const {
    return m_type;
}

void MessageModel::setType(const QString &type) {
    if (m_type != type) {
        m_type = type;
        emit typeChanged();
    }
}

QString MessageModel::content() const {
    return m_content;
}

void MessageModel::setContent(const QString &content) {
    if (m_content != content) {
        m_content = content;
        emit contentChanged();
    }
}

int MessageModel::time() const {
    return m_time;
}

void MessageModel::setTime(qint64 time) {
    if (m_time != time) {
        m_time = time;
        emit timeChanged();
    }
}

UserModel *MessageModel::user() const {
    return m_user;
}

void MessageModel::setUser(UserModel *user) {
    if (m_user != user) {
        m_user = user;
        emit userChanged();
    }
}

int MessageModel::gid() const {
    return m_gid;
}

void MessageModel::setGid(int gid) {
    if (m_gid != gid) {
        m_gid = gid;
        emit gidChanged();
    }
}

int MessageModel::mid() const {
    return m_mid;
}

void MessageModel::setMid(int sn) {
    if (m_mid != sn) {
        m_mid = sn;
        emit midChanged();
    }
}

bool MessageModel::recall() const {
    return m_recall;
}

void MessageModel::setRecall(bool recall) {
    if (m_recall != recall) {
        m_recall = recall;
        emit recallChanged();
    }
}


MessageListModel::MessageListModel(QObject *parent)
        : QObject{parent} {
    m_items = QList<MessageModel *>();
    m_waitForDeleteItems = QSet<MessageModel *>();
    m_hasMore = false;
}

QList<MessageModel *> MessageListModel::items() const {
    return m_items;
}

void MessageListModel::setItems(const QList<MessageModel *> &items) {
    if (m_items != items) {
        for (auto item: items) {
            if (m_waitForDeleteItems.contains(item)) {
                qDebug() << "remove item from m_waitForDeleteItems";
                m_waitForDeleteItems.remove(item);
            }
        }
        for (auto item: m_waitForDeleteItems) {
            delete item;
        }
        m_waitForDeleteItems.clear();
        auto uselessSet = QSet<MessageModel *>();
        for (auto item: m_items) {
            uselessSet.insert(item);
        }
        for (auto item: items) {
            uselessSet.remove(item);
        }
        for (auto item: uselessSet) {
            m_waitForDeleteItems.insert(item);
        }
        m_items = items;
        emit itemsChanged();
    }
}

bool MessageListModel::hasMore() const {
    return m_hasMore;
}

void MessageListModel::setHasMore(bool hasMore) {
    if (m_hasMore != hasMore) {
        m_hasMore = hasMore;
        emit hasMoreChanged();
    }
}



