// ServerApp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <winsock2.h>
#include "dpsi.h"
#pragma comment(lib,"ws2_32.lib") 


const int BACKLOG = 32;
int _tmain(int argc, _TCHAR* argv[])
{
	


	//step 1: initial winsock lib
	WORD wVersionRequested = MAKEWORD(2,2);
	WSADATA wsadata;
	int err = WSAStartup(wVersionRequested, &wsadata);
	if (err != 0)
		return -1;
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2){
		WSACleanup();
		return -1;
	}
		
	//step 2: create socket for internetwork: UDP, TCP, etc.
	SOCKET sockServer = socket(AF_INET, SOCK_STREAM, 0);
	//step 3: bind socket
	sockaddr_in addrServer;
	// for all IP
	addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY); 
	addrServer.sin_family = AF_INET;
	// listen port 
	addrServer.sin_port = 6666;

	err= bind(sockServer, (const sockaddr *)&addrServer, sizeof(sockaddr));
	if (err != 0){
		WSACleanup();
		return -1;
	}
	//step 4: listen
	err=listen(sockServer, BACKLOG);
	if (err != 0){
		WSACleanup();
		return -1;
	}
	//step 5: wating for the connection
	sockaddr_in addrClient;
	int len = sizeof(sockaddr_in);
	while (1){

		SOCKET sockConn = accept(sockServer, (sockaddr*)&addrClient, &len);

		char sendBuf[50];

		sprintf_s(sendBuf, "Welcome %s to here!", inet_ntoa(addrClient.sin_addr));

		send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);

		char recvBuf[50];

		recv(sockConn, recvBuf, 50, 0);

		printf("%s\n", recvBuf);

		closesocket(sockConn);

	}
	return 0;
}

