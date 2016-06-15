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
//直接在GV3dCore中实现
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include "property.h"
#include "buffer.h"
#include "geometry2d.h"
#include "geometry3d.h"
#include "topology.h"



begin_gtl_namespace
begin_gdb_namespace

#ifndef POINTRADIUS
/** 包围盒的最小有效值  */
#define  POINTRADIUS 0.000001 
#endif


// 这些信息存储在主文件的附属文件中，可以供建立索引和添加数据使用
struct FeatureIndexItem{
	FID    _fid; //对象的ID
	FCLSID _fclsid; //对象
	int    _state; //标示记录记录的状态：0原始数据；1删除标示；2更新标示
	unsigned long _offset; //记录文件指针的起始位置:相对于文件头进行偏移的长度
	unsigned long _bufLength; //记录该块buf的长度
	unsigned long _bufLengthNoPro;//记录feature没有属性信息的长度
	GDBTIME  _writeTime;	//记录文件写入的时间
	GDBTIME  _updateTime;	//记录文件写入的时间
	double _bound[6];//对象的包围盒:供建立索引使用
};

//查找Vector中是否已经存在给定的要素对象  [HZW, 2009-8-18]
struct FeatureItemExist 
{ 
	FeatureIndexItem Target; 
	FeatureItemExist( const FeatureIndexItem& t ):Target(t){}; 
	bool operator()( const FeatureIndexItem *s ) {
		return Target._fid == s->_fid && Target._state == s->_state; 
	} 
}; 




/** @defgroup Feature Feature-要素相关类 
*  @{
*/
class  Project;
class  Workspace;
class  Layer;
class  FeatureSet;
class  Feature;
class  FeatureClass;
class  LODInfo;


typedef std::shared_ptr<Project> ProjectSharedPtr;
typedef Project*  ProjectPtr;
typedef Project&  ProjectRef;

typedef std::shared_ptr<Workspace> WorkspaceSharedPtr;
typedef Workspace*  WorkspacePtr;
typedef Workspace&  WorkspaceRef;

typedef std::shared_ptr<Layer> LayerSharedPtr;
typedef Layer*  LayerPtr;
typedef Layer&  LayerRef;

typedef std::shared_ptr<FeatureSet> FeatureSetSharedPtr;
typedef FeatureSet*  FeatureSetPtr;
typedef FeatureSet&  FeatureSetRef;
typedef std::shared_ptr<Feature> FeatureSharedPtr;
typedef Feature&  FeatureRef;
typedef std::weak_ptr<Feature> FeatureWeakPtr;
typedef std::shared_ptr<FeatureClass> FeatureClassSharedPtr;
typedef FeatureClass& FeatureClassRef;
typedef FeatureClass* FeatureClassPtr;
typedef std::shared_ptr<LODInfo> LODInfoSharedPtr;
typedef LODInfo* LODInfoPtr;
typedef LODInfo& LODInfoRef;
typedef std::vector<FeatureSharedPtr > FeatureSharedPtrVector;
typedef std::vector< FeatureWeakPtr >   FeatureWeakPtrVector;
typedef std::shared_ptr<std::vector< FeatureSharedPtr> > FeatureVectorSharedPtr;

typedef Identifier::raw_type FCLSID;
typedef Identifier::raw_type FID;
typedef Identifier::raw_type FSID;
typedef Identifier::raw_type LAYERID;
/** @} */


/** @addtogroup Feature LODInfo 
*  @{
*/
class CORE_API LODInfo {
public:
	/**LOD结构体*/
	struct LOD_PARAM{
		int			nDrawFlag;		//[是否用switch距离，以何种方式绘制]
		double		dSwitchIn;		//该LOD层次的最近可视距离
		double		dSwitchOut;		//该LOD层次的最远可视距离
		double		nDataSize;		//该LOD层次的几何数据数据量
		int			nTriangleNum;	//该LOD层次的几何数据三角面片个数
		int			nVertexNum;		//该LOD层次的几何数据顶点数
		double		reserve[4];		//保留字段
	};

	/**LOD描述信息（LOD BLOB存储的信息）*/
	struct LOD_INFO {
		int			lodLevel;		//LOD的级别
		LOD_PARAM	lodparam;		//LOD描述信息
	};

