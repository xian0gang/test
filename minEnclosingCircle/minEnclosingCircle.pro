#-------------------------------------------------
#
# Project created by QtCreator 2016-04-27T18:55:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = minEnclosingCircle
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
