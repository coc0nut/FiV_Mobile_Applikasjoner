#include "maincontent.h"
#include "todopage.h"
#include "homepage.h"
#include "profilepage.h"

#include <QWidget>

MainContent::MainContent(Database *db, User *user, Todo *todo, QWidget *parent)
    : QStackedWidget{parent}, db(db), user(user), todo(todo)
{
    m_homePage = new HomePage(db, user);
    m_profilePage = new ProfilePage(db, user);
    m_settingsPage = new QWidget();
    m_todoPage = new TodoPage(db, user, todo);

    addWidget(m_homePage);
    addWidget(m_profilePage);
    addWidget(m_settingsPage);
    addWidget(m_todoPage);

    // forward signal todo change signal maincontent -> mainwindow
    connect(m_todoPage, SIGNAL(todosChanged()), this, SIGNAL(todosChanged()));
}

QWidget* MainContent::homePage() { return m_homePage; }
QWidget* MainContent::profilePage() { return m_profilePage; }
QWidget* MainContent::settingsPage() { return m_settingsPage; }
QWidget* MainContent::todoPage() { return m_todoPage; }
