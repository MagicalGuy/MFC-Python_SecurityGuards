#pragma once
#include "CMyList.h"


// ServicesDlg �Ի���

class ServicesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ServicesDlg)

public:
	ServicesDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ServicesDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SERVICES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnNMRClickListService(NMHDR *pNMHDR, LRESULT *pResult);		//�Ҽ��˵�
	afx_msg void OnLvnColumnclickListService(NMHDR *pNMHDR, LRESULT *pResult);	//��ͷ����
	afx_msg void OnMenuService(UINT uID);   //������ֹͣ����
	DECLARE_MESSAGE_MAP()
public:
	CMyList m_ListServices;
	virtual BOOL OnInitDialog();

	CString m_ServiceName;					  //����
	CString m_ServiceDesc;					  //����
	CString m_ServiceStatu;					  //״̬
	CString m_ServiceType;					  //����

	void UpDataService();					  //ˢ�·���
	void GetServices();						  //��ȡ����
	void StartAndStopService(bool IsStart);	  //����/ֹͣ����

	const WCHAR* ServiceStatus[8] = {	//����״̬
		L" ",
		L"��ֹͣ",
		L"��������",
		L"����ֹͣ",
		L"��������",
		L"��������",
		L"��ͣ��",
		L"����ͣ",
	};

	const WCHAR* ServerStartType[6] = {	//������������
		L" ",
		L" ",
		L"�Զ�",
		L"�ֶ�",
		L"����",
		L" "
	};
};
