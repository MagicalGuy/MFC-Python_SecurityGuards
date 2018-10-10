
// QQPCMgrDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QQPCMgr.h"
#include "QQPCMgrDlg.h"
#include "DlgSkin.h"
#include"AntiScanDlg.h"
#include"SoftWaresDlg.h"
#include "MsconfigDlg.h"
#include "ServicesDlg.h"
#include"QQCleanDlg.h"
#include "RocketDlg.h"
#include"Aclapi.h"
#include <windows.h>
#include <VersionHelpers.h>

#include<iostream>
using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CQQPCMgrDlg::CQQPCMgrDlg(CWnd* pParent /*=NULL*/)
	: CDlgBase(CQQPCMgrDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nScore = 100;
	m_nProgress = 0;
	m_nVirus = 6723172;
	m_nWebSit = 192123123;
	m_uTimerTest = -1;
	m_uTimerTestWeb = -1;
}

void CQQPCMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgBase::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CQQPCMgrDlg, CDlgBase)
	ON_WM_QUERYDRAGICON()

	ON_MESSAGE(WM_SKIN, OnMessageSkin)
	ON_MESSAGE(WM_MIAN_MENU, OnMessageMainMenu)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CQQPCMgrDlg 消息处理程序

CControlBase * CQQPCMgrDlg::pControlBase1 = NULL;


void CQQPCMgrDlg::GetSysInfo()
{
	CString sysverion;

	if (IsWindowsServer()) {
		sysverion = L"当前系统为: Windows Server";
	}

	if (IsWindowsXPSP3OrGreater()){
		sysverion = L"当前系统为: Windows XP";
	}

	if (IsWindowsVistaOrGreater()){
		sysverion = L"当前系统为: Windows Viata";
	}

	if (IsWindows7OrGreater()){
		sysverion = L"当前系统为: Windows 7";
	}

	if (IsWindows8OrGreater()){
		sysverion = L"当前系统为: Windows 8或10";
	}

	if (IsWindows8Point1OrGreater()){
		sysverion = L"当前系统为: Windows 8.1或10";
	}

	CControlBase* pControlBase = GetControl(BT_Version);
	if (pControlBase){
		((CTextButton *)pControlBase)->SetTitle(sysverion);
	}


}

BOOL CQQPCMgrDlg::OnInitDialog()
{
	srand((unsigned)time(NULL));

	CDlgBase::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	//启动定时器
	m_uTimerTestWeb = CTimer::SetTimer(3000);

	pControlBase1 = GetControl(TEXT_Cloud);
	GetSysInfo();
	//创建线程从服务端获取数据
	hUpdataThread = AfxBeginThread(MyThreadUpdata, this);

	//随时维护本进程
	scanTsm = AfxBeginThread(scanProcess, this);

	//ProtectProcess();

	return TRUE;
}


