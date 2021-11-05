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

//����ģʽ 0--7  8��ģʽ	����ʱ������
int WINAPI fnDevXISL_Acq_SetCameraMode();	//Acquisition_SetCameraMode

//�������汶��
int WINAPI fnDevXISL_Acq_SetCameraGain(int gain = 0);	//Acquisition_SetCameraGain

int WINAPI fnDevXISL_Acq_SetCameraTriggerMode(int TriggerMode = TRIGGERMODE_AED);	//Acquisition_SetCameraTriggerMode

//����ͬ��ģʽ
int WINAPI fnDevXISL_Acq_SetFrameSyncMode();	//Acquisition_SetFrameSyncMode

//���� ѭ��ʱ��
int WINAPI fnDevXISL_Acq_SetTimerSync();	//Acquisition_SetTimerSync

//�������غϲ�
int WINAPI fnDevXISL_Acq_SetCameraBinningMode(WORD wBinning=1);	//Acquisition_SetCameraBinningMode


//��ȡƽ��ƫ����������50����֡
int WINAPI fnDevXISL_Acq_OffsetImage(); //Acquisition_Acquire_OffsetImag

//����У��
//���ڴ���������������Ա����ǿ��Ի�ȡ����ͼ��
//��ȡ17����¶������������֡�����������ƽ��
int WINAPI fnDevXISL_Acq_GainImage(); //Acquisition_Acquire_GainImage

//����У�� ����У��
int WINAPI fnDevXISL_Acq_PixelMap();	//Acquisition_CreatePixelMap

//���òɼ����ž�̬ͼƬ������
int WINAPI fnDevXISL_Acq_Image(bool flag);	//Acquisition_Acquire_Image

//���òɼ�����ͼƬ/��Ƶ
int WINAPI fnDevXISL_Acq_Video();	//Acquisition_Acquire_Image

//ֹͣ��Ƶ�ɼ�
int WINAPI fnDevXISL_Acq_StopVideo();

//��ȡ��Դ״̬   ���к����˯��	powerstate 4 - IDLE; 3 - DEEP SLEEP
int WINAPI fnDevXISL_Acq_GetPowerstate();	// Acquisition_GetPowerstate

//����ϵͳ״̬�������� �������磬�ػ������˯�ߣ���
int WINAPI fnDevXISL_Acq_wpe_SystemControl();	//Acq_wpe_SystemControl

int WINAPI fnDev_WWWLMapping(unsigned short* input_image, unsigned char* output_image, int image_width, int image_height, float ww_val, float wl_val);

//����ص�����
typedef int(CALLBACK *fnDev_Acq_Imageback)(int frameId, int width, int height, void* data);

//ע��ص�����
int WINAPI fnDev_Acq_Register_ImageCallback(fnDev_Acq_Imageback callBackFun);

//���Իص�
int WINAPI fnDev_Acq_Register_Test();

#endif
