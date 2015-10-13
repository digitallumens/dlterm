REQUIRED_QT = 5.5.0
APPLICATION_VERSION = 0.1.0

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dlterm
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    cmdhelper.cpp \
    cmdhistory.cpp \
    solarized.cpp \
    preferencesdialog.cpp \
    interface.cpp \
    bifurcationdialog.cpp \
    emberdialog.cpp \
    globalgateway.cpp

HEADERS  += mainwindow.h \
    cmdhelper.h \
    cmdhistory.h \
    solarized.h \
    preferencesdialog.h \
    interface.h \
    bifurcationdialog.h \
    emberdialog.h \
    globalgateway.h

FORMS    += mainwindow.ui \
    preferencesdialog.ui \
    bifurcationdialog.ui \
    emberdialog.ui

RESOURCES += \
    Icons.qrc \

OTHER_FILES += DL.icns DL.ico

ICON += DL.icns

# Set up destination directory.
CONFIG(debug, debug|release) {
    message( "This is a debug build." )
    DESTTYPE = debug
    DEFINES += QT_DEBUG
} else {
    message ( "This is a release build." )
    DESTTYPE = release
    DEFINES += QT_NO_DEBUG
}

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
}

OBJECTS_DIR = temp
MOC_DIR = temp
UI_DIR = temp
RCC_DIR = temp