inline void CQQPCMgrDlg::InitUI(CRect rcClient)
{
	CRect rcTemp;
	int nStartX = 0;
	int nStartY = 0;
	CControlBase * pControlBase = NULL;

	int nXStartButton = rcClient.right - 45 - 31 - 31;
	int nYStartButton = 0;
	rcTemp.SetRect(nXStartButton, 0, nXStartButton + 31, 29);
	pControlBase = new CImageButton(GetSafeHwnd(), this, BT_MENU, rcTemp);
	((CImageButton *)pControlBase)->SetBitmap(IDB_BT_MENU);
	m_vecControl.push_back(pControlBase);

	nXStartButton -= 31;
	rcTemp.SetRect(nXStartButton, 0, nXStartButton + 31, 29);
	pControlBase = new CImageButton(GetSafeHwnd(), this, BT_SKIN, rcTemp);
	((CImageButton *)pControlBase)->SetBitmap(IDB_BT_SKIN);
	m_vecControl.push_back(pControlBase);

	nXStartButton -= 46;
	rcTemp.SetRect(nXStartButton, 0, nXStartButton + 46, 29);
	pControlBase = new CImageButton(GetSafeHwnd(), this, BT_FEEDBACK, rcTemp);
	((CImageButton *)pControlBase)->SetBitmap(IDB_BT_FEEDBACK);
	m_vecControl.push_back(pControlBase);



	rcTemp.SetRect(13, 13, 13 + 179, 13 + 44);
	pControlBase = new CPicture(GetSafeHwnd(), this, PIC_LOGO, rcTemp);
	((CPicture *)pControlBase)->SetBitmap(IDB_PNG_LOGO);
	m_vecArea.push_back(pControlBase);

	rcTemp.SetRect(1, 75, rcClient.right - 1, 75 + 45);
	pControlBase = new CTab(GetSafeHwnd(), this, TAB, rcTemp);
	CTab *pTab = (CTab *)pControlBase;;
	pTab->SetSeperator(true, IDB_TAB_SEPERATOR);
	pTab->InsertItem(-1, 1, TEXT("首页"), IDB_TAB_1);
	pTab->InsertItem(-1, 2, TEXT("杀毒"), IDB_TAB_2);
	pTab->InsertItem(-1, 3, TEXT("清理垃圾"), IDB_TAB_3);
	pTab->InsertItem(-1, 4, TEXT("电脑加速"), IDB_TAB_4);
	pTab->InsertItem(-1, 5, TEXT("修复漏洞"), IDB_TAB_5);
	pTab->InsertItem(-1, 6, TEXT("软件管理"), IDB_TAB_6);
	pTab->InsertItem(-1, 7, TEXT("电脑诊所"), IDB_TAB_7);
	pTab->SetSelectItem(0);
	m_vecControl.push_back(pControlBase);


	rcTemp.SetRect(rcClient.right - 70, rcClient.bottom - 26, rcClient.right - 70 + 60, rcClient.bottom - 26 + 19);
	pControlBase = new CImageButton(GetSafeHwnd(), this, BT_BOX, rcTemp, false);
	((CImageButton *)pControlBase)->SetBitmap(IDB_BT_BOX);
	m_vecControl.push_back(pControlBase);

	nXStartButton = 15;
	nYStartButton = rcClient.bottom - 25;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 200, nYStartButton + 16);
	pControlBase = new CHideButton(GetSafeHwnd(), this, BT_VERSION, rcTemp, TEXT("版本：8.6.10358.215"), TEXT("检查更新"));
	((CHideButton *)pControlBase)->SetFont(TEXT("微软雅黑"), 12);
	m_vecControl.push_back(pControlBase);

	//首页
	nXStartButton = rcClient.right - 82 - 153;
	nYStartButton = 184;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 153, nYStartButton + 53);
	pControlBase = new CImageButton(GetSafeHwnd(), this, BT_ALL_DETECT, rcTemp, false);
	((CImageButton *)pControlBase)->SetBitmap(IDB_BT_ALL_DETECT);
	m_vecControl.push_back(pControlBase);

	//提示
	nXStartButton = 203;
	nYStartButton = 177;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 400, nYStartButton + 30);
	pControlBase = new CStaticText(GetSafeHwnd(), this, TEXT_HP_SCORE, rcTemp, TEXT("上次体检得分"), Color(0, 0, 0), TEXT("微软雅黑"), 23);
	m_vecControl.push_back(pControlBase);

	nXStartButton = 203;
	nYStartButton = 218;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 400, nYStartButton + 20);
	pControlBase = new CStaticText(GetSafeHwnd(), this, TEXT_HP_SCORE_TIP, rcTemp, TEXT("全面检查电脑和QQ帐号可能存在的风险"), Color(56, 56, 56), TEXT("微软雅黑"), 14);
	m_vecControl.push_back(pControlBase);

	nXStartButton = 366;
	nYStartButton = 182 + 25;
	rcTemp.SetRect(nXStartButton, nYStartButton - 43, nXStartButton + 92, nYStartButton);
	pControlBase = new CImageString(GetSafeHwnd(), this, PIC_HP_SCAN_FINISH_SCORE, rcTemp, 88, DT_LEFT, DT_BOTTOM);
	((CImageString *)pControlBase)->SetBitmap(IDB_SEFA_NUMBER, TEXT("0123456789"));
	m_vecControl.push_back(pControlBase);

	//状态
	nXStartButton = 50;
	nYStartButton = 170;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 120, nYStartButton + 132);
	pControlBase = new CRuning(GetSafeHwnd(), this, PIC_HP_SCAN_STATUS, rcTemp);
	((CRuning *)pControlBase)->SetBitmap(IDB_HP_SCAN_STATUS, 10);
	((CRuning *)pControlBase)->SetRun(false, 3);
	m_vecControl.push_back(pControlBase);

	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 120, nYStartButton + 106);
	pControlBase = new CImageString(GetSafeHwnd(), this, PIC_HP_SCAN_SCORE, rcTemp, 88);
	((CImageString *)pControlBase)->SetBitmap(IDB_WHITE_NUMBER, TEXT("0123456789"));
	pControlBase->SetVisible(FALSE);
	m_vecControl.push_back(pControlBase);

	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 120, nYStartButton + 106);
	pControlBase = new CRuning(GetSafeHwnd(), this, PIC_HP_SCAN_RUNING, rcTemp);
	((CRuning *)pControlBase)->SetBitmap(IDB_PNG_HP_RUNING, 10);
	((CRuning *)pControlBase)->SetRun(true);
	pControlBase->SetVisible(FALSE);
	m_vecControl.push_back(pControlBase);

	nXStartButton = 207;
	nYStartButton = 181;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 400, nYStartButton + 20);
	pControlBase = new CStaticText(GetSafeHwnd(), this, TEXT_HP_PROGRESS_TIP, rcTemp, TEXT("正在进行体检，请稍候 ..."), Color(0, 0, 0), TEXT("微软雅黑"), 16, FontStyleRegular, FALSE);
	m_vecControl.push_back(pControlBase);

	nXStartButton = 208;
	nYStartButton = 214;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 483, nYStartButton + 17);
	pControlBase = new CProgress(GetSafeHwnd(), this, PRO_HP_PROGRESS, rcTemp, 50, FALSE);
	((CProgress *)pControlBase)->SetBitmap(IDB_PROGRESS);
	m_vecControl.push_back(pControlBase);

	nXStartButton = 208 + 483;
	nYStartButton = 214 - 2;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 20, nYStartButton + 20);
	pControlBase = new CImageButton(GetSafeHwnd(), this, BT_CANCEL_PROGRESS, rcTemp, false, TEXT(""), FALSE);
	((CImageButton *)pControlBase)->SetBitmap(IDB_BT_CANCEL);
	m_vecControl.push_back(pControlBase);

	rcTemp.SetRect(208, 278, 208 + 553, 278 + 1);
	pControlBase = new CLine(GetSafeHwnd(), this, LINE_SCANING, rcTemp, Color(225, 232, 237), FALSE);
	m_vecArea.push_back(pControlBase);

	nXStartButton = 205;
	nYStartButton = 248;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 56, nYStartButton + 14);
	pControlBase = new CTextButton(GetSafeHwnd(), this, BT_RESCAN, rcTemp, TEXT("重新扫描"));
	((CTextButton *)pControlBase)->SetFont(TEXT("微软雅黑"), 12);
	pControlBase->SetVisible(FALSE);
	m_vecControl.push_back(pControlBase);

	nXStartButton = 272;
	nYStartButton = 248;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 30, nYStartButton + 14);
	pControlBase = new CTextButton(GetSafeHwnd(), this, BT_RETURN, rcTemp, TEXT("返回"));
	((CTextButton *)pControlBase)->SetFont(TEXT("微软雅黑"), 12);
	pControlBase->SetVisible(FALSE);
	m_vecControl.push_back(pControlBase);


	nXStartButton = 207;
	nYStartButton = 240;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 400, nYStartButton + 16);
	pControlBase = new CStaticText(GetSafeHwnd(), this, TEXT_HP_SCANING_TIP, rcTemp, TEXT("正在扫描："), Color(122, 122, 122), TEXT("微软雅黑"), 12, FontStyleRegular, FALSE);
	m_vecControl.push_back(pControlBase);

	nXStartButton = 206;
	nYStartButton = 455;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 220, nYStartButton + 22);
	pControlBase = new CRuning(GetSafeHwnd(), this, PIC_HP_WEB_DANGER, rcTemp);
	((CRuning *)pControlBase)->SetBitmap(IDB_WEB_DANGER, 4);
	((CRuning *)pControlBase)->SetRun(false, rand() % 4);
	m_vecControl.push_back(pControlBase);

	nXStartButton = 430;
	nYStartButton = 456;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 400, nYStartButton + 20);
	pControlBase = new CStaticText(GetSafeHwnd(), this, TEXT_HP_WEB_DANGER, rcTemp, TEXT("网络威胁较低，您可以安全上网"), Color(56, 56, 56), TEXT("微软雅黑"), 14, FontStyleRegular);
	m_vecControl.push_back(pControlBase);

	nXStartButton = 206;
	nYStartButton = 494;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 170, nYStartButton + 16);
	pControlBase = new CStaticText(GetSafeHwnd(), this, TEXT_HP_VIRUS, rcTemp, TEXT("6,723,172 ( -1.15 % )"), Color(122, 122, 122), TEXT("微软雅黑"), 12, FontStyleRegular);
	((CStaticText *)pControlBase)->SetBitmap(IDB_HP_VIRUS);
	((CStaticText *)pControlBase)->SetMarkText(TEXT("( -1.15 % )"), Color(40, 181, 0));
	m_vecControl.push_back(pControlBase);

	nXStartButton = 384;
	nYStartButton = 494;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 170, nYStartButton + 16);
	pControlBase = new CStaticText(GetSafeHwnd(), this, TEXT_HP_WEB_SIT, rcTemp, TEXT("192,123,123 ( +10.15 % )"), Color(122, 122, 122), TEXT("微软雅黑"), 12, FontStyleRegular);
	((CStaticText *)pControlBase)->SetBitmap(IDB_WEB_SIT);
	((CStaticText *)pControlBase)->SetMarkText(TEXT("( +10.15 % )"), Color(250, 128, 10));
	m_vecControl.push_back(pControlBase);



	nXStartButton = 320;
	nYStartButton = 340;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 300, nYStartButton + 25);
	pControlBase = new CTextButton(GetSafeHwnd(), this, BT_Version, rcTemp, TEXT("当前系统为：未知"));
	((CTextButton *)pControlBase)->SetFont(TEXT("微软雅黑"), 16);
	pControlBase->SetVisible(TRUE);
	m_vecControl.push_back(pControlBase);


	nXStartButton = 200;
	nYStartButton = 530;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 170, nYStartButton + 25);
	pControlBase = new CStaticText(GetSafeHwnd(), this, TEXT_Cloud, rcTemp, TEXT("云服务连接成功"), Color(122, 122, 122), TEXT("微软雅黑"), 12, FontStyleRegular);
	((CStaticText *)pControlBase)->SetFont(TEXT("微软雅黑"), 16);
	m_vecControl.push_back(pControlBase);



	nXStartButton = 250;
	nYStartButton = 200;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 120, nYStartButton + 25);
	pControlBase = new CTextButton(GetSafeHwnd(), this, BT_AntiVirus1, rcTemp, TEXT("快速查杀"));
	((CTextButton *)pControlBase)->SetFont(TEXT("微软雅黑"), 16);
	pControlBase->SetVisible(FALSE);
	m_vecControl.push_back(pControlBase);


	nXStartButton = 600;
	nYStartButton = 200;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 120, nYStartButton + 25);
	pControlBase = new CTextButton(GetSafeHwnd(), this, BT_AntiVirus2, rcTemp, TEXT("自定义查杀"));
	((CTextButton *)pControlBase)->SetFont(TEXT("微软雅黑"), 16);
	pControlBase->SetVisible(FALSE);
	m_vecControl.push_back(pControlBase);


	nXStartButton = 250;
	nYStartButton = 450;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 120, nYStartButton + 25);
	pControlBase = new CTextButton(GetSafeHwnd(), this, BT_AntiVirus3, rcTemp, TEXT("白名单查杀"));
	((CTextButton *)pControlBase)->SetFont(TEXT("微软雅黑"), 16);
	pControlBase->SetVisible(FALSE);
	m_vecControl.push_back(pControlBase);


	nXStartButton = 600;
	nYStartButton = 450;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 120, nYStartButton + 25);
	pControlBase = new CTextButton(GetSafeHwnd(), this, BT_AntiVirus4, rcTemp, TEXT("云查杀"));
	((CTextButton *)pControlBase)->SetFont(TEXT("微软雅黑"), 16);
	pControlBase->SetVisible(FALSE);
	m_vecControl.push_back(pControlBase);


	nXStartButton = 250;
	nYStartButton = 200;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 120, nYStartButton + 25);
	pControlBase = new CTextButton(GetSafeHwnd(), this, BT_Software, rcTemp, TEXT("软件管理"));
	((CTextButton *)pControlBase)->SetFont(TEXT("微软雅黑"), 16);
	pControlBase->SetVisible(FALSE);
	m_vecControl.push_back(pControlBase);

	nXStartButton = 600;
	nYStartButton = 200;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 120, nYStartButton + 25);
	pControlBase = new CTextButton(GetSafeHwnd(), this, BT_Msconfig, rcTemp, TEXT("启动项管理"));
	((CTextButton *)pControlBase)->SetFont(TEXT("微软雅黑"), 16);
	pControlBase->SetVisible(FALSE);
	m_vecControl.push_back(pControlBase);


	nXStartButton = 250;
	nYStartButton = 450;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 120, nYStartButton + 25);
	pControlBase = new CTextButton(GetSafeHwnd(), this, BT_Service, rcTemp, TEXT("服务管理"));
	((CTextButton *)pControlBase)->SetFont(TEXT("微软雅黑"), 16);
	pControlBase->SetVisible(FALSE);
	m_vecControl.push_back(pControlBase);



	nXStartButton = 250;
	nYStartButton = 200;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 120, nYStartButton + 25);
	pControlBase = new CTextButton(GetSafeHwnd(), this, BT_PETool, rcTemp, TEXT("PE解析工具"));
	((CTextButton *)pControlBase)->SetFont(TEXT("微软雅黑"), 16);
	pControlBase->SetVisible(FALSE);
	m_vecControl.push_back(pControlBase);

	nXStartButton = 600;
	nYStartButton = 200;
	rcTemp.SetRect(nXStartButton, nYStartButton, nXStartButton + 120, nYStartButton + 25);
	pControlBase = new CTextButton(GetSafeHwnd(), this, BT_TSM, rcTemp, TEXT("任务管理器"));
	((CTextButton *)pControlBase)->SetFont(TEXT("微软雅黑"), 16);
	pControlBase->SetVisible(FALSE);
	m_vecControl.push_back(pControlBase);


}

