#-------------------------------------------------
#
# Project created by QtCreator 2015-12-29T15:01:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtOpenCvTongue
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    mylable.cpp \
    function.cpp \
    videodevice.cpp

HEADERS  += widget.h \
    mylable.h \
    function.h \
    videodevice.h

FORMS    += widget.ui

#for pc
#INCLUDEPATH += /usr/local/include \
#				/usr/local/include/opencv \
#				/usr/local/include/opencv2

#LIBS += /usr/local/lib/libopencv_*.so


#for arm
#INCLUDEPATH +=/usr/local/arm/opencv-2.4.10/include \
#						   /usr/local/arm/opencv-2.4.10/include/opencv \
#						 /usr/local/arm/v/include/opencv2
#LIBS +=/usr/local/arm/opencv-2.4.10/lib/libopencv_*.so
