#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    void HIST(cv::Mat img);

private:

};

#endif // WIDGET_H
