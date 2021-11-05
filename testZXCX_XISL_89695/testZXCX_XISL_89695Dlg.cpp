
// testZXCX_XISL_89695Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "testZXCX_XISL_89695.h"
#include "testZXCX_XISL_89695Dlg.h"
#include "afxdialogex.h"
#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/highgui.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestZXCXXISL89695Dlg 对话框
CtestZXCXXISL89695Dlg* CtestZXCXXISL89695Dlg::m_instance = NULL;

CtestZXCXXISL89695Dlg::CtestZXCXXISL89695Dlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_TESTZXCX_XISL_89695_DIALOG, pParent)
	,m_szInfo(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_instance = this;
}

void CtestZXCXXISL89695Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INFO, m_szInfo);
	DDX_Control(pDX, IDC_SLIDER1, m_ww_val);
	DDX_Control(pDX, IDC_SLIDER2, m_wl_val);
}

BEGIN_MESSAGE_MAP(CtestZXCXXISL89695Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CtestZXCXXISL89695Dlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON2, &CtestZXCXXISL89695Dlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_StartImage, &CtestZXCXXISL89695Dlg::OnBnClickedButtonStartimage)
	ON_BN_CLICKED(IDC_BUTTON_EndImage, &CtestZXCXXISL89695Dlg::OnBnClickedButtonEndimage)
	ON_BN_CLICKED(IDC_BUTTON_DarkCorrection, &CtestZXCXXISL89695Dlg::OnBnClickedButtonDarkcorrection)
	ON_BN_CLICKED(IDC_BUTTON_BrightCorrection, &CtestZXCXXISL89695Dlg::OnBnClickedButtonBrightcorrection)
	ON_BN_CLICKED(IDC_BUTTON_TestHuidiao, &CtestZXCXXISL89695Dlg::OnBnClickedButtonTesthuidiao)
	ON_BN_CLICKED(IDC_BUTTON_Huidiao2, &CtestZXCXXISL89695Dlg::OnBnClickedButtonHuidiao2)
	ON_BN_CLICKED(IDC_BUTTON_OpenImg, &CtestZXCXXISL89695Dlg::OnBnClickedButtonOpenimg)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CtestZXCXXISL89695Dlg::OnNMCustomdrawSlider_ww)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CtestZXCXXISL89695Dlg::OnNMCustomdrawSlider_wl)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_SaveBMP, &CtestZXCXXISL89695Dlg::OnBnClickedButtonSavebmp)
END_MESSAGE_MAP()


// CtestZXCXXISL89695Dlg 消息处理程序

BOOL CtestZXCXXISL89695Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//ShowWindow(SW_MAXIMIZE);

	g_ptestZXCX_XISL_89695Logger = new CZxcxLog((BYTE*)"testZXCX_XISL_89695");
	ZxcxLogINFO("(====================testZXCX_XISL_89695.exe START V1.0.0.1 2021-08-19====================)");

	m_ww_val.SetRange(0, 65535);
	m_wl_val.SetRange(0, 65535);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtestZXCXXISL89695Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CtestZXCXXISL89695Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int __stdcall OnImage(int frameId, int width, int height, void* data)
{
	ZxcxLogINFO("OnImage   ImageHandler, %d, %d, %d", frameId, width, height);
	
	CtestZXCXXISL89695Dlg::m_instance->m_SendMessageDlg(frameId, width, height, data);
	return TRUE;
}

void CtestZXCXXISL89695Dlg::m_SendMessageDlg(int frameId, int width, int height, void* data)
{
	CString tmp;
	tmp.Format("OnImage   ImageHandler frameId=%d. width=%d. height=%d", frameId, width, height);

	ZxcxLogINFO("tmp = %s", tmp);
	
	CtestZXCXXISL89695Dlg::m_instance->m_frameId = frameId;
	CtestZXCXXISL89695Dlg::m_instance->m_width = width;
	CtestZXCXXISL89695Dlg::m_instance->m_height = height;

	ZxcxLogINFO("m_frameId = %d", m_frameId);
	ZxcxLogINFO("m_width = %d", m_frameId);
	ZxcxLogINFO("m_height = %d", m_height);

	CtestZXCXXISL89695Dlg::m_instance->m_szInfo = tmp;
	CtestZXCXXISL89695Dlg::m_instance->UpdateData(FALSE);
	return;
}

