#include "mylable.h"
#include "function.h"
#include <stdio.h>
#include <QMessageBox>
MyLable::MyLable(QWidget *parent) : QLabel(parent)
{
    num_list = 0;
    video_or_pic = true;
    //初始化起始点，必须有
//    prev_pt =cvPoint (-1,-1);

//    storage = cvCreateMemStorage(0);
//    rng = cvRNG(-1);

    pp = (unsigned char *)malloc(640 * 480/*QWidget::width()*QWidget::height()*/* 3 * sizeof(char));
   // painter = new QPainter(this);
//    frame = new QImage(pp,800,480,QImage::Format_RGB888);
        frame = new QImage(pp,640,480,QImage::Format_RGB888);

//    frame = new QImage();

    vd = new VideoDevice(tr("/dev/video0"));

    connect(vd, SIGNAL(display_error(QString)), this,SLOT(display_error(QString)));
    rs = vd->open_device();
     qDebug("open_device");
    if(-1==rs)
    {
        QMessageBox::warning(this,tr("error"),tr("open /dev/dsp error"),QMessageBox::Yes);
        vd->close_device();
        qDebug("close_device");
    }

    rs = vd->init_device();
    qDebug("init_device");
    if(-1==rs)
    {
        QMessageBox::warning(this,tr("error"),tr("init failed"),QMessageBox::Yes);
        vd->close_device();
    }

    rs = vd->start_capturing();
    qDebug("start_capturing");
    if(-1==rs)
    {
        QMessageBox::warning(this,tr("error"),tr("start capture failed"),QMessageBox::Yes);
        vd->close_device();
    }

    if(-1==rs)
    {
        QMessageBox::warning(this,tr("error"),tr("get frame failed"),QMessageBox::Yes);
        vd->stop_capturing();
    }

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(up_date()));
    timer->start(30);

//connect(butten,SIGNAL(clicked(bool)),this,SLOT(save_pic()));
    //QHBoxLayout *hLayout = new QHBoxLayout();
    //hLayout->addWidget(label);
    //hLayout->addWidget(butten);
   // setLayout(hLayout);
    //setWindowTitle(tr("Capture"));



}
MyLable::~MyLable()
{
    rs = vd->stop_capturing();
    rs = vd->uninit_device();
    rs = vd->close_device();
}

/***********************************
 * 函  数  名：paintEvent(QPaintEvent *event)
 * 功        能：paintEvent函数
 * 参        数：QPaintEvent *event
 * 返  回  值：无
 *
 * ***********************************/
void MyLable::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

//    if( video_or_pic )
//    {
//        rs = vd->get_frame((void **)&p,&len);
//        convert_yuv_to_rgb_buffer(p,pp,640,480/*QWidget::width(),QWidget::height()*/);
//        frame->loadFromData((uchar *)pp,/*len*/640 * 480 * 3 * sizeof(char));
        painter.drawImage(0,0,*frame);
//        rs = vd->unget_frame();
//    QImage image("index.jpeg");
//    painter.drawImage(0,0,image);
    //    }
//    else
//    {
////        frame->save("picture.bmp");
////        qDebug("save pic!\n");
////        image.load("picture.bmp");
//        painter.drawImage(0,0,*frame);
//    }


//    image.load(set_pic_filename_path);
//    painter.drawImage(0,0,image,image.width(),image.height());
//      painter.drawImage(0,0,image);
//在选取的点上标号或圈出
//    painter.setPen(QPen(Qt::red,1));
//    painter.drawEllipse(x-3,y-3,6,6);

//    painter.setPen(QPen(Qt::blue,4));

//    if(!list.empty())
//    {
//        painter.drawLines(list);
//    }
//    if(!list1.empty())
//    {
//        painter.drawLines(list1);
//    }
//    if(!list2.empty())
//    {
//        painter.drawLines(list2);
//    }
//    if(!list3.empty())
//    {
//        painter.drawLines(list3);
//    }


}

/***********************************
 * 函  数  名：mousePressEvent(QMouseEvent *event)
 * 功        能：mousePressEvent函数，鼠标按下，记录位置event->x()，event->y()
 * 参        数：QMouseEvent *event
 * 返  回  值：无
 *
 * ***********************************/
//void MyLable::mousePressEvent(QMouseEvent *event)
//{
//    /*      contral mousemoveevent on or off    */
//    setMouseTracking(true);
//    if(event->button() == Qt::RightButton)
//    {
//        //获取点
//        x=event->x();
//        y=event->y();
//        qDebug("x:%d",x);
//        qDebug("y:%d\n",y);

//        prev_pt = cvPoint(event->x(),event->y());
//        update();
//    }
//}

