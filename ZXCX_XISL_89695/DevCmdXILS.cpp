#include "pch.h"


unsigned short *pAcqImgBuffer = NULL;
unsigned short *pAcqVideoBuffer = NULL;
HANDLE hevEndAcq = NULL; // signaled at end of acquisition by corresponding callback
fnDev_Acq_Imageback Image_callback;
//在每个帧的数据传输结束时调用OnEndFrameCallback函数//
//在帧结束回调中，我们可以放置输出，进行图像处理等//

//----------------------------------------------------------------------------------------------------------------------//
void CALLBACK OnEndFrameCallback(HACQDESC hAcqDesc)
{
	ZxcxLogINFO("");

	DWORD dwActFrame, dwSecFrame, dwRow = 128, dwCol = 128;
	UINT dwDataType, dwRows, dwColumns, dwFrames, dwSortFlags;
	DWORD dwAcqType, dwSystemID, dwAcqData, dwSyncMode, dwHwAccess;
	int iIRQFlags;
	unsigned int uiIsPreview = 0;
	dwAcqData = 0;
	//----------------------------------------------------------------------------------------------------------------------//
	// The pointer you retrieve by Acquisition_GetAcqData(..) has been set in the main function by Acquisition_SetAcqData.	//
	// Anyway, you are free to define for instance a global variable to indicate the different acquisition modes			//
	// in the callback functions. The above mentioned approach has the advantage of a encapsulation							//
	// similar to that used in object orientated programming.																//
	//----------------------------------------------------------------------------------------------------------------------//

#ifdef XIS_OS_64
	void *vpAcqData = NULL;
	Acquisition_GetAcqData(g_hAcqDesc, &vpAcqData);
	if (vpAcqData)
		dwAcqData = *((DWORD*)vpAcqData);
#else
	Acquisition_GetAcqData(g_hAcqDesc, &dwAcqData);
#endif

	Acquisition_GetConfiguration(g_hAcqDesc, &dwFrames, &dwRows,
		&dwColumns, &dwDataType, &dwSortFlags, &iIRQFlags, &dwAcqType,
		&dwSystemID, &dwSyncMode, &dwHwAccess);

	//获取当前的采集帧数
	Acquisition_GetActFrame(g_hAcqDesc, &dwActFrame, &dwSecFrame);

	// Depending from the passed status flag it is decided how to read out the acquisition buffer:

	//根据通过的状态标志，决定如何读取采集缓冲区：
	switch (dwAcqData)
	{
	case ACQ_IMG:
		ZxcxLogINFO("acq buffer frame: %d, dest frame %d, row: %d, col: %d, value: %d", dwActFrame, dwSecFrame,
			dwRow, dwCol, pAcqImgBuffer[dwColumns*dwRow + dwCol]);
		//PostMessage(g_hMsgWnd, WM_MSG_RECV_IMGDATA, (WPARAM)pAcqImgBuffer, 0);
		//可以添加获取图片进度条等过程
		break;
	case ACQ_VIDEO:
		ZxcxLogINFO("acq buffer frame: %d, dest frame %d, row: %d, col: %d, value: %d\n", dwActFrame, dwSecFrame,
			dwRow, dwCol, pAcqVideoBuffer[dwColumns*dwRow + dwCol]);
		//视频 一直抛数据出去
		PostMessage(g_hMsgWnd, WM_MSG_RECV_VIDEODATA, (WPARAM)pAcqVideoBuffer, 0);
		break;
	case ACQ_OFFSET:
		break;
	case ACQ_GAIN:
		break;
	default:
		break;
	}
}

// callback function that is called by XISL at end of acquisition·	//XISL在采集结束时调用的回调函数
void CALLBACK OnEndAcqCallback(HACQDESC hAcqDesc)
{
	ZxcxLogINFO("");
	DWORD dwActFrame, dwSecFrame;
	UINT dwDataType, dwRows, dwColumns, dwFrames, dwSortFlags;
	DWORD dwAcqType, dwSystemID, dwAcqData, dwSyncMode, dwHwAccess;
	int iIRQFlags;


#ifdef XIS_OS_64
	void *vpAcqData = NULL;
	Acquisition_GetAcqData(g_hAcqDesc, &vpAcqData);
	if (vpAcqData)
		dwAcqData = *((DWORD*)vpAcqData);
#else
	Acquisition_GetAcqData(g_hAcqDesc, &dwAcqData);
#endif

	Acquisition_GetConfiguration(hAcqDesc, &dwFrames, &dwRows,
		&dwColumns, &dwDataType, &dwSortFlags, &iIRQFlags, &dwAcqType,
		&dwSystemID, &dwSyncMode, &dwHwAccess);
	Acquisition_GetActFrame(hAcqDesc, &dwActFrame, &dwSecFrame);

	Acquisition_Reset_OnboardOptions(hAcqDesc);

	switch (dwAcqData)
	{
	case ACQ_IMG:
		//采图结束
		//发送message到主进程
		Image_callback(dwSecFrame, dwRows, dwColumns, pAcqImgBuffer);
		break;
	case ACQ_VIDEO:
		//视频结束
		//发送message到主进程
		break;
	case ACQ_OFFSET:
		SetEvent(hevEndAcq);
		break;
	case ACQ_GAIN:
		SetEvent(hevEndAcq);
		break;
	default:
		break;
	}

}

