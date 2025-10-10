#ifndef USER_H
#define USER_H

#include <QObject>
#include <QVector>
#include <QUrl>

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

    QString name() const;
    void setName(const QString &newName);

    QString email() const;
    void setEmail(const QString &newEmail);

    QUrl url() const;
    void setUrl(const QUrl &newUrl);

    QString first_name() const;
    void setFirst_name(const QString &newFirst_name);

    QString last_name() const;
    void setLast_name(const QString &newLast_name);

    QString last_login() const;
    void setLast_login(const QString &newLast_login);

    QString date_joined() const;
    void setDate_joined(const QString &newDate_joined);

    bool getIs_superuser() const;
    void setIs_superuser(bool newIs_superuser);

    bool getIs_staff() const;
    void setIs_staff(bool newIs_staff);

    bool getIs_active() const;
    void setIs_active(bool newIs_active);

    static QVector<User*> users;
    static int user_count;

signals:

    void usernameChanged();
    void passwordChanged();

private:
    int m_id;
    QUrl m_url;
    QString m_name, m_username, m_password, m_email, m_first_name, m_last_name, m_last_login, m_date_joined;
    bool is_superuser, is_staff, is_active;

};

#endif // USER_H
