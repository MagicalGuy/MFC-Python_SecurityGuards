#pragma once
#include "afxwin.h"
#include "CGdipButton.h"

// RocketDlg �Ի���

class RocketDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RocketDlg)

public:
	RocketDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~RocketDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ROCKET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	CGdipButton m_Rocket;

	HANDLE hRocketThread;
	static UINT  MyThreadRocket(LPVOID lpParam);	//�������ص�����;
	afx_msg void OnBnClickedRocket();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
