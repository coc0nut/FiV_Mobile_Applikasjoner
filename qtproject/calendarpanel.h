#ifndef CALENDARPANEL_H
#define CALENDARPANEL_H

#include <QWidget>
#include <QCalendarWidget>
#include <QListWidget>
#include <QMap>
#include <QDate>
#include <QVector>
#include <QEvent>

#include "todo.h" 

class CalendarPanel : public QWidget {
    Q_OBJECT
public:
    explicit CalendarPanel(QWidget *parent = nullptr);

    void setTodos(const QVector<Todo*> &todos);

    void clearSelection();
    void updateEventList(const QDate &date);

signals:
    void dateSelected(const QDate &date);

private slots:
    void onSelectionChanged();

private:
    void rebuildMap();
    void updateMarks();

    QCalendarWidget *calendar_ = nullptr;
    QListWidget *events_ = nullptr;
    QMap<QDate, QVector<Todo*>> todosByDate_;

    QString bgColor {"#a6a6a6"}, textColor {"#393e46"};
    QString bgColorDark {"#393e46"}, textColorDark {"#f0ece2"};
    QString btnBgColor {"#393e46"}, btnTextColor {"#f0ece2"};

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // CALENDARPANEL_H
