// AntiScanDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QQPCMgr.h"
#include "AntiScanDlg.h"
#include "afxdialogex.h"


// AntiScanDlg �Ի���

IMPLEMENT_DYNAMIC(AntiScanDlg, CDialogEx)

AntiScanDlg::AntiScanDlg(CString FilePath, DWORD dwType, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ANTISCAN, pParent)
{
	m_dwScanType = dwType;
	m_FilePath = FilePath;
	m_FolderPath = FilePath;
}

AntiScanDlg::~AntiScanDlg(){}

void AntiScanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ANTIVIRUS, m_ListAntiScan);
	DDX_Control(pDX, IDC_TEXT_SCANINFO, m_TextScanInfo);
}


void AntiScanDlg::OnBnClickedButtonAntiPause()
{
	//����δ��ʼ������ͣ
	if (!hScanThread)
	{
		MessageBox(L"ɨ�������ѽ�����");
	}


	m_IsPause = !m_IsPause;

	//���İ�ť״̬
	if (m_IsPause)
	{
		GetDlgItem(IDC_BUTTON_ANTI_PAUSE)->SetWindowTextW(L"����ɨ��");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_ANTI_PAUSE)->SetWindowTextW(L"��ͣɨ��");
	}
}

void AntiScanDlg::OnBnClickedButtonAntiCancle()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_IsCancle = true;
}

void AntiScanDlg::OnBnClickedButtonAntiOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//δ������Ҫ������Ŀ
	if (m_vecFilePath.size() == 0)
	{
		MessageBox(L"���޿ɴ�����Ŀ��");
		return;
	}

	//ɾ������
	for (DWORD i = 0; i < m_vecFilePath.size(); i++)
	{
		DeleteFile(m_vecFilePath[0]);
	}
	MessageBox(L"������ɣ�");
}

BEGIN_MESSAGE_MAP(AntiScanDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ANTI_PAUSE, &AntiScanDlg::OnBnClickedButtonAntiPause)
	ON_BN_CLICKED(IDC_BUTTON_ANTI_CANCLE, &AntiScanDlg::OnBnClickedButtonAntiCancle)
	ON_BN_CLICKED(IDC_BUTTON_ANTI_OK, &AntiScanDlg::OnBnClickedButtonAntiOk)
END_MESSAGE_MAP()


// AntiScanDlg ��Ϣ�������


void AntiScanDlg::AntiScan()
{
	//����ɨ�����ͷַ�
	switch (m_dwScanType)
	{
	case 0:
		//����ɨ��
		FastScan();
		break;
	case 1:
		//ȫ������ɨ��
		ScanAll();
		break;
	case 2:
		//�ļ�ɨ��
		ScanFile();
		break;
	case 3:
		//����
		ScanFloder(m_FolderPath);
		break;
	case 4:
		break;
	default:
		break;
	}
}

void AntiScanDlg::FastScan()
{
	//ɨ�����
	//ScanProcess();
	//ɨ��ϵͳ�ļ���
	ScanFloder(m_FolderPath);

	m_TextScanInfo.SetWindowTextW(L"ɨ�����");
	MessageBox(L"ɨ�����!\r\n�봦��");
}

void AntiScanDlg::ScanAll()
{

	//ɨ�����
	ScanProcess();

	MessageBox(L"ɨ�����!\r\n�봦��");
}

void AntiScanDlg::ScanFile()
{
	//ת���ļ���;
	CStringA FinalFileName = CW2A(m_FilePath.GetBuffer(), CP_THREAD_ACP);

	//��ȡMD5
	char* szMD5 = md5FileValue(FinalFileName.GetBuffer());
	m_ScanMd5 = CA2W(szMD5, CP_THREAD_ACP);

	m_TextScanInfo.SetWindowTextW(L"ɨ�����");
	//�Ա�MD5
	if (DataBaseCMP(m_ScanMd5))
	{
		m_vecFilePath.push_back(m_FilePath);
		m_ListAntiScan.AddItem(3, m_FilePath, m_ScanMd5, L"�����ļ�");
		MessageBox(L"�����ļ�!\r\n�봦��");
	}
	else
	{
		m_ListAntiScan.AddItem(3, m_FilePath, m_ScanMd5, L"�����ļ�");
		MessageBox(L"�����ļ�!");
	}
}

