#-------------------------------------------------
#
# Project created by QtCreator 2016-06-04T08:44:23
#
#-------------------------------------------------

QT       += core gui \
            sql\
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyChat
TEMPLATE = app


SOURCES += main.cpp\
    login.cpp \
    register.cpp \
    chatwindow.cpp \
    widget.cpp

HEADERS  += \
    login.h \
    register.h \
    chatwindow.h \
    widget.h

FORMS    += \
    login.ui \
    register.ui \
    chatwindow.ui \
    widget.ui

RESOURCES += \
    chat.qrc \
    res.qrc
