#ifndef MODELGROUP_H
#define MODELGROUP_H

#include "model/user.h"
#include "message.h"
#include <QObject>
#include <QList>

class GroupModel : public QObject {
Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString avatar READ avatar WRITE setAvatar NOTIFY avatarChanged)
    Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QString remark READ remark WRITE setRemark NOTIFY remarkChanged)
    Q_PROPERTY(UserModel *owner READ owner WRITE setOwner NOTIFY ownerChanged) //当twin时为对方
    Q_PROPERTY(MessageModel *last READ last WRITE setLast NOTIFY lastChanged)

    Q_PROPERTY(int read READ read WRITE setRead NOTIFY readChanged) //已读的mid
    Q_PROPERTY(int unreadNum READ unreadNum NOTIFY unreadNumChanged) //未读消息数

public:
    explicit GroupModel(QObject *parent = nullptr);

    GroupModel(int id, const QString &type, const QString &name, const QString &avatar, const QString &color,
               const QString &remark, UserModel *owner, MessageModel *last, int read, QObject *parent = nullptr);

    int id() const;

    void setId(int id);

    QString type() const;

    void setType(const QString &type);

    QString name() const;

    void setName(const QString &name);

    QString avatar() const;

    void setAvatar(const QString &avatar);

    QString color() const;

    void setColor(const QString &color);

    QString remark() const;

    void setRemark(const QString &remark);

    UserModel *owner() const;

    void setOwner(UserModel *owner);

    MessageModel *last() const;

    void setLast(MessageModel *last);

    int read() const;

    void setRead(int read);

    int unreadNum() const;


signals:

    void idChanged();

    void typeChanged();

    void nameChanged();

    void avatarChanged();

    void colorChanged();

    void remarkChanged();

    void ownerChanged();

    void lastChanged();

    void readChanged();

    void unreadNumChanged();


private:
    int m_id;
    QString m_type;
    QString m_name;
    QString m_avatar;
    QString m_color;
    QString m_remark;
    UserModel *m_owner;
    MessageModel *m_last;
    int m_read;

    MessageModel* m_waitForDeleteLast;
};


class GroupListModel : public QObject {
Q_OBJECT

    Q_PROPERTY(QList<GroupModel *> items READ items WRITE setItems NOTIFY itemsChanged FINAL)
    Q_PROPERTY(GroupModel *currentItem READ currentItem WRITE setCurrentItem NOTIFY currentItemChanged FINAL)

public:
    explicit GroupListModel(QObject *parent = nullptr);

    QList<GroupModel *> items() const;

    void setItems(QList<GroupModel *> items);

    void sortItems();

    GroupModel *currentItem() const;

    void setCurrentItem(GroupModel *item);

signals:

    void itemsChanged();

    void currentItemChanged();

private:
    QList<GroupModel *> m_items;
    GroupModel *m_currentItem;

    QList<GroupModel *> m_waitForDeleteItems;
};

#endif // MODELGROUP_H