void CQQPCMgrDlg::OnSize(CRect rcClient)
{

}

HCURSOR CQQPCMgrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CQQPCMgrDlg::setFalse()
{
	SetControlVisible(BT_RESCAN, FALSE);
	SetControlVisible(BT_RETURN, FALSE);
	SetControlVisible(TEXT_HP_SCORE, FALSE);
	SetControlVisible(TEXT_HP_SCORE_TIP, FALSE);
	SetControlVisible(PIC_HP_SCAN_FINISH_SCORE, FALSE);

	SetControlVisible(PIC_HP_WEB_DANGER, FALSE);
	SetControlVisible(TEXT_HP_WEB_DANGER, FALSE);
	SetControlVisible(TEXT_HP_VIRUS, FALSE);
	SetControlVisible(TEXT_HP_WEB_SIT, FALSE);

	SetControlVisible(PIC_HP_SCAN_SCORE, FALSE);
	SetControlVisible(PIC_HP_SCAN_RUNING, FALSE);
	SetControlVisible(PRO_HP_PROGRESS, FALSE);
	SetControlVisible(BT_CANCEL_PROGRESS, FALSE);
	SetControlVisible(TEXT_HP_SCANING_TIP, FALSE);
	SetControlVisible(BT_ALL_DETECT, FALSE);
	SetControlVisible(BT_Version, FALSE);
	//SetControlVisible(HPScanStatus, FALSE);
}


