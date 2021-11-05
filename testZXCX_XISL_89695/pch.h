// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#include "framework.h"


#include "../ZXCX_XISL_89695/ZXCX_XISL_89695.h"
#include "ZxcxLog.h"

extern CZxcxLog*			g_ptestZXCX_XISL_89695Logger;

#ifndef ZxcxLogINFO
#	define ZxcxLogINFO(format, ...)			g_ptestZXCX_XISL_89695Logger->LogInfo("<%s ,%d> %s\t"format,  __FILE__, __LINE__, __FUNCTION__,__VA_ARGS__)
#endif

#ifndef ZxcxLogERROR
#	define ZxcxLogERROR(format, ...)		g_ptestZXCX_XISL_89695Logger->LogError("<%s ,%d> %s\t"format, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#endif

#ifndef ZxcxLogWARN
#	define ZxcxLogWARN(format, ...)			g_ptestZXCX_XISL_89695Logger->LogWarn("<%s ,%d> %s\t"format,__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#endif

#ifndef ZxcxLogDEBUG
#	ifdef _DEBUG
#		define ZxcxLogDEBUG(format, ...)	g_ptestZXCX_XISL_89695Logger->LogDebug("<%s ,%d> %s\t"format,__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#	else // _DEUBG
#		if _MSC_VER <1300
#			define ZxcxLogDEBUG			((void)0)
#		else
#			define ZxcxLogDEBUG          (__noop)
#		endif
#	endif
#endif

#endif //PCH_H
