#ifndef TICONFMAIN_H
#define TICONFMAIN_H


#include <QSettings>

#include "otpprofile.h"

class tiConfMain
{
public:
    tiConfMain();
    ~tiConfMain();

    static QString main_config;

    void initMainConf();

    QVariant getValue(const QString &iniPath, const QVariant &defaultValue = QVariant());
    void setValue(const QString &iniPath, const QVariant &val);
    void sync();

    static QString formatPath(const QString &path);
    static QString formatPathReverse(const QString &path);
    static QString setMainConfig(const QString &config);

    static QString getAppDir();

private:
    QSettings *settings;
};

class tiConfOTPProfiles
{
public:
    tiConfOTPProfiles();
    ~tiConfOTPProfiles();

    void saveOTPProfile(const otpProfile &profile);
    void readOTPProfiles();

    QList<otpProfile*> getOTPProfiles();
    otpProfile* getOTPProfileByName(const QString &name);

    bool removeOTPProfileByName(const QString &name);

    bool renameOTPProfile(const QString &oldname, const QString &newname);
    bool copyOTPProfile(const QString &origname, const QString &cpname);

private:
    tiConfMain *main_settings;
    bool read_profile_passwords;

    QList<otpProfile*> otpprofiles;
};

#endif // TICONFMAIN_H
