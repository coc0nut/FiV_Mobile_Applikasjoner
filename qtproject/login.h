#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "database.h"

class QLineEdit;
class QPushButton;

class Login : public QDialog
{
    Q_OBJECT
public:
    explicit Login(Database *db, QWidget *parent = nullptr);

    QString const username();
    QString const password();

private:
    QLineEdit *userNameEdit;
    QLineEdit *passWordEdit;
    QPushButton *loginButton, *newAccountButton;
    Database *db;

signals:

private slots:
    void onLoginClicked();
};

#endif // LOGIN_H
