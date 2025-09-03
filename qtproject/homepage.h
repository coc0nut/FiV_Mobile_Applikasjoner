#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QLabel>

#include "database.h"
#include "user.h"

class HomePage : public QWidget {
    Q_OBJECT

public:
    explicit HomePage(Database *db, User *user, QWidget *parent = nullptr);

signals:


private:

    Database *db;
    User *user;
    QLabel *username;
    QLabel *password;
    QLabel *id;
    QLabel *bilde;

    QLabel *title;
    QLabel *text;
    QLabel *created_on;
    QLabel *updated_on;
    QLabel *due;

};


#endif // HOMEPAGE_H
