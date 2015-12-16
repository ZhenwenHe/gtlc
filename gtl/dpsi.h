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
/*
本文件实现了分布式并行时空索引（Distributed Parallel Spatio-temporal Index, DPSI）.DPSI分为两个部分，
一部分是本地索引（Local Spatio-temporal Index, LSI），
本地索引可以是集中式环境下的任意时空索引，默认的本地索引是Interval4dTrees；
另一部分是分布式时空索引（Distributed Spatio-temporal Index,DSI），
DSI分为两种，一种是主从模式的分布式时空索引，另一种是对等模式的分布式时空索引。
*/
#pragma once
#include "omp.h" 
#include "buffer.h"
#include "vertex.h"
#include "envelope.h"
#include "interval4dtrees.h" 
#include "socket.h"
begin_gtl_namespace

template<
typename COORDTYPE = double,
int      DIMS = 4,
typename CHARTYPE = char,
typename TRAITS = std::char_traits<CHARTYPE>,
typename ALLOC = std::allocator<CHARTYPE> >
class NodeInfo: public Object<CHARTYPE, TRAITS, ALLOC>
{
public:
	typedef Interval<COORDTYPE> interval_type;
	typedef Envelope<COORDTYPE,DIMS-1> envelope_type;
	typedef std::basic_string<CHARTYPE, TRAITS, ALLOC> ip_type;
	typedef unsigned int port_type;
	typedef unsigned int status_type;
	typedef unsigned long long  capacity_type;
public:
	//节点服务器的IP地址
	ip_type        ip;
	//节点服务器的端口
	port_type      port;
	//节点服务器上管理的数据的时间范围
	interval_type  interval;
	//节点服务器上管理的数据的空间范围
	envelope_type  envelope;
	//节点服务器上的状态
	status_type    status;
	//节点服务器上的最大数据容量，按照时空对象个数计算
	capacity_type capacity;
public:
	NodeInfo( )
	{
		status = 0;
		capacity = 1024 * 1024 * 1024;
	}
	NodeInfo(const ip_type & ip_in, const port_type & port_in)
	{
		ip = ip_in;
		port = port_in;
		status = 0;
		capacity = 1024 * 1024 * 1024;
	}
	NodeInfo(const ip_type & ip_in, const port_type & port_in,
		const interval_type & interval_in,const envelope_type & envelope_in,
		const status_type & status_in=0, const capacity_type& capacity_in=1024*1024*1024)
		:ip(ip_in), port(port_in), interval(interval_in), envelope(envelope_in),
		status(status_in), capacity(capacity_in)
	{
		
	}
	void set(const ip_type & ip_in, const port_type & port_in,
		const interval_type & interval_in, const envelope_type & envelope_in,
		const status_type & status_in = 0, const capacity_type& capacity_in = 1024 * 1024 * 1024)
	{

		ip = ip_in;
		port = port_in;
		interval = interval_in;
		envelope = envelope_in;
		status=status_in;
		capacity = capacity_in;
	}
	NodeInfo(const NodeInfo & n)
	{
		ip = n.ip;
		port = n.port;
		interval = n.interval;
		envelope = n.envelope;
		status = n.status;
		capacity = n.capacity;
	}
	NodeInfo operator = (const NodeInfo & n)
	{
		ip = n.ip;
		port = n.port;
		interval = n.interval;
		envelope = n.envelope;
		status = n.status;
		capacity = n.capacity;
		return *this;
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		Buffer<CHARTYPE, TRAITS, ALLOC> buf;
		//ip
		buf.write(ip);
		//port
		buf.write((void*)(&port), sizeof(port_type));
		//interval
		void * buf2 = 0;
		std::size_t s2 = 0;
		interval.writeBuffer(buf2, s2);
		int s = s2;
		buf.write((void*)&s, sizeof(int));
		buf.write(buf2, s2);
		delete[] buf2;
		buf2 = 0;
		s2 = 0;
		//envelope
		envelope.writeBuffer(buf2, s2);
		s = s2;
		buf.write((void*)&s, sizeof(int));
		buf.write(buf2, s2);
		delete[] buf2;
		buf2 = 0;
		s2 = 0;
		//status
		buf.write((void*)(&status), sizeof(status_type));
		//capacity
		buf.write((void*)(&capacity), sizeof(capacity_type));

		buffer = buf.detach(count);
	}
	virtual void readBuffer(const void *  buffer, size_t count = 0){
		Buffer<CHARTYPE, TRAITS, ALLOC> buf((void*)buffer, count, false);
		//ip
		buf.read(ip);
		//port
		buf.read((void*)(&port), sizeof(port_type));
		//interval
		int s = 0;
		buf.read((void*)&s, sizeof(int));
		void * buf2 = (void*)new char[s];
		std::size_t s2 = s;
		buf.read(buf2, s2);
		interval.readBuffer(buf2, s2);
		delete[] buf2;
		buf2 = 0;
		s2 = 0;
		s = 0;
		//envelope
		buf.read((void*)&s, sizeof(int));
		s2 = s;
		buf2 = (void*)new char[s];
		buf.read(buf2, s2);
		envelope.readBuffer(buf2, s2);
		delete[] buf2;
		buf2 = 0;
		s2 = 0;
		s = 0;
		//status
		buf.read((void*)(&status), sizeof(status_type));
		//capacity
		buf.read((void*)(&capacity), sizeof(capacity_type));
	}
	virtual Object<CHARTYPE, TRAITS, ALLOC>  * clone() {
		return (Object<CHARTYPE, TRAITS, ALLOC>  *)new NodeInfo(*this);
	}
	virtual std::basic_string<CHARTYPE, TRAITS, ALLOC> getClassName()
	{
		std::basic_string<CHARTYPE, TRAITS, ALLOC> sz; 
		return sz;
	}
	
};

