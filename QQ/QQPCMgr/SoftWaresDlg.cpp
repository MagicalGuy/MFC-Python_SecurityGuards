// SoftWaresDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QQPCMgr.h"
#include "SoftWaresDlg.h"
#include "afxdialogex.h"


// SoftWaresDlg 对话框

IMPLEMENT_DYNAMIC(SoftWaresDlg, CDialogEx)

SoftWaresDlg::SoftWaresDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SOFTWARE, pParent)
{
	m_SubKey1 = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
	m_SubKey2 = L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
	m_hRootKey = HKEY_LOCAL_MACHINE;

	m_KeyName = L"DisplayName";
	m_KeyNameVer = L"DisplayVersion";
	m_KeyNameSize = L"EstimatedSize";
	m_KeyNameDate = L"InstallDate";
	m_KeyNamePub = L"Publisher";
	m_KeyNameInsPath = L"InstallLocation";
	m_KeyNameUniPath = L"UninstallString";
}

SoftWaresDlg::~SoftWaresDlg()
{
}

void SoftWaresDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SOFTWARE, m_ListSoftware);
}


void SoftWaresDlg::OnMenuSoftMG(UINT uID)
{
	switch (uID)
	{
	case ID_32774:
		OpenExploer();
		break;
	case ID_32775:
		UnistallSoft();
		break;
	case ID_32776:
		UpdateSoftInfo();
		break;
	default:
		break;
	}
}

void SoftWaresDlg::OnNMRClickListSoftware(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//获取弹出菜单;
	CMenu* pMenu = new CMenu;
	pMenu->LoadMenuW(IDR_MENU1);

	CMenu* pSub = pMenu->GetSubMenu(1);

	//获取鼠标位置;
	CPoint point;
	GetCursorPos(&point);

	//弹出;
	pSub->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);


	*pResult = 0;
}

void SoftWaresDlg::OnLvnColumnclickListSoftware(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//获取点击列;
	int Sort_Colum = pNMLV->iSubItem;

	//列2字符串排序;
	if (Sort_Colum == 2)
		m_ListSoftware.SortItemsPlus(Sort_Colum, false);
	else
		m_ListSoftware.SortItemsPlus(Sort_Colum);

	*pResult = 0;
}

BEGIN_MESSAGE_MAP(SoftWaresDlg, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SOFTWARE, &SoftWaresDlg::OnNMRClickListSoftware)
	ON_CONTROL_RANGE(BN_CLICKED, ID_32774, ID_32776, OnMenuSoftMG)			//右键菜单消息处理;
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_SOFTWARE, &SoftWaresDlg::OnLvnColumnclickListSoftware)
END_MESSAGE_MAP()


// SoftWaresDlg 消息处理程序


