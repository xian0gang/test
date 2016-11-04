#include "widget.h"
#include "ui_widget.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QString>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

        QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");

        db.setDatabaseName("cie"); //连接一个已存在的数据
        db.setHostName("10.128.46.253");//指定数据库服务器名称
        db.setUserName("root");      //设置登录名
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

}

Widget::~Widget()
{
    delete ui;
}
