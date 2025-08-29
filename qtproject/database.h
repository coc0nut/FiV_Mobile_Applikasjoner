#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    ~Database();

    bool open(QString const &dbName = "qtproject.db");
    void close();

    bool createUsersTable();
    bool checkUserCredentials(QString const &username, QString const &password);
    bool checkUserExists(QString const &username);
    bool addUser(QString const &username, QString const &password);

    bool createTodosTable();

private:
    QSqlDatabase db;

signals:

};

#endif // DATABASE_H
