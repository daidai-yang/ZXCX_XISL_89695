#include "pch.h"

CString WINAPI GetRtStr(BYTE code)
{
	CString strResult = _T("");
	switch (code)
	{
	case DT_SUCCESS:
		strResult = _T("DT_SUCCESS");
		break;
	case DT_CLOSEFAIL:
		strResult = _T("DT_CLOSEFAIL");
		break;
	case DT_FAILURE:
		strResult = _T("DT_FAILURE");
		break;
	case DT_ERROR_ALREADYOPEN:
		strResult = _T("DT_ERROR_ALREADYOPEN");
		break;
	case DT_ERROR_LOADLIBRARY:
		strResult = _T("DT_ERROR_LOADLIBRARY");
		break;
	default:
		strResult = _T("UNKOWN_ERROR_CODE");
		break;
	}
	return strResult;
}

USHORT GetCRCC(BYTE *p, int len)  //Snakyo CRC校验算法,p为校验数组首地址，
{                                        //len为需要校验的数据个数。
	USHORT k, crcc = 0;
	int m, n;

	for (m = 0; m < len; m++)
	{
		k = (USHORT)p[m];
		k <<= 8;

		for (n = 8; n > 0; n--)
		{
			if (((k ^ crcc) & 0x8000) != 0)
				crcc = (USHORT)((crcc << 1) ^ 0x1021);
			else
				crcc <<= 1;
			k <<= 1;
		}
	}

	return crcc;
}

long doEnableLogging()
{
	unsigned int uiRet = HIS_ALL_OK;
	BOOL bEnableLoging = TRUE;
	BOOL consoleOnOff = TRUE;
	XislLoggingLevels xislLogLvl = LEVEL_INFO;
	BOOL bPerformanceLogging = TRUE;

	// enable loggin functionality	开启日志
	uiRet = Acquisition_EnableLogging(bEnableLoging);
	if (uiRet != HIS_ALL_OK)
	{
		printf("Acquisition_EnableLogging Error nr.: %d", uiRet);
	}

	// define log outputfile and consolelogging	//定义日志输出文件和控制台日志
	uiRet = Acquisition_SetLogOutput("XLSlog.txt", consoleOnOff);
	if (uiRet != HIS_ALL_OK)
	{
		printf("Acquisition_SetLogOutput Error nr.: %d", uiRet);
	}

	// set the desired log out level	//设置日志级别
	uiRet = Acquisition_SetLogLevel(xislLogLvl);
	if (uiRet != HIS_ALL_OK)
	{
		printf("Acquisition_SetLogLevel Error nr.: %d", uiRet);
	}

	// log Performance which will report the time a function call needs	//记录性能，它将报告函数调用所需的时间
	uiRet = Acquisition_TogglePerformanceLogging(bPerformanceLogging);
	if (uiRet != HIS_ALL_OK)
	{
		printf("Acquisition_TogglePerformanceLogging Error nr.: %d", uiRet);
	}
	return uiRet;
}

