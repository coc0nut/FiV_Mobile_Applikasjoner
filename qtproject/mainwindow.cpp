#include "mainwindow.h"
#include "todo.h"
#include "sidemenu.h"
#include "maincontent.h"
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Top menu bar
        QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

        QAction *exitAction = new QAction(tr("&Exit"), this);
        fileMenu->addAction(exitAction);
        connect(exitAction, &QAction::triggered, this, &QWidget::close); 
    
    // Main content
        QWidget *central = new QWidget(this);
        QHBoxLayout *mainLayout = new QHBoxLayout(central);

        // Side menu (left)
            SideMenu *sideMenu = new SideMenu(central);

        // Main content area
            MainContent *mainContent = new MainContent(central);

    // Connections
        // Sidemenu to main content area
        connect(sideMenu, &QTreeWidget::currentItemChanged, this, [mainContent, sideMenu](QTreeWidgetItem *current){
            if (current == sideMenu->homeItem)
                mainContent->setCurrentIndex(0);
            else if (current == sideMenu->profileItem)
                mainContent->setCurrentIndex(1);
            else if (current == sideMenu->settingsItem || current->parent() == sideMenu->settingsItem)
                mainContent->setCurrentIndex(2);
            else if (current == sideMenu->todoItem)
                mainContent->setCurrentIndex(3);
        });

    // Preparing

        mainLayout->addWidget(sideMenu);
        mainLayout->addWidget(mainContent, 1);

        central->setLayout(mainLayout);
        setCentralWidget(central);

    // Status bar
        statusBar()->showMessage("Ready");

}

MainWindow::~MainWindow()
{
    delete ui;
}
