#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "database.h"
#include "user.h"
#include "todo.h"

class HomePage : public QWidget {
    Q_OBJECT

public:
    explicit HomePage(Database *db, User *user, QWidget *parent = nullptr);

    QFrame *createLine(QWidget *parent);

public slots:
    void refreshTodos();
    void onEditButtonClicked();
    void onCompleteButtonClicked();

signals:
    void editTodoRequested(int todoId);
    


private:

    Database *db;
    User *user;
    QLabel *userDetails;
    QLabel *bilde;


    QLabel *title;
    QLabel *text;
    QLabel *created_on;
    QLabel *updated_on;
    QLabel *due;

    QPushButton *editButton, *completeButton;
    QString bgColor {"#d3d6db"}, textColor {"#393e46"};
    QString bgColorDark {"#393e46"}, textColorDark {"#f0ece2"};
    QString btnBgColor {"#393e46"}, btnTextColor {"#f0ece2"};

    QWidget *contentWidget;
    QVBoxLayout *homePageLayout;
    QVBoxLayout *todoLayout;

};


#endif // HOMEPAGE_H
