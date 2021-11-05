// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"

// 当使用预编译的头时，需要使用此源文件，编译才能成功。
HWND					g_hMsgWnd;
HINSTANCE				g_hInstance;
CString					g_strSubVersion;
CZxcxLog*				g_pXISL_89695Logger;

HACQDESC				g_hAcqDesc;

CString					g_CurrectionPath;
BOOL					g_bVideoAcqState;