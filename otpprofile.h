#ifndef OTPPROFILE_H
#define OTPPROFILE_H

#include <QObject>

#include "helper.h"

class otpProfile
{
public:
    otpProfile();

    void setToken(const QString &token);
    QString getToken();
    QString getTOTP();
    HelperResult removeTOTP();

    enum OTPtype
    {
        OTPtype_TOTP = 0
    };

    QString name;
    QString description;
    OTPtype otptype;
    QString uuid_token;
};

#endif // OTPPROFILE_H
