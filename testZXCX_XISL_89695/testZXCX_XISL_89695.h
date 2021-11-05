
// testZXCX_XISL_89695.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// 主符号


// CtestZXCXXISL89695App:
// 有关此类的实现，请参阅 testZXCX_XISL_89695.cpp
//

class CtestZXCXXISL89695App : public CWinApp
{
public:
	CtestZXCXXISL89695App();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CtestZXCXXISL89695App theApp;
