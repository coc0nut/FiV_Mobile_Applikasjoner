#ifndef MAINCONTENT_H
#define MAINCONTENT_H

#include <QStackedWidget>

class MainContent : public QStackedWidget
{
    Q_OBJECT
public:
    explicit MainContent(QWidget *parent = nullptr);

    QWidget *homePage(), *profilePage(), *settingsPage(), *todoPage();

private:
    QWidget *m_homePage, *m_profilePage, *m_settingsPage, *m_todoPage;
signals:
};

#endif // MAINCONTENT_H
