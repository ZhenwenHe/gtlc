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

#include "connector.h"
#include "Proxy.h"


begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

class Builder;

/** @defgroup Builder Builder-空间数据库编辑接口  
*  @{
*/
typedef std::shared_ptr<Proxy> ProxySharedPtr;
typedef std::shared_ptr<Builder> BuilderSharedPtr;

class GV3DCORE_API Builder  {

	/** @name 保护成员变量说明  
	*  @{
	*/
protected:
	/** 查询接口指针 */
	ProxySharedPtr _proxySptr;
	/** Builder类型，可以是文件数据库，关系数据库和通信传输类型 */
	int _builderType;
	/** 角色类型 */
	string    _role;
	/** 是否选择快速插入数据 : true为快速插入，false为需要判断是否存在ID */
	bool _quicktype; 
	/** 临时操作的中间结果记录，不做外存储 */
	TemporarySets  _tempsets;
	/** 选择集，不做外存储 */
	Selector       _selector;
	/** 数据库连接对象 */
	Connector      _connector;
	/** @} */ 
public:
	static BuilderSharedPtr newBuilder(int signal);
public:
	/** @name 构造与析构函数 
	*  @{
	*/
	Builder();
	virtual ~Builder();
	/** @} */ 


	/** @name 要素类FeatureClass操作函数集  
	*  @{
	*/
	/** 向库中添加一个FeatureClass
	* @param  [in] pfc 要添加的FeatureClass共享指针对象
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual bool addFeatureClass( FeatureClassSharedPtr pfc) = 0;
	/** 修改库中的一个FeatureClass，此要素对象必须存在于数据库中，否则不执行
	* @param  [in] p 要素类型对象
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual bool modifyFeatureClass( FeatureClassSharedPtr p) = 0;
	/** 从库中删除一个指定的FeatureClass
	* @param  [in] fclsid 要素类型的唯一ID
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual bool removeFeatureClass( const FCLSID &fclsid) = 0;


	/** 比较两个FeatureClass的名称、结构等是否相等
	* @param  [in] fclsid1 要素类型的唯一ID
	* @param  [in] fclsid2 要素类型的唯一ID
	* @return 成功合并的FeatureClass个数
	*/
	virtual long mergeFeatureClass( const FCLSID & fclsid,vector<FCLSID> &fclsids);
	/** @} */ 


	/** @name 要素与要素集合操作函数集  
	*  @{
	*/
	/** 向库中添加一个Features对象
	* @param  [in] pFeature 要素类Feature的共享指针对象
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual bool addFeature(FeatureSharedPtr pFeature ) = 0;
	/** 添加多个Feature对象进入库中,返回成功添加的个数
	* @param  [in] aFeatures 要素Feature的共享指针对象的数组
	* @return 如果执行成功返回成功个数
	*/
	virtual long addFeatures(std::vector<FeatureSharedPtr>& aFeatures ) = 0; 
	/** 修改要素对象的全部数据，此要素对象必须存在于数据库中，否则不执行
	* @param  [in] pFetaure  要素Feature的共享指针对象 
	* @return 如果执行成功则返回true,否则返回false
	*/	
	virtual bool modifyFeature(FeatureSharedPtr& pFetaure) = 0;
	/** 从数据库中移除指定要素对象
	* @param  [in] fid 要素Feature的唯一ID
	* @param  [in] fclsid 要素Feature的所在的要素类型FeatureClass的唯一ID 
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual bool removeFeature(const FID & fid, const FCLSID& fclsid) = 0;
	/** 修改要素对象的几何数据，此要素对象必须存在于数据库中，否则不执行
	* @param  [in,out] pFetaure 要素Feature的共享指针对象
	* @param  [in] pGeom  新的几何对象，用该几何对象替换Feature中原有的几何对象 
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual bool modifyGeometry(FeatureSharedPtr& pFetaure,const GeometrySharedPtr & pGeom ) = 0;
	/** 修改要素对象的属性数据，此要素对象必须存在于数据库中，否则不执行
	* @param  [in,out] pFetaure 要素Feature的共享指针对象
	* @param  [in] fields  要修改属性字段的名称 
	* @param  [in] values  要修改属性字段的对应的属性值 
	* @return 如果执行成功则返回true,否则返回false
	*/	
	virtual bool modifyProperty(FeatureSharedPtr& pFetaure, vector<string>& fields, vector<string>& values) = 0;
	/** 将要素集合中的要素添加到数据库中
	* @param  [in] fsp 要素FeatureSet的共享指针对象 
	* @return 如果执行成功则返回大于0的整数,否则返回0
	*/
	virtual long addFeatureSet(FeatureSetSharedPtr fsp)=0;
	/** 将Feature对象关联的LOD对象插入数据库
	* @param  [in,out] pFeature 要素Feature的共享指针对象，其中包含的LOD对象是新的LOD信息
	* @return 如果执行成功则返回true,否则返回false
	*/	
	virtual bool addLOD(FeatureSharedPtr pFeature) =0; 
	/** @} */ 