	typedef std::pair<LOD_INFO/*可视距离*/,GeometrySharedPtr> Info;	
	typedef std::vector<Info>   InfoArray;
public:
	virtual  int getCurLevel()=0;
	virtual int getLevel(double dis)=0;
	virtual void setID(FID id)=0;
	virtual FID getID()=0;
	virtual int getLevels()=0;
	virtual InfoArray & getLods()=0;
	virtual bool addInfo(const LOD_INFO &lodinfo, GeometrySharedPtr g)=0;
	virtual size_t sizeBuffer()=0;
	virtual bool writeBuffer(Buffer& buf) =0;
	virtual bool readBuffer(const Buffer & buf)	=0;
	virtual void write(std::ostream & f)=0;
	virtual void read(std::istream & f)=0;
public:
	static LODInfoSharedPtr create();
};
/** @} */

/** @addtogroup Feature FeatureClass 
*  @{
*/
class CORE_API FeatureClass {
public:
	virtual FCLSID getID()=0;
	virtual void  setID(FCLSID id )=0;
	virtual std::string getName()=0;
	virtual void  setName(std::string v )=0;	
	virtual std::string getCode()=0;
	virtual void  setCode(std::string v )=0;
	virtual int getStatus()=0;
	virtual void  setStatus(int s )=0;	
	virtual void copy(const FeatureClass & fc )=0;
	virtual bool isEqual(const FeatureClass & fc)=0;
	virtual void addPropType(const PropType & pt)=0;
	virtual void addPropType(std::string name,std::string code,std::string type,int length,int decimal=0,bool nullable=false)=0;
	virtual void removePropType(std::string name)=0;	
	virtual std::vector<PropType> & getPropTypes()=0;
	virtual void setLocalFeatureSet(FeatureSetSharedPtr pfs)=0;
	virtual FeatureSetSharedPtr getLocalFeatureSet()=0;
	virtual void write(std::ostream & f)=0;
	virtual void read(std::istream & f)=0;
	virtual size_t sizeBuffer()=0;
	virtual void writeBuffer(Buffer &buf)=0;
	virtual void readBuffer(Buffer &buf)=0;
	virtual FeatureClassSharedPtr clone()=0;
	virtual SREFID  getSpatialReferenceID()=0;
	virtual int  getPartitionNumber()=0;
	virtual void  setSpatialReferenceID(SREFID)=0;
	virtual void  setPartitionNumber(int )=0;
	
public:
	static FeatureClassSharedPtr findFeatureClass(FCLSID fclsid);
	static std::map<FCLSID,FeatureClassSharedPtr> & getFeatureClassMap();
	static void setFeatureClassMap(std::map<FCLSID,FeatureClassSharedPtr> * p);


	static FeatureClassSharedPtr create();
	static FeatureClassSharedPtr create(std::string name,std::string code);
	static FeatureClassSharedPtr create(FCLSID id, std::string name, std::string code);
};
/** @} */

/** @addtogroup Feature Feature  
*  @{
*/
class CORE_API Feature {
public:
	enum{
		STATUS_DELETED =0x00000001,
		STATUS_HIDED   =0x00000002,
		STATUS_SELECTED=0x00000004
	};
	virtual  void setID(FID id)=0 ;

	virtual FID getID()=0;
    // 获取要素对象名称

	virtual std::string getName()=0;
    // 设置要素对象名称

	virtual void setName(
        // 要素对象的名称
        std::string name)=0;

	virtual REGID getWorkspaceID()=0;
	/** 设置要素分区ID
	* @param  [in] v 要素分区ID
	* @return 无
	*/
	virtual void setWorkspaceID(REGID v)=0;

	virtual void setCode(std::string szCode)=0;

	virtual std::string getCode()=0;
	/** 图形的状态值,初始化值为 0x0000;//0位： 为1-表示逻辑删除 //1位： 为1-表示隐藏	//2位： 为1-表示选中*/	
	/** 3位： 为1-表示显示点//7位： 为1-表示显示方向，否则设为0 //8位： 为1-表示具有语义拓扑信息，否则设为0//9－15位 ：保留*/
	virtual int getStatus()=0;

	virtual void setStatus(int v)=0;

	virtual bool getPropAccessFlag()=0;

	virtual void setPropAccessFlag(bool propreaded)=0;


	virtual int getLODLevels()=0;
 
	virtual void setLODLevels(int v)=0;

	virtual LODInfoSharedPtr getLODInfo()=0;

	virtual void setLODInfo(LODInfoSharedPtr )=0;
	

	virtual void setFeatureClass(FeatureClassSharedPtr fc)=0;
	virtual void setFeatureClassID(FCLSID )=0;

	virtual FeatureClassSharedPtr getFeatureClass()=0;

	virtual FCLSID getFeatureClassID()=0;

	virtual void getProps( std::vector<std::string> & )=0;

	virtual bool setProp(int idex,const std::string & sz)=0;

	virtual const std::vector<std::string> & getProps()const =0;
	virtual std::vector<std::string> & getProps() =0;

