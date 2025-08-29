#ifndef USER_H
#define USER_H

#include <QObject>

class User : public QObject
{
    Q_OBJECT
public:
    explicit User(QObject *parent = nullptr);

    QString username() const;
    void setUsername(const QString &newUsername);
    void resetUsername();

signals:

    void usernameChanged();

private:
    int m_id;
    QString m_username, m_password;
    Q_PROPERTY(QString username READ username WRITE setUsername RESET resetUsername NOTIFY usernameChanged FINAL)
};

#endif // USER_H
