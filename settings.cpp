#include "dialogotpprofile.h"
#include "settings.h"
#include "ticonfmain.h"
#include "ui_settings.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    QStringList headers;
    headers << tr("Name") << tr("Description") << tr("Uuid") << tr("Use global AES encryption");
    QStandardItemModel *model = new QStandardItemModel(ui->tvOTPProfiles);
    model->setHorizontalHeaderLabels(headers);
    ui->tvOTPProfiles->setModel(model);
    ui->tvOTPProfiles->sortByColumn(0, Qt::AscendingOrder);

    ui->tvOTPProfiles->header()->resizeSection(0, 250);
    ui->tvOTPProfiles->header()->resizeSection(1, 250);
    ui->tvOTPProfiles->header()->resizeSection(2, 300);

    loadData();
}

Settings::~Settings()
{
    delete ui;
}

void Settings::loadData()
{
    tiConfOTPProfiles *ticonfotpp = tiConfOTPProfiles::instance();
    ticonfotpp->readOTPProfiles();

    tiConfMain *ticonfmain = tiConfMain::instance();

    ui->cbDebug->setChecked(ticonfmain->getValue("main/debug").toBool());
    ui->leGlobalAESKey->setText(Helper::systemPasswordStoreRead("globalaeskey").data);
    ui->leLogs->setText(ticonfmain->getValue("paths/logs").toString());
    ui->leOTPProfiles->setText(ticonfmain->getValue("paths/otpprofiles").toString());

    QStandardItemModel *model = dynamic_cast<QStandardItemModel *>(ui->tvOTPProfiles->model());
    model->removeRows(0, model->rowCount());

    QStandardItem *item = 0;
    QStandardItem *item2 = 0;
    QStandardItem *item3 = 0;
    QStandardItem *item4 = 0;

    QList<otpProfile*> otps = ticonfotpp->getOTPProfiles();
    for(int i=0; i < otps.count(); i++)
    {
        otpProfile *otp = otps.at(i);

        item = new QStandardItem(otp->name);
        item->setData(otp->name);
        item2 = new QStandardItem(otp->description);
        item3 = new QStandardItem(otp->uuid_token);
        item4 = new QStandardItem((otp->global_aes_enc) ? tr("yes") : tr("no"));

        model->setItem(i, 0, item);
        model->setItem(i, 1, item2);
        model->setItem(i, 2, item3);
        model->setItem(i, 3, item4);
    }
}

void Settings::on_btnAdd_clicked()
{
    DialogOtpProfile *d = new DialogOtpProfile(this);
    d->prepareDialog(DialogOtpProfile::ActionType_Add);
    connect(d, &DialogOtpProfile::form_finished, this, [=]() { loadData(); });
    d->show();
}

void Settings::on_tvOTPProfiles_doubleClicked(const QModelIndex &index)
{
    QStandardItemModel *model = dynamic_cast<QStandardItemModel *>(ui->tvOTPProfiles->model());
    QItemSelectionModel *selmodel = ui->tvOTPProfiles->selectionModel();
    QModelIndexList sellist = selmodel->selectedRows(0);

    if(sellist.count() < 1)
    {
        return;
    }

    QString otpName = model->itemFromIndex(sellist.at(0))->data().toString();
    if(otpName.isEmpty())
        return;

    DialogOtpProfile *d = new DialogOtpProfile(this);
    d->prepareDialog(DialogOtpProfile::ActionType_Edit, otpName);
    connect(d, &DialogOtpProfile::form_finished, this, [=]() { loadData(); });
    d->show();
}

void Settings::on_btnRemove_clicked()
{
    QStandardItemModel *model = dynamic_cast<QStandardItemModel *>(ui->tvOTPProfiles->model());
    QItemSelectionModel *selmodel = ui->tvOTPProfiles->selectionModel();
    QModelIndexList sellist = selmodel->selectedRows(0);

    if(sellist.count() < 1)
    {
        return;
    }

    QString otpName = model->itemFromIndex(sellist.at(0))->data().toString();
    if(otpName.isEmpty())
        return;

    QMessageBox::StandardButton sel = QMessageBox::question(this, tr("Remove OTP profile"), tr("You really want to remove profile %1").arg(otpName));
    if(sel == QMessageBox::Yes)
    {
        tiConfOTPProfiles *ticonfotpp = tiConfOTPProfiles::instance();
        ticonfotpp->removeOTPProfileByName(otpName);
        loadData();
    }
}

void Settings::pathChooser(QLineEdit *widget)
{
    QString startDir = (widget->text().isEmpty()) ? QDir::homePath() : tiConfMain::formatPath(widget->text());

    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a directory"),
                                                    startDir,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    if(!dir.isEmpty())
        widget->setText(dir);
}

void Settings::on_btnLogs_clicked()
{
    pathChooser(ui->leLogs);
}

void Settings::on_btnOTPProfiles_clicked()
{
    pathChooser(ui->leOTPProfiles);
}

void Settings::on_buttonBox_accepted()
{
    tiConfMain *ticonfmain = tiConfMain::instance();

    ticonfmain->setValue("main/debug", ui->cbDebug->isChecked());
    ticonfmain->setValue("paths/logs", tiConfMain::formatPathReverse(ui->leLogs->text()));
    ticonfmain->setValue("paths/otpprofiles", tiConfMain::formatPathReverse(ui->leOTPProfiles->text()));
    ticonfmain->sync();

    Helper::systemPasswordStoreWrite("globalaeskey", ui->leGlobalAESKey->text());
}

void Settings::on_btnGenAESKey_clicked()
{
    if(!ui->leGlobalAESKey->text().isEmpty())
    {
        QMessageBox::StandardButton sel = QMessageBox::question(this, tr("Changing AES Key"), tr("When changing the AES key, global encrypted profiles with the old key cannot be read anymore, continue?"));
        if(sel == QMessageBox::No)
        {
            return;
        }
    }

    ui->leGlobalAESKey->setText(Helper::randString(16));
}
