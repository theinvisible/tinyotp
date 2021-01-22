#include "otpprofile.h"
#include "cotp.h"

#include <QUuid>

otpProfile::otpProfile()
{
    name = "";
    description = "";
    uuid_token = QUuid::createUuid().toString().replace("{", "").replace("}", "");
    otptype = OTPtype::OTPtype_TOTP;
    token_enc = "";
    global_aes_enc = false;
}

void otpProfile::setToken(const QString &token)
{
    if(global_aes_enc)
    {
        QString iv = Helper::randString(16);
        QString key = Helper::systemPasswordStoreRead("globalaeskey").data;
        QString enc = Helper::Qaes128_encrypt(token, key, iv);
        token_enc = iv + ".." + enc;
    }
    else
    {
        Helper::systemPasswordStoreWrite(uuid_token, token);
    }
}

QString otpProfile::getToken()
{
    if(global_aes_enc)
    {
        QStringList encarr = token_enc.split("..");
        if(encarr.length() == 2)
        {
            QString iv = encarr[0];
            QString enc = encarr[1];
            QString key = Helper::systemPasswordStoreRead("globalaeskey").data;
            return Helper::Qaes128_decrypt(enc, key, iv);
        }
        else
            return "";

    }
    else
    {
        HelperResult res = Helper::systemPasswordStoreRead(uuid_token);
        return res.data;
    }
}

QString otpProfile::getTOTP()
{
    cotp_error_t err;
    char *totp = get_totp(getToken().toStdString().c_str(), 6, 30, SHA1, &err);
    QString totp_token(totp);
    free (totp);

    return totp_token;
}

HelperResult otpProfile::removeTOTP()
{
    return Helper::systemPasswordStoreDelete(uuid_token);
}