	virtual void setProps(std::vector<std::string>&  )=0;

	virtual GeometrySharedPtr getGeometry(int LODLevel=-1)=0;

	virtual void setGeometry(GeometrySharedPtr)=0;

	virtual bool attatchGeometry (Geometry * g)=0;


	virtual void copy(FeatureSharedPtr sp)=0;


	virtual void write(std::ostream & f)=0;

	virtual void read(std::istream & f)=0;

	virtual size_t sizeBuffer()=0;

	virtual void writeBuffer(Buffer & buf)=0;

	virtual void readBuffer(const Buffer & buf)=0;

	/** 获取要素的Buffer大小，此函数供缓存使用，避免每次计算，提高速度
	* @param  无 
	* @return 要素的Buffer长度
	*/
	virtual unsigned long getBufferSize()=0;
	/** 设置要素的Buffer大小，此函数供缓存使用，避免每次计算，提高速度
	* @param  [in]  Buffer大小
	* @return 无
	*/
	virtual void  setBufferSize(unsigned long buffersize )=0;

	virtual void getEnvelope(Envelope3dRef d)=0;
	virtual void setEnvelope(const Envelope3dRef d)=0;
	virtual void getEnvelope(double * )=0;
	virtual void setEnvelope(double *)=0;

	virtual bool isExist(FLOD f)=0;

	virtual GDBTIME getCreateTime()=0;
	virtual void setCreateTime(GDBTIME v)=0;
	virtual GDBTIME getUpdateTime()=0;
	virtual void setUpdateTime(GDBTIME v)=0;


	/** 获取要素除属性信息后的Buffer大小
	* @param  无
	* @return Buffer大小
	*/
	virtual size_t sizeBufferNoProp()=0;
	/**将feature去除属性信息后的内容写入Buffer
	* @param  [in] buf
	* @return  无
	*/
	virtual void writeBufferEndProp(Buffer & buf)=0;
	/**读取feature去除属性信息后的Buffer
	* @param  [in,out] buffer信息
	* @return  无
	*/
	virtual void readBufferNoProp(const Buffer & buf)=0;

public:
	/** @name 静态成员函数说明  
	*  @{
	*/
	/** 创建要素对象
	* @param  无
	* @return feature 指针
	*/
	static FeatureSharedPtr create();
	/** 设置要素的ID
	* @param  [in] fc  要素类指针
	* @param  [in] id1  要素ID
	* @param  [in] name 要素名称
	* @param  [in] lod   lod数
	* @param  [in] regid  区域ID
	* @param  [in] state  要素状态
	* @param  [in] pg      几何信息
	* @param  [in] creatTime 创建时间
	* @param  [in] updateTime 更新时间 
	* @param  [in] buffersize  buffer大小
	* @return 无
	*/
	static FeatureSharedPtr create(FeatureClassSharedPtr fc,FID id1, std::string name, int lod, REGID regid, int state=0, GeometrySharedPtr pg =GeometrySharedPtr(), 
		GDBTIME createTime=0 ,GDBTIME updateTime=0,unsigned long buffersize = 0);	
	static FeatureSharedPtr create(FCLSID fclsid,FID id1, std::string name, int lod, REGID regid, int state=0, GeometrySharedPtr pg =GeometrySharedPtr(), 
		GDBTIME createTime=0 ,GDBTIME updateTime=0,unsigned long buffersize = 0);
	/** 创建要素对象
	* @param  [in] f 要素对象
	* @return feature 指针
	*/
	static FeatureSharedPtr create (FeatureRef f);
	/** @} */ 

};
/** @} */

/** @addtogroup Feature FeatureSet  
*  @{
*/
class CORE_API FeatureSet {
public:
	virtual FSID getID()=0;
	virtual void setID(FSID id)=0;
	virtual std::string getName()=0;
	virtual void setName(std::string szName)=0;
	virtual void setFeatureClass( FeatureClassSharedPtr)=0;
	virtual FeatureClassSharedPtr   getFeatureClass()=0;
	virtual void setLayerID(LAYERID lyr)=0;
	virtual LAYERID getLayerID()=0;	

	virtual void clear()=0;
	virtual void copy(const FeatureSetSharedPtr  fSet , bool bContent=false)=0;
	virtual void copy(const FeatureSetRef  fSet , bool bContent=false)=0;
	virtual size_t  size() const =0;
	virtual void addFeature(FeatureSharedPtr p)=0;
	virtual void eraseFeature(FID fid)=0;
	virtual FeatureSharedPtr removeFeature(FID fid)=0;
	virtual FeatureSharedPtrVector  & getFeatures()=0;
	
