#pragma once

#ifndef FUNCTION_H
#define  FUNCTION_H

#include <string>
using namespace std;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);

BOOL				CreateMsgWnd(HINSTANCE hInst, HWND *phWnd);

// Util.cpp
USHORT				GetCRCC(BYTE *p, int len);
int					InitializeDll();

void				GetGenderStr(unsigned char *pucGender, unsigned long ulBufSize);

bool				CheckVersionFw(char *pcVersion);
bool				MatchVersionFw(char *pcVersion);

CString				GetFileVersion(char* strFileName);

CString				GetInstanceFolderPath(void);

int					wuslen(const unsigned short* str);
int					wuclen(const unsigned char* str);
int					wlslen(const unsigned long* str);

int					ProcImgBuf16();
int					ProcVideoBuf16();

#endif