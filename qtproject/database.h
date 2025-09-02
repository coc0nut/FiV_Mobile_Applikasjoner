#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>

#include "user.h"
#include "todo.h"

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(User *user, Todo *todo, QObject *parent = nullptr);
    ~Database();

    bool open(QString const &dbName = "qtproject.db");
    void close();

    bool createUsersTable();
    bool checkUserCredentials(QString const &username, QString const &password);
    bool checkUserExists(QString const &username);
    bool addUser(QString const &username, QString const &password);
    bool importUsers();

    bool createTodosTable();
    bool addTodo(
        int user_id, int completed, QString title, QString text,
        QString created_on, QString updated_on, QString due
    );
    bool importTodos();

private:
    QSqlDatabase db;
    User *user;
    Todo *todo;

signals:

};

#endif // DATABASE_H
