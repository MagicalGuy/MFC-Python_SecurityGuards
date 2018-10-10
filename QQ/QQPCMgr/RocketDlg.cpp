// RocketDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QQPCMgr.h"
#include "RocketDlg.h"
#include "afxdialogex.h"
#include"Psapi.h"


// RocketDlg 对话框

IMPLEMENT_DYNAMIC(RocketDlg, CDialogEx)

RocketDlg::RocketDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ROCKET, pParent)
{

}

RocketDlg::~RocketDlg()
{
}

void RocketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ROCKET, m_Rocket);
}


BEGIN_MESSAGE_MAP(RocketDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ROCKET, &RocketDlg::OnBnClickedRocket)
END_MESSAGE_MAP()


// RocketDlg 消息处理程序


BOOL RocketDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Rocket.LoadStdImage(IDB_PNG1, L"PNG");

	//灰色透明
	COLORREF maskColor = RGB(240, 240, 240);
	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE,
		GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE) | 0x80000);
	//设置透明
	SetLayeredWindowAttributes(maskColor, 255, 1);


	CRect cr;

	GetClientRect(&cr);//获取对话框客户区域大小

	ClientToScreen(&cr);//转换为荧幕坐标

	int x = GetSystemMetrics(SM_CXSCREEN);//获取荧幕坐标的宽度，单位为像素

	int y = GetSystemMetrics(SM_CYSCREEN);//获取荧幕坐标的高度，单位为像素

	MoveWindow((x - cr.Width() * 0.9), y - cr.Height()*0.9, cr.Width() * 2, cr.Height() * 2);//设置窗口显示的位置以及大小



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

UINT RocketDlg::MyThreadRocket(LPVOID lpParam)
{
	RocketDlg* pRocket = (RocketDlg*)lpParam;

	//获取屏幕尺寸
	int scrWidth, scrHeight;
	scrWidth = GetSystemMetrics(SM_CXSCREEN);
	scrHeight = GetSystemMetrics(SM_CYSCREEN);

	//计算坐标
	int nPosX = scrWidth - 200;
	int nPosY = scrHeight - 400;
	pRocket->MoveWindow(nPosX, nPosY, 100, 300);


	//循环移动
	while (true)
	{
		//间隔10
		Sleep(20);
		nPosY -= 10;
		pRocket->MoveWindow(nPosX, nPosY, 100, 300);

		//指定位置结束
		if (nPosY <= 30)
		{
			break;
		}
	}

	pRocket->EndDialog(0);


	return 0;
}


void RocketDlg::OnBnClickedRocket()
{
	// TODO: 在此添加控件通知处理程序代码
	//创建线程移动;
	hRocketThread = AfxBeginThread(MyThreadRocket, this);


	//获取当前内存状态
	MEMORYSTATUSEX stcMemStatusEX = { 0 };
	stcMemStatusEX.dwLength = sizeof(stcMemStatusEX);
	GlobalMemoryStatusEx(&stcMemStatusEX);
	DWORDLONG preUsedMem = stcMemStatusEX.ullTotalPhys - stcMemStatusEX.ullAvailPhys;

	//清理内存
	DWORD dwPIDList[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList, bufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); i++)
	{
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA, false, dwPIDList[i]);
		SetProcessWorkingSetSize(hProcess, -1, -1);
	}

	//获取清理后的内存状态
	GlobalMemoryStatusEx(&stcMemStatusEX);
	DWORDLONG afterCleanUsedMem = stcMemStatusEX.ullTotalPhys - stcMemStatusEX.ullAvailPhys;

	CString message;
	message.Format(L"已为您优化内存: %d MB", (preUsedMem - afterCleanUsedMem) / 1024 / 1024);


	MessageBox(message);


}

HBRUSH RocketDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	//绘制透明窗口
	if (nCtlColor == CTLCOLOR_DLG)
	{
		CBrush *brush;
		brush = new CBrush(RGB(240, 240, 240));
		return (HBRUSH)(brush->m_hObject);
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