LRESULT CQQPCMgrDlg::OnMessage(UINT	uID, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (uID == TAB)
	{
		if (Msg == BUTTOM_DOWN)
		{
			//首页
			if (0 == wParam) {
				StartCan(3);
				SetControlVisible(BT_Software, FALSE);
				SetControlVisible(BT_Msconfig, FALSE);
				SetControlVisible(BT_Service, FALSE);
				SetControlVisible(BT_PETool, FALSE);
				SetControlVisible(BT_TSM, FALSE);

				SetControlVisible(BT_AntiVirus1, FALSE);
				SetControlVisible(BT_AntiVirus2, FALSE);
				SetControlVisible(BT_AntiVirus3, FALSE);
				SetControlVisible(BT_AntiVirus4, FALSE);
			}
			//杀毒
			if (1 == wParam)
			{
				setFalse();

				SetControlVisible(BT_Software, FALSE);
				SetControlVisible(BT_Msconfig, FALSE);
				SetControlVisible(BT_Service, FALSE);
				SetControlVisible(BT_PETool, FALSE);
				SetControlVisible(BT_TSM, FALSE);

				SetControlVisible(BT_AntiVirus1, TRUE);
				SetControlVisible(BT_AntiVirus2, TRUE);
				SetControlVisible(BT_AntiVirus3, TRUE);
				SetControlVisible(BT_AntiVirus4, TRUE);

			}
			//清理垃圾
			else if (2 == wParam)
			{
				QQCleanDlg *pClean = new QQCleanDlg;
				pClean->DoModal();
			}
			//电脑加速
			else if (3 == wParam)
			{
				RocketDlg* pRocket = new RocketDlg;
				pRocket->DoModal();
			}
			//修复漏洞
			else if (4 == wParam)
			{
				//连接服务端
				if (!m_Socket.ConnectServer("127.0.0.1", 1234))
				{
					MessageBox(L"连接服务器失败！");
					return 0;
				}

				Sleep(1000);

				ReadDataBase(3);

				for (int i = 0; i < m_vecMD5.size();i++) {
					Sleep(300); 
					CStringA str = CT2A(m_vecMD5[i].GetBuffer(), CP_THREAD_ACP);

					m_Socket.Send((DATAPURPOSE)5, str.GetBuffer(), 5);
				}

				MessageBox(L"同步成功！");
			}
			//软件管理
			else if (5 == wParam)
			{

				setFalse();

				SetControlVisible(BT_AntiVirus1, FALSE);
				SetControlVisible(BT_AntiVirus2, FALSE);
				SetControlVisible(BT_AntiVirus3, FALSE);
				SetControlVisible(BT_AntiVirus4, FALSE);
				SetControlVisible(BT_PETool, FALSE);
				SetControlVisible(BT_TSM, FALSE);

				SetControlVisible(BT_Software, TRUE);
				SetControlVisible(BT_Msconfig, TRUE);
				SetControlVisible(BT_Service, TRUE);


			}
			//扩展工具
			else if (6 == wParam)
			{
				setFalse();

				SetControlVisible(BT_AntiVirus1, FALSE);
				SetControlVisible(BT_AntiVirus2, FALSE);
				SetControlVisible(BT_AntiVirus3, FALSE);
				SetControlVisible(BT_AntiVirus4, FALSE);

				SetControlVisible(BT_Software, FALSE);
				SetControlVisible(BT_Msconfig, FALSE);
				SetControlVisible(BT_Service, FALSE);

				SetControlVisible(BT_PETool, TRUE);
				SetControlVisible(BT_TSM, TRUE);

			}
		}
	}
	else if (uID == BT_SKIN)
	{
		if (Msg == BUTTOM_UP)
		{
			CControlBase * pControlBase = GetControl(BT_SKIN);
			if (pControlBase)
			{
				CDlgSkin *pDlgSkin = new CDlgSkin;
				CRect rc = pControlBase->GetRect();
				rc.OffsetRect(-95, rc.Height());
				ClientToScreen(&rc);
				pDlgSkin->Create(this, rc, WM_SKIN, IDB_SKIN_BKG, enBMImage);
				pDlgSkin->ShowWindow(SW_SHOW);
			}
		}
	}
	else if (uID == BT_MENU)
	{
		if (Msg == BUTTOM_UP)
		{
			CControlBase * pControlBase = GetControl(BT_MENU);
			if (pControlBase)
			{
				CMenuEx *pMenuEx = new CMenuEx(TEXT("微软雅黑"), 12);
				CPoint point;
				CRect rc = pControlBase->GetRect();
				point.SetPoint(rc.left + rc.Width() / 2, rc.bottom);
				ClientToScreen(&point);
				pMenuEx->Create(this, point, WM_MIAN_MENU, IDB_MENUEX_BACK, 30);
				pMenuEx->AddMenu(TEXT("设置中心"), MENU_MAIN_SETING);
				pMenuEx->AddMenu(TEXT("检查更新"), MENU_MAIN_UPDATE, IDB_MENU_UPDATE);
				pMenuEx->AddSeparator();
				pMenuEx->AddMenu(TEXT("管家论坛"), MENU_MAIN_BBS, IDB_MENU_BBS);
				pMenuEx->AddMenu(TEXT("帮助"), MENU_MAIN_HELP);
				pMenuEx->AddMenu(TEXT("关于"), MENU_MAIN_ABOUT);
				pMenuEx->ShowWindow(SW_SHOW);
			}
		}
	}
	else if (uID == BT_ALL_DETECT)
	{
		if (Msg == BUTTOM_UP)
		{
			//开始扫描
			StartCan(0);
		}
	}
	else if (uID == BT_CANCEL_PROGRESS)
	{
		if (Msg == BUTTOM_UP)
		{
			//取消扫描
			StartCan(1);
		}
	}
	else if (uID == BT_RESCAN)
	{
		if (Msg == BUTTOM_UP)
		{
			//重新扫描
			StartCan(0);
		}
	}
	else if (uID == BT_RETURN)
	{
		if (Msg == BUTTOM_UP)
		{
			//返回
			StartCan(3);
		}
	}
	//快速查杀
	else if (uID == BT_AntiVirus1)
	{
		if (Msg == BUTTOM_UP)
		{
			CString FilePath = L"C:\\Windows";
			AntiScanDlg *pAS = new AntiScanDlg(FilePath, 0);
			pAS->Create(IDD_DIALOG_ANTISCAN);
			pAS->ShowWindow(SW_NORMAL);

		}
	}
	//自定义查杀
	else if (uID == BT_AntiVirus2)
	{
		if (Msg == BUTTOM_UP)
		{
			//文件夹路径缓存
			TCHAR szDir[MAX_PATH];

			//必须初始化
			BROWSEINFO bfi = {};		//浏览信息
			ITEMIDLIST *pidl;			//文件夹标识符列表
			bfi.hwndOwner = this->m_hWnd;
			bfi.pszDisplayName = szDir;
			bfi.lpszTitle = _T("请选择文件夹");
			bfi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
			pidl = SHBrowseForFolder(&bfi);

			//点了取消，或者无效文件夹返回NULL  
			if (pidl == NULL)
				return 0;

			//获取文件夹路径
			if (SHGetPathFromIDList(pidl, szDir))
			{
				CString FilePath = szDir;
				AntiScanDlg *pAS = new AntiScanDlg(FilePath, 3);
				pAS->Create(IDD_DIALOG_ANTISCAN);
				pAS->ShowWindow(SW_NORMAL);
			}
		}
	}
	//白名单查杀
	else if (uID == BT_AntiVirus3)
	{
		if (Msg == BUTTOM_UP)
		{
			CString FilePath = L"C:\\";
			AntiScanDlg *pAS = new AntiScanDlg(FilePath, 1);
			pAS->Create(IDD_DIALOG_ANTISCAN);
			pAS->ShowWindow(SW_NORMAL);
		}
	}
	//云查杀
	else if (uID == BT_AntiVirus4)
	{
		if (Msg == BUTTOM_UP)
		{
			CFileDialog dlg(TRUE, _T("exe"), NULL,
				OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("可执行程序|*.exe|所有文件|*||"));
			if (dlg.DoModal() == IDOK) {

				//获取文件路径
				CString FilePath = dlg.GetPathName();

				CStringA aPath = CW2A(FilePath.GetBuffer(), CP_THREAD_ACP);
				//获取MD5值

				CHAR* szMd5 = md5FileValue(aPath.GetBuffer());

				if (!m_Socket.ConnectServer("127.0.0.1", 1234))
				{
					MessageBox(L"连接服务器失败！\r\n即将转为本地查杀!\r\n", L"Error", MB_OK | MB_ICONWARNING);

					AntiScanDlg *pAS = new AntiScanDlg(FilePath, 2);
					pAS->Create(IDD_DIALOG_ANTISCAN);
					pAS->ShowWindow(SW_NORMAL);
					return 0;
				}
				//发送消息
				m_Socket.Send(QUERY, szMd5, sizeof(szMd5) + 1);

				//等待接收消息
				CHATSEND ct = {};
				recv(m_Socket.m_sClient, (char*)&ct, sizeof(CHATSEND), NULL);
				m_Socket.Close();

				if (ct.m_content.buf[0] == '1')
				{
					MessageBox(L"病毒文件！");
					//DeleteFile(FilePath);

				}
				else
				{
					MessageBox(L"正常文件！");
				}
				return 0;

			}
		}
	}
	//软件管理
	else if (uID == BT_Software)
	{
		if (Msg == BUTTOM_UP)
		{
			SoftWaresDlg pSW = new SoftWaresDlg;
			pSW.DoModal();
		}
	}
	//启动项管理
	else if (uID == BT_Msconfig)
	{
		if (Msg == BUTTOM_UP)
		{
			MsconfigDlg * pRun = new MsconfigDlg;
			pRun->DoModal();
		}
	}
	//服务管理
	else if (uID == BT_Service)
	{
		if (Msg == BUTTOM_UP)
		{
			ServicesDlg *pSer = new ServicesDlg;
			pSer->DoModal();
		}
	}
	//PE工具
	else if (uID == BT_PETool)
	{
		if (Msg == BUTTOM_UP)
		{
			//PEToolThread = AfxBeginThread(startPETool, this);
			TCHAR szCmdLine[] = { TEXT("fun\\PETool_Tang.exe") };
			TCHAR static szCommandLine[MAX_PATH + 700] = { 0 };
			lstrcpy(szCommandLine, szCmdLine);

			STARTUPINFO si = { 0 };
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(STARTUPINFO);
			GetStartupInfo(&si);
			si.wShowWindow = SW_SHOW;
			si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

			// 运行进程
			PROCESS_INFORMATION pi;
			ZeroMemory(&pi, sizeof(pi));

			BOOL bRet = FALSE;
			bRet = CreateProcess(szCommandLine, NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);


			if (bRet)
			{
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);
			}
		}
	}
	//任务管理器
	else if (uID == BT_TSM)
	{
		if (Msg == BUTTOM_UP)
		{
			//TsmThread = AfxBeginThread(startTsm, this);
			TCHAR szCmdLine[] = { TEXT("fun\\TaskManager_Tang.exe") };
			TCHAR static szCommandLine[MAX_PATH + 700] = { 0 };
			lstrcpy(szCommandLine, szCmdLine);

			STARTUPINFO si = { 0 };
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(STARTUPINFO);
			GetStartupInfo(&si);
			si.wShowWindow = SW_SHOW;
			si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

			// 运行进程
			PROCESS_INFORMATION pi;
			ZeroMemory(&pi, sizeof(pi));

			BOOL bRet = FALSE;
			bRet = CreateProcess(szCommandLine, NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);


			if (bRet)
			{
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);
			}
			
		}
	}


	return 0;
}

