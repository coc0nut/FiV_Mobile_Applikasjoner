#ifndef USER_H
#define USER_H

#include <QObject>
#include <QVector>

class User : public QObject
{
    Q_OBJECT
public:

    explicit User(QObject *parent = nullptr);

    QString username() const;
    void setUsername(const QString &newUsername);

    QString password() const;
    void setPassword(QString const &newPassword);

    int id() const;
    void setId(int newId);

    static QVector<User*> users;
    static int user_count;

    QString name() const;
    void setName(const QString &newName);

    QString email() const;
    void setEmail(const QString &newEmail);

signals:

    void usernameChanged();
    void passwordChanged();

private:
    int m_id;
    QString m_username, m_password, m_name, m_email;
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged FINAL)
    Q_PROPERTY(QString password READ password CONSTANT FINAL)
};

#endif // USER_H
