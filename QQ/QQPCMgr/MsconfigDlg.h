#pragma once
#include "CMyList.h"


// MsconfigDlg �Ի���

class MsconfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MsconfigDlg)

public:
	MsconfigDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MsconfigDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RUN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnNMRClickListRun(NMHDR *pNMHDR, LRESULT *pResult);//�����˵�
	afx_msg void OnMenuModifyData(UINT uID);						//��Ӧ�˵�
	DECLARE_MESSAGE_MAP()
public:
	CMyList m_ListRun;//�������б�

	CString	m_Name;		  //����
	CString m_Key;		  //��ֵ

	HKEY m_hRootKey1;	  //����1
	HKEY m_hRootKey2;	  //����2
	CString m_Root1;	  //����1
	CString m_Root2;	  //����2
	CString m_SubKey1;	  //�Ӽ�1
	CString m_SubKey2;	  //�Ӽ�2

	void DeleRun();		  //ɾ��������
	void UpDataRun();	  //ˢ��������
	void AddRun();		  //���������
	void GetRunInfo(HKEY hRootKey, CString Path);//��ȡ������

	virtual BOOL OnInitDialog();
};
