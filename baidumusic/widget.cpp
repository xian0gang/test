#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->webView->setUrl(QUrl(QStringLiteral("http://fm.baidu.com/")));
}

Widget::~Widget()
{
    delete ui;
}