int WINAPI fnDevXISL_Acq_Open()
{
	ZxcxLogINFO("");
	int			iRet;							// Return value
	BYTE		nRet = DT_SUCCESS;

	//测试接收消息
	//SendMessage(g_hMsgWnd, WM_MSG_RECVDATA, 0, 0);

	int iarrayLength = 20;
	int iactiveConfig = 0;
	long idx = 0;
	BOOL bEnableIRQ = 0;

	HACQDESC hAcqDesc = NULL;

	unsigned int dwRows_init = 0, dwColumns_init = 0;

	UINT dwRows, dwColumns, dwFrames, dwDataType, dwSortFlags;
	DWORD dwAcqType, dwSystemID, dwSyncMode, dwHwAccess;

	BOOL bSelfInit = TRUE;
	long lOpenMode = HIS_GbIF_IP;
	long lPacketDelay = 256;

	unsigned short usTiming = 0;
	unsigned short usNetworkLoadPercent = 80;

	GBIF_DEVICE_PARAM Params;
	GBIF_Detector_Properties Properties;

	unsigned int uiNumSensors;			// Number of sensors
	long ulNumSensors = 0;				// nr of GigE detector in network

	char strBuffer[4000];  // buffer for outputs

	// First we tell the system to enumerate all available sensors
	// * to initialize them in polling mode, set bEnableIRQ = FALSE;
	// * otherwise, to enable interrupt support, set bEnableIRQ = TRUE;
	bEnableIRQ = TRUE;

	// event handle to retrieve the OnEndFrameCallback event
	if (!hevEndAcq)
		hevEndAcq = CreateEvent(NULL, FALSE, FALSE, NULL);

	// find GbIF Detectors in Subnet
	iRet = Acquisition_GbIF_GetDeviceCnt(&ulNumSensors);
	if (iRet != HIS_ALL_OK)
	{
		sprintf_s(strBuffer, "%s fail! Error Code %d", "Acquisition_GbIF_GetDetectorCnt", iRet);
		ZxcxLogINFO("strBuffer = %s", strBuffer);
		return iRet;
	}
	else
	{
		sprintf_s(strBuffer, "%s\t\t\t\tPASS!", "Acquisition_GbIF_GetDetectorCnt");
		ZxcxLogINFO("strBuffer = %s-----ulNumSensors = %d", strBuffer, ulNumSensors);
	}

	// do it with a single broadcast
	iRet = Acquisition_GbIF_DiscoverDetectors();
	if (HIS_ALL_OK != iRet)
	{
		sprintf_s(strBuffer, "%s fail! Error Code %d", "Acquisition_GbIF_DiscoverDetectors", iRet);
		ZxcxLogINFO("strBuffer = %s", strBuffer);
		return iRet;
	}
	else
	{
		sprintf_s(strBuffer, "%s\t\t\t\tPASS!", "Acquisition_GbIF_DiscoverDetectors");
		ZxcxLogINFO("strBuffer = %s", strBuffer);
	}

	// this cannot fail if the previous call was successful
		//// 返回已发现的设备数量
	Acquisition_GbIF_DiscoveredDetectorCount(&ulNumSensors);

	if (ulNumSensors > 0)
	{
		// get device params of GbIF Detectors in Subnet
		GBIF_DEVICE_PARAM* pGbIF_DEVICE_PARAM = (GBIF_DEVICE_PARAM*)malloc(sizeof(GBIF_DEVICE_PARAM)*(ulNumSensors));

		for (idx = 0; idx < ulNumSensors; ++idx)
		{
			// this cannot fail if the previous call was successful
			Acquisition_GbIF_DiscoveredDetectorByIndex(idx, &pGbIF_DEVICE_PARAM[idx]);
		}

		for (int iCnt = 0; iCnt < ulNumSensors; iCnt++)
		{
			sprintf_s(strBuffer, "%d - %s", iCnt, (pGbIF_DEVICE_PARAM[iCnt]).cDeviceName);
			ZxcxLogINFO("strBuffer = %s", strBuffer);
		}

		// try to init detector
		uiNumSensors = 0;

		iRet = Acquisition_GbIF_Init(
			&hAcqDesc,
			//iSelected,							// Index to access individual detector
			0,										// here set to zero for a single detector device
			bEnableIRQ,
			dwRows_init, dwColumns_init,						// Image dimensions
			bSelfInit,								// retrieve settings (rows,cols.. from detector
			FALSE,									// If communication port is already reserved by another process, do not open
			lOpenMode,								// here: HIS_GbIF_IP, i.e. open by IP address 
			pGbIF_DEVICE_PARAM[0].ucIP		// IP address of the connection to open
		);

		if (iRet != HIS_ALL_OK)
		{
			sprintf_s(strBuffer, "%s fail! Error Code %d\t\t\t\t", "Acquisition_GbIF_Init", iRet);
			ZxcxLogINFO("strBuffer = %s", strBuffer);
			free(pGbIF_DEVICE_PARAM);
			nRet = DT_FAILURE;
		}
		else
		{
			sprintf_s(strBuffer, "%s\t\t\t\tPASS!", "Acquisition_GbIF_Init");

			// Calibrate connection	设置网络
			if (Acquisition_GbIF_CheckNetworkSpeed(hAcqDesc, &usTiming, &lPacketDelay, usNetworkLoadPercent) == HIS_ALL_OK)
			{
				sprintf_s(strBuffer, "%s result: suggested timing: %d packetdelay %lu @%d networkload\t\t\t",
					"Acquisition_GbIF_CheckNetworkSpeed", usTiming, lPacketDelay, usNetworkLoadPercent);
				ZxcxLogINFO("strBuffer = %s", strBuffer);

			}


			iRet = Acquisition_GbIF_SetPacketDelay(hAcqDesc, lPacketDelay);
			if (iRet != HIS_ALL_OK)
			{
				sprintf_s(strBuffer, "%s fail! Error Code %d\t\t\t", "Acquisition_GbIF_SetPacketDelay", iRet);
				ZxcxLogINFO("strBuffer = %s", strBuffer);
				free(pGbIF_DEVICE_PARAM);
				nRet = DT_FAILURE;
			}
			else
			{
				sprintf_s(strBuffer, "%s %lu\t\t\t\tPASS!", "Acquisition_GbIF_SetPacketDelay", lPacketDelay);
			}
			ZxcxLogINFO("strBuffer = %s", strBuffer);

			// Get Detector Params of already opened GigE Detector
			iRet = Acquisition_GbIF_GetDeviceParams(hAcqDesc, &Params);
			if (iRet != HIS_ALL_OK)
			{
				sprintf_s(strBuffer, "%s fail! Error Code %d\t\t\t", "Acquisition_GBIF_GetDeviceParams", iRet);
				ZxcxLogINFO("strBuffer = %s", strBuffer);
				free(pGbIF_DEVICE_PARAM);
				nRet = DT_FAILURE;
			}
			else
			{
				sprintf_s(strBuffer, "%s \t\t\t\tPASS!\n", "Acquisition_GBIF_GetDeviceParams");
				sprintf_s(strBuffer, "Device Name: \t\t%s\nMAC: \t\t\t%s\nIP: \t\t\t%s\nSubnetMask: \t\t%s\nGateway: \t\t%s\nAdapterIP: \t\t%s\nAdapterSubnetMask: \t%s\nBootOptions Flag: \t%d\nGBIF Firmware: \t\t%s\n",
					Params.cDeviceName,
					Params.ucMacAddress,
					Params.ucIP,
					Params.ucSubnetMask,
					Params.ucGateway,
					Params.ucAdapterIP,
					Params.ucAdapterMask,
					Params.dwIPCurrentBootOptions,
					Params.cGBIFFirmwareVersion
				);
				ZxcxLogINFO("strBuffer = %s", strBuffer);
			}

			// Read production data
			iRet = Acquisition_GbIF_GetDetectorProperties(hAcqDesc, &Properties);
			if (iRet != HIS_ALL_OK)
			{
				sprintf_s(strBuffer, "%s fail! Error Code %d\t\t\t\n", "Acquisition_GbIF_GetDetectorProperties", iRet);
				ZxcxLogINFO("strBuffer = %s", strBuffer);
				free(pGbIF_DEVICE_PARAM);
				nRet = DT_FAILURE;
			}
			else
			{
				sprintf_s(strBuffer, "%s \t\t\t\tPASS!", "Acquisition_GbIF_GetDetectorProperties");
				ZxcxLogINFO("strBuffer = %s", strBuffer);
				sprintf_s(strBuffer, "Detector Type: \t\t%s\nManufDate: \t\t%s\nManufPlace: \t\t%s", Properties.cDetectorType,
					Properties.cManufacturingDate, Properties.cPlaceOfManufacture);
				ZxcxLogINFO("strBuffer = %s", strBuffer);
			}
			g_hAcqDesc = hAcqDesc;
		}
		free(pGbIF_DEVICE_PARAM);
	}
	else
	{
		nRet = DT_FAILURE;
	}

	ACQDESCPOS Pos = 0;
	int nChannelNr;
	UINT nChannelType;

	if ((nRet = Acquisition_GetNextSensor(&Pos, &g_hAcqDesc)) != HIS_ALL_OK)
	{
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		ZxcxLogERROR("szMsg = %s", szMsg);
	}
	ZxcxLogINFO("pos = %d", Pos);
	// ask for communication device type and its number	//询问通信设备类型及其编号
	if ((nRet = Acquisition_GetCommChannel(g_hAcqDesc, &nChannelType, &nChannelNr)) != HIS_ALL_OK)
	{
		// error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		ZxcxLogERROR("szMsg = %s", szMsg);
	}

	sprintf(strBuffer, "channel type: %d, ChannelNr: %d", nChannelType, nChannelNr);
	ZxcxLogINFO("strBuffer = %s", strBuffer);

	switch (nChannelType)
	{
	case HIS_BOARD_TYPE_ELTEC_XRD_FGE_Opto:
		sprintf(strBuffer, "%s%d", "HIS_BOARD_TYPE_ELTEC_XRD_FGE_Opto:", nChannelType);
		break;
	case HIS_BOARD_TYPE_ELTEC_XRD_FGX:
		sprintf(strBuffer, "%s%d", "HIS_BOARD_TYPE_ELTEC_XRD_FGX:", nChannelType);
		break;
	case HIS_BOARD_TYPE_ELTEC:
		sprintf(strBuffer, "%s%d", "HIS_BOARD_TYPE_ELTEC:", nChannelType);
		break;
	case HIS_BOARD_TYPE_ELTEC_GbIF:
		sprintf(strBuffer, "%s%d", "HIS_BOARD_TYPE_ELTEC_GbIF:", nChannelType);
		break;
	case HIS_BOARD_TYPE_ELTEC_EMBEDDED:
		sprintf(strBuffer, "%s%d", "HIS_BOARD_TYPE_ELTEC_EMBEDDED:", nChannelType);
		break;
	default:
		sprintf(strBuffer, "%s%d", "Unknown ChanelType:", nChannelType);
		break;
	}
	ZxcxLogINFO("strBuffer = %s", strBuffer);

	// ask for data organization of all sensors
	if ((nRet = Acquisition_GetConfiguration(hAcqDesc, &dwFrames, &dwRows,
		&dwColumns, &dwDataType, &dwSortFlags, &bEnableIRQ, &dwAcqType,
		&dwSystemID, &dwSyncMode, &dwHwAccess)) != HIS_ALL_OK)
	{
		// error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		ZxcxLogINFO("szMsg = %s", szMsg);
		return 0;
	}

	sprintf(strBuffer, "frames: %d", dwFrames);
	ZxcxLogINFO("strBuffer = %s", strBuffer);
	sprintf(strBuffer, "rows: %d\ncolumns: %d", dwRows, dwColumns);
	ZxcxLogINFO("strBuffer = %s", strBuffer);

	//// now set callbacks and messages for every sensor	//现在为每个传感器设置回调和消息
	//if ((nRet = Acquisition_SetCallbacksAndMessages(g_hAcqDesc, NULL, 0,
	//	0, OnEndFrameCallback, OnEndAcqCallback)) != HIS_ALL_OK)
	//{
	//	// error handling
	//	char szMsg[300];
	//	sprintf(szMsg, "Error nr.: %d", nRet);
	//	ZxcxLogERROR("szMsg = %s", szMsg);
	//}

	if (nRet != DT_SUCCESS)
		ZxcxLogERROR(_T("(return : %s)"), GetRtStr(nRet));
	else
		ZxcxLogINFO(_T("(return : %s)"), GetRtStr(nRet));

	ZxcxLogINFO("end");
	return nRet;
}
int WINAPI fnDevXISL_Acq_Close()
{
	ZxcxLogINFO("");
	BYTE		nRet = DT_SUCCESS;

	if (pAcqImgBuffer)
	{
		free(pAcqImgBuffer);
		pAcqImgBuffer = NULL;
	}
	if (pAcqVideoBuffer)
	{
		free(pAcqVideoBuffer);
		pAcqVideoBuffer = NULL;
	}
	if (hevEndAcq)
	{
		CloseHandle(hevEndAcq);
		hevEndAcq = NULL;
	}

	if (g_hAcqDesc)
	{
		nRet = Acquisition_Close(g_hAcqDesc);
		g_hAcqDesc = NULL;
	}

	if (nRet != DT_SUCCESS)
		ZxcxLogERROR(_T("(return : %s)"), GetRtStr(nRet));
	else
		ZxcxLogINFO(_T("(return : %s)"), GetRtStr(nRet));

	return nRet;
}
int WINAPI fnDevXISL_Acq_Reset_OnboardOptions()
{
	BYTE		nRet = DT_SUCCESS;

	// disable onboard stuff
	nRet = Acquisition_Reset_OnboardOptions(g_hAcqDesc);
	if (nRet != HIS_ALL_OK)
	{
		printf("Acquisition_Reset_OnboardOptions returned an Error: %d\n", nRet);
		return 0;
	}

	// set Free running as default to save power and keep the detector cool
	nRet = Acquisition_SetFrameSyncMode(g_hAcqDesc, HIS_SYNCMODE_FREE_RUNNING);

	// set timing to fastest/default readout
	nRet = Acquisition_SetCameraMode(g_hAcqDesc, 0);

	// select the gain		这是增益等级
	nRet = Acquisition_SetCameraGain(g_hAcqDesc, 5);

	return nRet;
}
int WINAPI fnDevXISL_Acq_Status()
{

	BYTE		nRet = DT_SUCCESS;

	return nRet;
}