	virtual void setFeatures(FeatureSharedPtrVector &feas ) =0;

	virtual void write(std::ostream & f)=0;
	virtual void read(std::istream & f)=0;

public:
	/**创建要素对象，并添加到要素集合中
	* @param [in] id  要素ID
	* @param [in] name  要素名称
	* @param [in] lod  要素Lod
	* @param [in] regid  要素要素分区ID
	* @param [in] state  要素图形的状态值
	* @param [in] pg  要素几何信息
	* @param [in] createTime  要素创建时间
	* @param [in] updateTime  要素更新时间
	* @return  Feature
	*/
	virtual FeatureSharedPtr createFeature(FID id, std::string name, int lod, REGID regid, int state=0, GeometrySharedPtr pg=GeometrySharedPtr() ,GDBTIME createTime=0 ,GDBTIME updateTime=0)=0;
	/**创建要素对象，并添加到要素集合中
	* @param [in] name  要素名称
	* @param [in] lod  要素Lod
	* @param [in] regid  要素要素分区ID
	* @param [in] state  要素图形的状态值
	* @param [in] pg  要素几何信息
	* @param [in] createTime  要素创建时间
	* @param [in] updateTime  要素更新时间
	* @return  Feature
	*/
	virtual FeatureSharedPtr createFeature(std::string name, int lod, REGID regid, int state=0, GeometrySharedPtr pg=GeometrySharedPtr() ,/*C3DSemanticTopology * pt=0 ,*/GDBTIME createTime=0 ,GDBTIME updateTime=0)=0;
	/**复制构造要素对象，并添加到要素集合中
	* @param [in] f 要素对象	
	* @return  Feature
	*/
	virtual FeatureSharedPtr createFeature(FeatureRef f )=0;
public:

	static FeatureSetSharedPtr create(FeatureClassSharedPtr );
	static FeatureSetSharedPtr create(const FeatureClassRef  fcls );
};
/** @} */

/** @addtogroup Feature Layer  
*  @{
*/
class CORE_API Layer {
public:
	/** @} */ 
	/** @name 拷贝构造与赋值构造函数 
	*  @{
	*/
	virtual void copy(const LayerRef lf )=0;
	/** @} */ 

	/** @name 属性设置获得操作集 
	*  @{
	*/

	/**获取图层ID
	* @param  无
	* @return   图层ID
	*/
	virtual LYRID getID()=0;
	/**设置图层ID
	* @param [in] layid 图层ID
	* @return 无
	*/
	virtual void  setID(LYRID layid )=0;

	/**获取图层名称
	* @param 无
	* @return  图层名称
	*/
	virtual std::string getName()=0;
	/**设置图层名称
	* @param [in] s 图层名称
	* @return  
	*/
	virtual void setName(std::string s)=0;


	/**获取图层类型所在工程
	* @param 无
	* @return  prjID 工程ID
	*/
	virtual PRJID getProjectID()=0;
	/**设置图层类型所在的工程
	* @param [in] prjID 工程ID
	* @return  
	*/
	virtual void  setProjectID(PRJID prjID )=0;

	/**获取图层对应的要素类型ID集合
	* @param 无
	* @return  图层对应的要素类型ID集合
	*/
	virtual vector<FCLSID>& getFeatureClassIDs()=0;
	/**设置图层对应的要素类型ID集合
	* @param [in] fclsids  要素类型ID集合
	* @return  无
	*/
	virtual void setFeatureClassIDs(vector<FCLSID>& fclsids)=0;

	/** 获取图层对应的要素集合 
	* @param 无
	* @return  图层对应的要素集合
	*/
	virtual std::vector<FeatureSetSharedPtr> & getFeatureSetVector()=0;
	/** 获取图层对应的当前要素 
	* @param 无
	* @return  图层对应的当前要素
	*/
	virtual FeatureSetSharedPtr getCurrentFeatureSet()=0;
	/** 设置图层对应的当前要素 
	* @param [in] s 当前要素集合索引
	* @return  无
	*/
	virtual void setCurrentFeatureSet(size_t s)=0;
	/** @} */ 

	/** @name 其它操作函数集  
	*  @{
	*/
	/** 在图层的要素类型ID集合中添加指定要素类型ID
	* @param [in] fclsid 要素类型ID
	* @return  无
	*/
	virtual void addFeatureClassID(FCLSID fclsid) =0;

	/** 在图层的要素类型ID集合中删除指定要素类型ID
	* @param [in] fclsid 要素类型ID
	*/
	virtual void eraseFeatureClassID(FCLSID fclsid)=0;


