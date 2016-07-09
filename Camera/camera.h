#ifndef CAMERA_H
#define CAMERA_H

#include <QWidget>
#include <fcntl.h>
#include <QMessageBox>
#include "imagewidget.h"
//#include <linux/videodev2.h>
#include <QFileDialog>
#include <QFile>
#include <qfile.h>

#define CLEAR(x) memset (&(x), 0, sizeof (x))

namespace Ui
{
class Camera;
}

class Camera : public QWidget
{
    Q_OBJECT
    
public:
    explicit Camera(QWidget *parent = 0);
    ~Camera();

    QWidget *parent;
    class ImageWidget *imageWidget;

    QTimer *timer;
    void closeEvent(QCloseEvent *event);

    void read_frame(void);


protected:
    void changeEvent(QEvent *e);


private slots:
    void on_pbt_start_clicked();

    void on_pbt_stop_clicked();
    void up_date();

    void on_pushButton_clicked();

private:
    Ui::Camera *ui;    
    int n_len;
    unsigned char *newBuf;
    unsigned char *starter;
//    FILE *file_fd;

};
void errno_exit(const char* s);
int xioctl(int fd, int request, void* argp);
void deviceOpen(void);
void deviceInit(void);
void mmapInit(void);
void captureStart(void);
void mainLoop(unsigned char* src);
unsigned char* frameRead(unsigned char* src);
unsigned char* imageProcess(const void* p,unsigned char* dst);

inline int fb_init(void);
void deviceUninit(void);

void yuyv2rgb();



#endif // CAMERA_H
