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

    // Completed checkbox

    completedCheckBox = new QCheckBox("Completed", this);
    todoLayout->addWidget(completedCheckBox);

    connect(completedCheckBox, &QCheckBox::toggled, this, [this](bool checked) {
        if (this->todo) {
            this->todo->setCompleted(checked ? 1 : 0);
        }
    });


    // Memo / text
    todoTextEdit = new QTextEdit(this);

    todoTextEdit->setStyleSheet(
        "QTextEdit {"
        "  border-radius: 8px;"
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

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(updateButton);
    buttonLayout->addWidget(deleteButton);

    todoLayout->addWidget(todoTextEdit);
    todoLayout->addLayout(buttonLayout);

    // addButton connection
    connect(addButton, &QPushButton::clicked, this, [this]() {
        QString completed, created_on, updated_on, due;

        created_on = QDateTime::currentDateTime().toString(Qt::ISODate);
        updated_on = created_on;
        due = dueEdit->dateTime().toString(Qt::ISODate);

        if (this->todo) {

            this->todo->setCreated_on(created_on);
            this->todo->setUpdated_on(updated_on);
            this->todo->setDue(due);

            try {
                int newId = this->db->addTodo(
                    this->user->id(),
                    this->todo->completed(),
                    this->todo->title(),
                    this->todo->text(),
                    created_on,
                    updated_on,
                    due
                );

                if (newId != -1) {
                    Todo* newTodo = new Todo(this->db);
                    newTodo->setId(newId);
                    newTodo->setUser_id(this->user->id());
                    newTodo->setCompleted(this->todo->completed());
                    newTodo->setTitle(this->todo->title());
                    newTodo->setText(this->todo->text());
                    newTodo->setCreated_on(this->todo->created_on());
                    newTodo->setUpdated_on(this->todo->updated_on());
                    newTodo->setDue(this->todo->due());

                    Todo::todos.push_back(newTodo);

                }
            } catch (std::exception const &e) {
                QMessageBox::warning(this, "Failed to add todo", QString("The todo was not added...\nPlease try again.\nError: %1").arg(e.what()));
            }

            QMessageBox::information(this, "Todo added", "The todo record was successfully added");
            emit todosChanged();

        } else {
            QMessageBox::warning(this, "Failed to add todo", "The todo was not added...\nPlease try again.)");
        }
    });

    // updateButton connection
    connect(updateButton, &QPushButton::clicked, this, [this]() {
        if (!this->todo) return;

        this->todo->setTitle(todoTitleEdit->text());
        this->todo->setText(todoTextEdit->toPlainText());
        this->todo->setDue(dueEdit->dateTime().toString(Qt::ISODate));
        this->todo->setUpdated_on(QDateTime::currentDateTime().toString(Qt::ISODate));
        
        if (this->db->updateTodo(this->todo)) {
            updated_on->setText("Updated on: " + this->todo->updated_on());
            QMessageBox::information(this, "Updated", "Todo updated successfully.");
            emit todosChanged();
        } else {
            QMessageBox::warning(this, "Error", "Failed to update todo.");
        }
    });

    // deleteButton connection
    connect(deleteButton, &QPushButton::clicked, this, [this]() {
        if (!this->todo) return;

        if (this->db->deleteTodo(this->todo)) {

            QMessageBox::information(this, "Deleted", "Todo was successfully deleted.");
            try {
                auto it = std::find(Todo::todos.begin(), Todo::todos.end(), this->todo);
                if (it != Todo::todos.end()) {
                    Todo::todos.erase(it);
                }
            } catch (std::exception const &e) {
                qDebug() << "Error:" << e.what();
            }

            emit todosChanged();
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete todo.");
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
    completedCheckBox->setChecked(todo->completed() != 0);
}
