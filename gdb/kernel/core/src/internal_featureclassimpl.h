#pragma once
#include "feature.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace 

class FeatureClassImpl :public std::enable_shared_from_this <FeatureClassImpl>, FeatureClass {

	/** @name 保护成员变量说明
	*  @{
	*/
protected:
	/** 要素类型编号 */
	FCLSID   _id;
	/** 要素类型名称 */
	std::string   _name;
	/** 要素类型编码 */
	std::string  _code;
	/** 空间参考系统ID */
	SREFID  _srefid;
	/**要素几何类型 */
	//int     _geom; 
	/** 几何表的分区个数*/
	int   _partitionnum;
	/** 对应的属性字段集合*/
	std::vector<PropType> _proptypes;
	/** 要素类所在的要素集合*/
	FeatureSetSharedPtr	_localfeatureset;
	/** 要素类型所在的工程*/
	PRJID   _prjID;
	/** 设置要素类的状态 */
	int	_fclstype;
	/** 要素类型对应要素集合管理类型 0:按要素类型建要素集合表；1：按几何类型建要素集合表*/
	int _fsettype;
	/** 要素类型对应集合数据表管理类型 0：要素集合的属性表单独建立；1：要素集合的属性表和要素集合表在一起*/
	int _fsetproptype;
	/** @} */

	/** @name 公用成员变量说明
	*  @{
	*/
public:
	/** 友元类 */
	friend class FeatureUtils;
	/** @} */


public:
	/** @name 构造与析构函数
	*  @{
	*/
	FeatureClassImpl();
	FeatureClassImpl(std::string name, std::string code, int  geomtype = 0, SREFID srefid = 0, std::string gTab = "", std::string aTab = "", int partitionnum = 0);
	FeatureClassImpl(FCLSID id, std::string name, std::string code, int geomtype = 0, SREFID srefid = 0, std::string gTab = "", std::string aTab = "", int partitionnum = 0);
	FeatureClassImpl(const FeatureClassImpl & fc);
	/** @} */

	/** 获取要素类型编号
	* @param  无
	* @return 要素类型编号
	*/
	virtual FCLSID getID();
	/** 设置要素类型编号
	* @param  [in] id 要素类型编号
	* @return  无
	*/
	virtual void  setID(FCLSID id);
	/** 获取要素类型名称
	* @param   无
	* @return  要素类型名称
	*/
	virtual std::string getName();
	/** 设置要素类型名称
	* @param  [in] v 要素类型名称
	* @return 无
	*/
	virtual void  setName(std::string v);
	/** 设置要素类型编码
	* @param   无
	* @return  要素类型编码
	*/
	virtual std::string getCode();
	/** 获取要素类型编码
	* @param  [in] v 要素类型编码
	* @return 无
	*/
	virtual void  setCode(std::string v);
	/** 设置空间参考系统ID
	* @param  [in] srefID 空间参考系统ID
	* @return 无
	*/
	inline void  setSpatialReferenceID(SREFID srefID);
	/** 获取空间参考系统ID
	* @param  无
	* @return 空间参考系统ID
	*/
	inline SREFID  getSpatialReferenceID();
	/** 设置几何表的分区个数
	* @param  [in] v 分区个数
	* @return  无
	*/
	inline void  setPartitionNumber(int v);
	/** 获取几何表的分区个数
	* @param   无
	* @return 几何表的分区个数
	*/
	inline int  getPartitionNumber();
	/** 获取要素类型所在的工程
	* @param   无
	* @return  工程ID
	*/
	inline PRJID getPrjID();
	/** 设置要素类型所在的工程
	* @param  [in] prjID 工程ID
	* @return  无
	*/
	inline void  setPrjID(PRJID prjID);
	/** 获取要素类的状态
	* @param  无
	* @return 要素类的状态
	*/
	inline int getFclsType();
	/** 设置要素类的状态
	* @param  [in] fclstype 要素类的状态
	* @return 无
	*/
	inline void  setFclsType(int fclstype);
	/** 要素类拷贝函数
	* @param  [in] fc 指向要素类的引用
	* @return  无
	*/
	void copy(const FeatureClassImpl & fc);
	/** 要素类赋值函数
	* @param  [in] 指向要素类的引用
	* @return 要素类
	*/
	FeatureClassImpl operator=(const FeatureClassImpl & fc);
	/** 判断要素类是否相等
	* @param  [in] 指向要素类的引用
	* @return  相等 返回true 不相等 返回false
	*/
	bool operator==(const FeatureClassImpl & fc);
	/** @} */


