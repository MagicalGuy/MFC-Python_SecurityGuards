
// QQPCMgrDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "./UI/IceDUI.h"
#include "CClientSocket.h"
#include <vector>

// CQQPCMgrDlg 对话框
class CQQPCMgrDlg : public CDlgBase
{
	UINT	m_uTimerTest;
	UINT	m_uTimerTestWeb;


	int m_nWebSit;
	int m_nVirus;
	int m_nScore;
	int m_nProgress;

	// 构造
public:
	
	CQQPCMgrDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_LINK_LOOK_DIALOG };

	//CClientSocket m_client;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()

	virtual void InitUI(CRect rcClient);
	virtual void OnSize(CRect rcClient);

	// 消息响应
	virtual LRESULT OnMessage(UINT	uID, UINT Msg, WPARAM wParam, LPARAM lParam);

	// 换肤
	afx_msg LRESULT OnMessageSkin(WPARAM wParam, LPARAM lParam);
	// 主菜单
	afx_msg LRESULT OnMessageMainMenu(WPARAM wParam, LPARAM lParam);

	// 扫描
	void StartCan(int nType);

	void setFalse();
		
public:

	// 定时器消息
	virtual void OnTimer(UINT uTimerID);

	CClientSocket m_Socket;			//Socket客户端
	CString		  m_MD5;			//MD5

	std::vector<CString>  m_vecMD5;	//数据MD5缓存

	void UpdataDataBase(DWORD dwType);	//读取云数据并保存至本地

	const TCHAR* szFileName[4] = {		//本地数据库文件名称
		L" ",
		L"db\\VirusData.txt",
		L"db\\White.txt",
		L"db\\Black.txt" };

	static CControlBase * pControlBase1;

	void GetSysInfo();	//获取系统版本信息

	void ReadDataBase(DWORD dwType);

private:

	HANDLE hUpdataThread;							//线程句柄
	static UINT  MyThreadUpdata(LPVOID lpParam);	//更新数据库回调函数;


	//HANDLE PEToolThread;
	//HANDLE TsmThread;

	//static UINT startPETool(LPVOID lpParam);
	//static UINT startTsm(LPVOID lpParam);

	//bool startProcess(const string name_);

	void protectProcess();

	HANDLE scanTsm;

	static UINT scanProcess(LPVOID lpParam);

	//增加权限
	bool getSeDebugPrivilge();

};

