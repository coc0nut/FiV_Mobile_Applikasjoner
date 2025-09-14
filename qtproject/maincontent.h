#ifndef MAINCONTENT_H
#define MAINCONTENT_H

#include <QStackedWidget>
#include "database.h"
#include "user.h"
#include "todo.h"
#include "sidemenu.h"
#include "todopage.h"
#include "homepage.h"
#include "profilepage.h"

class MainContent : public QStackedWidget
{
    Q_OBJECT
public:
    explicit MainContent(Database *db, User *user, Todo *todo, SideMenu *sideMenu, QWidget *parent = nullptr);

    QWidget *homePage(), *profilePage(), *settingsPage(), *todoPage();

public slots:
    void showTodoPage(int todoId);
    void showHomePage();

signals:
    void todosChanged();

private:
    HomePage *m_homePage;
    ProfilePage *m_profilePage;
    QWidget *m_settingsPage;
    TodoPage *m_todoPage;
    Database *db;
    User *user;
    Todo *todo;
    SideMenu *sideMenu;

signals:
};

#endif // MAINCONTENT_H