void AntiScanDlg::ScanFloder(CString FloderPath)
{
		//ȡ������
		if (m_IsCancle)
		{
			//��������

			return;
		}

		//��ͣ����
		while (m_IsPause)
		{
		}

		//��·���˳�;
		if (FloderPath.IsEmpty())
		{
			m_IsCancle = true;
			return;
		}

		//�򿪼�;
		WIN32_FIND_DATA wsd = {};
		HANDLE hFind = FindFirstFile(FloderPath, &wsd);
		FloderPath += L"\\*";
		hFind = FindFirstFile(FloderPath, &wsd);

		if (hFind == INVALID_HANDLE_VALUE)
		{
			return;
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
				CString NextDir = FloderPath.Left(FloderPath.GetLength() - 1);
				NextDir += wsd.cFileName;
				ScanFloder(NextDir);
			}
			//ת���ļ�����ʽ;
			TCHAR* szBuff = wsd.cFileName;
			//��ȡ��׺��;
			TCHAR* szExName = PathFindExtension(szBuff);
			//�ԱȺ�׺��;

			//�ԱȺ�׺�����˴�Ҳ��ѡ���ļ����ģ���е��ļ����ͶԱ�
			if (!wcscmp(szExName, L".exe") ||
				!wcscmp(szExName, L".EXE"))
			{
				//ƴ���ļ���;
				CString m_ScanFileFath = FloderPath.Left(FloderPath.GetLength() - 1);
				m_ScanFileFath += wsd.cFileName;
				CStringA FinalFileName = CW2A(m_ScanFileFath.GetBuffer(), CP_THREAD_ACP);

				//��ȡMD5
				char* szMD5 = md5FileValue(FinalFileName.GetBuffer());
				m_ScanMd5 = CA2W(szMD5, CP_THREAD_ACP);

				//����ɨ��״̬
				CString strScanInfo = L"����ɨ�裺";
				strScanInfo += m_ScanFileFath;
				m_TextScanInfo.SetWindowTextW(strScanInfo);

				//�Ա�MD5
				if (DataBaseCMP(m_ScanMd5))
				{
					m_vecFilePath.push_back(m_ScanFileFath);
					m_ListAntiScan.AddItem(3, m_ScanFileFath, m_ScanMd5, L"�����ļ�");
				}
			}

		} while (FindNextFile(hFind, &wsd));

		FindClose(hFind);

		return;
}

