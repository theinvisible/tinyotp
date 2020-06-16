#ifndef OTPPROFILE_H
#define OTPPROFILE_H

#include <QObject>

class otpProfile
{
public:
    otpProfile();

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
