#pragma once
#include "feature.h" 
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace 
/** @defgroup Builder 要素对象接口
*  @{
*/
class FeatureImpl : public Feature {
	/** @name 保护成员变量说明
	*  @{
	*/
protected:
	/** 要素ID*/
	FID    _id;
	/** 要素名称*/
	std::string   _name;
	/** 图形的状态值,初始化值为 0x0000;//0位： 为1-表示逻辑删除 //1位： 为1-表示隐藏	//2位： 为1-表示选中*/
	/** 3位： 为1-表示显示点//7位： 为1-表示显示方向，否则设为0 //8位： 为1-表示具有语义拓扑信息，否则设为0//9－15位 ：保留*/
	int   _state;
	/** 要素LOD的总级数*/
	int  _lod;
	/** LOD信息*/
	LODInfoSharedPtr  _lod_info;
	/** SDO_GEOMETRY，为了创建R树索引*/
	//SDO_GEOMETRY* _orclBound;
	/** 要素分区ID*/
	WKSPID  _regid;
	//double        _bound[6];//X的最小最大，Y的最小最大，Z的最小最大
	/** 指向几何信息*/
	GeometrySharedPtr   _geom;
	//Topology*     _topo;//指向拓扑信息缓冲区	
	/** 创建时间*/
	GDBTIME  _createTime;
	/** 更新时间*/
	GDBTIME  _updateTime;
	/** 要素对象对应的要素类*/
	FCLSID  _fclsid;
	/** 存放要素的属性*/
	std::vector<std::string> _props;
	/** 要素类型编码*/
	string		  _code;
	/** 去读要素时是否有读取属性 0没有读；1读取了*/
	bool _propreaded;
	/** 存buffer的大小*/
	unsigned long    _buffersize;
	/** @} */


public:
	/** @name 构造与析构函数
	*  @{
	*/
	FeatureImpl();
	FeatureImpl(FeatureClassSharedPtr fc, FID id1, std::string name, int lod, REGID regid, int state = 0, GeometrySharedPtr pg = GeometrySharedPtr(),
		GDBTIME createTime = 0, GDBTIME updateTime = 0, unsigned long buffersize = 0);
	FeatureImpl(FCLSID fc, FID id1, std::string name, int lod, REGID regid, int state = 0, GeometrySharedPtr pg = GeometrySharedPtr(),
		GDBTIME createTime = 0, GDBTIME updateTime = 0, unsigned long buffersize = 0);
	FeatureImpl(FeatureImpl & f);
	void copy(const FeatureImpl & f);

	void copy(FeatureSharedPtr sp);
	virtual ~FeatureImpl();
	/** @} */

public:
	/** @name 要素Feature操作函数集
	*  @{
	*/
	/** 设置要素的ID
	* @param  [in] id  要素ID
	* @return 无
	*/
	inline void setID(FID id);
	/** 获取要素的ID
	* @param  无
	* @return 要素ID
	*/
	inline FID getID();
	/** 获取要素的名称
	* @param   无
	* @return 要素名称
	*/
	inline std::string getName();
	/** 设置要素的名称
	* @param  [in]  v 要素名称
	* @return 无
	*/
	inline void setName(std::string v);
	/** 获取要素的LOD总级别数
	* @param  无
	* @return 要素的LOD总级别数
	*/
	inline int getLODLevels();
	/** 设置要素的LOD总级别数
	* @param  [in]  v 总级别数
	* @return 无
	*/
	inline void setLODLevels(int v);
	/** 获取要素的LOD信息
	* @param  无
	* @return 要素的LOD信息
	*/
	inline LODInfoSharedPtr getLODInfo();
	/** 设置要素的LOD信息
	* @param  [in]  v LOD信息
	* @return  无
	*/
	inline void setLODInfo(LODInfoSharedPtr v);
	/** 获取要素的分区ID
	* @param  无
	* @return 要素的LOD信息
	*/
	inline REGID getWorkspaceID();
	/** 设置要素分区ID
	* @param  [in] v 要素分区ID
	* @return 无
	*/
	inline void setWorkspaceID(REGID v);
	/** 设置要素编码
	* @param  [in] szCode 要素分区ID
	* @return 无
	*/
	inline void setCode(string szCode);
	/** 获取要素编码
	* @param  无
	* @return 要素编码
	*/
	inline std::string getCode();
	/** 获取要素的外包围盒
	* @param  无
	* @return 要素的外包围盒
	*/