	/** @name 拓扑信息操作函数集  
	*  @{
	*/
	/** 增加一个拓扑对象到数据库
	* @param [in]  topologyItem 需要增加的拓扑对象
	* @return 操作成功返回非0整数，否则返回0
	*/
	virtual long addTopo(const TopoItem& topologyItem) = 0;
	/** 删除一个拓扑对象
	* @param [in]  topologyItem 需要删除的拓扑对象
	* @return 操作成功返回非0整数，否则返回0
	*/
	virtual long deleteTopo(const TopoItem& topologyItem) = 0;
	/** 增加一个一组拓扑对象
	* @param [in]  topologyItems 需要增加的拓扑对象集合
	* @return 操作成功返回非0整数，否则返回0
	*/	
	virtual long addTopos(const vector<TopoItemSharedPtr>& topologyItems) = 0;
	/**增加一个语义拓扑信息对象，传入的语义拓扑信息对象通过 share_pointer 由使用者共同维护
	* @param [in]  topologyInfo 需要增加的语义拓扑信息对象
	* @return 操作成功返回非0整数，否则返回0
	*/
	virtual long addTopo(const TopoInfoSharedPtr& topologyInfo) = 0;
	/** 删除一个语义拓扑信息对象，在库中查找到与此组对象完全匹配的记录，则删除，否则不处理
	* @param [in]  fid 需要删除的语义拓扑信息对象所属的Feature 的ID
	* @param [in]  nType 需要删除的语义拓扑信息对象所属的类型
	* @return 操作成功返回非0整数，否则返回0
	*/
	virtual long deleteTopo(const FID  &fid, const int &nType) = 0;
	/** 增加一组语义拓扑信息对象，传入的语义拓扑信息对象通过 share_pointer 由使用者共同维护
	* @param [in]  topologyInfos 需要增加的语义拓扑信息对象集合
	* @return 操作成功返回非0整数，否则返回0
	*/	
	virtual long addTopos(const vector<TopoInfoSharedPtr>& topologyInfos) = 0;
	/**  删除一组语义拓扑信息对象，在库中查找到与此组对象完全匹配的记录，则删除，否则不处理
	* @param [in]  fids 需要删除的语义拓扑信息对象所属的Feature 的ID
	* @param [in]  types 需要删除的语义拓扑信息对象所属的类型
	* @return 操作成功返回非0整数，否则返回0
	*/	
	virtual long deleteTopos(const vector<FID>& fids,const  vector<int>& types) = 0;
	/**  存储topo的六个值到数据库中
	* @param [in]  user 数据库的用户名
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual bool saveTopoTypes(std::string user) = 0;
	/**  存储topo的一个类型到数据库中
	* @param [in]  typenum 存入的topo编号
	* @param [in]  relation topo关系说明
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual bool addTopoType(int typenum,std::string relation) = 0;
	/** @} */

