#include "todopage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QDateTime>
#include <QMessageBox>

TodoPage::TodoPage(Database *db, User *user, Todo *todo, QWidget *parent)
    : QWidget(parent), db(db), user(user), todo(todo) {

    QVBoxLayout *todoLayout = new QVBoxLayout(this);

    // Title edit
    todoTitleEdit = new QLineEdit(this);
    todoTitleEdit->setPlaceholderText("Title");
    todoTitleEdit->setStyleSheet(
        "QLineEdit {"
        " border-radius: 8px;"
        " border: 1px solid #cccccc;"
        " padding: 4px;"
        "}"
    );
    todoLayout->addWidget(todoTitleEdit);

    connect(todoTitleEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        if (this->todo) {
            this->todo->setTitle(text);
        }
    });

    // Times
    created_on = new QLabel("Created on: ", this);
    updated_on = new QLabel("Updated on: ", this);
    due = new QLabel("Due: ", this);

    dueEdit = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    dueEdit->setDisplayFormat("dd.MM.yyyy HH:mm");
    dueEdit->setCalendarPopup(true);

    todoLayout->addWidget(created_on);
    todoLayout->addWidget(updated_on);
    todoLayout->addWidget(due);
    todoLayout->addWidget(dueEdit);


    // Memo / text
    todoTextEdit = new QTextEdit(this);

    todoTextEdit->setStyleSheet(
        "QTextEdit {"
        "  border-radius: 12px;"
        "  border: 1px solid #cccccc;"
        "  padding: 4px;"
        "}"
    );

    connect(todoTextEdit, &QTextEdit::textChanged, this, [this]() {
        if (this->todo) {
            this->todo->setText(todoTextEdit->toPlainText());
        }
    });

    // Buttons

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    addButton = new QPushButton("Add", this);
    updateButton = new QPushButton("Update", this);
    deleteButton = new QPushButton("Delete", this);
    completeButton = new QPushButton("Complete", this);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(updateButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(completeButton);

    todoLayout->addWidget(todoTextEdit);
    todoLayout->addLayout(buttonLayout);

    // addButton connection
    connect(addButton, &QPushButton::clicked, this, [this]() {
        QString completed, created_on, updated_on, due;

        created_on = QDateTime::currentDateTime().toString(Qt::ISODate);
        updated_on = created_on;
        due = dueEdit->dateTime().toString(Qt::ISODate);

        if (this->todo) {
            try {
                this->db->addTodo(
                    this->user->id(),
                    this->todo->completed(),
                    this->todo->title(),
                    this->todo->text(),
                    created_on,
                    updated_on,
                    due
                );
            } catch (std::exception const &e) {
                QMessageBox::warning(this, "Failed to add todo", QString("The todo was not added...\nPlease try again.\nError: %1").arg(e.what()));
            }

            QMessageBox::information(this, "Todo added", "The todo record was successfully added");
        } else {
            QMessageBox::warning(this, "Failed to add todo", "The todo was not added...\nPlease try again.)");
        }
    });

    // updateButton connection
    connect(updateButton, &QPushButton::clicked, this, [this]() {
        QString updated_on = QDateTime::currentDateTime().toString(Qt::ISODate);

        if (this->todo) {

        }
    });

}

void TodoPage::setTodo(Todo* todo) {
    if (!todo) return;
    this->todo = todo;
    todoTitleEdit->setText(todo->title());
    created_on->setText("Created on: " + todo->created_on());
    updated_on->setText("Updated on: " + todo->updated_on());
    dueEdit->setDateTime(QDateTime::fromString(todo->due(), Qt::ISODate));
    todoTextEdit->setPlainText(todo->text());
}
