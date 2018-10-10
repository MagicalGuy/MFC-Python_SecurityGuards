// QQCleanDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QQPCMgr.h"
#include "QQCleanDlg.h"
#include "afxdialogex.h"


// QQCleanDlg �Ի���

IMPLEMENT_DYNAMIC(QQCleanDlg, CDialogEx)

QQCleanDlg::QQCleanDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLEAN_DIALOG, pParent)
	, m_CbSystem(FALSE)
	, m_CbRecycle(FALSE)
	, m_CbIE(FALSE)
	, m_CbAll(FALSE)
{

}

QQCleanDlg::~QQCleanDlg()
{
}

void QQCleanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_SYSTEM, m_CbSystem);
	DDX_Check(pDX, IDC_CHECK_RECYCLE, m_CbRecycle);
	DDX_Check(pDX, IDC_CHECK_IE, m_CbIE);
	DDX_Check(pDX, IDC_CHECK_ALL, m_CbAll);
	DDX_Control(pDX, IDC_LIST_CLEAN, m_ListClean);
}


BEGIN_MESSAGE_MAP(QQCleanDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_ALL, &QQCleanDlg::OnBnClickedCheckAll)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN, &QQCleanDlg::OnBnClickedButtonClean)
END_MESSAGE_MAP()


// QQCleanDlg ��Ϣ�������


void QQCleanDlg::CleanRecycle()
{
	//��ʼ��
	SHQUERYRBINFO RecycleBinInfo = {};
	RecycleBinInfo.cbSize = sizeof(SHQUERYRBINFO);

	//��ѯ��Ϣ
	SHQueryRecycleBin(NULL, &RecycleBinInfo);

	//���
	SHEmptyRecycleBin(NULL, NULL, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
}

bool QQCleanDlg::DeleFile(CString DirName, CString FileName)
{
	//��·���˳�;
	if (DirName.IsEmpty())
	{
		MessageBox(L"·������");
		return	false;
	}

	//�ж��Ƿ��ļ���;
	WIN32_FIND_DATA wsd = {};
	HANDLE hFind = FindFirstFile(DirName, &wsd);
	if (!(wsd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
	{
		MessageBox(L"�����ļ���");
		return false;
	}

	DirName += L"\\*";
	hFind = FindFirstFile(DirName, &wsd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"��·��ʧ��");
	}

	do
	{
		//���˱������ϼ�;
		if (!wcscmp(wsd.cFileName, L".")
			|| !wcscmp(wsd.cFileName, L".."))
		{
			continue;
		}
		//�ݹ�����ļ���;
		else if (wsd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			CString NextDir = DirName.Left(DirName.GetLength() - 1);
			NextDir += wsd.cFileName;
			DeleFile(NextDir, FileName);
		}
		//ת���ļ�����ʽ;
		TCHAR* szBuff = wsd.cFileName;
		//��ȡ��׺��;
		TCHAR* szExName = PathFindExtension(szBuff);
		//�ԱȺ�׺��;
		if (FileName.Find(szExName) >= 0)
		{
			//ɾ��ָ���ļ�;
			CString DeleFileName = DirName.Left(DirName.GetLength() - 1);
			DeleFileName += wsd.cFileName;
			DeleteFile(DeleFileName);
			m_Index.Format(L"%d", m_dwIndex);
			m_DeleFileName = DeleFileName;
			m_ListClean.AddItem(3, m_Index, m_DeleFileName, m_Result);
			m_dwIndex++;
		}

	} while (FindNextFile(hFind, &wsd));

	FindClose(hFind);

	return true;
}

void QQCleanDlg::OnBnClickedCheckAll()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_CbAll)
	{
		m_CbSystem = TRUE;
		m_CbRecycle = TRUE;
		m_CbIE = TRUE;
	}
	else
	{
		m_CbSystem = FALSE;
		m_CbRecycle = FALSE;
		m_CbIE = FALSE;
	}
	UpdateData(FALSE);
}


void QQCleanDlg::OnBnClickedButtonClean()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UpdateData(TRUE);
	m_ListClean.DeleteAllItems();
	m_dwIndex = 1;
	if (m_CbSystem)
	{
		m_Result = L"ϵͳ����";
		DeleFile(m_PathA, m_DeleFileName);
		//DeleFile(m_PathB, m_DeleFileName);
	}

	if (m_CbIE)
	{
		m_Result = L"���������";
		DeleFile(m_PathIE, m_DeleFileName);
	}

	if (m_CbRecycle)
	{
		m_Result = L"����վ";
		CleanRecycle();
	}

	MessageBox(L"������ɣ�");
}


BOOL QQCleanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//�������ԱȨ����ҷ�ļ�;
	ChangeWindowMessageFilterEx(this->m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(this->m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);
	// 0x0049 == WM_COPYGLOBALDATA


	//���������ļ�����
	m_DeleFileName = L"\\\.tmp \\\._mp \\\.log \\\.gid \\\.chk \\\.old \\\.bak";

	//����·��
	//m_PathA = L"%%windir%%";//C: windows
	//m_PathB = L"%%systemdrive%%";//  C:

	CString homeProfile = L"systemdrive";
	WCHAR homePath[1024] = { 0 };

	unsigned int pathSize = GetEnvironmentVariable(homeProfile, homePath, 1024);
	m_PathA = homePath + CString("\\Temp");


	CString homeProfile1 = L"userprofile";
	WCHAR homePath1[1024] = { 0 };

	unsigned int pathSize1 = GetEnvironmentVariable(homeProfile1, homePath1, 1024);
	m_PathIE = homePath1 + CString("\\AppData\\Local\\Microsoft\\Windows\\INetCache");


	//m_PathIE = L"%%userprofile%%\\AppData\\Local\\Microsoft\\Windows\\INetCache\\";//c: Users �û���
	m_ListClean.AddColumn(3, L"���", 50, L"�ļ�·��", 200, L"״̬", 100);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
