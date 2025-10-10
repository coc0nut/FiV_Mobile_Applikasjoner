#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "database.h"
#include "user.h"
#include "networkmanager.h"

class QLineEdit;
class QPushButton;

class Login : public QDialog
{
    Q_OBJECT
public:
    explicit Login(NetworkManager *net, Database *db, User *user, QWidget *parent = nullptr);

    QString const username();
    QString const password();


private:
    QLineEdit *userNameEdit;
    QLineEdit *passWordEdit;
    QPushButton *loginButton, *newAccountButton;
    Database *db;
    User *user;
    NetworkManager *net;

    QString bgColor {"#a6a6a6"}, textColor {"#393e46"};
    QString bgColorDark {"#393e46"}, textColorDark {"#f0ece2"};
    QString btnBgColor {"#393e46"}, btnTextColor {"#f0ece2"};

signals:

private slots:
    void onLoginClicked();
    void onNewAccountClicked();
    void onLoginSucceeded(const QString &access, const QString &refresh, int userId, const QString &username);
    void onLoginFailed(const QString &error);

};

#endif // LOGIN_H
