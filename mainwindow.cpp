#include "mainwindow.h"

#include "cotp.h"
#include <iostream>

#include <QCoreApplication>
#include <QEvent>
#include <QGuiApplication>
#include <QMenu>
#include <QScreen>
#include <QUuid>
#include <QClipboard>
#include <QFileSystemWatcher>

#include <iostream>
#include "helper.h"
#include "ticonfmain.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    installEventFilter(this);

    tray = new QSystemTrayIcon(this);
    tray->setIcon(QIcon(":/img/tinyotp_icon.png"));
    tray->show();
    tray_menu = tray->contextMenu();

    tiConfMain *ticonfmain = tiConfMain::instance();
    tiConfOTPProfiles *ticonfotpp = tiConfOTPProfiles::instance();
    ticonfotpp->readOTPProfiles();

    updateTray();

    QFileSystemWatcher *watcher = new QFileSystemWatcher(this);
    watcher->addPath(tiConfMain::formatPath(ticonfmain->getValue("paths/otpprofiles").toString()));
    connect(watcher, SIGNAL(directoryChanged(QString)), this, SLOT(updateTray()));
}

MainWindow::~MainWindow()
{
}

MainWindow::TASKBAR_POSITION MainWindow::taskbarPosition()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect geo = screen->geometry();
    QRect geoAvail = screen->availableGeometry();

    return (geoAvail.top() > geo.top()) ? MainWindow::TASKBAR_POSITION_TOP : MainWindow::TASKBAR_POSITION_BOTTOM;
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

void MainWindow::updateTray()
{
    tiConfOTPProfiles *ticonfotpp = tiConfOTPProfiles::instance();
    ticonfotpp->readOTPProfiles();

    if(tray_menu == 0)
        tray_menu = new QMenu();
    tray_menu->clear();

    if(MainWindow::taskbarPosition() == MainWindow::TASKBAR_POSITION_TOP)
    {
        tray_menu->addAction(QIcon(":/img/quit.png"), tr("Quit OpenFortiGUI"), this, SLOT(onQuit()));
        tray_menu->addAction(QIcon(":/img/settings.png"), tr("Settings"), this, SLOT(onSettings()));
        tray_menu->addSeparator();
    }

    QList<otpProfile*> otps = ticonfotpp->getOTPProfiles();
    for(int i=0; i < otps.count(); i++)
    {
        otpProfile *otp = otps.at(i);

        QAction *action = new QAction(otp->name, tray_menu);
        connect(action, &QAction::triggered, this, [=](bool checked) { genOTP(otp->name, checked); });
        tray_menu->insertAction(0, action);
    }

    if(MainWindow::taskbarPosition() == MainWindow::TASKBAR_POSITION_BOTTOM)
    {
        tray_menu->addSeparator();
        tray_menu->addAction(QIcon(":/img/settings.png"), tr("Settings"), this, SLOT(onSettings()));
        tray_menu->addAction(QIcon(":/img/quit.png"), tr("Quit OpenFortiGUI"), this, SLOT(onQuit()));
    }

    tray->setContextMenu(tray_menu);
}

void MainWindow::onQuit()
{
    QCoreApplication::quit();
}

void MainWindow::onSettings()
{
    Settings *s = new Settings(this);
    s->show();
}

void MainWindow::genOTP(const QString &name, bool checked)
{
    tiConfOTPProfiles *ticonfotpp = tiConfOTPProfiles::instance();
    otpProfile *p = ticonfotpp->getOTPProfileByName(name);

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(p->getTOTP());
}

