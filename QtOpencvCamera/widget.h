#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QImage>
#include <QTimer>
#include "vidcodcvicc.h"
#include <QLabel>
#include <QPaintEvent>

//#include "cv.hpp"
//#include <opencv2/highgui/highgui.hpp>
////#include <vector>
//#include "opencv2/opencv.hpp"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void paintEvent(QPaintEvent *);
    void display_error(QString err);
private slots:
//    void paintEvent(QPaintEvent *);
//    void paintEvent(QPaintEvent *);
////    void display_error(QString err);

private:
    Ui::Widget *ui;
    QPainter *painter;
        QLabel *label;
        QImage *frame;
        //QPixmap *frame;
        QTimer *timer;
        int rs;
        uchar *pp;
        uchar * p;
        unsigned int len;
        int convert_yuv_to_rgb_pixel(int y, int u, int v);
        int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);
        VideoDevice *vd;
};

#endif // WIDGET_H