//long DetectorInit(HACQDESC* phAcqDesc, long bGigETest)
//{
//
//	long idx = 0;    int major, minor, release, build;
//	char strVersion[256] = { 0 };
//	BOOL bEnableIRQ = 0;
//	int iRet;							// Return value
//	int iCnt;							// 
//	unsigned int uiNumSensors;			// Number of sensors
//	HACQDESC hAcqDesc = NULL;
//	unsigned short usTiming = 0;
//	unsigned short usNetworkLoadPercent = 80;
//
//	int iSelected;						// Index of selected GigE detector
//	long ulNumSensors = 0;				// nr of GigE detector in network
//
//	GBIF_DEVICE_PARAM Params;
//	GBIF_Detector_Properties Properties;
//
//	BOOL bSelfInit = TRUE;
//	long lOpenMode = HIS_GbIF_IP;
//	long lPacketDelay = 256;
//
//	unsigned int dwRows = 0, dwColumns = 0;
//
//	DWORD dwCharsWritten = 0;
//	char strBuffer[4000];  // buffer for outputs
//
//
//
//	// First we tell the system to enumerate all available sensors
//	// * to initialize them in polling mode, set bEnableIRQ = FALSE;
//	// * otherwise, to enable interrupt support, set bEnableIRQ = TRUE;
//	bEnableIRQ = TRUE;
//
//	if (bGigETest)
//	{
//		uiNumSensors = 0;
//
//		// find GbIF Detectors in Subnet
//		iRet = Acquisition_GbIF_GetDeviceCnt(&ulNumSensors);
//		if (iRet != HIS_ALL_OK)
//		{
//			sprintf_s(strBuffer, "%s fail! Error Code %d\t\t\t\t\n", "Acquisition_GbIF_GetDetectorCnt", iRet);
//			ZxcxLogINFO("strBuffer = %s", strBuffer);
//			return iRet;
//		}
//		else
//		{
//			sprintf_s(strBuffer, "%s\t\t\t\tPASS!\n", "Acquisition_GbIF_GetDetectorCnt");
//			//WriteConsole(hOutput, strBuffer, (DWORD)strlen(strBuffer), &dwCharsWritten, NULL);
//			ZxcxLogINFO("strBuffer = %s", strBuffer);
//		}
//
//		// do it with a single broadcast
//		iRet = Acquisition_GbIF_DiscoverDetectors();
//		if (HIS_ALL_OK != iRet)
//		{
//			sprintf_s(strBuffer, "%s fail! Error Code %d\t\t\t\t\n", "Acquisition_GbIF_DiscoverDetectors", iRet);
//			//WriteConsole(hOutput, strBuffer, (DWORD)strlen(strBuffer), &dwCharsWritten, NULL);
//			return iRet;
//		}
//		else
//		{
//			sprintf_s(strBuffer, "%s\t\t\t\tPASS!\n", "Acquisition_GbIF_DiscoverDetectors");
//			//WriteConsole(hOutput, strBuffer, (DWORD)strlen(strBuffer), &dwCharsWritten, NULL);
//		}
//
//		// this cannot fail if the previous call was successful
//		Acquisition_GbIF_DiscoveredDetectorCount(&ulNumSensors);
//
//		if (ulNumSensors > 0)
//		{
//			// get device params of GbIF Detectors in Subnet
//			GBIF_DEVICE_PARAM* pGbIF_DEVICE_PARAM = (GBIF_DEVICE_PARAM*)malloc(sizeof(GBIF_DEVICE_PARAM)*(ulNumSensors));
//
//#ifndef NEW_XISL
//			iRet = Acquisition_GbIF_GetDeviceList(pGbIF_DEVICE_PARAM, ulNumSensors);
//			if (iRet != HIS_ALL_OK)
//			{
//				sprintf_s(strBuffer, "%s fail! Error Code %d\t\t\t\t\n", "Acquisition_GbIF_GetDeviceList", iRet);
//				//WriteConsole(hOutput, strBuffer, (DWORD)strlen(strBuffer), &dwCharsWritten, NULL);
//				free(pGbIF_DEVICE_PARAM);
//				return iRet;
//			}
//			else
//			{
//				sprintf_s(strBuffer, "%s\t\t\t\tPASS!\n", "Acquisition_GbIF_GetDeviceList");
//			}
//
//			//WriteConsole(hOutput, strBuffer, (DWORD)strlen(strBuffer), &dwCharsWritten, NULL);
//#else
//			for (idx = 0; idx < ulNumSensors; ++idx)
//			{
//				// this cannot fail if the previous call was successful
//				Acquisition_GbIF_DiscoveredDetectorByIndex(idx, &pGbIF_DEVICE_PARAM[idx]);
//			}
//#endif
//
//			sprintf_s(strBuffer, "Select Sensor Nr:\n");
//			//WriteConsole(hOutput, strBuffer, (DWORD)strlen(strBuffer), &dwCharsWritten, NULL);
//
//			for (iCnt = 0; iCnt < ulNumSensors; iCnt++)
//			{
//				sprintf_s(strBuffer, "%d - %s\n", iCnt, (pGbIF_DEVICE_PARAM[iCnt]).cDeviceName);
//				//WriteConsole(hOutput, strBuffer, (DWORD)strlen(strBuffer), &dwCharsWritten, NULL);
//			}
//
//			while (!scanf_s("%d", &iSelected)) {}
//			sprintf_s(strBuffer, "%d - selected\n", iSelected);
//			//WriteConsole(hOutput, strBuffer, (DWORD)strlen(strBuffer), &dwCharsWritten, NULL);
//
//			if (iSelected > -1 && iSelected < ulNumSensors)
//			{
//				// try to init detector
//				uiNumSensors = 0;
//
//				iRet = Acquisition_GbIF_Init(
//					&hAcqDesc,
//					//iSelected,							// Index to access individual detector
//					0,										// here set to zero for a single detector device
//					bEnableIRQ,
//					dwRows, dwColumns,						// Image dimensions
//					bSelfInit,								// retrieve settings (rows,cols.. from detector
//					FALSE,									// If communication port is already reserved by another process, do not open
//					lOpenMode,								// here: HIS_GbIF_IP, i.e. open by IP address 
//					pGbIF_DEVICE_PARAM[iSelected].ucIP		// IP address of the connection to open
//				);
//
//				if (iRet != HIS_ALL_OK)
//				{
//					sprintf_s(strBuffer, "%s fail! Error Code %d\t\t\t\t\n", "Acquisition_GbIF_Init", iRet);
//					//WriteConsole(hOutput, strBuffer, (DWORD)strlen(strBuffer), &dwCharsWritten, NULL);
//					free(pGbIF_DEVICE_PARAM);
//					return iRet;
//				}
//				else
//				{
//					sprintf_s(strBuffer, "%s\t\t\t\tPASS!\n", "Acquisition_GbIF_Init");
//
//					// Calibrate connection
//					if (Acquisition_GbIF_CheckNetworkSpeed(hAcqDesc, &usTiming, &lPacketDelay, usNetworkLoadPercent) == HIS_ALL_OK)
//					{
//						sprintf_s(strBuffer, "%s result: suggested timing: %d packetdelay %lu @%d networkload\t\t\t\n",
//							"Acquisition_GbIF_CheckNetworkSpeed", usTiming, lPacketDelay, usNetworkLoadPercent);
//						//WriteConsole(hOutput, strBuffer, (DWORD)strlen(strBuffer), &dwCharsWritten, NULL);
//
//					}
//
//
//					iRet = Acquisition_GbIF_SetPacketDelay(hAcqDesc, lPacketDelay);
//					if (iRet != HIS_ALL_OK)
//					{
//						sprintf_s(strBuffer, "%s fail! Error Code %d\t\t\t\n", "Acquisition_GbIF_SetPacketDelay", iRet);
//						//WriteConsole(hOutput, strBuffer, (DWORD)strlen(strBuffer), &dwCharsWritten, NULL);
//						free(pGbIF_DEVICE_PARAM);
//						return iRet;
//					}
//					else
//					{
//						sprintf_s(strBuffer, "%s %lu\t\t\t\tPASS!\n", "Acquisition_GbIF_SetPacketDelay", lPacketDelay);
//					}
//					//WriteConsole(hOutput, strBuffer, (DWORD)strlen(strBuffer), &dwCharsWritten, NULL);
//
//					// Get Detector Params of already opened GigE Detector
//					iRet = Acquisition_GbIF_GetDeviceParams(hAcqDesc, &Params);
//					if (iRet != HIS_ALL_OK)
//					{
//						sprintf_s(strBuffer, "%s fail! Error Code %d\t\t\t\n", "Acquisition_GBIF_GetDeviceParams", iRet);
//						//WriteConsole(hOutput, strBuffer, (DWORD)strlen(strBuffer), &dwCharsWritten, NULL);
//						free(pGbIF_DEVICE_PARAM);
//						return iRet;
//					}
//					else
//					{
//						sprintf_s(strBuffer, "%s \t\t\t\tPASS!\n", "Acquisition_GBIF_GetDeviceParams");
//						sprintf_s(strBuffer, "Device Name: \t\t%s\nMAC: \t\t\t%s\nIP: \t\t\t%s\nSubnetMask: \t\t%s\nGateway: \t\t%s\nAdapterIP: \t\t%s\nAdapterSubnetMask: \t%s\nBootOptions Flag: \t%d\nGBIF Firmware: \t\t%s\n",
//							Params.cDeviceName,
//							Params.ucMacAddress,
//							Params.ucIP,
//							Params.ucSubnetMask,
//							Params.ucGateway,
//							Params.ucAdapterIP,
//							Params.ucAdapterMask,
//							Params.dwIPCurrentBootOptions,
//							Params.cGBIFFirmwareVersion
//						);
//						//WriteConsole(hOutput, strBuffer, (DWORD)strlen(strBuffer), &dwCharsWritten, NULL);
//					}
//
//					// Read production data
//					iRet = Acquisition_GbIF_GetDetectorProperties(hAcqDesc, &Properties);
//					if (iRet != HIS_ALL_OK)
//					{
//						sprintf_s(strBuffer, "%s fail! Error Code %d\t\t\t\n", "Acquisition_GbIF_GetDetectorProperties", iRet);
//						//WriteConsole(hOutput, strBuffer, (DWORD)strlen(strBuffer), &dwCharsWritten, NULL);
//						free(pGbIF_DEVICE_PARAM);
//						return -1;
//					}
//					else
//					{
//						sprintf_s(strBuffer, "%s \t\t\t\tPASS!\n", "Acquisition_GbIF_GetDetectorProperties");
//						sprintf_s(strBuffer, "Detector Type: \t\t%s\nManufDate: \t\t%s\nManufPlace: \t\t%s\n", Properties.cDetectorType,
//							Properties.cManufacturingDate, Properties.cPlaceOfManufacture);
//						///WriteConsole(hOutput, strBuffer, (DWORD)strlen(strBuffer), &dwCharsWritten, NULL);
//					}
//
//					*phAcqDesc = hAcqDesc;
//				}
//			}
//
//			free(pGbIF_DEVICE_PARAM);
//		}
//		else
//			return HIS_ERROR_NO_BOARD_IN_SUBNET;
//	}
//	else
//	{
//		//------------------------------------------------------------------------------------------------------------------//
//		// Automatic Initialization:																						//
//		// The XISL is able to recognize all sensors connected to the system automatically by its internal PROM settings.	//
//		// In case of GigE connected detectors only point-to-point connected are opened with Enum-Sensors					//
//		// The following code fragment shows the corresponding function call:												//
//		//------------------------------------------------------------------------------------------------------------------//
//		// ------------------------------------------------------------------------------------------------------------------//
//
//		//自动初始化：//
//
//		//XISL能够通过其内部PROM设置自动识别连接到系统的所有传感器//
//
//		//如果是GigE连接的探测器，则仅使用Enum传感器打开点对点连接//
//
//		//以下代码片段显示了相应的函数调用：//
//
//		//------------------------------------------------------------------------------------------------------------------//
//
//		if ((iRet = Acquisition_EnumSensors(&uiNumSensors,	// Number of sensors
//			1,				// Enable Interrupts
//			FALSE			// If communication port is already reserved by another process, do not open. If this parameter is TRUE the XISL is capturing all communication port regardless if this port is already opened by other processes running on the system, which is only recommended for debug versions.
//		)
//			) != HIS_ALL_OK)
//		{
//			sprintf_s(strBuffer, "%s fail! Error Code %d\t\t\t\t\n", "Acquisition_EnumSensors", iRet);
//			ZxcxLogERROR("strBuffer = %s", strBuffer);
//			return iRet;
//		}
//		else
//		{
//			sprintf_s(strBuffer, "%s\t\t\t\tPASS!\n", "Acquisition_EnumSensors");
//		}
//		ZxcxLogERROR("strBuffer = %s", strBuffer);
//	}
//
//	return HIS_ALL_OK;
//}

