#pragma once
#include "CMyList.h"
#include <vector>

// SoftWaresDlg �Ի���
//�Զ��������Ϣ�ṹ��
typedef struct _SOFTINFO
{
	WCHAR szSoftName[MAX_PATH];//�������
	WCHAR szSoftPub[100];//����汾��
	WCHAR szSoftDate[20];//��װ����
	WCHAR szSoftSize[20];//�����С
	WCHAR szSoftVer[30];//��������
	WCHAR szSoftInsPath[MAX_PATH];//��װ·��
	WCHAR szSoftUniPath[MAX_PATH];//ж��·��

}SOFTINFO, *PSOFTINFO;


class SoftWaresDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SoftWaresDlg)

public:
	SoftWaresDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SoftWaresDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SOFTWARE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnMenuSoftMG(UINT uID);										//��Ӧ�˵�
	afx_msg void OnNMRClickListSoftware(NMHDR *pNMHDR, LRESULT *pResult);		//�����˵�
	afx_msg void OnLvnColumnclickListSoftware(NMHDR *pNMHDR, LRESULT *pResult);	//�б���ͷ����
	DECLARE_MESSAGE_MAP()
public:
	CMyList m_ListSoftware;

	HKEY m_hRootKey;					  //����
	CString m_SubKey1;					  //�Ӽ�1
	CString m_SubKey2;					  //�Ӽ�2

	CString m_KeyName;					  //����
	CString m_KeyNameVer;				  //�汾
	CString m_KeyNameDate;				  //����
	CString m_KeyNameSize;				  //��С
	CString m_KeyNamePub;				  //����
	CString m_KeyNameInsPath;			  //��װ·��
	CString m_KeyNameUniPath;			  //ж��·��

	CString m_SoftName;					  //��ʾ����
	CString m_SoftPub;					  //����
	CString m_SoftDate;					  //����
	CString m_SoftSize;					  //�ߴ�
	CString m_SoftVer;					  //�汾
	CString m_SoftInsPath;				  //��װ·��
	CString m_SoftUniPath;				  //ж��·��

	std::vector<SOFTINFO> m_vecSoftInfo;	//�洢��Ϣ
	void GetSoftwareInfo(CString SubKey);	//��ȡ�����Ϣ
	void UpdateSoftInfo();					//ˢ��
	void OpenExploer();						//���ļ���
	void UnistallSoft();					//ж�����
	virtual BOOL OnInitDialog();
};
