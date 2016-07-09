#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
{
    exitButton=new QPushButton("Exit",this);
    startCapture=new QPushButton("Start Capture",this);
    stopCapture=new QPushButton("Stop Capture",this);

    connect(exitButton, SIGNAL(clicked()), this, SLOT(on_exit_clicked()));
    connect(startCapture, SIGNAL(clicked()), this, SLOT(onStartCapture()));
    connect(stopCapture, SIGNAL(clicked()), this, SLOT(onStopCapture()));

    video=new videowidget();
    video->show();

    vbox=new QVBoxLayout(this);
    vbox->addWidget(video);
    vbox->addWidget(startCapture);
    vbox->addWidget(stopCapture);
    vbox->addWidget(exitButton);

    setLayout(vbox);

    resize(1024,768);

    t=new CaptureThread(video);

}

MainWindow::~MainWindow()
{

}

void MainWindow::on_exit_clicked()
{
    qDebug("bye bye");
    exit(0);
}

void MainWindow::onStartCapture()
{
    if(t->devam==false){
        t->devam=true;
        t->start();
    }
}
void MainWindow::onStopCapture()
{
   t->devam=false;

}