/***********************************
 * 函  数  名：mouseMoveEvent(QMouseEvent *event)
 * 功        能：mouseMoveEvent函数，鼠标移动，记录鼠标用的路径
 * 参        数：QMouseEvent *event
 * 返  回  值：无
 *
 * ***********************************/
//void MyLable::mouseMoveEvent(QMouseEvent *event)
//{
//    /*      contral mousemoveevent on or off    */
//    //setMouseTracking(true);

//    //把鼠标移动的路径记录在QPoint里面
//    QPoint x_y;
//    x_y.setX(event->x());
//    x_y.setY(event->y());
//    switch (num_list) {
//    case 0:
//        list.append(x_y);
//        break;
//    case 1:
//        list1.append(x_y);
//        break;
//    case 2:
//        list2.append(x_y);
//        break;
//    case 3:
//        list3.append(x_y);
//        break;
//    default:
//        break;
//    }

//        CvPoint pt = cvPoint(event->x(),event->y());
//        if( prev_pt.x < 0 )
//        {
//            prev_pt = pt;
//        }
//        //CvScalar 成员：double val[4] RGBA值A=alpha
//        cvLine( marker_mask, prev_pt, pt, cvScalarAll(255), 5, 8, 0 );
//        cvLine( draw_img, prev_pt, pt, cvScalarAll(255), 5, 8, 0 );
//        prev_pt = pt;
////        cvShowImage("draw_img",draw_img);

//    update();
//}

/***********************************
 * 函  数  名：mouseReleaseEvent(QMouseEvent *event)
 * 功        能：mouseMoveEvent函数，鼠标松开，
 *                    记录鼠标松开的位置，以此来判断完成一次鼠标事件
 * 参        数：QMouseEvent *event
 * 返  回  值：无
 *
 * ***********************************/
//void MyLable::mouseReleaseEvent(QMouseEvent *event)
//{
//    /*      contral mousemoveevent on or off    */
//    setMouseTracking(false);
//    if(event->button() == Qt::LeftButton)
//    {
//        num_list = num_list + 1;
//        if(num_list == 4)
//            num_list = 0;
//    }

//    prev_pt = cvPoint(-1,-1);
//    update();
//}

///***********************************
// * 函  数  名：set_filename()
// * 功        能：
// *
// * 参        数：
// * 返  回  值：无
// *
// * ***********************************/
//void MyLable::set_filename()
//{
//    QString xgfilename;
//    xgfilename=QFileDialog::getOpenFileName(this,
//                                     tr("选择图像"),
//                                     "",
//                                     tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
//    if(xgfilename.isEmpty())
//    {
//        return;
//    }
//    else
//    {
//        set_pic_filename_path=xgfilename;
//        //*  clear  */
//        clear_list();
////        image.load(set_pic_filename_path);
//        std::string str = set_pic_filename_path.toStdString();
//        const char* ch = str.c_str();
//        printf("ch:",&ch);
//        qDebug()<<"ch:"<<&ch;
//        img0 = cvLoadImage(ch,1);
//        img = Function::CjwIplToQImg(img0);
//        image = *img;

////        cvShowImage("img0",img0);

//        markes = cvCreateImage(cvSize(image.width(),image.height()),IPL_DEPTH_32S, 1 );
//        marker_mask = cvCreateImage( cvSize(image.width(),image.height()), 8, 1 );
//        wshed = cvCloneImage( img0 );
//        img_gray = cvCloneImage( img0 );
//        draw_img = cvCloneImage( img0 );
//        tongue_body = cvCloneImage( img0 );
//        cvCvtColor( img0, marker_mask, CV_BGR2GRAY );
//        cvCvtColor( marker_mask, img_gray, CV_GRAY2BGR );
//        cvZero(markes);
//        cvZero(marker_mask);
//        cvZero( wshed );
//        cvZero(tongue_body);
//        update();
//    }
//    qDebug()<<"filenam:"<<xgfilename;
//    num = 0;
//}

///***********************************
// * 函  数  名：set_filename()
// * 功        能：
// *
// * 参        数：
// * 返  回  值：无
// *
// * ***********************************/
//void MyLable::set_scaled(QVector<QPoint> list, int num)
//{

//}

///***********************************
// * 函  数  名：set_filename()
// * 功        能：
// *
// * 参        数：
// * 返  回  值：无
// *
// * ***********************************/
//void MyLable::set_scaled_button()
//{
//    int i, j, comp_count = 0;
//    CvMat* color_tab = 0;
//    CvSeq* contours = 0;