int WINAPI fnDevXISL_Acq_GetVersionInfo()
{
	BYTE		nRet = DT_SUCCESS;

	XRpad_VersionInfo versionInfo;
	nRet = Acquisition_GetVersionInfo(g_hAcqDesc, &versionInfo);
	if (HIS_ALL_OK != nRet)
	{
		printf("Error retrieving version info: %d\n", nRet);
	}

	printf("Received version information:\n");
	printf("-------------------------------------------\n");
	printf("Subversion:\t\t%s\n", versionInfo.subversion);
	printf("Linux Kernel:\t\t%s\n", versionInfo.linux_kernel);
	printf("Software:\t\t%s\n", versionInfo.software);
	printf("Hardware driver:\t%s\n", versionInfo.hwdriver);
	printf("Zynq Firmware:\t\t%s\n", versionInfo.zynq_firmware);
	printf("Spartan Firmware:\t%s\n", versionInfo.spartan_firmware);
	printf("MSP Firmware:\t\t%s\n", versionInfo.msp_firmware);
	printf("PLD Firmware:\t\t%s\n", versionInfo.pld_firmware);
	printf("XRPad Daemon:\t\t%s\n\n", versionInfo.xrpd);

	return nRet;
}

int WINAPI fnDevXISL_Acq_SetCameraMode()
{
	BYTE		nRet = DT_SUCCESS;
	nRet = Acquisition_SetCameraMode(g_hAcqDesc, 0);

	return nRet;
}

