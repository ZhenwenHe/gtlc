#pragma once
#include "topology.h"
#include "internal_topoinfoimpl.h"


begin_gtl_namespace
begin_gdb_namespace

class   SematicGeometryTopoInfoImpl : virtual public TopoInfoImpl, virtual public SemanticGeometryTopoInfo
{
	/** @defgroup SematicGeometryTopoInfoImpl  SematicGeometryTopoInfoImpl-语义几何信息对象类
	*  @{
	*/
protected:
	/** @name 属性（成员变量）
	* @{
	*/

	/** 语义信息 */
	string	m_sematicName;

	/** 语义拓扑信息类型，每个类型对应一个编号 */
	GeometrySharedPtr m_geometry;

	/** @} */ // 属性结尾

public:

	/** @defgroup SematicGeometryTopoInfoImpl 操作（成员函数）
	* @{
	*/
	explicit SematicGeometryTopoInfoImpl(FIDITEM fiditem) :
		TopoInfoImpl(fiditem, TOPOINFOTYPE_SEMATIC_GEOMETRY),
		m_sematicName(""){}


	virtual ~SematicGeometryTopoInfoImpl(void);

	/** 获得对象的语义信息
	* @param
	* @return 对象的语义信息
	*/
	inline string getSemanticName() const { return m_sematicName; }

	/** 设置对象的语义信息
	* @param name 对象的语义信息
	* @return
	*/
	inline void setSemanticName(const string name)  { m_sematicName = name; }

	/** 设置对象的拓扑几何
	* @param [in]  geo 对象的拓扑几何
	* @return void
	*/
	inline void setGeometry(GeometrySharedPtr  geo) { m_geometry = geo; }

	/** 获得对象的拓扑几何
	* @param
	* @return 对象的拓扑几何
	*/
	inline GeometrySharedPtr getGeometry()  { return m_geometry; }

	/** 计算当前对象所需内存的大小
	* @param
	* @return 返回类的参数m_nSize
	*/
	virtual size_t sizeBuffer();

	/** 从缓存读取对象记录
	* @param [in] pBuffer 缓存
	* @return 读取成功与否
	*/
	virtual bool readBuffer(Buffer & buf);

	/** 将对象写入缓存
	* @param [in] pBuffer 缓存
	* @return 写入成功与否
	*/
	virtual bool writeBuffer(Buffer& buf);

	/** 将指定对象拷贝给当前对象
	* @return 拷贝对象指针
	*/
	virtual bool copy(TopoInfoImpl * topoInfo);

	/** 将语义拓扑信息对象写入文件
	* @param [in] f 缓存
	* @return 写入成功与否
	*/
	virtual void write(std::ostream & f);

	/** 从文件中读取语义拓扑信息对象记录
	* @param [in] f 缓存
	* @return 读取成功与否
	*/
	virtual void read(std::istream & f);
};
/////////////////////////////////////////////////////////////////////

end_gdb_namespace
end_gtl_namespace


