#pragma once
#include "CMyList.h"


// QQCleanDlg �Ի���

class QQCleanDlg : public CDialogEx
{
	DECLARE_DYNAMIC(QQCleanDlg)

public:
	QQCleanDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~QQCleanDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLEAN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	bool DeleFile(CString DirName, CString FileName);		//�������;
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnBnClickedButtonClean();//��ʼ����ť
	virtual BOOL OnInitDialog();
};
