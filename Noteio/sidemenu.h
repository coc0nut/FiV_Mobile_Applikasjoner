#ifndef SIDEMENU_H
#define SIDEMENU_H

#include <QTreeWidget>


class SideMenu : public QTreeWidget
{
    Q_OBJECT
public:
    explicit SideMenu(QWidget *parent = nullptr);
    QTreeWidgetItem *homeItem, *profileItem, *settingsItem, *todoItem;


    void populateTodos(int user_id);
    void setCurrentItemByName(const QString& itemName);

public slots:
    void refreshTodos();

private:
    int currentUserId = -1;

};

#endif // SIDEMENU_H
