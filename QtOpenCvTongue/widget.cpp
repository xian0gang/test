#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //markes = cvCreateImage()

    //open file
   connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(OpenFile()));
   //send filename
   connect(this,SIGNAL(send_filename(QString)),ui->label,SLOT(set_filename()));
   connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(close_pushbutton()));
   connect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(set_scale_pushbutton()));
   connect(this,SIGNAL(send_set_scale()),ui->label,SLOT(set_scaled_button()));

}

Widget::~Widget()
{
    delete ui;
}

void Widget::OpenFile()
{
    emit send_filename(filename);
}

void Widget::set_scale_pushbutton()
{
    emit send_set_scale();
}

void Widget::close_pushbutton()
{
    this->close();
}
