#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private slots:
    void on_btnAdd_clicked();
    void loadData();
    void pathChooser(QLineEdit *widget);

    void on_tvOTPProfiles_doubleClicked(const QModelIndex &index);
    void on_btnRemove_clicked();

    void on_btnLogs_clicked();

    void on_btnOTPProfiles_clicked();

    void on_buttonBox_accepted();

    void on_btnGenAESKey_clicked();

private:
    Ui::Settings *ui;

};

#endif // SETTINGS_H
