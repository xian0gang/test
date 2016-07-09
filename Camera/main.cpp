#include <QtGui/QApplication>
#include "camera.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Camera w;
//      w.setWindowFlags(w.windowFlags()& ~Qt::WindowMaximizeButtonHint& ~Qt::WindowMinimizeButtonHint );
    w.show();
    
    return a.exec();
}
