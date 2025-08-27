#ifndef TODO_H
#define TODO_H

#include <QWidget>

class QTextEdit;
class QPushButton;

class Todo : public QWidget
{
    Q_OBJECT
public:
    Todo(QWidget *parent = nullptr);

private:
    QTextEdit *todoTextEdit;
    QPushButton *addButton, *updateButton, *deleteButton;
};

#endif // TODO_H