int WINAPI fnDevXISL_Acq_SetCameraGain(int gain)
{
	BYTE		nRet = DT_SUCCESS;
	nRet = Acquisition_SetCameraGain(g_hAcqDesc, gain);

	return nRet;
}

int WINAPI fnDevXISL_Acq_SetCameraTriggerMode(int TriggerMode)
{
	BYTE		nRet = DT_SUCCESS;
	nRet = Acquisition_SetCameraTriggerMode(g_hAcqDesc, TriggerMode);

	return nRet;
}

int WINAPI fnDevXISL_Acq_SetFrameSyncMode()
{
	BYTE		nRet = DT_SUCCESS;
	nRet = Acquisition_SetFrameSyncMode(g_hAcqDesc, HIS_SYNCMODE_FREE_RUNNING);

	return nRet;
}

int WINAPI fnDevXISL_Acq_SetTimerSync()
{
	BYTE		nRet = DT_SUCCESS;

	DWORD dwTime;
	dwTime = 1000 * 500;
	nRet = Acquisition_SetTimerSync(g_hAcqDesc, &dwTime);

	return nRet;
}

int WINAPI fnDevXISL_Acq_SetCameraBinningMode(WORD wBinning)
{
	BYTE		nRet = DT_SUCCESS;
	nRet = Acquisition_SetCameraBinningMode(g_hAcqDesc, wBinning);

	return nRet;
}

