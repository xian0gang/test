#include "widget.h"
#include "ui_widget.h"


//using namespace cv;
//using namespace std;
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    IplImage *image,*hsv,*hue,*saturation,*value;//图像空间

//    cv::Mat image = cv::imread("/home/xiangang/qt/qt_example/yuzhi/pic.bmp" ,CV_LOAD_IMAGE_GRAYSCALE);
image = cvLoadImage("picture.bmp");
//    image = cvLoadImage("2.jpg");
cvShowImage("image",image);
    hsv = cvCreateImage(cvGetSize(image),8,3);//给hsv色系的图像申请空间
    hue = cvCreateImage(cvGetSize(image),8,1);  //色调
    saturation = cvCreateImage(cvGetSize(image),8,1);//饱和度
    value = cvCreateImage(cvGetSize(image),8,1);//亮度

    cvNamedWindow("image",CV_WINDOW_AUTOSIZE);//用于显示图像的窗口
//    cvNamedWindow("hsv",CV_WINDOW_AUTOSIZE);
//    cvNamedWindow("hue",CV_WINDOW_AUTOSIZE);
//    cvNamedWindow("saturation",CV_WINDOW_AUTOSIZE);
//    cvNamedWindow("value",CV_WINDOW_AUTOSIZE);

    cvCvtColor(image,hsv,CV_BGR2HSV);//将RGB色系转为HSV色系

    cvSplit(hsv, hue, 0, 0, 0 );//分离三个通道
    cvSplit(hsv, 0, saturation, 0, 0 );
    cvSplit(hsv, 0, 0, value, 0 );

    cvShowImage("hue",hue);
    cvShowImage("saturation",saturation);
    cvShowImage("value",value);

    int th1 = 140;
    cv::Mat global1;
    cv::threshold(cv::Mat(hue,false),global1,th1,360,CV_THRESH_BINARY_INV);

    int th3 = 100;
    cv::Mat global3;
    cv::threshold(cv::Mat(saturation,false),global3,th3,255,CV_THRESH_BINARY_INV);

    int th2 = 80;
    cv::Mat global2;
    cv::threshold(cv::Mat(value,false),global2,th2,255,CV_THRESH_BINARY_INV);
/*
    int blockSize = 2;
        int constValue = 10;
        cv::Mat local;
        cv::adaptiveThreshold(cv::Mat( hue,false), local, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, blockSize, constValue);
*/
        cv::imwrite("hue.jpg",cv::Mat(hue,false));
        cv::imwrite("global1.jpg", global1);
        cv::imwrite("global2.jpg", global2);
 //       cv::imwrite("local.jpg", local);

//cv::imshow("h分量otsu全局阈值", hue);
//cvShowImage("h分量otsu全局阈值", hue);
        cv::imshow("h分量全局阈值", global1);
        cv::imshow("v分量全局阈值", global2);
        cv::imshow("s分量全局阈值", global3);
 //      cv::imshow("h分量局部阈值", local);

        //otsu
ImageBinarization(hue);
        IplImage *img = cvLoadImage("hue.jpg");    //加载图片
        IplImage *imgBuf = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);   //通道为3
        cvConvert(img, imgBuf);   //将源图片转化为位深度为8，通道为3的图片
        for (int i = 0; i < img->height; i++)
            {
                for(int j = 0; j < img->width; j++)
                {
                    if((((uchar *)(img->imageData +  i * img->widthStep ))[j * img->nChannels + 0] > 200)/*&&
                            (((uchar *)(img->imageData +  i * img->widthStep ))[j * img->nChannels + 0] < 100)*/)
                    {
                        ((uchar *)(img->imageData +  i * img->widthStep ))[j * img->nChannels + 0] = 234;   //改变该像素B的颜色分量
                        ((uchar *)(img->imageData +  i * img->widthStep ))[j * img->nChannels + 1] = 200;   //改变该像素G的颜色分量
                        ((uchar *)(img->imageData +  i * img->widthStep ))[j * img->nChannels + 2] = 150;   //改变该像素R的颜色分量
                    }
                }
            }
        cv::imshow("h分量测试",cv::Mat(img,false));

        for (int i = 0; i <(global2.rows)/2; i++)
            {
                for(int j = 0; j < global2.cols; j++)
                {
                    for(int n = 0; n < global2.channels(); n++ )
                    {
                        if(global2.at<uchar>(i,j*global2.channels()+n) > 125)
                        {
                            global1.at<uchar>(i,j*global1.channels()+n) = 255;
                            //local.at<uchar>(i,j*local.channels()+n) = 255;
                        }
                    }
                }
            }
        cv::imshow("合并h分量和v分量",global1);
 //       cv::imwrite("success.jpg",local);



        IplImage *img3 = cvLoadImage("success.jpg",0);
