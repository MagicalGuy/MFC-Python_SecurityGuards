// SoftWaresDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QQPCMgr.h"
#include "SoftWaresDlg.h"
#include "afxdialogex.h"


// SoftWaresDlg �Ի���

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��ȡ�����˵�;
	CMenu* pMenu = new CMenu;
	pMenu->LoadMenuW(IDR_MENU1);

	CMenu* pSub = pMenu->GetSubMenu(1);

	//��ȡ���λ��;
	CPoint point;
	GetCursorPos(&point);

	//����;
	pSub->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);


	*pResult = 0;
}

void SoftWaresDlg::OnLvnColumnclickListSoftware(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��ȡ�����;
	int Sort_Colum = pNMLV->iSubItem;

	//��2�ַ�������;
	if (Sort_Colum == 2)
		m_ListSoftware.SortItemsPlus(Sort_Colum, false);
	else
		m_ListSoftware.SortItemsPlus(Sort_Colum);

	*pResult = 0;
}

BEGIN_MESSAGE_MAP(SoftWaresDlg, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SOFTWARE, &SoftWaresDlg::OnNMRClickListSoftware)
	ON_CONTROL_RANGE(BN_CLICKED, ID_32774, ID_32776, OnMenuSoftMG)			//�Ҽ��˵���Ϣ����;
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_SOFTWARE, &SoftWaresDlg::OnLvnColumnclickListSoftware)
END_MESSAGE_MAP()


// SoftWaresDlg ��Ϣ�������


void SoftWaresDlg::GetSoftwareInfo(CString SubKey)
{
		//���������Ϣ
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

		//�򿪴���ע����Ӽ�
		lRet = RegOpenKeyEx(m_hRootKey, SubKey, 0, KEY_READ | KEY_WOW64_64KEY, &hKey);

		if (lRet == ERROR_SUCCESS)
		{
			//ö�����UninstallĿ¼�Ӽ�
			while (ERROR_NO_MORE_ITEMS != RegEnumKeyEx(hKey, dwIndex, szKeyName, &dwKeyLen, 0, NULL, NULL, NULL))
			{

				strBuffer.Format(L"%s", szKeyName);
				if (!strBuffer.IsEmpty())
				{
					//��ʽ��ƴ���Ӽ�
					strMidReg = SubKey + L"\\" + strBuffer;
					//�򿪵�������Ӽ�
					if (ERROR_SUCCESS == RegOpenKeyEx(m_hRootKey, strMidReg, 0, KEY_READ | KEY_WOW64_64KEY, &hSubKey))
					{
						//��ȡ��Ϣ
						SOFTINFO SoftInfo = { 0 };

						//����
						RegQueryValueEx(hSubKey, m_KeyName, 0, &dwType, (LPBYTE)SoftInfo.szSoftName, &dwNameLen);
						dwNameLen = MAXBYTE;	//��Ҫ�ָ���ֵ

						//����
						RegQueryValueEx(hSubKey, m_KeyNamePub, 0, &dwType, (LPBYTE)SoftInfo.szSoftPub, &dwNameLen);
						dwNameLen = MAXBYTE;

						//����
						RegQueryValueEx(hSubKey, m_KeyNameDate, 0, &dwType, (LPBYTE)SoftInfo.szSoftDate, &dwNameLen);
						dwNameLen = MAXBYTE;

						//�汾
						RegQueryValueEx(hSubKey, m_KeyNameVer, 0, &dwType, (LPBYTE)SoftInfo.szSoftVer, &dwNameLen);
						dwNameLen = MAXBYTE;

						//��װ·��
						RegQueryValueEx(hSubKey, m_KeyNameInsPath, 0, &dwType, (LPBYTE)SoftInfo.szSoftInsPath, &dwNameLen);
						dwNameLen = MAXBYTE;

						//ж��·��
						RegQueryValueEx(hSubKey, m_KeyNameUniPath, 0, &dwType, (LPBYTE)SoftInfo.szSoftUniPath, &dwNameLen);
						dwNameLen = MAXBYTE;

						//��СΪDWORDֵ
						RegQueryValueEx(hSubKey, m_KeyNameSize, 0, &dwType, (LPBYTE)&dwSoftSize, &dwNameLen);
						dwNameLen = MAXBYTE;

						//ת���ļ���С
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
	//��ȡѡ����
	int nSel = m_ListSoftware.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"��ѡ������");
	}
	//��ȡ·��
	CString SoftIniPath = m_vecSoftInfo[nSel].szSoftInsPath;
	if (SoftIniPath.IsEmpty())
	{
		MessageBox(L"��·��ʧ�ܣ�");
	}
	else
	{
		ShellExecute(NULL, _T("open"), _T("explorer.exe"), SoftIniPath, NULL, SW_SHOWNORMAL);
	}
}

void SoftWaresDlg::UnistallSoft()
{
	//��ȡѡ����
	int nSel = m_ListSoftware.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"��ѡ������");
	}
	//��ȡ·��
	CString SoftUniPath = m_vecSoftInfo[nSel].szSoftUniPath;
	if (SoftUniPath.IsEmpty())
	{
		MessageBox(L"ж��ʧ�ܣ�");
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ListSoftware.AddColumn(5, L"����", 350, L"������", 150, L"��װ����", 80, L"��С", 80, L"�汾", 150);

	//��ȡ�����Ϣ
	UpdateSoftInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
