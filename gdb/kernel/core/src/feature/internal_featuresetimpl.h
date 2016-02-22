#pragma once
#include "feature.h"



begin_gtl_namespace
begin_gdb_namespace

/** @defgroup FeatureSetImpl 要素集合接口
*  @{
*/
class  FeatureSetImpl :public std::enable_shared_from_this<FeatureSetImpl>, public FeatureSet
{
protected:
	/**要素集合ID*/
	FSID  _fsid;

	/**要素集合名称*/
	std::string  _name;
	/**要素对象对应的要素类*/
	FeatureClassSharedPtr _fclass;
	/**要素集合*/
	FeatureSharedPtrVector _fset;//要素
	/**要素集合所在的图层*/
	LYRID       _localLayer;



	/** @name 构造与析构函数
	*  @{
	*/
	FeatureSetImpl(const FeatureClass & fcls);
	FeatureSetImpl(FeatureClassSharedPtr fcls);
	/** @} */
public:

	/** @name 构造与析构函数
	*  @{
	*/
	FeatureSetImpl();
	void clear();
	void copy(const FeatureSetSharedPtr fSet, bool bContent = false);
	void copy(const FeatureSetRef fSet, bool bContent);
	~FeatureSetImpl();
	/** @} */

	/** @name 运算符重载
	*  @{
	*/
	Feature & operator() (size_t s);
	const Feature & operator()  (size_t s) const;
	inline size_t  size() const { return _fset.size(); }
	/** @} */

public:
	/** @name 枚举成员变量说明
	*  @{
	*/
	/**
	* SINGLE_FILE_DB = 0,整个工程存放为一个GDB文件，属于单文件系统的格式，要进行具体要素对象的读写
	* MULTI_FILE_DB = 1,整个工程存放为多个文件，属于文件系统的数据库格式，不进行具体要素对象的读写
	* ORA_FILE_DB = 2,基于关系数据库的存储,要进行具体要素对象的读写
	*/
	enum DatasetType{
		SINGLE_FILE_DB = 0,
		MULTI_FILE_DB,
		ORA_FILE_DB
	};
	/** @} */

	/** @name 静态成员函数说明
	*  @{
	*/
	/** 获取数据集类型
	* @param  无
	* @return 数据集类型
	*/
	static DatasetType getDatasetType();

	/** 设置数据集类型
	* @param  [in]  dst 数据集类型
	* @return 无
	*/
	static void setDatasetType(DatasetType dst);
	/** @} */


public:
	/** @name 属性设置获得操作集
	*  @{
	*/

	/**获取要素集合ID
	* @param  无
	* @return   要素集合ID
	*/
	inline FSID getID(){ return _fsid; }
	/**设置要素集合ID
	* @param [in] id 要素集合ID
	* @return  无
	*/
	inline void setID(FSID id){ _fsid = id; }

	/** 获取要素集合名称
	* @param  无
	* @return  要素集合名称
	*/
	inline std::string getName() { return _name; }
	/** 设置要素集合的名称
	* @param  [in] szName 要素集合名称
	* @return 无
	*/
	inline void setName(std::string szName) { _name = szName; }


	/** 设置要素集合对应的要素类
	* @param  [in] f 要素集合对应的要素类
	* @return 无
	*/
	inline void setFeatureClass(FeatureClassSharedPtr  f);

	/** 设置要素集合对应的要素类
	* @param  [in] f 要素集合对应的要素类
	* @return 无
	*/
	inline void attachFeatureClass(FeatureClass* f);
	/** 获取要素集合对应的要素类
	* @param  无
	* @return 要素集合对应的要素类
	*/
	inline FeatureClassSharedPtr   getFeatureClass();
	/** @} */

	/** @name 其他操作函数集
	*  @{
	*/
	/**在要素集合中添加指定要素ID的要素对象
	* @param [in] p 要素对象
	* @return  无
	*/
	void addFeature(FeatureSharedPtr p);
	///**在要素集合中删除指定要素ID的要素数据
	//* @param [in] fid  要素ID
	//* @return  无
	//*/
	//inline void removeAFeature(FID fid);
	/**在要素集合中删除指定要素ID的要素数据
	* @param [in] fid  要素ID
	* @return  要素对象指针
	*/
	FeatureSharedPtr removeFeature(FID fid);
	/** 获取要素集合中的要素集
	* @param  无
	* @return 要素集合中的要素集
	*/
	inline FeatureSharedPtrVector& getFeatures(){ return _fset; }
	/** 设置要素集合所在的图层
	* @param  [in] lyr 要素集合所在的图层
	* @return 无
	*/
	virtual void setFeatures(FeatureSharedPtrVector &feas){ _fset = feas; }

	inline void setLayerID(LYRID lyr){ _localLayer = lyr; }
	/** 获取要素集合所在的图层
	* @param  无
	* @return 要素集合所在的图层
	*/
	inline LYRID getLayerID(){ return _localLayer; }
	/**在要素集合中删除指定要素ID的要素数据
	* @param [in] fid  要素ID
	* @return
	*/
	void eraseFeature(FID fid);

	/** @} *



	/** @name 文件IO操作函数集
	*  @{
	*/
	/** 写文件
	* @param [in] f输出流
	* @return 无
	*/
	virtual void write(std::ostream & f);
	/** 读文件
	* @param [in] f输入流
	* @return 无
	*/
	virtual void read(std::istream & f);
	/** @} */

	virtual FeatureSharedPtr createFeature(FID id, std::string name, int lod, REGID regid,
		int state = 0, GeometrySharedPtr pg = GeometrySharedPtr(), GDBTIME createTime = 0, GDBTIME updateTime = 0);
	virtual FeatureSharedPtr createFeature(std::string name, int lod, REGID regid,
		int state = 0, GeometrySharedPtr pg = GeometrySharedPtr(), GDBTIME createTime = 0, GDBTIME updateTime = 0);
	virtual FeatureSharedPtr createFeature(FeatureRef f);
public:
	friend class FeatureSet;

};
end_gdb_namespace
end_gtl_namespace


