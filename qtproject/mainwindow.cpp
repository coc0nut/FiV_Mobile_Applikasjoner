#include "mainwindow.h"

#include "maincontent.h"
#include "todopage.h"
#include "homepage.h"
#include "./ui_mainwindow.h"

#include <QWidget>
#include <QStackedWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QMenu>
#include <QAction>
#include <QPalette>

MainWindow::MainWindow(Database *db, User *user, Todo *todo, QWidget *parent)
    : QMainWindow(parent), db(db), user(user), todo(todo), ui(new Ui::MainWindow) {

    ui->setupUi(this);
    newTodo = new Todo(this);


    // Top menu bar
        QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

        QAction *exitAction = new QAction(tr("&Exit"), this);
        fileMenu->addAction(exitAction);
        connect(exitAction, &QAction::triggered, this, &QWidget::close);

        fileMenu->setStyleSheet(
            QString(
                "background: %1;"
                "color: %2;"
                ).arg(bgColor).arg(textColor)

        );

        menuBar()->setStyleSheet(
            QString(
                "background: %1;"
                "color: %2;"
                ).arg(bgColor).arg(textColor)
        );

    // Main content
        QWidget *central = new QWidget(this);


        central->setStyleSheet(
            QString(
                "background: %1;"
                "color: %2;"
            ).arg(bgColor).arg(textColor)


        );

        QHBoxLayout *mainLayout = new QHBoxLayout(central);

        // Side menu (left)
            sideMenu = new SideMenu(central);
            sideMenu->populateTodos(user->id());

        // Main content area
            MainContent *mainContent = new MainContent(db, user, todo, sideMenu, central);

    // Connections
        // Sidemenu to main content area
        connect(sideMenu, &QTreeWidget::currentItemChanged, this, [this, mainContent](QTreeWidgetItem *current) {
            if (current == this->sideMenu->homeItem)
                mainContent->setCurrentIndex(0);
            // Profile page
            else if (current == this->sideMenu->profileItem) {
                mainContent->setCurrentIndex(1);


            }
            // Settings Page
            else if (current == this->sideMenu->settingsItem || current->parent() == this->sideMenu->settingsItem) {
                mainContent->setCurrentIndex(2);
            }
            // TodoPage
            else if (current == this->sideMenu->todoItem) {
                mainContent->setCurrentIndex(3);

                auto todoPageWidget = mainContent->todoPage();
                auto todoPage = qobject_cast<TodoPage*>(todoPageWidget);
                if (todoPage) {
                    
                    newTodo->setTitle("");
                    newTodo->setText("");
                    newTodo->setCompleted(0);
                    todoPage->setTodo(newTodo);
                }
            }
            else if (current && current->parent() == this->sideMenu->todoItem) {
                mainContent->setCurrentIndex(3);

                auto todoPageWidget = mainContent->todoPage();
                auto todoPage = qobject_cast<TodoPage*>(todoPageWidget);
                Todo *selectedTodo = static_cast<Todo*>(current->data(0, Qt::UserRole).value<void*>());
                if (todoPage && selectedTodo) {
                    todoPage->setTodo(selectedTodo);
                }
            }
        });

        connect(mainContent, &MainContent::todosChanged, sideMenu, &SideMenu::refreshTodos);

        
        auto todoPageWidget = mainContent->todoPage();
        auto todoPage = qobject_cast<TodoPage*>(todoPageWidget);

        auto homePageWidget = mainContent->homePage();
        auto homePage = qobject_cast<HomePage*>(homePageWidget);

        if (todoPage && homePage) {
            connect(todoPage, &TodoPage::todosChanged, homePage, &HomePage::refreshTodos);
        }

        if (homePage && mainContent) {
            connect(homePage, &HomePage::editTodoRequested, mainContent, &MainContent::showTodoPage);
        }





    // Preparing

        mainLayout->addWidget(sideMenu);
        mainLayout->addWidget(mainContent, 1);

        central->setLayout(mainLayout);
        setCentralWidget(central);

    // Status bar
        statusBar()->showMessage("Ready");
        statusBar()->setStyleSheet(
            QString(
                "background: %1;"
                "color: %2;"
                ).arg(bgColor).arg(textColor)
        );

}

MainWindow::~MainWindow()
{

    delete ui;

}
