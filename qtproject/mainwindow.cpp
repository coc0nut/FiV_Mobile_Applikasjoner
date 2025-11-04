#include "mainwindow.h"

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

MainWindow::MainWindow(NetworkManager *net, Database *db, User *user, Todo *todo, QWidget *parent)
    : QMainWindow(parent), net(net), db(db), user(user), todo(todo), ui(new Ui::MainWindow) {

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
            ).arg(bgColorDark, textColorDark)

        );

        menuBar()->setStyleSheet(
            QString(
                "background: %1;"
                "color: %2;"
            ).arg(bgColorDark, textColorDark)
        );

    // Main content
        QWidget *central = new QWidget(this);


        central->setStyleSheet(
            QString(
                "background: %1;"
                "color: %2;"
            ).arg(bgColor, textColor)
        );

        QHBoxLayout *mainLayout = new QHBoxLayout(central);
        mainLayout->setSpacing(10);

            QVBoxLayout *sideLayout = new QVBoxLayout();

            // Logo
                QLabel *logo = new QLabel("Note.io", this);
                logo->setAlignment(Qt::AlignCenter);
                logo->setStyleSheet(QString(
                        "background: %1;"
                        "color: %2;"
                        "font-size: 46px;"
                        "font-weight: bold;"
                    ).arg(bgColor, textColor)
                );

                sideLayout->addWidget(logo);


            // Side menu (left)
                sideMenu = new SideMenu(central);
                sideMenu->populateTodos(user->id());
                sideMenu->setStyleSheet(
                    QString(
                        "background: %1;"
                        "color: %2;"
                        "border-radius: 8px;"
                        "padding: 4px;"
                    ).arg(bgColorDark, textColorDark)
                );
                sideLayout->addWidget(sideMenu);

        // Line to separate sidemenu and maincontent visually
        QFrame *vLine = new QFrame(this);
        vLine->setFrameShape(QFrame::VLine);
        vLine->setFrameShadow(QFrame::Sunken);
        vLine->setLineWidth(2);




        // Main content area
            mainContent = new MainContent(net, db, user, todo, sideMenu, central);
            mainContent->setStyleSheet(
                QString(
                    "background: %1;"
                    "color: %2;"
                    "padding: 5;"
                ).arg(bgColor, textColor)
            );


    // Connections
        // Sidemenu to main content area
        connect(sideMenu, &QTreeWidget::currentItemChanged, this, [this](QTreeWidgetItem *current) {
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


        mainLayout->addLayout(sideLayout);
        mainLayout->addWidget(vLine);
        mainLayout->addWidget(mainContent, 1);

        setCentralWidget(central);

    // Status bar
        statusBar()->showMessage("Ready");
        statusBar()->setStyleSheet(
            QString(
                "background: %1;"
                "color: %2;"
                ).arg(bgColorDark, textColorDark)
        );

}

MainWindow::~MainWindow()
{

    delete ui;

}
