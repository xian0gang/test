#include "level.h"

void initializePhi(Mat img, int iterNum, Rect boxPhi)
{
    //boxPhi是前景区域
    m_iterNum = iterNum;
    cvtColor(img, m_mImage, CV_BGR2GRAY);

    m_iCol = img.cols;
    m_iRow = img.rows;
    m_depth = CV_32FC1;

    //显式分配内存
    m_mPhi = Mat::zeros(m_iRow, m_iCol, m_depth);
    m_mDirac = Mat::zeros(m_iRow, m_iCol, m_depth);
    m_mHeaviside = Mat::zeros(m_iRow, m_iCol, m_depth);

    //初始化惩罚性卷积核
    m_mK = (Mat_<float>(3, 3) << 0.5, 1, 0.5,
        1, -6, 1,
        0.5, 1, 0.5);

    int c = 2;
    for (int i = 0; i < m_iRow; i++)
    {
        for (int j = 0; j < m_iCol; j++)
        {
            if (i<boxPhi.y || i>boxPhi.y + boxPhi.height || j<boxPhi.x || j>boxPhi.x + boxPhi.width)
            {
                m_mPhi.at<float>(i, j) = -c;
            }
            else
            {
                m_mPhi.at<float>(i, j) = c;
            }
        }
    }
}

void Dirac()
{
    //狄拉克函数
    float k1 = m_epsilon / CV_PI;
    float k2 = m_epsilon*m_epsilon;
    for (int i = 0; i < m_iRow; i++)
    {
        float *prtDirac = &(m_mDirac.at<float>(i, 0));
        float *prtPhi = &(m_mPhi.at<float>(i, 0));

        for (int j = 0; j < m_iCol; j++)
        {
            float *prtPhi = &(m_mPhi.at<float>(i, 0));
            prtDirac[j] = k1 / (k2 + prtPhi[j] * prtPhi[j]);
        }
    }
}

void Heaviside()
{
    //海氏函数
    float k3 = 2 / CV_PI;
    for (int i = 0; i < m_iRow; i++)
    {
        float *prtHeaviside = (float *)m_mHeaviside.ptr(i);
        float *prtPhi = (float *)m_mPhi.ptr(i);

        for (int j = 0; j < m_iCol; j++)
        {
            prtHeaviside[j] = 0.5 * (1 + k3 * atan(prtPhi[j] / m_epsilon));
        }
    }
}

void Curvature()
{
    //计算曲率
    Mat dx, dy;
    Sobel(m_mPhi, dx, m_mPhi.depth(), 1, 0, 1);
    Sobel(m_mPhi, dy, m_mPhi.depth(), 0, 1, 1);

    for (int i = 0; i < m_iRow; i++)
    {
        float *prtdx = (float *)dx.ptr(i);
        float *prtdy = (float *)dy.ptr(i);
        for (int j = 0; j < m_iCol; j++)
        {
            float val = sqrtf(prtdx[j] * prtdx[j] + prtdy[j] * prtdy[j] + 1e-10);
            prtdx[j] = prtdx[j] / val;
            prtdy[j] = prtdy[j] / val;
        }
    }
    Mat ddx, ddy;
    Sobel(dx, ddy, m_mPhi.depth(), 0, 1, 1);
    Sobel(dy, ddx, m_mPhi.depth(), 1, 0, 1);
    m_mCurv = ddx + ddy;
}

void BinaryFit()
{
    //先计算海氏函数
    Heaviside();

    //计算前景与背景灰度均值
    float sumFG = 0;
    float sumBK = 0;
    float sumH = 0;
    //float sumFH = 0;
    Mat temp = m_mHeaviside;
    Mat temp2 = m_mImage;
    float fHeaviside;
    float fFHeaviside;
    float fImgValue;
    for (int i = 1; i < m_iRow; i++)
    {
        float *prtHeaviside = &(m_mHeaviside.at<float>(i, 0));
        uchar *prtImgValue = &(m_mImage.at<uchar>(i, 0));
        for (int j = 1; j < m_iCol; j++)
        {
            fImgValue = prtImgValue[j];
            fHeaviside = prtHeaviside[j];
            fFHeaviside = 1 - fHeaviside;

            sumFG += fImgValue*fHeaviside;
            sumBK += fImgValue*fFHeaviside;
            sumH += fHeaviside;
        }
    }
    m_FGValue = sumFG / (sumH + 1e-10);         //前景灰度均值
    m_BKValue = sumBK / (m_iRow*m_iCol - sumH + 1e-10); //背景灰度均值
}

Mat EVolution()
{
    float fCurv;
    float fDirac;
    float fPenalize;
    float fImgValue;

Mat showIMG;

    for (int i = 0; i < m_iterNum; i++)
    {
        Dirac();
        Curvature();
        BinaryFit();
        filter2D(m_mPhi, m_mPenalize, m_depth, m_mK, Point(1, 1));//惩罚项的△φ
        for (int i = 0; i < m_iRow; i++)
        {
            float *prtCurv = &(m_mCurv.at<float>(i, 0));
            float *prtDirac = &(m_mDirac.at<float>(i, 0));
            float *prtPenalize = &(m_mPenalize.at<float>(i, 0));
            uchar *prtImgValue = &(m_mImage.at<uchar>(i, 0));
            for (int j = 0; j < m_iCol; j++)
            {
                fCurv = prtCurv[j];
                fDirac = prtDirac[j];
                fPenalize = prtPenalize[j];
                fImgValue = prtImgValue[j];

                float lengthTerm = m_nu* fDirac * fCurv;                    //长度约束
                float penalizeTerm = m_mu*(fPenalize - fCurv);                  //惩罚项
                float areaTerm = fDirac * m_lambda1 *                       //全局项
                    (-((fImgValue - m_FGValue)*(fImgValue - m_FGValue))
                    + ((fImgValue - m_BKValue)*(fImgValue - m_BKValue)));

                m_mPhi.at<float>(i, j)=m_mPhi.at<float>(i, j)+m_timestep*(lengthTerm + penalizeTerm + areaTerm);
            }
        }

        //显示每一次演化的结果
        Mat showIMG;
        cvtColor(m_mImage, showIMG, CV_GRAY2BGR);
        Mat Mask = m_mPhi >= 0;   //findContours的输入是二值图像
        dilate(Mask, Mask, Mat(), Point(-1, -1), 3);
        erode(Mask, Mask, Mat(), Point(-1, -1), 3);
        vector<vector<Point> > contours;
        findContours(Mask,
        contours,// 轮廓点
        RETR_EXTERNAL,// 只检测外轮廓
        CHAIN_APPROX_NONE);// 提取轮廓所有点
        drawContours(showIMG, contours, -1, Scalar::all(255), 1);
//        imshow("showIMG", showIMG);
//        waitKey(1);
    }
    return showIMG;
}
