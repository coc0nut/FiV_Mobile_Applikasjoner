#include "homepage.h"


#include <QHBoxLayout>
#include <QPixmap>
#include <QFrame>
#include <QWidget>
#include <QScrollArea>
#include <algorithm>
#include <QDateTime>
#include <QTextEdit>
#include <QLocale>

#include <QTimer>
#include "todo.h"



HomePage::HomePage(NetworkManager *net, Database *db, User *user, QWidget *parent)
    : QWidget{parent}, db(db), user(user), net(net) {

    connect(net, &NetworkManager::currentUserUpdated, this, &HomePage::refreshUserDetails);
    connect(net, &NetworkManager::currentUserFetched, this, &HomePage::refreshUserDetails);

    // Main layout
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(10);

    // Center widget
    QWidget *centerWidget = new QWidget();
    
    // Center layout (profile card + todos) - now inside centerWidget
    QVBoxLayout *centerLayout = new QVBoxLayout(centerWidget);
    centerLayout->setAlignment(Qt::AlignTop);
    centerLayout->setSpacing(20);
    centerLayout->setContentsMargins(8, 8, 8, 8);

    // Header
    QLabel *header = new QLabel("Home", centerWidget);
    header->setStyleSheet("font-weight: bold; font-size: 46px;");
    centerLayout->addWidget(header);

    // Profile Card
    QPixmap profile_pic(":pictures/monkey.jpg");
    bilde = new QLabel(centerWidget);  // CHANGED: parent to centerWidget
    bilde->setFixedSize(150, 150);
    bilde->setAlignment(Qt::AlignCenter);
    bilde->setPixmap(profile_pic.scaled(
        bilde->size(),
        Qt::KeepAspectRatioByExpanding,
        Qt::SmoothTransformation
    ));

    userDetails = new QLabel(
        "Profile:\n"
        "Username: " + user->username()
        + "\nName: " + user->first_name()
        + " " + user->last_name()
        + "\nEmail: " + user->email()
        + "\nLast login: " + QDateTime::fromString(user->last_login(), Qt::ISODate).toString("dd.MM.yyyy HH:mm"),
        centerWidget
    );

    permissions = new QLabel(
        "Roles:\n"
        "isStaff: " + QString::number(user->getIs_staff())
        + "\nisActive: " + QString::number(user->getIs_active())
        + "\nisSuperUser: " + QString::number(user->getIs_superuser()),
        centerWidget
    );

    QWidget *profileCardWidget = new QWidget(centerWidget); 
    QHBoxLayout *profileCardLayout = new QHBoxLayout(profileCardWidget);
    profileCardLayout->setAlignment(Qt::AlignVCenter);
    profileCardWidget->setLayout(profileCardLayout);
    profileCardWidget->setStyleSheet(
        QString(
            "background: %1;"
            "border-radius: 12px;"
            "padding: 12px;"
            "color: %2;"
        ).arg(bgColorDark, textColorDark)
    );

    profileCardLayout->addWidget(bilde, 0);

    QHBoxLayout *profileTextLayout = new QHBoxLayout();
    profileTextLayout->setAlignment(Qt::AlignTop);
    profileTextLayout->setSpacing(8);
    profileTextLayout->addWidget(userDetails);
    profileTextLayout->addWidget(permissions);

    QHBoxLayout *profileDetailsLayout = new QHBoxLayout();
    profileDetailsLayout->setAlignment(Qt::AlignTop);
    profileDetailsLayout->setSpacing(20);
    profileDetailsLayout->setContentsMargins(0, 12, 0, 0);
    profileDetailsLayout->addLayout(profileTextLayout);

    QWidget *profileDetailsWidget = new QWidget(centerWidget);
    profileDetailsWidget->setStyleSheet(
        "background: transparent;"
        "border-radius: 8px;"
        "padding: 2px;"
        "margin-bottom: 12px;"
    );
    profileDetailsWidget->setLayout(profileDetailsLayout);
    profileCardLayout->addWidget(profileDetailsWidget, 1);

    centerLayout->addWidget(profileCardWidget);
    centerLayout->addWidget(createLine(centerWidget));

    // Counter
    QHBoxLayout *counter = new QHBoxLayout();
    counter->setAlignment(Qt::AlignHCenter);
    counter->setSpacing(500);

    completedCounts = new QLabel("Completed todos: " + QString::number(completedCount), centerWidget);
    activeCounts = new QLabel("Active todos: " + QString::number(activeCount), centerWidget);
    totalCounts = new QLabel("Total todos: " + QString::number(totalCount), centerWidget);
    completedCounts->setObjectName("counterLabel");
    activeCounts->setObjectName("counterLabel");
    totalCounts->setObjectName("counterLabel");

    counter->addWidget(activeCounts);
    counter->addWidget(completedCounts);
    counter->addWidget(totalCounts);

    centerLayout->addLayout(counter);
    centerLayout->addWidget(createLine(centerWidget));

    // Search bar
    QHBoxLayout *searchTextLayout = new QHBoxLayout();
    searchTextLayout->setAlignment(Qt::AlignLeft);

    searchTextEdit = new QLineEdit(centerWidget);
    searchTextEdit->setFixedWidth(350);
    searchTextEdit->setObjectName("searchTextEdit");
    searchTextEdit->setPlaceholderText("Search todos");
    searchTextEdit->setStyleSheet(QString("background: %1; color: %2; border-radius: 8px; border: none;").arg(bgColorDark, textColorDark));

    connect(searchTextEdit, &QLineEdit::textChanged, this, &HomePage::onSearchTextChanged);

    searchTextLayout->addWidget(searchTextEdit);
    centerLayout->addLayout(searchTextLayout);

    // Todos Layout
    todoLayout = new QVBoxLayout();
    todoLayout->setSpacing(20);
    centerLayout->addLayout(todoLayout);

    // create a scroll area for center content
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidget(centerWidget);
    scrollArea->setStyleSheet(
        "QScrollArea { border: none; }"
        "QScrollBar { border: none; }"
        "QScrollArea > QWidget { border: none; }"
    );

    mainLayout->addWidget(scrollArea, 1);  // Stretch factor 1

    // Calendar Panel (Right Section)
    calendarPanel = new CalendarPanel(this);
    calendarPanel->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
    calendarPanel->setStyleSheet(QString("background: %1; color: %2; border-radius: 8px;").arg(bgColorDark, textColorDark));

    connect(calendarPanel, &CalendarPanel::dateSelected, this, [this](const QDate &d) {
        dateFilter = d;
        refreshTodos();
    });

    mainLayout->addWidget(calendarPanel, 0, Qt::AlignTop);

    // Initialize
    refreshTodos();
}

