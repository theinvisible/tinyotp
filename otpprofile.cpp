#include "helper.h"
#include "otpprofile.h"
#include "cotp.h"

#include <QUuid>

otpProfile::otpProfile()
{
    name = "";
    description = "";
    uuid_token = QUuid::createUuid().toString().replace("{", "").replace("}", "");
    otptype = OTPtype::OTPtype_TOTP;
}

void otpProfile::setToken(const QString &token)
{
    Helper::systemPasswordStoreWrite(uuid_token, token);
}

QString otpProfile::getToken()
{
    HelperResult res = Helper::systemPasswordStoreRead(uuid_token);
    return res.data;
}

QString otpProfile::getTOTP()
{
    cotp_error_t err;
    char *totp = get_totp(getToken().toStdString().c_str(), 6, 30, SHA1, &err);
    QString totp_token(totp);
    free (totp);

    return totp_token;
}

void otpProfile::removeTOTP()
{
    Helper::systemPasswordStoreDelete(uuid_token);
}
