/*
* Geosciences Template Library
*
* Copyright (c) 2008
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
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include "buffer.h"



begin_gtl_namespace
begin_gdb_namespace


/** @defgroup Connector Connector-空间数据库连接接口  
*  @{
*/
class CORE_API Connector  {	
	int _type;
	std::string _pathName;
	std::string _user;
	std::string _password;
	std::string _dbname;
	std::string _svrname;
	std::string _port;//端口号
public:
	enum{
		FDB_CONNECTOR,//文件系统类型 
		RDB_CONNECTOR,//关系数据库系统类型
		DDB_CONNECTOR,//分布式数据库系统类型
		UNKNOW_CONNECTOR//未知类型
	};
	/** 获取连接对象的类型，FDB_CONNECTOR表示文件系统类型，RDB_CONNECTOR表示关系数据库系统类型，
	DDB_CONNECTOR分布式数据库系统类型,UNKNOW_CONNECTOR表示未知类型
	* @return 连接对象的类型
	*/
	inline int getType() { return _type;}
public:
	/** 默认构造函数
	*/
	Connector();
	/** 构造函数，通过传入路径构建连接对象，构建的是文件系统类型的数据连接
	* @param [in] pathName  路径名称
	* @return 
	*/
	Connector(std::string pathName);
	/** 构造函数，通过传入数据库的连接信息构建连接对象，构建的是文件系统类型的数据连接
	* @param [in] user      数据库用户名
	* @param [in] password  数据库用户密码
	* @param [in] dbname    数据库
	* @param [in] svr       数据库服务器
	* @return 
	*/
	Connector(std::string user,std::string password,std::string dbname,std::string svr);
	/** 构造函数，通过传入数据库的连接信息构建连接对象，构建的是文件系统类型的数据连接
	* @param [in] user      数据库用户名
	* @param [in] password  数据库用户密码
	* @param [in] dbname    数据库
	* @param [in] svrIP     数据库服务器
	* @param [in] port      数据库服务器端口号
	* @return 
	*/
	Connector(std::string user,std::string password,std::string dbname,std::string svrIP,std::string port);

	/** 拷贝构造函数
	* @param [in] c      连接对象
	* @return 
	*/
	Connector(const Connector & c);
	/** 拷贝函数
	* @param [in] c      连接对象
	* @return 
	*/
	void copy(const Connector & c);
	/** 等号赋值函数
	* @param [in] c      连接对象
	* @return 
	*/
	Connector operator = (const Connector & c);
	/** 获取连接字符串，如果类型是FDB_CONNECTOR，返回的是工程文件的路径，如果类型是RDB_CONNECTOR返回的是包括关系数据库的连接字符串
	* @param [in] pBlock    内存块首地址指针
	* @param [in] blockSize 内存块大小，单位为字节
	* @return std::string 连接对象连接字符串
	*/
	std::string getConnectString();

	/** 获取连接字符串，如果类型是FDB_CONNECTOR，返回的是工程文件的路径，如果类型是RDB_CONNECTOR返回的是包括关系数据库的连接字符串
	* @param [in] pBlock    内存块首地址指针
	* @param [in] blockSize 内存块大小，单位为字节
	* @return std::string 连接对象连接字符串
	*/
	const std::string getConnectString() const;

	const std::string getServeName() ;
	const std::string getPort() ;
	const std::string getDatabaseName() ;
	const std::string getUser();
	const std::string getPassword() ;
	void  setDatabaseName(string &dbname); 


};
/** @} */ 


end_gdb_namespace
end_gtl_namespace


