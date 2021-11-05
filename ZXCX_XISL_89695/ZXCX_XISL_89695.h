// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 ZXCXXISL89695_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// ZXCXXISL89695_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef ZXCXXISL89695_EXPORTS
#define ZXCXXISL89695_API __declspec(dllexport)
#else
#define ZXCXXISL89695_API __declspec(dllimport)
#endif

ZXCXXISL89695_API CString WINAPI GetRtStr(BYTE code);

ZXCXXISL89695_API int WINAPI fnZXCX_XISL_Open();
ZXCXXISL89695_API int WINAPI fnZXCX_XISL_Close();
ZXCXXISL89695_API int WINAPI fnZXCX_XISL_GetXISLVersion();
ZXCXXISL89695_API int WINAPI fnZXCX_XISL_GetStatus();
ZXCXXISL89695_API int WINAPI fnZXCX_XISL_Reset();
ZXCXXISL89695_API int WINAPI fnZXCX_XISL_GetDevVersion();

//开始采集图片
ZXCXXISL89695_API int WINAPI fnZXCX_XISL_Start_GetImage();

//结束采集图片
ZXCXXISL89695_API int WINAPI fnZXCX_XISL_Stop_GetImage();

//暗场校正
ZXCXXISL89695_API int WINAPI fnZXCX_XISL_Dark_Currection();

//亮场校正
ZXCXXISL89695_API int WINAPI fnZXCX_XISL_Bright_Currection();

//设置积分时间
ZXCXXISL89695_API int WINAPI fnZXCX_XISL_SetCameraMode();

//设置增益倍数
ZXCXXISL89695_API int WINAPI fnZXCX_XISL_SetCameraGain();

//设置信号触发方式
ZXCXXISL89695_API int WINAPI fnZXCX_XISL_SetCameraTrigger();

//窗宽窗位设置 16位图像数据按照指定窗宽窗位保存到8位数据中
ZXCXXISL89695_API int WINAPI fnZXCX_XISL_WWWLMapping(unsigned short* input_image, unsigned char* output_image, int image_width, int image_height, float ww_val, float wl_val);

//16位转8位
ZXCXXISL89695_API int WINAPI fnZXCX_XISL_Bright_Currection();

//定义回调函数
typedef int(CALLBACK *fnZXCX_XISL_ImageHandler)(int frameId, int width, int height, void* data);

//注册回调函数
ZXCXXISL89695_API int WINAPI fnZXCX_XISL_Register_ImageCallback(fnZXCX_XISL_ImageHandler callBackFun);

ZXCXXISL89695_API int WINAPI fnZXCX_XISL_Register_Test();