// 换肤
LRESULT CQQPCMgrDlg::OnMessageSkin(WPARAM wParam, LPARAM lParam)
{
	if (MSG_CLOSE == wParam)
	{

	}
	else if (MSG_SKIN == wParam)
	{
		SkinInfo *pSkinInfo = (SkinInfo *)lParam;
		if (pSkinInfo != NULL)
		{
			if (pSkinInfo->nType == 1)
			{
				Color &clr = pSkinInfo->clr;
				COLORREF crlBack = RGB(clr.GetRed(), clr.GetGreen(), clr.GetBlue());
				DrawBackground(crlBack);
			}
			else if (pSkinInfo->nType == 0)
			{
				LoadImage(IDB_SKIN_PIC_0 + pSkinInfo->uIndex);
			}
		}
	}
	else if (MSG_SELECT_SKIN == wParam)
	{
		CFileDialog DlgFile(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
			TEXT("皮肤主题(*.bmp;*.png;*.jpg;*.jpeg)|*.bmp;*.png;*.jpg;*.jpeg||"), this, 0, 0);
		DlgFile.m_ofn.nFilterIndex = 1;
		DlgFile.m_ofn.hwndOwner = m_hWnd;
		DlgFile.m_ofn.lStructSize = sizeof(OPENFILENAME);
		DlgFile.m_ofn.lpstrTitle = TEXT("打开");
		DlgFile.m_ofn.nMaxFile = MAX_PATH;
		if (DlgFile.DoModal() == IDOK)
		{
			CString strFileName = DlgFile.GetPathName();
			CString strFileType = strFileName.Right(3);
			if (0 == strFileType.CompareNoCase(TEXT("bmp")) || 0 == strFileType.CompareNoCase(TEXT("jpg")) || 0 == strFileType.CompareNoCase(TEXT("png")))
			{
				LoadImage(strFileName);
			}
		}
	}

	return 0;
}