//    cvSave("markes.xml",marker_mask);
//    cvClearMemStorage(storage);
//    cvFindContours( marker_mask, storage, &contours, sizeof(CvContour),
//                    CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
//    cvZero( markes );
//    for( ; contours != 0; contours = contours->h_next, comp_count++ )
//    {
//        //cvDrawContours(markers, contours, cvScalarAll(comp_count+1),
//            //cvScalarAll(comp_count+1), -1, -1, 8, cvPoint(0,0) );
//        cvDrawContours(markes, contours, cvScalarAll(comp_count+1),
//        cvScalarAll(comp_count+1), 1, -1, 8, cvPoint(0,0) );
//    }

//    color_tab = cvCreateMat( 1, comp_count, CV_8UC3 );//创建随机颜色列表
//    for( i = 0; i < comp_count; i++ )	//不同的整数标记
//    {
//        uchar* ptr = color_tab->data.ptr + i*3;
//        ptr[0] = (uchar)(cvRandInt(&rng)%180 + 50);
//        ptr[1] = (uchar)(cvRandInt(&rng)%180 + 50);
//        ptr[2] = (uchar)(cvRandInt(&rng)%180 + 50);
//    }

//    double t = (double)cvGetTickCount();
//    cvSave("img0.xml",markes);
//    cvWatershed( img0, markes );
//    cvSave("img1.xml",markes);
////                cvShowImage("imgo",img0);
//    t = (double)cvGetTickCount() - t;
//    printf( "exec time = %gms\n", t/(cvGetTickFrequency()*1000.) );

//    /*********/
//    for( i = 0; i < markes->height; i++ )
//        for( j = 0; j < markes->width; j++ )
//        {
//            int idx = CV_IMAGE_ELEM( markes, int, i, j );//markers的数据类型为IPL_DEPTH_32S
//            uchar* dst = &CV_IMAGE_ELEM( wshed, uchar, i, j*3 );//BGR三个通道的数是一起的,故要j*3
//            uchar* t_body = &CV_IMAGE_ELEM( tongue_body, uchar, i, j*3 );
//            uchar* src_pic = &CV_IMAGE_ELEM( img0, uchar, i, j*3 );
//            if( idx == -1 ) //输出时若为-1，表示各个部分的边界
//            {
//                //dst[0] = dst[1] = dst[2] = (uchar)128;
//                dst[2] = (uchar)255;
//            }
//            else if( idx <= 0 || idx > comp_count )  //异常情况
//            {
//                //dst[0] = dst[1] = dst[2] = (uchar)0; // should not get here
//            }
//            else if( idx == 1 )  //异常情况
//            {
//                //green      first
//                dst[0] = (uchar)255;
//                t_body[0] = src_pic[0];
//                t_body[1] = src_pic[1];
//                t_body[2] = src_pic[2];
//            }
//            else if( idx == 2 )  //异常情况
//            {
//                //blue        second
//                dst[1] = (uchar)255;
//            }
//            else if( idx == 3 )  //异常情况
//            {
//                //red         third
//                dst[2] = (uchar)255;
//            }
//            else //正常情况
//            {
//                uchar* ptr = color_tab->data.ptr + (idx-1)*3;
//                dst[0] = ptr[0]; dst[1] = ptr[1]; dst[2] = ptr[2];
//            }
//        }

////    cvShowImage("img_gray",img_gray);
////    cvShowImage("wshed",wshed);
//    cvAddWeighted( wshed, 0.5, img_gray, 0.5, 0, wshed );
//                //wshed.x.y=0.5*wshed.x.y+0.5*img_gray+0加权融合图像
//                //cvShowImage("swhed",wshed);
//                //cvShowImage("img_gray",img_gray);
//                //cvShowImage( "watershed transform", wshed );
//                //cvShowImage("img_final",img_final);
////    cvShowImage( "watershed transform", wshed );

//    img = Function::CjwIplToQImg(tongue_body);
//    image = *img;

//    cvReleaseMat( &color_tab );
//    update();
//    clear_list();
//}


///***********************************
// * 函  数  名：set_filename()
// * 功        能：
// *
// * 参        数：
// * 返  回  值：无
// *
// * ***********************************/
//void MyLable::watersged()
//{

//}

///***********************************
// * 函  数  名：clear_list()
// * 功        能：
// *
// * 参        数：
// * 返  回  值：无
// *
// * ***********************************/
//void MyLable::clear_list()
//{
//    list.clear();
//    list1.clear();
//    list2.clear();
//    list3.clear();
//}


///***********************************
// * 函  数  名：closeEvent(QCloseEvent *event)
// * 功        能：
// *
// * 参        数：
// * 返  回  值：无
// *
// * ***********************************/
//void MyLable::closeEvent(QCloseEvent *event)
//{
//        printf("close\n");
//        event->accept();  //exit
//}


