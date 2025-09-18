#include "mainwindow.h"
#include "database.h"
#include "login.h"
#include "user.h"
#include "todo.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QFont appFont("Calibri");
    a.setFont(appFont);

    User user;
    Todo todo;

    Database database(&user, &todo);
    database.open();
    database.createUsersTable();

    database.importUsers();

    for (auto &u : User::users) {
        User::user_count++;
        qInfo() << u->id() <<  u->username() << u->password();
    }

    database.createTodosTable();
    database.importTodos();

    for (auto &t : Todo::todos) {
        Todo::todo_count++;
        //qInfo() << t->id() << t->user_id() << t->title() << t->text() << t->completed() << t->created_on() << t->updated_on() << t->due();
    }


    Login login(&database, &user);
    if (login.exec() == QDialog::Accepted) {
        MainWindow w(&database, &user, &todo);
        w.setWindowTitle("Note.io");
        w.show();
    
    
        return a.exec();
    }

    return 0;

}
