#include "user.h"

QVector<User*> User::users;
int User::id_count = 0;

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
