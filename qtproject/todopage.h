#ifndef TODOPAGE_H
#define TODOPAGE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include "database.h"
#include "todo.h"
#include "user.h"

class QTextEdit;
class QPushButton;

class TodoPage : public QWidget
{
    Q_OBJECT
public:
    TodoPage(Database *db, User *user, Todo *todo, QWidget *parent = nullptr);

    void setTodo(Todo *todo);

private:
    Database *db;
    User *user;
    Todo *todo;
    QLineEdit *todoTitleEdit;
    QLabel *created_on;
    QLabel *updated_on;
    QLabel *due;
    QLabel *completed_on;
    QTextEdit *todoTextEdit;
    QDateTimeEdit *dueEdit;
    QPushButton *addButton, *updateButton, *deleteButton, *completeButton;
};

#endif // TODOPAGE_H
