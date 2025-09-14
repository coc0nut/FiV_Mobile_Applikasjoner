#include "maincontent.h"


#include <QWidget>

MainContent::MainContent(Database *db, User *user, Todo *todo, SideMenu *sideMenu, QWidget *parent)
    : QStackedWidget{parent}, db(db), user(user), todo(todo), sideMenu(sideMenu)
{
    m_homePage = new HomePage(db, user, this);
    m_profilePage = new ProfilePage(db, user, this);
    m_settingsPage = new QWidget(this);
    m_todoPage = new TodoPage(db, user, todo, this);



    addWidget(m_homePage);
    addWidget(m_profilePage);
    addWidget(m_settingsPage);
    addWidget(m_todoPage);

    // forward signal todo change signal maincontent -> mainwindow
    connect(m_todoPage, SIGNAL(todosChanged()), this, SIGNAL(todosChanged()));
    connect(m_todoPage, SIGNAL(navigateToHomePage()), this, SLOT(showHomePage()));

    connect(m_profilePage, &ProfilePage::userDetailsChanged, m_homePage, &HomePage::refreshUserDetails);
}

QWidget* MainContent::homePage() { return m_homePage; }
QWidget* MainContent::profilePage() { return m_profilePage; }
QWidget* MainContent::settingsPage() { return m_settingsPage; }
QWidget* MainContent::todoPage() { return m_todoPage; }

void MainContent::showTodoPage(int todoId)
{
    Todo* todoToEdit = nullptr;

    for (Todo* t: Todo::todos) {
        if (t->id() == todoId) {
            todoToEdit = t;
            sideMenu->setCurrentItemByName(t->title());
            break;
        }
    }

    if (todoToEdit) {
        static_cast<TodoPage*>(m_todoPage)->setTodo(todoToEdit);
    }

    setCurrentWidget(m_todoPage);
}

void MainContent::showHomePage() {
    setCurrentWidget(m_homePage);
    if (sideMenu) {
        sideMenu->setCurrentItemByName("Home");
    }
}
