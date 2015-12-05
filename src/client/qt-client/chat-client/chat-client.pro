#-------------------------------------------------
#
# Project created by QtCreator 2015-11-14T20:30:55
#
#-------------------------------------------------

QT       += core gui widgets network declarative

CONFIG   += C++11

TARGET = Simple-Chat
TEMPLATE = app

SOURCES +=  main.cpp\
            remote_server.cpp \
    chat_core.cpp

QT += qml quick widgets
RESOURCES += qml.qrc
include(deployment.pri)

HEADERS  += \
            remote_server.hpp \
    chat_core.hpp

FORMS    +=
