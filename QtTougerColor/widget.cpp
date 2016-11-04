#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <qmath.h>
#include <QtMath>
#include "header.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    /*让图片自适应lable的大小，图片会被拉伸等*/
    //ui->label->setScaledContents(true);

    x=0;
    y=0;

    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
//    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("cie"); //连接一个已存在的数据
    db.setHostName("localhost");//指定数据库服务器名称
//    db.setHostName("10.14.17.228");//指定数据库服务器名称
//    db.setHostName("169.254.142.11");//指定数据库服务器名称
    db.setUserName("xiangang");      //设置登录名
    db.setPassword("656758"); //设置登录密码

    //打开数据库连接
    if(db.open())
    {
        qDebug()<<"QSQLITE open success!";
    }
    else
    {
        qDebug()<<"QSQLITE open error!";
    }



    //open file
   connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(OpenFile()));
    //set color
   connect(ui->label,SIGNAL(send_rgb_num(int,int,int)),this,SLOT(set_rgb_num(int,int,int)));
   //send filename
   connect(this,SIGNAL(send_filename(QString)),ui->label,SLOT(set_filename()));
}

void Widget::Munsell_hvc1()
{
    QSqlQuery query;
/*************************Munsell_hvc1************************************/


    QString str;

    str = "select * from v"+QString("%1").arg(munsell_v1);

    query.exec(str);
    qDebug()<<"str"<<str;
    double min_temp = 100.00;


    //获取第一个点
    while(query.next())
    {
        double x = query.value(1).toDouble();
        double y = query.value(2).toDouble();
        double min;
        //距离最近的点
        min = sqrt(qPow((y-CIE_y),2)+qPow((x-CIE_x),2));
        if(min < min_temp)
        {

            min_temp = min;
            num = query.value(0).toString();
            str_x = query.value(1).toString();
            str_y = query.value(2).toString();
            str_v = query.value(3).toString();
            str_c = query.value(4).toString();
            str_h = query.value(5).toString();
            str_color = query.value(6).toString();
            str_slope = query.value(7).toString();
        }
    }

    double d_y = str_y.toDouble();
    qDebug("d_y:%f",d_y);

    //根据斜率获取第二个点
    if(slope>str_slope.toDouble())
    {
        double h = str_h.toDouble();
        qDebug()<<"h is <10:"<<h;
        if(h<10.0)
        {
            h = h + 2.5;
        }

        QString str2 = "select * from v"+QString("%1").arg(munsell_v1)+
                " where h="+QString("%1").arg(h)+
                " and c="+str_c +
                " and color='"+str_color+"'";
        query.exec(str2);
        qDebug()<<"str2:"<<str2;

        if(query.first())
        {
            num2 = query.value(0).toString();
            str_x2 = query.value(1).toString();
            str_y2 = query.value(2).toString();
            str_v2 = query.value(3).toString();
            str_c2 = query.value(4).toString();
            str_h2 = query.value(5).toString();
            str_color2 = query.value(6).toString();
            str_slope2 = query.value(7).toString();
        }
    }
    else
    {
        double h = str_h.toDouble();
        qDebug()<<"h is >0:"<<h;
        if(h>0.0)
        {
            h = h - 2.5;
        }

        QString str2 = "select * from v"+QString("%1").arg(munsell_v1)+
                " where h="+QString("%1").arg(h)+
                " and c="+str_c +
                " and color='"+str_color+"'";
        query.exec(str2);
        qDebug()<<"str2:"<<str2;

        if(query.first())
        {
            num2 = query.value(0).toString();
            str_x2 = query.value(1).toString();
            str_y2 = query.value(2).toString();
            str_v2 = query.value(3).toString();
            str_c2 = query.value(4).toString();
            str_h2 = query.value(5).toString();
            str_color2 = query.value(6).toString();
            str_slope2 = query.value(7).toString();
        }
    }

/***********判断最近的点是否符合**************************/
    if(slope>str_slope.toDouble())
    {
        if(slope>str_slope2.toDouble())
        {
            qDebug()<<"超出一个范围，从新选择第一个和第二个点";
            str_x = str_x2;
            str_y = str_y2;
            str_v = str_v2;
            str_c = str_c2;
            str_h = str_h2;
            str_color = str_color2;
            str_slope = str_slope2;

            double h = str_h.toDouble();

            if(h<10.0)
            {
                h = h + 2.5;
            }
            qDebug()<<"h is <10:"<<h;

            QString str2 = "select * from v"+QString("%1").arg(munsell_v1)+
                    " where h="+QString("%1").arg(h)+
                    " and c="+str_c +
                    " and color='"+str_color+"'";
            query.exec(str2);
            if(query.first())
            {
                str_x2 = query.value(1).toString();
                str_y2 = query.value(2).toString();
                str_v2 = query.value(3).toString();
                str_c2 = query.value(4).toString();
                str_h2 = query.value(5).toString();
                str_color2 = query.value(6).toString();
                str_slope2 = query.value(7).toString();
            }
        }
    }
    else
    {
        if(slope<str_slope2.toDouble())
        {
            qDebug()<<"超出一个范围，从新选择第一个和第二个点";
            str_x = str_x2;
            str_y = str_y2;
            str_v = str_v2;
            str_c = str_c2;
            str_h = str_h2;
            str_color = str_color2;
            str_slope = str_slope2;

            double h = str_h.toDouble();

            if(h>0.0)
            {
                h = h - 2.5;
            }

            QString str2 = "select * from v"+QString("%1").arg(munsell_v1)+
                    " where h="+QString("%1").arg(h)+
                    " and c="+str_c +
                    " and color='"+str_color+"'";
            query.exec(str2);
            if(query.first())
            {
                str_x2 = query.value(1).toString();
                str_y2 = query.value(2).toString();
                str_v2 = query.value(3).toString();
                str_c2 = query.value(4).toString();
                str_h2 = query.value(5).toString();
                str_color2 = query.value(6).toString();
                str_slope2 = query.value(7).toString();
            }
        }
    }
/*************************************/

    qDebug("str_x:%f",str_x.toDouble());
    qDebug("str_y:%f",str_y.toDouble());
    qDebug("str_v:%f",str_v.toDouble());
    qDebug("str_c:%f",str_c.toDouble());
    qDebug("str_h:%f",str_h.toDouble());
    qDebug("str_color:%f\n",str_color.toDouble());

    qDebug("str_x2:%f",str_x2.toDouble());
    qDebug("str_y2:%f",str_y2.toDouble());
    qDebug("str_v2:%f",str_v2.toDouble());
    qDebug("str_c2:%f",str_c2.toDouble());
    qDebug("str_h2:%f",str_h2.toDouble());
    qDebug("str_color2:%f\n",str_color2.toDouble());
       /*********************/
    double x_a,x_e,y_a,y_e,x1,y1;

    x_a = str_x.toDouble()-0.3101;
    y_a = str_y.toDouble()-0.3163;
    x_e = str_x2.toDouble()-0.3101;
    y_e = str_y2.toDouble()-0.3163;
    x1 = (y_a-((y_e-y_a)/(x_e-x_a))*x_a)/(slope-(y_e-y_a)/(x_e-x_a));
    y1 = slope*x1;
    qDebug("slope:%f",slope);
    qDebug("第一个点(x_d:%f,y_d:%f),(x_g:%f,x_g:%f),(x1:%f,y1:%f)",x_a,y_a,x_e,y_e,x1,y1);

        /**********************/

        //v+2 在左边
    if((CIE_x > qAbs(x1+0.3101)) || (CIE_y > qAbs(y1+0.3163)))
    {
        //直接找出左边的两个点
        //第三个点
        str_c3 = QString("%1").arg(str_c.toDouble()+2.0);
        str_c3_3 = QString("%1").arg(str_c_1.toDouble()+2.0);

        QString str3 = "select * from v"+QString("%1").arg(munsell_v1)+
                " where h="+str_h+
                " and c="+str_c3 +
                " and color='"+str_color+"'";
        query.exec(str3);
        qDebug()<<"str3:"<<str3;

        if(query.first())
        {
            num3 = query.value(0).toString();
            str_x3 = query.value(1).toString();
            str_y3 = query.value(2).toString();
            str_v3 = query.value(3).toString();
            str_c3 = query.value(4).toString();
            str_h3 = query.value(5).toString();
            str_color3 = query.value(6).toString();
            str_slope3 = query.value(7).toString();

            qDebug()<<"x3"<<query.value(1).toString();
            qDebug()<<"y3"<<query.value(2).toString();
            qDebug()<<"v3"<<query.value(3).toString();
            qDebug()<<"c3"<<query.value(4).toString();
            qDebug()<<"h3"<<query.value(5).toString();
            qDebug()<<"color3"<<query.value(6).toString();
        }

            //第四个点
        QString str4 = "select * from v"+QString("%1").arg(munsell_v1)+
                " where h="+str_h2+
                " and c="+str_c3 +
                " and color='"+str_color2+"'";
        query.exec(str4);
        qDebug()<<"str4:"<<str4;

        if(query.first())
        {
            num4 = query.value(0).toString();
            str_x4 = query.value(1).toString();
            str_y4 = query.value(2).toString();
            str_v4 = query.value(3).toString();
            str_c4 = query.value(4).toString();
            str_h4 = query.value(5).toString();

            str_color4 = query.value(6).toString();
            str_slope4 = query.value(7).toString();
//                qDebug()<<"num4"<<query.value(0).toString();
            qDebug()<<"x4"<<query.value(1).toString();
            qDebug()<<"y4"<<query.value(2).toString();
            qDebug()<<"v4"<<query.value(3).toString();
            qDebug()<<"c4"<<query.value(4).toString();
            qDebug()<<"h4"<<query.value(5).toString();
            qDebug()<<"color4"<<query.value(6).toString();
        }

    }
    else
    {
        //v-2 在右边
        if((CIE_x < qAbs(x1+0.3101)) || (CIE_y < qAbs(y1+0.3163)))
        {
            //直接找出右边的两个点
            //第三个点
            str_c3 = QString("%1").arg(str_c.toDouble() - 2.0);
            str_c3_3 = QString("%1").arg(str_c_1.toDouble() - 2.0);

            QString str3 = "select * from v"+QString("%1").arg(munsell_v1)+
                    " where h="+str_h+
                    " and c="+str_c3 +
                    " and color='"+str_color+"'";
            query.exec(str3);
            qDebug()<<"str3:"<<str3;

            if(query.first())
            {
                num3 = query.value(0).toString();
                str_x3 = query.value(1).toString();
                str_y3 = query.value(2).toString();
                str_v3 = query.value(3).toString();
                str_c3 = query.value(4).toString();
                str_h3 = query.value(5).toString();
                str_color3 = query.value(6).toString();
                str_slope3 = query.value(7).toString();

                qDebug()<<"x3"<<query.value(1).toString();
                qDebug()<<"y3"<<query.value(2).toString();
                qDebug()<<"v3"<<query.value(3).toString();
                qDebug()<<"c3"<<query.value(4).toString();
                qDebug()<<"h3"<<query.value(5).toString();
                qDebug()<<"color3"<<query.value(6).toString();
            }

                    //第四个点
            QString str4 = "select * from v"+QString("%1").arg(munsell_v1)+
                    " where h="+str_h2+
                    " and c="+str_c3 +
                    " and color='"+str_color2+"'";
            query.exec(str4);
            qDebug()<<"str3:"<<str4;

            if(query.first())
            {
                num4 = query.value(0).toString();
                str_x4 = query.value(1).toString();
                str_y4 = query.value(2).toString();
                str_v4 = query.value(3).toString();
                str_c4 = query.value(4).toString();
                str_h4 = query.value(5).toString();

                qDebug()<<"x4"<<query.value(1).toString();
                qDebug()<<"y4"<<query.value(2).toString();
                qDebug()<<"v4"<<query.value(3).toString();
                qDebug()<<"c4"<<query.value(4).toString();
                qDebug()<<"h4"<<query.value(5).toString();
                qDebug()<<"color4"<<query.value(6).toString();
            }
        }

    }
    /****************************************************************/
    /*******计算第二个点**********/
    double x_d,x_g,y_d,y_g,x2,y2;
    x_d = str_x3.toDouble()-0.3101;
    y_d = str_y3.toDouble()-0.3163;
    x_g = str_x4.toDouble()-0.3101;
    y_g = str_y4.toDouble()-0.3163;
    x2 = (y_g-((y_d-y_g)/(x_d-x_g))*x_g)/(slope-(y_d-y_g)/(x_d-x_g));
    y2 = slope*x2;
    qDebug("(x_d:%f,y_d:%f),(x_g:%f,yg:%f),(x2:%f,x2:%f)",x_d,y_d,x_g,y_g,x2,y2);

    //i=0 第一层 计算饱和度c的值
    if(str_c.toDouble()<str_c3.toDouble())
    {
        double r;
        //计算v-1的c值
        r =qSqrt(qPow((CIE_y-0.3163-y1),2)+qPow((CIE_x-0.3101-x1),2))/qSqrt(qPow(qAbs(y2-y1),2)+qPow(qAbs(x2-x1),2));
         really_c = str_c.toDouble() + 2*r;

        //计算色调H值
        if(str_h.toDouble()<str_h2.toDouble())
        {
            double r2;
            r2 =qSqrt(qPow(qAbs(y1+0.3163-str_y.toDouble()),2)+qPow(qAbs(x1+0.3101-str_x.toDouble()),2))/qSqrt(qPow(qAbs(str_y2.toDouble()-str_y.toDouble()),2)+qPow(qAbs(str_x2.toDouble()-str_x.toDouble()),2));
            really_h = str_h.toDouble() + 2.5*r2;
             qDebug("r2_a:%f",r2);
             qDebug("str_h_1:%f",str_h_1.toDouble());
        }
        else
        {
            double r2;
            r2 =qSqrt(qPow(qAbs(y1+0.3163-str_y2.toDouble()),2)+qPow(qAbs(x1+0.3101-str_x2.toDouble()),2))/qSqrt(qPow(qAbs(str_y2.toDouble()-str_y.toDouble()),2)+qPow(qAbs(str_x2.toDouble()-str_x.toDouble()),2));
             really_h = str_h2.toDouble() + 2.5*r2;
             qDebug("r2_b:%f",r2);
             qDebug("str_h2:%f",str_h_1.toDouble());
        }
    }
    else
    {
        double r;
        r =qSqrt(qPow(qAbs(CIE_y-0.3163-y2),2)+qPow(qAbs(CIE_x-0.3101-x2),2))/qSqrt(qPow(qAbs(y2-y1),2)+qPow(qAbs(x2-x1),2));
         really_c = str_c3.toDouble() + 2*r;
        ui->label_27->setText(QString("%1").arg(really_c,0,'f',2));

        //计算色调H值
        if(str_h.toDouble()<str_h2.toDouble())
        {
            double r2;
            r2 =qSqrt(qPow(qAbs(y2+0.3163-str_y3.toDouble()),2)+qPow(qAbs(x2+0.3101-str_x3.toDouble()),2))/qSqrt(qPow(qAbs(str_y4.toDouble()-str_y3.toDouble()),2)+qPow(qAbs(str_x4.toDouble()-str_x3.toDouble()),2));
            really_h = str_h.toDouble() + 2.5*r2;
            qDebug("r2_c:%f",r2);
            qDebug("str_h:%f",str_h.toDouble());
        }
        else
        {
            double r2;
            r2 =qSqrt(qPow(qAbs(y2+0.3163-str_y4.toDouble()),2)+qPow(qAbs(x2+0.3101-str_x4.toDouble()),2))/qSqrt(qPow(qAbs(str_y4.toDouble()-str_y3.toDouble()),2)+qPow(qAbs(str_x4.toDouble()-str_x3.toDouble()),2));
            really_h = str_h2.toDouble() + 2.5*r2;
             qDebug("r2_d:%f",r2);
             qDebug("str_h2:%f",str_h_1.toDouble());
        }
    }
}