int InitializeDll()
{
	ZxcxLogINFO("");
	BYTE	bRet = DT_SUCCESS;
	unsigned int nRet = HIS_ALL_OK;
	int major, minor, release, build;
	char strVersion[256] = { 0 };
	char strBuffer[4000];  // buffer for outputs

	g_hAcqDesc = NULL;
	g_hMsgWnd = NULL;

	int iSelected = 1;

	CString path = GetInstanceFolderPath();

	ZxcxLogINFO("path:[%s]", path);
	g_CurrectionPath = path;

	//支持日志
	doEnableLogging();

	// Print the version number of the used library
	nRet = Acquisition_GetVersion(&major, &minor, &release, &build);

	if (HIS_ALL_OK != nRet)
	{
		sprintf_s(strBuffer, "%s fail! Error Code %d", "Acquisition_GetVersion", nRet);
		ZxcxLogINFO("strBuffer = %s", strBuffer);
		bRet = DT_ERROR_LOADLIBRARY;
	}

	sprintf_s(strBuffer, "Using XISL Version: %d.%d.%d.%d", major, minor, release, build);
	ZxcxLogINFO("strBuffer = %s", strBuffer);

	// Print the version number of the used GbIF library
	nRet = Acquisition_GbIF_GetVersion(&major, &minor, &release, strVersion, 256);

	if (HIS_ALL_OK != nRet)
	{
		sprintf_s(strBuffer, "%s fail! Error Code %d", "Acquisition_GbIF_GetVersion", nRet);
		ZxcxLogINFO("strBuffer = %s", strBuffer);
		bRet = DT_ERROR_LOADLIBRARY;
	}

	sprintf_s(strBuffer, "Using GbIF Version: %d.%d.%d.%s", major, minor, release, strVersion);
	ZxcxLogINFO("strBuffer = %s", strBuffer);

	// Print the version number of the used wpe library
	nRet = Acquisition_wpe_GetVersionNEW(&major, &minor, &release, &build);

	if (HIS_ALL_OK != nRet)
	{
		sprintf_s(strBuffer, "%s fail! Error Code %d", "Acquisition_wpe_GetVersionNEW", nRet);
		ZxcxLogINFO("strBuffer = %s", strBuffer);
		bRet = DT_ERROR_LOADLIBRARY;
	}

	sprintf_s(strBuffer, "Using wpe Version: %d.%d.%d.%d", major, minor, release, build);
	ZxcxLogINFO("strBuffer = %s", strBuffer);


	if (bRet == DT_SUCCESS)
		ZxcxLogINFO("(return bRet:[%s])", GetRtStr(bRet));
	else
		ZxcxLogERROR("(return bRet:[%s])", GetRtStr(bRet));

	return bRet;
}

