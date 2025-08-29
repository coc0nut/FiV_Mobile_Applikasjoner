#include "mainwindow.h"
#include "database.h"
#include "login.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Database database;
    database.open();
    database.createUsersTable();
    database.addUser("test", "test123");

    Login login(&database);
    if (login.exec() == QDialog::Accepted) {
        MainWindow w;
        w.setWindowTitle("Note.io");
        w.show();
    
    
        return a.exec();
    }

    return 0;
}
