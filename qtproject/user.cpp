#include "user.h"

QVector<User*> User::users;
int User::user_count = 0;

User::User(QObject *parent)
    : QObject{parent}
{}

QString User::username() const
{
    return m_username;
}

void User::setUsername(const QString &newUsername)
{
    if (m_username == newUsername)
        return;
    m_username = newUsername;
    emit usernameChanged();
}

QString User::password() const
{
    return m_password;
}

void User::setPassword(QString const &newPassword) {
    if (m_password == newPassword) {
        return;
    }
    m_password = newPassword;
    emit passwordChanged();
}

int User::id() const
{
    return m_id;
}

void User::setId(int newId)
{
    m_id = newId;
}

QString User::name() const
{
    return m_name;
}

void User::setName(const QString &newName)
{
    m_name = newName;
}

QString User::email() const
{
    return m_email;
}

void User::setEmail(const QString &newEmail)
{
    m_email = newEmail;
}

QUrl User::url() const
{
    return m_url;
}

void User::setUrl(const QUrl &newUrl)
{
    m_url = newUrl;
}

QString User::first_name() const
{
    return m_first_name;
}

void User::setFirst_name(const QString &newFirst_name)
{
    m_first_name = newFirst_name;
}

QString User::last_name() const
{
    return m_last_name;
}

void User::setLast_name(const QString &newLast_name)
{
    m_last_name = newLast_name;
}

QString User::last_login() const
{
    return m_last_login;
}

void User::setLast_login(const QString &newLast_login)
{
    m_last_login = newLast_login;
}

QString User::date_joined() const
{
    return m_date_joined;
}

void User::setDate_joined(const QString &newDate_joined)
{
    m_date_joined = newDate_joined;
}

bool User::getIs_superuser() const
{
    return is_superuser;
}

void User::setIs_superuser(bool newIs_superuser)
{
    is_superuser = newIs_superuser;
}

bool User::getIs_staff() const
{
    return is_staff;
}

void User::setIs_staff(bool newIs_staff)
{
    is_staff = newIs_staff;
}

bool User::getIs_active() const
{
    return is_active;
}

void User::setIs_active(bool newIs_active)
{
    is_active = newIs_active;
}
