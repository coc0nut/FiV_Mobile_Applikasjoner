#ifndef PROFILEPAGE_H
#define PROFILEPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include "database.h"
#include "user.h"

class ProfilePage : public QWidget
{
    Q_OBJECT
public:
    explicit ProfilePage(Database *db, User *user, QWidget *parent = nullptr);

private slots:
    void onChangePasswordClicked();

signals:

private:
    Database *db;
    User *user;

    QLabel *chpasswd;
    QLineEdit *oldPasswordEdit;
    QLineEdit *newPasswordEdit;
    QLineEdit *confirmPasswordEdit;

    QString bgColor {"#d3d6db"}, textColor {"#393e46"};
    QString bgColorDark {"#393e46"}, textColorDark {"#f0ece2"};
    QString btnBgColor {"#393e46"}, btnTextColor {"#f0ece2"};
};

#endif // PROFILEPAGE_H