	/** @name 索引操作函数集  
	*  @{
	*/
	/** 判断存储空间索引的文件或者字段是否存在
	* @param 无
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual bool isIndexExist()=0;
	/** 重新构建整个工程级别的索引，索引信息存放在_prj中
	* @param  [in] pBox 包围盒指针，划分网格的空间范围
	* @param  [in] dx X方向的网格间隔
	* @param  [in] dy Y方向的网格间隔
	* @param  [in] dz Z方向的网格间隔
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual bool recreateIndexes(Envelope3d * pBox=NULL,int dx=0,int dy=0,int dz=0) = 0;
	/** 将空间索引存储到索引文件中
	* @param 无
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual bool saveIndexes()=0; 
	/** 删除空间索引的文件
	* @param 无
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual bool deleteIndexes()=0;
	/** @} */ 


	/** @name 材质操作函数集  
	*  @{
	*/	
	/**  向数据库中递交一个材质数据
	* @param [in] material 要提交的材质对象共享指针对象
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual bool addMaterial(MaterialSharedPtr material) = 0;
	/**  从数据库中删除一个ID为nMatID的材质对象
	* @param [in] nMatID 要删除的材质对象唯一ID
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual MaterialSharedPtr removeMaterial(const MATID &nMatID) = 0;
	/**  在数据库中修改一个已经存在的材质数据，如果传入材质的ID已经存在则替换，不存在则直接添加到材质库
	* @param [in] matPtr 新的材质对象的材质对象
	* @return 如果执行成功则返回true,否则返回false
	*/	
	virtual bool modifyMaterial( MaterialSharedPtr matPtr) = 0;
	/**  在数据库中查询使用了指定材质的所有对象集
	* @param [in] nMatID 材质对象唯一ID
	* @param [in,out] fs 使用了指定材质的所有Feature对象集
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual bool queryFeatureByMaterial(const MATID &nMatID,vector<FeatureSharedPtr>& fs) = 0;
	/** @} */ 


	/** @name 纹理操作函数集  
	*  @{
	*/	
	/**  向数据库中递交一个纹理数据
	* @param [in] m 要提交的纹理对象共享指针对象
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual bool addTexture(TextureSharedPtr m,int iLevel = 0) = 0;
	/**  从数据库中删除一个ID为nID的纹理对象
	* @param [in] nID 要删除的纹理对象唯一ID
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual TextureSharedPtr removeTexture(const TEXID &nID,int iLevel = 0) = 0;
	/**  在数据库中修改一个已经存在的纹理数据
	* @param [in] t 新的纹理对象
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual bool modifyTexture( TextureSharedPtr t,int iLevel = 0) = 0;
	/**  在数据库中查询使用了指定纹理的所有材质的ID集合
	* @param [in] nID 纹理对象唯一ID
	* @param [in,out] matIdVec 使用了指定纹理的所有materail的ID集合
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual  bool queryMaterialByTexture(const TEXID& nID, vector<MATID> &matIdVec) = 0 ;
	/**  在数据库中查询使用了指定纹理的所有材质对象集
	* @param [in] nID 纹理对象唯一ID
	* @param [in,out]  使用了指定纹理的所有materail对象集
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual  bool queryMaterialByTexture(const TEXID &nID,vector<MaterialSharedPtr>&) =0 ;
	/** @} */ 

	/** @name 共享模型操作函数集  
	*  @{
	*/
	/**  向数据库中递交一个共享模型数据
	* @param [in] m 要提交的共享模型象共享指针对象
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual bool addSharedModel(SharedModelSharedPtr m) = 0;
	/**  从数据库中删除一个ID为nID的共享模型对象
	* @param [in] nID 要删除的共享模型对象唯一ID
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual bool removeSharedModel(const MDLID &nID) = 0;
	/**  在数据库中修改一个已经存在的共享模型数据
	* @param [in] t 新的共享模型对象
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual bool modifySharedModel( SharedModelSharedPtr t) = 0;
	/**  在数据库中查询使用了指定共享模型的所有对象集
	* @param [in] nID 材质对象唯一ID
	* @param [in,out] 使用了指定共享模型的所有Feature对象集
	* @return 如果执行成功则返回true,否则返回false
	*/
	virtual  bool queryFeatureBySharedModel(const MDLID& nID,vector<FeatureSharedPtr>& ) = 0;
    /** @} */ 

