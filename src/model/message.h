#ifndef MODELMESSAGE_H
#define MODELMESSAGE_H

#include "model/user.h"
#include <QObject>
#include <QList>
#include <QSet>

class MessageModel : public QObject {
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)

    Q_PROPERTY(QString
    type READ
    type WRITE
    setType NOTIFY
    typeChanged)
    Q_PROPERTY(QString
    content READ
    content WRITE
    setContent NOTIFY
    contentChanged)
    Q_PROPERTY(qint64
    time READ
    time WRITE
    setTime NOTIFY
    timeChanged)
    Q_PROPERTY(UserModel
    *
    user READ
    user WRITE
    setUser NOTIFY
    userChanged)

    Q_PROPERTY(int gid READ gid WRITE setGid NOTIFY gidChanged)

    Q_PROPERTY(int mid READ mid WRITE setMid NOTIFY midChanged)

    Q_PROPERTY(bool recall READ recall WRITE setRecall NOTIFY recallChanged)

public:
    explicit MessageModel(QObject *parent = nullptr);

    MessageModel(int id, const QString &type, const QString &content, qint64 time, UserModel *user, int gid, int mid,
                 bool recall, QObject *parent = nullptr);

    int id() const;

    void setId(int id);

    QString type() const;

    void setType(const QString &type);

    QString content() const;

    void setContent(const QString &content);

    int time() const;

    void setTime(qint64 time);

    UserModel *user() const;

    void setUser(UserModel *user);

    int gid() const;

    void setGid(int gid);

    int mid() const;

    void setMid(int sn);

    bool recall() const;

    void setRecall(bool recall);

    signals:

            void idChanged();

    void typeChanged();

    void contentChanged();

    void timeChanged();

    void userChanged();

    void gidChanged();

    void midChanged();

    void recallChanged();

private:
    int m_id;
    QString m_type;
    QString m_content;
    qint64 m_time;
    UserModel *m_user;
    int m_gid;
    int m_mid;
    bool m_recall;
};

class MessageListModel : public QObject {
    Q_OBJECT

    Q_PROPERTY(QList<MessageModel *> items READ items WRITE setItems NOTIFY itemsChanged)

    Q_PROPERTY(bool hasMore READ hasMore WRITE setHasMore NOTIFY hasMoreChanged)

public:
    explicit MessageListModel(QObject *parent = nullptr);

    QList<MessageModel *> items() const;

    void setItems(const QList<MessageModel *> &items);

    bool hasMore() const;

    void setHasMore(bool hasMore);

signals:

    void itemsChanged();

    void hasMoreChanged();

private:
    QList<MessageModel *> m_items;

    QSet<MessageModel *> m_waitForDeleteItems;

    bool m_hasMore;

};

#endif // MODELMESSAGE_H