void CtestZXCXXISL89695Dlg::OnBnClickedButtonOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	int Ret = fnZXCX_XISL_Open();
	if (Ret == 0)
	{
		m_szInfo = "打开成功！";
		UpdateData(FALSE);
	}
	else
	{
		m_szInfo = "打开失败！";
		UpdateData(FALSE);
	}

	//注册回调
	//fnZXCX_XISL_Register_ImageCallback(OnImage);

}

void CtestZXCXXISL89695Dlg::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	int Ret = fnZXCX_XISL_Close();
	if (Ret == 0)
	{
		m_szInfo = "关闭成功！";
		UpdateData(FALSE);
	}
	else
	{
		m_szInfo = "关闭失败！";
		UpdateData(FALSE);
	}

}

void CtestZXCXXISL89695Dlg::OnBnClickedButtonStartimage()
{
	// TODO: 在此添加控件通知处理程序代码
	int Ret = fnZXCX_XISL_Start_GetImage();
	if (Ret == 0)
	{
		m_szInfo = "开始成功！";
		UpdateData(FALSE);
	}
	else
	{
		m_szInfo = "开始失败！";
		UpdateData(FALSE);
	}
}

void CtestZXCXXISL89695Dlg::OnBnClickedButtonEndimage()
{
	// TODO: 在此添加控件通知处理程序代码
	int Ret = fnZXCX_XISL_Stop_GetImage();
	if (Ret == 0)
	{
		m_szInfo = "结束成功！";
		UpdateData(FALSE);
	}
	else
	{
		m_szInfo = "结束失败！";
		UpdateData(FALSE);
	}
}


void CtestZXCXXISL89695Dlg::OnBnClickedButtonDarkcorrection()
{
	// TODO: 在此添加控件通知处理程序代码
	int Ret = fnZXCX_XISL_Dark_Currection();
		if (Ret == 0)
		{
			m_szInfo = "暗场校正成功！";
			UpdateData(FALSE);
		}
		else
		{
			m_szInfo = "暗场校正失败！";
			UpdateData(FALSE);
		}
}

void CtestZXCXXISL89695Dlg::OnBnClickedButtonBrightcorrection()
{
	// TODO: 在此添加控件通知处理程序代码
	int Ret = fnZXCX_XISL_Bright_Currection();
	if (Ret == 0)
	{
		m_szInfo = "亮场校正成功！";
		UpdateData(FALSE);
	}
	else
	{
		m_szInfo = "亮场校正失败！";
		UpdateData(FALSE);
	}
}

void CtestZXCXXISL89695Dlg::OnBnClickedButtonTesthuidiao()
{
	// TODO: 在此添加控件通知处理程序代码
	int Ret = fnZXCX_XISL_Register_Test();
	if (Ret == 0)
	{
		m_szInfo = "回调测试成功！";
		UpdateData(FALSE);
	}
	else
	{
		m_szInfo = "回调测试失败！";
		UpdateData(FALSE);
	}
}

void CtestZXCXXISL89695Dlg::OnBnClickedButtonHuidiao2()
{
	// TODO: 在此添加控件通知处理程序代码
	int Ret = fnZXCX_XISL_Register_ImageCallback(OnImage);
	if (Ret == 0)
	{
		m_szInfo = "注册成功！";
		UpdateData(FALSE);
	}
	else
	{
		m_szInfo = "注册失败！";
		UpdateData(FALSE);
	}
}


