// ClientApp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Winsock2.h> 
#pragma comment(lib,"ws2_32.lib") 
int _tmain(int argc, _TCHAR* argv[])
{
	//step 1: initial winsock lib
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsadata;
	int err = WSAStartup(wVersionRequested, &wsadata);
	if (err != 0){
		WSACleanup();
		return -1;
	}
		
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2){
		WSACleanup();
		return -1;
	}

	//step 2: create socket for internetwork: UDP, TCP, etc.
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0); 
	//step 3: connect to server
	sockaddr_in addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = 6666;
	err=connect(sockClient, (const sockaddr*)&addrSrv, sizeof(sockaddr));
	if (err != 0){
		WSACleanup();
		return -1;
	}
	//step 4: send information to server
	send(sockClient, "hello", strlen("hello") + 1, 0);
	char recvBuf[50];
	recv(sockClient, recvBuf, 50, 0);
	printf("%s\n", recvBuf);
	//step 5: close socket and cleanup
	closesocket(sockClient);
	WSACleanup();
}

