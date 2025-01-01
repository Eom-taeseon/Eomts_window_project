// step43_ex1.cpp : 이 파일에는 "main" 함수 포함
//
#include "pch.h"
#include <stdio.h>

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

int main()
{
	ADDRINFOA hints;
	PADDRINFOA result;

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
		
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;				// IPv4
	hints.ai_socktype = SOCK_STREAM;		// TCP stream
	hints.ai_protocol = IPPROTO_TCP;		// TCP protocal

	int nRet;
	nRet = getaddrinfo(NULL, "512", &hints, &result);
	// nRet = getaddrinfo("loopback", "512", &hints, &result);
	// nRet = getaddrinfo("127.0.0.1", "512", &hints, &result);
	// nRet = getaddrinfo("www.gooogle.com", "80", &hints, &result);
	if (nRet != 0) {
		printf("getaddrinfo failed with error : %d\n", nRet);
		WSACleanup();
		return 1;
	}

	printf("result -> ai_family = %d\n", result->ai_family);
	printf("result -> ai_socktype = %d\n", result->ai_socktype);
	printf("result -> ai_protocol = %d\n", result->ai_protocol);
	printf("result -> ai_addr -> sa_family = %d\n", result->ai_addr->sa_family);

	/*
	for(int i = 0 ; i < 14; i++)
		printf("result -> ai_addr -> sa_data[%d] = %d\n",
			i, (BYTE)result->ai_arrd->sa_data[i]);
	*/
	USHORT port = MAKEWORD(result->ai_addr->sa_data[0], result->ai_addr->sa_data[1]);
	printf("port = %hu\n", ntohs(port));	// 리틀 엔디아(little-endian)

	// SOCKADDR to SOCKADDR_IN 주소 변환
	SOCKADDR_IN* addrIPv4 = (SOCKADDR_IN*)(result->ai_addr);
	printf("addrIPv4->sin_family = %d\n", addrIPv4->sin_family);
	printf("addrIPv4->sim_port = %hu\n", ntohs(addrIPv4->sin_port));

	// IN_ADDR sin_addr
	printf("addrIPv4->sin_addr = %d\n", addrIPv4->sin_addr.S_un.S_un_b.s_b1);
	printf("addrIPv4->sin_addr = %d\n", addrIPv4->sin_addr.S_un.S_un_b.s_b2);
	printf("addrIPv4->sin_addr = %d\n", addrIPv4->sin_addr.S_un.S_un_b.s_b3);
	printf("addrIPv4->sin_addr = %d\n", addrIPv4->sin_addr.S_un.S_un_b.s_b4);

	freeaddrinfo(result);
	WSACleanup();
	return 0;
}