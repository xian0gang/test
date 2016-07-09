#-------------------------------------------------
#
# Project created by QtCreator 2015-12-05T20:12:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QlableOpencv
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    level.cpp

HEADERS  += widget.h \
    level.h

FORMS    += widget.ui

INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv \
                /usr/local/include/opencv2

LIBS += /usr/local/lib/libopencv_*.so
