#ifndef SIDEMENU_H
#define SIDEMENU_H

#include <QTreeWidget>

class SideMenu : public QTreeWidget
{
    Q_OBJECT
public:
    explicit SideMenu(QWidget *parent = nullptr);
    QTreeWidgetItem *homeItem, *profileItem, *settingsItem, *todoItem;

private:
};

#endif // SIDEMENU_H
