#ifndef MODELUSER_H
#define MODELUSER_H

#include <QObject>
#include <QString>

class UserModel : public QObject {
Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString nickname READ nickname WRITE setNickname NOTIFY nicknameChanged)
    Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QString avatar READ avatar WRITE setAvatar NOTIFY avatarChanged)
    Q_PROPERTY(QString remark READ remark WRITE setRemark NOTIFY remarkChanged)
    Q_PROPERTY(bool online READ online WRITE setOnline NOTIFY onlineChanged)

public:
    explicit UserModel(QObject *parent = nullptr);

    UserModel(int id, const QString &username, const QString &nickname, const QString &color, const QString &avatar,
              const QString &remark, bool online, QObject *parent = nullptr);

    int id() const;

    void setId(int id);

    QString username() const;

    void setUsername(const QString &username);

    QString nickname() const;

    void setNickname(const QString &nickname);

    QString color() const;

    void setColor(const QString &color);

    QString avatar() const;

    void setAvatar(const QString &avatar);

    QString remark() const;

    void setRemark(const QString &remark);

    bool online() const;

    void setOnline(bool online);

signals:

    void idChanged();

    void usernameChanged();

    void nicknameChanged();

    void colorChanged();

    void avatarChanged();

    void remarkChanged();

    void onlineChanged();

private:
    int m_id;
    QString m_username;
    QString m_nickname;
    QString m_color;
    QString m_avatar;
    QString m_remark;
    bool m_online;
};

#endif // MODELUSER_H