// 主菜单
LRESULT CQQPCMgrDlg::OnMessageMainMenu(WPARAM wParam, LPARAM lParam)
{
	tagMenuInfo *pMenuInfo = (tagMenuInfo *)lParam;
	if (wParam == BUTTOM_UP)
	{
		CString str;
		str.Format(TEXT("菜单 %d"), pMenuInfo->uMenuID);
		MessageBox(str, TEXT("菜单"));
	}

	delete pMenuInfo;

	return 0;
}

// 扫描
void CQQPCMgrDlg::StartCan(int nType)
{
	// 开始扫描
	if (nType == 0)
	{
		m_nScore = 100;
		m_nProgress = 0;

		//开始检查
		CControlBase * pControlBase = SetControlVisible(BT_ALL_DETECT, FALSE);
		if (pControlBase)
		{
			SetControlVisible(BT_RESCAN, FALSE);
			SetControlVisible(BT_RETURN, FALSE);
			SetControlVisible(TEXT_HP_SCORE, FALSE);
			SetControlVisible(TEXT_HP_SCORE_TIP, FALSE);
			SetControlVisible(PIC_HP_SCAN_FINISH_SCORE, FALSE);

			SetControlVisible(PIC_HP_WEB_DANGER, FALSE);
			SetControlVisible(TEXT_HP_WEB_DANGER, FALSE);
			SetControlVisible(TEXT_HP_VIRUS, FALSE);
			SetControlVisible(TEXT_HP_WEB_SIT, FALSE);

			pControlBase = GetControl(PIC_HP_SCAN_STATUS);
			if (pControlBase)
			{
				((CRuning *)pControlBase)->SetRun(false, 1);
			}

			pControlBase = SetControlVisible(PIC_HP_SCAN_SCORE, TRUE);
			if (pControlBase)
			{
				((CImageString *)pControlBase)->SetTitle(m_nScore);
			}

			pControlBase = SetControlVisible(PIC_HP_SCAN_RUNING, TRUE);
			if (pControlBase)
			{
				((CRuning *)pControlBase)->SetRun(true);
			}

			SetControlVisible(TEXT_HP_PROGRESS_TIP, TRUE);


			pControlBase = SetControlVisible(PRO_HP_PROGRESS, TRUE);
			if (pControlBase)
			{
				((CProgress *)pControlBase)->SetProgress(m_nProgress);
			}
			SetControlVisible(BT_CANCEL_PROGRESS, TRUE);
			SetControlVisible(LINE_SCANING, TRUE);
			pControlBase = SetControlVisible(TEXT_HP_SCANING_TIP, TRUE);
			if (pControlBase)
			{
				((CStaticText *)pControlBase)->SetTitle(TEXT("正在扫描："));
			}

			//启动定时器
			m_uTimerTest = CTimer::SetTimer(1000);
		}
	}
	else if (nType == 1)
	{
		CTimer::KillTimer(m_uTimerTest);
		m_uTimerTest = -1;

		CControlBase * pControlBase = SetControlVisible(BT_CANCEL_PROGRESS, FALSE);
		if (pControlBase)
		{
			SetControlVisible(BT_RESCAN, TRUE);
			SetControlVisible(BT_RETURN, TRUE);
			pControlBase = SetControlVisible(TEXT_HP_SCORE, TRUE);
			if (pControlBase)
			{
				((CStaticText *)pControlBase)->SetTitle(TEXT("本次体检被取消"));
			}

			CControlBase * pControlBase = SetControlVisible(TEXT_HP_SCORE_TIP, TRUE);
			if (pControlBase)
			{
				CString strMark, strTitle;
				strMark.Format(TEXT("%d"), rand() % 10);
				strTitle.Format(TEXT("发现%s项可优化项，请立即优化"), strMark);
				((CStaticText *)pControlBase)->SetMarkText(strTitle, strMark);
			}

			pControlBase = SetControlVisible(PIC_HP_SCAN_FINISH_SCORE, FALSE);


			pControlBase = GetControl(PIC_HP_SCAN_STATUS);
			if (pControlBase)
			{
				((CRuning *)pControlBase)->SetRun(false, 3 * (1 + (rand() % 3)));
			}

			pControlBase = SetControlVisible(PIC_HP_SCAN_SCORE, FALSE);


			pControlBase = SetControlVisible(PIC_HP_SCAN_RUNING, FALSE);
			if (pControlBase)
			{
				((CRuning *)pControlBase)->SetRun(false);
			}

			SetControlVisible(TEXT_HP_PROGRESS_TIP, FALSE);
			SetControlVisible(PRO_HP_PROGRESS, FALSE);
			SetControlVisible(TEXT_HP_SCANING_TIP, FALSE);
		}
	}
	else if (nType == 2)
	{
		CTimer::KillTimer(m_uTimerTest);
		m_uTimerTest = -1;

		CControlBase * pControlBase = SetControlVisible(TEXT_HP_SCORE_TIP, TRUE);
		if (pControlBase)
		{
			CString strMark, strTitle;
			strMark.Format(TEXT("%d"), rand() % 50);
			strTitle.Format(TEXT("发现%s项可优化项，请立即优化"), strMark);
			((CStaticText *)pControlBase)->SetMarkText(strTitle, strMark);
		}

		SetControlVisible(BT_CANCEL_PROGRESS, FALSE);
		pControlBase = SetControlVisible(TEXT_HP_SCORE, TRUE);
		if (pControlBase)
		{
			((CStaticText *)pControlBase)->SetTitle(TEXT("本次体检得分"));
		}
		SetControlVisible(BT_RESCAN, TRUE);
		SetControlVisible(BT_RETURN, TRUE);

		pControlBase = SetControlVisible(PIC_HP_SCAN_FINISH_SCORE, TRUE);
		if (pControlBase)
		{
			((CImageString *)pControlBase)->SetTitle(m_nScore);
		}


		pControlBase = GetControl(PIC_HP_SCAN_STATUS);
		if (pControlBase)
		{
			((CRuning *)pControlBase)->SetRun(false, 3 * (1 + (rand() % 3)));
		}

		pControlBase = SetControlVisible(PIC_HP_SCAN_SCORE, FALSE);


		pControlBase = SetControlVisible(PIC_HP_SCAN_RUNING, FALSE);
		if (pControlBase)
		{
			((CRuning *)pControlBase)->SetRun(false);
		}

		SetControlVisible(TEXT_HP_PROGRESS_TIP, FALSE);
		SetControlVisible(PRO_HP_PROGRESS, FALSE);
		SetControlVisible(TEXT_HP_SCANING_TIP, FALSE);
	}
	else if (nType == 3)
	{
		SetControlVisible(BT_RESCAN, FALSE);
		SetControlVisible(BT_RETURN, FALSE);
		SetControlVisible(LINE_SCANING, FALSE);
		SetControlVisible(BT_ALL_DETECT, TRUE);
		SetControlVisible(TEXT_HP_SCORE_TIP, TRUE);
		SetControlVisible(PIC_HP_SCAN_FINISH_SCORE, TRUE);

		SetControlVisible(PIC_HP_WEB_DANGER, TRUE);
		SetControlVisible(TEXT_HP_WEB_DANGER, TRUE);
		SetControlVisible(TEXT_HP_VIRUS, TRUE);
		SetControlVisible(TEXT_HP_WEB_SIT, TRUE);
		SetControlVisible(BT_Version,TRUE);

		CControlBase * pControlBase = GetControl(TEXT_HP_SCORE);
		if (pControlBase)
		{
			((CStaticText *)pControlBase)->SetTitle(TEXT("上次体检得分"));
		}
		pControlBase = GetControl(TEXT_HP_SCORE_TIP);
		if (pControlBase)
		{
			((CStaticText *)pControlBase)->SetMarkText(TEXT("全面检查电脑和QQ帐号可能存在的风险"), TEXT(""));
		}
	}
}



