#include "dialogotpprofile.h"
#include "ticonfmain.h"
#include "ui_dialogotpprofile.h"
#include "config.h"

#include <QMessageBox>

DialogOtpProfile::DialogOtpProfile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOtpProfile)
{
    ui->setupUi(this);

    // Validators
    QRegExp rx(tinyotp_config::validatorName);
    QValidator *validatorName = new QRegExpValidator(rx, this);
    ui->leName->setValidator(validatorName);
}

DialogOtpProfile::~DialogOtpProfile()
{
    delete ui;
}

void DialogOtpProfile::prepareDialog(DialogOtpProfile::ActionType action, const QString &otpprofile_name)
{
   action_type = action;
   profile_name = otpprofile_name;

   if(action_type == ActionType_Edit)
   {
       tiConfOTPProfiles *ticonfotpp = tiConfOTPProfiles::instance();
       otpProfile *p = ticonfotpp->getOTPProfileByName(profile_name);

        ui->leName->setText(p->name);
        ui->leDescription->setText(p->description);
        ui->leToken->setText(p->getToken());
        ui->cbUseGlobalEnc->setChecked(p->global_aes_enc);
   }
}

void DialogOtpProfile::on_buttonBox_accepted()
{
    if(ui->leName->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Missing information"), tr("You must provide a profile name!"));
        return;
    }

    if(ui->leToken->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Missing information"), tr("You must provide a token!"));
        return;
    }

    tiConfOTPProfiles *ticonfotpp = tiConfOTPProfiles::instance();
    otpProfile p;

    if(action_type == ActionType_Edit)
    {
        tiConfOTPProfiles *ticonfotpp = tiConfOTPProfiles::instance();
        otpProfile *po = ticonfotpp->getOTPProfileByName(profile_name);

        p.name = ui->leName->text();
        p.uuid_token = po->uuid_token;
        if(p.name != profile_name)
            ticonfotpp->renameOTPProfile(profile_name, p.name);
    }

    p.name = ui->leName->text();
    p.otptype = otpProfile::OTPtype_TOTP;
    p.description = ui->leDescription->text();
    p.global_aes_enc = ui->cbUseGlobalEnc->isChecked();
    p.setToken(ui->leToken->text());
    ticonfotpp->saveOTPProfile(p);

    emit form_finished();

    done(0);
}

void DialogOtpProfile::on_buttonBox_rejected()
{
    emit form_finished();
}

void DialogOtpProfile::accept()
{

}