//获取平均偏移量，例如30个暗帧
int WINAPI fnDevXISL_Acq_OffsetImage()
{
	BYTE		nRet = DT_SUCCESS;

	DWORD dwDemoParam;
	int bEnableIRQ = 0;
	UINT dwRows, dwColumns, dwFrames, dwDataType, dwSortFlags;
	DWORD dwAcqType, dwSystemID, dwSyncMode, dwHwAccess;

	// ask for detector data organization to	获取探测器的数据
	if ((nRet = Acquisition_GetConfiguration(g_hAcqDesc, &dwFrames, &dwRows,
		&dwColumns, &dwDataType, &dwSortFlags, &bEnableIRQ, &dwAcqType,
		&dwSystemID, &dwSyncMode, &dwHwAccess)) != HIS_ALL_OK)
	{
		ZxcxLogERROR("Acquisition_GetConfiguration error");
		nRet = DT_FAILURE;
	}
	unsigned short *pOffsetBuffer = NULL;

	pOffsetBuffer = (unsigned short*)malloc(dwRows * dwColumns * sizeof(short));

	dwDemoParam = ACQ_OFFSET;

	if ((nRet = Acquisition_SetAcqData(g_hAcqDesc, (void*)&dwDemoParam)) != HIS_ALL_OK)
	{
		ZxcxLogERROR("Acquisition_SetAcqData error!");
	}

	if ((nRet = Acquisition_Acquire_OffsetImage(g_hAcqDesc, pOffsetBuffer, dwRows, dwColumns, 50)) != HIS_ALL_OK)
	{
		// error handler
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		ZxcxLogERROR("Acquisition_Acquire_OffsetImage error");
		nRet = DT_FAILURE;
	}

	//等待暗场数据返回
	DWORD dwReturn = 0;
	dwReturn = WaitForSingleObject(hevEndAcq, INFINITE);
	if (dwReturn == WAIT_TIMEOUT)
	{
		ZxcxLogERROR("Acquisition_Acquire_OffsetImage time out!");
	}
	else if (dwReturn == WAIT_OBJECT_0)
	{
		//把读取到pOffsetBuffer写到本地文件中
		CString OffsetPath = g_CurrectionPath + "offset.cor";
		//获取文件指针
		FILE *poffsetFile = fopen(OffsetPath, "w"); // 文件打开方式 如果原来有内容也会销毁
	//向文件写数据
		fwrite(pOffsetBuffer, //要输入的文字
			1,//文字每一项的大小 这里是字符型的 就设置为1 如果是汉字就设置为4
			wuslen(pOffsetBuffer), //单元个数 我们也可以直接写5
			poffsetFile //我们刚刚获得到的地址
		);
		fclose(poffsetFile); //告诉系统我们文件写完了数据更新，但是我们要要重新打开才能在写
		//fflush(pFile); //数据刷新 数据立即更新     
	}

	if (pOffsetBuffer) {
		free(pOffsetBuffer);
		pOffsetBuffer = NULL;
	}

	return nRet;
}

int WINAPI fnDevXISL_Acq_GainImage()
{
	BYTE		nRet = DT_SUCCESS;

	DWORD dwDemoParam;
	int bEnableIRQ = 0;
	UINT dwRows, dwColumns, dwFrames, dwDataType, dwSortFlags;
	DWORD dwAcqType, dwSystemID, dwSyncMode, dwHwAccess;

	// ask for detector data organization to	获取探测器的数据
	if ((nRet = Acquisition_GetConfiguration(g_hAcqDesc, &dwFrames, &dwRows,
		&dwColumns, &dwDataType, &dwSortFlags, &bEnableIRQ, &dwAcqType,
		&dwSystemID, &dwSyncMode, &dwHwAccess)) != HIS_ALL_OK)
	{
		ZxcxLogERROR("Acquisition_GetConfiguration error");
		nRet = DT_FAILURE;
	}

	//1.读取暗场校正的数据
	CString OffsetPath = g_CurrectionPath + "offset.cor";
	FILE *pOffsetFile = fopen(OffsetPath, "r");
	if (pOffsetFile == NULL)
	{
		ZxcxLogERROR("fopen fail!");
		nRet = DT_FAILURE;
		return nRet;
	}
	unsigned short *pBuffer = NULL;
	fseek(pOffsetFile, 0, SEEK_END);
	int len = ftell(pOffsetFile);
	pBuffer = new unsigned short(len + 1);
	rewind(pOffsetFile);
	fread(pBuffer, 1, len, pOffsetFile);
	pBuffer[len] = 0;
	fclose(pOffsetFile);

	DWORD *pGainBuffer = NULL;

	pGainBuffer = (DWORD*)malloc(dwRows * dwColumns * sizeof(DWORD));

	dwDemoParam = ACQ_GAIN;

	if ((nRet = Acquisition_SetAcqData(g_hAcqDesc, (void*)&dwDemoParam)) != HIS_ALL_OK)
	{
		ZxcxLogERROR("Acquisition_SetAcqData error!");
	}

	if ((nRet = Acquisition_Acquire_GainImage(g_hAcqDesc, pBuffer, pGainBuffer, dwRows, dwColumns, 120)) != HIS_ALL_OK)
	{
		// error handler
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
	}

	//等待亮场数据返回
	DWORD dwReturn = 0;
	dwReturn = WaitForSingleObject(hevEndAcq, INFINITE);
	if (dwReturn == WAIT_TIMEOUT)
	{
		ZxcxLogERROR("Acquisition_Acquire_GainImage time out!");
	}
	else if (dwReturn == WAIT_OBJECT_0)
	{

		//把读取到pGainBuffer写到本地文件中
		CString GainPath = g_CurrectionPath + "gain.cor";
		//获取文件指针
		FILE *pGainFile = fopen(GainPath, "w"); // 文件打开方式 如果原来有内容也会销毁
	//向文件写数据
		fwrite(pGainBuffer, //要输入的文字
			1,//文字每一项的大小 这里是字符型的 就设置为1 如果是汉字就设置为4
			wlslen(pGainBuffer), //单元个数 我们也可以直接写5
			pGainFile //我们刚刚获得到的地址
		);
		fclose(pGainFile); //告诉系统我们文件写完了数据更新，但是我们要要重新打开才能在写
		//fflush(pFile); //数据刷新 数据立即更新     
	}

	if (pGainBuffer) {
		free(pGainBuffer);
		pGainBuffer = NULL;
	}

	if (pBuffer) {
		delete[] pBuffer;
		pBuffer = NULL;
	}

	return nRet;
}

