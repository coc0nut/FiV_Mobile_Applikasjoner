#include "todo.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QPushButton>

Todo::Todo(QWidget *parent) : QWidget(parent) {



    QVBoxLayout *todoLayout = new QVBoxLayout(this);
    todoTextEdit = new QTextEdit(this);

    todoTextEdit->setStyleSheet(
        "QTextEdit {"
        "  border-radius: 12px;"
        "  border: 1px solid #cccccc;"
        "  padding: 4px;"
        "}"
    );

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    addButton = new QPushButton("Add", this);
    updateButton = new QPushButton("Update", this);
    deleteButton = new QPushButton("Delete", this);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(updateButton);
    buttonLayout->addWidget(deleteButton);

    todoLayout->addWidget(todoTextEdit);
    todoLayout->addLayout(buttonLayout);
}