	virtual void getEnvelope(Envelope3dRef d){
		_geom->getEnvelope(d);
	}
	/** 设置要素的外包围盒
	* @param   env3d 要素的外包围盒
	* @return 无
	*/
	virtual void setEnvelope(const Envelope3dRef env3d);
	/** 获取要素的外包围盒
	* @param  [out] pv 要素的外包围盒
	* @return 无
	*/

	virtual void getEnvelope(double *d);
	/** 设置要素的外包围盒
	* @param  [in] d[6] 要素的外包围盒
	* @return 无
	*/
	virtual void setEnvelope(double *d);
	/** 获取要素创建时间
	* @param  无
	* @return GDBTIME 创建时间
	*/
	inline GDBTIME getCreateTime();
	/** 设置要素创建时间
	* @param  [in] v 创建时间
	* @return 无
	*/
	inline void setCreateTime(GDBTIME v);
	/** 获取要素更新时间
	* @param   无
	* @return  更新时间
	*/
	inline GDBTIME getUpdateTime();
	/** 设置要素更新时间
	* @param  [in] v 更新时间
	* @return 无
	*/
	inline void setUpdateTime(GDBTIME v);
	/** 获取要素图形的状态值
	* @param   无
	* @return 要素图形的状态值
	*/
	inline int getStatus();
	/** 设置要素图形的状态值
	* @param  [in]  v 图形的状态值
	* @return 无
	*/
	inline void setStatus(int v);
	/** 设置要素所属的要素类
	* @param  [in] fc 要素类指针
	* @return 无
	*/
	inline void setFeatureClass(FeatureClassSharedPtr fc);
	inline void setFeatureClassID(FCLSID);
	/** 获取要素所属的要素类
	* @param  无
	* @return 要素类指针
	*/
	inline FeatureClassSharedPtr getFeatureClass();
	/** 获取要素所属的要素类ID
	* @param   无
	* @return 要素类ID
	*/
	inline FCLSID getFeatureClassID();
	/** 获取该要素是否有组成语义信息_该信息按位存储在_state变量中
	* @param  无
	* @return 要素类ID
	*/
	inline bool hasSemantic();
	/** 设置该要素是否有组成语义信息_该信息按位存储在_state变量中
	* @param  [in] hassemantic 该要素是否有组成语义信息
	* @return true 有 false 无
	*/
	inline void hasSemantic(bool hassemantic);
	/** @} */


	/** @name 要素Feature属性操作函数集
	*  @{
	*/
	/** 获取要素的属性字段集合
	* @param  无
	* @return 要素的属性字段集合
	*/
	virtual const std::vector<std::string>& getProps() const;
	virtual   std::vector<std::string>& getProps();
	virtual void getProps(std::vector<std::string> & sa);
	///** 设置要素的属性字段集合大小
	//* @param   无
	//* @return 属性字段集合大小
	//*/
	//inline int setPropSize();
	/** 设置要素的属性字段集合
	* @param  [in]  prop 属性字段集合
	* @return 无
	*/
	inline void setProps(std::vector<std::string>& prop);
	/** 获取读取要素时，是否有读取属性
	* @param  无
	* @return true 读取了 false 没有读
	*/
	inline bool getPropAccessFlag();
	/** 获取读取要素时，是否有读取属性
	* @param  无
	* @return true 读取了 false 没有读
	*/
	inline void setPropAccessFlag(bool propreaded);

