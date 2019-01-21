#ifndef RM_BIG_CHYSANTHEMUM_H
#define RM_BIG_CHYSANTHEMUM_H

#include "configure.h"
#include "contourfeature.h"

class RMBigChrysanthemum
{
public:
    /*----------参数初始化----------*/
    Mat src_img;    //原图
    Mat gray_img;   //灰度图
    Mat hsv_img;    //HSV图
    Mat bin_img;    //二值图
    Mat dst_img;    //输出图
    int threshold_value = 20;
    /*----------参数初始化----------*/

public:
    RMBigChrysanthemum()
    {
        cout<<"Big Chrysanthemum ready"<<endl;
    }

    void imageProcessing(Mat src);
    void rectFilter();
};

void RMBigChrysanthemum::imageProcessing(Mat src)
{
    src_img = src;
    src_img.copyTo(dst_img);

    //--------------色彩分割	-----------------//
    cvtColor(src_img, gray_img, COLOR_BGR2GRAY);
    threshold(gray_img, bin_img, threshold_value, 255, THRESH_BINARY);

    //Canny(bin_img,bin_img,120,240);
    GaussianBlur(bin_img,bin_img,Size(7,7),0,0);
    medianBlur(bin_img, bin_img,5);
    Canny(bin_img,bin_img,120,240);
    Mat element = getStructuringElement(MORPH_RECT,Size(5,5));
    dilate(bin_img,bin_img,element);
}

void RMBigChrysanthemum::rectFilter()
{
    vector<RotatedRect> rotateRect;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(bin_img,contours,hierarchy,CV_RETR_LIST, CHAIN_APPROX_NONE, Point(0,0));
    for (int i = 0; i<(int)contours.size(); ++i)
    {
        if (contours.size() <= 1)
            break;
        RotatedRect R_rect_i = minAreaRect(contours[i]);
        int w;
        int h;
        if (R_rect_i.size.height >= R_rect_i.size.width)
        {
            h = R_rect_i.size.height;
            w = R_rect_i.size.width;
        }
        else
        {
            h = R_rect_i.size.width;
            w = R_rect_i.size.height;
        }
        float Area = w * h;

        if(1000 < Area && Area <5000)
        {
            Point2f vtx1[4];
            R_rect_i.points(vtx1);
            for(int ii = 0; ii < 4; ii++ )
            {
                line(dst_img, vtx1[ii], vtx1[(ii+1)%4], Scalar(255, 255, 0),2,8,0);
            }
            rotateRect.push_back(R_rect_i);
        }
    }

    for(int j1 = 0;j1<rotateRect.size();++j1)
    {
        if(rotateRect.size() <= 1)
            break;
        for(int j2 = j1+1;j2<rotateRect.size();++j2)
        {
            float x1 = rotateRect[j1].center.x;
            float y1 = rotateRect[j2].center.y;
            float x2 = rotateRect[j1].center.x;
            float y2 = rotateRect[j2].center.y;
            float distance = sqrt(pow(x1-x2,2)+pow(y1-y2,2));

            float ratio1 = (float)rotateRect[j1].size.width / (float)rotateRect[j1].size.height;
            float ratio2 = (float)rotateRect[j2].size.width / (float)rotateRect[j2].size.height;
            if(0 <= distance && distance < 20 && fabs(ratio1 - ratio2) < 0.2)
            {
//                Point2f vtx1[4];
//                rotateRect[j1].points(vtx1);
//                for(int ii = 0; ii < 4; ii++ )
//                {
//                    line(dst_img, vtx1[ii], vtx1[(ii+1)%4], Scalar(255, 255, 0),2,8,0);
//                }
//                Point2f vtx2[4];
//                rotateRect[j2].points(vtx2);
//                for(int ii = 0; ii < 4; ii++ )
//                {
//                    line(dst_img, vtx2[ii], vtx2[(ii+1)%4], Scalar(255, 255, 0),2,8,0);
//                }
                Point center;
                center.x = (rotateRect[j1].center.x + rotateRect[j1].center.x) /2;
                center.y = (rotateRect[j1].center.y + rotateRect[j1].center.y) /2;
                circle(dst_img,center,3,Scalar(255,255,0),2,8);
            }
        }
    }
    imshow("dst",dst_img);
    imshow("th",bin_img);
}

#endif // RM_BIG_CHYSANTHEMUM_H
