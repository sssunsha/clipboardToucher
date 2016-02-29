#-------------------------------------------------
#
# Project created by QtCreator 2016-02-26T10:11:56
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cbt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    httpservicehandler.cpp

HEADERS  += mainwindow.h \
    httpservicehandler.h

FORMS    += mainwindow.ui

RESOURCES += \
    cbt.qrc

include(..//httpserver/httpserver.pri)

DISTFILES += \
    etc/cbt.ini
