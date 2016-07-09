#ifndef QPICLABLE_H
#define QPICLABLE_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QImage>
#include <QRgb>
#include <QColor>

class QPicLable : public QLabel
{
    Q_OBJECT
public:
    explicit QPicLable(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    
signals:

public slots:
//    void set_filename();

private:
//        QString set_pic_filename_path;
        QImage* img;
        QImage image;
        int red;
        int blue;
        int green;
        int x;
        int y;
        int num;
};

#endif // QPICLABLE_H
