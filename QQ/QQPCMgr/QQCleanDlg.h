#pragma once
#include "CMyList.h"


// QQCleanDlg 对话框

class QQCleanDlg : public CDialogEx
{
	DECLARE_DYNAMIC(QQCleanDlg)

public:
	QQCleanDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~QQCleanDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLEAN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	BOOL m_CbSystem;
	BOOL m_CbRecycle;
	BOOL m_CbIE;
	BOOL m_CbAll;
	CMyList m_ListClean;

	CString m_Index;
	CString m_DeleFileName;
	CString m_Result;

	DWORD m_dwIndex;

	CString m_PathA;
	CString m_PathB;

	CString m_PathIE;
	void CleanRecycle();
	bool DeleFile(CString DirName, CString FileName);		//清理程序;
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnBnClickedButtonClean();//开始清理按钮
	virtual BOOL OnInitDialog();
};
