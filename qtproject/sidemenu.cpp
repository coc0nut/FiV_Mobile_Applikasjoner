#include "sidemenu.h"
#include "todo.h"

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

    profileItem = new QTreeWidgetItem(this, QStringList("Profile"));
    homeItem = new QTreeWidgetItem(this, QStringList("Home"));
    todoItem = new QTreeWidgetItem(this, QStringList("Todo"));
    settingsItem = new QTreeWidgetItem(this, QStringList("Settings"));
}

void SideMenu::populateTodos(int user_id) {
    currentUserId = user_id;
    todoItem->takeChildren();

    for (Todo* todo: Todo::todos) {
        if (todo->user_id() == user_id) {
            auto *todoChild = new QTreeWidgetItem(todoItem, QStringList(todo->title()));
            todoChild->setData(0, Qt::UserRole, QVariant::fromValue(static_cast<void*>(todo)));
        }
    }
    todoItem->setExpanded(true);
}

void SideMenu::refreshTodos() {
    if (currentUserId != -1) {
        populateTodos(currentUserId);
    }
}
