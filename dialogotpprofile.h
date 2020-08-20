#ifndef DIALOGOTPPROFILE_H
#define DIALOGOTPPROFILE_H

#include <QDialog>

namespace Ui {
class DialogOtpProfile;
}

class DialogOtpProfile : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOtpProfile(QWidget *parent = nullptr);
    ~DialogOtpProfile();

    enum ActionType
    {
        ActionType_Add,
        ActionType_Edit
    };

    void prepareDialog(ActionType action, const QString &otpprofile_name = "");

signals:
    void form_finished();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

public slots:
    void accept();

private:
    Ui::DialogOtpProfile *ui;
    ActionType action_type;
    QString profile_name;
};

#endif // DIALOGOTPPROFILE_H
