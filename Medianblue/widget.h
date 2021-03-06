#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void salt(cv::Mat image, int n);
    void pepper(cv::Mat image, int n);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
