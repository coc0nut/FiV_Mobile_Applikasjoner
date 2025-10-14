#include "maincontent.h"


#include <QWidget>
#include <QDebug>

MainContent::MainContent(NetworkManager *net, Database *db, User *user, Todo *todo, SideMenu *sideMenu, QWidget *parent)
    : QStackedWidget{parent}, db(db), user(user), todo(todo), sideMenu(sideMenu), net(net)
{
    m_homePage = new HomePage(net, db, user, this);
    m_profilePage = new ProfilePage(net, db, user, this);
    m_settingsPage = new QWidget(this);
    m_todoPage = new TodoPage(net, db, user, todo, this);



    addWidget(m_homePage);
    addWidget(m_profilePage);
    addWidget(m_settingsPage);
    addWidget(m_todoPage);

    // forward signal todo change signal maincontent -> mainwindow
    connect(m_todoPage, SIGNAL(todosChanged()), this, SIGNAL(todosChanged()));
    connect(m_todoPage, &TodoPage::navigateToHomePage, this, &MainContent::showHomePage);
    connect(m_profilePage, &ProfilePage::userDetailsChanged, m_homePage, &HomePage::refreshUserDetails);

    connect(net, &NetworkManager::todoCreated, m_homePage, &HomePage::refreshTodos);
    connect(net, &NetworkManager::todoUpdated, m_homePage, &HomePage::refreshTodos);
    connect(net, &NetworkManager::todoDeleted, m_homePage, &HomePage::refreshTodos);
    connect(net, &NetworkManager::todosFetched, m_homePage, &HomePage::refreshTodos);

    connect(net, &NetworkManager::todoDeleted, [](int todoId) {
        qDebug() << "MainContent: Received todoDeleted signal for ID:" << todoId;
    });
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
    emit todosChanged();
}