int WINAPI fnDevXISL_Acq_PixelMap()
{
	BYTE		nRet = DT_SUCCESS;
	int iListSize = 0;
	unsigned short* pwPixelMapData = NULL;
	int* pCorrList = NULL;

	int bEnableIRQ = 0;
	UINT dwRows, dwColumns, dwFrames, dwDataType, dwSortFlags;
	DWORD dwAcqType, dwSystemID, dwSyncMode, dwHwAccess;

	// ask for detector data organization to	获取探测器的数据
	if ((nRet = Acquisition_GetConfiguration(g_hAcqDesc, &dwFrames, &dwRows,
		&dwColumns, &dwDataType, &dwSortFlags, &bEnableIRQ, &dwAcqType,
		&dwSystemID, &dwSyncMode, &dwHwAccess)) != HIS_ALL_OK)
	{
		ZxcxLogERROR("Acquisition_GetConfiguration error");
		nRet = DT_FAILURE;
	}

	// create a defect pixel image or load the defect pixel image which is already exiting (e.g. from the PKI detector CD) 
	pwPixelMapData = (unsigned short*)malloc(dwRows*dwColumns*(sizeof(unsigned short)));

	memset(pwPixelMapData, 0, dwRows*dwColumns * sizeof(unsigned short));
	pwPixelMapData[100 + 102 * dwColumns] = 0xFFFF; // mark pixel in image as defect
	pwPixelMapData[125 + 113 * dwColumns] = 0xFFFF; // mark pixel in image as defect

	pCorrList = NULL;

	// 1) retrieve the required size for the list "&iListSize"
	Acquisition_CreatePixelMap(pwPixelMapData, dwRows, dwColumns, pCorrList, &iListSize);

	// 2) Allocate the memory for the list
	pCorrList = (int *)malloc(iListSize);
	// 3) Import the data into the list
	Acquisition_CreatePixelMap(pwPixelMapData, dwRows, dwColumns, pCorrList, &iListSize);

	//把pCorrList中的数据写入到文件中

	if (pCorrList)
	{
		free(pCorrList);
		pCorrList = NULL;
	}
	return nRet;
}

