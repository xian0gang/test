#ifndef WIDGET_H
#define WIDGET_H

#include <QDebug>
#include <QWidget>
//void send_filename(QString filename);


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

signals:
    void send_filename(QString filename);
    void send_set_scale();

public slots:
    void OpenFile();
    void close_pushbutton();
    void set_scale_pushbutton();

private:
    Ui::Widget *ui;
    QString filename;
};

#endif // WIDGET_H
