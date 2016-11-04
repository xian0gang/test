/*******************************
 * 功能：画直方图，添加了坐标刻度和竖线，方便观察，写在一个函数中，方便调用
 *
 * xiangang
 * 20时40分55秒
 * ******************************/

#include "widget.h"
#include <stdio.h>
#include <QDebug>
using namespace cv;


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //读取图像
    Mat img = imread("picture.bmp",0);
    imshow("原图",img);
    //调用直方图函数
    vector<Mat> rgb_planes;
    split( img, rgb_planes );
    HIST(rgb_planes[0]);

}

Widget::~Widget()
{

}

void Widget::HIST(Mat srcimage)
{
    MatND dstHist;
    //字符串，用于显示刻度
    char string[10];
    /// 设定bin数目
    int histSize = 256;

    /// 设定取值范围 ( R,G,B) )
    float range[] = { 0, 255 } ;
    const float* histRange = { range };
    int channels = 0;

    bool uniform = true;
    bool accumulate = false;

    /// 计算直方图:
    calcHist( &srcimage, 1, &channels, Mat(), dstHist, 1, &histSize, &histRange, uniform, accumulate );
    int scale = 1;

    //创建直方图空间
    Mat dstimage(histSize *scale,histSize,CV_8U,Scalar(0));
    qDebug("直方图大小：%d*%d",histSize *scale,histSize);

    double minvalue = 0;
    double maxvalue = 0;
    //找出最大值和最小值
    minMaxLoc(dstHist,&minvalue,&maxvalue,0,0);
    int hpt = saturate_cast<int>(0.9 * histSize);
    //画直方图
    for(int i = 0;i < 256; i++)
    {
        float binvalue = dstHist.at<float>(i);
        int realvalue = saturate_cast<int>(binvalue * hpt / maxvalue);
        rectangle(dstimage,Point(i *scale,histSize-1),Point((i+1)*scale -1,histSize - realvalue),Scalar(255,0,0));
    }
    //画纵坐标刻度（像素个数）
       int kedu=0;
       qDebug("rows:%d;cols:%d",dstimage.rows,dstimage.cols);
       double bin_w =(double) dstimage.cols / 50;  // hdims: 条的个数，则 bin_w 为条的宽度
       double bin_u = (double)dstimage.rows/ maxvalue;  //// max: 最高条的像素个数，则 bin_u 为单个像素的高度
       for(int i=1;kedu<maxvalue;i++)
       {
           kedu=i*maxvalue/10;

           sprintf(string,"%d",kedu);
           qDebug("%s",string);
//               //在图像中显示文本字符串
           putText(
                   dstimage,                                                //源图
                   string,                                                      //字符串
                   cvPoint(0,dstimage.rows-kedu*bin_u), //点
                   CV_FONT_HERSHEY_COMPLEX,              //
                   0.3,                                                           //字体大小
                   Scalar(255, 255, 0)                                     //颜色
                   );
           }
           //画横坐标刻度（像素灰度值）
                 kedu=0;
                 for(int i=1;kedu<256;i++)
                {
                     kedu=i*20;
//                     itoa(kedu,string,10);//把一个整数转换为字符串
                     sprintf(string,"%d",kedu);
                     qDebug("%s",string);
                     //在图像中显示文本字符串
                     putText(
                             dstimage,                                                //
                             string,                                                      //
                             cvPoint(kedu*(dstimage.cols/256),10/*dstimage.rows*/), //
                             CV_FONT_HERSHEY_COMPLEX,              //
                             0.3,                                                           //
                             Scalar(255, 0, 0)                                  //
                             );
                     qDebug("x:%d;y:%d",kedu*(dstimage.cols/256),dstimage.rows);
                    //画竖线
                     line(dstimage,cvPoint(kedu*(dstimage.cols/256),10),cvPoint(kedu*(dstimage.cols/256),dstimage.rows),Scalar(255, 0, 0),0.02,4,0);
                }
    imshow("一维直方图",dstimage);

}
