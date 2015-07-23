#-------------------------------------------------
#
# Project created by QtCreator 2015-07-16T10:51:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dlterm
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cmdhelper.cpp

HEADERS  += mainwindow.h \
    cmdhelper.h

FORMS    += mainwindow.ui
