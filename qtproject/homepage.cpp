#include "homepage.h"


#include <QHBoxLayout>
#include <QPixmap>
#include <QFrame>
#include <QWidget>
#include <QScrollArea>
#include <algorithm>
#include <QDateTime>
#include <QTimer>
#include "todo.h"


HomePage::HomePage(Database *db, User *user, QWidget *parent) : QWidget{parent}, db(db), user(user) {

    contentWidget = new QWidget(this);
    homePageLayout = new QVBoxLayout(contentWidget);

    homePageLayout->setAlignment(Qt::AlignTop);
    homePageLayout->setSpacing(2);
    homePageLayout->setContentsMargins(8, 8, 8, 8);

    // Profile Card

        QPixmap profile_pic(":pictures/robot.png");
        bilde = new QLabel(contentWidget);
        bilde->setFixedSize(100, 100);
        bilde->setAlignment(Qt::AlignCenter);
        bilde->setPixmap(profile_pic.scaled(
            bilde->size(),
            Qt::KeepAspectRatioByExpanding,
            Qt::SmoothTransformation
        ));

        userDetails = new QLabel("Username: " + user->username() + "\nName: " + user->name() + "\nEmail: " + user->email(), contentWidget);




        QWidget *profileCardWidget = new QWidget(contentWidget);
        QHBoxLayout *profileCardLayout = new QHBoxLayout();

        profileCardLayout->setAlignment(Qt::AlignVCenter);

        profileCardWidget->setLayout(profileCardLayout);
        profileCardWidget->setStyleSheet(
            QString(
                "background: #d3d6db;"
                "border-radius: 12px;"
                "padding: 12px;"
                "color: #000;"
                )
            );

        profileCardLayout->addWidget(bilde, 0);



        QVBoxLayout *profileDetailsLayout = new QVBoxLayout();

        profileDetailsLayout->setAlignment(Qt::AlignTop);
        profileDetailsLayout->setSpacing(1);
        profileDetailsLayout->setContentsMargins(0, 12, 0, 0);

        profileDetailsLayout->addWidget(userDetails);

        QWidget *profileDetailsWidget = new QWidget(contentWidget);
        profileDetailsWidget->setStyleSheet(
            "background: transparent;"
            "border-radius: 8px;"
            "padding: 2px;"
        );

        profileDetailsWidget->setLayout(profileDetailsLayout);
        profileCardLayout->addWidget(profileDetailsWidget, 1);

        homePageLayout->addWidget(profileCardWidget);

        QFrame *line = new QFrame(contentWidget);

        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line->setStyleSheet("color: #ccc;");

        homePageLayout->addWidget(line);

    // Todos Layout
        todoLayout = new QVBoxLayout();
        todoLayout->setSpacing(10);
        homePageLayout->addLayout(todoLayout);
        refreshTodos();
        

        contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QScrollArea *scrollArea = new QScrollArea(this);
        scrollArea->setWidgetResizable(true);
        scrollArea->setWidget(contentWidget);
        scrollArea->setStyleSheet(
            "QScrollArea { border: none; }"
            "QScrollBar { border: none; background: transparent; }"
            "QScrollArea > QWidget { border: none; }"
        );

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(scrollArea);
        this->setLayout(mainLayout);
}


void HomePage::refreshTodos() {
    QLayoutItem *child;
    while ((child = todoLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }

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
            Todo *todo = userTodos[i];


            // Format DateTime strings
            QDateTime dueDT = QDateTime::fromString(todo->due(), Qt::ISODate);
            QString dueStr = dueDT.toString("dd.MM.yyyy HH:mm");

            QDateTime createdDT = QDateTime::fromString(todo->created_on(), Qt::ISODate);
            QString createdStr = createdDT.toString("dd.MM.yyyy HH:mm");

            QDateTime updatedDT = QDateTime::fromString(todo->updated_on(), Qt::ISODate);
            QString updatedStr = updatedDT.toString("dd.MM.yyyy HH:mm");

            QDateTime now = QDateTime::currentDateTime();
            int daysToDue = now.daysTo(dueDT);

            QWidget *alertSquare = nullptr;
            QTimer *flashTimer = nullptr;

        
            if (daysToDue >= 0 && daysToDue < 3) {
                alertSquare = new QWidget(contentWidget);
                alertSquare->setFixedSize(25, 25);
                alertSquare->setStyleSheet(
                    "background: #be3144;"
                    "border-radius: 50%;"
                    "padding: 8px;"
                    "color: #000000;"
                    "margin-bottom: 12px;"
                );

                flashTimer = new QTimer(alertSquare);
                QObject::connect(flashTimer, &QTimer::timeout, alertSquare, [alertSquare]() {
                    bool on = alertSquare->property("flashOn").toBool();
                    if (on) {
                        alertSquare->setStyleSheet(
                            "background: #be3144;"
                            "border-radius: 8px;"
                            "padding: 8px;"
                            "margin-bottom: 12px;"
                        );
                    } else {
                        alertSquare->setStyleSheet(
                            "background: transparent;"
                            "border-radius: 8px;"
                            "padding: 8px;"
                            "margin-bottom: 12px;"
                            );
                    }
                    alertSquare->setProperty("flashOn", !on);

                });
                flashTimer->start(500);
                
            }


            QVBoxLayout *todoItemLayout = new QVBoxLayout();
            todoItemLayout->setSpacing(1);

            QLabel *title = new QLabel(userTodos[i]->title(), contentWidget);
            title->setStyleSheet("font-weight: bold; font-size: 18px;");

            QLabel *text = new QLabel(userTodos[i]->text(), contentWidget);
            text->setStyleSheet("font-size: 15px;");

            QLabel *created_on = new QLabel("Created: " + createdStr + " | Updated: " + updatedStr, contentWidget);
            created_on->setStyleSheet("color: #000; font-size: 12px; font-weight: bold;");

            QLabel *due = new QLabel("Due: " + dueStr, contentWidget);
            due->setStyleSheet("font-weight: bold; color: #888; font-size: 15px;");

            todoItemLayout->addWidget(due);
            todoItemLayout->addWidget(title);
            todoItemLayout->addWidget(text);
            todoItemLayout->addWidget(created_on);

            QHBoxLayout *rowLayout = new QHBoxLayout();
            rowLayout->addLayout(todoItemLayout);
            if (alertSquare) rowLayout->addWidget(alertSquare);

            QWidget *todoWidget = new QWidget(contentWidget);
            todoWidget->setLayout(rowLayout);
            todoWidget->setStyleSheet(
                "background: #d3d6db;"
                "border-radius: 8px;"
                "padding: 8px;"
                "color: #000000;"
                "margin-bottom: 12px;"
            );

            todoLayout->addWidget(todoWidget);

        }

}
