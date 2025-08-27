#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *exitAction = new QAction(tr("&Exit"), this);
    fileMenu->addAction(exitAction);
    connect(exitAction, &QAction::triggered, this, &QWidget::close); 
    
    QWidget *central = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(central);

    QPushButton *button1 = new QPushButton("Button 1", central);
    QPushButton *button2 = new QPushButton("Button 2", central);
    QPushButton *button3 = new QPushButton("Button 3", central);

    layout->addWidget(button1);
    layout->addWidget(button2);
    layout->addWidget(button3);

    central->setLayout(layout);
    setCentralWidget(central);

}

MainWindow::~MainWindow()
{
    delete ui;
}
