#include "mylable.h"
#include "function.h"
#include <stdio.h>
MyLable::MyLable(QWidget *parent) : QLabel(parent)
{
    num_list = 0;
//    prev_pt = (-1,-1);

    storage = cvCreateMemStorage(0);
    rng = cvRNG(-1);
    Function::deviceOpen();
    printf("deviceOpen\n");
    Function::deviceInit();
    printf("deviceInit\n");
    Function::captureStart();
    printf("captureStart\n");
    unsigned char image_buf[1536000+54];
    Function::frameRead(image_buf);
    printf("frameRead\n");

}

void MyLable::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

//    image.load(set_pic_filename_path);
//    painter.drawImage(0,0,image,image.width(),image.height());
    painter.drawImage(0,0,image);

    //在选取的点上标号或圈出
//    painter.setPen(QPen(Qt::red,1));
//    painter.drawEllipse(x-3,y-3,6,6);

    painter.setPen(QPen(Qt::blue,4));

    if(!list.empty())
    {
        painter.drawLines(list);
    }
    if(!list1.empty())
    {
        painter.drawLines(list1);
    }
    if(!list2.empty())
    {
        painter.drawLines(list2);
    }
    if(!list3.empty())
    {
        painter.drawLines(list3);
    }
}

void MyLable::mousePressEvent(QMouseEvent *event)
{
    /*      contral mousemoveevent on or off    */
    setMouseTracking(true);
    if(event->button() == Qt::RightButton)
    {
        //获取点
        x=event->x();
        y=event->y();
        qDebug("x:%d",x);
        qDebug("y:%d\n",y);

        prev_pt = cvPoint(event->x(),event->y());
        update();
    }
}

void MyLable::mouseMoveEvent(QMouseEvent *event)
{
    /*      contral mousemoveevent on or off    */
    //setMouseTracking(true);

    QPoint x_y;
    x_y.setX(event->x());
    x_y.setY(event->y());
    switch (num_list) {
    case 0:
        list.append(x_y);
        break;
    case 1:
        list1.append(x_y);
        break;
    case 2:
        list2.append(x_y);
        break;
    case 3:
        list3.append(x_y);
        break;
    default:
        break;
    }

        CvPoint pt = cvPoint(event->x(),event->y());
        if( prev_pt.x < 0 )
        {
            prev_pt = pt;
        }
        //CvScalar 成员：double val[4] RGBA值A=alpha
        cvLine( marker_mask, prev_pt, pt, cvScalarAll(255), 5, 8, 0 );
        cvLine( draw_img, prev_pt, pt, cvScalarAll(255), 5, 8, 0 );
        prev_pt = pt;
        cvShowImage("draw_img",draw_img);

    update();
}

void MyLable::mouseReleaseEvent(QMouseEvent *event)
{
    /*      contral mousemoveevent on or off    */
    setMouseTracking(false);
    if(event->button() == Qt::LeftButton)
    {
        num_list = num_list + 1;
        if(num_list == 4)
            num_list = 0;
    }

    prev_pt = cvPoint(-1,-1);
    update();
}

