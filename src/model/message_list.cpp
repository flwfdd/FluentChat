#include "message_list.h"
#include "global/store.h"
#include <QDateTime>
#include <QDebug>

MessageListModelItem::MessageListModelItem(QObject *parent)
    : QObject(parent)
{
}

MessageListModelItem::MessageListModelItem(int id, const QString &type, const QString &content, qint64 time, UserModel* user, int gid, int sn, bool recall, QObject *parent)
    : QObject(parent), m_id(id), m_type(type), m_content(content), m_time(time), m_user(user), m_gid(gid), m_sn(sn), m_recall(recall)
{
}

int MessageListModelItem::id() const
{
    return m_id;
}

void MessageListModelItem::setId(int id)
{
    if (m_id != id) {
        m_id = id;
        emit idChanged();
    }
}

QString MessageListModelItem::type() const
{
    return m_type;
}

void MessageListModelItem::setType(const QString &type)
{
    if (m_type != type) {
        m_type = type;
        emit typeChanged();
    }
}

QString MessageListModelItem::content() const
{
    return m_content;
}

void MessageListModelItem::setContent(const QString &content)
{
    if (m_content != content) {
        m_content = content;
        emit contentChanged();
    }
}

int MessageListModelItem::time() const
{
    return m_time;
}

void MessageListModelItem::setTime(qint64 time)
{
    if (m_time != time) {
        m_time = time;
        emit timeChanged();
    }
}

UserModel* MessageListModelItem::user() const
{
    return m_user;
}

void MessageListModelItem::setUser(UserModel *user)
{
    if (m_user != user) {
        m_user = user;
        emit userChanged();
    }
}

int MessageListModelItem::gid() const
{
    return m_gid;
}

void MessageListModelItem::setGid(int gid)
{
    if (m_gid != gid) {
        m_gid = gid;
        emit gidChanged();
    }
}

int MessageListModelItem::sn() const
{
    return m_sn;
}

void MessageListModelItem::setSn(int sn)
{
    if (m_sn != sn) {
        m_sn = sn;
        emit snChanged();
    }
}

bool MessageListModelItem::recall() const
{
    return m_recall;
}

void MessageListModelItem::setRecall(bool recall)
{
    if (m_recall != recall) {
        m_recall = recall;
        emit recallChanged();
    }
}



MessageListModel::MessageListModel(QObject *parent)
    : QObject{parent}
{
}

QList<MessageListModelItem*> MessageListModel::items() const
{
    return m_items;
}

void MessageListModel::setItems(const QList<MessageListModelItem*> &items)
{
    if (m_items != items) {
        m_items = items;
        emit itemsChanged();
    }
}

void MessageListModel::append(MessageListModelItem* item)
{
    m_items.append(item);
    emit itemsChanged();
}

void MessageListModel::prepend(MessageListModelItem* item)
{
    m_items.prepend(item);
    emit itemsChanged();
}

void MessageListModel::send(const QString &content)
{
    if (content.isEmpty())
        return;
    auto item = new MessageListModelItem{int(items().size()), "text", content, QDateTime::currentMSecsSinceEpoch()/1000, (*(Store::instance()->users()))[1], 0, 0, false};
    append(item);
}



