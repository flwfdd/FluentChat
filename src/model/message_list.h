#ifndef MESSAGELISTMODEL_H
#define MESSAGELISTMODEL_H

#include "model/user.h"
#include <QObject>
#include <QColor>
#include <QList>

class MessageListModelItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString content READ content WRITE setContent NOTIFY contentChanged)
    Q_PROPERTY(qint64 time READ time WRITE setTime NOTIFY timeChanged)
    Q_PROPERTY(UserModel* user READ user WRITE setUser NOTIFY userChanged)
    Q_PROPERTY(int gid READ gid WRITE setGid NOTIFY gidChanged)
    Q_PROPERTY(int sn READ sn WRITE setSn NOTIFY snChanged)
    Q_PROPERTY(bool recall READ recall WRITE setRecall NOTIFY recallChanged)

public:
    explicit MessageListModelItem(QObject *parent = nullptr);
    MessageListModelItem(int id, const QString &type, const QString &content, qint64 time, UserModel* user, int gid, int sn, bool recall, QObject *parent = nullptr);

    int id() const;
    void setId(int id);

    QString type() const;
    void setType(const QString &type);

    QString content() const;
    void setContent(const QString &content);

    int time() const;
    void setTime(qint64 time);

    UserModel* user() const;
    void setUser(UserModel *user);

    int gid() const;
    void setGid(int gid);

    int sn() const;
    void setSn(int sn);

    bool recall() const;
    void setRecall(bool recall);

signals:
    void idChanged();
    void typeChanged();
    void contentChanged();
    void timeChanged();
    void userChanged();
    void gidChanged();
    void snChanged();
    void recallChanged();

private:
    int m_id;
    QString m_type;
    QString m_content;
    qint64 m_time;
    UserModel* m_user;
    int m_gid;
    int m_sn;
    bool m_recall;
};

class MessageListModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<MessageListModelItem*> items READ items WRITE setItems NOTIFY itemsChanged)
public:
    explicit MessageListModel(QObject *parent = nullptr);
    QList<MessageListModelItem*> items() const;
    void setItems(const QList<MessageListModelItem*> &items);

    void append(MessageListModelItem* item);
    void prepend(MessageListModelItem* item);

    Q_INVOKABLE void send(const QString &content);

signals:
    void itemsChanged();

private:
    QList<MessageListModelItem*> m_items;

};

#endif // MESSAGELISTMODEL_H
