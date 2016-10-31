#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //markes = cvCreateImage()

    //open file
//   connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(OpenFile()));
   //send filename
//   connect(this,SIGNAL(send_filename(QString)),ui->label,SLOT(set_filename()));

//   connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(close_pushbutton()));

//   connect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(set_scale_pushbutton()));
//   connect(this,SIGNAL(send_set_scale()),ui->label,SLOT(set_scaled_button()));

    connect(ui->pushButton_4,SIGNAL(clicked(bool)),this,SLOT(capture_pic()));
    connect(this,SIGNAL(show_pic()),ui->label,SLOT(show_picture()));

}

Widget::~Widget()
{
    delete ui;
}

//打开文件    图片
void Widget::OpenFile()
{
    emit send_filename(filename);
}

//利用opencv获取轮廓
void Widget::set_scale_pushbutton()
{
    emit send_set_scale();
}

//关闭程序
void Widget::close_pushbutton()
{
    this->close();
}

//拍照
void Widget::capture_pic()
{
    //出发拍照信号
    emit show_pic();
}
