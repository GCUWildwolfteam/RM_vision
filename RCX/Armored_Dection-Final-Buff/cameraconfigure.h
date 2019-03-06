#ifndef CAMERACONFIGURE_H
#define CAMERACONFIGURE_H

#include "configure.h"

class CameraConfigure
{
public:
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

    CameraConfigure();

    int cameraSet();
};

#endif // CAMERACONFIGURE_H
