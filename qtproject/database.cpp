#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

// Constructors

    Database::Database(User *user, Todo *todo, QObject *parent)
    : QObject{parent}, user(user), todo(todo) {

    }

    Database::~Database() {
        
        close();
    }

// Initializing

    bool Database::open(QString const &dbName) {

        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbName);

        if (!db.open()) {
            qDebug() << "Database open error:" << db.lastError().text();
            return false;
        }

        return true;
    }

    void Database::close() {

        if (db.isOpen()) {
            db.close();
        }
    }

// Users

    bool Database::createUsersTable() {

        QSqlQuery query;

        return query.exec(
            "create table if not exists users ("
            "id integer primary key autoincrement, "
            "username text unique, "
            "password text, "
            "name text, "
            "email text, "
            ")"
        );
    }

    bool Database::checkUserCredentials(QString const &username, QString const &password) {

        QSqlQuery query;

        query.prepare("SELECT id, username, password FROM users WHERE username = ? AND password = ?");
        query.addBindValue(username);
        query.addBindValue(password);

        if (query.exec() && query.next()) {
            user->setId(query.value(0).toInt());
            user->setUsername(query.value(1).toString());
            user->setPassword(query.value(2).toString());
            return true;
        }

        return false;
    }

    bool Database::checkUserExists(QString const &username) {

        QSqlQuery query;

        query.prepare("SELECT FROM users WHERE username = ?");
        query.addBindValue(username);

        if (query.exec() && query.next()) {
            return true;
        } else {
            return false;
        }
    }

    bool Database::addUser(QString const &username, QString const &password) {

        QSqlQuery query;

        query.prepare("INSERT INTO users (username, password) VALUES (?, ?)");
        query.addBindValue(username);
        query.addBindValue(password);

        return query.exec();
    }

    bool Database::importUsers()
    {
        QSqlQuery query;

        if (query.exec("SELECT * FROM users")) {
            
            for (User* user : User::users) {
                delete user;
            }
            User::users.clear();

            while (query.next()) {
                User *user = new User(this);
                user->setId(query.value(0).toInt());
                user->setUsername(query.value(1).toString());
                user->setPassword(query.value(2).toString());
                User::users.push_back(user);
            }
            return true;
        } else {
            return false;
        }
        
    }

// Todos

    bool Database::createTodosTable() {

        QSqlQuery query;

        return query.exec(
            "create table if not exists todos ("
            "id integer primary key autoincrement, "
            "user_id integer, "
            "title text, "
            "text text, "
            "completed integer default 0, "
            "created_on text, "
            "updated_on text, "
            "due text, "
            "foreign key (user_id) references users(id)"
            ")"
        );
    }

    int Database::addTodo(int user_id, int completed, QString title, QString text,
                           QString created_on, QString updated_on, QString due) {
        QSqlQuery query;

        query.prepare(
            "insert into todos (user_id, title, text, completed, created_on, updated_on, due) "
            "values (?, ?, ?, ?, ?, ?, ?)"
        );
        query.addBindValue(user_id);
        query.addBindValue(title);
        query.addBindValue(text);
        query.addBindValue(0);
        query.addBindValue(created_on);
        query.addBindValue(updated_on);
        query.addBindValue(due);

        if (!query.exec()) {
            qDebug() << "failed to add todo:" << query.lastError().text();
            return -1;
        }

        QSqlQuery idQuery("SELECT last_insert_rowid()");
        if (idQuery.next()) {
            return idQuery.value(0).toInt();
        }


        return -1;
    }

    bool Database::importTodos()
    {
        QSqlQuery query;


        if (query.exec("SELECT * FROM todos")) {

            for (Todo *todo: Todo::todos) {
                delete todo;
            }
            Todo::todos.clear();

            while (query.next()) {
                Todo *todo = new Todo(this);
                todo->setId(query.value("id").toInt());
                todo->setUser_id(query.value("user_id").toInt());
                todo->setTitle(query.value("title").toString());
                todo->setText(query.value("text").toString());
                todo->setCompleted(query.value("completed").toInt());
                todo->setCreated_on(query.value("created_on").toString());
                todo->setUpdated_on(query.value("updated_on").toString());
                todo->setDue(query.value("due").toString());

                Todo::todos.push_back(todo);
            }
            return true;
        } else {
            return false;
        }

        return true;
    }

    bool Database::updateTodo(Todo *todo)
    {
        if (!todo) return false;

        QSqlQuery query;

        query.prepare(
            "update todos set "
            "title = ?, "
            "text = ?, "
            "completed = ?, "
            "updated_on = ?, "
            "due = ? "
            "where id = ?"
        );
        query.addBindValue(todo->title());
        query.addBindValue(todo->text());
        query.addBindValue(todo->completed());
        query.addBindValue(todo->updated_on());
        query.addBindValue(todo->due());
        query.addBindValue(todo->id());

        if (!query.exec()) {
            qDebug() << "Failed to update todo: " << query.lastError().text();
            return false;
        }

        return true;
    }

    bool Database::deleteTodo(Todo *todo)
    {
        if (!todo) return false;

        qDebug() << "Attempting to delete todo with id:" << todo->id();

        QSqlQuery query;

        query.prepare(
            "delete from todos where id = ?"
        );

        query.addBindValue(todo->id());

        if (!query.exec()) {
            qDebug() << "Failed to delete todo:" << query.lastError().text();
            return false;
        }
        qDebug() << "Delete query executed, affected rows:" << query.numRowsAffected();
        return true;
    }
