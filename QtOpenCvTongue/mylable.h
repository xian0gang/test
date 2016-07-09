#ifndef MYLABLE_H
#define MYLABLE_H

#include <QWidget>
#include <QLabel>
#include <QVector>
#include <QImage>
#include <QMouseEvent>
#include <QPainter>
#include <QFileDialog>
#include <QDebug>
#include <QWidget>
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>


class MyLable : public QLabel
{
    Q_OBJECT
public:
    explicit MyLable(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *event);

signals:
    void send_rgb_num(int red,int green,int blue);

protected:
    void clear_list();
    void watersged();
    void set_scaled(QVector<QPoint> list,int num);

public slots:
        void set_filename();
        void set_scaled_button();

private:
        QString set_pic_filename_path;
        QImage* img;
        QImage image;
        QList<int> listx;
        QList<int> listy;
        QVector<QPoint> list;
        QVector<QPoint> list1;
        QVector<QPoint> list2;
        QVector<QPoint> list3;
        int num_list;
        int red;
        int blue;
        int green;
        int x;
        int y;
        int num;

        IplImage *markes;
        IplImage *marker_mask;
        CvPoint prev_pt;

        CvMemStorage* storage;
        IplImage* img0;
        IplImage* draw_img;
        IplImage* wshed;
        IplImage* img_gray;
        IplImage* tongue_body;
        CvRNG rng;

};

#endif // MYLABLE_H
