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

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    QMenu *tray_menu;

private slots:
    void onQuit();
    void onSettings();
};
#endif // MAINWINDOW_H
