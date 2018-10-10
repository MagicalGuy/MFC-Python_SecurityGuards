// QQCleanDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QQPCMgr.h"
#include "QQCleanDlg.h"
#include "afxdialogex.h"


// QQCleanDlg 对话框

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


// QQCleanDlg 消息处理程序


void QQCleanDlg::CleanRecycle()
{
	//初始化
	SHQUERYRBINFO RecycleBinInfo = {};
	RecycleBinInfo.cbSize = sizeof(SHQUERYRBINFO);

	//查询信息
	SHQueryRecycleBin(NULL, &RecycleBinInfo);

	//清空
	SHEmptyRecycleBin(NULL, NULL, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
}

bool QQCleanDlg::DeleFile(CString DirName, CString FileName)
{
	//空路径退出;
	if (DirName.IsEmpty())
	{
		MessageBox(L"路径有误");
		return	false;
	}

	//判断是否文件夹;
	WIN32_FIND_DATA wsd = {};
	HANDLE hFind = FindFirstFile(DirName, &wsd);
	if (!(wsd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
	{
		MessageBox(L"不是文件夹");
		return false;
	}

	DirName += L"\\*";
	hFind = FindFirstFile(DirName, &wsd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"打开路径失败");
	}

	do
	{
		//过滤本级和上级;
		if (!wcscmp(wsd.cFileName, L".")
			|| !wcscmp(wsd.cFileName, L".."))
		{
			continue;
		}
		//递归遍历文件夹;
		else if (wsd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			CString NextDir = DirName.Left(DirName.GetLength() - 1);
			NextDir += wsd.cFileName;
			DeleFile(NextDir, FileName);
		}
		//转换文件名格式;
		TCHAR* szBuff = wsd.cFileName;
		//获取后缀名;
		TCHAR* szExName = PathFindExtension(szBuff);
		//对比后缀名;
		if (FileName.Find(szExName) >= 0)
		{
			//删除指定文件;
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
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	m_ListClean.DeleteAllItems();
	m_dwIndex = 1;
	if (m_CbSystem)
	{
		m_Result = L"系统垃圾";
		DeleFile(m_PathA, m_DeleFileName);
		//DeleFile(m_PathB, m_DeleFileName);
	}

	if (m_CbIE)
	{
		m_Result = L"浏览器垃圾";
		DeleFile(m_PathIE, m_DeleFileName);
	}

	if (m_CbRecycle)
	{
		m_Result = L"回收站";
		CleanRecycle();
	}

	MessageBox(L"清理完成！");
}


BOOL QQCleanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//允许管理员权限拖曳文件;
	ChangeWindowMessageFilterEx(this->m_hWnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
	ChangeWindowMessageFilterEx(this->m_hWnd, 0x0049, MSGFLT_ALLOW, NULL);
	// 0x0049 == WM_COPYGLOBALDATA


	//定义清理文件类型
	m_DeleFileName = L"\\\.tmp \\\._mp \\\.log \\\.gid \\\.chk \\\.old \\\.bak";

	//定义路径
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


	//m_PathIE = L"%%userprofile%%\\AppData\\Local\\Microsoft\\Windows\\INetCache\\";//c: Users 用户名
	m_ListClean.AddColumn(3, L"序号", 50, L"文件路径", 200, L"状态", 100);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
