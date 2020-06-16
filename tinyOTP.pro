QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    helper.cpp \
    libbaseencode/src/base32.c \
    libbaseencode/src/base64.c \
    libcotp/src/otp.c \
    main.cpp \
    mainwindow.cpp \
    otpprofile.cpp \
    ticonfmain.cpp

HEADERS += \
    config.h \
    helper.h \
    libbaseencode/src/baseencode.h \
    libbaseencode/src/common.h \
    libcotp/src/cotp.h \
    mainwindow.h \
    otpprofile.h \
    ticonfmain.h

TRANSLATIONS += \
    tinyOTP_de_DE.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/libbaseencode/src $$PWD/libcotp/src

unix:!symbian: LIBS += -lcrypto -lpthread -lssl -lutil -lqt5keychain -lgcrypt

RESOURCES += \
    res.qrc