template<
typename COORDTYPE=double,
int      DIMS=4,
typename CHARTYPE = char,
typename TRAITS = std::char_traits<CHARTYPE>,
typename ALLOC = std::allocator<CHARTYPE> >
class NodeInfoTab : public Object<CHARTYPE, TRAITS, ALLOC>
{
public:
	typedef typename NodeInfo<COORDTYPE,DIMS,CHARTYPE,TRAITS, ALLOC> nodeinfo_type;
	typedef typename nodeinfo_type::interval_type interval_type;
	typedef typename nodeinfo_type::envelope_type envelope_type;
	typedef typename nodeinfo_type::ip_type ip_type;
	typedef typename nodeinfo_type::port_type port_type;
	typedef typename nodeinfo_type::status_type status_type;
	typedef typename nodeinfo_type::capacity_type capacity_type; 
protected:
	//主服务器记录的全局时间范围
	interval_type _gloablInterval;
	//主服务器记录的全局空间范围
	envelope_type _gloablEnvelope;
	//主服务器上维护的节点信息表
	std::vector<nodeinfo_type>  _nodeinfos;
public:
	NodeInfoTab()
	{

	}
	NodeInfoTab(const interval_type& i, const envelope_type& e)
		: 
	{

	}
	NodeInfoTab(const NodeInfoTab& n )
	{
		_gloablInterval = n._gloablInterval;
		_gloablEnvelope = n._gloablEnvelope;
		_nodeinfos.resize(n._nodeinfos.size());
		std::copy(n._nodeinfos.begin(), n._nodeinfos.end(), _nodeinfos.begin());
	}
	NodeInfoTab operator=(const NodeInfoTab& n)
	{
		_globalInterval = n._gloablInterval;
		_gloablEnvelope = n._gloablEnvelope;
		_nodeinfos.resize(n._nodeinfos.size());
		std::copy(n._nodeinfos.begin(), n._nodeinfos.end(), _nodeinfos.begin());
		return *this;
	}
	virtual std::basic_string<CHARTYPE, TRAITS, ALLOC> getClassName(){
		return "NodeInfoTab";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		Buffer<CHARTYPE, TRAITS, ALLOC> buf; 
		//_gloablInterval
		void * buf2 = 0;
		std::size_t s2 = 0;
		_gloablInterval.writeBuffer(buf2, s2);
		int s = s2;
		buf.write((void*)&s, sizeof(int));
		buf.write(buf2, s2);
		delete[] buf2;
		buf2 = 0;
		s2 = 0;
		//_gloablEnvelope
		_gloablEnvelope.writeBuffer(buf2, s2);
		s = s2;
		buf.write((void*)&s, sizeof(int));
		buf.write(buf2, s2);
		delete[] buf2;
		buf2 = 0;
		s2 = 0;
		//节点个数
		s = _nodeinfos.size();
		buf.write((void*)&s, sizeof(int));
		//每个节点的信息
		for (auto it = _nodeinfos.begin(); it != _nodeinfos.end(); it++)
		{
			it->writeBuffer(buf2, s2);
			s = s2;
			buf.write((void*)&s, sizeof(int));
			buf.write(buf2, s2);
			delete[] buf2;
			buf2 = 0;
			s2 = 0;
		}

		buffer = buf.detach(count);
	}
	virtual void readBuffer(const void *  buffer, size_t count = 0){
		Buffer<CHARTYPE, TRAITS, ALLOC> buf((void*)buffer,count,false);
		//_gloablInterval
		int s = 0;
		buf.read((void*)&s, sizeof(int));
		void * buf2 = (void*) new char[s];
		std::size_t s2 = s;
		buf.read(buf2, s2);		
		_gloablInterval.readBuffer(buf2, s2);		
		delete[] buf2;
		buf2 = 0;
		s2 = 0;
		//_gloablEnvelope
		s = 0;
		buf.read((void*)&s, sizeof(int));
		buf2 = (void*) new char[s];
		s2 = s;
		buf.read(buf2, s2);
		_gloablEnvelope.readBuffer(buf2, s2);
		delete[] buf2;
		buf2 = 0;
		s2 = 0;
		//节点个数
		s = 0;
		buf.read((void*)&s, sizeof(int));
		_nodeinfos.resize(s);
		int n = s;
		//每个节点的信息
		for (auto it = 0; it <n; it++)
		{
			s = 0;
			buf.read((void*)&s, sizeof(int));
			buf2 = new char[s];
			s2 = s;
			buf.read(buf2, s2);
			_nodeinfos[it].readBuffer(buf2, s2);
			delete[] buf2;
			buf2 = 0;
			s2 = 0;
		}
	}
	virtual Object<CHARTYPE, TRAITS, ALLOC> * clone() {
		return (Object<CHARTYPE, TRAITS, ALLOC> *)new NodeInfoTab(*this);
	}
	void add(const nodeinfo_type & ni)
	{
		_nodeinfos.push_back(ni);
	}
};


