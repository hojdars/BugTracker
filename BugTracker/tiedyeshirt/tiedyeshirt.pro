#-------------------------------------------------
#
# Project created by QtCreator 2015-10-30T10:22:19
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tiedyeshirt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dbsetdialog.cpp

HEADERS  += mainwindow.hpp \
    dbsetdialog.hpp

FORMS    += mainwindow.ui \
    dbsetdialog.ui
