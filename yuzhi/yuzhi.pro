#-------------------------------------------------
#
# Project created by QtCreator 2016-04-10T15:30:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = yuzhi
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui
INCLUDEPATH += /usr/local/include \
				/usr/local/include/opencv \
				/usr/local/include/opencv2

LIBS += /usr/local/lib/libopencv_*.so
