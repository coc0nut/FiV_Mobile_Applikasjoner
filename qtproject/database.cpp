#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

// Constructors

    Database::Database(QObject *parent) : QObject{parent} {

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
            "password text)"
        );
    }

    bool Database::checkUserCredentials(QString const &username, QString const &password) {

        QSqlQuery query;

        query.prepare("SELECT COUNT(*) FROM users WHERE username = ? AND password = ?");
        query.addBindValue(username);
        query.addBindValue(password);

        if (query.exec() && query.next()) {
            return query.value(0).toInt() > 0;
        }

        return false;
    }

    bool Database::checkUserExists(QString const &username) {

        QSqlQuery query;

        query.prepare("SELECT FROM users WHERE username = ?");
        query.addBindValue(username);

        if (query.exec() && query.next()) {
            return false;
        } else {
            return true;
        }
    }

    bool Database::addUser(QString const &username, QString const &password) {

        QSqlQuery query;

        query.prepare("INSERT INTO users (username, password) VALUES (?, ?)");
        query.addBindValue(username);
        query.addBindValue(password);

        return query.exec();
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
