#include "pch.h"
#include <iostream>
#include <locale>	// setlocale

#include <WinSock2.h>
#include <WS2tcpip.h>	// inet_pton
#pragma comment(lib, "Ws2_32.lib")

#define BUFSIZE 256
using namespace std;
int main()
{
	WSADATA wsaData;
	SOCKADDR_IN addr;
	SOCKET client;

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	// IPv4, TCP stream, protocol
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = 10000;
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
	// addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	int nRet = connect(client, (LPSOCKADDR)&addr, sizeof(addr));
	if (nRet == SOCKET_ERROR)
	{
		closesocket(client);
		return 1;
	}
	wcout << L"The client is connected to the server." << endl;

	setlocale(LC_ALL, "");	// 유니코드 한글 출력을 위해
	int nR, nS;
	WCHAR buf[BUFSIZE];
	do {
		wcout << L"[client] : ";
		wcin.getline(buf, BUFSIZE);
		if (wcscmp(buf, L"bye") == 0)
			break;
		nS = send(client, (char*)buf, (wcslen(buf) + 1) * 2, 0);
		wcout << L"nS = " << nS << endl;
		nR = recv(client, (char*)buf, BUFSIZE, 0);
		wcout << L"nR" << nR << endl;

		wcout << L"[server] : " << buf << endl;
	} while (nR > 0);
	
	closesocket(client);	// 서버의 recv의 반화값이 0
	WSACleanup();
	return 0;
}
