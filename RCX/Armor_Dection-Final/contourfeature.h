
#ifndef CONTOURFEATURE_H
#define CONTOURFEATURE_H

#include "configure.h"

bool isarmoredColor(Mat color_roi)
{
    Mat hsv_roi;
    cvtColor(color_roi,hsv_roi,COLOR_BGR2HSV);
    bool is_color = 0;
    double H=0.0,S=0.0,V=0.0;
    int x,y;
    int flag = 0;
    for(x = 0;x < color_roi.cols; ++x)
    {
        for(y = 0;y < color_roi.rows; ++y)
        {
            H = hsv_roi.at<Vec3b>(y,x)[0];
            S = hsv_roi.at<Vec3b>(y,x)[1];
            V = hsv_roi.at<Vec3b>(y,x)[2];
            //red
            if(armor_color == 0)
            {
                if((H>=145 && H<180)||(H>=0 && H<=13))
                {   if(S >= 135 && S <= 255)
                    {   if(V > 148 && V <= 255)
                        {
                            flag += 1;
                        }
                    }
                }
            }
            else
            {   //blue
                if(H>=75 && H<=120)
                {   if(S >= 195 && S <= 255)
                    {   if(V >= 185 && V <= 255)
                        {
                            flag += 1;
                        }
                    }
                }
            }

            if(armor_color == 0)
            {
                if((flag / color_roi.cols*color_roi.rows) > 0.5)
                {
                    is_color = 1;
                    continue;
                }
            }
            else
            {
                if((flag / color_roi.cols*color_roi.rows) > 0.5)
                {
                    is_color = 1;
                    continue;
                }
            }
        }
    }
    return is_color;
}

float centerDistance(Point p1,Point p2)
{
    float D = sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
    return D;
}

int lightState(const RotatedRect &rect)
{
    int w;
    int h;
    if (rect.size.height > rect.size.width)
    {
        h = rect.size.height;
        w = rect.size.width;
    }
    else
    {
        w = rect.size.height;
        h = rect.size.width;
    }

    float ratio = (float)w/(float)h;
    if(ratio <= 0.2)
    {
        return 1;
    }
    else if (ratio > 0.2 && ratio <= 0.25)
    {
        return 2;
    }
    else if (ratio > 0.25 && ratio <= 0.3)
    {
        return 3;
    }
    else if (ratio > 0.3 && ratio <= 0.4)
    {
        return 4;
    }
    else
    {
        return 5;
    }
}

bool catchState(float ratio,int value)
{
    bool is;
    switch (value)
    {
    case 1:
        if (ratio < 0.7)
            is = true;
        else
            is = false;
        break;
    case 2:
        if (ratio < 0.8)
            is = true;
        else
            is = false;
        break;
    case 3:
        if (ratio < 0.9)
            is = true;
        else
            is = false;
        break;
    case 4:
        if (ratio < 1)
            is = true;
        else
            is = false;
        break;
    default:
        break;
    }
    return is;
}

void getcolorROI(Mat src,const RotatedRect &rect, Mat roi)
{
    Point2f verices[4];
    Point2f verdst[4];
    int roi_w;
    int roi_h;
    rect.points(verices);
    if(rect.size.width > rect.size.height)
    {
        roi_w = rect.size.height;
        roi_h = rect.size.width;
        verdst[0] = Point2f(0,roi_h);
        verdst[1] = Point2f(0,0);
        verdst[2] = Point2f(roi_w,0);
        verdst[3] = Point2f(roi_w,roi_h);
    }
    else
    {
        roi_w = rect.size.width;
        roi_h = rect.size.height;
        verdst[0] = Point2f(roi_w,roi_h);
        verdst[1] = Point2f(0,roi_h);
        verdst[2] = Point2f(0,0);
        verdst[3] = Point2f(roi_w,0);
    }

    roi = Mat(roi_h,roi_w,CV_8UC1);
    Mat warpMatrix = getPerspectiveTransform(verices,verdst);
    warpPerspective(src,roi,warpMatrix,roi.size(),INTER_LINEAR, BORDER_CONSTANT);
}

#endif // CONTOURFEATURE_H
