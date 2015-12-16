
#ifdef WIN32 

begin_gtl_namespace


int Socket::startup()
{
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsadata;
	int err = WSAStartup(wVersionRequested, &wsadata);
	if (err != 0)
		return -1;
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2){
		WSACleanup();
		return -1;
	}
	return 0;
}
int Socket::cleanup()
{
	return WSACleanup();
}


Socket::Socket()
{
	_af = AF_INET;
	_type = SOCK_STREAM;
	_protocol = 0;
	_socket = 0;
}
Socket::Socket(int af, int type, int protocol)
{
	_af = af;
	_type = type;
	_protocol = protocol;
	_socket = 0;
}
Socket::Socket(const Socket & s)
{
	_af = s._af;
	_type = s._type;
	_protocol = s._protocol;
	_socket = s._socket;
}
Socket Socket::operator = (const Socket & s)
{
	_af = s._af;
	_type = s._type;
	_protocol = s._protocol;
	_socket = s._socket;
	return *this;
}
Socket::~Socket()
{

}


inline bool Socket::open()
{
	if (_socket)
		return true;

	_socket = socket(_af, _type, _protocol);
	return _socket != 0;
}
inline bool Socket::close()
{
	int r = closesocket(_socket);
	_socket = 0;
	return r!=0;
}


inline bool Socket::isOpen()
{
	return _socket != 0;
}
 
inline bool Socket::open(int af, int type, int protocol)
{
	_af = af;
	_type = type;
	_protocol = protocol;
	if (isOpen())
		closesocket(_socket);
	_socket = 0;
	_socket = socket(_af, _type, _protocol);
	return _socket != 0;
}


end_gtl_namespace



#endif