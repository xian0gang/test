#include "camera.h"
#include "ui_camera.h"

#include <assert.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <QCloseEvent>
#include <QDebug>

static int fd = -1;
struct buffer
{
    void *  start;
    size_t  length;
};

struct buffer *       buffers = NULL;
static unsigned int   n_buffers = 0;
static unsigned int   width = 800;
static unsigned int   height = 480;
//static unsigned int   width = 1024;
//static unsigned int   height = 768;
static char* deviceName = "/dev/video1";

char image_buf[1536000+54];

unsigned char bmp_h[] =
{
                0x42,0x4d,            //BM
                0x42,0x70,0x17,0x00,  // 172000+66
                0x00,0x00,0x00,0x00,
                0x36,0x00,0x00,0x00,  //bmp_data offset
                0x28,0x00,0x00,0x00,
                0x20,0x03,0x00,0x00,   //width
                0xE0,0x01,0x00,0x00,   //hieght
                0x01,0x00,
                0x20,0x00,             //32bit
                0x00,0x00,0x00,0x00,
                0x00,0x70,0x17,0x00,  //bmp_data size
                0x00,0x00,0x00,0x00,
                0x00,0x00,0x00,0x00,
                0x00,0x00,0x00,0x00,
                0x00,0x00,0x00,0x00,
};

Camera::Camera(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Camera)
{
      ui->setupUi(this);

      timer = new QTimer;

      this->imageWidget = new ImageWidget(this);
      this->imageWidget->setBackgroundRole(QPalette::Dark);
      this->imageWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
      this->imageWidget->setObjectName(QString::fromUtf8("imageWidget"));
//      this->imageWidget->setGeometry(QRect(5, 50, 491, 371));
      this->imageWidget->setGeometry(QRect(5, 50, 800, 480));

      connect(this->timer, SIGNAL(timeout()), this, SLOT(up_date()));
      deviceOpen();
      deviceInit();
      captureStart();

//      yuyv2rgb();
}

Camera::~Camera()
{
      delete ui;
}

void Camera::closeEvent(QCloseEvent *event)
{
        deviceUninit();
        event->accept();  //exit
}

void deviceOpen(void)
{
       fd = open(deviceName, O_RDWR | O_NONBLOCK, 0);

       if (-1 == fd)
       {
            QMessageBox::about(NULL, "About", "camera open error");
            exit(EXIT_FAILURE);
       }
}


/*  initialize device*/

void deviceInit(void)
{
    struct v4l2_capability cap;
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    struct v4l2_format fmt;
    unsigned int min;

     if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap))
     {
            if (EINVAL == errno)
            {
                 QMessageBox::about(NULL,"Information"," no V4L2 device");
                 exit(EXIT_FAILURE);
            }
           else
            {
                 errno_exit("VIDIOC_QUERYCAP");
            }
      }
//     else
//     {
//     ///    2016/2/22 add
//         printf("Driver Name:%s\nCard Name:%s\nBus info:%s\nDriver Version:%u.%u.%u\n", cap.driver, cap.card, cap.bus_info, (cap.version>>16)&0XFF, (cap.version>>8)&0XFF, cap.version&0XFF);
//     }

      if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
      {
             QMessageBox::about(NULL,"Information"," no video capture device");
             exit(EXIT_FAILURE);
      }

      struct v4l2_input input;
      input.index = 0;
      if ( ioctl(fd, VIDIOC_ENUMINPUT, &input) != 0)
      {
            QMessageBox::about(NULL,"Information","set input error");
            exit(0);
      }

      if ((ioctl(fd, VIDIOC_S_INPUT, &input)) < 0)
      {
            QMessageBox::about(NULL,"Information","set s_input error");
             exit(0);
       }

       CLEAR(cropcap);

       cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

      if (0 == xioctl(fd, VIDIOC_CROPCAP, &cropcap))
      {
           crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
           crop.c = cropcap.defrect; /* reset to default */

           if (-1 == xioctl(fd, VIDIOC_S_CROP, &crop))
           {
                switch (errno)
                {
                     case EINVAL:
                          break;
                     default:
                        break;
                }
            }
       }

      CLEAR (fmt);

       // v4l2_format
       fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
       fmt.fmt.pix.width       = width;
       fmt.fmt.pix.height      = height;
       //   RGB32 and MJPEG can use 
       fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB32;
