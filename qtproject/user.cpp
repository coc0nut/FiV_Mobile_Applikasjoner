#include "user.h"

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

void User::resetUsername()
{
    setUsername({}); // TODO: Adapt to use your actual default value
}
