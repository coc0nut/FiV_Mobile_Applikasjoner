#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "database.h"
#include "user.h"
#include "todo.h"


QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Database *db, User *user, Todo *todo, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    Database *db;
    User *user;
    Todo *todo;

    Todo *newTodo = nullptr;

};

#endif // MAINWINDOW_H