	/** @name 其它操作函数集  
	*  @{
	*/	
	/**  获取builder的类型
	* @param [in] 无
	* @return 返回1为文件系统,否则返回2为数据库系统
	*/
	virtual int getType();

	/**  获取临时操作的中间结果记录
	* @param [in] 无
	* @return 返回临时操作中间结果记录TemporarySets
	*/
	virtual TemporarySets & getTemporarySets();

	/**  获取选择集合
	* @param [in] 无
	* @return 返回临时操作中间结果记录TemporarySets
	*/
	virtual Selector & getSelector();

	/**  删除选择集中的要素对象
	* @return 返回删除的个数 
	*/
	long eraseSelectedFeatures();

	/**  获取ProxySharedPtr指针
	* @param [in] 无
	* @return 返回proxy指针
	*/
	virtual ProxySharedPtr getProxy() = 0;

	/** 保存数据库
	* @param  无
	* @return 无
	*/
	virtual void save() = 0;

	/** 关闭数据库
	* @param  无
	* @return 无
	*/
	virtual void close() = 0;

	/** 清空数据库
	* @param  无
	* @return 无
	*/
	virtual void removeAll() = 0;

	/** 清空数据库 
	* @param  
	* @return 无
	*/
	virtual void clear( ) = 0;

	/** 激活当前buider
	* @param  无
	* @return 无
	*/
	virtual void setActive()=0;
	/** 打开数据库
	* @param  Connector: c数据库连接对象
	* @return int: 0为正常打开
	*/
	virtual int open(Connector c)=0;
	/** 新建数据库
	* @param  Connector: c数据库连接对象
	* @return int: 成功
	*/
	virtual bool create(const Connector & c)=0;

	/**  获取工程指针
	* @param 无 
	* @return ProjectSharedPtr：工程对象指针
	*/
	virtual ProjectSharedPtr getProject()=0;

	/**  判断工程是否为空
	* @param 无 
	* @return 如果为空返回true，否则返回false
	*/
	virtual bool isEmpty()=0;

	/**  获取数据库中的所有的要素类
	* @param 无 
	* @return std::map<FCLSID,FeatureClassSharedPtr> ：数据库中所有要素类的映射表
	*/
	virtual std::map<FCLSID,FeatureClassSharedPtr> & getFeatureClassMap() =0;

	/** 将工程坐标平移
	* @param  [in] dx	x坐标平移数
	* @param  [in] dy	y坐标平移数
	* @param  [in] dz	z坐标平移数
	* @return 无
	*/
	virtual void translate(double dx, double dy, double dz);

	/**  对工程进行克隆
	* @param [in] targetBuilder 目标工程 
	* @return 返回目标工程Builder指针
	*/
	virtual BuilderSharedPtr  clone( BuilderSharedPtr targetBuilder);


	/**  对数据库进行裁剪，得到的结果保存到resultBuilder中
	* @param [in,out] resultBuilder 
	* @return 执行成功返回true，否则返回false
	*/
	virtual bool clip( const Envelope3dRef  bound, Builder * resultBuilder) ;

	/**  对数据库进行选择性排除，包围盒在bound之外的则删除
	* @param [in] bound 指定包围盒
	* @return 执行成功返回true，否则返回false
	*/
	virtual bool exclude( const Envelope3dRef  );

	/**  将fromBuilder中的数据导入进本数据库中
	* @param [in] fromBuilder 
	* @return 执行成功返回true，否则返回false
	*/
	virtual bool import( Builder * fromBuilder) ;
	/**  设置缓冲的大小
	* @param [in] fpsize  要素缓冲的大小，单位为字节数 
	* @param [in] mpsize  材质缓冲的大小，单位为字节数 
	* @param [in] tpsize  纹理缓冲的大小，单位为字节数 
	* @param [in] spsize  共享模型缓冲的大小，单位为字节数 
	* @return 执行成功返回true，否则返回false
	*/	
	virtual bool setCacheSizes(unsigned long long fpsize,unsigned long long  mpsize,unsigned long long  tpsize,unsigned long long  spsize)=0; 
};
/** @} */ 

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
