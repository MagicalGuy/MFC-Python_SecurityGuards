
// QQPCMgrDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "./UI/IceDUI.h"
#include "CClientSocket.h"
#include <vector>

// CQQPCMgrDlg �Ի���
class CQQPCMgrDlg : public CDlgBase
{
	UINT	m_uTimerTest;
	UINT	m_uTimerTestWeb;


	int m_nWebSit;
	int m_nVirus;
	int m_nScore;
	int m_nProgress;

	// ����
public:
	
	CQQPCMgrDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_LINK_LOOK_DIALOG };

	//CClientSocket m_client;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()

	virtual void InitUI(CRect rcClient);
	virtual void OnSize(CRect rcClient);

	// ��Ϣ��Ӧ
	virtual LRESULT OnMessage(UINT	uID, UINT Msg, WPARAM wParam, LPARAM lParam);

	// ����
	afx_msg LRESULT OnMessageSkin(WPARAM wParam, LPARAM lParam);
	// ���˵�
	afx_msg LRESULT OnMessageMainMenu(WPARAM wParam, LPARAM lParam);

	// ɨ��
	void StartCan(int nType);

	void setFalse();
		
public:

	// ��ʱ����Ϣ
	virtual void OnTimer(UINT uTimerID);

	CClientSocket m_Socket;			//Socket�ͻ���
	CString		  m_MD5;			//MD5

	std::vector<CString>  m_vecMD5;	//����MD5����

	void UpdataDataBase(DWORD dwType);	//��ȡ�����ݲ�����������

	const TCHAR* szFileName[4] = {		//�������ݿ��ļ�����
		L" ",
		L"db\\VirusData.txt",
		L"db\\White.txt",
		L"db\\Black.txt" };

	static CControlBase * pControlBase1;

	void GetSysInfo();	//��ȡϵͳ�汾��Ϣ

	void ReadDataBase(DWORD dwType);

private:

	HANDLE hUpdataThread;							//�߳̾��
	static UINT  MyThreadUpdata(LPVOID lpParam);	//�������ݿ�ص�����;


	//HANDLE PEToolThread;
	//HANDLE TsmThread;

	//static UINT startPETool(LPVOID lpParam);
	//static UINT startTsm(LPVOID lpParam);

	//bool startProcess(const string name_);

	void protectProcess();

	HANDLE scanTsm;

	static UINT scanProcess(LPVOID lpParam);

	//����Ȩ��
	bool getSeDebugPrivilge();

};

