#include "ticonfmain.h"

#include <QDebug>
#include <QFile>
#include <QDirIterator>
#include <QValidator>
#include <QEventLoop>

#include "config.h"
#include "helper.h"
#include <qt5keychain/keychain.h>

QString tiConfMain::main_config = tiConfMain::formatPath(tinyotp_config::file_main);

tiConfMain::tiConfMain()
{
    settings = 0;

    initMainConf();

    if(!QFile(tiConfMain::formatPath(tiConfMain::main_config)).exists())
    {
        qCritical() << QString("tiConfMain::tiConfMain() -> Main configuration file <").append(tiConfMain::main_config).append("> not found, please fix this...");
        exit(EXIT_FAILURE);
    }

    settings = new QSettings(tiConfMain::formatPath(tiConfMain::main_config), QSettings::IniFormat);
}

tiConfMain::~tiConfMain()
{
    if(settings != 0)
        delete settings;
}

void tiConfMain::initMainConf()
{
    QFile conf_main(tiConfMain::formatPath(tiConfMain::main_config));
    if(!conf_main.exists())
    {
        QFileInfo finfo(tiConfMain::formatPath(tiConfMain::main_config));
        QDir conf_main_dir = finfo.absoluteDir();
        conf_main_dir.mkpath(conf_main_dir.absolutePath());
        QFile::setPermissions(conf_main_dir.absolutePath(), QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner | QFileDevice::ReadGroup | QFileDevice::ExeGroup);

        QString logs_dir = QString("%1/logs").arg(conf_main_dir.absolutePath());

        QDir profiles_path(tiConfMain::formatPath(tinyotp_config::profiles_dir));
        profiles_path.mkpath(tiConfMain::formatPath(tinyotp_config::profiles_dir));
        QDir logsdir_path(logs_dir);
        logsdir_path.mkpath(logs_dir);

        QSettings conf(tiConfMain::formatPath(tiConfMain::main_config), QSettings::IniFormat);
        conf.setValue("main/debug", false);
        conf.setValue("main/changelogrev_read", 0);
        conf.setValue("paths/otpprofiles", tinyotp_config::profiles_dir);
        conf.setValue("paths/logs", logs_dir);
        conf.setValue("paths/initd", tinyotp_config::initd_default);
        conf.sync();
    }
    else
    {
        QFileInfo finfo(tiConfMain::formatPath(tiConfMain::main_config));
        QDir conf_main_dir = finfo.absoluteDir();
        conf_main_dir.mkpath(conf_main_dir.absolutePath());
        QFile::setPermissions(conf_main_dir.absolutePath(), QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner | QFileDevice::ReadGroup | QFileDevice::ExeGroup);

        QString profiles_dir = QString("%1/profiles").arg(conf_main_dir.absolutePath());
        QString logs_dir = QString("%1/logs").arg(conf_main_dir.absolutePath());

        QDir profiles_path(tiConfMain::formatPath(tinyotp_config::profiles_dir));
        profiles_path.mkpath(tiConfMain::formatPath(tinyotp_config::profiles_dir));
        QDir logsdir_path(logs_dir);
        logsdir_path.mkpath(logs_dir);
    }
}

QVariant tiConfMain::getValue(const QString &iniPath, const QVariant &defaultValue)
{
    return settings->value(iniPath, defaultValue);
}

void tiConfMain::setValue(const QString &iniPath, const QVariant &val)
{
    settings->setValue(iniPath, val);
}

void tiConfMain::sync()
{
    settings->sync();
}

QString tiConfMain::formatPath(const QString &path)
{
    QString p = path;
    return p.replace("~", QDir::homePath());
}

QString tiConfMain::formatPathReverse(const QString &path)
{
    QString p = path;
    return p.replace(QDir::homePath(), "~");
}

QString tiConfMain::setMainConfig(const QString &config)
{
    QFile conf_main(tiConfMain::formatPath(config));
    if(conf_main.exists())
        tiConfMain::main_config = config;

    return tiConfMain::main_config;
}

QString tiConfMain::getAppDir()
{
    QFileInfo finfo(tiConfMain::formatPath(tiConfMain::main_config));
    return finfo.absoluteDir().absolutePath();
}