void SoftWaresDlg::GetSoftwareInfo(CString SubKey)
{
		//定义相关信息
		HKEY hKey = NULL;
		HKEY hSubKey = NULL;
		LONG lRet;
		CString strBuffer;
		CString strMidReg;

		DWORD dwIndex = 0;
		DWORD dwSoftSize = 0;
		WCHAR szKeyName[MAX_PATH] = { 0 };
		WCHAR szBuffer[MAX_PATH] = { 0 };
		DWORD dwKeyLen = MAXBYTE;
		DWORD dwNameLen = MAXBYTE;
		DWORD dwType = REG_DWORD | REG_EXPAND_SZ | REG_SZ;

		//打开存在注册表子键
		lRet = RegOpenKeyEx(m_hRootKey, SubKey, 0, KEY_READ | KEY_WOW64_64KEY, &hKey);

		if (lRet == ERROR_SUCCESS)
		{
			//枚举软件Uninstall目录子键
			while (ERROR_NO_MORE_ITEMS != RegEnumKeyEx(hKey, dwIndex, szKeyName, &dwKeyLen, 0, NULL, NULL, NULL))
			{

				strBuffer.Format(L"%s", szKeyName);
				if (!strBuffer.IsEmpty())
				{
					//格式化拼接子键
					strMidReg = SubKey + L"\\" + strBuffer;
					//打开单个软件子键
					if (ERROR_SUCCESS == RegOpenKeyEx(m_hRootKey, strMidReg, 0, KEY_READ | KEY_WOW64_64KEY, &hSubKey))
					{
						//获取信息
						SOFTINFO SoftInfo = { 0 };

						//名称
						RegQueryValueEx(hSubKey, m_KeyName, 0, &dwType, (LPBYTE)SoftInfo.szSoftName, &dwNameLen);
						dwNameLen = MAXBYTE;	//需要恢复此值

						//厂商
						RegQueryValueEx(hSubKey, m_KeyNamePub, 0, &dwType, (LPBYTE)SoftInfo.szSoftPub, &dwNameLen);
						dwNameLen = MAXBYTE;

						//日期
						RegQueryValueEx(hSubKey, m_KeyNameDate, 0, &dwType, (LPBYTE)SoftInfo.szSoftDate, &dwNameLen);
						dwNameLen = MAXBYTE;

						//版本
						RegQueryValueEx(hSubKey, m_KeyNameVer, 0, &dwType, (LPBYTE)SoftInfo.szSoftVer, &dwNameLen);
						dwNameLen = MAXBYTE;

						//安装路径
						RegQueryValueEx(hSubKey, m_KeyNameInsPath, 0, &dwType, (LPBYTE)SoftInfo.szSoftInsPath, &dwNameLen);
						dwNameLen = MAXBYTE;

						//卸载路径
						RegQueryValueEx(hSubKey, m_KeyNameUniPath, 0, &dwType, (LPBYTE)SoftInfo.szSoftUniPath, &dwNameLen);
						dwNameLen = MAXBYTE;

						//大小为DWORD值
						RegQueryValueEx(hSubKey, m_KeyNameSize, 0, &dwType, (LPBYTE)&dwSoftSize, &dwNameLen);
						dwNameLen = MAXBYTE;

						//转换文件大小
						CString strSize;
						if (dwSoftSize > 0 && dwSoftSize < 1024)
						{
							strSize.Format(L"%dKB", dwSoftSize);
						}
						else if (dwSoftSize > 1024 && dwSoftSize < 1024 * 1024)
						{
							strSize.Format(L"%dMB", dwSoftSize / 1024);
						}
						else if (dwSoftSize > 1024 * 1024)
						{
							strSize.Format(L"%dGB", dwSoftSize / 1024 / 1024);
						}
						else
						{
							strSize = L" ";
						}
						memcpy_s(SoftInfo.szSoftSize, (strSize.GetLength() + 1) * sizeof(WCHAR),
							strSize.GetBuffer(), (strSize.GetLength() + 1) * sizeof(WCHAR));

						CString strName = SoftInfo.szSoftName;
						if (!strName.IsEmpty())
						{
							m_vecSoftInfo.push_back(SoftInfo);
						}
					}
					dwIndex++;
					dwKeyLen = MAXBYTE;
					dwNameLen = MAXBYTE;
					dwSoftSize = 0;
					ZeroMemory(szKeyName, MAX_PATH);
				}

			}

		}
}

void SoftWaresDlg::UpdateSoftInfo()
{
		m_ListSoftware.DeleteAllItems();
		m_vecSoftInfo.clear();

		GetSoftwareInfo(m_SubKey1);

		GetSoftwareInfo(m_SubKey2);


		for (size_t i = 0; i < m_vecSoftInfo.size(); i++)
		{
			m_SoftName = m_vecSoftInfo[i].szSoftName;
			m_SoftUniPath = m_vecSoftInfo[i].szSoftPub;
			m_SoftDate = m_vecSoftInfo[i].szSoftDate;
			m_SoftSize = m_vecSoftInfo[i].szSoftSize;
			m_SoftVer = m_vecSoftInfo[i].szSoftVer;

			m_ListSoftware.AddItem(5,
				m_SoftName,
				m_SoftUniPath,
				m_SoftDate,
				m_SoftSize,
				m_SoftVer);
		}

}

void SoftWaresDlg::OpenExploer()
{
	//获取选中项
	int nSel = m_ListSoftware.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"请选中数据");
	}
	//获取路径
	CString SoftIniPath = m_vecSoftInfo[nSel].szSoftInsPath;
	if (SoftIniPath.IsEmpty())
	{
		MessageBox(L"打开路径失败！");
	}
	else
	{
		ShellExecute(NULL, _T("open"), _T("explorer.exe"), SoftIniPath, NULL, SW_SHOWNORMAL);
	}
}

void SoftWaresDlg::UnistallSoft()
{
	//获取选中项
	int nSel = m_ListSoftware.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"请选中数据");
	}
	//获取路径
	CString SoftUniPath = m_vecSoftInfo[nSel].szSoftUniPath;
	if (SoftUniPath.IsEmpty())
	{
		MessageBox(L"卸载失败！");
		return;
	}
	else
	{
		ShellExecute(NULL, _T("open"), SoftUniPath, NULL, NULL, SW_SHOW);
	}
}

BOOL SoftWaresDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ListSoftware.AddColumn(5, L"名称", 350, L"发布者", 150, L"安装日期", 80, L"大小", 80, L"版本", 150);

	//获取软件信息
	UpdateSoftInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
