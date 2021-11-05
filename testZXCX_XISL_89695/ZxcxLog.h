// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� ZxcxLog_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// ZxcxLog_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef ZXCXLOG_EXPORTS
#define ZxcxLog_API __declspec(dllexport)
#else
#define ZxcxLog_API __declspec(dllimport)
#endif

#include <windows.h>

// �����Ǵ� ZxcxLog.dll ������
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

