#include "profilepage.h"

#include <QVBoxLayout>
#include <QLabel>

ProfilePage::ProfilePage(Database *db, User *user, QWidget *parent)
    : QWidget{parent}, db(db), user(user)
{

    QLabel *test = new QLabel(user->username(), this);

    QVBoxLayout *profileLayout = new QVBoxLayout(this);
    profileLayout->addWidget(test);


}
