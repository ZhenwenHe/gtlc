
#ifdef WIN32 

begin_gtl_namespace

void SocketServer::listen_thread_function(SocketServer * ss)
{
	 
	int err = bind(ss->_socket._socket, (const sockaddr *)&(ss->_sockaddr), sizeof(sockaddr));
	if (err != 0){
		WSACleanup();
		return;
	}
	//step 4: listen
	err = listen(ss->_socket._socket, ss->_clients);
	if (err != 0){
		WSACleanup();
		return;
	}
	//step 5: wating for the connection
	sockaddr_in addrClient;
	int len = sizeof(sockaddr_in);
	while (ss->_always_run ){
		SOCKET sockConn = accept(ss->_socket._socket, (sockaddr*)&addrClient, &len);
		if (sockConn != INVALID_SOCKET){
			SessionSharedPtr p = Session::create(sockConn);
			p->addTaskList(ss->_tasks);
			ss->_sessions.push_back(p);
			p->run();
		}	
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
}
SocketServer::SocketServer()
{
	_clients = 5;
	_sockaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	_sockaddr.sin_family = AF_INET;
	// listen port 
	_sockaddr.sin_port = 6666;//Ä¬ÈÏ¼àÌý¶Ë¿Ú

	_socket.open();

	_listenthread = 0;
	_always_run = false;

	//-------------------------
	// Set the socket I/O mode: In this case FIONBIO
	// enables or disables the blocking mode for the 
	// socket based on the numerical value of iMode.
	// If iMode = 0, blocking is enabled; 
	// If iMode != 0, non-blocking mode is enabled.
	u_long iMode = 0;
	ioctlsocket(_socket._socket, FIONBIO, &iMode);
}

bool SocketServer::start(unsigned short port, int backlog)
{
	_sockaddr.sin_port = port;
	_clients = backlog;

	_always_run = true;
	if (!_listenthread)
	{
		_listenthread = new std::thread(SocketServer::listen_thread_function, this);
		_listenthread->join();
	}
	return true;
}
bool SocketServer::stop()
{
	_always_run = false;

	if (_listenthread)
	{
		delete _listenthread;
	}
	return true;
}
/*
SocketServer::SocketServer(const Socket & s, unsigned short port, int backlog)
{
	_socket = s;
	_clients = backlog;
	_sockaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	_sockaddr.sin_family = AF_INET;
	// listen port 
	_sockaddr.sin_port = port;
	if (!_socket.isOpen())
		_socket.open();

	int err = bind(_socket._socket, (const sockaddr *)&_sockaddr, sizeof(sockaddr));
	if (err != 0){
		WSACleanup();
		return ;
	}
	//step 4: listen
	err = listen(_socket._socket, _clients);
	if (err != 0){
		WSACleanup();
		return ;
	}
	//step 5: wating for the connection
	sockaddr_in addrClient;
	int len = sizeof(sockaddr_in);
	while (true){
		SOCKET sockConn = accept(_socket._socket, (sockaddr*)&addrClient, &len);
		SessionSharedPtr p = Session::create(sockConn); 
		_sessions.push_back(p);
		p->run();
	}	 
}
*/

SocketServer::~SocketServer()
{

}
end_gtl_namespace



#endif