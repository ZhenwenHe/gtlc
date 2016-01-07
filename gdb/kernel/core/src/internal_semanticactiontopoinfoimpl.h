#pragma once
#include "internal_topoinfoimpl.h"


begin_gtl_namespace
begin_gdb_namespace

class SemanticActionTopoInfoImpl :virtual public TopoInfoImpl, virtual public SemanticActionTopoInfo
{
public:
	explicit SemanticActionTopoInfoImpl(FIDITEM fiditem) :
		TopoInfoImpl(fiditem, TOPOINFOTYPE_SEMATIC_ACTION)
	{

	}
	 
	~SemanticActionTopoInfoImpl();
	virtual std::string getName();

	/** 计算当前对象所需内存的大小
	* @param
	* @return 返回类的参数m_nSize
	*/
	virtual size_t sizeBuffer() { return 0; }

	/** 从缓存读取对象记录
	* @param [in] pBuffer 缓存
	* @return 读取成功与否
	*/
	virtual bool readBuffer(Buffer & buf) { return false; }

	/** 将对象写入缓存
	* @param [in] pBuffer 缓存
	* @return 写入成功与否
	*/
	virtual bool writeBuffer(Buffer& buf) { return false; }

	/** 将指定对象拷贝给当前对象
	* @return 拷贝对象指针
	*/
	virtual bool copy(TopoInfoImpl * topoInfo){ return false; }

	/** 将语义拓扑信息对象写入文件
	* @param [in] f 缓存
	* @return 写入成功与否
	*/
	virtual void write(std::ostream & f) {}

	/** 从文件中读取语义拓扑信息对象记录
	* @param [in] f 缓存
	* @return 读取成功与否
	*/
	virtual void read(std::istream & f) {}
};

end_gdb_namespace
end_gtl_namespace



