#ifndef TODOPAGE_H
#define TODOPAGE_H

#include <QWidget>

class QTextEdit;
class QPushButton;

class TodoPage : public QWidget
{
    Q_OBJECT
public:
    TodoPage(QWidget *parent = nullptr);

private:
    QTextEdit *todoTextEdit;
    QPushButton *addButton, *updateButton, *deleteButton;
};

#endif // TODOPAGE_H
