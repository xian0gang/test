#ifndef WIDGET_H
#define WIDGET_H
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <QWidget>
#include <iostream>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void zhifangtu(IplImage *hist_img);
    void ImageBinarization(IplImage *src);
private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
