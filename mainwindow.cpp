#include "mainwindow.h"

#include "cotp.h"
#include <iostream>

#include <QCoreApplication>
#include <QEvent>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    installEventFilter(this);

    cotp_error_t err;
    char *totp = get_totp ("YOXLJV3LNOJ7AMDT", 6, 30, SHA1, &err);
    std::cout << "totp::" << totp << std::endl;
    free (totp);

    tray = new QSystemTrayIcon(this);
    tray->setIcon(QIcon(":/img/tinyotp_icon.png"));
    tray->show();
    tray_menu = tray->contextMenu();
    if(tray_menu == 0)
        tray_menu = new QMenu();

    tray_menu->addAction(QIcon(":/img/settings.png"), tr("Settings"), this, SLOT(onSettings()));
    tray_menu->addAction(QIcon(":/img/quit.png"), tr("Quit OpenFortiGUI"), this, SLOT(onQuit()));

    tray->setContextMenu(tray_menu);
}

MainWindow::~MainWindow()
{
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if(object == this && event->type() == QEvent::Close)
    {
        hide();

        event->ignore();
        return true;
    }

    return false;
}

void MainWindow::onQuit()
{
    QCoreApplication::quit();
}

void MainWindow::onSettings()
{

}

