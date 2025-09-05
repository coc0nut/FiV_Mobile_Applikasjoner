#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "database.h"
#include "user.h"

class HomePage : public QWidget {
    Q_OBJECT

public:
    explicit HomePage(Database *db, User *user, QWidget *parent = nullptr);

public slots:
    void refreshTodos();

signals:


private:

    Database *db;
    User *user;
    QLabel *userDetails;
    QLabel *name;
    QLabel *email;
    QLabel *bilde;

    QLabel *title;
    QLabel *text;
    QLabel *created_on;
    QLabel *updated_on;
    QLabel *due;

    QPushButton *editButton;

    QWidget *contentWidget;
    QVBoxLayout *homePageLayout;
    QVBoxLayout *todoLayout;

};


#endif // HOMEPAGE_H
