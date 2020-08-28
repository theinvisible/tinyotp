#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSystemTrayIcon *tray;

    enum TASKBAR_POSITION {
        TASKBAR_POSITION_TOP,
        TASKBAR_POSITION_BOTTOM
    };

    static TASKBAR_POSITION taskbarPosition();

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QMenu *tray_menu;

private slots:
    void onQuit();
    void onSettings();
    void genOTP(const QString &name, bool checked);
    void updateTray();
    void onAbout();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
};
#endif // MAINWINDOW_H
