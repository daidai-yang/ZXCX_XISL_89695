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

// 自定义消息
#define WM_MSG_RECV_IMGDATA        WM_APP + 0x10
#define WM_MSG_RECV_VIDEODATA      WM_APP + 0x11


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// XISL ERROR CODE
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEV_ERR_XISL_HWERROR_LOW			0x40			//设备故障			
#define DEV_ERR_XISL_RETUIN					0x41			//回收卡片
#define DEV_ERR_XISL_TIMEOUT				0x42			//超时
#define DEV_ERR_XISL_TYPE					0x43			//卡类型错误
#define	DEV_ERR_XISL_RF						0x44			//读卡失败
#define	DEV_ERR_XISL_MOVE					0x45			//移卡失败
#define	DEV_ERR_XISL_RESET					0x46			//复位失败
#define	DEV_ERR_XISL_STATUS					0x47			//获取状态
#define	DEV_ERR_XISL_MEDIA					0x48			//无卡
#define	DEV_ERR_XISL_JAM					0x49			//夹卡
#define	DEV_ERR_XISL_SCAN					0x50			//扫描失败
#define	DEV_ERR_XISL_RFCONTROL				0x51			//射频失败
#define	DEV_ERR_XISL_AUTHEN					0x52			//认证失败
#define	DEV_ERR_XISL_CALIB					0x53			//校正
#define	DEV_ERR_XISL_BMP2JPEG				0x54			//bmp2jpeg
#define	DEV_ERR_XISL_UPGRADER				0x55			//升级固件
#define	DEV_ERR_XISL_SCANLEN				0x56			//增加扫描长度

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DLL RESPONSE ERROR CODE
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DT_SUCCESS							0x00				//成功执行
#define DT_CLOSEFAIL						0x33				//成功执行
#define	DT_FAILURE							0x34				//失败
#define DT_ERROR_ALREADYOPEN				0x35				//设备已打开
#define DT_ERROR_LOADLIBRARY				0x36				//加载动态库失败


#endif