template<
	typename COORDTYPE = double,
	int      DIMS = 4,
	typename CHARTYPE = char,
	typename TRAITS = std::char_traits<CHARTYPE>,
	typename ALLOC = std::allocator<CHARTYPE> >
class IndexCommand : public Object<CHARTYPE, TRAITS, ALLOC>
{
	typedef NodeInfoTab<COORDTYPE,DIMS,CHARTYPE, TRAITS, ALLOC> nodeinfotab_type;
	typedef typename nodeinfotab_type::interval_type  interval_type;
	typedef typename nodeinfotab_type::envelope_type  envelope_type;
	typedef typename nodeinfotab_type::ip_type        ip_type;
	typedef typename nodeinfotab_type::port_type      port_type;
	typedef typename nodeinfotab_type::status_type    status_type;
	//命令类型 0- query 1-insert 2-delete
	typedef int                                         command_type;
protected:
	//命令类型
	command_type  _type;

};

template<
	typename COORDTYPE = double,
	int      DIMS = 4,
	typename CHARTYPE = char,
	typename TRAITS = std::char_traits<CHARTYPE>,
	typename ALLOC = std::allocator<CHARTYPE> >
class QueryCommand : public IndexCommand<COORDTYPE, DIMS, CHARTYPE, TRAITS, ALLOC>
{
	typedef IndexCommand<COORDTYPE, DIMS, CHARTYPE, TRAITS, ALLOC> base_type;
	typedef typename base_type::interval_type  interval_type;
	typedef typename base_type::envelope_type  envelope_type;
	typedef typename base_type::ip_type        ip_type;
	typedef typename base_type::port_type      port_type;
	typedef typename base_type::status_type    status_type; 
protected:
	//命令类型
	command_type  _type;
public:
	QueryCommand(){ _type = 0; }

};

