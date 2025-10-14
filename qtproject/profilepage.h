#ifndef PROFILEPAGE_H
#define PROFILEPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "database.h"
#include "user.h"
#include "networkmanager.h"

class ProfilePage : public QWidget
{
    Q_OBJECT
public:
    explicit ProfilePage(NetworkManager *net, Database *db, User *user, QWidget *parent = nullptr);

private slots:
    void onChangePasswordClicked();

signals:
    void userDetailsChanged();

private:
    Database *db;
    NetworkManager *net;
    User *user;

    QLabel *chpasswd, *username, *firstName, *lastName;
    QLineEdit *oldPasswordEdit, *newPasswordEdit, *confirmPasswordEdit;
    QLineEdit *first_nameEdit, *last_nameEdit, *emailEdit;

    QPushButton *changePasswordButton;

    QString bgColor {"#a6a6a6"}, textColor {"#393e46"};
    QString bgColorDark {"#393e46"}, textColorDark {"#f0ece2"};
    QString btnBgColor {"#393e46"}, btnTextColor {"#f0ece2"};

    QPushButton *changeUserDetailsButton;
};

#endif // PROFILEPAGE_H
