#include "maincontent.h"
#include "todo.h"

#include <QWidget>

MainContent::MainContent(QWidget *parent)
    : QStackedWidget{parent}
{
    m_homePage = new QWidget();
    m_profilePage = new QWidget();
    m_settingsPage = new QWidget();
    m_todoPage = new Todo();

    addWidget(m_homePage);
    addWidget(m_profilePage);
    addWidget(m_settingsPage);
    addWidget(m_todoPage);
}

QWidget* MainContent::homePage() { return m_homePage; }
QWidget* MainContent::profilePage() { return m_profilePage; }
QWidget* MainContent::settingsPage() { return m_settingsPage; }
QWidget* MainContent::todoPage() { return m_todoPage; }