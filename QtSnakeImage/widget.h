#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    int display_caption( char* caption );
     int display_dst( int delay );

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
