/*******************************
 * 直方图均衡
 * 20时02分53秒 2016-7-9
 * xiangang
 * ******************************/
#include "widget.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    Mat src, dst;

     /// 装载图像
     src = imread("picture.bmp", 1 );

//     if( !src.data )
//       { }

     /// 分割成3个单通道图像 ( R, G 和 B )
     vector<Mat> rgb_planes;
     split( src, rgb_planes );

     /// 设定bin数目
     int histSize = 255;

     /// 设定取值范围 ( R,G,B) )
     float range[] = { 0, 255 } ;
     const float* histRange = { range };

     bool uniform = true;
     bool accumulate = false;

     Mat r_hist, g_hist, b_hist;

     /// 计算直方图:
     calcHist( &rgb_planes[0], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
     calcHist( &rgb_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
     calcHist( &rgb_planes[2], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );

     double minvalue = 0;
     double maxvalue = 0;
     //找出最大值和最小值
     minMaxLoc(r_hist,&minvalue,&maxvalue,0,0);
     qDebug("max:%f",maxvalue);
      qDebug("min:%f",minvalue);

     // 创建直方图画布
     int hist_w = 400; int hist_h = 400;
     int bin_w = cvRound( (double) hist_w/histSize );
     qDebug("hist_w:%d",hist_w);
     qDebug("histSize:%d",histSize);
     qDebug("bin_w:%d",bin_w);

     Mat histImage( hist_w, hist_h, CV_8UC3, Scalar( 0,0,0) );
     Mat histImage_g( hist_w, hist_h, CV_8UC3, Scalar( 0,0,0) );
     Mat histImage_b( hist_w, hist_h, CV_8UC3, Scalar( 0,0,0) );
     Mat histImage_xg( hist_w, hist_h, CV_8UC3, Scalar( 0,0,0) );
qDebug("rows:%d",histImage.rows);
     /// 将直方图归一化到范围 [ 0, histImage.rows ]
     normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
     normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
     normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

     /// 在直方图画布上画出直方图
     for( int i = 1; i < histSize; i++ )
       {
         line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                          Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                          Scalar( 0, 0, 255), 2, 8, 0  );
         line( histImage_xg, Point( (i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                          Point( (i), hist_h - cvRound(r_hist.at<float>(i)) ),
                          Scalar( 0, 0, 255), 2, 8, 0  );
         line( histImage_g, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                          Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                          Scalar( 0, 255, 0), 2, 8, 0  );
         line( histImage_b, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                          Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                          Scalar( 255, 0, 0), 2, 8, 0  );
        }

     /// 显示直方图
     namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
     imshow("calcHist Demo", histImage );
     imshow("calcHist Demo g", histImage_g );
     imshow("calcHist Demo b", histImage_b );
      imshow("calcHist Demo xg", histImage_xg );

     waitKey(0);

}

Widget::~Widget()
{

}
