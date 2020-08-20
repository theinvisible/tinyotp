#ifndef TICONFMAIN_H
#define TICONFMAIN_H


#include <QSettings>

#include "otpprofile.h"

class tiConfMain
{
public:
    tiConfMain();
    ~tiConfMain();

    static tiConfMain* instance()
    {
       static CGuard g;
       if (!_instance)
          _instance = new tiConfMain();
       return _instance;
    }

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
    static tiConfMain* _instance;
    QSettings *settings;

    class CGuard
    {
    public:
       ~CGuard()
       {
          if( NULL != tiConfMain::_instance )
          {
             delete tiConfMain::_instance;
             tiConfMain::_instance = NULL;
          }
       }
    };
};

class tiConfOTPProfiles
{
public:
    tiConfOTPProfiles();
    ~tiConfOTPProfiles();

    static tiConfOTPProfiles* instance()
    {
       static CGuard g;
       if (!_instance)
          _instance = new tiConfOTPProfiles();
       return _instance;
    }

    void saveOTPProfile(const otpProfile &profile);
    void readOTPProfiles();

    QList<otpProfile*> getOTPProfiles();
    otpProfile* getOTPProfileByName(const QString &name);

    bool removeOTPProfileByName(const QString &name);

    bool renameOTPProfile(const QString &oldname, const QString &newname);
    bool copyOTPProfile(const QString &origname, const QString &cpname);

private:
    static tiConfOTPProfiles* _instance;
    tiConfMain *main_settings;
    bool read_profile_passwords;

    QList<otpProfile*> otpprofiles;

    class CGuard
    {
    public:
       ~CGuard()
       {
          if( NULL != tiConfOTPProfiles::_instance )
          {
             //delete tiConfOTPProfiles::_instance;
             tiConfOTPProfiles::_instance = NULL;
          }
       }
    };
};

#endif // TICONFMAIN_H
