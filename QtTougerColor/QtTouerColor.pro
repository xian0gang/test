#-------------------------------------------------
#
# Project created by QtCreator 2015-10-13T01:12:24
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtMouse
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    mylable.cpp

HEADERS  += widget.h \
    mylable.h \
    header.h

FORMS    += widget.ui

RESOURCES += \
    images.qrc

#LIBS += -lsqlite3
