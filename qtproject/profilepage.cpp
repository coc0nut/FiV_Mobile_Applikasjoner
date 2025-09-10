#include "profilepage.h"

#include <QVBoxLayout>
#include <QLabel>

ProfilePage::ProfilePage(Database *db, User *user, QWidget *parent)
    : QWidget{parent}, db(db), user(user)
{

    QLabel *test = new QLabel("username: " + user->username(), this);

    usernameEdit = new QLineEdit("", this);

    QVBoxLayout *profileLayout = new QVBoxLayout(this);
    profileLayout->addWidget(test);
    profileLayout->setAlignment(Qt::AlignTop);
    profileLayout->setSpacing(20);


    profileLayout->addWidget(usernameEdit);



}
