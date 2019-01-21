#ifndef RM_ARMOR_FITTED_H
#define RM_ARMOR_FITTED_H

#include "configure.h"
#include "contourfeature.h"
#include "matchandgroup.h"
#include "databuff.h"
#include "serialport.h"

class RMArmorFitted
{
private:
    /*----------参数初始化----------*/
    Mat src_img;    //原图
    Mat gray_img;   //灰度图
    Mat bin_img;    //二值图
    Mat dst_img;    //输出图
    int threshold_value_blue = 20;
    int threshold_Value_red = 80;
    /*----------参数初始化----------*/

public:
    RMArmorFitted()
    {
        cout<<"Armor is ready"<<endl;
    }
    void imageProcessing(Mat src);
    void lightFitted();
    SerialPort port;
};

void RMArmorFitted::imageProcessing(Mat src)
{
    src_img = src;
    int threshold_Value;
    if(armor_color == 0)
    {
        threshold_Value = threshold_Value_red;
    }
    else
    {
        threshold_Value = threshold_value_blue;
    }
    resize(src_img,src_img,Size(640,480),INTER_NEAREST);
    src_img.copyTo(dst_img);
    //--------------色彩分割	-----------------//
    cvtColor(src_img, gray_img, COLOR_BGR2GRAY);
    threshold(gray_img, bin_img, threshold_Value, 255, THRESH_BINARY);
    medianBlur(bin_img, bin_img,5);
    Canny(bin_img,bin_img,120,240);
}

