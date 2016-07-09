#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <opencv2/core/cuda.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
       cv::Mat image;
     QImage img;

    int m_iterNum;      //迭代次数
    float m_lambda1;    //全局项系数
    float m_nu;     //长度约束系数ν
    float m_mu;     //惩罚项系数μ
    float m_timestep;   //演化步长δt
    float m_epsilon;    //规则化参数ε

//    int m_iterNum;

    //过程数据
    Mat m_mImage;       //源图像

    int m_iCol;     //图像宽度
    int m_iRow;     //图像高度
    int m_depth;        //水平集数据深度
    float m_FGValue;    //前景值
    float m_BKValue;    //背景值

    //初始化水平集
    void initializePhi(Mat img,  //输入图像
          int iterNum, //迭代次数
          Rect boxPhi);//前景区域
    Mat EVolution();   //演化

private slots:
    void on_pushButton_click();

    void on_pushButton_2_click();

    void on_pushButton_3_clicked();

private:
    Ui::Widget *ui;

    Mat m_mPhi;     //水平集：φ
    Mat m_mDirac;       //狄拉克处理后水平集：δ（φ）
    Mat m_mHeaviside;   //海氏函数处理后水平集：Н（φ）
    Mat m_mCurv;        //水平集曲率κ=div(▽φ/|▽φ|)
    Mat m_mK;       //惩罚项卷积核
    Mat m_mPenalize;    //惩罚项中的▽<sup>2</sup>φ

    void Dirac();       //狄拉克函数
    void Heaviside();   //海氏函数
    void Curvature();   //曲率
    void BinaryFit();   //计算前景与背景值
};

#endif // WIDGET_H