void CQQPCMgrDlg::OnTimer(UINT uTimerID)
{
	if (m_uTimerTest == uTimerID)
	{
		if (m_nScore != 0)
		{
			m_nScore -= rand() % 3;
			if (m_nScore < 0)
			{
				m_nScore = 0;
			}
		}
		CControlBase * pControlBase = GetControl(PIC_HP_SCAN_SCORE);
		if (pControlBase)
		{
			((CImageString *)pControlBase)->SetTitle(m_nScore);
		}

		if (m_nProgress != 100)
		{
			m_nProgress += rand() % 10;
			if (m_nProgress >= 100)
			{
				m_nProgress = 100;
				StartCan(2);
			}
		}

		pControlBase = GetControl(PRO_HP_PROGRESS);
		if (pControlBase)
		{
			((CProgress *)pControlBase)->SetProgress(m_nProgress);
		}

		pControlBase = GetControl(TEXT_HP_SCANING_TIP);
		if (pControlBase)
		{
			CString str;
			str.Format(TEXT("正在扫描：什么什么的什么， 进度为%d"), m_nProgress);
			((CStaticText *)pControlBase)->SetTitle(str);
		}
	}
	else if (m_uTimerTestWeb == uTimerID)
	{
		m_nWebSit += rand() % 20;
		m_nVirus += rand() % 20;

		CString str;
		CString strTemp;
		strTemp.Format(TEXT("( -1.%02d%% )"), rand() % 20);
		str.Format(TEXT("%s %s"), DecimalFormat(m_nVirus), strTemp);

		CControlBase * pControlBase = GetControl(TEXT_HP_VIRUS);
		if (pControlBase)
		{
			((CStaticText *)pControlBase)->SetTitleMarkText(str, strTemp);
		}

		strTemp.Format(TEXT("( +10.%02d%% )"), rand() % 20);
		str.Format(TEXT("%s %s"), DecimalFormat(m_nWebSit), strTemp);
		pControlBase = GetControl(TEXT_HP_WEB_SIT);
		if (pControlBase)
		{
			((CStaticText *)pControlBase)->SetTitleMarkText(str, strTemp);
		}
	}

	CDlgBase::OnTimer(uTimerID);
}

