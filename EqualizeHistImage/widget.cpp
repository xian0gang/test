#include "widget.h"
#include "ui_widget.h"
using namespace cv;
using namespace std;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    const char *pstrWindowsSrcTitle = "原图";
    const char *pstrWindowsHisEquaTitle = "直方图均衡化后";

    // 从文件中加载原图
    IplImage *pSrcImage = cvLoadImage("picture.bmp", CV_LOAD_IMAGE_UNCHANGED);
    IplImage *pHisEquaImage = EqualizeHistColorImage(pSrcImage);

    cvNamedWindow(pstrWindowsSrcTitle, CV_WINDOW_AUTOSIZE);
    cvNamedWindow(pstrWindowsHisEquaTitle, CV_WINDOW_AUTOSIZE);
    cvShowImage(pstrWindowsSrcTitle, pSrcImage);
    cvShowImage(pstrWindowsHisEquaTitle, pHisEquaImage);


    cvWaitKey(0);

    cvDestroyWindow(pstrWindowsSrcTitle);
    cvDestroyWindow(pstrWindowsHisEquaTitle);
    cvReleaseImage(&pSrcImage);
    cvReleaseImage(&pHisEquaImage);
}

Widget::~Widget()
{
    delete ui;
}

IplImage* Widget::EqualizeHistColorImage(IplImage *pImage)
{
    IplImage *pEquaImage = cvCreateImage(cvGetSize(pImage), pImage->depth, 3);

    // 原图像分成各通道后再均衡化,最后合并即彩色图像的直方图均衡化
    const int MAX_CHANNEL = 4;
    IplImage *pImageChannel[MAX_CHANNEL] = {NULL};

    int i;
    for (i = 0; i < pImage->nChannels; i++)
        pImageChannel[i] = cvCreateImage(cvGetSize(pImage), pImage->depth, 1);

    cvSplit(pImage, pImageChannel[0], pImageChannel[1], pImageChannel[2], pImageChannel[3]);

    for (i = 0; i < pImage->nChannels; i++)
        cvEqualizeHist(pImageChannel[i], pImageChannel[i]);

    cvMerge(pImageChannel[0], pImageChannel[1], pImageChannel[2], pImageChannel[3], pEquaImage);

    for (i = 0; i < pImage->nChannels; i++)
        cvReleaseImage(&pImageChannel[i]);

    return pEquaImage;
}