//            if(img3 == NULL)
//            {
////                printf("img load failed!\n");
//                return 0;
//            }
            IplImage *img_erode = cvCreateImage(cvGetSize(img), 8, 1);
            IplImage *img_dilate = cvCreateImage(cvGetSize(img), 8, 1);

            cvErode( img3,img_erode, NULL,1); //腐蚀
            cvDilate( img_erode,img_dilate, NULL,3); //膨胀
//            cvErode( img_dilate,img_erode, NULL,1); //腐蚀
        cvShowImage("腐蚀",img_erode);
        cvShowImage("膨胀",img_dilate);
        cv::imwrite("dilate_success.jpg",cv::Mat(img_dilate,false));
/*****************提取轮廓*****************/
 //       IplImage* src = NULL;
 //           IplImage* img = NULL;
            IplImage* dst = NULL;
        CvMemStorage* storage = cvCreateMemStorage (0);;
            CvSeq* contour = 0;
            double contour_area_temp = 0, contour_area_max = 0, contour_area_2 = 0;
            CvSeq* area_max_contour = 0;
            CvSeq* area_2_contour = 0;
            int contours = 0;
            CvScalar external_color;
            CvScalar hole_color;
            //寻找轮廓   CV_RETR_EXTERNAL     CV_RETR_LIST
        contours = cvFindContours (img_dilate, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
qDebug("contours:%d",contours);

        //创建新的图像显示轮廓
        dst = cvCreateImage (cvGetSize(image), image->depth, image->nChannels);
        cvCopy(image,dst);

        for (;contour != 0; contour = contour->h_next)
       {
            contour_area_temp = fabs(cvContourArea(contour,CV_WHOLE_SEQ));
            if(contour_area_temp > contour_area_max)
            {
                contour_area_2 = contour_area_max;
                contour_area_max = contour_area_temp;
                area_2_contour = area_max_contour;
                area_max_contour = contour;
            }
        }
            external_color = CV_RGB(rand()&255, rand()&255, rand()&255);
           hole_color = CV_RGB(rand()&255, rand()&255, rand()&255);
           //画轮廓
            cvDrawContours (dst, area_2_contour, external_color, hole_color, 0, 2, 8);



           cvNamedWindow ("Contour", 1);
           cvShowImage ("Contour", dst);

           for (int i = 0; i < image->height; i++)
               {
                   for(int j = 0; j < image->width; j++)
                   {
                       //-1 0 1分别代表轮廓外，轮廓上，轮廓里面
                       if(-1 == (cvPointPolygonTest(area_2_contour,cv::Point2f(j,i),0)))
                       {
                           ((uchar *)(image->imageData +  i * image->widthStep ))[j * image->nChannels + 0] = 234;   //改变该像素B的颜色分量
                           ((uchar *)(image->imageData +  i * image->widthStep ))[j * image->nChannels + 1] = 200;   //改变该像素G的颜色分量
                           ((uchar *)(image->imageData +  i * image->widthStep ))[j * image->nChannels + 2] = 150;   //改变该像素R的颜色分量
                       }
                   }
               }
           cv::imshow("final",cv::Mat(image,false));


/*****************************************/
//        cvWrite("dilate_success.jpg",img_dilate);

//        cv::waitKey(0); /// 等待用户按键。如果是ESC健则退出等待过程。
//        while(true)
//        {
//          int c;
//          c = cv::waitKey( 20 );
//          if( (char)c == 27 )
//            { break; }
//         }


zhifangtu(value);

        cvReleaseMemStorage (&storage);
 //      cvReleaseImage (&src);
          cvReleaseImage (&img);
            cvReleaseImage (&dst);
}

