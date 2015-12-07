#-------------------------------------------------
#
# Project created by QtCreator 2015-12-07T13:23:50
#
#-------------------------------------------------

QT       += core gui sql

QMAKE_CXXFLAGS += -std=c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DBConnectPrototype
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    dbsetdialog.cpp

HEADERS  += mainwindow.hpp \
    dbsetdialog.hpp \
    dbsetdialog.hpp

FORMS    += mainwindow.ui \
    dbsetdialog.ui