void AntiScanDlg::ScanProcess()
{
		HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (hSnapShot == INVALID_HANDLE_VALUE){
			MessageBox(L"HELP ��������ʧ�ܣ�");
			return;
		}

		//��ʼ��������Ϣ�ṹ��;
		PROCESSENTRY32 pi = { sizeof(PROCESSENTRY32) };
		if (!Process32First(hSnapShot, &pi))
		{
			MessageBox(L"��ȡ������Ϣʧ��");
			return;
		}
		do
		{
			TCHAR *szBuf[5] = { new TCHAR[MAX_PATH], new TCHAR[6],  new TCHAR[6],  new TCHAR[6], new TCHAR[MAX_PATH], };
			szBuf[0] = pi.szExeFile;

			//��Ҫ��ȡ·���Ľ���
		/*	HANDLE hPro = OpenProcess(PROCESS_QUERY_INFORMATION
				| PROCESS_QUERY_LIMITED_INFORMATION,
				FALSE, pi.th32ProcessID);*/
			HANDLE hPro = OpenProcess(PROCESS_TERMINATE | PROCESS_ALL_ACCESS,
				FALSE, pi.th32ProcessID);
			
			//��ȡ��������·��;
			if (hPro)
			{
				DWORD dwSize = MAX_PATH;
				QueryFullProcessImageName(hPro, NULL, szBuf[4], &dwSize);

			}
			else
			{
				szBuf[4] = L"��Ȩ����";
			}

			CString  FileFathW = szBuf[4];
			CStringA FinalFileName = CW2A(szBuf[4], CP_THREAD_ACP);
			//��ȡMD5
			char* szMD5 = md5FileValue(FinalFileName.GetBuffer());
			m_ScanMd5 = CA2W(szMD5, CP_THREAD_ACP);

			CString strScanInfo = L"����ɨ����̣�";
			strScanInfo += FileFathW;
			m_TextScanInfo.SetWindowTextW(strScanInfo);

			//�Ա�MD5
			if (DataBaseCMP(m_ScanMd5))
			{

				if (TerminateProcess(hPro, 0))
				{
					MessageBox(L"���ֿ��ɽ���!\r\n�ѽ�������\r\n");
				}
				else
				{
					MessageBox(L"���ֿ��ɽ���!\r\n����ʧ��!\r\n���ֶ�����");
				}
				m_vecFilePath.push_back(FileFathW);
				m_ListAntiScan.AddItem(3, FileFathW, m_ScanMd5, L"��������");
			}
			CloseHandle(hPro);

		} while (Process32Next(hSnapShot, &pi));

}

void AntiScanDlg::ReadDataBase()
{
	//��ȡ������
	CString strDatabase = szFileName[0];
	CStdioFile sfDatabase;
	if (sfDatabase.Open(strDatabase, CFile::modeRead))
	{
		CString temp;
		//ѭ����ȡ
		while (sfDatabase.ReadString(temp))
		{
			m_vecVirusDatabase.push_back(temp);
		}
		sfDatabase.Close();
	}
	else
	{
		MessageBox(L"��ȡ���ز�����ʧ�ܣ�");
		m_dwScanType = 4;
		return;
	}

	//��ȡ������
	CString strWhite = szFileName[1];
	CStdioFile sfWhite;
	if (sfWhite.Open(strWhite, CFile::modeRead))
	{
		CString temp;
		//ѭ����ȡ
		while (sfWhite.ReadString(temp))
		{
			m_vecWhite.push_back(temp);
		}
		sfWhite.Close();
	}

	//��ȡ������
	CString strBlack = szFileName[2];
	CStdioFile sfBlack;
	if (sfBlack.Open(strBlack, CFile::modeRead))
	{
		CString temp;
		//ѭ����ȡ
		while (sfBlack.ReadString(temp))
		{
			m_vecBlack.push_back(temp);
		}
		sfBlack.Close();
	}
}

bool AntiScanDlg::DataBaseCMP(CString strMD5)
{
	//�ԱȰ�����
	for (DWORD i = 0; i < m_vecWhite.size(); i++)
	{
		if (strMD5 == m_vecWhite[i])
		{
			return false;
		}
	}

	//�ԱȺ�����
	for (DWORD i = 0; i < m_vecBlack.size(); i++)
	{
		if (strMD5 == m_vecBlack[i])
		{
			return true;
		}
	}

	//�ԱȲ�����
	for (DWORD i = 0; i < m_vecVirusDatabase.size(); i++)
	{
		if (strMD5 == m_vecVirusDatabase[i])
		{
			return true;
		}
	}
	return false;
}

UINT AntiScanDlg::MyThreadScan(LPVOID lpParam)
{
	AntiScanDlg* pAntiScan = (AntiScanDlg*)lpParam;

	pAntiScan->AntiScan();

	pAntiScan->hScanThread = NULL;

	return 0;
}

BOOL AntiScanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ListAntiScan.AddColumn(3, L"·��", 350, L"MD5", 250, L"����", 100);

	//��ȡ���ݿ�
	ReadDataBase();
	m_IsCancle = false;
	m_IsPause = false;
	//����ɨ���߳�
	hScanThread = AfxBeginThread(MyThreadScan, this);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
