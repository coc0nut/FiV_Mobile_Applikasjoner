#include "todo.h"

QVector<Todo*> Todo::todos;
int Todo::todo_count = 0;

Todo::Todo(QObject *parent)
    : QObject{parent}
{

}

int Todo::id() const
{
    return m_id;
}

void Todo::setId(int newId)
{
    if (m_id == newId)
        return;
    m_id = newId;
    emit idChanged();
}

int Todo::user_id() const
{
    return m_user_id;
}

void Todo::setUser_id(int newUser_id)
{
    if (m_user_id == newUser_id)
        return;
    m_user_id = newUser_id;
    emit user_idChanged();
}

int Todo::completed() const
{
    return m_completed;
}

void Todo::setCompleted(int newCompleted)
{
    if (m_completed == newCompleted)
        return;
    m_completed = newCompleted;
    emit completedChanged();
}

QString Todo::title() const
{
    return m_title;
}

void Todo::setTitle(const QString &newTitle)
{
    if (m_title == newTitle)
        return;
    m_title = newTitle;
    emit titleChanged();
}

QString Todo::text() const
{
    return m_text;
}

void Todo::setText(const QString &newText)
{
    if (m_text == newText)
        return;
    m_text = newText;
    emit textChanged();
}

QString Todo::created_on() const
{
    return m_created_on;
}

void Todo::setCreated_on(const QString &newCreated_on)
{
    if (m_created_on == newCreated_on)
        return;
    m_created_on = newCreated_on;
    emit created_onChanged();
}

QString Todo::updated_on() const
{
    return m_updated_on;
}

void Todo::setUpdated_on(const QString &newUpdated_on)
{
    if (m_updated_on == newUpdated_on)
        return;
    m_updated_on = newUpdated_on;
    emit updated_onChanged();
}

QString Todo::due() const
{
    return m_due;
}

void Todo::setDue(const QString &newDue)
{
    if (m_due == newDue)
        return;
    m_due = newDue;
    emit dueChanged();
}
