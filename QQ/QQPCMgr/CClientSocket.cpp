#include "stdafx.h"
#include "CClientSocket.h"
//Sock不进行重复检查；放到最上边;
#include <WinSock2.h>
#include <windows.h>
#include <WS2tcpip.h>
//载入套接字库;
#pragma comment(lib,"ws2_32.lib")


CClientSocket::CClientSocket()
{

}


CClientSocket::~CClientSocket()
{
}

//****************************************************************
// Brief 	: 初始化套接字
// Method	: ConnectServer
// FullName	: CClientSocket::ConnectServer
// Access	: public 
// Returns	: bool
// Parameter: char * szIP
// Parameter: WORD port
//****************************************************************

bool CClientSocket::ConnectServer(char *szIP, WORD port)
{
	//初始化套接字;
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		return false;
	}

	//检查库版本
	if (LOBYTE(wsd.wVersion) != 2 || HIBYTE(wsd.wHighVersion) != 2)
	{
		WSACleanup();
		return false;
	}

	//创建套接字;
	m_sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sClient == INVALID_SOCKET)
	{
		WSACleanup();
		return false;
	}

	//设置服务器地址;
	SOCKADDR_IN serverAddr = {};
	serverAddr.sin_family = AF_INET;
	//serverAddr.sin_addr.s_addr = inet_addr(szIP);
	inet_pton(AF_INET,szIP,&(serverAddr.sin_addr.S_un.S_addr));
	serverAddr.sin_port = htons((short)port);

	//连接服务端
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
// Brief 	: 消息接收
// Method	: Recv
// FullName	: CClientSocket::Recv
// Access	: public 
// Returns	: char *
//****************************************************************

char * CClientSocket::Recv()
{
	//接收消息;
	CHATSEND ct = {};
	if (SOCKET_ERROR == recv(m_sClient, (char*)&ct, sizeof(CHATSEND), NULL))
	{
		printf("recv error!\n");
		return nullptr;
	}
	//转存数据
	m_pObjChatRecv = &ct;

	//分发消息
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
// Brief 	: 发送消息
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
	//分发消息
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

//关闭套接字
bool CClientSocket::Close()
{
	closesocket(m_sClient);
	WSACleanup();
	return true;
}



//查询MD5
void CClientSocket::SendQuery(char * bufSend, DWORD dwLen)
{
	CHATSEND ct = { QUERY };
	char * pwd = nullptr;
	//处理数据
	memcpy_s(ct.m_content.fMD5.szMD5, strlen(bufSend) + 1, bufSend, strlen(bufSend) + 1);
	send(m_sClient, (char *)&ct, sizeof(ct), NULL);
}


//获取数据库
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

