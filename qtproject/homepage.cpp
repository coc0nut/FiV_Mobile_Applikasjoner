#include "homepage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QFrame>
#include <QWidget>
#include <QScrollArea>
#include <algorithm>
#include <QDateTime>
#include "todo.h"



HomePage::HomePage(Database *db, User *user, QWidget *parent) : QWidget{parent}, db(db), user(user) {

    QWidget *contentWidget = new QWidget(this);
    QVBoxLayout *homePageLayout = new QVBoxLayout(contentWidget);

    homePageLayout->setAlignment(Qt::AlignTop);
    homePageLayout->setSpacing(2);
    homePageLayout->setContentsMargins(8, 8, 8, 8);

    // Profile Card

        QPixmap profile_pic(":pictures/robot.png");
        bilde = new QLabel(contentWidget);
        bilde->setPixmap(profile_pic.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        bilde->setFixedSize(100, 100);
        bilde->setStyleSheet(
            "QLabel {"
            " border-radius: 8px;"
            " border: 1px solid #ccc;"
            " background: #fff;"
            "}"
            );

        username = new QLabel("Username: " + user->username(), contentWidget);
        password = new QLabel("Password: " + user->password(), contentWidget);
        id = new QLabel("ID:" + QString::number(user->id()), contentWidget);

        username->setFixedHeight(20);
        password->setFixedHeight(20);
        id->setFixedHeight(20);

        QHBoxLayout *profileCardLayout = new QHBoxLayout();

        profileCardLayout->addWidget(bilde);

        QVBoxLayout *profileDetailsLayout = new QVBoxLayout();

        profileDetailsLayout->setAlignment(Qt::AlignTop);
        profileDetailsLayout->setSpacing(1);
        profileDetailsLayout->setContentsMargins(4, 4, 4, 4);

        profileDetailsLayout->addWidget(id);
        profileDetailsLayout->addWidget(username);
        profileDetailsLayout->addWidget(password);
        profileCardLayout->addLayout(profileDetailsLayout);

        homePageLayout->addLayout(profileCardLayout);

        QFrame *line = new QFrame(contentWidget);

        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line->setStyleSheet("color: #ccc;");

        homePageLayout->addWidget(line);

    // Todos overview

        QVBoxLayout *todoLayout = new QVBoxLayout();
        todoLayout->setSpacing(10);

        QVector<Todo*> userTodos;
        for (Todo* t: Todo::todos) {
            if (t->user_id() == user->id()) {
                userTodos.append(t);
            }
        }

        std::sort(userTodos.begin(), userTodos.end(), [](Todo *a, Todo *b) {
            QDateTime dueA = QDateTime::fromString(a->due(), Qt::ISODate);
            QDateTime dueB = QDateTime::fromString(b->due(), Qt::ISODate);
            return dueA < dueB;
        });

        for (int i = 0; i < userTodos.length(); i++) {
            if (userTodos[i]->user_id() == user->id()) {
                QVBoxLayout *todoItemLayout = new QVBoxLayout();
                todoItemLayout->setSpacing(1);

                QLabel *title = new QLabel(userTodos[i]->title(), contentWidget);
                title->setStyleSheet("font-weight: bold; font-size: 15px;");

                QLabel *text = new QLabel(userTodos[i]->text(), contentWidget);

                QLabel *created_on = new QLabel("Created: " + userTodos[i]->created_on() + " Updated: " + userTodos[i]->updated_on(), contentWidget);
                created_on->setStyleSheet("color: #aaa; font-size: 11px; font-weight: bold;");

                QLabel *due = new QLabel("Due: " + userTodos[i]->due(), contentWidget);
                due->setStyleSheet("font-weight: bold; color: #888; font-size: 15px;");

                todoItemLayout->addWidget(due);
                todoItemLayout->addWidget(title);
                todoItemLayout->addWidget(text);
                todoItemLayout->addWidget(created_on);

                QWidget *todoWidget = new QWidget(contentWidget);
                todoWidget->setLayout(todoItemLayout);
                todoWidget->setStyleSheet(
                    "background: #fff;"
                    "border-radius: 8px;"
                    "padding: 8px;"
                    "margin-bottom: 12px;"
                );

                todoLayout->addWidget(todoWidget);
            }
        }

        homePageLayout->addLayout(todoLayout);

        contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QScrollArea *scrollArea = new QScrollArea(this);
        scrollArea->setWidgetResizable(true);
        scrollArea->setWidget(contentWidget);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(scrollArea);
        this->setLayout(mainLayout);


}


