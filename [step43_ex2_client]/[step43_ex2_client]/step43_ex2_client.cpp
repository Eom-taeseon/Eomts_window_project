#include "pch.h"
#include <iostream>

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
	// addr.sin_addr.s_addr = inet_addr("127.0.0.1");	// "loopback"
	int nRet = connect(client, (LPSOCKADDR)&addr, sizeof(addr));
	if (nRet == SOCKET_ERROR)
	{
		closesocket(client);
		return 1;
	}
	cout << "The Client is connected to the server." << endl;
	int nR, nS;
	char buf[BUFSIZE];

	do {
		cout << "[client] : ";
		cin.getline(buf, BUFSIZE);
		if (strcmp(buf, "bye") == 0)
			break;
		nS = send(client, buf, strlen(buf) + 1, 0);
		nR = recv(client, buf, BUFSIZE, 0);
		cout << "[server] : " << buf << endl;
	} while (nR > 0);

	closesocket(client);	// 서버의 recv 반환값이 0
	WSACleanup();
	return 0;
}