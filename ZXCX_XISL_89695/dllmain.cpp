// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		g_hInstance = hModule;

		g_pXISL_89695Logger = new CZxcxLog((BYTE*)"ZXCX_XISL_89695");
		ZxcxLogINFO("(====================ZXCX_XISL_89695 DLL START V1.0.0.1 2021-08-19====================)");

		char	tempPath[MAX_PATH] = { 0 };
		GetModuleFileName(g_hInstance, tempPath, MAX_PATH);

		g_strSubVersion = GetFileVersion(tempPath);

		InitializeDll();
		CreateMsgWnd(g_hInstance, &g_hMsgWnd);
	}
	break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
	{
		ZxcxLogINFO("DLL_PROCESS_DETACH");
		if (g_pXISL_89695Logger != NULL)
		{
			delete g_pXISL_89695Logger;
			g_pXISL_89695Logger = NULL;
		}
		if (g_hMsgWnd != NULL)
		{
			DestroyWindow(g_hMsgWnd);
			g_hMsgWnd = NULL;
		}
		break;
	}
	}
	return TRUE;
}

BOOL CreateMsgWnd(HINSTANCE hInst, HWND *phWnd) // Message Window Create
{
	WNDCLASS stWndClass;

	stWndClass.cbClsExtra = 0;
	stWndClass.cbWndExtra = 0;
	stWndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	stWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	stWndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	stWndClass.hInstance = hInst;
	stWndClass.lpfnWndProc = (WNDPROC)WndProc;
	stWndClass.lpszClassName = "MsgWnd";
	stWndClass.lpszMenuName = NULL;
	stWndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&stWndClass);

	*phWnd = CreateWindow("MsgWnd", NULL, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInst, NULL);

	if (*phWnd == NULL)
	{
		return FALSE;
	}

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)
	{
	case WM_MSG_RECV_IMGDATA:
		ZxcxLogINFO("case WM_MSG_RECVDATA");
		ProcImgBuf16();
		break;
	case WM_MSG_RECV_VIDEODATA:
		ZxcxLogINFO("case WM_MSG_RECVDATA");
		ProcVideoBuf16();
		break;
	case WM_CLOSE:
		ZxcxLogINFO("case WM_CLOSE");
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
	default:
		break;
	}
	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}

