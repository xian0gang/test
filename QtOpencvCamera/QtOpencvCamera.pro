#-------------------------------------------------
#
# Project created by QtCreator 2016-02-27T15:31:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtOpencvCamera
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    vidcodcvicc.cpp

HEADERS  += widget.h \
    vidcodcvicc.h

FORMS    +=
#INCLUDEPATH +=/usr/local/arm/opencv-2.4.10/include \
#						   /usr/local/arm/opencv-2.4.10/include/opencv \
#						 /usr/local/arm/opencv-2.4.10/include/opencv2
#LIBS +=/usr/local/arm/opencv-2.4.10/lib/libopencv_*.so
#INCLUDEPATH += /usr/local/include \
#				/usr/local/include/opencv \
#				/usr/local/include/opencv2

#LIBS += /usr/local/lib/libopencv_*.so
