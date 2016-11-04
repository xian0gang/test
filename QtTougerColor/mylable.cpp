#include "mylable.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
MyLable::MyLable(QWidget *parent) : QLabel (parent)
{


}

void MyLable::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    image.load(set_pic_filename_path);
    painter.drawImage(0,0,image,0,0,image.width(),image.height());


    //在选取的点上标号或圈出
    painter.setPen(QPen(Qt::red,1));
    painter.drawEllipse(x-3,y-3,6,6);
}

void MyLable::mousePressEvent(QMouseEvent *event)
{
    //获取点
    x=event->x();
    y=event->y();
qDebug("x:%d",x);
qDebug("y:%d\n",y);

    num = num + 1;
    if(num>9)
        num=1;

//    /*获取rgb值*/
    QRgb rgb=image.pixel(x,y);
    red = qRed(rgb);
    blue = qBlue(rgb);
    green = qGreen(rgb);
qDebug("r:%d",red);
qDebug("g:%d",green);
qDebug("b:%d\n",blue);
    emit send_rgb_num(red,green,blue);

    update();
}


void MyLable::set_filename()
{
    QString xgfilename;
    xgfilename=QFileDialog::getOpenFileName(this,
                                     tr("选择图像"),
                                     "",
                                     tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
    if(xgfilename.isEmpty())
    {
        return;
    }
    else
    {
        set_pic_filename_path=xgfilename;
    }
    qDebug("b:%d\n",green);


    num = 0;
}
