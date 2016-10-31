#include "widget.h"
#include "ui_widget.h"
using namespace cv;
using namespace std;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    IplImage* img = cvLoadImage( "picture4.bmp", 1);//三通道图像
//       IplImage* img = cvLoadImage( "5.jpg", 1);//三通道图像

    IplImage* lab;
    lab = cvCreateImage(cvGetSize(img),8,3);//给hsv色系的图像申请空间
    cvCvtColor(img,lab,CV_BGR2Lab);

    /****************************************/
    IplImage *dst;
     IplImage *dst1;
     IplImage *dst2;
     IplImage *dst3;

     dst1 = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 1);
     dst2 = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 1);
     dst3 = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 1);
     dst = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 3);

     cvSplit(lab, dst1, dst2, dst3, 0);//分离RGB通道，分别得到BGR的单通道图像
      cvMerge(0, dst2, dst3, 0, dst);//逆运算，将三通道图像融合得到彩色图像
/******************************************************/

cvShowImage("dst",dst);
        int total= img->height*img->width;
        int cluster_num = 3;
        CvMat *row = cvCreateMat( img->height,img->width,CV_32FC3 );
        cvConvert(dst,row);//转一下类型！
        CvMat *clusters = cvCreateMat( total, 1, CV_32SC1 );
        cvReshape(row,row,3,total);//修改矩阵的形状,每个数据一行，使row指向修改后的数据，不修改通道数
        cvKMeans2( row, cluster_num, clusters,cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10/* 迭代次数*/, 1.0/*迭代误差*/ ));
        cvReshape(clusters,clusters,3,dst->width);//聚类完的结果再reshape回来方便看

        int i=0,j=0;
        CvScalar s;
        IplImage* resImg = cvCreateImage( cvSize(img->width,img->height), 8, 1 );//生成用来显示结果的图像
        IplImage* img1 = cvCreateImage( cvSize(img->width,img->height), 8, 1 );//生成用来显示结果的图像
        IplImage* img2 = cvCreateImage( cvSize(img->width,img->height), 8, 1 );//生成用来显示结果的图像
        IplImage* img3 = cvCreateImage( cvSize(img->width,img->height), 8, 1 );//生成用来显示结果的图像
        s=cvGet2D(img,i,j);
        for(i=0;i<img->height;i++)
        {
            for (j=0;j<img->width;j++)
            {
                if (clusters->data.i[i*img->width+j]==1)
                {
                    s.val[0]=255;
                    s.val[1]=255;
                    s.val[2]=255;
                    cvSet2D(resImg,i,j,s);//注意循环顺序
                    cvSet2D(img1,i,j,s);
                }
                else
                {
                    if (clusters->data.i[i*img->width+j]==2)
                    {
                        s.val[0]=155;
                        s.val[1]=155;
                        s.val[2]=155;
                        cvSet2D(resImg,i,j,s);//注意循环顺序
                        cvSet2D(img2,i,j,s);
                    }
                    else
                    {
                        s.val[0]=55;
                        s.val[1]=55;
                        s.val[2]=55;
                        cvSet2D(resImg,i,j,s);
                        cvSet2D(img3,i,j,s);
                    }
                }
            }
        }
        cvShowImage( "original", img );
        cvShowImage( "resImg", resImg );

        IplImage *img1_dst = cvCreateImage(cvGetSize(img), 8, 1);
        chuli(img1,img1_dst);
        cvShowImage( "img1_dst", img1_dst );
        IplImage *img2_dst = cvCreateImage(cvGetSize(img), 8, 1);
        chuli(img2,img2_dst);
        cvShowImage( "img2_dst", img2_dst);
        IplImage *img3_dst = cvCreateImage(cvGetSize(img), 8, 1);
        chuli(img3,img3_dst);
        cvShowImage( "img3_dst", img3_dst );


        //合并两个图像
        IplImage *img_hebing = cvCreateImage(cvGetSize(img), 8, 1);
        cvAddWeighted(img1_dst,0.5,img2_dst,0.5,0.0,img_hebing);
        cvShowImage( "img_hebing", img_hebing );

        /*****************分水岭处理**********************/
        water(img_hebing,img);


        /***************************************/
        int key = cvWaitKey(0);

        cvReleaseImage(&img);//记得释放内存
        cvReleaseImage (&resImg);
        cvReleaseMat(&row);
        cvReleaseMat(&clusters);

        cvReleaseImage (&img1);
        cvReleaseImage (&img2);
        cvReleaseImage (&img3);
cvReleaseImage (&lab);
cvReleaseImage (&dst);
cvReleaseImage (&dst1);
cvReleaseImage (&dst2);
cvReleaseImage (&dst3);
cvReleaseImage (&img1_dst);
cvReleaseImage (&img2_dst);
cvReleaseImage (&img3_dst);
}