bool CheckVersionFw(char *pcVersion)
{
	bool bRet = true;
	char acFilePath[500];
	char *pcEnd = 0;

	//GetModuleFileName (m_hInstance, acFilePath, sizeof (acFilePath));
	for (pcEnd = acFilePath + lstrlen(acFilePath); *(pcEnd - 1) != '.'; pcEnd--);
	lstrcpy(pcEnd, "ini");

	if (GetFileAttributes(acFilePath) != -1)
	{
		char acVersion[50];
		long lTotal = 0;

		bRet = false;
		lTotal = GetPrivateProfileInt("fw_version", "total", 0, acFilePath);
		for (long lIdx = 1; false == bRet && lIdx <= lTotal; lIdx++)
		{
			CString str;

			str.Format("fw_%d", lIdx);
			GetPrivateProfileString("fw_version", str, "", acVersion, sizeof(acVersion), acFilePath);
			bRet = (lstrcmpi(pcVersion, acVersion) == 0);
		}
	}
	return bRet;

}

bool MatchVersionFw(char *pcVersion)
{
	bool bRet = true;
	//char acFilePath[500];
	//char *pcEnd = 0;

	////GetModuleFileName (theApp.m_hInstance, acFilePath, sizeof (acFilePath));
	//for (pcEnd = acFilePath + lstrlen(acFilePath); *(pcEnd - 1) != '.'; pcEnd--);
	//lstrcpy(pcEnd, "ini");

	//if (GetFileAttributes(acFilePath) != -1)
	//{
	//	CString str;
	//	char acVersion[50];
	//	long lCurrent = 0;

	//	lCurrent = GetPrivateProfileInt("fw_version", "current", 0, acFilePath);

	//	str.Format("fw_%d", lCurrent);
	//	GetPrivateProfileString("fw_version", str, "", acVersion, sizeof(acVersion), acFilePath);
	//	bRet = (lstrcmpi(pcVersion, acVersion) == 0);
	//}
	return bRet;
}

