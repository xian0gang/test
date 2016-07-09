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
    function.cpp

HEADERS  += widget.h \
    mylable.h \
    function.h

FORMS    += widget.ui

#for pc
INCLUDEPATH += /usr/local/include \
				/usr/local/include/opencv \
				/usr/local/include/opencv2

LIBS += /usr/local/lib/libopencv_*.so


#for arm
#INCLUDEPATH +=/usr/local/arm/opencv/include \
#						   /usr/local/arm/opencv/include/opencv \
#						 /usr/local/arm/opencv/include/opencv2
#LIBS +=/usr/local/arm/opencv/lib/libopencv_*.so
