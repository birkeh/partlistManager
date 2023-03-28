VERSION = "0.0.1.0"

QMAKE_TARGET_COMPANY = "WIN-DESIGN"
QMAKE_TARGET_PRODUCT = "partlistManager"
QMAKE_TARGET_DESCRIPTION = "partlistManager"
QMAKE_TARGET_COPYRIGHT = "(c) 2019 WIN-DESIGN"
QMAKE_TARGET_DOMAIN = "windesign.at"

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32-msvc* {
    contains(QT_ARCH, i386) {
        message("msvc 32-bit")
    } else {
        message("msvc 64-bit")
    }
}

win32-g++ {
    message("mingw")
}

unix {
    message("*nix")
}

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

SOURCES += \
    cdatabase.cpp \
    cdistributor.cpp \
    cdistributorwindow.cpp \
    cmanufacturer.cpp \
    cmanufacturerwindow.cpp \
    cmdisubwindow.cpp \
    common.cpp \
    csplashscreen.cpp \
    cstorage.cpp \
    cstoragecategory.cpp \
    cwidget.cpp \
    main.cpp \
    cmainwindow.cpp

HEADERS += \
    cdatabase.h \
    cdistributor.h \
    cdistributorwindow.h \
    cmainwindow.h \
    cmanufacturer.h \
    cmanufacturerwindow.h \
    cmdisubwindow.h \
    common.h \
    csplashscreen.h \
    cstorage.h \
    cstoragecategory.h \
    cwidget.h

FORMS += \
    cdistributorwindow.ui \
    cmainwindow.ui \
    cmanufacturerwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    partlistmanager.qrc
