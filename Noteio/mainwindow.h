#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "styles.h"
#include "sidemenu.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QHBoxLayout *mainLayout = nullptr;
    QVBoxLayout *sideLayout = nullptr;
    QWidget *central = nullptr;
    SideMenu *sideMenu = nullptr;
    QLabel *logo = nullptr;
    Styles *styles = new Styles;
};
#endif // MAINWINDOW_H