template<
	typename COORDTYPE = double,
	int      DIMS = 4,
	typename CHARTYPE = char,
	typename TRAITS = std::char_traits<CHARTYPE>,
	typename ALLOC = std::allocator<CHARTYPE> >
class InsertCommand : public IndexCommand<COORDTYPE, DIMS, CHARTYPE, TRAITS, ALLOC>
{
	typedef IndexCommand<COORDTYPE, DIMS, CHARTYPE, TRAITS, ALLOC> base_type;
	typedef typename base_type::interval_type  interval_type;
	typedef typename base_type::envelope_type  envelope_type;
	typedef typename base_type::ip_type        ip_type;
	typedef typename base_type::port_type      port_type;
	typedef typename base_type::status_type    status_type;
public:
	InsertCommand(){ _type = 1; }

};

template<
	typename COORDTYPE = double,
	int      DIMS = 4,
	typename CHARTYPE = char,
	typename TRAITS = std::char_traits<CHARTYPE>,
	typename ALLOC = std::allocator<CHARTYPE> >
class DeleteCommand : public IndexCommand<COORDTYPE, DIMS, CHARTYPE, TRAITS, ALLOC>
{
	typedef IndexCommand<COORDTYPE, DIMS, CHARTYPE, TRAITS, ALLOC> base_type;
	typedef typename base_type::interval_type  interval_type;
	typedef typename base_type::envelope_type  envelope_type;
	typedef typename base_type::ip_type        ip_type;
	typedef typename base_type::port_type      port_type;
	typedef typename base_type::status_type    status_type;
 
public:
	DeleteCommand(){ _type = 2; }
};

template<
	typename COORDTYPE = double,
	int      DIMS = 4,
	typename CHARTYPE = char,
	typename TRAITS = std::char_traits<CHARTYPE>,
	typename ALLOC = std::allocator<CHARTYPE> >
class MasterServer : public Object<CHARTYPE, TRAITS, ALLOC>
{
	typedef typename Interval4dTrees<TriTree>  localindex_type;
	typedef typename NodeInfoTab<COORDTYPE,DIMS,CHARTYPE, TRAITS, ALLOC> nodeinfotab_type;	
	typedef typename nodeinfotab_type::nodeinfo_type nodeinfo_type;
	typedef typename nodeinfo_type::interval_type interval_type;
	typedef typename nodeinfo_type::envelope_type envelope_type;
	typedef typename nodeinfo_type::ip_type ip_type;
	typedef typename nodeinfo_type::port_type port_type;
	typedef typename nodeinfo_type::status_type status_type;
	typedef typename nodeinfo_type::capacity_type capacity_type;
protected:
	//主服务器上维护的所有节点信息表
	nodeinfotab_type          _nodeInfos;	
	//本地的基于间隔关系算子的并行时空索引(IPSI)
	//在实现过程中我们将主服务器上也存储数据
	//因此其也有本地IPSI
	localindex_type    * _localIndex;
	SocketServer*        _socksvr;
	
public:
	MasterServer()
	{
		_socksvr = 0;
		_localIndex = 0;
	}
	~MasterServer()
	{
		stop();
		if (_localIndex)
			delete _localIndex;
	}
	bool start(unsigned short listenport=6666)
	{
		if (!_socksvr)
		{
			_socksvr = new SocketServer(listenport, 1024); 
		}
		return true;
	}
	bool stop()
	{
		if (_socksvr)
		{
			delete _socksvr;
		}
		return true;
	}
	localindex_type * getLocalIndex() 
	{ 
		return _localIndex;
	}
	bool addSlave(const nodeinfo_type & ni)
	{
		_nodeInfos.add(ni);
	}
	SessionSharedPtr connectSlave(const nodeinfo_type & ni)
	{
		Socket s;
		SocketClient sockclt(s,ni.ip,ni.port);
		return sockclt.connect();
	}
	virtual void setupLocalIndex(const interval_type & i, const envelope_type& e, unsigned short blocksize=32)
	{
		if (_localIndex == 0)
		{
			_localIndex = new localindex_type(i.start(), i.end(), blocksize,
				e.getLowerBound(0), e.getUpperBound(0), blocksize,
				e.getLowerBound(1), e.getUpperBound(1), blocksize,
				e.getLowerBound(2), e.getUpperBound(2), blocksize);
		}
	}
	virtual std::basic_string<CHARTYPE, TRAITS, ALLOC> getClassName()
	{
		return "MasterServer";
	}
	virtual void writeBuffer(void* & buffer, size_t & count)
	{

	}
	virtual void readBuffer(const void *  buffer, size_t count = 0)
	{

	}
	virtual Object * clone()
	{
		return 0;
	}
};

