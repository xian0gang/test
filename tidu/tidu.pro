#-------------------------------------------------
#
# Project created by QtCreator 2016-07-31T16:29:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tidu
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui

#for pc
INCLUDEPATH += /usr/local/include \
				/usr/local/include/opencv \
				/usr/local/include/opencv2

LIBS += /usr/local/lib/libopencv_*.so
