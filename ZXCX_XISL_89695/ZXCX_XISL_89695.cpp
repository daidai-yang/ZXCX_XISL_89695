// ZXCX_XISL_89695.cpp : 定义 DLL 的导出函数。
//

#include "pch.h"

//fnZXCX_XISL_ImageHandler XISL_Image_callback;

ZXCXXISL89695_API int WINAPI fnZXCX_XISL_Open()
{
	ZxcxLogINFO("");
	int nRet = DT_FAILURE;

	if ((nRet = fnDevXISL_Acq_Open()) != DT_SUCCESS)
	{
		ZxcxLogERROR(_T("<%s  %d> %s(return nRet:[%s])"), __FILE__, __LINE__, __FUNCTION__, GetRtStr(nRet));
		return nRet;
	}


	ZxcxLogINFO("end");
	return nRet;
}
ZXCXXISL89695_API int WINAPI fnZXCX_XISL_Close()
{
	ZxcxLogINFO("");
	int nRet = DT_FAILURE;

	if ((nRet = fnDevXISL_Acq_Close()) != DT_SUCCESS)
	{
		ZxcxLogERROR(_T("<%s  %d> %s(return nRet:[%s])"), __FILE__, __LINE__, __FUNCTION__, GetRtStr(nRet));
		return nRet;
	}

	return nRet;

}
ZXCXXISL89695_API int WINAPI fnZXCX_XISL_GetXISLVersion()
{
	int nRet = DT_FAILURE;

	return nRet;

}
ZXCXXISL89695_API int WINAPI fnZXCX_XISL_GetStatus()
{
	int nRet = DT_FAILURE;

	return nRet;

}
ZXCXXISL89695_API int WINAPI fnZXCX_XISL_Reset()
{
	int nRet = DT_FAILURE;

	return nRet;

}
ZXCXXISL89695_API int WINAPI fnZXCX_XISL_GetDevVersion()
{
	int nRet = DT_FAILURE;

	return nRet;

}

ZXCXXISL89695_API int WINAPI fnZXCX_XISL_Start_GetImage()
{
	ZxcxLogINFO("");
	int nRet = DT_FAILURE;

	int frameId = 0;
	int width = 0;
	int height = 0;
	void* data = NULL;

	fnDevXISL_Acq_Image(TRUE);
	
	return nRet;
}

ZXCXXISL89695_API int WINAPI fnZXCX_XISL_Stop_GetImage()
{
	ZxcxLogINFO("");
	int nRet = DT_FAILURE;

	int frameId = 0;
	int width = 0;
	int height = 0;
	void* data = NULL;

	fnDevXISL_Acq_Image(FALSE);


	return nRet;
}

//暗场校正
ZXCXXISL89695_API int WINAPI fnZXCX_XISL_Dark_Currection()
{
	ZxcxLogINFO("");
	int nRet = DT_FAILURE;

	if ((nRet = fnDevXISL_Acq_OffsetImage()) != DT_SUCCESS)
	{
		ZxcxLogERROR(_T("<%s  %d> %s(return nRet:[%s])"), __FILE__, __LINE__, __FUNCTION__, GetRtStr(nRet));
		return nRet;
	}

	return nRet;
}

//亮场校正
ZXCXXISL89695_API int WINAPI fnZXCX_XISL_Bright_Currection()
{
	ZxcxLogINFO("");
	int nRet = DT_FAILURE;

	if ((nRet = fnDevXISL_Acq_GainImage()) != DT_SUCCESS)
	{
		ZxcxLogERROR("return nRet:[%s]", GetRtStr(nRet));
		return nRet;
	}

	return nRet;
}

ZXCXXISL89695_API int WINAPI fnZXCX_XISL_SetCameraMode()
{
	ZxcxLogINFO("");
	int nRet = DT_FAILURE;

	if ((nRet = fnDevXISL_Acq_SetCameraMode()) != DT_SUCCESS)
	{
		ZxcxLogERROR("return nRet:[%s]", GetRtStr(nRet));
		return nRet;
	}

	return nRet;
}

ZXCXXISL89695_API int WINAPI fnZXCX_XISL_SetCameraGain()
{
	ZxcxLogINFO("");
	int nRet = DT_FAILURE;

	if ((nRet = fnDevXISL_Acq_SetCameraGain(0)) != DT_SUCCESS)
	{
		ZxcxLogERROR("return nRet:[%s]", GetRtStr(nRet));
		return nRet;
	}

	return nRet;
}

ZXCXXISL89695_API int WINAPI fnZXCX_XISL_SetCameraTrigger()
{
	ZxcxLogINFO("");
	int nRet = DT_FAILURE;

	if ((nRet = fnDevXISL_Acq_SetCameraTriggerMode(0)) != DT_SUCCESS)
	{
		ZxcxLogERROR("return nRet:[%s]", GetRtStr(nRet));
		return nRet;
	}

	return nRet;
}

ZXCXXISL89695_API int WINAPI fnZXCX_XISL_WWWLMapping(unsigned short* input_image, unsigned char* output_image, int image_width, int image_height, float ww_val, float wl_val)
{
	ZxcxLogINFO("");
	int nRet = DT_FAILURE;

	if ((nRet = fnDev_WWWLMapping(input_image, output_image, image_width, image_height, ww_val, wl_val)) != DT_SUCCESS)
	{
		ZxcxLogERROR("return nRet:[%s]", GetRtStr(nRet));
		return nRet;
	}

	return nRet;
}

ZXCXXISL89695_API int WINAPI fnZXCX_XISL_Register_ImageCallback(fnZXCX_XISL_ImageHandler callBackFun)
{
	ZxcxLogINFO("");
	int nRet = DT_FAILURE;

	//XISL_Image_callback = callBackFun;
	fnDev_Acq_Register_ImageCallback(callBackFun);
	return nRet;
}

ZXCXXISL89695_API int WINAPI fnZXCX_XISL_Register_Test()
{
	ZxcxLogINFO("");
	int nRet = DT_FAILURE;

	if ((nRet = fnDev_Acq_Register_Test()) != DT_SUCCESS)
	{
		ZxcxLogERROR("return nRet:[%s]", GetRtStr(nRet));
		return nRet;
	}

	return nRet;
}