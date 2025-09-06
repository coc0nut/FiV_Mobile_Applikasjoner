#ifndef MAINCONTENT_H
#define MAINCONTENT_H

#include <QStackedWidget>
#include "database.h"
#include "user.h"
#include "todo.h"
#include "sidemenu.h"

class MainContent : public QStackedWidget
{
    Q_OBJECT
public:
    explicit MainContent(Database *db, User *user, Todo *todo, SideMenu *sideMenu, QWidget *parent = nullptr);

    QWidget *homePage(), *profilePage(), *settingsPage(), *todoPage();

public slots:
    void showTodoPage(int todoId);

signals:
    void todosChanged();

private:
    QWidget *m_homePage, *m_profilePage, *m_settingsPage, *m_todoPage;
    Database *db;
    User *user;
    Todo *todo;
    SideMenu *sideMenu;

signals:
};

#endif // MAINCONTENT_H
