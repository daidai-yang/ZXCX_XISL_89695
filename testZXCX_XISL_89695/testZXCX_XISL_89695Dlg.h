
// testZXCX_XISL_89695Dlg.h: 头文件
//

#pragma once


// CtestZXCXXISL89695Dlg 对话框
class CtestZXCXXISL89695Dlg : public CDialog
{
// 构造
public:
	CtestZXCXXISL89695Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTZXCX_XISL_89695_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_szInfo;
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonStartimage();
	afx_msg void OnBnClickedButtonEndimage();
	afx_msg void OnBnClickedButtonDarkcorrection();
	afx_msg void OnBnClickedButtonBrightcorrection();
	afx_msg void OnBnClickedButtonTesthuidiao();
	afx_msg void OnBnClickedButtonHuidiao2();

public:
	void m_SendMessageDlg(int frameId, int width, int height, void* data);

	static CtestZXCXXISL89695Dlg* m_instance;
	
	int m_frameId;
	int m_width;
	int m_height;
	afx_msg void OnBnClickedButtonOpenimg();

private:
	void Showpicture(CString imgpath);
	int nWwPos;
	int nWlPos;
	unsigned short *pRawBuffer = NULL;
	unsigned char * pOutBuffer = NULL;
public:
	afx_msg void OnNMCustomdrawSlider_ww(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider_wl(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_ww_val;
	CSliderCtrl m_wl_val;


	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonSavebmp();
};