void Widget::Munsell_hvc2()
{
    QSqlQuery query;
qDebug()<<"/***********************Munsell_hvc2**************************************/";


    QString str;

    str = "select * from v"+QString("%1").arg(munsell_v1+1);

    query.exec(str);
    qDebug()<<"str"<<str;
    double min_temp = 100.00;


    //获取第一个点
    while(query.next())
    {
        double x = query.value(1).toDouble();
        double y = query.value(2).toDouble();
        double min;
        //距离最近的点
        min = sqrt(qPow((y-CIE_y),2)+qPow((x-CIE_x),2));
        if(min < min_temp)
        {

            min_temp = min;
            num = query.value(0).toString();
            str_x_1 = query.value(1).toString();
            str_y_1 = query.value(2).toString();
            str_v_1 = query.value(3).toString();
            str_c_1 = query.value(4).toString();
            str_h_1 = query.value(5).toString();
            str_color_1 = query.value(6).toString();
            str_slope_1 = query.value(7).toString();
        }
    }

    double d_y = str_y_1.toDouble();
    qDebug("d_y:%f",d_y);

    //根据斜率获取第二个点
    //此处也要分类讨论
    if(slope>str_slope_1.toDouble())
    {
        double h = str_h_1.toDouble();
        qDebug()<<"h is <10:"<<h;
        if(h<10.0)
        {
            h = h + 2.5;
        }

        QString str2 = "select * from v"+QString("%1").arg(munsell_v1+1)+
                " where h="+QString("%1").arg(h)+
                " and c="+str_c_1 +
                " and color='"+str_color_1+"'";
        query.exec(str2);
        qDebug()<<"str2:"<<str2;

        if(query.first())
        {
            str_x2_2 = query.value(1).toString();
            str_y2_2 = query.value(2).toString();
            str_v2_2 = query.value(3).toString();
            str_c2_2 = query.value(4).toString();
            str_h2_2 = query.value(5).toString();
            str_color2_2 = query.value(6).toString();
            str_slope2_2 = query.value(7).toString();
        }
    }
    else
    {
        double h = str_h_1.toDouble();
        qDebug()<<"h is >0:"<<h;
        if(h>0.0)
        {
            h = h - 2.5;
        }

        QString str2 = "select * from v"+QString("%1").arg(munsell_v1+1)+
                " where h="+QString("%1").arg(h)+
                " and c="+str_c_1 +
                " and color='"+str_color_1+"'";
        query.exec(str2);
        qDebug()<<"str2:"<<str2;

        if(query.first())
        {
            num2 = query.value(0).toString();
            str_x2_2 = query.value(1).toString();
            str_y2_2 = query.value(2).toString();
            str_v2_2 = query.value(3).toString();
            str_c2_2 = query.value(4).toString();
            str_h2_2 = query.value(5).toString();
            str_color2_2 = query.value(6).toString();
            str_slope2_2 = query.value(7).toString();
        }
    }

/*******************判断第一个点是否符合*********************************/
    if(slope>str_slope_1.toDouble())
    {
        if(slope>str_slope2_2.toDouble())
        {
            qDebug()<<"超出一个范围，从新选择第一个和第二个点";
            str_x_1 = str_x2_2;
            str_y_1 = str_y2_2;
            str_v_1 = str_v2_2;
            str_c_1 = str_c2_2;
            str_h_1 = str_h2_2;
            str_color_1 = str_color2_2;
            str_slope_1 = str_slope2_2;

            double h = str_h_1.toDouble();
            if(h<10.0)
            {
                h = h + 2.5;
            }

            QString str2 = "select * from v"+QString("%1").arg(munsell_v1+1)+
                    " where h="+QString("%1").arg(h)+
                    " and c="+str_c_1 +
                    " and color='"+str_color_1+"'";
            query.exec(str2);
            if(query.first())
            {
                str_x2_2 = query.value(1).toString();
                str_y2_2 = query.value(2).toString();
                str_v2_2 = query.value(3).toString();
                str_c2_2 = query.value(4).toString();
                str_h2_2 = query.value(5).toString();
                str_color2_2 = query.value(6).toString();
                str_slope2_2 = query.value(7).toString();
            }
        }
    }
    else
    {
        if(slope<str_slope2_2.toDouble())
        {
            qDebug()<<"超出一个范围，从新选择第一个和第二个点";
            str_x_1 = str_x2_2;
            str_y_1 = str_y2_2;
            str_v_1 = str_v2_2;
            str_c_1 = str_c2_2;
            str_h_1 = str_h2_2;
            str_color_1 = str_color2_2;
            str_slope_1 = str_slope2_2;

            double h = str_h_1.toDouble();
            if(h>0.0)
            {
                h = h - 2.5;
            }

            QString str2 = "select * from v"+QString("%1").arg(munsell_v1+1)+
                    " where h="+QString("%1").arg(h)+
                    " and c="+str_c_1 +
                    " and color='"+str_color_1+"'";
            query.exec(str2);
            qDebug()<<"str2:"<<str2;
            if(query.first())
            {
                num2 = query.value(0).toString();
                str_x2_2 = query.value(1).toString();
                str_y2_2 = query.value(2).toString();
                str_v2_2 = query.value(3).toString();
                str_c2_2 = query.value(4).toString();
                str_h2_2 = query.value(5).toString();
                str_color2_2 = query.value(6).toString();
                str_slope2_2 = query.value(7).toString();
            }
        }
    }
/****************************************************************/
        qDebug("str_x:%f",str_x_1.toDouble());
        qDebug("str_y:%f",str_y_1.toDouble());
        qDebug("str_v:%f",str_v_1.toDouble());
        qDebug("str_c:%f",str_c_1.toDouble());
        qDebug("str_h:%f",str_h_1.toDouble());
        qDebug("str_color:%f\n",str_color_1.toDouble());

        qDebug("str_x2:%f",str_x2_2.toDouble());
        qDebug("str_y2:%f",str_y2_2.toDouble());
        qDebug("str_v2:%f",str_v2_2.toDouble());
        qDebug("str_c2:%f",str_c2_2.toDouble());
        qDebug("str_h2:%f",str_h2_2.toDouble());
        qDebug("str_color2:%f\n",str_color2_2.toDouble());

       /*********************/
    double x_a,x_e,y_a,y_e,x1,y1;

    x_a = str_x_1.toDouble()-0.3101;
    y_a = str_y_1.toDouble()-0.3163;
    x_e = str_x2_2.toDouble()-0.3101;
    y_e = str_y2_2.toDouble()-0.3163;
    x1 = (y_a-((y_e-y_a)/(x_e-x_a))*x_a)/(slope-(y_e-y_a)/(x_e-x_a));
    y1 = slope*x1;

        /**********************/

        //v+2 在左边
    if((CIE_x > qAbs(x1+0.3101)) || (CIE_y > qAbs(y1+0.3163)))
    {
        //直接找出左边的两个点
        //第三个点
//        str_c3 = QString("%1").arg(str_c_1.toDouble()+2.0);
        str_c3_3 = QString("%1").arg(str_c_1.toDouble()+2.0);

        QString str3 = "select * from v"+QString("%1").arg(munsell_v1+1)+
                " where h="+str_h_1+
                " and c="+str_c3_3 +
                " and color='"+str_color_1+"'";
        query.exec(str3);
        qDebug()<<"str3:"<<str3;

        if(query.first())
        {
//            num3 = query.value(0).toString();
            str_x3_3 = query.value(1).toString();
            str_y3_3 = query.value(2).toString();
            str_v3_3 = query.value(3).toString();
            str_c3_3 = query.value(4).toString();
            str_h3_3 = query.value(5).toString();
            str_color3_3 = query.value(6).toString();
            str_slope3_3 = query.value(7).toString();

            qDebug()<<"x3"<<query.value(1).toString();
            qDebug()<<"y3"<<query.value(2).toString();
            qDebug()<<"v3"<<query.value(3).toString();
            qDebug()<<"c3"<<query.value(4).toString();
            qDebug()<<"h3"<<query.value(5).toString();
            qDebug()<<"color3"<<query.value(6).toString();
        }

            //第四个点
        QString str4 = "select * from v"+QString("%1").arg(munsell_v1+1)+
                " where h="+str_h2_2+
                " and c="+str_c3_3 +
                " and color='"+str_color2_2+"'";
        query.exec(str4);
        qDebug()<<"str4:"<<str4;

        if(query.first())
        {
            num4_4 = query.value(0).toString();
            str_x4_4 = query.value(1).toString();
            str_y4_4 = query.value(2).toString();
            str_v4_4 = query.value(3).toString();
            str_c4_4 = query.value(4).toString();
            str_h4_4 = query.value(5).toString();

            str_color4_4 = query.value(6).toString();
            str_slope4_4 = query.value(7).toString();
//                qDebug()<<"num4"<<query.value(0).toString();
            qDebug()<<"x4"<<query.value(1).toString();
            qDebug()<<"y4"<<query.value(2).toString();
            qDebug()<<"v4"<<query.value(3).toString();
            qDebug()<<"c4"<<query.value(4).toString();
            qDebug()<<"h4"<<query.value(5).toString();
            qDebug()<<"color4"<<query.value(6).toString();
        }

    }
    else
    {
        //v-2 在右边
        if((CIE_x < qAbs(x1+0.3101)) || (CIE_y < qAbs(y1+0.3163)))
        {
            //直接找出右边的两个点
            //第三个点
            str_c3_3 = QString("%1").arg(str_c_1.toDouble() - 2.0);
//            str_c3_3 = QString("%1").arg(str_c_1.toDouble() - 2.0);

            QString str3 = "select * from v"+QString("%1").arg(munsell_v1+1)+
                    " where h="+str_h_1+
                    " and c="+str_c3_3 +
                    " and color='"+str_color_1+"'";
            query.exec(str3);
            qDebug()<<"str3:"<<str3;

            if(query.first())
            {
//                num3 = query.value(0).toString();
                str_x3_3 = query.value(1).toString();
                str_y3_3 = query.value(2).toString();
                str_v3_3 = query.value(3).toString();
                str_c3_3 = query.value(4).toString();
                str_h3_3 = query.value(5).toString();
                str_color3_3 = query.value(6).toString();
                str_slope3_3 = query.value(7).toString();

                qDebug()<<"x3"<<query.value(1).toString();
                qDebug()<<"y3"<<query.value(2).toString();
                qDebug()<<"v3"<<query.value(3).toString();
                qDebug()<<"c3"<<query.value(4).toString();
                qDebug()<<"h3"<<query.value(5).toString();
                qDebug()<<"color3"<<query.value(6).toString();
            }

                    //第四个点
            QString str4 = "select * from v"+QString("%1").arg(munsell_v1+1)+
                    " where h="+str_h2_2+
                    " and c="+str_c3_3 +
                    " and color='"+str_color2_2+"'";
            query.exec(str4);
            qDebug()<<"str3:"<<str4;

            if(query.first())
            {
                num4_4 = query.value(0).toString();
                str_x4_4 = query.value(1).toString();
                str_y4_4 = query.value(2).toString();
                str_v4_4 = query.value(3).toString();
                str_c4_4 = query.value(4).toString();
                str_h4_4 = query.value(5).toString();

                str_color4_4 = query.value(6).toString();
                str_slope4_4 = query.value(7).toString();

                qDebug()<<"x4"<<query.value(1).toString();
                qDebug()<<"y4"<<query.value(2).toString();
                qDebug()<<"v4"<<query.value(3).toString();
                qDebug()<<"c4"<<query.value(4).toString();
                qDebug()<<"h4"<<query.value(5).toString();
                qDebug()<<"color4"<<query.value(6).toString();
            }
        }

    }
    /****************************************************************/
    /*******计算第二个点**********/
        double x_d,x_g,y_d,y_g,x2,y2;
        x_d = str_x3_3.toDouble()-0.3101;
        y_d = str_y3_3.toDouble()-0.3163;
        x_g = str_x4_4.toDouble()-0.3101;
        y_g = str_y4_4.toDouble()-0.3163;
        x2 = (y_g-((y_d-y_g)/(x_d-x_g))*x_g)/(slope-(y_d-y_g)/(x_d-x_g));
        y2 = slope*x2;

        //i=0 第一层 计算饱和度c的值
        if(str_c_1.toDouble()<str_c3_3.toDouble())
        {
            double r;
            //计算v-1的c值
            r =qSqrt(qPow((CIE_y-0.3163-y1),2)+qPow((CIE_x-0.3101-x1),2))/qSqrt(qPow(qAbs(y2-y1),2)+qPow(qAbs(x2-x1),2));
             really_c_2 = str_c_1.toDouble() + 2*r;
             qDebug("r:%f",r);
             qDebug("str_c_1:%f",str_c_1.toDouble());
//            ui->label_27->setText(QString("%1").arg(really_c,0,'f',2));

            //计算色调H值
            if(str_h_1.toDouble()<str_h2_2.toDouble())
            {
                double r2;
                r2 =qSqrt(qPow(qAbs(y1+0.3163-str_y_1.toDouble()),2)+qPow(qAbs(x1+0.3101-str_x_1.toDouble()),2))/qSqrt(qPow(qAbs(str_y2_2.toDouble()-str_y_1.toDouble()),2)+qPow(qAbs(str_x2_2.toDouble()-str_x_1.toDouble()),2));
                really_h_2 = str_h_1.toDouble() + 2.5*r2;
//                ui->label_31->setText(QString("%1").arg(really_h,0,'f',2));
                 qDebug("r2_a:%f",r2);
                 qDebug("str_h_1:%f",str_h_1.toDouble());
            }
            else
            {
                double r2;
                r2 =qSqrt(qPow(qAbs(y1+0.3163-str_y2_2.toDouble()),2)+qPow(qAbs(x1+0.3101-str_x2_2.toDouble()),2))/qSqrt(qPow(qAbs(str_y2_2.toDouble()-str_y_1.toDouble()),2)+qPow(qAbs(str_x2_2.toDouble()-str_x_1.toDouble()),2));
                 really_h_2 = str_h2_2.toDouble() + 2.5*r2;
//                ui->label_31->setText(QString("%1").arg(really_h,0,'f',2));
                 qDebug("r2_b:%f",r2);
                 qDebug("str_h2:%f",str_h2_2.toDouble());
            }
        }
        else
        {
            double r;
            r =qSqrt(qPow(qAbs(CIE_y-0.3163-y2),2)+qPow(qAbs(CIE_x-0.3101-x2),2))/qSqrt(qPow(qAbs(y2-y1),2)+qPow(qAbs(x2-x1),2));
             really_c_2 = str_c3_3.toDouble() + 2*r;
             qDebug("r:%f",r);
             qDebug("str_c3_3:%f",str_c3_3.toDouble());
//            ui->label_27->setText(QString("%1").arg(really_c_2,0,'f',2));

            //计算色调H值
            if(str_h_1.toDouble()<str_h2_2.toDouble())
            {
                double r2;
                r2 =qSqrt(qPow(qAbs(y2+0.3163-str_y3_3.toDouble()),2)+qPow(qAbs(x2+0.3101-str_x3_3.toDouble()),2))/qSqrt(qPow(qAbs(str_y4_4.toDouble()-str_y3_3.toDouble()),2)+qPow(qAbs(str_x4_4.toDouble()-str_x3_3.toDouble()),2));
                really_h_2 = str_h_1.toDouble() + 2.5*r2;
//                ui->label_31->setText(QString("%1").arg(really_h,0,'f',2));
                qDebug("r2_c:%f",r2);
                qDebug("str_h:%f",str_h_1.toDouble());
            }
            else
            {
                double r2;
                r2 =qSqrt(qPow(qAbs(y2+0.3163-str_y4_4.toDouble()),2)+qPow(qAbs(x2+0.3101-str_x4_4.toDouble()),2))/qSqrt(qPow(qAbs(str_y4_4.toDouble()-str_y3_3.toDouble()),2)+qPow(qAbs(str_x4_4.toDouble()-str_x3_3.toDouble()),2));
                really_h_2 = str_h2_2.toDouble() + 2.5*r2;
//                ui->label_31->setText(QString("%1").arg(really_h,0,'f',2));
                 qDebug("r2_d:%f",r2);
                 qDebug("str_h2:%f",str_h2_2.toDouble());
            }
        }

    qDebug()<<"really_c:"<<really_c;
    qDebug()<<"really_c_2:"<<really_c_2<<"\n";
    qDebug()<<"really_h:"<<really_h;
    qDebug()<<"really_h_2:"<<really_h_2<<"";

    if(really_c_2>really_c)
    {
        really_c = really_c + (qAbs(really_c_2 - really_c))*munsell_v_r;
    }
    else
    {
        really_c = really_c_2 + (qAbs(really_c_2 - really_c))*munsell_v_r;
    }

    if(really_h < really_h_2)
    {
        really_h = really_h + (qAbs(really_h_2 - really_h))*munsell_v_r;
    }
    else
    {
        really_h = really_h_2 + (qAbs(really_h_2 - really_h))*munsell_v_r;
    }

    //C
    ui->label_27->setText(QString("%1").arg(really_c,0,'f',2));

    ui->label_31->setText(QString("%1").arg(really_h,0,'f',2)+str_color);



    qDebug()<<"munsell_v_r:"<<munsell_v_r;
}