void MyLable::set_filename()
{
    QString xgfilename;
    xgfilename=QFileDialog::getOpenFileName(this,
                                     tr("选择图像"),
                                     "",
                                     tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
    if(xgfilename.isEmpty())
    {
        return;
    }
    else
    {
        set_pic_filename_path=xgfilename;
        //*  clear  */
        clear_list();
//        image.load(set_pic_filename_path);
        std::string str = set_pic_filename_path.toStdString();
        const char* ch = str.c_str();
        printf("ch:",&ch);
        qDebug()<<"ch:"<<&ch;
        img0 = cvLoadImage(ch,1);
        img = Function::CjwIplToQImg(img0);
        image = *img;

        cvShowImage("img0",img0);

        markes = cvCreateImage(cvSize(image.width(),image.height()),IPL_DEPTH_32S, 1 );
        marker_mask = cvCreateImage( cvSize(image.width(),image.height()), 8, 1 );
        wshed = cvCloneImage( img0 );
        img_gray = cvCloneImage( img0 );
        draw_img = cvCloneImage( img0 );
        tongue_body = cvCloneImage( img0 );
        cvCvtColor( img0, marker_mask, CV_BGR2GRAY );
        cvCvtColor( marker_mask, img_gray, CV_GRAY2BGR );
        cvZero(markes);
        cvZero(marker_mask);
        cvZero( wshed );
        cvZero(tongue_body);
        update();
    }
    qDebug()<<"filenam:"<<xgfilename;
    num = 0;
}

void MyLable::set_scaled(QVector<QPoint> list, int num)
{

}

void MyLable::set_scaled_button()
{
    int i, j, comp_count = 0;
    CvMat* color_tab = 0;
    CvSeq* contours = 0;

    cvSave("markes.xml",marker_mask);
    cvClearMemStorage(storage);
    cvFindContours( marker_mask, storage, &contours, sizeof(CvContour),
                    CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
    cvZero( markes );
    for( ; contours != 0; contours = contours->h_next, comp_count++ )
    {
        //cvDrawContours(markers, contours, cvScalarAll(comp_count+1),
            //cvScalarAll(comp_count+1), -1, -1, 8, cvPoint(0,0) );
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
    cvSave("img0.xml",markes);
    cvWatershed( img0, markes );
    cvSave("img1.xml",markes);
//                cvShowImage("imgo",img0);
    t = (double)cvGetTickCount() - t;
    printf( "exec time = %gms\n", t/(cvGetTickFrequency()*1000.) );

    /*********/
    for( i = 0; i < markes->height; i++ )
        for( j = 0; j < markes->width; j++ )
        {
            int idx = CV_IMAGE_ELEM( markes, int, i, j );//markers的数据类型为IPL_DEPTH_32S
            uchar* dst = &CV_IMAGE_ELEM( wshed, uchar, i, j*3 );//BGR三个通道的数是一起的,故要j*3
            uchar* t_body = &CV_IMAGE_ELEM( tongue_body, uchar, i, j*3 );
            uchar* src_pic = &CV_IMAGE_ELEM( img0, uchar, i, j*3 );
            if( idx == -1 ) //输出时若为-1，表示各个部分的边界
            {
                //dst[0] = dst[1] = dst[2] = (uchar)128;
                dst[2] = (uchar)255;
            }
            else if( idx <= 0 || idx > comp_count )  //异常情况
            {
                //dst[0] = dst[1] = dst[2] = (uchar)0; // should not get here
            }
            else if( idx == 1 )  //异常情况
            {
                //green      first
                dst[0] = (uchar)255;
                t_body[0] = src_pic[0];
                t_body[1] = src_pic[1];
                t_body[2] = src_pic[2];
            }
            else if( idx == 2 )  //异常情况
            {
                //blue        second
                dst[1] = (uchar)255;
            }
            else if( idx == 3 )  //异常情况
            {
                //red         third
                dst[2] = (uchar)255;
            }
            else //正常情况
            {
                uchar* ptr = color_tab->data.ptr + (idx-1)*3;
                dst[0] = ptr[0]; dst[1] = ptr[1]; dst[2] = ptr[2];
            }
        }

    cvShowImage("img_gray",img_gray);
    cvShowImage("wshed",wshed);
    cvAddWeighted( wshed, 0.5, img_gray, 0.5, 0, wshed );
                //wshed.x.y=0.5*wshed.x.y+0.5*img_gray+0加权融合图像
                //cvShowImage("swhed",wshed);
                //cvShowImage("img_gray",img_gray);
                //cvShowImage( "watershed transform", wshed );
                //cvShowImage("img_final",img_final);
    cvShowImage( "watershed transform", wshed );

    img = Function::CjwIplToQImg(tongue_body);
    image = *img;

    cvReleaseMat( &color_tab );
    update();
    clear_list();
}

void MyLable::watersged()
{

}

void MyLable::clear_list()
{
    list.clear();
    list1.clear();
    list2.clear();
    list3.clear();
}

void MyLable::closeEvent(QCloseEvent *event)
{
        Function::deviceUninit();
        printf("close\n");
        event->accept();  //exit
}
