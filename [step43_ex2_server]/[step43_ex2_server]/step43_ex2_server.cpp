#include "pch.h"
#include "iostream"

// # define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>	
#include <WS2tcpip.h>	// inet_pton
#pragma comment(lib, "Ws2_32.lib")	// WinSock version2 사용

using namespace std;
#define BUFSIZE		256
int main()
{
	WSADATA wsaData;
	SOCKADDR_IN addr;
	SOCKET server;
	WSAStartup(MAKEWORD(2, 2, ), &wsaData);	// 윈속 버전 2.2 chrlghk

	// IPv4, TCP stream, protocol
	server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	// IPv4, TCP 서버 소켓 server 생성

	ZeroMemory(&addr, sizeof(addr));
	
	// SOCKADDR_IN 구조체인 addr에 IPv4 주소체계, 포트번호, 주소를 초기화
	addr.sin_family = AF_INET;	
	addr.sin_port = 10000;
	inet_pton(AF_INET, "loopback", &addr.sin_addr.s_addr);	// "loopback"으로 서버 주소를 addr.sin_addr.s_addr에 저장
	// addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// bind 함수로 server에 설정
	bind(server, (LPSOCKADDR)&addr, sizeof(addr));

	// listen 함수는 server 소켓을 client의 연결 접속을 대기시킴. 만약 error가 발생하면 조건문 발동, 에러메시지를 남기고 종료
	if (listen(server, SOMAXCONN) == SOCKET_ERROR)	
	{
		printf("Listen error: %ld\n", WSAGetLastError());
		closesocket(server);
		WSACleanup();
		return 1;
	}
	cout << " The Server is Listening... " << endl;	// server 연결 접속 완료

	SOCKET client;
	client = accept(server, NULL, NULL);	// server 연결 접속 완료시 accept 함수에 의해 승인하고, client와 통신할 socket을 반환하여 client에 저장
	if (client == INVALID_SOCKET)	// client와 통신할 socket 확인
	{
		closesocket(server);
		WSACleanup();
		return 1;
	}
	cout << "A Client is Accepted." << endl;
	closesocket(server);	// 1개 클라이언트만 받고 서버 닫음

	// 데이터 송수신
	int nR, nS;
	char buf[BUFSIZE];
	do {
		nR = recv(client, buf, BUFSIZE, 0);
		nS = send(client, buf, nR, 0);	// echo back
		if (nS == SOCKET_ERROR)
			break;
	}while (nR > 0);

	//shutdown(client, SD_SEND);
	closesocket(client);
	WSACleanup();
	return 0;
}