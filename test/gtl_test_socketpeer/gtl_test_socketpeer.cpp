// gtl_test_socketpeer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "stdafx.h"
#include "../../gtl/socket.h"

class BeginSessionTask : public gtl::Task
{
public:
	BeginSessionTask()
	{
		_cmdid = gtl::Session::BEGINSESSION;
	}
protected:
	virtual void run()
	{
		std::cout << "BEGINSESSION\n";
		this->_session->sendOkay();
	}
};


class EndSessionTask : public gtl::Task
{
public:
	EndSessionTask()
	{
		_cmdid = gtl::Session::ENDSESSION;
	}
protected:
	virtual void run()
	{
		std::cout << "ENDSESSION\n";
		_session->sendOkay();
		_session->setTaskLoop(false);
	}
};

class RangeQueryTask : public gtl::Task
{
public:
	RangeQueryTask()
	{
		_cmdid = gtl::Session::RANGEQUERY;
	}
protected:
	virtual void run()
	{

		std::cout << "RANGEQUERY\n";
		_session->sendOkay();
		//接收参数
		char buf[256];
		_session->receive(buf, 256, 0);
		//DO 查询
		//Sleep(1000 * 60);
		//发送查询结果
		_session->send(buf, 256, 0);
	}
};


class KNNQueryTask : public gtl::Task
{
public:
	KNNQueryTask()
	{
		_cmdid = gtl::Session::KNNQUERY;
	}
protected:
	virtual void run()
	{

		std::cout << "KNNQUERY\n";
		_session->sendOkay();
		//接收参数
		char buf[256];
		_session->receive(buf, 256, 0);
		//DO 查询
		//Sleep(1000 * 60);
		//发送查询结果
		_session->send(buf, 256, 0);
	}
};


class TrajactoryQueryTask : public gtl::Task
{
public:
	TrajactoryQueryTask()
	{
		_cmdid = gtl::Session::TRAJACTORYQUERY;
	}
protected:
	virtual void run()
	{

		std::cout << "TRAJACTORYQUERY\n";
		_session->sendOkay();
		//接收参数
		char buf[256];
		_session->receive(buf, 256, 0);
		//DO 查询
		//Sleep(1000 * 60);
		//发送查询结果
		_session->send(buf, 256, 0);
	}
};



int _tmain(int argc, _TCHAR* argv[])
{

	gtl::Socket::startup();

	gtl::SocketPeer  speer;

	std::list<gtl::TaskSharedPtr> ls;
	ls.push_back(gtl::TaskSharedPtr(new BeginSessionTask()));
	ls.push_back(gtl::TaskSharedPtr(new EndSessionTask()));
	ls.push_back(gtl::TaskSharedPtr(new RangeQueryTask()));
	ls.push_back(gtl::TaskSharedPtr(new KNNQueryTask()));
	ls.push_back(gtl::TaskSharedPtr(new TrajactoryQueryTask()));

	speer.addTasks(ls);

	speer.start(6666);



	gtl::SessionSharedPtr ssp = speer.connect("127.0.0.1", 6666);

	int ok = -1;
	int cmd = gtl::Session::BEGINSESSION;
	ssp->send((const char *)&cmd, sizeof(int), 0);

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

