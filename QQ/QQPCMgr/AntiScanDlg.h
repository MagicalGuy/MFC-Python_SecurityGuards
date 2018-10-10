#pragma once
#include "CMyList.h"
#include "afxwin.h"
#include <vector>
#include <TlHelp32.h>
#include <tchar.h>
#include "Md5.h"

// AntiScanDlg 对话框

class AntiScanDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AntiScanDlg)

public:
	AntiScanDlg(CString FilePath, DWORD dwType, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~AntiScanDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ANTISCAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnBnClickedButtonAntiPause();		  //暂停
	afx_msg void OnBnClickedButtonAntiCancle();		  //取消
	afx_msg void OnBnClickedButtonAntiOk();			  //OK
	DECLARE_MESSAGE_MAP()
public:
	CMyList m_ListAntiScan;
	CStatic m_TextScanInfo;

	DWORD 	m_dwScanType;		  //扫描类型
	bool	m_IsPause;			  //暂停
	bool	m_IsCancle;			  //取消
	CString m_FolderPath;		  //文件夹路径
	CString m_FilePath;			  //文件路径

	CString m_ScanFileFath;		  //扫描路径
	CString m_ScanMd5;			  //扫描MD5
	CString m_ScanType;			  //扫描类型

	std::vector<CString>  m_vecDisk;		   //驱动器盘符
	std::vector<CString>  m_vecFilePath;	   //文件路径

	std::vector<CString>  m_vecVirusDatabase;  //病毒库
	std::vector<CString>  m_vecBlack;		   //黑名单
	std::vector<CString>  m_vecWhite;		   //白名单

	const TCHAR* szFileName[3] = {			   //本地病毒库文件
		L"db\\VirusData.txt",
		L"db\\White.txt",
		L"db\\Black.txt" };

	void AntiScan();							//扫描分发
	void FastScan();							//快速扫描
	void ScanAll();								//扫描进程
	void ScanFile();							//文件
	void ScanFloder(CString FloderName);		//文件夹
												//void CloudScan();		//备用
	void ScanProcess();							//进程												
	void ReadDataBase();						//读取病毒库												
	bool DataBaseCMP(CString strMD5);			//对比MD5

	HANDLE hScanThread;
	static UINT  MyThreadScan(LPVOID lpParam);	//全盘扫描回调函数;
	virtual BOOL OnInitDialog();
};
