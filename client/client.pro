#-------------------------------------------------
#
# Project created by QtCreator 2014-03-21T17:22:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        bustrackwindow.cpp\
        mapview.cpp

HEADERS  += bustrackwindow.h\
            mapview.h

FORMS    += bustrackwindow.ui

RESOURCES += bustrack.qrc