template<
	typename COORDTYPE = double,
	int      DIMS = 4,
	typename CHARTYPE = char,
	typename TRAITS = std::char_traits<CHARTYPE>,
	typename ALLOC = std::allocator<CHARTYPE> >
class SlaveServer : public Object<CHARTYPE, TRAITS, ALLOC>
{
	typedef Interval4dTrees<TriTree>  localindex_type;
	typedef NodeInfoTab<COORDTYPE, DIMS, CHARTYPE, TRAITS, ALLOC> nodeinfotab_type;
	typedef NodeInfo<COORDTYPE, DIMS, CHARTYPE, TRAITS, ALLOC> nodeinfo_type;
	typedef typename nodeinfo_type::interval_type interval_type;
	typedef typename nodeinfo_type::envelope_type envelope_type;
	typedef typename nodeinfo_type::ip_type ip_type;
	typedef typename nodeinfo_type::port_type port_type;
	typedef typename nodeinfo_type::status_type status_type;
	typedef typename nodeinfo_type::capacity_type capacity_type;
protected:
	//节点服务器记录的本地索引时间范围
	interval_type _localInterval;
	//节点服务器记录的本地索引空间范围
	envelope_type _localEnvelope;
	//本地的基于间隔关系算子的并行时空索引(IPSI)
	localindex_type    _localIndex;
	SocketServer*      _socksvr;
public:
	SlaveServer()
	{
		_socksvr = 0;
	}
	~SlaveServer()
	{
		stop();
	}
	bool start(unsigned short listenport = 6666)
	{
		if (!_socksvr)
		{
			_socksvr = new SocketServer(listenport, 1024);
		}
		return true;
	}
	bool stop()
	{
		if (_socksvr)
		{
			delete _socksvr;
		}
		return true;
	}
	localindex_type & getLocalIndex()
	{
		return _localIndex;
	}
	virtual std::basic_string<CHARTYPE, TRAITS, ALLOC> getClassName()
	{
		return "SlaveServer";
	}
	virtual void writeBuffer(void* & buffer, size_t & count)
	{

	}
	virtual void readBuffer(const void *  buffer, size_t count = 0)
	{

	}
	virtual Object * clone()
	{
		return 0;
	}
};

template<
	typename COORDTYPE = double,
	int      DIMS = 4,
	typename CHARTYPE = char,
	typename TRAITS = std::char_traits<CHARTYPE>,
	typename ALLOC = std::allocator<CHARTYPE> >
class PeerServer : public Object<CHARTYPE, TRAITS, ALLOC>
{
	typedef NodeInfoTab<COORDTYPE, DIMS, CHARTYPE, TRAITS, ALLOC> nodeinfotab_type;
	typedef typename nodeinfotab_type::interval_type  interval_type;
	typedef typename nodeinfotab_type::envelope_type  envelope_type;
	typedef typename nodeinfotab_type::ip_type        ip_type;
	typedef typename nodeinfotab_type::port_type      port_type;
	typedef typename nodeinfotab_type::status_type    status_type;
protected:
	//服务器上维护的所有其他节点信息表
	nodeinfotab_type  _nodeInfos;
	//节点服务器记录的本地索引时间范围
	interval_type _localInterval;
	//节点服务器记录的本地索引空间范围
	envelope_type _localEnvelope;
	//本地的基于间隔关系算子的并行时空索引(IPSI)
	Interval4dTrees<TriTree>    _localIndex;
public:
	virtual std::basic_string<CHARTYPE, TRAITS, ALLOC> getClassName()
	{
		return "PeerServer";
	}
	virtual void writeBuffer(void* & buffer, size_t & count)
	{

	}
	virtual void readBuffer(const void *  buffer, size_t count = 0)
	{

	}
	virtual Object * clone()
	{
		return 0;
	}
};



end_gtl_namespace
