
#ifdef WIN32 

begin_gtl_namespace




Session::Session(socket_type connSocket)
{
	_connSocket = connSocket;
}
Session::Session(const Session& s)
{
	_connSocket = s._connSocket;
}
Session Session::operator = (const Session& s)
{
	_connSocket = s._connSocket;
	return *this;
}
Session::~Session()
{
	close();
}

inline int Session::send(const char * buf, int len, int flags)
{
	return ::send(_connSocket,buf, len, flags);
}
inline int Session::receive(char * buf, int len, int flags)
{
	return ::recv(_connSocket, buf, len, flags);
}
inline int Session::close()
{
	if (_connSocket)
		return closesocket(_connSocket);
	return -1;
}

inline void  Session::run()
{
	std::thread t(Session::thread_procedure, this);
	t.join();
}

SessionSharedPtr Session::create(socket_type connSocket)
{
	return SessionSharedPtr(new Session(connSocket), Session::Deleter());
}

void  Session::thread_procedure(Session * p)
{
	p->runTasks();
}
inline int Session::sendOkay()
{
	int data = Session::OKAY;
	return send((const char*)&data, sizeof(int), 0);
}

/* 这是直接重装runTasks函数实现的服务器端会话例子
void Session::runTasks()
{
	int cmd;
	_task_loop = true;
	while (_task_loop)
	{
		cmd = -1;
		receive((char*)&cmd, sizeof(int), 0);
		switch (cmd)
		{
		case BEGINSESSION:
		{
							std::cout << "BEGINSESSION\n";
							sendOkay();
							break;
		}
		case ENDSESSION:
		{
						   std::cout << "ENDSESSION\n";
						   cmd = -1;
						   sendOkay();
						   _task_loop = false;
						   break;
		}
		case RANGEQUERY:
		{
						   std::cout << "RANGEQUERY\n";
						   cmd = -1;
						   sendOkay();
						   //接收参数
						   char buf[256];
						   receive(buf,256,0);
						   //DO 查询
						   Sleep(1000 * 60);
						   //发送查询结果
						   send(buf, 256, 0);
						   break;
		}
		case KNNQUERY:
		{
							std::cout << "KNNQUERY\n";
							cmd = -1;
							sendOkay();
							//接收参数
							char buf[256];
							receive(buf, 256, 0);
							//DO 查询
							Sleep(1000 * 60);
							//发送查询结果
							send(buf, 256, 0);
							break;
		}
		case  TRAJACTORYQUERY:
		{
							std::cout << "TRAJACTORYQUERY\n";
							cmd = -1;
							sendOkay();
							//接收参数
							char buf[256];
							receive(buf, 256, 0);
							//DO 查询
							Sleep(1000 * 60);
							//发送查询结果
							send(buf, 256, 0);
							break;
		}
		default:
			break;
		}
	}
	
}
*/
void Session::runTasks()
{
	int cmd;
	_task_loop = true;
	while (_task_loop)
	{
		cmd = -1;
		receive((char*)&cmd, sizeof(int), 0);
		auto it = _tasks.find(cmd);
		if (it != _tasks.end())
		{
			it->second->run();
		}
	}
}

inline void Session::addTask(TaskSharedPtr &p)
{
	if (p.get()){
		_tasks[p->_cmdid] = p;
		p->_session = this;
	}		
}
inline void Session::addTaskList(std::list<TaskSharedPtr> &ps)
{
	for (auto it = ps.begin(); it != ps.end(); it++)
	{
		if ((*it).get()){
			_tasks[(*it)->_cmdid] = (*it);
			(*it)->_session = this;
		}
	}
}
inline TaskSharedPtr Session::removeTask(int cmdid)
{
	TaskSharedPtr p;
	auto it = _tasks.find(cmdid);
	if (it != _tasks.end())
	{
		p = it->second;
		_tasks.erase(it);
	}
	return p;
}

end_gtl_namespace



#endif