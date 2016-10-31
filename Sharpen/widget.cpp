#include "widget.h"
#include "ui_widget.h"
using namespace cv;
using namespace std;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    Mat image = imread("picture.bmp");
        Mat result, result1;

//        Sharpen(image,result);
//        SharpenKernel(image,result1);
        Mat sharpenedLena;

        Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);

        cv::filter2D(image, sharpenedLena, image.depth(), kernel);

        imshow("原图", image);

        imshow("图像锐化后", sharpenedLena);

//        namedWindow("image");
//        imshow("image",image);

//        namedWindow("image_process");
//        imshow("image_process",result);

//        namedWindow("image_process_ker");
//        imshow("image_process_ker",result1);
        waitKey();
}

Widget::~Widget()
{
    delete ui;
}
void Widget::Sharpen(Mat &image, Mat &result)
{
    result.create( image.size(), image.type() );
    for(int j=1; j<image.rows-1; j++)
    {
        const uchar* previous = image.ptr<const uchar>(j-1);
        const uchar* current = image.ptr<const uchar>(j);
        const uchar* next = image.ptr<const uchar>(j+1);

        uchar * output = result.ptr<uchar>(j);
        for(int i=1; i<(image.cols-1); i++)
        {
            output[i*3] = saturate_cast<uchar>(5*current[i*3] - current[(i-1)*3] - current[(i+1)*3] - previous[i*3] - next[i*3]);
            output[i*3+1] = saturate_cast<uchar>(5*current[i*3+1] - current[(i-1)*3+1] - current[(i+1)*3+1] - previous[i*3+1] - next[i*3+1]);
            output[i*3+2] = saturate_cast<uchar>(5*current[i*3+2] - current[(i-1)*3+2] - current[(i+1)*3+2] - previous[i*3+2] - next[i*3+2]);
        }
    }
    result.row(0).setTo(Scalar(0,0,0));
    result.row(result.rows-1).setTo(Scalar(0,0,0));
    result.col(0).setTo(Scalar(0,0,0));
    result.col(result.cols-1).setTo(Scalar(0,0,0));
}

void Widget::SharpenKernel(Mat &image, Mat &result)
{
    Mat kernel(3,3,CV_32F,Scalar(0));
    kernel.at<float>(1,1) = 5;
    kernel.at<float>(0,1) = -1;
    kernel.at<float>(2,1) = -1;
    kernel.at<float>(1,0) = -1;
    kernel.at<float>(1,2) = -1;
    filter2D(image,result,image.depth(),kernel);
    result.row(0).setTo(Scalar(0,0,0));
    result.row(result.rows-1).setTo(Scalar(0,0,0));
    result.col(0).setTo(Scalar(0,0,0));
    result.col(result.cols-1).setTo(Scalar(0,0,0));
}
