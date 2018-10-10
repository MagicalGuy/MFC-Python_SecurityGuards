#pragma once
#include "afxwin.h"
#include "CGdipButton.h"

// RocketDlg 对话框

class RocketDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RocketDlg)

public:
	RocketDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RocketDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ROCKET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	CGdipButton m_Rocket;

	HANDLE hRocketThread;
	static UINT  MyThreadRocket(LPVOID lpParam);	//火箭发射回调函数;
	afx_msg void OnBnClickedRocket();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
