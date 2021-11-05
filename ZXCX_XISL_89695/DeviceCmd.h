#pragma once

#ifndef DEVICECMD_H
#define DEVICECMD_H



///////////////////////////////////////////////////////////////////////
// XISL DEV COMMAND
///////////////////////////////////////////////////////////////////////
int WINAPI fnDevXISL_Acq_Open();
int WINAPI fnDevXISL_Acq_Close();
int WINAPI fnDevXISL_Acq_Reset_OnboardOptions();	//Acquisition_Reset_OnboardOptions
int WINAPI fnDevXISL_Acq_Status();

int WINAPI fnDevXISL_Acq_GetVersionInfo();	//Acquisition_GetVersionInfo

//设置模式 0--7  8个模式	积分时间设置
int WINAPI fnDevXISL_Acq_SetCameraMode();	//Acquisition_SetCameraMode

//设置增益倍数
int WINAPI fnDevXISL_Acq_SetCameraGain(int gain = 0);	//Acquisition_SetCameraGain

int WINAPI fnDevXISL_Acq_SetCameraTriggerMode(int TriggerMode = TRIGGERMODE_AED);	//Acquisition_SetCameraTriggerMode

//设置同步模式
int WINAPI fnDevXISL_Acq_SetFrameSyncMode();	//Acquisition_SetFrameSyncMode

//设置 循环时间
int WINAPI fnDevXISL_Acq_SetTimerSync();	//Acquisition_SetTimerSync

//设置像素合并
int WINAPI fnDevXISL_Acq_SetCameraBinningMode(WORD wBinning=1);	//Acquisition_SetCameraBinningMode


//获取平均偏移量，例如50个暗帧
int WINAPI fnDevXISL_Acq_OffsetImage(); //Acquisition_Acquire_OffsetImag

//亮场校正
//现在传感器必须点亮，以便我们可以获取增益图像：
//获取17个暴露传感器的明亮帧，并对其进行平均
int WINAPI fnDevXISL_Acq_GainImage(); //Acquisition_Acquire_GainImage

//像素校正 坏点校正
int WINAPI fnDevXISL_Acq_PixelMap();	//Acquisition_CreatePixelMap

//设置采集单张静态图片多序列
int WINAPI fnDevXISL_Acq_Image(bool flag);	//Acquisition_Acquire_Image

//设置采集连续图片/视频
int WINAPI fnDevXISL_Acq_Video();	//Acquisition_Acquire_Image

//停止视频采集
int WINAPI fnDevXISL_Acq_StopVideo();

//获取电源状态   空闲和深度睡眠	powerstate 4 - IDLE; 3 - DEEP SLEEP
int WINAPI fnDevXISL_Acq_GetPowerstate();	// Acquisition_GetPowerstate

//设置系统状态（重启， 重置网络，关机，深度睡眠，）
int WINAPI fnDevXISL_Acq_wpe_SystemControl();	//Acq_wpe_SystemControl

int WINAPI fnDev_WWWLMapping(unsigned short* input_image, unsigned char* output_image, int image_width, int image_height, float ww_val, float wl_val);

//定义回调函数
typedef int(CALLBACK *fnDev_Acq_Imageback)(int frameId, int width, int height, void* data);

//注册回调函数
int WINAPI fnDev_Acq_Register_ImageCallback(fnDev_Acq_Imageback callBackFun);

//测试回调
int WINAPI fnDev_Acq_Register_Test();

#endif
