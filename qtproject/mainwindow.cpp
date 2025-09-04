#include "mainwindow.h"
#include "sidemenu.h"
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

    QString bgColor = "#394359";
    QString textColor = "#f95959";

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
            SideMenu *sideMenu = new SideMenu(central);
            sideMenu->populateTodos(user->id());

        // Main content area
            MainContent *mainContent = new MainContent(db, user, todo, central);

    // Connections
        // Sidemenu to main content area
        connect(sideMenu, &QTreeWidget::currentItemChanged, this, [this, sideMenu, mainContent](QTreeWidgetItem *current) {
            if (current == sideMenu->homeItem)
                mainContent->setCurrentIndex(0);
            else if (current == sideMenu->profileItem)
                mainContent->setCurrentIndex(1);
            else if (current == sideMenu->settingsItem || current->parent() == sideMenu->settingsItem)
                mainContent->setCurrentIndex(2);
            else if (current == sideMenu->todoItem) {
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
            else if (current && current->parent() == sideMenu->todoItem) {
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
