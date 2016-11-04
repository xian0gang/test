#include "widget.h"
#include <QApplication>

void loadMySqlDriver()
{
    QPluginLoader loader;
    loader.setFileName("/opt/qtsdk-2010.04/qt/plugins/sqldrivers/libqsqlmysql.so");
    qDebug()<<loader.load();
    qDebug()<<loader.errorString();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    loadMySqlDriver();
    w.setWindowTitle(QString("舌色标定系统"));
    w.show();

    return a.exec();
}
