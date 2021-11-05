// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 ZxcxLog_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// ZxcxLog_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef ZXCXLOG_EXPORTS
#define ZxcxLog_API __declspec(dllexport)
#else
#define ZxcxLog_API __declspec(dllimport)
#endif

#include <windows.h>

// 此类是从 ZxcxLog.dll 导出的
class ZxcxLog_API CZxcxLog {
public:
	CZxcxLog(BYTE* pszSubName);
	~CZxcxLog();

	enum EnumType{
		CX_LOG_MESSAGE = 0,
		CX_LOG_WARNING,
		CX_LOG_EXCEPTION,
		CX_LOG_ERROR,
		CX_LOG_DEBUG
	};

	BOOL Open(LPCTSTR log_base_dir, int enable_auto_delete, int auto_delete_keep_days);
	void Close();

	void LogInfo(LPCTSTR lpszFormat, ...);
	void LogError(LPCTSTR lpszFormat, ...);
	void LogWarn(LPCTSTR lpszFormat, ...);
	void LogDebug(LPCTSTR lpszFormat, ...);
	void LogException(LPCTSTR lpszFormat, ...);
private:
	void Logv(int type, LPCTSTR lpszFormat, va_list arg);
	void CheckChangeFile(SYSTEMTIME &st, int curr_stamp, int day_sec);
	void WriteRecord(const char *line);
	void CleanFiles();

private:
	TCHAR m_logdir[260];
	CRITICAL_SECTION m_lock;
	HANDLE m_logfile;
	int m_log_enable;
	int m_logfile_stamp;
	int m_auto_delete;
	int m_auto_delete_keep_days;
};
  //dd
/* Class object or Class object point use module name
#ifndef ZxcxLogINFO
#	define ZxcxLogINFO	g_logger->LogInfo
#endif

#ifndef ZxcxLogERROR
#	define ZxcxLogERROR	g_logger->LogError
#endif

#ifndef ZxcxLogWARN
#	define ZxcxLogWARN	g_logger->LogWarn
#endif

#ifndef ZxcxLogDEBUG
#	ifdef _DEBUG
#		define ZxcxLogDEBUG				g_logger->LogDebug
#	else // _DEUBG
#		if _MSC_VER <1300
#			define ZxcxLogDEBUG			((void)0)
#		else
#			define ZxcxLogDEBUG          (__noop)
#		endif
#	endif
#endif
*/

