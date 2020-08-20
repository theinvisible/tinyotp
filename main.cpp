#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QProcess>
#include <QFileInfo>
#include <QCommandLineParser>

#include <iostream>
#include "config.h"
#include "otpprofile.h"
#include "helper.h"
#include "ticonfmain.h"

bool isRunningAlready()
{
    QStringList arguments;
    arguments << "-A";

    QProcess *ch = new QProcess();
    ch->start("ps", arguments);
    ch->waitForFinished(5000);
    ch->waitForReadyRead(5000);
    QString line;
    int count = 0;
    while(!ch->atEnd())
    {
        line = QString::fromLatin1(ch->readLine());
        if(line.contains(QFileInfo(QCoreApplication::applicationFilePath()).fileName()))
            count++;
    }
    delete ch;

    return (count > 1) ? true : false;
}

int main(int argc, char *argv[])
{
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));

    if(argc > 1)
    {
        QApplication a(argc, argv);
        QApplication::setApplicationName(tinyotp_config::name);
        QApplication::setApplicationVersion(tinyotp_config::version);

        QCommandLineParser parser;
        parser.setApplicationDescription("Help for tinyOTP options");
        parser.addHelpOption();
        parser.addVersionOption();

        QCommandLineOption addOTPProcess("add-profile", QCoreApplication::translate("main", "Creates new OTP profile"));
        parser.addOption(addOTPProcess);

        QCommandLineOption removeOTPProcess("remove-profile", QCoreApplication::translate("main", "Removes given OTP profile"));
        parser.addOption(removeOTPProcess);

        QCommandLineOption otpName("otp-name",
                    QCoreApplication::translate("main", "Set OTP name <otpname>"),
                    QCoreApplication::translate("main", "otpname"));
        parser.addOption(otpName);

        QCommandLineOption otpDesc("otp-desc",
                    QCoreApplication::translate("main", "Set OTP description <otpdesc>"),
                    QCoreApplication::translate("main", "otpdesc"));
        parser.addOption(otpDesc);

        QCommandLineOption otpToken("otp-token",
                    QCoreApplication::translate("main", "Set OTP token <otptoken>"),
                    QCoreApplication::translate("main", "otptoken"));
        parser.addOption(otpToken);

        parser.process(a);

        bool arg_addprofile = parser.isSet(addOTPProcess);
        bool arg_removeprofile = parser.isSet(removeOTPProcess);
        QString arg_otpname = parser.value(otpName);
        QString arg_otpdesc = parser.value(otpDesc);
        QString arg_otptoken = parser.value(otpToken);

        if(arg_addprofile)
        {
            if(arg_otpname.isEmpty() || arg_otptoken.isEmpty())
            {
                std::cout << "Arguments otpname and otptoken must be provided to add a new OTP profile!" << std::endl;
                parser.showHelp();
            }
            else
            {
                tiConfOTPProfiles *ticonfotpp = tiConfOTPProfiles::instance();
                otpProfile p;
                p.name = arg_otpname;
                p.otptype = otpProfile::OTPtype_TOTP;
                p.description = arg_otpdesc;
                ticonfotpp->saveOTPProfile(p);
                p.setToken(arg_otptoken);

                std::cout << "OTP profile " << arg_otpname.toStdString() << " created!" << std::endl;
            }
        }
        else if(arg_removeprofile)
        {
            if(arg_otpname.isEmpty())
            {
                std::cout << "Argument otpname must be provided to remove an OTP profile!" << std::endl;
                parser.showHelp();
            }
            else
            {
                tiConfOTPProfiles *ticonfotpp = tiConfOTPProfiles::instance();
                otpProfile *profile = ticonfotpp->getOTPProfileByName(arg_otpname);
                if(profile != 0) {
                    ticonfotpp->removeOTPProfileByName(arg_otpname);
                    std::cout << "OTP profile " << arg_otpname.toStdString() << " deleted!" << std::endl;
                } else {
                    std::cout << "OTP profile " << arg_otpname.toStdString() << " not found!" << std::endl;
                }
            }
        }
        else
        {
            std::cout << "No action given, exiting!" << std::endl;
            parser.showHelp();
        }
    }
    else
    {
        QApplication a(argc, argv);
        QApplication::setApplicationName(tinyotp_config::name);
        QApplication::setApplicationVersion(tinyotp_config::version);

        if(isRunningAlready())
        {
            std::cout << "App already running" << std::endl;
            exit(0);
        }

        HelperResult res = Helper::checkSystemPasswordStoreAvailable();
        if(res.status == false)
        {
            std::cout << "No password store available, aborting" << std::endl;
            exit(0);
        }

        MainWindow w;
        return a.exec();
    }
}