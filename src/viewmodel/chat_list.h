#ifndef CHATLIST_H
#define CHATLIST_H

#include <QObject>
#include <QColor>
#include <QList>

class ChatListModelItem: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(int unreadNum READ unreadNum WRITE setUnreadNum NOTIFY unreadNumChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QString avatar READ avatar WRITE setAvatar NOTIFY avatarChanged)

public:
    explicit ChatListModelItem(QObject *parent = nullptr);
    ChatListModelItem(int id, const QString &title, const QString &text, int unreadNum, const QColor &color, const QString &avatar, QObject *parent = nullptr);

    int id() const;
    void setId(int id);

    QString title() const;
    void setTitle(const QString &title);

    QString text() const;
    void setText(const QString &text);

    int unreadNum() const;
    void setUnreadNum(int unreadNum);

    QColor color() const;
    void setColor(const QColor &color);

    QString avatar() const;
    void setAvatar(const QString &avatar);

signals:
    void idChanged();
    void titleChanged();
    void textChanged();
    void unreadNumChanged();
    void colorChanged();
    void avatarChanged();

private:
    int m_id;
    QString m_title;
    QString m_text;
    int m_unreadNum;
    QColor m_color;
    QString m_avatar;
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


