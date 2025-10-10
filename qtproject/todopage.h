#ifndef TODOPAGE_H
#define TODOPAGE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include "database.h"
#include "todo.h"
#include "user.h"
#include "networkmanager.h"

class QTextEdit;
class QPushButton;

class TodoPage : public QWidget
{
    Q_OBJECT
public:
    TodoPage(NetworkManager *net, Database *db, User *user, Todo *todo, QWidget *parent = nullptr);

    void setTodo(Todo *todo);



signals:
    void todosChanged();
    void navigateToHomePage();

private:
    NetworkManager *net;
    Database *db;
    User *user;
    Todo *todo;
    QLineEdit *todoTitleEdit;
    QLabel *created_on;
    QLabel *updated_on;
    QLabel *due;
    QLabel *completed_on;
    QLabel *header;
    QTextEdit *todoTextEdit;
    QDateTimeEdit *dueEdit;
    QPushButton *addButton, *updateButton, *deleteButton;
    QCheckBox *completedCheckBox;

    QString bgColor {"#a6a6a6"}, textColor {"#393e46"};
    QString bgColorDark {"#393e46"}, textColorDark {"#f0ece2"};
    QString btnBgColor {"#393e46"}, btnTextColor {"#f0ece2"};
};

#endif // TODOPAGE_H