void GetGenderStr(unsigned char *pucGender, unsigned long ulBufSize)
{
	if ('1' == pucGender[0])
	{
		lstrcpy((char *)pucGender, "男");
	}
	else if ('2' == pucGender[0])
	{
		lstrcpy((char *)pucGender, "女");
	}
	else
	{
		lstrcpy((char *)pucGender, "未知");
	}
}

void CErrorInfo::SetMTC(BYTE bCode)
{
	//CString	m_strMTCData;
	//BYTE  convertCode[5];

	//memset(m_szMTC, 0x00, sizeof(m_szMTC));
	//memset(m_szMTCDesc, 0x00, sizeof(m_szMTCDesc));
	//memset(convertCode, 0x00, sizeof(convertCode));

	//MakeUnPack(&bCode, convertCode, 1);

	//m_szMTC[0] = MTC_SUB;
	//m_szMTC[1] = MTC_DEVICE;
	//m_szMTC[2] = '0';
	//m_szMTC[3] = convertCode[0];
	//m_szMTC[4] = convertCode[1];

	//m_strMTCData = MTCodeToData(bCode);
	//memcpy(m_szMTCDesc, m_strMTCData, m_strMTCData.GetLength());
}


CString CErrorInfo::MTCodeToData(int errCode)
{
	CString m_strMTCdata = _T("");
	//CString strIniFileName = _T("");
	//char szCurPath[MAX_PATH] = { 0 };
	//char szErrCode[4] = { 0 };
	//MakeUnPack(&errCode, szErrCode, 1);

	//switch (errCode)
	//{
	//case DEV_ERR_IDC_HWERROR_LOW:
	//	m_strMTCdata = _T("Device Hwerror_Low");
	//	break;
	//case DEV_ERR_IDC_RETUIN:
	//	m_strMTCdata = _T("Device Retuin Card");
	//	break;
	//case DEV_ERR_IDC_TIMEOUT:
	//	m_strMTCdata = _T("Device TimeOut");
	//	break;
	//case DEV_ERR_IDC_TYPE:
	//	m_strMTCdata = _T("Device Card Type");
	//	break;
	//case DEV_ERR_IDC_RF:
	//	m_strMTCdata = _T("Device Card Rf Fail");
	//	break;
	//case DEV_ERR_IDC_MOVE:
	//	m_strMTCdata = _T("Device Card Move Fail");
	//	break;
	//case DEV_ERR_IDC_JAM:
	//	m_strMTCdata = _T("Device Card JAM");
	//	break;
	//case DEV_ERR_IDC_MEDIA:
	//	m_strMTCdata = _T("Device Card Media");;
	//	break;
	//case DEV_ERR_IDC_RESET:
	//	m_strMTCdata = _T("Device Reset Fail");;
	//	break;
	//case DEV_ERR_IDC_STATUS:
	//	m_strMTCdata = _T("Device Status Fail");
	//	break;
	//case DEV_ERR_IDC_RFCONTROL:
	//	m_strMTCdata = _T("Device RfControl Fail");;
	//	break;
	//case DEV_ERR_IDC_SCAN:
	//	m_strMTCdata = _T("Device Scan Fail");;
	//	break;
	//case DEV_ERR_IDC_AUTHEN:
	//	m_strMTCdata = _T("Device Authen Fail");;
	//	break;
	//case DEV_ERR_IDC_CALIB:
	//	m_strMTCdata = _T("Device Calib Fail");;
	//	break;
	//case DEV_ERR_IDC_BMP2JPEG:
	//	m_strMTCdata = _T("Device BMP2JPEG Fail");;
	//	break;
	//case DEV_ERR_IDC_UPGRADER:
	//	m_strMTCdata = _T("Device Upgrader Fail");;
	//	break;
	//case DEV_ERR_IDC_SCANLEN:
	//	m_strMTCdata = _T("Device ScanLen Fail");
	//	break;
	//case DT_SUCCESS:
	//	m_strMTCdata = _T("Device Success");
	//	break;
	//case DT_CLOSEFAIL:
	//	m_strMTCdata = _T("Device Close Fail");
	//	break;
	//case DT_FAILURE:
	//	m_strMTCdata = _T("Device Send Recv Fail");
	//	break;
	//case DT_ERROR_ALREADYOPEN:
	//	m_strMTCdata = _T("Device Already Open");
	//	break;
	//case DT_ERROR_LOADLIBRARY:
	//	m_strMTCdata = _T("Device Loadlibrary Fail");
	//	break;
	//default:
	//	m_strMTCdata = _T("Unknown MTC code");
	//	break;
	//}
	return m_strMTCdata;
}
CString WINAPI GetLastSubErr(BYTE code)
{
	CString ErrorMsg = _T("");

	//g_szMTCErrorCode.SetMTC(code);
	//ErrorMsg.Format("%.200s[MTC:%.5s]", g_szMTCErrorCode.m_szMTCDesc, g_szMTCErrorCode.m_szMTC);

	return ErrorMsg;
}