void RMArmorFitted::lightFitted()
{
    /*----------参数初始化----------*/
    bool SuccessSend = false;   //初始化成功发送标为0　防止程序一开始就发送错误数据
    int SendBuf_COUNT = 0;    //ifSendSuccess
    /*----------参数初始化----------*/

    vector<vector<Point> > contours;
    vector<Rect> boundRect;
    vector<RotatedRect> rotateRect;
    vector<Vec4i> hierarchy;
    vector<Point2f> midPoint(2);
    vector<vector<Point2f> > midPoint_pair;
    //查找轮廓
    findContours(bin_img, contours, hierarchy, RETR_LIST, CHAIN_APPROX_NONE, Point(0,0));
    //第一遍过滤
    for (int i = 0; i < (int)contours.size(); ++i)
    {
        if (contours.size() <= 1)
            break;
        Rect B_rect_i = boundingRect(contours[i]);
        RotatedRect R_rect_i = minAreaRect(contours[i]);
        float ratio = (float)B_rect_i.width / (float)B_rect_i.height;
        bool H_W = false;
        if(B_rect_i.height >= B_rect_i.width)
        {
            H_W = catchState(ratio,lightState(R_rect_i));
            if (H_W)
            {
                boundRect.push_back(B_rect_i);
                rotateRect.push_back(R_rect_i);
            }
        }
    }
    float distance_max = 0.f;
    //第二遍两个循环匹配灯条
    for (int k1 = 0;k1<(int)rotateRect.size();++k1)
    {
        if(rotateRect.size()<=1)
            break;
        for (int k2 = k1+1;k2<(int)rotateRect.size();++k2)
        {
            /*面积比例限制*/
            if(lightFilter(rotateRect[k1],rotateRect[k2]))
            {
                /*中心距与最大宽比例限制*/
                if(rectDifferent(rotateRect[k1],rotateRect[k2]))
                {
                    float distance_temp = centerDistance(rotateRect[k1].center,rotateRect[k2].center);
                    /*中心距与最大高比例限制*/
                    if (distanceHeight(rotateRect[k1],rotateRect[k2]))
                    {
                        Mat roi_1;
                        getcolorROI(src_img,rotateRect[k1],roi_1);
                        Mat roi_2;
                        getcolorROI(src_img,rotateRect[k2],roi_2);
//                        /*ROI_1　获取旋转矩形ROI*/
//                        Point2f verices_1[4];
//                        Point2f verdst_1[4];
//                        int roi_w1;
//                        int roi_h1;
//                        if (rotateRect[k1].size.width > rotateRect[k1].size.height)
//                        {
//                            rotateRect[k1].points(verices_1);
//                            roi_w1 = rotateRect[k1].size.width;
//                            roi_h1 = rotateRect[k1].size.height;
//                            verdst_1[0] = Point2f(0,roi_h1);
//                            verdst_1[1] = Point2f(0,0);
//                            verdst_1[2] = Point2f(roi_w1,0);
//                            verdst_1[3] = Point2f(roi_w1,roi_h1);
//                        }
//                        else
//                        {
//                            rotateRect[k1].points(verices_1);
//                            roi_w1 = rotateRect[k1].size.height;
//                            roi_h1 = rotateRect[k1].size.width;
//                            verdst_1[0] = Point2f(roi_w1,roi_h1);
//                            verdst_1[1] = Point2f(0,roi_h1);
//                            verdst_1[2] = Point2f(0,0);
//                            verdst_1[3] = Point2f(roi_w1,0);
//                        }
//                        /*ROI_2　获取旋转矩形ROI*/
//                        Point2f verices_2[4];
//                        Point2f verdst_2[4];
//                        int roi_w2;
//                        int roi_h2;
//                        if (rotateRect[k2].size.width > rotateRect[k2].size.height)
//                        {
//                            rotateRect[k2].points(verices_2);
//                            roi_w2 = rotateRect[k2].size.width;
//                            roi_h2 = rotateRect[k2].size.height;
//                            verdst_2[0] = Point2f(0,roi_h2);
//                            verdst_2[1] = Point2f(0,0);
//                            verdst_2[2] = Point2f(roi_w2,0);
//                            verdst_2[3] = Point2f(roi_w2,roi_h2);
//                        }
//                        else
//                        {
//                            rotateRect[k2].points(verices_2);
//                            roi_w2 = rotateRect[k2].size.width;
//                            roi_h2 = rotateRect[k2].size.height;
//                            verdst_2[0] = Point2f(roi_w2,roi_h2);
//                            verdst_2[1] = Point2f(0,roi_h2);
//                            verdst_2[2] = Point2f(0,0);
//                            verdst_2[3] = Point2f(roi_w2,0);
//                        }
//                        Mat roi_1 = Mat(roi_h1,roi_w1,CV_8UC1);
//                        Mat warpMatrix1 = getPerspectiveTransform(verices_1,verdst_1);
//                        warpPerspective(dst_img,roi_1,warpMatrix1,roi_1.size(),INTER_LINEAR, BORDER_CONSTANT);
//                        Mat roi_2 = Mat(roi_h2,roi_w2,CV_8UC1);
//                        Mat warpMatrix2 = getPerspectiveTransform(verices_2,verdst_2);
//                        warpPerspective(dst_img,roi_2,warpMatrix2,roi_2.size(),INTER_LINEAR, BORDER_CONSTANT);
                        /*颜色检测*/
                        if(isarmoredColor(roi_1)==1)
                        {
                            if(isarmoredColor(roi_2)==1)
                            {
                                /*交换最大宽标记量*/
                                if(distance_temp >= distance_max)
                                {
                                    distance_max = distance_temp;
                                }

                                //imshow("roi",roi_1);
                                rectangle(dst_img,boundRect[k1].tl(), boundRect[k1].br(), Scalar(0,255,0),2,8,0);
                                //imshow("roi2",roi_2);
                                rectangle(dst_img,boundRect[k2].tl(), boundRect[k2].br(), Scalar(0,255,0),2,8,0);
                                midPoint[0].x = rotateRect[k1].center.x;
                                midPoint[0].y = rotateRect[k1].center.y;
                                midPoint[1].x = rotateRect[k2].center.x;
                                midPoint[1].y = rotateRect[k2].center.y;
                                midPoint_pair.push_back(midPoint);
                            }
                        }
                    }
                }
            }
        }
    }
    int Recoginition_FLAG = 0;    //初始化发送类型标记 0表示发送丢失判断数据
    int X_Widht;                  //声明数据量
    int Y_height;

    //第三遍求最优灯条
    for (int k3 = 0;k3<(int)midPoint_pair.size();++k3)
    {
        float distance = centerDistance(midPoint_pair[k3][0],midPoint_pair[k3][1]);
        float slope = fabs((midPoint_pair[k3][0].y-midPoint_pair[k3][1].y)/(midPoint_pair[k3][0].x-midPoint_pair[k3][1].x));
        if(distance >= distance_max)
        {
            if(slope <= 0.26)
            {
                line(dst_img,midPoint_pair[k3][0],midPoint_pair[k3][1],Scalar(0,0,255),2,8);
                int x1 = midPoint_pair[k3][0].x;
                int y1 = midPoint_pair[k3][0].y;
                int x2 = midPoint_pair[k3][1].x;
                int y2 = midPoint_pair[k3][1].y;
                Point mid_point = Point(int((x1 + x2)/2), int((y1 + y2)/2));
                //cout<<"x:"<<mid_point.x<<"   y:"<<mid_point.y;
                X_Widht = mid_point.x;
                Y_height = mid_point.y;

                x_buff = mid_point.x;    //更新缓存帧
                y_buff = mid_point.y;    //更新缓存帧

                Recoginition_FLAG = 2;      //重置标记　2 表示正常发送数据
                if(iscentralBUffer(src_img,mid_point))
                {
                    Recoginition_FLAG = 1;      //重置标记　1 表示位于中心区
                }
                SuccessSend = true;     //成功发送标记置　1
                cout<<"X"<<src_img.cols/2<<"  "<<"Y"<<src_img.rows/2<<endl;
                break;
            }
        }
    }
    /*判断串口是否开启*/
    if(serialisopen == 1)
    {
        /*获取发送类型标记,选择串口发送类型*/
        switch (Recoginition_FLAG)
        {
        case 0:
        {
            /*　判断上一次是否有数据成功发送　有则使用缓存,无则发送停止符*/
            if(SuccessSend == true)
            {
                int leftorright = missingFlag(src_img,Point(X_Widht,Y_height));
                switch(leftorright)
                {
                case 1:
                {
                    /*判断发送缓存帧是否小于一定次数*/
                    if(SendBuf_COUNT < bufftime)
                    {
                        int buff_reduce_value_x = abs(src_img.cols/2-x_buff);
                        int buff_reduce_value_y = abs(src_img.rows/2-y_buff);
                        int send_buff_x = bufferReturn(src_img,x_buff,buff_reduce_value_x,SendBuf_COUNT,0);
                        int send_buff_y = bufferReturn(src_img,y_buff,buff_reduce_value_y,SendBuf_COUNT,1);
                        port.RMSerialWrite(send_buff_x,send_buff_y,2);
                        SendBuf_COUNT += 1;
                        cout<<send_buff_x<<"  "<<send_buff_y<<"  ";
                        cout<<"send Buff"<<endl;
                    }
                    else
                    {
                        port.RMSerialWrite(X_Widht,Y_height,1);
                        cout<<"send None"<<endl;
                    }
                }
                    break;
                case 2:
                {
                    port.RMSerialWrite(X_Widht,Y_height,1);
                    cout<<"send None"<<endl;
                }
                    break;
                default:
                    break;
                }
            }
            else
            {
                port.RMSerialWrite(X_Widht,Y_height,1);
                cout<<"send None"<<endl;
            }
        }
            break;
        case 1:
        {
            SendBuf_COUNT = 0;    //成功发送数据时将缓存计数器置零
            int X = src_img.cols/2;
            int Y = src_img.rows/2;
            port.RMSerialWrite(X,Y,0);
            cout<<"send center"<<endl;
        }
            break;
        case 2:
        {
            SendBuf_COUNT = 0;
            port.RMSerialWrite(X_Widht,Y_height,0);
            cout<<"send success"<<endl;
/*
            t2 = getTickCount();
            RunTime = (t2-t1)/getTickFrequency();
            FPS = 1 / RunTime;
            cout<<"time:"<<RunTime<<endl;
            cout<<"FPS:"<<FPS<<endl;
*/
        }
            break;
        default:
            break;
        }
    }
    imshow("th",bin_img);
    imshow("input" ,src_img);
    imshow("output",dst_img);
}

#endif // RM_ARMOR_FITTED_H
