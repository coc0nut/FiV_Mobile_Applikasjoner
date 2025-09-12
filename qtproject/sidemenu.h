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
    QString bgColor {"#a6a6a6"}, textColor {"#393e46"};
    QString bgColorDark {"#393e46"}, textColorDark {"#f0ece2"};
    QString btnBgColor {"#393e46"}, btnTextColor {"#f0ece2"};
};

#endif // SIDEMENU_H
