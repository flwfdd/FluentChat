#ifndef CHATLIST_H
#define CHATLIST_H

#include "model/user.h"
#include <QObject>
#include <QColor>
#include <QList>

class ChatListModelItem: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(UserModel* user READ user WRITE setUser NOTIFY userChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(qint64 time READ time WRITE setTime NOTIFY timeChanged)
    Q_PROPERTY(int unreadNum READ unreadNum WRITE setUnreadNum NOTIFY unreadNumChanged)

public:
    explicit ChatListModelItem(QObject *parent = nullptr);
    ChatListModelItem(int id, UserModel *user, const QString &text, qint64 time, int unreadNum, QObject *parent = nullptr);

    int id() const;
    void setId(int id);

    UserModel* user() const;
    void setUser(UserModel *user);

    QString text() const;
    void setText(const QString &text);

    qint64 time() const;
    void setTime(qint64 time);

    int unreadNum() const;
    void setUnreadNum(int unreadNum);


signals:
    void idChanged();
    void userChanged();
    void textChanged();
    void timeChanged();
    void unreadNumChanged();


private:
    int m_id;
    UserModel* m_user;
    QString m_text;
    qint64 m_time;
    int m_unreadNum;
};

class ChatListModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<ChatListModelItem*> items READ items NOTIFY itemsChanged FINAL)
    Q_PROPERTY(ChatListModelItem* currentItem READ currentItem WRITE setCurrentItem NOTIFY currentItemChanged FINAL)

public:
    explicit ChatListModel(QObject *parent = nullptr);
    QList<ChatListModelItem*> items() const;
    ChatListModelItem* currentItem() const;
    void setCurrentItem(ChatListModelItem* item);
    void append(ChatListModelItem* item);
    Q_INVOKABLE void top(ChatListModelItem* item);
    Q_INVOKABLE void onClick(ChatListModelItem* item);

signals:
    void itemsChanged();
    void currentItemChanged();

private:
    QList<ChatListModelItem*> m_items;
    ChatListModelItem* m_currentItem;
};

#endif // CHATLIST_H


