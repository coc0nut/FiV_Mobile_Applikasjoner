#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>

#include "database.h"
#include "user.h"

class HomePage : public QWidget {
    Q_OBJECT

public:
    explicit HomePage(Database *db, User *user, QWidget *parent = nullptr);

    QFrame *createLine(QWidget *parent);

public slots:
    void refreshTodos();
    void onEditButtonClicked();
    void onCompleteButtonClicked();
    void refreshUserDetails();
    void onSearchTextChanged(const QString &text);

signals:
    void editTodoRequested(int todoId);
    
private:

    Database *db;
    User *user;
    Todo *todo;

    int completedCount {0}, activeCount {0}, totalCount {0};

    QLabel *completedCounts, *activeCounts, *totalCounts;

    QString searchString;
    QLineEdit *searchTextEdit;

    QLabel *userDetails;
    QLabel *bilde;

    QLabel *title;
    QLabel *text;
    QLabel *created_on;
    QLabel *updated_on;
    QLabel *due;

    QPushButton *editButton, *completeButton;

    QString bgColor {"#a6a6a6"}, textColor {"#393e46"};
    QString bgColorDark {"#393e46"}, textColorDark {"#f0ece2"};
    QString btnBgColor {"#393e46"}, btnTextColor {"#f0ece2"};

    QWidget *contentWidget;
    QVBoxLayout *homePageLayout;
    QVBoxLayout *todoLayout;


};


#endif // HOMEPAGE_H
