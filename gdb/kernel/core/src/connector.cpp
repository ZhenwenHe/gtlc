#include "connector.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

Connector::Connector()
{
	_type = UNKNOW_CONNECTOR;
}

Connector::Connector(std::string pathName){
	_pathName = pathName;
	_type = FDB_CONNECTOR;
}
std::string Connector::getConnectString(){
	if(_type==FDB_CONNECTOR){
		return _pathName;
	}
	else if(_type==RDB_CONNECTOR){
		return std::string("");
	}
	else if(_type==RDB_CONNECTOR){
		return std::string("");
	}
	else
		return std::string("");
}
const std::string Connector::getConnectString() const {
	if(_type==FDB_CONNECTOR){
		return _pathName;
	}
	else if(_type==RDB_CONNECTOR){
		return std::string("");
	}
	else if(_type==RDB_CONNECTOR){
		return std::string("");
	}
	else
		return std::string("");
}
Connector::Connector(const Connector & c){
	copy(c);
}
void Connector::copy(const Connector & c){
	_type = c._type;
	_user = c._user;
	_password = c._password;
	_dbname = c._dbname;
	_svrname = c._svrname;
	_pathName =c._pathName;
	_port = c._port;
}
Connector Connector::operator = (const Connector & c){
	copy(c);
	return *this;
}
Connector::Connector(std::string user,std::string password,std::string dbname,std::string svr){
	_type = RDB_CONNECTOR;
	_user = user;
	_password = password;
	_dbname = dbname;
	_svrname = svr;
}

Connector::Connector(std::string user,std::string password,std::string dbname,std::string svrIP,std::string port)
{
	_type = DDB_CONNECTOR;
	_user = user;
	_password = password;
	_dbname = dbname;
	_svrname = svrIP;
	_port = port;
}

const std::string Connector::getServeName() 
{
	return _svrname;
}
const std::string Connector::getPort() 
{
	return _port;
}
const std::string Connector::getDatabaseName()
{
	return _dbname;
}
const std::string Connector::getUser() 
{
	return _user;
}
const std::string Connector::getPassword() 
{
	return _password;
}

void  Connector::setDatabaseName(string &dbname)
{
	_dbname = dbname;
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace