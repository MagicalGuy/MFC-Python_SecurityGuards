// RocketDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QQPCMgr.h"
#include "RocketDlg.h"
#include "afxdialogex.h"
#include"Psapi.h"


// RocketDlg �Ի���

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


// RocketDlg ��Ϣ�������


BOOL RocketDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_Rocket.LoadStdImage(IDB_PNG1, L"PNG");

	//��ɫ͸��
	COLORREF maskColor = RGB(240, 240, 240);
	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE,
		GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE) | 0x80000);
	//����͸��
	SetLayeredWindowAttributes(maskColor, 255, 1);


	CRect cr;

	GetClientRect(&cr);//��ȡ�Ի���ͻ������С

	ClientToScreen(&cr);//ת��ΪӫĻ����

	int x = GetSystemMetrics(SM_CXSCREEN);//��ȡӫĻ����Ŀ�ȣ���λΪ����

	int y = GetSystemMetrics(SM_CYSCREEN);//��ȡӫĻ����ĸ߶ȣ���λΪ����

	MoveWindow((x - cr.Width() * 0.9), y - cr.Height()*0.9, cr.Width() * 2, cr.Height() * 2);//���ô�����ʾ��λ���Լ���С



	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

UINT RocketDlg::MyThreadRocket(LPVOID lpParam)
{
	RocketDlg* pRocket = (RocketDlg*)lpParam;

	//��ȡ��Ļ�ߴ�
	int scrWidth, scrHeight;
	scrWidth = GetSystemMetrics(SM_CXSCREEN);
	scrHeight = GetSystemMetrics(SM_CYSCREEN);

	//��������
	int nPosX = scrWidth - 200;
	int nPosY = scrHeight - 400;
	pRocket->MoveWindow(nPosX, nPosY, 100, 300);


	//ѭ���ƶ�
	while (true)
	{
		//���10
		Sleep(20);
		nPosY -= 10;
		pRocket->MoveWindow(nPosX, nPosY, 100, 300);

		//ָ��λ�ý���
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�����߳��ƶ�;
	hRocketThread = AfxBeginThread(MyThreadRocket, this);


	//��ȡ��ǰ�ڴ�״̬
	MEMORYSTATUSEX stcMemStatusEX = { 0 };
	stcMemStatusEX.dwLength = sizeof(stcMemStatusEX);
	GlobalMemoryStatusEx(&stcMemStatusEX);
	DWORDLONG preUsedMem = stcMemStatusEX.ullTotalPhys - stcMemStatusEX.ullAvailPhys;

	//�����ڴ�
	DWORD dwPIDList[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList, bufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); i++)
	{
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA, false, dwPIDList[i]);
		SetProcessWorkingSetSize(hProcess, -1, -1);
	}

	//��ȡ�������ڴ�״̬
	GlobalMemoryStatusEx(&stcMemStatusEX);
	DWORDLONG afterCleanUsedMem = stcMemStatusEX.ullTotalPhys - stcMemStatusEX.ullAvailPhys;

	CString message;
	message.Format(L"��Ϊ���Ż��ڴ�: %d MB", (preUsedMem - afterCleanUsedMem) / 1024 / 1024);


	MessageBox(message);


}

HBRUSH RocketDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	//����͸������
	if (nCtlColor == CTLCOLOR_DLG)
	{
		CBrush *brush;
		brush = new CBrush(RGB(240, 240, 240));
		return (HBRUSH)(brush->m_hObject);
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