void CtestZXCXXISL89695Dlg::OnBnClickedButtonOpenimg()
{
	// TODO: 在此添加控件通知处理程序代码
	CString BmpName;
	//CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("位图文件(*.BMP)|*.BMP|jpg文件(*.jpg)|*.jpg|raw文件(*.raw)|*.raw||")); //这行代码可以打开BMP和JPG格式的图片
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, _T("raw文件(*.raw)|*.raw|位图文件(*.BMP)|*.BMP|jpg文件(*.jpg)|*.jpg||"));
	if (IDOK == dlg.DoModal())
	{
		BmpName.Format("%s", dlg.GetPathName());

		m_ww_val.SetPos(0);
		m_wl_val.SetPos(0);
		Showpicture(BmpName);
	}
}

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

void CtestZXCXXISL89695Dlg::Showpicture(CString imgpath)
{
	//1.读取raw的数据
	CString rawPath = imgpath;
	FILE *pFile = fopen(rawPath, "rb");

	fseek(pFile, 0, SEEK_END);
	int Offsetlen = ftell(pFile);
	rewind(pFile);

	if (pRawBuffer)
	{
		free(pRawBuffer);
		pRawBuffer = NULL;
	}
	
	pRawBuffer = (unsigned short *)malloc(sizeof(unsigned short)*Offsetlen);
	fread(pRawBuffer, 1, Offsetlen, pFile);
	fclose(pFile);

	if (pOutBuffer)
	{
		delete[] pOutBuffer;
		pOutBuffer = NULL;
	}
	pOutBuffer = new unsigned char[1024 * 1024];

	fnZXCX_XISL_WWWLMapping(pRawBuffer, pOutBuffer, 1024, 1024, 0, 0);

	cv::Mat tmpImg1(1024, 1024, CV_8UC1, pOutBuffer);

	rotate(tmpImg1, tmpImg1, cv::ROTATE_90_COUNTERCLOCKWISE);

	//cv::imwrite("1.bmp", tmpImg1);

	CRect rect;
	GetDlgItem(IDC_STATIC_ShowImg)->GetClientRect(&rect);  // 获取图片控件矩形框
	// 缩放Mat，以适应图片控件大小
	cv::resize(tmpImg1, tmpImg1, cv::Size(rect.Width(), rect.Height()));

	// 转换格式 ,便于获取BITMAPINFO
	switch (tmpImg1.channels())
	{
	case 1:
		cv::cvtColor(tmpImg1, tmpImg1, CV_GRAY2BGRA); // GRAY单通道
		break;
	case 3:
		cv::cvtColor(tmpImg1, tmpImg1, CV_BGR2BGRA);  // BGR三通道
		break;
	default:
		break;
	}

	int pixelBytes = tmpImg1.channels()*(tmpImg1.depth() + 1); // 计算一个像素多少个字节
	// 制作bitmapinfo(数据头)
	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
	bitInfo.bmiHeader.biWidth = tmpImg1.cols;
	bitInfo.bmiHeader.biHeight = -tmpImg1.rows;   //注意"-"号(正数时倒着绘制)
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0;

	CDC *pDC = GetDlgItem(IDC_STATIC_ShowImg)->GetDC();  //获取图片控件DC

	//绘图
	::StretchDIBits(
		pDC->GetSafeHdc(),
		0, 0, rect.Width(), rect.Height(),
		0, 0, tmpImg1.cols, tmpImg1.rows,
		tmpImg1.data,
		&bitInfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);
	ReleaseDC(pDC);  //释放DC

}