//void MyLable::display_error(QString err)
//{
//    QMessageBox::warning(this,tr("error"), err,QMessageBox::Yes);
//}


///***********************************
// * 函  数  名：convert_yuv_to_rgb_buffer(unsigned char *yuv,
// *                                                                  unsigned char *rgb,
// *                                                                  unsigned int width,
// *                                                                  unsigned int height)
// * 功        能：把采集的yuv视频数据转换为rgb用于保存图片和显示
// *
// * 参        数：unsigned char *yuv
// *                     unsigned char *rgb
// *                     unsigned int width,
// *                     unsigned int height
// * 返  回  值：0
// *
// * ***********************************/
int MyLable::convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height)
{
 unsigned int in, out = 0;
 unsigned int pixel_16;
 unsigned char pixel_24[3];
 unsigned int pixel32;
 int y0, u, y1, v;
 for(in = 0; in < width * height * 2; in += 4) {
  pixel_16 =
   yuv[in + 3] << 24 |
   yuv[in + 2] << 16 |
   yuv[in + 1] <<  8 |
   yuv[in + 0];
  y0 = (pixel_16 & 0x000000ff);
  u  = (pixel_16 & 0x0000ff00) >>  8;
  y1 = (pixel_16 & 0x00ff0000) >> 16;
  v  = (pixel_16 & 0xff000000) >> 24;
  pixel32 = convert_yuv_to_rgb_pixel(y0, u, v);
  pixel_24[0] = (pixel32 & 0x000000ff);
  pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
  pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
  rgb[out++] = pixel_24[0];
  rgb[out++] = pixel_24[1];
  rgb[out++] = pixel_24[2];
  pixel32 = convert_yuv_to_rgb_pixel(y1, u, v);
  pixel_24[0] = (pixel32 & 0x000000ff);
  pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
  pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
  rgb[out++] = pixel_24[0];
  rgb[out++] = pixel_24[1];
  rgb[out++] = pixel_24[2];
 }
 return 0;
}

///***********************************
// * 函  数  名：convert_yuv_to_rgb_pixel(int y, int u, int v)
// * 功        能：
// *
// * 参        数：int y, int u, int v
// * 返  回  值：pixel32
// *
// * ***********************************/
int MyLable::convert_yuv_to_rgb_pixel(int y, int u, int v)
{
 unsigned int pixel32 = 0;
 unsigned char *pixel = (unsigned char *)&pixel32;
 int r, g, b;
 r = y + (1.370705 * (v-128));
 g = y - (0.698001 * (v-128)) - (0.337633 * (u-128));
 b = y + (1.732446 * (u-128));
 if(r > 255) r = 255;
 if(g > 255) g = 255;
 if(b > 255) b = 255;
 if(r < 0) r = 0;
 if(g < 0) g = 0;
 if(b < 0) b = 0;
 pixel[0] = r * 220 / 256;
 pixel[1] = g * 220 / 256;
 pixel[2] = b * 220 / 256;
 return pixel32;
}

void MyLable::show_picture()
{
    //
//    video_or_pic = false;

    //定时器暂停
//    timer->stop();

    //更新下，先显示图片
//    update();

    //保存图片
    frame->save("picture.bmp");
    qDebug("save pic!\n");

 /*****/

    //*  clear  */
//    clear_list();

//    img0 = cvLoadImage("picture.bmp",1);
//    img = Function::CjwIplToQImg(img0);
//    image = *img;

////    cvShowImage("img0",img0);

//    markes = cvCreateImage(cvSize(image.width(),image.height()),IPL_DEPTH_32S, 1 );
//    marker_mask = cvCreateImage( cvSize(image.width(),image.height()), 8, 1 );

//    wshed = cvCloneImage( img0 );
//    img_gray = cvCloneImage( img0 );
//    draw_img = cvCloneImage( img0 );
//    tongue_body = cvCloneImage( img0 );

//    cvCvtColor( img0, marker_mask, CV_BGR2GRAY );
//    cvCvtColor( marker_mask, img_gray, CV_GRAY2BGR );

//    cvZero(markes);
//    cvZero(marker_mask);
//    cvZero( wshed );
//    cvZero(tongue_body);

//    num = 0;
//  /*****/

//    update();
}

void MyLable::up_date()
{
        rs = vd->get_frame((void **)&p,&len);
        convert_yuv_to_rgb_buffer(p,pp,640,480/*QWidget::width(),QWidget::height()*/);
        frame->loadFromData((uchar *)pp,/*len*/640 * 480 * 3 * sizeof(char));
        vd->unget_frame();
        qDebug("sss\n");
        update();
}
