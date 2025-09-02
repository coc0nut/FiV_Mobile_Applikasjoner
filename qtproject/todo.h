#ifndef TODO_H
#define TODO_H

#include <QObject>
#include <QVector>

class Todo : public QObject
{
    Q_OBJECT
public:
    explicit Todo(QObject *parent = nullptr);

    int id() const;
    void setId(int newId);
    int user_id() const;
    void setUser_id(int newUser_id);
    int completed() const;
    void setCompleted(int newCompleted);
    QString title() const;
    void setTitle(const QString &newTitle);
    QString text() const;
    void setText(const QString &newText);
    QString created_on() const;
    void setCreated_on(const QString &newCreated_on);
    QString updated_on() const;
    void setUpdated_on(const QString &newUpdated_on);
    QString due() const;
    void setDue(const QString &newDue);


    static QVector<Todo*> todos;
    static int todo_count;


signals:

    void idChanged();
    void user_idChanged();
    void completedChanged();
    void titleChanged();
    void textChanged();
    void created_onChanged();
    void updated_onChanged();
    void dueChanged();

private:
    int m_id, m_user_id, m_completed;
    QString m_title, m_text, m_created_on, m_updated_on, m_due;
};

#endif // TODO_H
