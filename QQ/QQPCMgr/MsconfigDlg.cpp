// MsconfigDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QQPCMgr.h"
#include "MsconfigDlg.h"
#include "afxdialogex.h"


// MsconfigDlg �Ի���

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��ȡ�����˵�;
	CMenu* pMenu = new CMenu;
	pMenu->LoadMenuW(IDR_MENU1);

	CMenu* pSub = pMenu->GetSubMenu(2);

	//��ȡ���λ��;
	CPoint point;
	GetCursorPos(&point);

	//����;
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
	ON_CONTROL_RANGE(BN_CLICKED, ID_32777, ID_32779, OnMenuModifyData)	//�Ҽ��˵���Ϣ����;
END_MESSAGE_MAP()


// MsconfigDlg ��Ϣ�������


void MsconfigDlg::DeleRun()
{
		//��ȡѡ����
		int nSel = m_ListRun.GetSelectionMark();
		if (nSel < 0)
		{
			MessageBox(L"��ѡ������");
		}
		//��ȡ·��
		CString strName = m_ListRun.GetItemText(nSel, 0);
		CString strRoot = m_ListRun.GetItemText(nSel, 2);
		CString strSub = m_ListRun.GetItemText(nSel, 3);
		HKEY hRoot;

		//��ȡ����
		if (strRoot == m_Root1)
		{
			hRoot = m_hRootKey1;
		}
		else
		{
			hRoot = m_hRootKey2;
		}

		//��ʱ����
		DWORD dwType = 0;
		DWORD dwBuffSize = MAXBYTE;
		DWORD dwKeySize = MAXBYTE;
		WCHAR szValueNme[MAXBYTE] = { 0 };
		WCHAR szValueKey[MAXBYTE] = { 0 };
		HKEY hKey = NULL;

		//�򿪼����˴���Ҫ����Ȩ��
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

	//�����Ӽ���ϻ�ȡ������
	GetRunInfo(m_hRootKey1, m_SubKey1);

	GetRunInfo(m_hRootKey1, m_SubKey2);

	GetRunInfo(m_hRootKey2, m_SubKey1);
}

void MsconfigDlg::AddRun()
{
		CFileDialog dlg(TRUE, _T("exe"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("��ִ�г���|*.exe|�����ļ�|*||"));
		if (dlg.DoModal() == IDOK) {

			m_Key = dlg.GetPathName();

			//��ȡ�ļ���
			int nPos = m_Key.ReverseFind(L'\\');
			m_Name = m_Key.Right(m_Key.GetLength() - nPos - 1);

			HKEY hKey = NULL;

			//����ע����ֵ
			RegCreateKeyEx(m_hRootKey1, m_SubKey1, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);

			if (ERROR_SUCCESS == RegSetValueEx(hKey, m_Name, 0, REG_SZ,
				(LPBYTE)m_Key.GetBuffer(), (m_Key.GetLength() + 1) * sizeof(TCHAR)))
			{
				MessageBox(L"��ӳɹ���");
			}
			else
			{
				MessageBox(L"���ʧ�ܣ�");
			}
			RegCloseKey(hKey);

			//ˢ��
			UpDataRun();
		}
}

void MsconfigDlg::GetRunInfo(HKEY hRootKey, CString Path)
{
		//�ж�����
		CString strRoot;
		if (hRootKey == HKEY_LOCAL_MACHINE)
		{
			strRoot = m_Root1;
		}
		else
		{
			strRoot = m_Root2;
		}

		//�򿪼�
		HKEY hKey = NULL;
		if (ERROR_SUCCESS != RegOpenKeyEx(hRootKey, Path, 0,
			KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | KEY_WOW64_64KEY, &hKey))
		{
			return;
		}

		//��ѯ����
		int i = 0;
		DWORD dwType = REG_SZ | REG_EXPAND_SZ;
		//ѭ����ȡ

		while (true)
		{
			DWORD dwBuffSize = MAXBYTE;
			DWORD dwKeySize = MAXBYTE;
			WCHAR szValueNme[MAXBYTE] = { 0 };
			WCHAR szValueKey[MAXBYTE] = { 0 };

			//ö�ټ���
			int rect = RegEnumValue(hKey, i, szValueNme, &dwBuffSize,
				NULL, &dwType, (LPBYTE)szValueKey, &dwKeySize);

			//��ѯ��ֵ
			int rect2 = RegQueryValueEx(hKey, szValueNme, 0,
				&dwType, (LPBYTE)szValueKey, &dwKeySize);

			//��Ͻ���
			if (rect == ERROR_NO_MORE_ITEMS)
			{
				break;
			}

			//���
			m_Name = szValueNme;
			m_Key = szValueKey;
			m_ListRun.AddItem(4, m_Name, m_Key, strRoot, Path);

			i++;

		}
		//��ϼ�
		RegCloseKey(hKey);
}

BOOL MsconfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ListRun.AddColumn(4, L"����", 100, L"��ֵ", 150, L"����", 140, L"�Ӽ�", 250);

	//��Ҫ������·��
	m_SubKey1 = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	m_SubKey2 = L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Run";
	m_Root1 = L"HKEY_LOCAL_MACHINE";
	m_Root2 = L"HKEY_CURRENT_USER";
	m_hRootKey1 = HKEY_LOCAL_MACHINE;
	m_hRootKey2 = HKEY_CURRENT_USER;

	//��ȡ����
	UpDataRun();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
