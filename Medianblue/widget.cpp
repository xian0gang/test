/**************************************************
*中值滤波
*2016.9.23
*xiangang
****************************************************/

#include "widget.h"
#include "ui_widget.h"
using namespace cv;
using namespace std;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    // load image
    cv::Mat image = imread("picture.bmp",1);
//    cv::resize(image, image, cv::Size(), 0.3, 0.3);
imshow("src picture",image);
    // call function to add noise
//    salt(image,3000);//加入盐噪声255
//    pepper(image,3000);//加入椒噪声0

    // display result
    cv::imshow("salted image",image);

    //median filte
    Mat resutl;
    cv::medianBlur(image,resutl,3);

    //display result
    cv::imshow("median filted image",resutl);
    cv::waitKey();
}

Widget::~Widget()
{
    delete ui;
}
void Widget::salt(cv::Mat image, int n)
{

    int i,j;
    for (int k=0; k<n/2; k++)
    {

        // rand() is the random number generator
        i = std::rand()%image.cols; // % 整除取余数运算符,rand=1022,cols=1000,rand%cols=22
        j = std::rand()%image.rows;

        if (image.type() == CV_8UC1) { // gray-level image

            image.at<uchar>(j,i)= 255; //at方法需要指定Mat变量返回值类型,如uchar等

        } else if (image.type() == CV_8UC3) { // color image

            image.at<cv::Vec3b>(j,i)[0]= 255; //cv::Vec3b为opencv定义的一个3个值的向量类型
            image.at<cv::Vec3b>(j,i)[1]= 255; //[]指定通道，B:0，G:1，R:2
            image.at<cv::Vec3b>(j,i)[2]= 255;
        }
    }
}

//椒噪声
void Widget::pepper(cv::Mat image, int n)
{

    int i,j;
    for (int k=0; k<n; k++) {

        // rand() is the random number generator
        i = std::rand()%image.cols; // % 整除取余数运算符,rand=1022,cols=1000,rand%cols=22
        j = std::rand()%image.rows;

        if (image.type() == CV_8UC1) { // gray-level image

            image.at<uchar>(j,i)= 0; //at方法需要指定Mat变量返回值类型,如uchar等

        } else if (image.type() == CV_8UC3) { // color image

            image.at<cv::Vec3b>(j,i)[0]= 0; //cv::Vec3b为opencv定义的一个3个值的向量类型
            image.at<cv::Vec3b>(j,i)[1]= 0; //[]指定通道，B:0，G:1，R:2
            image.at<cv::Vec3b>(j,i)[2]= 0;
        }
    }
}
