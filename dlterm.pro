#-------------------------------------------------
#
# Project created by QtCreator 2015-07-16T10:51:15
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dlterm
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cmdhelper.cpp \
    cmdhistory.cpp \
    solarized.cpp

HEADERS  += mainwindow.h \
    cmdhelper.h \
    cmdhistory.h \
    solarized.h

FORMS    += mainwindow.ui

DLLIB_DIR = ../DLLib

macx {
    DEFINES += _TTY_POSIX_
    message     ( "Using libDLLib-mac.a" )
    LIBS        += $$DLLIB_DIR/lib/libDLLib-mac.a
    INCLUDEPATH += $$DLLIB_DIR/DLLib
    INCLUDEPATH += $$DLLIB_DIR/DLLib/ui
    INCLUDEPATH += $$DLLIB_DIR/DLLib/zlib
    DEFINES     += APP_DEPTH=0

    DESTDIR = $$join(DESTTYPE,,,-mac)

    #LIBS += /Applications/Xcode.app//Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk/System/Library/Frameworks/Carbon.framework/Carbon
    #LIBS += /Applications/Xcode.app//Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk/System/Library/Frameworks/Foundation.framework/Foundation
}

DISTFILES += \
    solarized-light.qss

RESOURCES +=