int WINAPI fnDevXISL_Acq_Image(bool flag)
{
	ZxcxLogINFO("");
	BYTE		nRet = DT_SUCCESS;

	if (g_bVideoAcqState)
	{
		g_bVideoAcqState = FALSE;
		Acquisition_Abort(g_hAcqDesc);
	}
	int bEnableIRQ;
	UINT dwRows, dwColumns, dwFrames, dwDataType, dwSortFlags;
	DWORD dwAcqType, dwSystemID, dwSyncMode, dwHwAccess;

	DWORD dwDemoParam;

	dwFrames = 1;

	if ((nRet = Acquisition_GetConfiguration(g_hAcqDesc, &dwFrames, &dwRows,
		&dwColumns, &dwDataType, &dwSortFlags, &bEnableIRQ, &dwAcqType,
		&dwSystemID, &dwSyncMode, &dwHwAccess)) != HIS_ALL_OK)
	{

	}

	pAcqImgBuffer = (unsigned short*)malloc(dwFrames * dwRows * dwColumns * sizeof(short));
	if (!pAcqImgBuffer)
	{
		// error handling
	}

	// route acquisition buffer to XISL
	if ((nRet = Acquisition_DefineDestBuffers(g_hAcqDesc, pAcqImgBuffer,
		dwFrames, dwRows, dwColumns)) != HIS_ALL_OK)
	{
		// error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		ZxcxLogERROR("szMsg = %s", szMsg);
	}

	// Timing 0 Internal Timer 500msec  设置第二档
	Acquisition_SetCameraMode(g_hAcqDesc, 1);

	// change of the syncmode from to internal trigger mode  设置为4
	Acquisition_SetFrameSyncMode(g_hAcqDesc, HIS_SYNCMODE_FREE_RUNNING);

	Acquisition_SetCameraGain(g_hAcqDesc, 4);
	// set detector to default binning mode
	WORD wBinning = 1;
	Acquisition_SetCameraBinningMode(g_hAcqDesc, wBinning);

	dwDemoParam = ACQ_IMG;		//采集图片

	if ((nRet = Acquisition_SetAcqData(g_hAcqDesc, (void*)&dwDemoParam)) != HIS_ALL_OK)
	{

	}

	//1.读取暗场校正的数据
	CString OffsetPath = g_CurrectionPath + "offset.cor";
	FILE *pOffsetFile = fopen(OffsetPath, "r");
	unsigned short *pOffsetBuffer = NULL;
	fseek(pOffsetFile, 0, SEEK_END);
	int Offsetlen = ftell(pOffsetFile);
	pOffsetBuffer = new unsigned short(Offsetlen + 1);
	rewind(pOffsetFile);
	fread(pOffsetBuffer, 1, Offsetlen, pOffsetFile);
	pOffsetBuffer[Offsetlen] = 0;
	fclose(pOffsetFile);

	//2.读取亮场校正的数据
	CString GainPath = g_CurrectionPath + "gain.cor";
	FILE *pGainFile = fopen(GainPath, "r");
	DWORD *pGainBuffer = NULL;;
	fseek(pOffsetFile, 0, SEEK_END);
	int Gainlen = ftell(pGainFile);
	pGainBuffer = new DWORD(Gainlen + 1);
	rewind(pGainFile);
	fread(pGainBuffer, 1, Gainlen, pGainFile);
	pGainBuffer[Gainlen] = 0;
	fclose(pGainFile);

	//continuous acquisition		HIS_SEQ_AVERAGE   平均
	if ((nRet = Acquisition_Acquire_Image(g_hAcqDesc, 5, 2, HIS_SEQ_AVERAGE, pOffsetBuffer, pGainBuffer, NULL)) != HIS_ALL_OK)
	{
		// error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		ZxcxLogINFO("szMsg = %s", szMsg);
	}

	if (pOffsetBuffer) {
		delete[] pOffsetBuffer;
		pOffsetBuffer = NULL;
	}
	if (pGainBuffer) {
		delete[] pGainBuffer;
		pGainBuffer = NULL;
	}

	return nRet;
}

int WINAPI fnDevXISL_Acq_Video()
{
	ZxcxLogINFO("");

	BYTE		nRet = DT_SUCCESS;

	if (!g_bVideoAcqState)
	{
		g_bVideoAcqState = TRUE;
		Acquisition_Abort(g_hAcqDesc);
	}
	int bEnableIRQ;
	UINT dwRows, dwColumns, dwFrames, dwDataType, dwSortFlags;
	DWORD dwAcqType, dwSystemID, dwSyncMode, dwHwAccess;

	DWORD dwDemoParam;

	dwFrames = 1;

	if ((nRet = Acquisition_GetConfiguration(g_hAcqDesc, &dwFrames, &dwRows,
		&dwColumns, &dwDataType, &dwSortFlags, &bEnableIRQ, &dwAcqType,
		&dwSystemID, &dwSyncMode, &dwHwAccess)) != HIS_ALL_OK)
	{

	}

	pAcqVideoBuffer = (unsigned short*)malloc(dwFrames * dwRows * dwColumns * sizeof(short));
	if (!pAcqVideoBuffer)
	{
		// error handling
	}

	// route acquisition buffer to XISL
	if ((nRet = Acquisition_DefineDestBuffers(g_hAcqDesc, pAcqVideoBuffer,
		dwFrames, dwRows, dwColumns)) != HIS_ALL_OK)
	{
		// error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		MessageBox(NULL, szMsg, "debug message!", MB_OK | MB_ICONSTOP);
	}

	// Timing 0 Internal Timer 500msec  设置第二档
	Acquisition_SetCameraMode(g_hAcqDesc, 2);

	// change of the syncmode from to internal trigger mode  设置为4
	Acquisition_SetFrameSyncMode(g_hAcqDesc, HIS_SYNCMODE_FREE_RUNNING);

	Acquisition_SetCameraGain(g_hAcqDesc, 4);
	// set detector to default binning mode
	WORD wBinning = 2;
	Acquisition_SetCameraBinningMode(g_hAcqDesc, wBinning);

	dwDemoParam = ACQ_VIDEO;		//采集图片

	if ((nRet = Acquisition_SetAcqData(g_hAcqDesc, (void*)&dwDemoParam)) != HIS_ALL_OK)
	{

	}

	//1.读取暗场校正的数据
	CString OffsetPath = g_CurrectionPath + "offset.cor";
	FILE *pOffsetFile = fopen(OffsetPath, "r");
	unsigned short *pOffsetBuffer = NULL;
	fseek(pOffsetFile, 0, SEEK_END);
	int Offsetlen = ftell(pOffsetFile);
	pOffsetBuffer = new unsigned short(Offsetlen + 1);
	rewind(pOffsetFile);
	fread(pOffsetBuffer, 1, Offsetlen, pOffsetFile);
	pOffsetBuffer[Offsetlen] = 0;
	fclose(pOffsetFile);

	//2.读取亮场校正的数据
	CString GainPath = g_CurrectionPath + "gain.cor";
	FILE *pGainFile = fopen(GainPath, "r");
	DWORD *pGainBuffer = NULL;;
	fseek(pOffsetFile, 0, SEEK_END);
	int Gainlen = ftell(pGainFile);
	pGainBuffer = new DWORD(Gainlen + 1);
	rewind(pGainFile);
	fread(pGainBuffer, 1, Gainlen, pGainFile);
	pGainBuffer[Gainlen] = 0;
	fclose(pGainFile);

	//continuous acquisition		HIS_SEQ_CONTINUOUS   连续采集
	if ((nRet = Acquisition_Acquire_Image(g_hAcqDesc, 20, 0, HIS_SEQ_CONTINUOUS, pOffsetBuffer, pGainBuffer, NULL)) != HIS_ALL_OK)
	{
		// error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		ZxcxLogINFO("szMsg = %s", szMsg);
	}

	if (pOffsetBuffer) {
		delete[] pOffsetBuffer;
		pOffsetBuffer = NULL;
	}
	if (pGainBuffer) {
		delete[] pGainBuffer;
		pGainBuffer = NULL;
	}

	return nRet;
}

