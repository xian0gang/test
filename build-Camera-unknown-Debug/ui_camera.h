/********************************************************************************
** Form generated from reading UI file 'camera.ui'
**
** Created: Sat Mar 5 17:07:46 2016
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERA_H
#define UI_CAMERA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Camera
{
public:
    QPushButton *pbt_start;
    QPushButton *pbt_stop;
    QPushButton *pushButton;

    void setupUi(QWidget *Camera)
    {
        if (Camera->objectName().isEmpty())
            Camera->setObjectName(QString::fromUtf8("Camera"));
        Camera->resize(640, 480);
        pbt_start = new QPushButton(Camera);
        pbt_start->setObjectName(QString::fromUtf8("pbt_start"));
        pbt_start->setGeometry(QRect(70, 10, 93, 27));
        pbt_stop = new QPushButton(Camera);
        pbt_stop->setObjectName(QString::fromUtf8("pbt_stop"));
        pbt_stop->setGeometry(QRect(190, 10, 93, 27));
        pushButton = new QPushButton(Camera);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(310, 10, 99, 27));

        retranslateUi(Camera);

        QMetaObject::connectSlotsByName(Camera);
    } // setupUi

    void retranslateUi(QWidget *Camera)
    {
        Camera->setWindowTitle(QApplication::translate("Camera", "Camera", 0, QApplication::UnicodeUTF8));
        pbt_start->setText(QApplication::translate("Camera", "start", 0, QApplication::UnicodeUTF8));
        pbt_stop->setText(QApplication::translate("Camera", "stop", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Camera", "PushButton", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Camera: public Ui_Camera {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERA_H
