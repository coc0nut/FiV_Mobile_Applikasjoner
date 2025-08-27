#include "sidemenu.h"


SideMenu::SideMenu(QWidget *parent) : QTreeWidget(parent) {
    setHeaderHidden(true);

    setStyleSheet(
        "QTreeWidget {"
        " border-radius: 12px;"
        " border: 1px solid #cccccc;"
        " background: #f8f8f8;"
        " padding: 10px;"
        "}"
    );

    homeItem = new QTreeWidgetItem(this, QStringList("Home"));
    profileItem = new QTreeWidgetItem(this, QStringList("Profile"));
    settingsItem = new QTreeWidgetItem(this, QStringList("Settings"));
    todoItem = new QTreeWidgetItem(this, QStringList("Todo"));
}