int WINAPI fnDevXISL_Acq_StopVideo()
{
	BYTE		nRet = DT_SUCCESS;
	nRet = Acquisition_Abort(g_hAcqDesc);

	return nRet;
}

int WINAPI fnDevXISL_Acq_GetPowerstate()
{
	BYTE		nRet = DT_SUCCESS;

	unsigned int uiPowerstate;

	nRet = Acquisition_GetPowerstate(g_hAcqDesc, &uiPowerstate);

	if (nRet != HIS_ALL_OK)
	{
		printf("Error Acquisition_GetPowerstate:%d\n", nRet);
	}
	else
	{
		printf("Current Power state: %d\n", uiPowerstate);
	}
	return nRet;
}

int WINAPI fnDevXISL_Acq_wpe_SystemControl()
{
	BYTE		nRet = DT_SUCCESS;

	GBIF_DEVICE_PARAM GbIfDevice;
	XRpad_SystemControlEnum eSystemControl;

	eSystemControl = XRpad_SYSTEM_CONTROL_REBOOT;

	// get ipaddr. of device to control it 
	nRet = Acquisition_GbIF_GetDeviceParams(g_hAcqDesc, &GbIfDevice);
	if (nRet != HIS_ALL_OK)
	{
		printf("Error Acquisition_GetPowerstate:%d\n", nRet);
	}
	else
	{
		printf("Current Power state: %d\n", nRet);
	}
	/*
	XRpad_SYSTEM_CONTROL_REBOOT = 0,           // restart XRpad
	XRpad_SYSTEM_CONTROL_RESTART_NETWORK = 1,  // restart XRpad Network
	XRpad_SYSTEM_CONTROL_SHUTDOWN = 2,         // shutdown XRpad
	XRpad_SYSTEM_CONTROL_SET_DEEP_SLEEP = 3,   // power down analog circuitry and sensor FPGA
	XRpad_SYSTEM_CONTROL_SET_IDLE = 4          // power up analog circuitry and sensor FPGA
	*/
	nRet = Acq_wpe_SystemControl((const char*)GbIfDevice.ucIP, eSystemControl);
	if (nRet != HIS_ALL_OK)
	{
		printf("Error Acquisition_GetPowerstate:%d\n", nRet);
	}
	else
	{
		printf("Current Power state: %d\n", nRet);
	}

	return nRet;
}

//非线性+线性window-leveling算法	wl_val窗位	ww_val窗宽
int WINAPI fnDev_WWWLMapping(unsigned short* input_image, unsigned char* output_image, int image_width, int image_height, float ww_val, float wl_val)
{
	ZxcxLogINFO("");
	BYTE		nRet = DT_SUCCESS;
	double dFactor, min, max;
	int nPixelVal = 0;
	int nNumPixels = image_width * image_height;

	ZxcxLogINFO("nNumPixels:%d", nNumPixels);

	int pixel_val = 0;

	min = (2 * wl_val - ww_val) / 2.0 + 0.5;
	max = (2 * wl_val + ww_val) / 2.0 + 0.5;

	dFactor = 255.0 / (double)(max - min);

	for (int i = 0; i < nNumPixels; i++)
	{
		pixel_val = input_image[i];
		if (pixel_val < min) {
			output_image[i] = 0;
			continue;
		}
		if (pixel_val > max) {
			output_image[i] = 255;
			continue;
		}

		nPixelVal = (int)((pixel_val - min)*dFactor);

		if (nPixelVal < 0) 
			output_image[i] = 0;
		else if (nPixelVal > 255) 
			output_image[i] = 255;
		else 
			output_image[i] = nPixelVal;
	}

	return nRet;
}

int WINAPI fnDev_Acq_Register_ImageCallback(fnDev_Acq_Imageback acq_Imageback)
{
	ZxcxLogINFO("");
	BYTE		nRet = DT_SUCCESS;

	Image_callback = acq_Imageback;

	// now set callbacks and messages for every sensor	//现在为每个传感器设置回调和消息
	if ((nRet = Acquisition_SetCallbacksAndMessages(g_hAcqDesc, NULL, 0,
		0, OnEndFrameCallback, OnEndAcqCallback)) != HIS_ALL_OK)
	{
		// error handling
		char szMsg[300];
		sprintf(szMsg, "Error nr.: %d", nRet);
		ZxcxLogERROR("szMsg = %s", szMsg);
	}

	return TRUE;
}
int WINAPI fnDev_Acq_Register_Test()
{
	ZxcxLogINFO("");
	BYTE		nRet = DT_SUCCESS;
	Image_callback(2, 1024, 1024, NULL);

	return TRUE;
}