	virtual bool setProp(int idex, const std::string & sz){
		if (_props.size() <= idex)
			return false;
		else{
			_props[idex] = sz;
			return true;
		}
	}

	/** @} */



public:
	/** @name 几何信息函数集
	*  @{
	*/
	/** 获取要素的几何信息
	* @param  无
	* @return Geometry 几何信息
	*/
	inline GeometrySharedPtr geometry();

	/** 拷贝输入的Geometry，然后绑定到当前feature上
	* @param  [in] g  Geometry指针
	* @return true 绑定成功 false 绑定失败
	*/
	inline void setGeometry(GeometrySharedPtr g);
	/**替换要素的几何信息
	* @param  [in] g  Geometry指针
	* @return true 绑定成功 false 绑定失败
	*/
	inline bool attatchGeometry(GeometryPtr g);
	/** 判断指定LOD级别的Geometry对象是否已经调出
	* @param  [in] flod  要素LOD的ID
	* @return true 绑定成功 false 绑定失败
	*/
	virtual bool isExist(FLOD flod);
	/** 获取指定LOD级别的Geometry
	* @param  [in] flod  要素LOD的ID
	* @return  几何信息指针
	*/
	GeometrySharedPtr getLodGeom(FLOD flod);
	/** @} */

	virtual GeometrySharedPtr getGeometry(int LODLevel = -1){
		if (LODLevel <= 0)
			return _geom;
		else
			return getLodGeom(LODLevel);
	}

public:
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

public:
	/** @name 内存读写函数集
	*  @{
	*/
	/** 获取要素的Buffer大小
	* @param   无
	* @return Buffer大小
	*/
	size_t sizeBuffer();
	/**将feature信息写入Buffer
	* @param  [in] buf
	* @return  无
	*/
	void writeBuffer(Buffer & buf);
	/**从Buffer读取feature信息
	* @param  [in] buf
	* @return  无
	*/
	void readBuffer(const Buffer & buf);
	/** 获取要素除属性信息后的Buffer大小
	* @param  无
	* @return Buffer大小
	*/
	size_t sizeBufferNoProp();
	/**将feature去除属性信息后的内容写入Buffer
	* @param  [in] buf
	* @return  无
	*/
	void writeBufferEndProp(Buffer & buf);
	/**读取feature去除属性信息后的Buffer
	* @param  [in,out] buffer信息
	* @return  无
	*/
	void readBufferNoProp(const Buffer & buf);
	/** 获取要素的Buffer大小，此函数供缓存使用
	* @param  无
	* @return 要素的Buffer长度
	*/
	inline unsigned long getBufferSize();
	/** 设置要素的Buffer大小，此函数供缓存使用
	* @param  [in]  Buffer大小
	* @return 无
	*/
	inline void  setBufferSize(unsigned long buffersize);
	/** @} */


	/** @name 要素的几何对象操作函数集
	*  @{
	*/
	/** 根据传入参数，生成变换矩阵，完成对象的几何变换
	* @param  [in]  dx  x值
	* @param  [in]  dy  y值
	* @param  [in]  dz  z值
	* @return 无
	*/
	void transferCoordinate(double dx, double dy, double dz);
	/** 根据传入参数，生成变换矩阵，完成对象的几何变换
	* @param  [in]  v3d  顶点值
	* @return 无
	*/
	void transferCoordinate(Vertex3d& v3d);
	/** 根据传入参数，生成变换矩阵，完成对象的几何变换
	* @param  [in]  m4x4  矩阵值
	* @return 无
	*/
	void transferCoordinate(Matrix4x4& m4x4);
	/** @} */


	/** @name 公用成员变量说明
	*  @{
	*/
public:
	/**  声明友类*/
	friend class FeatureSet;
	friend class Feature;
	/** @} */
};
/** @} */ // End of the group block

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
