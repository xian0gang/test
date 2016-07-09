#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <qmath.h>

using namespace cv;
using namespace std;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);


    int max_thresh = 255;
    RNG rng(12345);

    Mat src;
    Mat src_gray;
    src = imread("bw.jpg");
    imshow("src",src);
    cvtColor(src,src_gray,CV_BGR2GRAY);
    imshow("src_gray",src_gray);

    Mat threshold_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;


     threshold( src_gray, threshold_output, 125, 255, THRESH_BINARY );
  findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
//     findContours( threshold_output, contours, hierarchy ,CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE );

   vector<RotatedRect> minRect( contours.size() );
    Mat drawing_xg = Mat::zeros( src.size(), CV_8UC3 );
  /// 计算矩
  vector<Moments> mu(contours.size() );
  for( int i = 0; i < contours.size(); i++ )
  {
      minRect[i] = minAreaRect( Mat(contours[i]) );
      mu[i] = moments( contours[i], false );
   }

  for( int i = 0; i< contours.size(); i++ )
     {
qDebug(" * Contour[%d] - Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", i, mu[i].m00, contourArea(contours[i]), arcLength( contours[i], true ) );
//       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//       drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
//       circle( drawing, mc[i], 4, color, -1, 8, 0 );

Point2f rect_points[4];
minRect[i].points( rect_points );
Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       for( int j = 0; j < 4; j++ )
       {
           //画出矩形的边
          line( src, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );
          //打印4个点
            qDebug("x[%d]:%f;  y[%d]:%f",j, rect_points[j].x, j, rect_points[j].y);
       }
       qDebug("wwww:%f",qSqrt(qPow((rect_points[0].x-rect_points[1].x),2)+qPow(rect_points[0].y-rect_points[1].y,2)));
       qDebug("hhhh:%f",qSqrt(qPow((rect_points[1].x-rect_points[2].x),2)+qPow(rect_points[1].y-rect_points[2].y,2)));

     }



    qDebug("xg:%f",contourArea(contours[0]));

  vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    vector<Point2f>center( contours.size() );
    vector<float>radius( contours.size() );

    for( int i = 0; i < contours.size(); i++ )
       {
                //这必须先逼近多边形，然后在计算各种外接 内切
//        利用approxPolyDP来逼近多边形
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );


//        功能：计算点集的最外面矩形边界
         boundRect[i] = boundingRect( Mat(contours_poly[i]) );
//         功能： 计算完全包围已有轮廓最小圆，外接圆
//         minEnclosingCircle( contours_poly[i], center[i], radius[i] );
       }


    /// 画多边形轮廓 + 包围的矩形框 + 圆形框
    Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
    qDebug("contours.size:%d",contours.size());
    for( int i = 0; i< contours.size(); i++ )
       {
         Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//画轮廓
         drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
//画外接最小矩形
         rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
         rectangle( src, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );

//画外接圆
//         circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );

//         CvBox2D rect = minAreaRect(contours,stor)

       }



    /// 显示在一个窗口
    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
    imshow( "Contours", drawing );
    imshow( "Contours_xg", drawing_xg );

    imshow("src_contours",src);

}

Widget::~Widget()
{
    delete ui;
}
