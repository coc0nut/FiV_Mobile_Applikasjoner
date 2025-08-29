#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

Database::Database(QObject *parent) : QObject{parent} {
    
}

Database::~Database() {
    close();
}

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

bool Database::createUsersTable() {
    QSqlQuery query;
    return query.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT UNIQUE, "
        "password TEXT)"
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

bool Database::addUser(QString const &username, QString const &password) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (?, ?)");
    query.addBindValue(username);
    query.addBindValue(password);
    return query.exec();
}