#include "dialogotpprofile.h"
#include "settings.h"
#include "ticonfmain.h"
#include "ui_settings.h"

#include <QMessageBox>
#include <QStandardItemModel>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    QStringList headers;
    headers << tr("Name") << tr("Description") << tr("Uuid");
    QStandardItemModel *model = new QStandardItemModel(ui->tvOTPProfiles);
    model->setHorizontalHeaderLabels(headers);
    ui->tvOTPProfiles->setModel(model);
    ui->tvOTPProfiles->sortByColumn(0, Qt::AscendingOrder);

    ui->tvOTPProfiles->header()->resizeSection(0, 150);
    ui->tvOTPProfiles->header()->resizeSection(1, 250);

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

    QStandardItemModel *model = dynamic_cast<QStandardItemModel *>(ui->tvOTPProfiles->model());
    model->removeRows(0, model->rowCount());

    QStandardItem *item = 0;
    QStandardItem *item2 = 0;
    QStandardItem *item3 = 0;

    QList<otpProfile*> otps = ticonfotpp->getOTPProfiles();
    for(int i=0; i < otps.count(); i++)
    {
        otpProfile *otp = otps.at(i);

        item = new QStandardItem(otp->name);
        item->setData(otp->name);
        item2 = new QStandardItem(otp->description);
        item3 = new QStandardItem(otp->uuid_token);

        model->setItem(i, 0, item);
        model->setItem(i, 1, item2);
        model->setItem(i, 2, item3);
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