	/** 获取指定要素ID的要素所在要素集合索引 
	* @param [in] id 要素ID
	* @return  失败 -1,成功所在要素集合索引
	*/
	virtual size_t indexFeatureSet(FSID id)=0;

	/** 创建一个指定要素类的要素集
	* @param [in] fc 指定要素数据
	* @return 要素集指针
	*/
	virtual FeatureSetSharedPtr createFeatureSet(const FeatureClassRef fc)=0;
	/** 向图层中增加一个要素集合数据
	* @param [in] p 要素数据指针
	* return 要素数据指针
	*/
	virtual FeatureSetSharedPtr addFeatureSet(FeatureSetSharedPtr fs)=0;
	/** 在图层的要素集合中删除指定要素集合ID的要素集合数据
	* @param [in] fid 要素ID
	* return 无
	*/
	virtual void eraseFeatureSet(FSID fsid)=0;
	/** 查询图层要素集合大小
	* @param 无
	* return 图层要素集合大小
	*/
	virtual size_t  size() const=0;

	/** 设置图层所在的工作区ID的要素集合数据
	* @param [in] pws 当前工作区指针
	*/
	virtual void setLocalWorkspace(WorkspaceSharedPtr pws)=0;
	/** 获取图层所在的工作区
	* @param [in] 
	* @return 获取图层所在的工作区
	*/
	virtual WorkspaceSharedPtr getLocalWorkspace()=0;

	/** 清空图层对应的要素集合 
	* @param 无 
	* @return 无
	*/
	virtual void clear()=0;

	virtual FeatureSetSharedPtr getFeatureSet(int i)=0;
	/** @} */ 


	/** @name 文件IO操作函数集  
	*  @{
	*/
public:
	/** 写文件
	* @param [in] f输出流
	* @return 无
	*/
	virtual void write(std::ostream & f)=0;
	/** 读文件
	* @param [in] f输入流
	* @return 无
	*/
	virtual void read(std::istream & f)=0;
	/** 加载文件
	* @param [in] sz文件路径
	* @return 无
	*/
	virtual void load(std::string & sz)=0;
	/** 保存文件
	* @param [in] sz文件路径
	* @return 无
	*/
	virtual void save(std::string & sz)=0; 
	/** @} */ 

public:
	virtual bool isEqual(const LayerRef )=0;
	virtual bool isEqual(const LayerSharedPtr )=0;
public:
	static LayerSharedPtr create();
};
/** @} */

/** @addtogroup Feature Workspace  
*  @{
*/
class CORE_API Workspace {
public:
	virtual WKSPID getID()=0;
	virtual void  setID(WKSPID id )=0;
	virtual std::string getName()=0;
	virtual void  setName(std::string v )=0;
	virtual std::string getCode()=0;
	virtual void  setCode(std::string v )=0;
	virtual void setLocalProject(ProjectSharedPtr )=0;
	virtual ProjectSharedPtr getLocalProject()=0;
	
	virtual void write(std::ostream & f)=0;	
	virtual void read(std::istream & f)=0;	
	virtual void load(std::string & sz)=0;	
	virtual void save(std::string & sz)=0; 
public:
	static WorkspaceSharedPtr create();
};
/** @} */



class CORE_API ProjectAdapter
{
public:
	virtual void save(string &sz)=0;
	virtual void load(string &sz)=0;
};

typedef std::shared_ptr<ProjectAdapter> ProjectAdapterSharedPtr;

/** @addtogroup Feature Project  
*  @{
*/
class  CORE_API Project{
public:
	virtual PRJID getID()const=0;
	virtual void  setID(PRJID id )=0;
	virtual std::string getName()=0;
	virtual void  setName(std::string v )=0;
	virtual std::vector<WorkspaceSharedPtr> & getWorkspaces()=0;
	virtual void addWorkspace(WorkspaceSharedPtr p)=0;
	virtual size_t  size() const=0;
	virtual void write(std::ostream & f)=0;
	virtual int read(std::istream & f)=0;
	virtual void load(std::string & sz)=0;
	virtual void save(std::string & sz)=0; 
	virtual void getEnvelope(Envelope3dRef )=0;
	virtual void setEnvelope(Envelope3dRef )=0;
	virtual void setEnvelope( double * p )=0;

	//2013.9.13XiangFly
	virtual void setProjectAdapter(ProjectAdapterSharedPtr prjadapt)=0;
public:
	static ProjectSharedPtr create(int signal =0, ProjectAdapterSharedPtr prjadapt =NULL );
	
;
};
/** @} */





//XiangFly


end_gdb_namespace
end_gtl_namespace


