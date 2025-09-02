#include "homepage.h"

#include <QVBoxLayout>
#include <QPixmap>


HomePage::HomePage(Database *db, User *user, QWidget *parent) : QWidget{parent}, db(db), user(user) {

    QVBoxLayout *homePageLayout = new QVBoxLayout(this);

    homePageLayout->setAlignment(Qt::AlignTop);
    homePageLayout->setSpacing(2);
    homePageLayout->setContentsMargins(8, 8, 8, 8);

    QPixmap profile_pic(":pictures/robot.png");
    bilde = new QLabel(this);
    bilde->setPixmap(profile_pic.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    bilde->setFixedSize(100, 100);
    bilde->setStyleSheet(
        "QLabel {"
        " border-radius: 8px;"
        " border: 1px solid #ccc;"
        " background: #fff;"
        "}"
        );

    username = new QLabel("Username: " + user->username(), this);
    password = new QLabel("Password: " + user->password(), this);
    id = new QLabel("ID:" + QString::number(user->id()), this);

    username->setFixedHeight(20);
    password->setFixedHeight(20);
    id->setFixedHeight(20);

    homePageLayout->addWidget(bilde);
    homePageLayout->addWidget(id);
    homePageLayout->addWidget(username);
    homePageLayout->addWidget(password);
}
