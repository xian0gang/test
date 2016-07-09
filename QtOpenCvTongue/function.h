#ifndef FUNCTION_H
#define FUNCTION_H
#include <QImage>
#include <fcntl.h>
//#include <opencv2/core.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
//#include <linux/videodev2.h>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#define CLEAR(x) memset (&(x), 0, sizeof (x))

class Function
{
public:
//    Function();
    static QImage* CjwIplToQImg(const IplImage *cvImg);
    static void deviceOpen(void);
    static void errno_exit(const char* s);
    static int xioctl(int fd, int request, void* argp);
//    static void deviceOpen(void);
    static void deviceInit(void);
    static void mmapInit(void);
    static void captureStart(void);
    static void mainLoop(unsigned char* src);
    static  unsigned char* frameRead(unsigned char* src);
    static unsigned char* imageProcess(const void* p,unsigned char* dst,int len);

     static void deviceUninit(void);
private:

};

#endif // FUNCTION_H
