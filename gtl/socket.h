/*
* Geosciences Template Library
*
* Copyright (c) 2013
* Zhenwen He (zwhe@hotmail.com)
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Zhenwen He  makes no  representations
* about the suitability of this software for any purpose.
* It is provided "as is" without express or implied warranty.
*/
#pragma once

#include "object.h"

#ifdef WIN32
#include <winsock2.h> 
#pragma comment(lib,"ws2_32.lib") 
typedef  SOCKET socket_type;
#else
typedef  uint64_t socket_type;
#endif

begin_gtl_namespace

class SocketServer;
class SocketClient;
class Task;
class Session;


typedef std::shared_ptr<Session> SessionSharedPtr;
typedef std::shared_ptr<Task> TaskSharedPtr;

class Socket{
protected:
	socket_type  _socket; 
	int _af;
	int _type;
	int _protocol;
public:
	Socket();
	Socket(int af, int type, int protocol);
	Socket(const Socket & s);
	Socket operator = (const Socket & s);
	virtual ~Socket();

	inline bool isOpen();
	inline bool open();
	inline bool open(int af, int type, int protocol);
	inline bool close();
public:
	static int startup();
	static int cleanup();

	friend class  SocketServer;
	friend class SocketClient;
};



class Session  
{
	socket_type _connSocket;
	std::map<int, TaskSharedPtr> _tasks;
	std::atomic<bool> _task_loop;
public:
	static const int OKAY = 0;

	static const int BEGINSESSION = 1;
	static const int ENDSESSION = -1;
	
	static const int RANGEQUERY = 2;
	static const int KNNQUERY = 3;
	static const int TRAJACTORYQUERY = 4;
public:
	struct Deleter
	{
		void  operator ()(Session * p){ p->close(); delete p; }
	};
public:
	static std::shared_ptr<Session> create(socket_type connSocket);
	inline void  run();
	inline int send(const char * buf, int len, int flags = 0);
	inline int receive(char * buf, int len, int flags = 0);
	inline int close();	
	inline int sendOkay();
public:
	inline void addTask(TaskSharedPtr& p);
	inline void addTaskList(std::list<TaskSharedPtr>& p);
	inline TaskSharedPtr removeTask(int cmdid);
	inline void setTaskLoop(bool b){ _task_loop = b; }
protected:
	Session(socket_type connSocket);
	Session(const Session& s);
	Session operator=(const Session& s);
	virtual ~Session();
	static void  thread_procedure(Session* p);
protected:
	//override this function to do other things
	virtual void runTasks();
	
};

class Task
{
protected:
	int _cmdid;
	Session * _session;
	virtual void run() = 0;
public:
	friend class Session;
};


class SocketServer
{
	sockaddr_in  _sockaddr ;
	Socket       _socket;
	int          _clients;//允许连接的客户端数

	std::list<SessionSharedPtr> _sessions;
	std::thread *               _listenthread;
	std::atomic<bool>           _always_run;
	std::list<TaskSharedPtr>    _tasks;
public:
	SocketServer();
	virtual ~SocketServer();
	virtual bool start(unsigned short port, int backlog = 5);
	virtual bool stop();

	inline void addTasks(const std::list<TaskSharedPtr> & ts)
	{
		_tasks.insert(_tasks.end(), ts.begin(), ts.end());
	}
protected:
	//采用单独的线程监听，接收到一个连接则启动一个新的会话线程
	static void listen_thread_function(SocketServer * s);

};

class SocketClient
{
	sockaddr_in  _sockaddr;
	Socket       _socket; 
public:
	SocketClient();
	virtual ~SocketClient();
	virtual SessionSharedPtr connect(const char * svrip, unsigned short port);

};


class SocketPeer :public SocketServer, public SocketClient
{

};

end_gtl_namespace

#include "socket.inl"
#include "session.inl"
#include "socketServer.inl"
#include "socketClient.inl"