//       fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
//              fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;

       fmt.fmt.pix.field       = V4L2_FIELD_ANY;

      if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
          errno_exit("VIDIOC_S_FMT");

      /* Note VIDIOC_S_FMT may change width and height.*/
      if (width != fmt.fmt.pix.width)
      {
           width = fmt.fmt.pix.width;
          //fprintf(stderr,"Image width set to %i by device %s.\n",width,deviceName);
       }
      if (height != fmt.fmt.pix.height)
      {
           height = fmt.fmt.pix.height;
           //fprintf(stderr,"Image height set to %i by device %s.\n",height,deviceName);
      }

      /*Buggy driver paranoia. */
      min = fmt.fmt.pix.width * 2;
      if (fmt.fmt.pix.bytesperline < min)
         fmt.fmt.pix.bytesperline = min;

       min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
      if (fmt.fmt.pix.sizeimage < min)
          fmt.fmt.pix.sizeimage = min;

      /***** 2016/2/22 add *****************************
      ioctl(fd, VIDIOC_G_FMT, &fmt);

      printf("Current data format information:\n\twidth:%d\n\theight:%d\n", fmt.fmt.pix.width,fmt.fmt.pix.height);

      struct v4l2_fmtdesc fmtdesc;
      fmtdesc.index=0;
      fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
      while(ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc)!=-1)
      {
          if(fmtdesc.pixelformat & fmt.fmt.pix.pixelformat)
          {
              printf("\tformat:%s\n",fmtdesc.description);
//              break;
          }
            fmtdesc.index++;
      }
      *****************************/
       mmapInit();
}

void mmapInit(void)
{
        struct v4l2_requestbuffers req;
        CLEAR (req);

        req.count = 4;
        req.type  = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory= V4L2_MEMORY_MMAP;

        if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req))
        {
                if (EINVAL == errno)
                {
                        QMessageBox::about(NULL,"Information","does not support memory mapping");
                        exit(EXIT_FAILURE);
                 }
                else
                {
                         errno_exit("VIDIOC_REQBUFS");
                }
        }

        if (req.count < 2)
        {
                QMessageBox::about(NULL,"Information","nsufficient buffer memory on device");
                exit(EXIT_FAILURE);
        }

        buffers = (struct buffer*)calloc(req.count, sizeof(*buffers));

        if (!buffers)
        {
             QMessageBox::about(NULL,"Information","Out of memory");
             exit(EXIT_FAILURE);
        }

        for (n_buffers = 0; n_buffers < req.count; ++n_buffers)
        {
                struct v4l2_buffer buf;

                CLEAR (buf);

                buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory      = V4L2_MEMORY_MMAP;
                buf.index       = n_buffers;

               if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
                errno_exit("VIDIOC_QUERYBUF");

                buffers[n_buffers].length = buf.length;

                buffers[n_buffers].start =
                   mmap (NULL /* start anywhere */, buf.length, PROT_READ | PROT_WRITE /* required */, MAP_SHARED /* recommended */, fd, buf.m.offset);

               if (MAP_FAILED == buffers[n_buffers].start)
                   errno_exit("mmap");
        }
}

void captureStart(void)
{
        unsigned int i;
        enum v4l2_buf_type type;

        for (i = 0; i < n_buffers; ++i)
        {
                struct v4l2_buffer buf;

                CLEAR (buf);
                buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory      = V4L2_MEMORY_MMAP;
                buf.index       = i;

                if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
                        errno_exit("VIDIOC_QBUF");
        }

        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
           errno_exit("VIDIOC_STREAMON");
}

