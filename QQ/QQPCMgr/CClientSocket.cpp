#include "stdafx.h"
#include "CClientSocket.h"
//Sock�������ظ���飻�ŵ����ϱ�;
#include <WinSock2.h>
#include <windows.h>
#include <WS2tcpip.h>
//�����׽��ֿ�;
#pragma comment(lib,"ws2_32.lib")


CClientSocket::CClientSocket()
{

}


CClientSocket::~CClientSocket()
{
}

//****************************************************************
// Brief 	: ��ʼ���׽���
// Method	: ConnectServer
// FullName	: CClientSocket::ConnectServer
// Access	: public 
// Returns	: bool
// Parameter: char * szIP
// Parameter: WORD port
//****************************************************************

bool CClientSocket::ConnectServer(char *szIP, WORD port)
{
	//��ʼ���׽���;
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		return false;
	}

	//����汾
	if (LOBYTE(wsd.wVersion) != 2 || HIBYTE(wsd.wHighVersion) != 2)
	{
		WSACleanup();
		return false;
	}

	//�����׽���;
	m_sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sClient == INVALID_SOCKET)
	{
		WSACleanup();
		return false;
	}

	//���÷�������ַ;
	SOCKADDR_IN serverAddr = {};
	serverAddr.sin_family = AF_INET;
	//serverAddr.sin_addr.s_addr = inet_addr(szIP);
	inet_pton(AF_INET,szIP,&(serverAddr.sin_addr.S_un.S_addr));
	serverAddr.sin_port = htons((short)port);

	//���ӷ����
	int nRetValue = connect(m_sClient, (sockaddr*)&serverAddr, sizeof(SOCKADDR_IN));
	if (nRetValue == SOCKET_ERROR)
	{
		closesocket(m_sClient);
		m_sClient = NULL;
		WSACleanup();
		return false;
	}

	return true;
}

//****************************************************************
// Brief 	: ��Ϣ����
// Method	: Recv
// FullName	: CClientSocket::Recv
// Access	: public 
// Returns	: char *
//****************************************************************

char * CClientSocket::Recv()
{
	//������Ϣ;
	CHATSEND ct = {};
	if (SOCKET_ERROR == recv(m_sClient, (char*)&ct, sizeof(CHATSEND), NULL))
	{
		printf("recv error!\n");
		return nullptr;
	}
	//ת������
	m_pObjChatRecv = &ct;

	//�ַ���Ϣ
	switch (ct.m_type)
	{
	case VIRUSINFO:
		break;
	case WHITE:
		break;
	case BLACK:
		break;
	case QUERY:
		break;
	case SUBMIT:
		break;
	default:
		break;
	}

	return nullptr;
}

//****************************************************************
// Brief 	: ������Ϣ
// Method	: Send
// FullName	: CClientSocket::Send
// Access	: public 
// Returns	: bool
// Parameter: CHATPURPOSE purpose
// Parameter: char * bufSend
// Parameter: DWORD dwLen
//****************************************************************

bool CClientSocket::Send(DATAPURPOSE purpose, char* bufSend, DWORD dwLen)
{
	//�ַ���Ϣ
	switch (purpose)
	{
	case VIRUSINFO:
	case WHITE:
	case BLACK:
		SendUpdataDataBase(bufSend, dwLen);
		break;
	case QUERY:
		SendQuery(bufSend, dwLen);
		break;
	case SUBMIT:
		SendSubmit(bufSend, dwLen);
		break;
	default:
		break;
	}

	return true;
}

//�ر��׽���
bool CClientSocket::Close()
{
	closesocket(m_sClient);
	WSACleanup();
	return true;
}



//��ѯMD5
void CClientSocket::SendQuery(char * bufSend, DWORD dwLen)
{
	CHATSEND ct = { QUERY };
	char * pwd = nullptr;
	//��������
	memcpy_s(ct.m_content.fMD5.szMD5, strlen(bufSend) + 1, bufSend, strlen(bufSend) + 1);
	send(m_sClient, (char *)&ct, sizeof(ct), NULL);
}


//��ȡ���ݿ�
void CClientSocket::SendUpdataDataBase(char * bufSend, DWORD dwLen)
{
	CHATSEND ct = { (DATAPURPOSE)dwLen };
	memcpy_s(ct.m_content.buf, strlen(bufSend), bufSend, strlen(bufSend));
	send(m_sClient, (char *)&ct, sizeof(ct), NULL);
}

void CClientSocket::SendSubmit(char * bufSend, DWORD dwLen)
{
	CHATSEND ct = { (DATAPURPOSE)dwLen };
	memcpy_s(ct.m_content.fMD5.szMD5, strlen(bufSend), bufSend, strlen(bufSend));
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}

