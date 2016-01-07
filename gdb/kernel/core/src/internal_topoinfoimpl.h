#pragma once
#include "internal_topoitemimpl.h"
 


begin_gtl_namespace
begin_gdb_namespace
 

class  TopoInfoImpl : virtual public TopoInfo
{
	/** @defgroup TopoInfoImpl TopoInfoImpl-语义拓扑信息对象基类
	*  @{
	*/
protected:
	/** @name 属性（成员变量）
	* @{
	*/

	//原始的要素信息
	FIDITEM m_originalItem;

	/** 语义拓扑信息类型，每个类型对应一个编号 */
	int m_type;

	/** @} */ // 属性结尾

public:


	virtual Identifier::raw_type getFeatureClassID() const {
		return m_originalItem.second;
	}
	virtual Identifier::raw_type getFeatureID() const  {
		return m_originalItem.first;
	}

	virtual void setFeatureClassID(Identifier::raw_type d){
		m_originalItem.second = d;
	}
	virtual void setFeatureID(Identifier::raw_type d){
		m_originalItem.first = d;
	}

	virtual bool copy(const TopoInfo & topoInfo){
		TopoInfoImpl* p = dynamic_cast<TopoInfoImpl*>((TopoInfo*)(&topoInfo));
		return copy(p);
	}


	/** @defgroup TopoInfoImpl 操作（成员函数）
	* @{
	*/
	TopoInfoImpl(FIDITEM fiditem, int type = 0);
	virtual ~TopoInfoImpl(void);

	/** 获得对象的语义拓扑信息类型编号
	* @param
	* @return 对象的语义拓扑信息类型编号
	*/
	inline int getType() const { return m_type; }

	/** 设置对象的语义拓扑信息类型编号
	* @param [in]  type 对象的语义拓扑信息类型编号
	* @return void
	*/
	inline void setType(int type) { m_type = type; }

	/** 获得原始的要素信息
	* @param
	* @return 对象的拓扑类型编号
	*/
	inline FIDITEM getOriginalItem()  { return m_originalItem; }

	/** 设置原始的要素信息
	* @param [in]  type 拓扑类型编号
	* @return void
	*/
	inline void setOriginalItem(FIDITEM & item) { m_originalItem = item; }

	/** 计算当前对象所需内存的大小
	* @param
	* @return 返回类的参数m_nSize
	*/
	virtual size_t sizeBuffer() = 0;

	/** 从缓存读取对象记录
	* @param [in] pBuffer 缓存
	* @return 读取成功与否
	*/
	virtual bool readBuffer(Buffer & buf) = 0;

	/** 将对象写入缓存
	* @param [in] pBuffer 缓存
	* @return 写入成功与否
	*/
	virtual bool writeBuffer(Buffer& buf) = 0;

	/** 将指定对象拷贝给当前对象
	* @return 拷贝对象指针
	*/
	virtual bool copy(TopoInfoImpl * topoInfo);

	/** 将语义拓扑信息对象写入文件
	* @param [in] f 缓存
	* @return 写入成功与否
	*/
	virtual void write(std::ostream & f) = 0;

	/** 从文件中读取语义拓扑信息对象记录
	* @param [in] f 缓存
	* @return 读取成功与否
	*/
	virtual void read(std::istream & f) = 0;

};
end_gdb_namespace
end_gtl_namespace


