#-------------------------------------------------
#
# Project created by QtCreator 2016-07-07T21:25:48
#
#-------------------------------------------------

QT       += core gui

QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = try
TEMPLATE = app

include(./widgettilemanager/widgettilemanager.pri)

SOURCES += main.cpp\
    tilecontroller.cpp


HEADERS  +=    tilecontroller.h

FORMS    += mainwindow.ui \
    tilecontroller.ui

DISTFILES += \
    widgettilemanager/widgettilemanager.pri