void Widget::zhifangtu(IplImage *hist_img)
{
    IplImage *histimg = 0;
    CvHistogram *hist = 0;                    //直方图
    int hdims=255;
     float hranges_arr[] = {0,255};
        float* hranges = hranges_arr;
        int bin_w;
        float max_val,min_val;
        int i;

        cvNamedWindow( "Histogram", 0 );
//        cvNamedWindow( "src", 0);
hist = cvCreateHist( 1, &hdims, CV_HIST_ARRAY, &hranges, 1 );   //创建直方图
histimg = cvCreateImage( cvSize(320,200), 8, 3 );
    cvZero( histimg );                                            //清零

    cvCalcHist( &hist_img, hist, 0, 0 );                               // 计算直方图,即每个bin的大小
    cvGetMinMaxHistValue( hist, &min_val, &max_val, 0, 0 );              // 只找最大值
    cvThreshHist(hist,50);
//qDebug("max:%4.2f",max_val);
//qDebug("min:%4.2f",min_val);
 cvConvertScale( hist->bins,hist->bins, max_val ? 255. / max_val : 0., 0 );             // 缩放 bin 到区间 [0,255]

    cvZero( histimg );
    bin_w = histimg->width / hdims;                               // hdims: 直方图竖条的个数，则 bin_w 为条的宽度

    // 画直方图
    for( i = 0; i < hdims; i++ )
    {
        double val = ( cvGetReal1D(hist->bins,i)*histimg->height/255 );
        CvScalar color = CV_RGB(255,255,0);                                 //(hsv2rgb(i*180.f/hdims);
        cvRectangle( histimg, cvPoint(i*bin_w,histimg->height),
            cvPoint((i+1)*bin_w,(int)(histimg->height - val)),
            color, 1, 8, 0 );
    }

//    cvShowImage( "src", hue);
        cvShowImage( "Histogram", histimg );
cv::waitKey(0);
//    cvDestroyWindow("src");
    cvDestroyWindow("Histogram");
    //cvReleaseImage( &src );
    cvReleaseImage( &histimg );
    cvReleaseHist ( &hist );
}

void Widget::ImageBinarization(IplImage *src)
{
    int i,j,width,height,step,chanel,threshold;

    float size,avg,va,maxVa,p,a,s;
    unsigned char *dataSrc;
    float histogram[256];

    width = src->width;
    height = src->height;
    dataSrc = (unsigned char *)src->imageData;
    step = src->widthStep/sizeof(char);
    chanel = src->nChannels;

    for(i=0; i<256; i++)
        histogram[i] = 0;
    for(i=0; i<height; i++)
        for(j=0; j<width*chanel; j++)
        {
            histogram[dataSrc[i*step+j]-'0'+48]++;
        }
    size = width * height;
    for(i=0; i<256; i++)
        histogram[i] /=size;

    avg = 0;
    for(i=0; i<256; i++)
        avg += i*histogram[i];
    va = 0;
    for(i=0; i<256; i++)
        va += fabs(i*i*histogram[i]-avg*avg);

    threshold = 20;
    maxVa = 0;
    p = a = s = 0;
    for(i=0; i<256; i++)
    {
        p += histogram[i];
        a += i*histogram[i];
        s = (avg*p-a)*(avg*p-a)/p/(1-p);
        if(s > maxVa)
        {
            threshold = i;
            maxVa = s;
        }
    }

    for(i=0; i<height; i++)
        for(j=0; j<width*chanel; j++)
        {
            if(dataSrc[i*step+j] > threshold)
                dataSrc[i*step+j] = 0;
            else
                dataSrc[i*step+j] = 255;
        }
    cvShowImage("otsu",src);
}


Widget::~Widget()
{
    delete ui;

}
