// gtl_test_socketclient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../../gtl/socket.h"

int _tmain(int argc, _TCHAR* argv[])
{
	gtl::Socket::startup();

	gtl::SocketClient sc;
	gtl::SessionSharedPtr ssp = sc.connect("127.0.0.1", 6666);

	int ok = -1;
	int cmd = gtl::Session::BEGINSESSION;
	ssp->send((const char *)&cmd, sizeof(int),0);
	
	ssp->receive((char*)&ok, sizeof(int), 0);
	if (ok == gtl::Session::OKAY)
	{
		std::cout << "开始会话\n";
	}
////////////////////////////////////////////////////////////
	cmd = gtl::Session::RANGEQUERY;
	ssp->send((const char *)&cmd, sizeof(int), 0);
	ssp->receive((char*)&ok, sizeof(int), 0);
	if (ok == gtl::Session::OKAY)
	{
		std::cout << "开始范围查询\n";
	}
	
	//发送参数
	char buf[256];
	ssp->send(buf, 256, 0);

	//接收查询结果
	int result = ssp->receive(buf, 256, 0);
////////////////////////////////////////////////////////////////
	cmd = gtl::Session::KNNQUERY;
	ssp->send((const char *)&cmd, sizeof(int), 0);
	ssp->receive((char*)&ok, sizeof(int), 0);
	if (ok == gtl::Session::OKAY)
	{
		std::cout << "开始邻近查询\n";
	}

	//发送参数
	
	ssp->send(buf, 256, 0);

	//接收查询结果
	result = ssp->receive(buf, 256, 0);
//////////////////////////////////////////////////////////////////
	cmd = gtl::Session::KNNQUERY;
	ssp->send((const char *)&cmd, sizeof(int), 0);
	ssp->receive((char*)&ok, sizeof(int), 0);
	if (ok == gtl::Session::OKAY)
	{
		std::cout << "开始邻近查询\n";
	}

	//发送参数

	ssp->send(buf, 256, 0);

	//接收查询结果
	result = ssp->receive(buf, 256, 0);
////////////////////////////////////////////////////////////////////
	cmd = gtl::Session::ENDSESSION;
	ssp->send((const char *)&cmd, sizeof(int), 0);
	ssp->receive((char*)&ok, sizeof(int), 0);
	if (ok == gtl::Session::OKAY)
	{
		std::cout << "结束会话\n";
	}

	gtl::Socket::cleanup();
	return 0;
}