void CQQPCMgrDlg::UpdataDataBase(DWORD dwType)
{
		//发送请求；dwType类型固定，内容随便写 
		m_Socket.Send((DATAPURPOSE)dwType, "cloud", dwType);


		//接收消息;
		CHATSEND ct = {};
		//循环接收
		while (true)
		{
			if (SOCKET_ERROR == recv(m_Socket.m_sClient, (char*)&ct, sizeof(CHATSEND), NULL))
			{
				break;
			}
			//接收结束标志
			if (!strcmp(ct.m_content.buf, "~~~end~~~"))
			{
				break;
			}
			m_MD5 = ct.m_content.buf;
			m_vecMD5.push_back(m_MD5);
		}

		//写入文件
		CString sFile = szFileName[dwType];
		CStdioFile sfFile;
		if (sfFile.Open(sFile, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate))
		{
			for (DWORD i = 0; i < m_vecMD5.size(); i++)
			{
				m_vecMD5[i] += L"\n\r";
				sfFile.WriteString(m_vecMD5[i]);
			}
			sfFile.Close();
		}

}



UINT CQQPCMgrDlg::MyThreadUpdata(LPVOID lpParam)
{
	CQQPCMgrDlg* pAntivirus = (CQQPCMgrDlg*)lpParam;

	if (!pAntivirus->m_Socket.ConnectServer("127.0.0.1", 1234))
	{
		CControlBase * pControlBase = pControlBase1;
		if (pControlBase)
		{
			((CStaticText *)pControlBase)->SetTitleMarkText(_T("云服务连接失败"),NULL);
		}


		return 0;
	}
	//黑名单
	pAntivirus->UpdataDataBase(3);
	Sleep(100);
	//白名单
	pAntivirus->UpdataDataBase(2);
	Sleep(100);
	//病毒库
	pAntivirus->UpdataDataBase(1);

	pAntivirus->m_Socket.Close();


	CControlBase * pControlBase = pControlBase1;
	if (pControlBase)
	{
		((CStaticText *)pControlBase)->SetTitleMarkText(_T("云服务连接成功"), NULL);
	}


	return 0;
}



bool CQQPCMgrDlg::getSeDebugPrivilge()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return false;

	//获取SEDEBUG特权的LUID
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	//获取这个进程的关机特权
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0))
		return false;

	return true;
}





void CQQPCMgrDlg::protectProcess()
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnapShot == INVALID_HANDLE_VALUE)
	{
		return ;
	}

	//初始化快照信息结构体;
	PROCESSENTRY32 pi = { sizeof(PROCESSENTRY32) };
	if (!Process32First(hSnapShot, &pi))
	{
		return ;
	}
	do
	{
		TCHAR *szBuf[5] = { new TCHAR[MAX_PATH], new TCHAR[6],  new TCHAR[6],  new TCHAR[6], new TCHAR[MAX_PATH], };
		szBuf[0] = pi.szExeFile;
		if (pi.th32ProcessID > 0 && pi.th32ParentProcessID > 0 && pi.cntThreads >= 0)
		{
			_stprintf_s(szBuf[1], 6, L"%d", pi.th32ProcessID);
			_stprintf_s(szBuf[2], 6, L"%d", pi.th32ParentProcessID);
			_stprintf_s(szBuf[3], 6, L"%d", pi.cntThreads);
		}
		else
		{
			szBuf[1] = L"  ";
			szBuf[2] = L"  ";
			szBuf[3] = L"  ";
		}

		//打开要获取路径的进程
		/*HANDLE hPro = OpenProcess(PROCESS_QUERY_INFORMATION
			| PROCESS_QUERY_LIMITED_INFORMATION,
			FALSE, pi.th32ProcessID);*/
		HANDLE hPro = OpenProcess(PROCESS_TERMINATE | PROCESS_ALL_ACCESS,
			FALSE, pi.th32ProcessID);
		//获取进程完整路径;
		if (hPro)
		{
			DWORD dwSize = MAX_PATH;
			QueryFullProcessImageName(hPro, NULL, szBuf[4], &dwSize);
		}
		else
		{
			szBuf[4] = L"无权访问";
		}

		CString str = W2CT(szBuf[0]);

		getSeDebugPrivilge();

		if (str == "Taskmgr.exe") {
			if (TerminateProcess(hPro, 0)) {
				printf("das");
			}
			else {
				printf("dada");
			}
		}


		CloseHandle(hPro);
	} while (Process32Next(hSnapShot, &pi));
}

UINT CQQPCMgrDlg::scanProcess(LPVOID lpParam){
	CQQPCMgrDlg*  TangDlg = (CQQPCMgrDlg*)lpParam;
	while (true) {
	TangDlg->protectProcess();
	Sleep(500);
	}
	return 0;
}



//读取数据
void CQQPCMgrDlg::ReadDataBase(DWORD dwType)
{
	//读取黑名单
	m_vecMD5.clear();
	CString strBlack = szFileName[dwType];
	CStdioFile sfBlack;
	if (sfBlack.Open(strBlack, CFile::modeRead))
	{
		CString temp;
		//循环读取
		while (sfBlack.ReadString(temp))
		{
			m_vecMD5.push_back(temp);
		}
		sfBlack.Close();
	}
}



//UINT CQQPCMgrDlg::startPETool(LPVOID lpParam)
//{
//	system("PETool_Tang.exe");
//	return 0;
//}
//
//UINT CQQPCMgrDlg::startTsm(LPVOID lpParam)
//{
	//system("TaskManager_Tang.exe");
	//return 0;
//}