CString GetFileVersion(char* strFileName)
{
	CString strVersion = "";
	DWORD dwLen = GetFileVersionInfoSize(strFileName, NULL);
	ASSERT(dwLen != 0);
	BYTE* pBlock = new BYTE[dwLen + 1];

	ZeroMemory(pBlock, dwLen + 1);
	GetFileVersionInfo(strFileName, 0, dwLen, pBlock);

	VS_FIXEDFILEINFO* pFileInfo;
	UINT uLen = 0;
	VerQueryValue(pBlock, TEXT("\\"), (void**)&pFileInfo, &uLen);

	WORD h_1 = HIWORD(pFileInfo->dwFileVersionMS);
	WORD h_2 = LOWORD(pFileInfo->dwFileVersionMS);
	WORD l_1 = HIWORD(pFileInfo->dwFileVersionLS);
	WORD l_2 = LOWORD(pFileInfo->dwFileVersionLS);
	strVersion.Format("%d.%d.%d.%d", h_1, h_2, l_1, l_2);
	delete[]pBlock;

	return strVersion;
}

CString GetInstanceFolderPath(void)
{
	CString strPath;
	char tcFullPath[MAX_PATH];

	GetModuleFileName(g_hInstance, tcFullPath, MAX_PATH);

	if (0 != GetModuleFileName(g_hInstance, tcFullPath, MAX_PATH - 1))
	{
		tcFullPath[MAX_PATH - 1] = '\0';
	}

	TCHAR* p = _tcsrchr(tcFullPath, '\\') + 1;
	if (p)
	{
		*p = '\0';
		strPath = tcFullPath;
	}

	return strPath;
}

/*
 *函 数 名：wuslen
 *功    能：计算unsigned short*字符串长度
 *输入参数：str unsigned short*字符串
 *输出参数：无
 *返 回 值：unsigned short*字符串
 */
int wuslen(const unsigned short* str)
{
	int i = 0;

	if (str == NULL)
		return i;

	while (str[i])
	{
		i++;
	}

	return i;
}

int wuclen(const unsigned char* str)
{
	int i = 0;

	if (str == NULL)
		return i;

	while (str[i])
	{
		i++;
	}

	return i;
}

int wlslen(const unsigned long* str)
{
	int i = 0;

	if (str == NULL)
		return i;

	while (str[i])
	{
		i++;
	}

	return i;
}

int ProcImgBuf16()
{
	return TRUE;
}
int ProcVideoBuf16()
{
	//Image_callback(2, 3, 4, data);
	return TRUE;
}