#ifndef MAINCONTENT_H
#define MAINCONTENT_H

#include <QStackedWidget>
#include "database.h"
#include "user.h"
#include "todo.h"

class MainContent : public QStackedWidget
{
    Q_OBJECT
public:
    explicit MainContent(Database *db, User *user, Todo *todo, QWidget *parent = nullptr);

    QWidget *homePage(), *profilePage(), *settingsPage(), *todoPage();

private:
    QWidget *m_homePage, *m_profilePage, *m_settingsPage, *m_todoPage;
    Database *db;
    User *user;
    Todo *todo;

signals:
};

#endif // MAINCONTENT_H