tiConfOTPProfiles::tiConfOTPProfiles()
{
    main_settings = new tiConfMain();
    QList<otpProfile*> otpprofiles;
    read_profile_passwords = false;
}

tiConfOTPProfiles::~tiConfOTPProfiles()
{
    delete main_settings;
}

void tiConfOTPProfiles::saveOTPProfile(const otpProfile &profile)
{
    QRegExp rexpName(tinyotp_config::validatorName);
    if(!rexpName.exactMatch(profile.name))
    {
        qWarning() << "tiConfOTPProfile::saveOTPProfile() -> profile has not a valid name: " << profile.name;
        return;
    }

    QString filename = QString(tiConfMain::formatPath(main_settings->getValue("paths/otpprofiles").toString())).append("/%1.conf").arg(profile.name);
    QDir otpdir(tiConfMain::formatPath(main_settings->getValue("paths/otpprofiles").toString()));
    if(!otpdir.exists())
        otpdir.mkpath(tiConfMain::formatPath(main_settings->getValue("paths/otpprofiles").toString()));

    if(QFile::exists(filename))
        QFile::remove(filename);

    QSettings *f = new QSettings(filename, QSettings::IniFormat);

    f->beginGroup("otp");
    f->setValue("name", profile.name);
    f->setValue("description", profile.description);
    f->setValue("otptype", profile.otptype);
    f->setValue("uuid_token", "");
    f->endGroup();

    f->sync();
    delete f;
}

void tiConfOTPProfiles::readOTPProfiles()
{
    otpprofiles.clear();

    QDirIterator it_otpdir(tiConfMain::formatPath(main_settings->getValue("paths/otpprofiles").toString()));
    QString otpprofilefilepath;
    while (it_otpdir.hasNext())
    {
        otpprofilefilepath = it_otpdir.next();
        if(otpprofilefilepath.endsWith(".conf"))
        {
            qDebug() << "tiConfOTPProfiles::readOTPProfiles() -> otpprofile found:" << otpprofilefilepath;

            QSettings *f = new QSettings(otpprofilefilepath, QSettings::IniFormat);
            otpProfile *otpprofile = new otpProfile;

            f->beginGroup("otp");
            otpprofile->name = f->value("name").toString();
            otpprofile->description = f->value("description").toString();
            otpprofile->otptype = static_cast<otpProfile::OTPtype>(f->value("otptype").toInt());
            otpprofile->uuid_token = f->value("uuid_token").toString();
            f->endGroup();

            otpprofiles.append(otpprofile);
            delete f;
        }
    }
}

QList<otpProfile *> tiConfOTPProfiles::getOTPProfiles()
{
    return otpprofiles;
}

otpProfile *tiConfOTPProfiles::getOTPProfileByName(const QString &otpname)
{
    readOTPProfiles();
    otpProfile *otp = 0;

    for(int i=0; i < otpprofiles.count(); i++)
    {
        otp = otpprofiles.at(i);
        if(otp->name == otpname)
            return otp;
    }

    return otp;
}

bool tiConfOTPProfiles::removeOTPProfileByName(const QString &otpname)
{
    qDebug() << "deleteotp:::::" << QString("%1/%2.conf").arg(tiConfMain::formatPath(main_settings->getValue("paths/otpprofiles").toString()), otpname);
    return QFile::remove(QString("%1/%2.conf").arg(tiConfMain::formatPath(main_settings->getValue("paths/otpprofiles").toString()), otpname));
}

bool tiConfOTPProfiles::renameOTPProfile(const QString &oldname, const QString &newname)
{
    return QFile::rename(QString("%1/%2.conf").arg(tiConfMain::formatPath(main_settings->getValue("paths/otpprofiles").toString()), oldname),
                         QString("%1/%2.conf").arg(tiConfMain::formatPath(main_settings->getValue("paths/otpprofiles").toString()), newname));
}

bool tiConfOTPProfiles::copyOTPProfile(const QString &origname, const QString &cpname)
{
    otpProfile *otp = getOTPProfileByName(origname);
    otpProfile newotp = *otp;
    newotp.name = cpname;
    saveOTPProfile(newotp);

    return true;
}
