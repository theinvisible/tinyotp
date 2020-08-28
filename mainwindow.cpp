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
#include <QIcon>
#include <QMessageBox>

#include <iostream>
#include "helper.h"
#include "ticonfmain.h"
#include "settings.h"
#include "config.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    installEventFilter(this);

    tray = new QSystemTrayIcon(this);
    tray->setIcon(QIcon(":/img/tinyotp_icon.png"));
    tray->show();
    tray_menu = tray->contextMenu();
    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onTrayIconActivated(QSystemTrayIcon::ActivationReason)));

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
        tray_menu = new QMenu(this);
    tray_menu->clear();

    if(MainWindow::taskbarPosition() == MainWindow::TASKBAR_POSITION_TOP)
    {
        tray_menu->addAction(QIcon::fromTheme("application-exit", QIcon(":/img/quit.png")), tr("Quit tinyOTP"), this, SLOT(onQuit()));
        tray_menu->addAction(QIcon::fromTheme("preferences-system", QIcon(":/img/settings.png")), tr("Settings"), this, SLOT(onSettings()));
        tray_menu->addSeparator();
    }

    QList<otpProfile*> otps = ticonfotpp->getOTPProfiles();
    for(int i=0; i < otps.count(); i++)
    {
        otpProfile *otp = otps.at(i);

        QAction *action = new QAction(QIcon(":/img/key.png"), otp->name, tray_menu);
        connect(action, &QAction::triggered, this, [=](bool checked) { genOTP(otp->name, checked); });
        tray_menu->insertAction(0, action);
    }

    if(MainWindow::taskbarPosition() == MainWindow::TASKBAR_POSITION_BOTTOM)
    {
        tray_menu->addSeparator();
        tray_menu->addAction(QIcon::fromTheme("help-about", QIcon(":/img/about.png")), tr("About"), this, SLOT(onAbout()));
        tray_menu->addAction(QIcon::fromTheme("preferences-system", QIcon(":/img/settings.png")), tr("Settings"), this, SLOT(onSettings()));
        tray_menu->addAction(QIcon::fromTheme("application-exit", QIcon(":/img/quit.png")), tr("Quit tinyOTP"), this, SLOT(onQuit()));
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

void MainWindow::onAbout()
{
    QMessageBox::about(0, tr("About tinyOTP"), tr("<b>tinyOTP %1</b><br>"
                                                             "<table><tr><td width='150'>Developer:</td> <td><b>Rene Hadler</b></td></tr>"
                                                             "<tr><td>eMail:</td> <td> <a href=mailto:'rene@hadler.me'>rene@hadler.me</a></td></tr>"
                                                             "<tr><td>Website:</td> <td> <a href=https://hadler.me>https://hadler.me</a></td></tr></table>"
                                                             "<p>This program uses following libs/resources:</p>"
                                                             "<table><tr><td width='150'>GCC %2:</td> <td> <a href='https://gcc.gnu.org/'>https://gcc.gnu.org</a></td></tr>"
                                                             "<tr><td>QT %3:</td> <td> <a href='https://www.qt.io'>https://www.qt.io</a></td></tr>"
                                                             "<tr><td>Icons8:</td> <td> <a href='https://icons8.com/'>https://icons8.com</a></td></tr></table>").arg(tinyotp_config::version, __VERSION__, QT_VERSION_STR));
}

void MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
    {
        tray->contextMenu()->popup(QCursor::pos());
    }
}
