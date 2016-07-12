#-------------------------------------------------
#
# Project created by QtCreator 2016-07-12T14:51:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtHistogram
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

#for pc
INCLUDEPATH += /usr/local/include \
				/usr/local/include/opencv \
				/usr/local/include/opencv2

LIBS += /usr/local/lib/libopencv_*.so