void mainLoop(unsigned char* src)
{
    unsigned int count;
    count = 1;

    while (count-- > 0)
    {
            for (;;)
            {
                    fd_set fds;
                    struct timeval tv;
                    int r;

                    FD_ZERO(&fds);
                    FD_SET(fd, &fds);

                    /* Timeout. */
                    tv.tv_sec = 2;
                    tv.tv_usec = 0;

                    r = select(fd + 1, &fds, NULL, NULL, &tv);

                    if (-1 == r)
                    {
                            if (EINTR == errno)
                                    continue;
                            errno_exit("select");
                    }

                    if (0 == r)
                    {
                           QMessageBox::about(NULL,"Information","select timeout");
                            exit(EXIT_FAILURE);
                     }
                    if (frameRead(src))
                             break;
            }
       }
}

unsigned char* frameRead(unsigned char* src)
{
    struct v4l2_buffer buf;
//printf("xiangang1\n");
    CLEAR (buf);

    printf("length1:%d\n",buffers[buf.index].length);
    printf("length2:%d\n",buf.length);

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
//    printf("xiangang2\n");
    if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf))
    {
            switch (errno)
            {
                    case EAGAIN:
                            return 0;

                    case EIO:
                    default:
                         errno_exit("VIDIOC_DQBUF");
            }
    }

    assert (buf.index < n_buffers);

    imageProcess(buffers[buf.index].start,src);
 //  printf("frameR  = %d\n",buffers[buf.index].start);
//printf("xiangang3\n");
//    FILE *file_fd;
//    file_fd = fopen("test-mmap.bmp","w");
//    fwrite(buffers[buf.index].start,buffers[buf.index].length, 1, file_fd); //将其写入文件中


    if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
            errno_exit("VIDIOC_QBUF");

   // return (unsigned char*)n;

}

unsigned char* imageProcess(const void* p,unsigned char* dst)
{
        unsigned char* src = (unsigned char*)p;

        memcpy(dst,bmp_h,54);
        memcpy(dst+54,src,800*480*4);

        return (unsigned char*)image_buf;

}

void deviceUninit(void)
{
     unsigned int i;

     for (i = 0; i < n_buffers; ++i)
     {
         if (-1 == munmap (buffers[i].start, buffers[i].length))
             errno_exit("munmap");
      }

     free(buffers);
     //close(fd);
}

void errno_exit(const char* s)
{
        //fprintf(stderr, "%s error %d, %s\n", s, errno, strerror (errno));
        exit(EXIT_FAILURE);
}


int xioctl(int fd, int request, void* argp)
{
        int r;
        do r = ioctl(fd, request, argp);
        while (-1 == r && EINTR == errno);

        return r;
}

void Camera::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Camera::up_date()
{
    unsigned char image_buf[1536000+54];
    frameRead(image_buf);
    this->imageWidget->setPixmap(image_buf);
}


void Camera::on_pbt_start_clicked()
{
     timer->start(100);
     qDebug("start~~~~~~\n");
}

void Camera::on_pbt_stop_clicked()
{
     timer->stop();
     this->imageWidget->savepic();
     qDebug("stop\n");
}

//void Camera::read_frame()
//{
//    struct v4l2_buffer buf;
//    unsigned int i;
//    CLEAR(buf);
//    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//    buf.memory  = V4L2_MEMORY_MMAP;
//    ioctl(fd,VIDIOC_DQBUF,&buf);
//    assert(buf.index<4);
//    printf("buf.index dq is %d\n",buf.index);
//    fwrite(buffers[buf.index].start, buffers[buf.index].length, 1, file_fd); //将其写入文件中

//    ioctl (fd, VIDIOC_QBUF, &buf); //再将其入列
////    return 1;
//}

void Camera::on_pushButton_clicked()
{
//    file_fd = fopen("test-mmap.jpg","w");
//    read_frame();
    unsigned char image_buf[1536000+54];
    frameRead(image_buf);


}



