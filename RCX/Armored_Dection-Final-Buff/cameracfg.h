#ifndef CAMERACFG_H
#define CAMERACFG_H

#include "CameraApi.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include "configure.h"

unsigned char *g_pRgbBuffer; //处理后数据缓存区

int                     iCameraCounts = 1;
int                     iStatus=-1;
tSdkCameraDevInfo       tCameraEnumList;
int                     hCamera;
tSdkCameraCapbility     tCapability;      //设备描述信息
tSdkFrameHead           sFrameInfo;
BYTE*			        pbyBuffer;
IplImage                *iplImage = NULL;
int                     channel=3;
BOOL                    AEstate=FALSE;

bool                    cameraisopen = 0;

void CameraSet()
{
    switch (cameramode)
    {
    case 0:
        CameraSdkInit(1);
        //枚举设备，并建立设备列表
        iStatus = CameraEnumerateDevice(&tCameraEnumList,&iCameraCounts);
        printf("state = %d\n", iStatus);
        printf("count = %d\n", iCameraCounts);
        //没有连接设备
        if(iCameraCounts==0)
        {
            break;
        }
        //相机初始化。初始化成功后，才能调用任何其他相机相关的操作接口
        iStatus = CameraInit(&tCameraEnumList,-1,-1,&hCamera);
        //初始化失败
        printf("state = %d\n", iStatus);
        if(iStatus!=CAMERA_STATUS_SUCCESS)
        {
            break;
        }
        //获得相机的特性描述结构体。该结构体中包含了相机可设置的各种参数的范围信息。决定了相关函数的参数
        CameraGetCapability(hCamera,&tCapability);
        g_pRgbBuffer = (unsigned char*)malloc(tCapability.sResolutionRange.iHeightMax*tCapability.sResolutionRange.iWidthMax*3);
        std::cout<<CameraGetAeState(hCamera,&AEstate);
        std::cout<<CameraSetAeState(hCamera,FALSE);
        if(armor_color == 0)
        {
            CameraSetExposureTime(hCamera,350);
        }
        else
        {
            CameraSetExposureTime(hCamera,150);
        }
        /*让SDK进入工作模式，开始接收来自相机发送的图像数据。
         *如果当前相机是触发模式，则需要接收到触发帧以后才会更新图像*/
        CameraPlay(hCamera);
        /*
        其他的相机参数设置
        例如 CameraSetExposureTime   CameraGetExposureTime  设置/读取曝光时间
             CameraSetImageResolution  CameraGetImageResolution 设置/读取分辨率
             CameraSetGamma、CameraSetConrast、CameraSetGain等设置图像伽马、对比度、RGB数字增益等等。
             更多的参数的设置方法，，清参考MindVision_Demo。本例程只是为了演示如何将SDK中获取的图像，转成OpenCV的图像格式,以便调用OpenCV的图像处理函数进行后续开发
        */
        if(tCapability.sIspCapacity.bMonoSensor){
            channel=1;
            CameraSetIspOutFormat(hCamera,CAMERA_MEDIA_TYPE_MONO8);
        }else{
            channel=3;
            CameraSetIspOutFormat(hCamera,CAMERA_MEDIA_TYPE_BGR8);
        }
        cameraisopen = 1;
        break;
    case 1:

        break;
    default:
        break;
    }

}

#endif // CAMERACFG_H
