#include "mainwindow.h"
#include "./ui_mainwindow.h"





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet(
        QString("background: %1; color: %2;").arg(
            styles->getBgColorDark(),
            styles->getTextColorDark()
        )
    );


    // Top menu

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *exitAction = new QAction(tr("&Exit"));
    fileMenu->addAction(exitAction);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // Main Content
    central = new QWidget();
    setCentralWidget(central);

    mainLayout = new QHBoxLayout();
    mainLayout->setSpacing(10);

        sideLayout = new QVBoxLayout();
            // Logo
            logo = new QLabel("Note.io");

        sideLayout->addWidget(logo);

        sideMenu = new SideMenu();
        sideLayout->addWidget(sideMenu);

    mainLayout->addLayout(sideLayout);

    // Status bar
    statusBar()->showMessage("Ready");
}

MainWindow::~MainWindow()
{
    delete ui;
}
