#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv/cxcore.h>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void chuli(IplImage* src,IplImage *dst);

    void water(IplImage* src,IplImage *dst);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
