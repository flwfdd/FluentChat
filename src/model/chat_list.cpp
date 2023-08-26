#include "chat_list.h"
#include <QDebug>

ChatListModelItem::ChatListModelItem(QObject *parent) : QObject(parent),
    m_id(0),
    m_unreadNum(0)
{
}

ChatListModelItem::ChatListModelItem(int id, UserModel* user, const QString &text, qint64 time, int unreadNum, QObject *parent)
    : QObject(parent), m_id(id), m_user(user), m_text(text), m_unreadNum(unreadNum), m_time(time)
{
}

int ChatListModelItem::id() const
{
    return m_id;
}

void ChatListModelItem::setId(int id)
{
    if (m_id != id) {
        m_id = id;
        emit idChanged();
    }
}

UserModel* ChatListModelItem::user() const
{
    return m_user;
}

void ChatListModelItem::setUser(UserModel *user)
{
    if (m_user != user) {
        m_user = user;
        emit userChanged();
    }
}

QString ChatListModelItem::text() const
{
    return m_text;
}

void ChatListModelItem::setText(const QString &text)
{
    if (m_text != text) {
        m_text = text;
        emit textChanged();
    }
}

qint64 ChatListModelItem::time() const
{
    return m_time;
}

void ChatListModelItem::setTime(qint64 time)
{
    if (m_time != time) {
        m_time = time;
        emit timeChanged();
    }
}

int ChatListModelItem::unreadNum() const
{
    return m_unreadNum;
}

void ChatListModelItem::setUnreadNum(int unreadNum)
{
    if (m_unreadNum != unreadNum) {
        m_unreadNum = unreadNum;
        emit unreadNumChanged();
    }
}



ChatListModel::ChatListModel(QObject *parent) : QObject(parent)
{
}

QList<ChatListModelItem*> ChatListModel::items() const
{
    return m_items;
}

ChatListModelItem* ChatListModel::currentItem() const
{
    return m_currentItem;
}

void ChatListModel::setCurrentItem(ChatListModelItem* currentItem)
{
    if (m_currentItem != currentItem) {
        m_currentItem = currentItem;
        emit currentItemChanged();
    }
}

void ChatListModel::append(ChatListModelItem* item)
{
    m_items.append(item);
    emit itemsChanged();
}

void ChatListModel::top(ChatListModelItem* item)
{
    m_items.removeOne(item);
    m_items.insert(0,item);
    emit itemsChanged();
}


void ChatListModel::onClick(ChatListModelItem* item)
{
    item->setUnreadNum(0);
    setCurrentItem(item);
}
