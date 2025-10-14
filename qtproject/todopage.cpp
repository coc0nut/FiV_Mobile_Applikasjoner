#include "todopage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QDateTime>
#include <QMessageBox>
#include <QJsonObject>  

TodoPage::TodoPage(NetworkManager *net, Database *db, User *user, Todo *todo, QWidget *parent)
    : QWidget(parent), db(db), user(user), todo(todo), net(net) {
    
    QVBoxLayout *todoLayout = new QVBoxLayout(this);
    todoLayout->setSpacing(1);
    
    
    setStyleSheet(
        QString(
            // "background: %1;"
            // "border-radius: 8px;"
            // "padding: 12px;"
            // "color: %2;"
        )//.arg(bgColorDark, textColorDark)
    );
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    header = new QLabel("Todo", this);
    header->setStyleSheet(
        QString(
            "font-weight: bold;"
            "font-size: 35px;"
            "background: %1;"
            "color %2;"
        ).arg(bgColor, textColor));
    todoLayout->addWidget(header);

    // Title edit
    todoTitleEdit = new QLineEdit(this);
    todoTitleEdit->setPlaceholderText("Title");
    todoTitleEdit->setStyleSheet(
        QString(
            "QLineEdit {"
            " border-radius: 8px;"
            " padding: 4px;"
            " font-size: 23px;"
            " font-weight: bold;"
            " background: %1;"
            " color: %2;"
            "}"
        ).arg(bgColorDark, textColorDark)
    );
    todoLayout->addWidget(todoTitleEdit);

    connect(todoTitleEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        if (this->todo) {
            this->todo->setTitle(text);
        }
    });

    // Times
    created_on = new QLabel("Created on: ", this);
    created_on->setStyleSheet(
        QString("background: %1; color: %2; font-weight: bold;").arg(bgColor, textColor)
    );
    updated_on = new QLabel("Updated on: ", this);
    updated_on->setStyleSheet(
        QString("background: %1; color: %2; font-weight: bold;").arg(bgColor, textColor)
    );
    due = new QLabel("Due: ", this);
    due->setStyleSheet(
        QString("background: %1; color: %2; border-radius: 8px; padding: 8px; font-weight: bold;").arg(bgColor, textColor)
    );

    dueEdit = new QDateTimeEdit(QDateTime::currentDateTime().addDays(7), this);
    dueEdit->setDisplayFormat("dd.MM.yyyy HH:mm");
    dueEdit->setCalendarPopup(true);
    dueEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    //dueEdit->setStyleSheet(QString("background: %1; color: %2;").arg(bgColor, textColor));

    QHBoxLayout *dueLayout = new QHBoxLayout();
    QWidget *dueContainer = new QWidget(this);
    
    dueContainer->setLayout(dueLayout);
    dueContainer->setFixedWidth(250);
    dueContainer->setFixedHeight(55);
    // dueContainer->setStyleSheet(
    //     QString("background: %1; color: %2; border-radius: 8px;").arg(bgColorDark, textColorDark)
    // );
    
    dueLayout->addWidget(due);
    dueLayout->addWidget(dueEdit);
    
    todoLayout->addWidget(created_on);
    todoLayout->addWidget(updated_on);
    todoLayout->addWidget(dueContainer);

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
    todoTextEdit->setPlaceholderText("Enter markdown here...");
    todoTextEdit->setStyleSheet(
        QString(
        "QTextEdit {"
        "  background: %1;"
        "  border-radius: 8px;"
        "  border: 1px solid #cccccc;"
        "  padding: 4px;"
        "  font-size: 15px;"
        "  color: %2;"
            "}").arg(bgColorDark, textColorDark)
    );

    connect(todoTextEdit, &QTextEdit::textChanged, this, [this]() {
        if (this->todo) {
            this->todo->setText(todoTextEdit->toPlainText());
        }
    });

    // Buttons

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    int buttonHeight = 40;
    addButton = new QPushButton("Add", this);
    addButton->setStyleSheet(QString("background: %1; color: %2; border-radius: 8px;").arg(bgColorDark, textColorDark));
    addButton->setFixedHeight(buttonHeight);
    
    updateButton = new QPushButton("Update", this);
    updateButton->setStyleSheet(QString("background: %1; color: %2; border-radius: 8px;").arg(bgColorDark, textColorDark));
    updateButton->setFixedHeight(buttonHeight);
    
    deleteButton = new QPushButton("Delete", this);
    deleteButton->setStyleSheet(QString("background: %1; color: %2; border-radius: 8px;").arg(bgColorDark, textColorDark));
    deleteButton->setFixedHeight(buttonHeight);


    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(updateButton);
    buttonLayout->addWidget(deleteButton);

    todoLayout->addWidget(todoTextEdit);
    todoLayout->addLayout(buttonLayout);

    // addButton connection
    connect(addButton, &QPushButton::clicked, this, [this]() {
        if (!this->todo || !this->net) {
            QMessageBox::warning(this, "Error", "Todo or NetworkManager not available.");
            return;
        }

        qDebug() << "=== TodoPage: Todos BEFORE creating ===";
        qDebug() << "Todo::todos.size():" << Todo::todos.size();
        for (const auto &v : Todo::todos) {
            qDebug() << "Existing todo:" << v->id() << v->title() << v->text();
        }

        addButton->setEnabled(false);

        QString dueString = dueEdit->dateTime().toString(Qt::ISODate);

        if (this->net->createTodo(
            this->todo->title(),
            this->todo->text(),
            this->todo->completed() != 0,
            dueString
        )) {
            qDebug() << "TodoPage: Create request started for:" << this->todo->title();
        } else {
            QMessageBox::warning(this, "Error", "Failed to start create request.");
            addButton->setEnabled(true);
        }
    });

    // updateButton connection
    connect(updateButton, &QPushButton::clicked, this, [this]() {
        if (!this->todo || !this->net) {
            QMessageBox::warning(this, "Error", "Todo or NetworkManager not available.");
            return;
        }

        updateButton->setEnabled(false);

        QString title = todoTitleEdit->text();
        QString text = todoTextEdit->toPlainText();
        bool completed = completedCheckBox->isChecked();
        QString dueString = dueEdit->dateTime().toString(Qt::ISODate);

        if (this->net->updateTodo(
            this->todo->id(),
            title,
            text,
            completed,
            dueString
        )) {
            qDebug() << "TodoPage: Update request started for todo ID:" << this->todo->id();
        } else {
            QMessageBox::warning(this, "Error", "Failed to start update request.");
            updateButton->setEnabled(true);
        }
    });

    // deleteButton connection
    connect(deleteButton, &QPushButton::clicked, this, [this]() {
        if (!this->todo || !this->net) {
            QMessageBox::warning(this, "Error", "Todo or NetworkManager not available.");
            return;
        }

        QMessageBox::StandardButton reply = QMessageBox::question(
            this, 
            "Delete Todo", 
            QString("Are you sure you want to delete '%1'?").arg(this->todo->title()),
            QMessageBox::Yes | QMessageBox::No
        );
        
        if (reply != QMessageBox::Yes) {
            return;
        }

        deleteButton->setEnabled(false);

        if (this->net->deleteTodo(this->todo->id())) {
            qDebug() << "TodoPage: Delete request started for todo ID:" << this->todo->id();
        } else {
            QMessageBox::warning(this, "Error", "Failed to start delete request.");
            deleteButton->setEnabled(true);
        }
    });

    connect(net, &NetworkManager::todoDeleted, this, [this](int todoId) {
        deleteButton->setEnabled(true);
        
        qDebug() << "=== TodoPage: Todo deleted successfully ===";
        qDebug() << "Deleted todo ID:" << todoId;
        
        QMessageBox::information(this, "Deleted", "Todo was successfully deleted.");
        emit navigateToHomePage();
    });

    connect(net, &NetworkManager::todoDeleteFailed, this, [this](const QString &error) {
        deleteButton->setEnabled(true);
        QMessageBox::warning(this, "Failed to delete todo", QString("Failed to delete todo: %1").arg(error));
    });

    connect(net, &NetworkManager::todoCreated, this, [this](const QJsonObject &todoObj) {
        addButton->setEnabled(true);
        
        // Debug todos AFTER creating
        qDebug() << "=== TodoPage: Todos AFTER creating ===";
        qDebug() << "Todo::todos.size():" << Todo::todos.size();
        for (const auto &v : Todo::todos) {
            qDebug() << "Todo after create:" << v->id() << v->title() << v->text();
        }
        
        QMessageBox::information(this, "Todo added", "The todo record was successfully added");
        emit navigateToHomePage();
    });

    connect(net, &NetworkManager::todoCreateFailed, this, [this](const QString &error) {
        addButton->setEnabled(true);
        QMessageBox::warning(this, "Failed to add todo", QString("Failed to create todo: %1").arg(error));
    });

    connect(net, &NetworkManager::todoUpdated, this, [this](const QJsonObject &todoObj) {
        updateButton->setEnabled(true);
        
        if (todoObj.contains("updated_on")) {
            updated_on->setText("Updated on: " + todoObj.value("updated_on").toString());
        }
        
        qDebug() << "=== TodoPage: Todo updated successfully ===";
        QMessageBox::information(this, "Updated", "Todo updated successfully.");
        emit navigateToHomePage();
    });

    connect(net, &NetworkManager::todoUpdateFailed, this, [this](const QString &error) {
        updateButton->setEnabled(true);
        QMessageBox::warning(this, "Failed to update todo", QString("Failed to update todo: %1").arg(error));
    });



}

void TodoPage::setTodo(Todo* todo) {
    if (!todo) return;

    this->todo = todo;

    bool isExisting = todo && std::find(Todo::todos.begin(), Todo::todos.end(), todo) != Todo::todos.end();
    if (isExisting) {
        header->setText("Edit Todo");
        addButton->hide();
        updateButton->show();
    } else {
        header->setText("Add new Todo");
        dueEdit->setDateTime(QDateTime::currentDateTime().addDays(7));
        addButton->show();
        updateButton->hide();
    }

    todoTitleEdit->setText(todo->title());
    created_on->setText("Created on: " + todo->created_on());
    updated_on->setText("Updated on: " + todo->updated_on());
    dueEdit->setDateTime(QDateTime::fromString(todo->due(), Qt::ISODate));
    todoTextEdit->setPlainText(todo->text());
    completedCheckBox->setChecked(todo->completed() != 0);
}
