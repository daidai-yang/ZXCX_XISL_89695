#pragma once

#ifndef DEFINE_H
#define DEFINE_H

#define TIMEOUT_RESPONSE					2000

#define NAME_SUB_MODEL						"ZXCX.XISL_89695"


#define ACQ_VIDEO			1
#define ACQ_IMG				2
#define ACQ_OFFSET			3
#define ACQ_GAIN			4
#define ACQ_Brightoc		16

// �Զ�����Ϣ
#define WM_MSG_RECV_IMGDATA        WM_APP + 0x10
#define WM_MSG_RECV_VIDEODATA      WM_APP + 0x11


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// XISL ERROR CODE
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEV_ERR_XISL_HWERROR_LOW			0x40			//�豸����			
#define DEV_ERR_XISL_RETUIN					0x41			//���տ�Ƭ
#define DEV_ERR_XISL_TIMEOUT				0x42			//��ʱ
#define DEV_ERR_XISL_TYPE					0x43			//�����ʹ���
#define	DEV_ERR_XISL_RF						0x44			//����ʧ��
#define	DEV_ERR_XISL_MOVE					0x45			//�ƿ�ʧ��
#define	DEV_ERR_XISL_RESET					0x46			//��λʧ��
#define	DEV_ERR_XISL_STATUS					0x47			//��ȡ״̬
#define	DEV_ERR_XISL_MEDIA					0x48			//�޿�
#define	DEV_ERR_XISL_JAM					0x49			//�п�
#define	DEV_ERR_XISL_SCAN					0x50			//ɨ��ʧ��
#define	DEV_ERR_XISL_RFCONTROL				0x51			//��Ƶʧ��
#define	DEV_ERR_XISL_AUTHEN					0x52			//��֤ʧ��
#define	DEV_ERR_XISL_CALIB					0x53			//У��
#define	DEV_ERR_XISL_BMP2JPEG				0x54			//bmp2jpeg
#define	DEV_ERR_XISL_UPGRADER				0x55			//�����̼�
#define	DEV_ERR_XISL_SCANLEN				0x56			//����ɨ�賤��

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DLL RESPONSE ERROR CODE
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DT_SUCCESS							0x00				//�ɹ�ִ��
#define DT_CLOSEFAIL						0x33				//�ɹ�ִ��
#define	DT_FAILURE							0x34				//ʧ��
#define DT_ERROR_ALREADYOPEN				0x35				//�豸�Ѵ�
#define DT_ERROR_LOADLIBRARY				0x36				//���ض�̬��ʧ��


#endif