	/** @name 要素类属性结构操作函数集
	*  @{
	*/
	/** 添加属性结构
	* @param  [in] pt属性结构的引用
	* @return 无
	*/
	void addPropType(const PropType & pt);
	/** 添加属性结构
	* @param  [in] name 属性名称
	* @param  [in] code 属性编码
	* @param  [in] type 属性类型
	* @param  [in] length 属性字段的长度
	* @param  [in] decimal 是否为小数
	* @param  [in] nullabe 是否为空
	* @return 无
	*/
	void addPropType(std::string name, std::string code, std::string type, int length, int decimal = 0, bool nullable = false);
	/** 删除属性结构
	* @param  [in] name 属性结构的名称
	* @return  无
	*/
	virtual void removePropType(std::string name);
	/**获取属性结构的集合
	* @param   无
	* @return  对应的属性字段集合
	*/
	inline std::vector<PropType> & getPropTypes();
	/** @} */


	/** @name 要素类关于要素集操作函数集（要素集已不用）
	*  @{
	*/
	/**设置要素类所在的要素集合
	* @param  [in] pfs 要素集合指针
	* @return  无
	*/
	inline void setLocalFeatureSet(FeatureSetSharedPtr pfs){ _localfeatureset = pfs; }
	/**获取要素类所在的要素集合
	* @param   无
	* @return 要素类所在的要素集合
	*/
	inline FeatureSetSharedPtr getLocalFeatureSet(){ return _localfeatureset; }
	/**获取要素类型的要素集合管理类型
	* @param 无
	* @return  要素集合管理类型
	*/
	inline int getFsetType();
	/**设置要素类型的要素集合管理类型
	* @param [in] v  要素类型的要素集合管理类型
	* @return  无
	*/
	inline void setFsetType(int v);
	/**获取要素类型的属性字段建表类型
	* @param   无
	* @return 要素类型的属性字段建表类型
	*/
	inline int getFsetPropType();
	/**设置要素类型属性字段建表类型
	* @param [in] v	属性字段建表类型
	* @return 无
	*/
	inline void setFsetPropType(int v);
	/** @} */


	/** @name 文件读写函数集
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


	/** @name 要素类Buffer函数操作集
	*  @{
	*/
	/** 获取要素类的Buffer大小
	* @param  [in] 无
	* @return Buffer大小
	*/
	size_t sizeBuffer();
	/**将要素类信息写入Buffer
	* @param  [in] buf
	* @return  无
	*/
	void writeBuffer(Buffer &buf);
	/**从Buffer读取要素类信息
	* @param  [in,out] buf
	* @return  无
	*/
	void readBuffer(Buffer &buf);
	/** @} */


public:
	virtual int getStatus() {
		return 0;
	}
	virtual void  setStatus(int s) {

	}

	virtual void copy(const FeatureClass & fc){
		copy((const FeatureClassImpl &)fc);
	}
	virtual bool isEqual(const FeatureClass & fc) {
		return *this == (const FeatureClassImpl &)fc;
	}

	virtual FeatureClassSharedPtr clone() {
		FeatureClassImpl * p = new FeatureClassImpl(*this);
		return FeatureClassSharedPtr((FeatureClass*)p);
	}
};
/** @} */ // End of the group block
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