void Widget::chuli(IplImage* src,IplImage *dst)
{
        CvSeq* contour = 0;
//        IplImage* dst = cvCreateImage(cvGetSize(src), 8, 1);
CvMemStorage* storage = cvCreateMemStorage(0);

        cvFindContours( src, storage, &contour, sizeof(CvContour),
                        CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
        cvZero(dst);        // 清空数组

        double maxarea = 5000;
        double minarea = 1000;
        int m = 0;
        for( ; contour != 0; contour = contour->h_next )
        {

            double tmparea = fabs(cvContourArea(contour));
            if(tmparea < minarea)
            {
                cvSeqRemove(contour, 0); // 删除面积小于设定值的轮廓
                continue;
            }
            CvRect aRect = cvBoundingRect( contour, 0 );
            if ((aRect.width/aRect.height)<1)
            {
                cvSeqRemove(contour, 0); //删除宽高比例小于设定值的轮廓
                continue;
            }
            if(tmparea > maxarea)
            {
                maxarea = tmparea;
            }
            m++;
            // 创建一个色彩值
            CvScalar color = CV_RGB( 0, 255, 255 );

            //max_level 绘制轮廓的最大等级。如果等级为0，绘制单独的轮廓。如果为1，绘制轮廓及在其后的相同的级别下轮廓
            //如果值为2，所有的轮廓。如果等级为2，绘制所有同级轮廓及所有低一级轮廓，诸此种种
            //如果值为负数，函数不绘制同级轮廓，但会升序绘制直到级别为abs(max_level)-1的子轮廓
            cvDrawContours(dst, contour, color, color, -1, 1, 8);   //绘制外部和内部的轮廓
            cvDrawContours(dst, contour, CV_RGB(255, 255, 255),
                           CV_RGB(255, 255, 255), 0, CV_FILLED, 8, cvPoint(0, 0));   //绘制外部和内部的轮廓
        }

//        /*先膨胀在腐蚀*/
//            IplImage *img_erode = cvCreateImage(cvGetSize(dst), 8, 1);
            IplImage *img_dilate = cvCreateImage(cvGetSize(dst), 8, 1);

           cvDilate( dst,img_dilate, NULL,1); //腐蚀
            cvErode( img_dilate,dst, NULL,3); //膨胀
//            cvShowImage("腐蚀",img_erode);
            cvShowImage("膨胀",img_dilate);


        cvNamedWindow("Components", 1);
        cvShowImage("Components", dst);
//        cvWaitKey(0);
        cvDestroyWindow("Source");
//        cvReleaseImage(&img);
        cvDestroyWindow("Components");
//        cvReleaseImage(&dst);
//        cvReleaseImage(&img_erode);
        cvReleaseImage(&img_dilate);
}

void Widget::water(IplImage *src, IplImage *src_img)
{
    int i, j, comp_count = 0;
    CvMat* color_tab = 0;
    CvRNG rng = cvRNG(-1);

    CvSeq* contours = 0;
    CvMemStorage* storage = cvCreateMemStorage(0);;
    cvClearMemStorage(storage);
    IplImage* markes = cvCreateImage(cvGetSize(src),IPL_DEPTH_32S, 1 );
    cvFindContours( src, storage, &contours, sizeof(CvContour),CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

    for( ; contours != 0; contours = contours->h_next, comp_count++ )
    {
        cvDrawContours(markes, contours, cvScalarAll(comp_count+1),
        cvScalarAll(comp_count+1), 1, -1, 8, cvPoint(0,0) );
    }

    color_tab = cvCreateMat( 1, comp_count, CV_8UC3 );//创建随机颜色列表
    for( i = 0; i < comp_count; i++ )	//不同的整数标记
    {
        uchar* ptr = color_tab->data.ptr + i*3;
        ptr[0] = (uchar)(cvRandInt(&rng)%180 + 50);
        ptr[1] = (uchar)(cvRandInt(&rng)%180 + 50);
        ptr[2] = (uchar)(cvRandInt(&rng)%180 + 50);
    }

    double t = (double)cvGetTickCount();
    cvWatershed( src_img, markes );
    t = (double)cvGetTickCount() - t;
    qDebug( "exec time = %gms\n", t/(cvGetTickFrequency()*1000.) );
IplImage* test = cvCloneImage(src_img);
    /*********/
    for( i = 0; i < markes->height; i++ )
        for( j = 0; j < markes->width; j++ )
        {
            int idx = CV_IMAGE_ELEM( markes, int, i, j );//markers的数据类型为IPL_DEPTH_32S
            uchar* dst = &CV_IMAGE_ELEM( src_img, uchar, i, j*3 );//BGR三个通道的数是一起的,故要j*3
            uchar* dst_test = &CV_IMAGE_ELEM( test, uchar, i, j*3 );//BGR三个通道的数是一起的,故要j*3

            if( idx == -1 ) //输出时若为-1，表示各个部分的边界
            {
                dst_test[2] = (uchar)0;
                dst[0] = dst[1] = dst[2] = (uchar)255; // should not get here
            }
            else if( idx <= 0 || idx > comp_count )  //异常情况
            {
                dst_test[0] = dst_test[1] = dst_test[2] = (uchar)0; // should not get here
            }
            else if( idx == 1 )  //异常情况
            {
                //green      first
                dst_test[0] = (uchar)0;
                dst[0] = dst[1] = dst[2] = (uchar)255; // should not get here
            }
            else if( idx == 2 )  //异常情况
            {
                //blue        second
                dst_test[1] = (uchar)255;
//                dst[0] = dst[1] = dst[2] = (uchar)255; // should not get here

            }
            else if( idx == 3 )  //异常情况
            {
                //red         third
                dst_test[2] = (uchar)255;
//                dst[0] = dst[1] = dst[2] = (uchar)0; // should not get here
            }
            else //正常情况
            {
                uchar* ptr = color_tab->data.ptr + (idx-1)*3;
                dst_test[0] = ptr[0]; dst_test[1] = ptr[1]; dst_test[2] = ptr[2];
                dst[0] = dst[1] = dst[2] = (uchar)255; // should not get here
            }
        }
    cvShowImage("water",src_img);
    cvShowImage("water_test",test);
    cvReleaseImage(&markes);
    cvReleaseImage(&test);

}


Widget::~Widget()
{
    delete ui;
}
