#include "widget.h"
#include "ui_widget.h"
#include <cv.h>
#include <highgui.h>
#include <iostream>
using namespace cv;
using namespace std;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    Mat image = imread("picture.bmp");
    namedWindow("原图",WINDOW_AUTOSIZE);
    imshow( "原图",image );
    if(image.empty() == true)
    {
        cout<<"Can't Load Image ." << endl;
    }

    Mat tmp = image;
    //定义核
    Mat element = getStructuringElement(MORPH_RECT, Size(3,3));
    morphologyEx(image,tmp,MORPH_GRADIENT,element);

    namedWindow("梯度图",WINDOW_AUTOSIZE);
    imshow( "梯度图",tmp );
    //调用直方图函数
    vector<Mat> rgb_planes;
    split( tmp, rgb_planes );
    HIST(rgb_planes[0]);

    Mat im_gray;
    cvtColor(tmp,im_gray,CV_BGR2GRAY);

    /// 应用直方图均衡化
    equalizeHist( im_gray, im_gray );
        imshow("im_eq_gray",im_gray);

    threshold(im_gray,im_gray,10,255,CV_THRESH_OTSU+CV_THRESH_BINARY);

    imshow("yuzhi",im_gray);

    cvWaitKey(0);
    destroyWindow("原图");
    destroyWindow("梯度图");

}

Widget::~Widget()
{
    delete ui;
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
