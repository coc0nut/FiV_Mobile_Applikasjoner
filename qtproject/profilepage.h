#ifndef PROFILEPAGE_H
#define PROFILEPAGE_H

#include <QWidget>
#include "database.h"
#include "user.h"

class ProfilePage : public QWidget
{
    Q_OBJECT
public:
    explicit ProfilePage(Database *db, User *user, QWidget *parent = nullptr);

signals:

private:
    Database *db;
    User *user;
};

#endif // PROFILEPAGE_H
