#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QCalendarWidget>
#include <QListWidget>
#include <QDate>

#include "database.h"
#include "user.h"
#include "calendarpanel.h"
#include "networkmanager.h"


class HomePage : public QWidget {
    Q_OBJECT

public:
    explicit HomePage(NetworkManager *net, Database *db, User *user, QWidget *parent = nullptr);

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

    NetworkManager *net;
    Database *db;
    User *user;
    Todo *todo;
    QMap<QDate, QVector<Todo*>> todosByDate;
    QDate dateFilter;
    CalendarPanel *calendarPanel;

    int completedCount {0}, activeCount {0}, totalCount {0};

    QLabel *completedCounts, *activeCounts, *totalCounts;

    QString searchString;
    QLineEdit *searchTextEdit;

    // profile
    QLabel *userDetails, *bilde;

    // Todo
    QLabel *title, *text, *created_on, *updated_on, *due;
    QPushButton *editButton, *completeButton;

    // CSS -- QSS
    QString bgColor {"#a6a6a6"}, textColor {"#393e46"};
    QString bgColorDark {"#393e46"}, textColorDark {"#f0ece2"};
    QString btnBgColor {"#393e46"}, btnTextColor {"#f0ece2"};

    QWidget *contentWidget;
    QVBoxLayout *homePageLayout;
    QVBoxLayout *todoLayout;

private slots:

};


#endif // HOMEPAGE_H
