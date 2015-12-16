
#ifdef WIN32 

begin_gtl_namespace
 
SocketClient::SocketClient( )
{
	_socket.open();	
}
SocketClient::~SocketClient()
{

}
SessionSharedPtr SocketClient::connect(const char * svrip, unsigned short port)
{
	_sockaddr.sin_addr.S_un.S_addr = inet_addr(svrip); 
	_sockaddr.sin_family = AF_INET;
	_sockaddr.sin_port = port;
	int err = ::connect(_socket._socket, (const sockaddr*)&_sockaddr, sizeof(sockaddr));
	if (err != 0){
		return SessionSharedPtr();
	}
	return Session::create(_socket._socket);
}

end_gtl_namespace



#endif