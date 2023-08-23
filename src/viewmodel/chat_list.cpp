#include "chat_list.h"
#include <QDebug>

ChatListModelItem::ChatListModelItem(QObject *parent) : QObject(parent),
    m_id(0),
    m_unreadNum(0)
{
}

ChatListModelItem::ChatListModelItem(int id, const QString &title, const QString &text, int unreadNum, const QColor &color, const QString &avatar, QObject *parent)
    : QObject(parent), m_id(id), m_title(title), m_text(text), m_unreadNum(unreadNum), m_color(color), m_avatar(avatar)
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

QString ChatListModelItem::title() const
{
    return m_title;
}

void ChatListModelItem::setTitle(const QString &title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged();
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

QColor ChatListModelItem::color() const
{
    return m_color;
}

void ChatListModelItem::setColor(const QColor &color)
{
    if (m_color != color) {
        m_color = color;
        emit colorChanged();
    }
}

QString ChatListModelItem::avatar() const
{
    return m_avatar;
}

void ChatListModelItem::setAvatar(const QString &avatar)
{
    if (m_avatar != avatar) {
        m_avatar = avatar;
        emit avatarChanged();
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