void Widget::OpenFile()
{

    emit send_filename(filename);
}

void Widget::set_rgb_num(int r, int g, int b)
{
    ui->label_5->setText(QString("%1").arg(r));
    ui->label_6->setText(QString("%1").arg(g));
    ui->label_7->setText(QString("%1").arg(b));

    /*在lable中显示颜色*/
    QPalette palette;
    ui->label_8->setAutoFillBackground(true);
    palette.setColor(QPalette::Background, QColor (r, g , b, 255));
    ui->label_8->setPalette(palette);

    rgb_to_xyz_to_Lab(r,g,b);

}


void Widget::rgb_to_xyz_to_Lab(int r, int g, int b)
{
    double gamma_r,gamma_g,gamma_b;

    gamma_r = r/255.00;
    gamma_g = g/255.00;
    gamma_b = b/255.00;

    //CIE-R
    if(gamma_r > 0.04045)
    {
        CIE_R = qPow(((gamma_r + 0.055)/1.055),2.4);
    }
    else {

        CIE_R = gamma_r/12.92;
    }

    //CIE-G
    if(gamma_g > 0.04045)
    {
        CIE_G = qPow(((gamma_g + 0.055)/1.055),2.4);
    }
    else {

        CIE_G = gamma_g/12.92;
    }

    //CIE-B
    if(gamma_b > 0.04045)
    {
        CIE_B = qPow(((gamma_b + 0.055)/1.055),2.4);
    }
    else {

        CIE_B = gamma_b/12.92;
    }

    CIE_X = (0.4124*CIE_R+0.3576*CIE_G+0.1805*CIE_B)*100;
    CIE_Y = (0.2126*CIE_R+0.7152*CIE_G+0.0722*CIE_B)*100;
    CIE_Z = (0.0193*CIE_R+0.1192*CIE_G+0.9505*CIE_B)*100;

    ui->label_10->setText(QString("%1").arg(CIE_X,0,'f',3));
    ui->label_9->setText(QString("%1").arg(CIE_Y,0,'f',3));
    ui->label_12->setText(QString("%1").arg(CIE_Z,0,'f',3));


    CIE_x = CIE_X/(CIE_X+CIE_Y+CIE_Z);
    CIE_y = CIE_Y/(CIE_X+CIE_Y+CIE_Z);

    ui->label_16->setText(QString("%1").arg(CIE_Y,0,'f',3));
    ui->label_15->setText(QString("%1").arg(CIE_x,0,'f',3));
    ui->label_18->setText(QString("%1").arg(CIE_y,0,'f',3));

    double CIE_L,CIE_a,CIE_b;
    CIE_L = 25.00*qPow(CIE_Y,0.333333)-16.00;
    CIE_a = 109.5*qPow(CIE_X,0.333333)-107.7*qPow(CIE_Y,0.333333);
    CIE_b = 43.0*qPow(CIE_Y,0.333333)-41.92*qPow(CIE_Z,0.333333);

//    ui->label_21->setText(QString("%1").arg(CIE_L,0,'f',3));
//    ui->label_25->setText(QString("%1").arg(CIE_a,0,'f',3));
//    ui->label_22->setText(QString("%1").arg(CIE_b,0,'f',3));

//    munsell_v = qPow((1.474*CIE_Y-0.00474*qPow(CIE_Y,2)),0.5);
    munsell_v = (25*(qPow(CIE_Y,(0.333333)))-17)/10;
    ui->label_32->setText(QString("%1").arg(munsell_v,0,'f',2));

    /****test****/
//    CIE_x = 0.4002;
//    CIE_y = 0.3504;
//    munsell_v = 3.69;
    /******/

    slope = (CIE_y-0.3163)/(CIE_x-0.3101);
    qDebug("CIE_x:%f,CIE_y:%f",CIE_x,CIE_y);
    qDebug("CIE_x:%f,CIE_y:%f",CIE_y-0.3163,CIE_x-0.3101);
    qDebug("***************************slope1:%f",slope);
    munsell_v1 = (munsell_v*10)/10;
//    munsell_v_r = (int)(munsell_v*10)%10;
    munsell_v_r = munsell_v - munsell_v1;



    Munsell_hvc1();
    Munsell_hvc2();

}

Widget::~Widget()
{
    delete ui;
}