void CtestZXCXXISL89695Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateData(TRUE);
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSliderCtrl   *pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	//m_int 即为当前滑块的值。
	nWwPos = 1 * pSlidCtrl->GetPos();//取得当前位置值  
	char str1[256] = { 0 };
	_itoa(nWwPos, str1, 10); // 将滑动值由int型变为char 
	GetDlgItem(IDC_EDIT1)->SetWindowText(str1);//显示在编辑框

	CSliderCtrl   *pSlidCtr2 = (CSliderCtrl*)GetDlgItem(IDC_SLIDER2);
	//m_int 即为当前滑块的值。
	nWlPos = 1 * pSlidCtr2->GetPos();//取得当前位置值  
	char str2[256] = { 0 };
	_itoa(nWlPos, str2, 10); // 将滑动值由int型变为char 
	GetDlgItem(IDC_EDIT2)->SetWindowText(str2);//显示在编辑框

	fnZXCX_XISL_WWWLMapping(pRawBuffer, pOutBuffer, 1024, 1024, nWwPos, nWlPos);

	cv::Mat tmpImg1(1024, 1024, CV_8UC1, pOutBuffer);

	rotate(tmpImg1, tmpImg1, cv::ROTATE_90_COUNTERCLOCKWISE);

	CRect rect;
	GetDlgItem(IDC_STATIC_ShowImg)->GetClientRect(&rect);  // 获取图片控件矩形框
	// 缩放Mat，以适应图片控件大小
	cv::resize(tmpImg1, tmpImg1, cv::Size(rect.Width(), rect.Height()));

	// 转换格式 ,便于获取BITMAPINFO
	switch (tmpImg1.channels())
	{
	case 1:
		cv::cvtColor(tmpImg1, tmpImg1, CV_GRAY2BGRA); // GRAY单通道
		break;
	case 3:
		cv::cvtColor(tmpImg1, tmpImg1, CV_BGR2BGRA);  // BGR三通道
		break;
	default:
		break;
	}

	int pixelBytes = tmpImg1.channels()*(tmpImg1.depth() + 1); // 计算一个像素多少个字节
	// 制作bitmapinfo(数据头)
	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
	bitInfo.bmiHeader.biWidth = tmpImg1.cols;
	bitInfo.bmiHeader.biHeight = -tmpImg1.rows;   //注意"-"号(正数时倒着绘制)
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0;

	CDC *pDC = GetDlgItem(IDC_STATIC_ShowImg)->GetDC();  //获取图片控件DC

	//绘图
	::StretchDIBits(
		pDC->GetSafeHdc(),
		0, 0, rect.Width(), rect.Height(),
		0, 0, tmpImg1.cols, tmpImg1.rows,
		tmpImg1.data,
		&bitInfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);
	ReleaseDC(pDC);  //释放DC

	UpdateData(FALSE);

}

//判读文件是否存在
//成功返回0
int isfileexist(char *filename)
{
	struct _stat buf;
	int result;

	result = _stat(filename, &buf);

	return result;
}

int createemptyfile(char *filename)
{
	FILE *f;
	f = fopen(filename, "wb");
	if (!f) return -1;
	fclose(f);
	return 0;
}

void CtestZXCXXISL89695Dlg::OnBnClickedButtonSavebmp()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime t = CTime::GetCurrentTime();
	int d = t.GetDay(); //获得几号
	int y = t.GetYear(); //获取年份
	int m = t.GetMonth(); //获取当前月份
	int h = t.GetHour(); //获取当前为几时
	int mm = t.GetMinute(); //获取分钟
	int s = t.GetSecond(); //获取秒
	int w = t.GetDayOfWeek(); //获取星期几，注意1为星期天，7为星期六

	CString dirName;
	CString fileName;

	dirName.Format("\\%d %d %d %d\\", y, m, d, h);
	fileName.Format("%d.%d.%d.PWVIA0S0M0N8R1E0Ozxcx.bmp", h, mm, s);

	//CString strCurPath;
	//GetCurrentDirectory(2048, strCurPath.GetBuffer());

	//CString tmpPath = strCurPath + dirName + fileName;

	////判断文件是否存在，如果不存在，则创建一个空的此文件
	//if (0 != isfileexist(tmpPath.GetBuffer()))
	//{
	//	if (0 != createemptyfile(tmpPath.GetBuffer()))
	//	{
	//		ZxcxLogERROR("create file fail!");
	//	}
	//}

	cv::Mat tmpImg(1024, 1024, CV_8UC1, pOutBuffer);

	cv::imwrite(fileName.GetBuffer(), tmpImg);
}
