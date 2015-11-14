#-------------------------------------------------
#
# Project created by QtCreator 2015-11-14T20:30:55
#
#-------------------------------------------------

QT       += core gui
CONFIG += C++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Simple-Chat
TEMPLATE = app

SOURCES +=  main.cpp\
            main_window.cpp \
            login_dialog.cpp \
            remote_server.cpp

HEADERS  += main_window.hpp \
            login_dialog.hpp \
            remote_server.hpp

FORMS    += main_window.ui \
            login_dialog.ui