void HomePage::refreshUserDetails(){
    userDetails->setText("Username: " + user->username()
        + "\nName: " + user->first_name() + " " + user->last_name()
        + "\nEmail: " + user->email());
}

void HomePage::refreshTodos() {
    qDebug() << "=== HomePage::refreshTodos() called ===";
    qDebug() << "Current Todo::todos.size():" << Todo::todos.size();

    QLayoutItem *child;
    while ((child = todoLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }
    completedCount = 0;
    activeCount = 0;
    totalCount = 0;

    QVector<Todo*> userTodos;

    for (Todo* t: Todo::todos) {

        if (t->user_id() == user->id() ) {

            if (searchString.isEmpty() && QDateTime::fromString(t->due()) < QDateTime::currentDateTime()) {
                userTodos.append(t);

            } else {
                if (t->title().contains(searchString, Qt::CaseInsensitive)) {
                    userTodos.append(t);
                } else if (t->text().contains(searchString, Qt::CaseInsensitive)) {
                    userTodos.append(t);
                }

            }

            if (t->completed()) {
                completedCount++;
            } else {
                activeCount++;
            }
            totalCount++;


        }
    }
    std::sort(userTodos.begin(), userTodos.end(), [](Todo *a, Todo *b) {
        QDateTime dueA = QDateTime::fromString(a->due(), Qt::ISODate);
        QDateTime dueB = QDateTime::fromString(b->due(), Qt::ISODate);
        return dueA < dueB;
    });

    if (calendarPanel) {
        calendarPanel->setTodos(userTodos);
    }


    if (dateFilter.isValid()) {
        QVector<Todo*> filtered;
        for (Todo* t : userTodos) {
            QDate d = QDate::fromString(t->due(), Qt::ISODate);
            if (d == dateFilter) filtered.append(t);
        }
        userTodos = filtered;
    }

    for (int i = 0; i < userTodos.length(); i++) {
        todo = userTodos[i];
        int todoId = todo->id();

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

        if (!todo->completed() && dueDT > QDateTime::currentDateTime() ) {

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

            QLabel *title = new QLabel(QString::number(i + 1) + ". " + userTodos[i]->title(), contentWidget);
            title->setWordWrap(true);
            title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
            title->setStyleSheet("font-weight: bold; font-size: 23px;");

            QTextEdit *text = new QTextEdit(contentWidget);
            text->setMarkdown(userTodos[i]->text());
            text->setReadOnly(true);
            text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

            text->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            text->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


            QTextDocument *doc = text->document();
            doc->setTextWidth(300);
            int contentHeight = doc->size().height();


            int desiredHeight = qMax(100, qMin(300, contentHeight + 30));
            text->setFixedHeight(desiredHeight);

            text->setStyleSheet(
                QString(
                    "background: %1;"
                    "color: %2;"
                    "font-size: 15px;"
                ).arg(bgColorDark, textColorDark)
            );


            created_on = new QLabel("Created: " + createdStr + " | Updated: " + updatedStr, contentWidget);
            created_on->setStyleSheet("color: #000; font-size: 12px; font-weight: bold;");

            due = new QLabel("Due: " + dueStr + " - " + QString::number(daysToDue) + " days left", contentWidget);
            due->setStyleSheet(QString("font-weight: bold; background: %1; color: %2; font-size: 15px;").arg(bgColorDark, textColorDark));


            QHBoxLayout *todoButtons = new QHBoxLayout();
            todoButtons->setSpacing(20);
            int buttonHeight = 45;
            editButton = new QPushButton("Edit", this);
            editButton->setFixedHeight(buttonHeight);
            editButton->setFixedWidth(200);
            editButton->setStyleSheet(
                QString(
                    "background: %1;"
                    "color: %2;"
                    ).arg(btnBgColor, btnTextColor)

                );

            connect(editButton, &QPushButton::clicked, this, [this, todoId]() {
                emit editTodoRequested(todoId);

            });

            completeButton = new QPushButton("Complete", this);
            completeButton->setFixedHeight(buttonHeight);
            completeButton->setFixedWidth(200);
            completeButton->setStyleSheet(
                QString(
                    "background: %1;"
                    "color: %2;"
                    )
                    .arg(btnBgColor, btnTextColor)
                );

            connect(completeButton, &QPushButton::clicked, this, [this, todoId]() {
                Todo* todoToComplete = nullptr;
                for (Todo* t : Todo::todos) {
                    if (t->id() == todoId) {
                        todoToComplete = t;
                        break;
                    }
                }

                if (!todoToComplete || !net) return;

                net->updateTodo(
                    todoToComplete->id(),
                    todoToComplete->title(),
                    todoToComplete->text(),
                    true, // Mark as completed
                    todoToComplete->due()
                );

            });

            todoButtons->addWidget(editButton);
            todoButtons->addWidget(completeButton);

            todoItemLayout->addWidget(due);
            if (alertSquare) todoItemLayout->addWidget(alertSquare);
            todoItemLayout->addWidget(title);
            todoItemLayout->addWidget(text);
            todoItemLayout->addLayout(todoButtons);
            todoItemLayout->addWidget(created_on);

            QHBoxLayout *rowLayout = new QHBoxLayout();
            rowLayout->addLayout(todoItemLayout);
            // if (alertSquare) rowLayout->addWidget(alertSquare);

            QWidget *todoWidget = new QWidget();
            todoWidget->setLayout(rowLayout);
            todoWidget->setStyleSheet(QString(
                "background: %1;"
                "border-radius: 8px;"
                "padding: 8px;"
                "color: %2;"
                "margin-bottom: 12px;"
                ).arg(bgColor, textColor)
            );

            todoLayout->addWidget(todoWidget);
            todoLayout->addWidget(createLine(this));
        }
        activeCounts->setText("Active todos: " + QString::number(activeCount));
        completedCounts->setText("Completed todos: " + QString::number(completedCount));
        totalCounts->setText("Total todos: " + QString::number(totalCount));

    }
    qDebug() << "=== HomePage::refreshTodos() completed ===";

}


void HomePage::onEditButtonClicked()
{
    qDebug() << "editButton Clicked...";

}

void HomePage::onCompleteButtonClicked()
{
    qDebug() << "completeButton clicked...";

}

QFrame *HomePage::createLine(QWidget *parent) {
    QFrame *line = new QFrame(parent);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    return line;
}

void HomePage::onSearchTextChanged(const QString &text) {
    searchString = text;
    refreshTodos();
}
