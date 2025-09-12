#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "database.h"
#include "user.h"

class QLineEdit;
class QPushButton;

class Login : public QDialog
{
    Q_OBJECT
public:
    explicit Login(Database *db, User *user, QWidget *parent = nullptr);

    QString const username();
    QString const password();


private:
    QLineEdit *userNameEdit;
    QLineEdit *passWordEdit;
    QPushButton *loginButton, *newAccountButton;
    Database *db;
    User *user;

    QString bgColor {"#a6a6a6"}, textColor {"#393e46"};
    QString bgColorDark {"#393e46"}, textColorDark {"#f0ece2"};
    QString btnBgColor {"#393e46"}, btnTextColor {"#f0ece2"};

signals:

private slots:
    void onLoginClicked();
    void onNewAccountClicked();
};

#endif // LOGIN_H
