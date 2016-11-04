#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QImage>
#include <QRgb>
#include <QColor>
#include <QButtonGroup>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "mylable.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);

    void rgb_to_xyz_to_Lab(int ,int ,int );
    void Munsell_hvc1();
    void Munsell_hvc2();

    ~Widget();

signals:
    void send_filename(QString filename);

public slots:
    void OpenFile();
//    void onRadioClick();
    void set_rgb_num(int r,int g,int b);
private:
    Ui::Widget *ui;
    QString filename;
    QImage* img;
    QButtonGroup *btnGroupFruits;
    int red;
    int blue;
    int green;
    int x;
    int y;
    double CIE_R;
    double CIE_G;
    double CIE_B;

    double CIE_X;
    double CIE_Y;
    double CIE_Z;
    double CIE_x;
    double CIE_y;
    double munsell_v;
    double slope;
    int munsell_v1;
    double munsell_v_r;

};

#endif // WIDGET_H
