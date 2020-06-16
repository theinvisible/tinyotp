#ifndef HELPER_H
#define HELPER_H


#include <QString>

struct HelperResult
{
    bool status;
    QString msg;
    QString data;
};

class Helper
{
public:
    Helper();

    static QString formatByteUnits(qint64 num);

    static HelperResult checkSystemPasswordStoreAvailable();
    static HelperResult systemPasswordStoreWrite(const QString &key, const QString &data);
    static HelperResult systemPasswordStoreRead(const QString &key);
    static HelperResult systemPasswordStoreDelete(const QString &key);

    static int aes128_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext);
    static int aes128_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,unsigned char *iv, unsigned char *plaintext);
    static QString Qaes128_encrypt(const QString &plain, const QString &key, const QString &iv);
    static QString Qaes128_decrypt(const QString &cipher, const QString &key, const QString &iv);

    static void ssl_handleErrors(void);
    static QString getOSCodename();

    static QString runCommandwithOutput(const QString &cmd);
    static int runCommandwithReturnCode(const QString &cmd);

};

#endif // HELPER_H
