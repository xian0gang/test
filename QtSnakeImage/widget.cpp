#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    /// 全局变量
    int DELAY_CAPTION = 1500;
    int DELAY_BLUR = 100;
    int MAX_KERNEL_LENGTH = 31;

    Mat src; Mat dst;
    char window_name[] = "Filter Demo 1";

       namedWindow( window_name, CV_WINDOW_AUTOSIZE );

       /// 载入原图像
       src = imread( "picture.bmp", 1 );

       if( display_caption( "Original Image" ) != 0 )
       {  }

       dst = src.clone();
       if( display_dst( DELAY_CAPTION ) != 0 )
       { }

       /// 使用 均值平滑
       if( display_caption( "Homogeneous Blur" ) != 0 )
       {  }

       for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
           {
           blur( src, dst, Size( i, i ), Point(-1,-1) );
             if( display_dst( DELAY_BLUR ) != 0 )
             {

             }
           }

        /// 使用高斯平滑
        if( display_caption( "Gaussian Blur" ) != 0 ) {}

        for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
            {
            GaussianBlur( src, dst, Size( i, i ), 0, 0 );
              if( display_dst( DELAY_BLUR ) != 0 )
              { }
            }

         /// 使用中值平滑
         if( display_caption( "Median Blur" ) != 0 ) {}

         for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
             { medianBlur ( src, dst, i );
               if( display_dst( DELAY_BLUR ) != 0 ) { } }

         /// 使用双边平滑
         if( display_caption( "Bilateral Blur" ) != 0 ) { }

         for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
             { bilateralFilter ( src, dst, i, i*2, i/2 );
               if( display_dst( DELAY_BLUR ) != 0 ) {  } }

         /// 等待用户输入
         display_caption( "End: Press a key!" );

         waitKey(0);
}

int Widget::display_caption( char* caption )
{
  dst = Mat::zeros( src.size(), src.type() );
  putText( dst, caption,
           Point( src.cols/4, src.rows/2),
           CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255) );

  imshow( window_name, dst );
  int c = waitKey( DELAY_CAPTION );
  if( c >= 0 ) { }
  return 0;
 }

 int Widget::display_dst( int delay )
 {
   imshow( window_name, dst );
   int c = waitKey ( delay );
   if( c >= 0 ) { }
   return 0;
 }
Widget::~Widget()
{
    delete ui;
}
