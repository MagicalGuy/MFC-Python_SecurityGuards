// MsconfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QQPCMgr.h"
#include "MsconfigDlg.h"
#include "afxdialogex.h"


// MsconfigDlg 对话框

IMPLEMENT_DYNAMIC(MsconfigDlg, CDialogEx)

MsconfigDlg::MsconfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_RUN, pParent)
{

}

MsconfigDlg::~MsconfigDlg()
{
}

void MsconfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RUN, m_ListRun);
}


void MsconfigDlg::OnNMRClickListRun(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//获取弹出菜单;
	CMenu* pMenu = new CMenu;
	pMenu->LoadMenuW(IDR_MENU1);

	CMenu* pSub = pMenu->GetSubMenu(2);

	//获取鼠标位置;
	CPoint point;
	GetCursorPos(&point);

	//弹出;
	pSub->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);

	*pResult = 0;
}

void MsconfigDlg::OnMenuModifyData(UINT uID)
{
	switch (uID)
	{
	case ID_32777:
		AddRun();
		break;
	case ID_32778:
		DeleRun();
		break;
	case ID_32779:
		UpDataRun();
		break;
	default:
		break;
	}
}

BEGIN_MESSAGE_MAP(MsconfigDlg, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_RUN, &MsconfigDlg::OnNMRClickListRun)
	ON_CONTROL_RANGE(BN_CLICKED, ID_32777, ID_32779, OnMenuModifyData)	//右键菜单消息处理;
END_MESSAGE_MAP()


// MsconfigDlg 消息处理程序


void MsconfigDlg::DeleRun()
{
		//获取选中项
		int nSel = m_ListRun.GetSelectionMark();
		if (nSel < 0)
		{
			MessageBox(L"请选中数据");
		}
		//获取路径
		CString strName = m_ListRun.GetItemText(nSel, 0);
		CString strRoot = m_ListRun.GetItemText(nSel, 2);
		CString strSub = m_ListRun.GetItemText(nSel, 3);
		HKEY hRoot;

		//获取主键
		if (strRoot == m_Root1)
		{
			hRoot = m_hRootKey1;
		}
		else
		{
			hRoot = m_hRootKey2;
		}

		//临时数据
		DWORD dwType = 0;
		DWORD dwBuffSize = MAXBYTE;
		DWORD dwKeySize = MAXBYTE;
		WCHAR szValueNme[MAXBYTE] = { 0 };
		WCHAR szValueKey[MAXBYTE] = { 0 };
		HKEY hKey = NULL;

		//打开键，此处需要设置权限
		if (RegOpenKeyEx(hRoot, strSub, 0,
			KEY_SET_VALUE | KEY_WOW64_64KEY, &hKey) != ERROR_SUCCESS)
		{
			return;
		}

		RegDeleteValue(hKey, strName);

		UpDataRun();

}

void MsconfigDlg::UpDataRun()
{
	m_ListRun.DeleteAllItems();

	//主键子键组合获取启动项
	GetRunInfo(m_hRootKey1, m_SubKey1);

	GetRunInfo(m_hRootKey1, m_SubKey2);

	GetRunInfo(m_hRootKey2, m_SubKey1);
}

void MsconfigDlg::AddRun()
{
		CFileDialog dlg(TRUE, _T("exe"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("可执行程序|*.exe|所有文件|*||"));
		if (dlg.DoModal() == IDOK) {

			m_Key = dlg.GetPathName();

			//获取文件名
			int nPos = m_Key.ReverseFind(L'\\');
			m_Name = m_Key.Right(m_Key.GetLength() - nPos - 1);

			HKEY hKey = NULL;

			//创建注册表键值
			RegCreateKeyEx(m_hRootKey1, m_SubKey1, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);

			if (ERROR_SUCCESS == RegSetValueEx(hKey, m_Name, 0, REG_SZ,
				(LPBYTE)m_Key.GetBuffer(), (m_Key.GetLength() + 1) * sizeof(TCHAR)))
			{
				MessageBox(L"添加成功！");
			}
			else
			{
				MessageBox(L"添加失败！");
			}
			RegCloseKey(hKey);

			//刷新
			UpDataRun();
		}
}

void MsconfigDlg::GetRunInfo(HKEY hRootKey, CString Path)
{
		//判断主键
		CString strRoot;
		if (hRootKey == HKEY_LOCAL_MACHINE)
		{
			strRoot = m_Root1;
		}
		else
		{
			strRoot = m_Root2;
		}

		//打开键
		HKEY hKey = NULL;
		if (ERROR_SUCCESS != RegOpenKeyEx(hRootKey, Path, 0,
			KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | KEY_WOW64_64KEY, &hKey))
		{
			return;
		}

		//查询类型
		int i = 0;
		DWORD dwType = REG_SZ | REG_EXPAND_SZ;
		//循环获取

		while (true)
		{
			DWORD dwBuffSize = MAXBYTE;
			DWORD dwKeySize = MAXBYTE;
			WCHAR szValueNme[MAXBYTE] = { 0 };
			WCHAR szValueKey[MAXBYTE] = { 0 };

			//枚举键名
			int rect = RegEnumValue(hKey, i, szValueNme, &dwBuffSize,
				NULL, &dwType, (LPBYTE)szValueKey, &dwKeySize);

			//查询键值
			int rect2 = RegQueryValueEx(hKey, szValueNme, 0,
				&dwType, (LPBYTE)szValueKey, &dwKeySize);

			//完毕结束
			if (rect == ERROR_NO_MORE_ITEMS)
			{
				break;
			}

			//输出
			m_Name = szValueNme;
			m_Key = szValueKey;
			m_ListRun.AddItem(4, m_Name, m_Key, strRoot, Path);

			i++;

		}
		//完毕键
		RegCloseKey(hKey);
}

BOOL MsconfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListRun.AddColumn(4, L"名称", 100, L"键值", 150, L"主键", 140, L"子键", 250);

	//需要遍历的路径
	m_SubKey1 = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	m_SubKey2 = L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Run";
	m_Root1 = L"HKEY_LOCAL_MACHINE";
	m_Root2 = L"HKEY_CURRENT_USER";
	m_hRootKey1 = HKEY_LOCAL_MACHINE;
	m_hRootKey2 = HKEY_CURRENT_USER;

	//获取数据
	UpDataRun();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
