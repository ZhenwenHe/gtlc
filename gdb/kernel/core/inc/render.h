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
#include "Buffer.h"
#include "geometry2d.h"
#include "geometry3d.h"
#include "builders.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
/** @defgroup Render Render-渲染引擎  
*  @{
*/
class CORE_API GraphicsDevice{
	void *  _handle;
public:
	explicit GraphicsDevice(void * pHandle): _handle(pHandle) { } 
	inline  void* getHandle() const {return _handle;}
	inline void setHandle(void* pHandle) {_handle=pHandle;}
	inline void copy(const GraphicsDevice & g) {_handle = g._handle;} 
	virtual ~GraphicsDevice(){_handle=0;}
};
typedef std::shared_ptr<GraphicsDevice> GraphicsDeviceSharedPtr;
/** @} */ 


/** @addtogroup Render Camera-相机类  
*  @{
*/
class CORE_API Camera{
public:	
	virtual void setViewMatrix(const Vertex3d eye,const Vertex3d center, const Vertex3d up)=0;
	virtual void getViewMatrix(Vertex3d &eye,  Vertex3d & center,  Vertex3d & up)=0;
	virtual void setProjectionMatrix(double  FovAngleY,double AspectRatio,double NearZ, double FarZ )=0;
	virtual void getProjectionMatrix(double & FovAngleY,double& AspectRatio,double &NearZ, double &FarZ )=0;
	virtual Matrix4x4SharedPtr getProjectionMatrix()=0;
	virtual void setProjectionMatrix(Matrix4x4SharedPtr)=0;
	virtual void WP2VP(const Vertex3d & w ,Vertex3f& v)=0;
	virtual void VP2WP(const  Vertex3f & v ,Vertex3d & w)=0;
};		  
typedef std::shared_ptr<Camera> CameraSharedPtr;
/** @} */

class Render3d;
class Render2d;
class Render;
typedef std::shared_ptr<Render> RenderSharedPtr;
typedef Render2d* Render2dPtr;
typedef std::shared_ptr<Render2d> Render2dSharedPtr;
typedef Render3d* Render3dPtr;
typedef std::shared_ptr<Render3d> Render3dSharedPtr;

/** @addtogroup Render Render-渲染引擎类  
*  @{
*/
class  CORE_API Render 
{
protected:
	int               _dimension;
protected:
	GeometrySharedPtr _geometricTrack;
protected:
	BuildersSharedPtr  _builders;
	Vertex3d          _origin;//主要用于将大数据处理成小数据，默认为0
	
public:
	virtual void setGeometricTrack(GeometrySharedPtr p) =0;
	inline GeometrySharedPtr getGeometricTrack() {return _geometricTrack;}
	virtual void addNode(FeatureSharedPtr fsp)=0;
public:
	inline void setModelBuilder(BuilderSharedPtr bsp) {_builders->setModelBuilder(bsp);}
	inline BuilderSharedPtr getModelBuilder( ) {return _builders->getModelBuilder();}
	inline void setMeshBuilder(MeshBuilderSharedPtr bsp) {_builders->setMeshBuilder(bsp);}
	inline MeshBuilderSharedPtr getMeshBuilder( ) {return _builders->getMeshBuilder();}
	inline void setTerrianBuilder(TerrainBuilderSharedPtr bsp) {_builders->setTerrainBuilder(bsp);}
	inline TerrainBuilderSharedPtr getTerrainBuilder( ) {return _builders->getTerrainBuilder();}

	inline void setBuilders(BuildersSharedPtr p){_builders = p;}
	inline BuildersSharedPtr getBuilders() {return _builders;}
	
	inline Vertex3d & getOrigin() {return _origin;}
	inline void setOrigin(const Vertex3d & v) {_origin = v;}
	inline double getOriginX() {return _origin.x;}
	inline double getOriginY() {return _origin.y;}
	inline double getOriginZ() {return _origin.z;}

	inline  int getDimension() {return _dimension;}

public:
	virtual bool initial(const GraphicsDevice & gd)=0;
	virtual void cleanup()=0;

	virtual void WP2VP(const Vertex3d & w ,Vertex3f& v)=0;
	virtual void VP2WP(const  Vertex3f & v ,Vertex3d & w, bool bNearest=false)=0;

	virtual void WP2VP(const Vertex2d & w ,Vertex2d & v){}
	virtual void VP2WP(const Vertex2d & v ,Vertex2d & w){}
	virtual void WP2VP(double wpx,double wpy,double& vpx,double& vpy){}
	virtual void VP2WP(double vpx,double vpy,double &wpx,double &wpy){}

	virtual gdb::SelectorRef  select(const gdb::Vertex3f &   mouse_point_in_viewport ) = 0;
	virtual gdb::SelectorRef  select(const gdb::Vertex3f &  mouse_point_left_bottom_in_viewport,
		const gdb::Vertex3f &  mouse_point_right_top_in_viewport) = 0;
	/** 清空选择集, 并自动恢复原状态
	* @param [in] 无
	* @return 无
	*/
	virtual void clearSelector()=0;
	/** 强制重新绘制场景
	* @param [in] dynymic_loading_data  绘制过程中动态调度数据的模式，0为不调度
	* @return 无
	*/
	virtual void requestRedraw(int dynymic_loading_data=0)=0;

	Render();

	virtual Render3d * asRender3d();
	virtual Render2d * asRender2d();

};

/** @} */

/** @addtogroup Render Render3d-渲染引擎类  
*  @{
*/
class  CORE_API Render3d : public Render 
{
public:
	Render3d();
public://相机相关的操作
	virtual void home()=0;
	
		
	virtual void setLookAt(const Vertex3d& eye, const Vertex3d& center,const Vertex3d& up)=0;
	virtual void getLookAt( Vertex3d&eye,  Vertex3d&center, Vertex3d&up)=0;
	virtual CameraSharedPtr  getCamera()=0;
	
	virtual Vertex3d getSceneCenter()=0;
	virtual double getSceneRadius()=0;
	virtual void rotate(const cn::edu::cug::gdb::Quaternion & q)=0;
	virtual void translate(const cn::edu::cug::gdb::Vertex3d & delta)=0;

public:
	virtual void addNode(FeatureSharedPtr fsp)=0;
	virtual void addNode(BlockSharedPtr bsp)=0;
	virtual void setGeometricTrack(GeometrySharedPtr p) =0;
protected:	
	virtual void renderScene()=0;	
public:
	virtual void startRenderingThread() = 0;
};

/** @} */



/** @addtogroup Render  Coordsys2d-坐标系统换算
*  @{
*/
class Coordsys2d
{
public:

	/** @name WPTOLP转换的类型
	*  @{
	*/
	enum WptoLpConvertMode
	{
		WPTOLP_ENVELOPE3D_TO_CRECT = 0,
		WPTOLP_ENVELOPE3D_TO_GDIPLUS_RECT ,
		WPTOLP_ENVELOPE3D_TO_GDIPLUS_RECTF ,

		WPTOLP_VERTEX2D_TO_CPOINT ,
		WPTOLP_VERTEX2D_TO_POINT ,
		WPTOLP_VERTEX2D_TO_GDIPLUS_POINT ,
		WPTOLP_VERTEX2D_TO_GDIPLUS_POINTF ,

		WPTOLP_VERTEXCOLLECTION2D_TO_CPOINTS ,
		WPTOLP_VERTEXCOLLECTION2D_TO_POINTS ,
		WPTOLP_VERTEXCOLLECTION2D_TO_GDIPLUS_POINTS ,
		WPTOLP_VERTEXCOLLECTION2D_TO_GDIPLUS_POINTFS
	};
	/** @}*/


	/** @name WPTOLP转换的类型
	*  @{
	*/
	enum LptoWpConvertMode
	{
		LPTOWP_POINT_TO_VERTEX2D = 0 ,
		LPTOWP_CPOINT_TO_VERTEX2D ,
		LPTOWP_GDIPLUS_POINT_TO_VERTEX2D ,
		LPTOWP_GDIPLUS_POINTF_TO_VERTEX2D ,
		LPTOWP_CRECT_TO_ENVELOPE3D,
	};
	/** @}*/

	/** @name 坐标系统换算
	*  @{
	*/
	enum  enum_GEO_WORLD_MAP_MODE//世界坐标和图纸坐标之间的映射关系
	{
		WMM_NORMAL=0X0000,        //单独使用，
		WMM_X_AXIS_REVERSE=0X0001,//可以单独使用，也可以与WMM_Y_AXIS_REVERSE联合使用，效果等同于WMM_XY_AXIS_REVERSE
		WMM_Y_AXIS_REVERSE=0X0002,//可以单独使用，也可以与WMM_X_AXIS_REVERSE联合使用，效果等同于WMM_XY_AXIS_REVERSE
		WMM_XY_AXIS_REVERSE=0X0003,//单独使用
		WMM_AXIS_ROTATE=0X0004     //单独使用
	};
	enum enum_GEO_MAP_PROJECTION_TYPE//投影类型
	{
		MPT_UNKNOWN=0,
		MPT_GAUSS=1,
		MPT_ALBERS,
		MPT_LAMBERT,
		MPT_UTM,
		MPT_MAX
	};

	enum enum_GEO_COORDSYS_TYPE//坐标系统类型
	{
		CST_UNKNOWN=0,
		CST_BEIJING54=1,
		CST_XIAN80,
		CST_WGS84,
		CST_MAX
	};

	enum enum_GEO_ELLIPSIOD_TYPE//椭球体类型
	{
		ET_UNKNOWN=0,
		ET_KLSFSJ=1,
		ET_IUGG75,
		ET_WGS84,
		ET_USERDEF,//用户自定义椭球
		ET_MAX
	};
	/** @}*/

public:
	/** 世界坐标 ==>  逻辑坐标 
	* @param [in] wx,wy,世界坐标
	* @param [out] nx,ny,lx,ly,dx,dy:逻辑坐标 
	* @return 
	*/
	virtual void WP2VP(double wx,double wy,int *nx,   int *ny   ) = 0;
	virtual void WP2VP(double wx,double wy,long *lx,  long *ly  ) = 0;
	virtual void WP2VP(double wx,double wy,float *dx, float *dy ) = 0;
	virtual void WP2VP(double wx,double wy,double *dx,double *dy) = 0;


	/**  获取显示比例
	* @return 显示比例
	*/
	virtual double getDisplayScale()=0;

	/**  获取显示比例
	* @param [in] ds 显示比例
	* @return 
	*/
	virtual void setDisplayScale(double ds) = 0;

	/** WP2VP坐标转换接口
	* @param [in] srcPtr: 世界坐标下点
	* @param [in]   convertMode模式如下
	* 				WPTOLP_ENVELOPE3D_TO_CRECT,
	* 				WPTOLP_ENVELOPE3D_TO_GDIPLUS_RECT ,
	* 				WPTOLP_ENVELOPE3D_TO_GDIPLUS_RECTF ,
	* 				WPTOLP_VERTEX2D_TO_CPOINT ,
	* 				WPTOLP_VERTEX2D_TO_POINT ,
	* 				WPTOLP_VERTEX2D_TO_GDIPLUS_POINT ,
	* 				WPTOLP_VERTEX2D_TO_GDIPLUS_POINTF ,
	* 				WPTOLP_VERTEXCOLLECTION2D_TO_CPOINTS ,
	* 				WPTOLP_VERTEXCOLLECTION2D_TO_POINTS ,
	* 				WPTOLP_VERTEXCOLLECTION2D_TO_GDIPLUS_POINTS ,
	* 				WPTOLP_VERTEXCOLLECTION2D_TO_GDIPLUS_POINTFS 
	* @param [out] tgtPtr: 逻辑坐标下点
	* @return 
	*/
	virtual void WP2VP(void* srcPtr, void* tgtPtr, int convertMode) = 0;

	/** 逻辑坐标 ==>  世界坐标 
	* @param [in] x,y,lx,ly:逻辑坐标
	* @param [out] wx,wy,世界坐标 
	* @return 
	*/
	virtual void VP2WP(long x,    long y,    double * wx, double *wy) = 0;	
	virtual void VP2WP(double lx, double ly, double * wx, double *wy) = 0;

	/** VP2WP坐标转换接口
	* @param [in] srcPtr: 逻辑坐标下点
	* @param [in]   convertMode模式如下
	* 				LPTOWP_POINT_TO_VERTEX2D = 0 ,
	* 				LPTOWP_CPOINT_TO_VERTEX2D ,
	* 				LPTOWP_GDIPLUS_POINT_TO_VERTEX2D ,
	* 				LPTOWP_GDIPLUS_POINTF_TO_VERTEX2D ,
	* 				LPTOWP_CRECT_TO_ENVELOPE3D,
	* @param [out] tgtPtr: 世界坐标下点
	* @return 
	*/	
	virtual void VP2WP(void* srcPtr, void* tgtPtr, int convertMode) = 0;

	/** Windows的逻辑长度向世界长度转化 
	* @param [in] lLen逻辑长度
	* @return 世界长度
	*/
	virtual double LLtoWL(double lLen) = 0;	

	/** 世界长度向Windows的逻辑长度转化 
	* @param [in] fLen世界长度
	* @return 逻辑长度
	*/
	virtual double WLtoLL(double fLen) = 0;

	/** 图纸长度（单位：当前坐标系单位，米、公里、或毫米） => 逻辑长度 
	* @param [in] m 图纸长度
	* @return 逻辑长度
	*/
	virtual double mapLengthtoLL(double m) = 0;

	/** 逻辑长度 => 图纸长度（单位：当前坐标系单位，米、公里、或毫米） 
	* @param [in] m 逻辑长度
	* @return 图纸长度
	*/
	virtual double LLtoMapLength(double m) = 0;

	/** Windows的逻辑长度转换成毫米长度 
	* @param [in] lLen逻辑长度
	* @return 毫米长度
	*/
	virtual double LLtoMM(double lLen) = 0;

	/** 毫米长度转换成Windows的逻辑长度 
	* @param [in] fLen毫米长度
	* @return 逻辑长度
	*/
	virtual double MMtoLL(double fLen) = 0;

	/** 在比例尺scale (比例尺分母)下， mm 毫米纸上单位长度所对应的世界坐标
	* 注意：此转换与图形的显示比例无关 
	* @param [in] mm毫米纸上单位长度
	* @return 世界坐标长度
	*/
	virtual double MMtoWL(double mm) = 0;

	/** 在比例尺dMapScale (比例尺分母)下， mm 毫米纸上单位长度所对应的世界坐标 
	* @param [in] mm:毫米纸上单位长度, dMapScale:比例尺
	* @return 世界坐标长度
	*/
	virtual double MMtoWL(double mm, double dMapScale) = 0;

	/** 根据图形显示比例，计算mm 毫米纸上单位长度所对应的世界坐标 
	* @param [in] mm:毫米纸上单位长度
	* @return 世界坐标长度
	*/
	virtual double MMtoWL_ByDisplayScale(double mm) = 0;

	/** 根据图形显示比例，在比例尺dMapScale (比例尺分母)下，mm 毫米纸上单位长度所对应的世界坐标 
	* @param [in] mm:毫米纸上单位长度, dMapScale:比例尺
	* @return 世界坐标长度
	*/
	virtual double MMtoWL_ByDisplayScale(double mm,double dMapScale) = 0;

	/** 在比例尺scale (比例尺分母)下， 世界坐标长度wl所对应的纸上单位长度(mm 毫米)
	* 注意：此转换与图形的显示比例无关 
	* @param [in] wl:世界坐标长度
	* @return 纸上单位长度(mm 毫米)
	*/
	virtual double WLtoMM(double wl) = 0;

	/** 在比例尺dMapScale (比例尺分母)下， 世界坐标长度wl所对应的纸上单位长度(mm 毫米) 
	* @param [in] wl:世界坐标长度, dMapScale:比例尺
	* @return 纸上单位长度(mm 毫米)
	*/
	virtual double WLtoMM(double wl, double dMapScale) = 0;

	/** 根据图形显示比例，计算世界坐标长度wl所对应的纸上单位长度(mm 毫米) 
	* @param [in] wl:世界坐标长度
	* @return 纸上单位长度(mm 毫米)
	*/
	virtual double WLtoMM_ByDisplayScale(double wl) = 0;

	/** 根据图形显示比例，在比例尺dMapScale (比例尺分母)下，计算世界坐标长度wl所对应的纸上单位长度(mm 毫米)
	* @param [in] wl:世界坐标长度, dMapScale:比例尺
	* @return 纸上单位长度(mm 毫米)
	*/
	virtual double WLtoMM_ByDisplayScale(double wl,double dMapScale) = 0;

	/** 获取系统单位对应的中文名称：0-米,1-厘米,2-毫米,3-英寸,4-千米，5－度分秒, 6-度 
	* @return 系统单位对应的中文名称
	*/
	virtual std::string getUnitNameCHS() = 0;

	/** 符号内部坐标系  ==> 设备坐标系 
	* @param [in] orgX,orgY: 原点坐标
	* @param [in] dScaleX,dScaleY: X、Y轴的缩放比例
	* @param [in] dAngRad:旋转角度（弧度单位）
	* @param [in] apX,apY: 符号内部坐标系坐标
	* @param [out] dpX, dpY: 设备坐标系坐标
	* @return 
	*/
	virtual void APtoDP(double orgX,double orgY, double dScaleX,double dScaleY,
		double dAngRad,double apX,double apY,double &dpX, double &dpY) = 0;

	/** 设备坐标系  ==> 符号内部内部坐标系 
	* @param [in] orgX,orgY: 原点坐标
	* @param [in] dScaleX,dScaleY: X、Y轴的缩放比例
	* @param [in] dAngRad:旋转角度（弧度单位）
	* @param [in] dpX, dpY: 设备坐标系坐标
	* @param [out] apX,apY: 符号坐标系坐标
	* @return 
	*/
	virtual void DPtoAP(double orgX,double orgY, double dScaleX,double dScaleY,
		double dAngRad,double dpX, double dpY,double &apX,double &apY) = 0;

public:
	/** 逻辑坐标系内进行平移
	* @param [in] dOffX,dOffY: X、Y轴的偏移量
	* @return 
	*/
	virtual void translateInVp(double dOffX, double dOffY) = 0;

	/** 世界坐标系内进行平移
	* @param [in] dOffX,dOffY: X、Y轴的偏移量
	* @return 
	*/
	virtual void translateInWp(double dOffX, double dOffY) = 0;

	/** 设置为1:1显示 
	* @param [in] dMinX, dMinY,dMaxX,dMaxY: 指定显示范围
	* @param [out] 
	* @return 
	*/
	virtual void make11Display(double dMinX, double dMinY,double dMaxX,double dMaxY) = 0;

	/** 以指定点为中心（视口逻辑坐标），放大或缩小图形，dZoomFactor>1为放大，小于1为缩小 
	* @param [in] vpCenterX, vpCenterY: 中心点坐标； dZoomFactor：缩放因子
	* @return 
	*/
	virtual void zoomVp(double vpCenterX, double vpCenterY, double dZoomFactor ) = 0;
	
	/** 以指定点为中心，放大或缩小图形，dZoomFactor>1为放大，小于1为缩小
	* @param [in] ptCenterX, ptCenterY，缩放中心点
	* @param [in] dZoomFactor，缩放因子
	* @return 
	*/
	virtual void zoomWp(double wpCenterX, double wpCenterY, double dZoomFactor ) = 0;

	/** 将矩形范围内的图形显示在View视图中, 并自动进行相应的缩放 
	* @param [in] pRect: 矩形范围
	* @return 
	*/
	virtual void displayWpRangeInCurView(Envelope3d* pRect) = 0;

	/** 将矩形范围内的图形显示在View视图中, 并自动进行相应的缩放 
	* @param [in]  dMinX, dMinY,dMaxX,dMaxY: 矩形范围
	* @return 
	*/
	virtual void displayWpRangeInCurView(double dMinX, double dMinY,double dMaxX,double dMaxY) = 0;

	/** 将指定范围rctBound内的图形显示到指定的逻辑坐标范围rctLog内
	* @param [in] rctBound: 指定的世界坐标范围
	* @param [in] rctLogX,rctLogY,rctLogCX,rctLogCY: 界定逻辑坐标范围
	* @param [in] visiableWndRect为指定的可见窗口矩形（逻辑坐标），它的大小影响到其范围内的对象是否可见
	* @param [in] visbRctX,visbRctY,visbRctCX,visbRctCY: 界定可见窗口范围 
	* @return 
	*/
	virtual void displayWpRangeInVpRect(Envelope3d& rctBound,
		double rctLogX, double rctLogY, double rctLogCX, double rctLogCY,
		double visbRctX, double visbRctY, double visbRctCX, double visbRctCY) = 0;

	/** 获取单位类型 
	* @return 单位类型 
	*/
	virtual int getUnitType()const = 0;

	/** 设置单位类型 
	* @param [in] it 单位类型
	* @return 
	*/
	virtual void setUnitType(int it) = 0;

	/** 设置每米的逻辑单位 
	* @param [in] x,y: X轴和Y轴方向下每米的逻辑单位
	* @return 
	*/
	virtual void setLogicalUnitsPtrMeter(double x,double y) = 0;

	/** 获取每米的逻辑单位 
	* @param [out] x,y: X轴和Y轴方向下每米的逻辑单位
	* @return 
	*/
	virtual void getLogicalUnitsPtrMeter(double &x,double &y) = 0;

	/** 设置X轴每英寸的像素数 
	* @param [in] X轴每英寸的像素数
	* @return 
	*/
	virtual void setDpiX(double x) = 0;

	/** 设置Y轴每英寸的像素数 
	* @param [in] Y轴每英寸的像素数
	* @return 
	*/
	virtual void setDpiY(double y) = 0;

	/** 获取X轴每英寸的像素数 
	* @return X轴每英寸的像素数
	*/
	virtual double getDpiX() = 0;

	/** 获取Y轴每英寸的像素数 
	* @return Y轴每英寸的像素数 
	*/
	virtual double getDpiY() = 0;

public:
	/** 获取绘图模式 
	* @return 绘图模式
	*/
	virtual int getMapMode() = 0;

	/** 设置绘图模式 
	* @param [in] newMapMode: 绘图模式
	* @return 
	*/
	virtual void setMapMode(int newMapMode) = 0;

	/** 获取窗口的位置信息 
	* @param [out] x,y: 窗口的起始点坐标
	* @param [out] cx,cy: 窗口的宽和高
	* @return 
	*/
	virtual void getViewport(long &x,long &y,long &cx,long &cy) = 0;

	/** 设置窗口的位置 
	* @param [in] x,y: 窗口的起始点坐标
	* @param [in] cx,cy: 窗口的宽和高
	* @return 
	*/
	virtual void setViewport(long x,long y,long cx,long cy) = 0;

	/** 获取窗口的起始点 
	* @param [out] x,y: 窗口的起始点
	* @return 
	*/
	virtual void getStartOfViewport(long & x,long & y) = 0;

	/** 设置窗口的起始点 
	* @param [in] xNew,yNew: 窗口的起始点
	* @return 
	*/
	virtual void setStartOfViewport(long xNew,long yNew) = 0;

	/** 获取窗口尺寸大小 
	* @param [out] cx, cy: 窗口的宽和高
	* @return 
	*/
	virtual void  getViewportSize(int &cx, int &cy) = 0;

	/** 设置窗口尺寸大小 
	* @param [in] cx, cy: 窗口的宽和高
	* @return 
	*/
	virtual void  setViewportSize(int cx, int cy) = 0;

	/** 设置世界坐标系起点 
	* @param [in] sxNew,syNew: 世界坐标系起点
	* @return 
	*/
	virtual void setStartOfWp(double sxNew,double syNew) = 0; 

	/** 获取世界坐标系起点 
	* @param [out] sxNew,syNew: 世界坐标系起点
	* @return 
	*/
	virtual void getStartOfWp(double & sxNew,double & syNew) = 0;

	/** 获取世界坐标系起点X 
	* @return 世界坐标系起点X
	*/
	virtual double getStartOfWpX() = 0;

	/** 获取世界坐标系起点Y 
	* @return 世界坐标系起点Y 
	*/
	virtual double getStartOfWpY() = 0;

	/** 设置旋转角度（角度单位） 
	* @param [in] dAngle:旋转角度（角度单位） 
	* @return 
	*/
	virtual void   setRotateAngle(double dAngle) = 0; 

	/** 获取旋转角度 （角度单位） 
	* @return 旋转角度 （角度单位） 
	*/
	virtual double getRotateAngle() const  = 0;

	/** 获取世界地图模式 
	* @return 世界地图模式
	*/
	virtual unsigned int getWpMapMode()const = 0 ;

	/** 设置世界地图模式 
	* @param [in] wWorldMapMode:世界地图模式
	* @return 
	*/
	virtual void setWpMapMode(unsigned int  wWorldMapMode) = 0;

};
/** @}*/


/** @addtogroup Render  二维显示库类型
*  @{
*/
enum Render2dType
{
	RENDER_TYPE_2D_GDI_PLUS,	//Gdiplus显示库
	RENDER_TYPE_2D_GDI			//Gdi显示库
};
/** @}*/

/** @addtogroup Render Rect2d-逻辑坐标下的矩形范围结构体
*  @{
*/
struct Rect2d
{
	long    left;
	long    top;
	long    right;
	long    bottom;
} ;
/** @}*/


/** @addtogroup Render  二维显示接口
*  @{
*/
class CORE_API Render2d : public Render
{
public:
	Render2d();
public:

	/** @name 二维几何对象的显示模式
	*  @{
	*/
	enum GeometryDisplayMode2d
	{
		RENDER2D_GEOMETRY_DISPLAY_MODE_NORMAL,		//正常显示
		RENDER2D_GEOMETRY_DISPLAY_MODE_DEFINE_COLOR	//指定颜色显示
	};
	/** @}*/

	/** @name 二维图幅的全局显示模式
	*  @{
	*/
	enum GV2dGlobalDisplayMode2d
	{
		RENDER2D_GLOBAL_DISPLAY_MODE_REAL,	//正常显示，显示所有真实的线型与花纹
		RENDER2D_GLOBAL_DISPLAY_MODE_EDIT	//编辑模式，只显示系统线型，纯色填充
	};
	/** @}*/

	/** @name 显示的质量
	*  @{
	*/
	enum GV2dGlobalDisplayQuality2d
	{
		RENDER2D_GLOBAL_DISPLAY_QUALITY_LOW,	//低质量显示，速度优先
		RENDER2D_GLOBAL_DISPLAY_QUALITY_HIGH	//高质量显示，画质优先
	};
	/** @}*/

	/** @name 刷新视图时，数据调度的模式
	*  @{
	*/
	enum GV2dScheduleDataMode2d
	{
		RENDER2D_SCHEDULE_DATA_NO,					//不调度数据
		RENDER2D_SCHEDULE_DATA_WITH_CHANGED_BOX,	//自动根据当前视口，前次视口，计算出来的变化包围盒来调度
		RENDER2D_SCHEDULE_DATA_WITH_CURRENT_BOX		//直接根据当前的视口，进行调度
	};
	/** @}*/
	
public:	

	/** 以用户自定义的方式，重新绘制当前视口中的几何对象，
	* 如果当前的视口发生变化，会根据改变的矩形,自动用调度类更新显示的缓存
	* @param [in] nDispMode-显示模式：
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_NORMAL,         正常显示
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_DEFINE_COLOR    指定颜色显示 
	* @param [in] pColor: 自定义颜色
	* @param [in] drawGeometricTrack: 是否需要绘制轨迹线（橡皮筋线）
	* @param [in] scheduleDataMode: 刷新视图时，数据调度的模式:GV2dScheduleDataMode2d
	* RENDER2D_SCHEDULE_DATA_NO,				//不调度数据
	* RENDER2D_SCHEDULE_DATA_WITH_CHANGED_BOX,	//自动根据当前视口，前次视口，计算出来的变化包围盒来调度
	* RENDER2D_SCHEDULE_DATA_WITH_CURRENT_BOX	//直接根据当前的视口，进行调度
	* @return 无
	*/
	virtual void redrawView(int nDispMode,Color4f* pColor,bool drawGeometricTrack,int scheduleDataMode) = 0;

	/** 以用户自定义的方式，重新绘制当前视口中的几何对象
	* 此函数会根据当前的视口范围，强制调度类重新更新显示缓存
	* @param [in] nDispMode-显示模式：
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_NORMAL,         正常显示
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_DEFINE_COLOR    指定颜色显示 
	* @param [in] pColor: 自定义颜色
	* @param [in] drawGeometricTrack: 是否需要绘制轨迹线（橡皮筋线）
	* @return 无
	*/
	//virtual void redrawViewForceRefreshScene(int nDispMode,Color4f* pColor,bool drawGeometricTrack) = 0;
	
	/** 用缓存的位图，刷新当前视口，其余部分，用背景色填充，此函数专用于移屏时的刷新
	* @param [in] offX,offY: 位图左下角点的坐标
	* @param [in] bmpWidth,bmpHeight: 位图的宽和高
	* @param [in] drawGeometricTrack: 是否需要绘制轨迹线（橡皮筋线）
	* @return 
	*/
	virtual void redrawViewWithCachedBmp(double vpx, double vpy, double vpBmpWidth, double vpBbmpHeight, bool drawGeometricTrack) = 0;
 
	
	/** 根据当前窗口和旧的窗口位置，计算改变的矩形,并用调度类更新显示的缓存
	* @return 无
	*/
	virtual void refreshSceneWithChangedBox() = 0;

	/** 根据当前的视口范围，强制调度类重新更新显示缓存
	* @return 无
	*/
	virtual void refreshSceneWithCurrentWndBox() = 0;

	/** 根据传入的更新矩形，刷新场景中的数据
	* @param [in] changedBoxes:更新矩形列表
	* @return 无
	*/
	virtual void refreshScence(std::list<Envelope3d> & changedBoxes) = 0;
	
	/** 设置显示的质量
	* @param [in] nQuality:显示质量
	* RENDER2D_GLOBAL_DISPLAY_QUALITY_LOW,	//低质量显示，速度优先
	* RENDER2D_GLOBAL_DISPLAY_QUALITY_HIGH	//高质量显示，画质优先
	* @return 无
	*/
	virtual void setRenderQuality(int nQuality) = 0;

	/** 重新根据当前视口范围，重新从缓存中获取数据，并复位视图，全图显示
	* @param [in] curViewRange:当前视口范围
	* @return 无
	*/
	virtual void home(Envelope3d& curViewRange) = 0;

	/** 设置坐标系统 
	* @param [in] pCoorSys:坐标系统
	* @return 
	*/
	virtual void setCoordsys(Coordsys2dSharedPtr& pCoorSys) = 0;

	/** 设置背景颜色 
	* @param [in] cr:背景颜色 
	* @return 
	*/
	virtual void setBackgroundColor(Color4f& cr) = 0;

	/** 获取背景颜色 
	* @param [out] cr:背景颜色 
	* @return 
	*/
	virtual void getBackgroundColor(Color4f& cr) = 0;

	/** 获取用于显示的，缓存的位图指针
	* @param [out] gdiplusBitmap:缓存的位图指针,Gdiplus::Bitmap类型
	* @return 
	*/
	virtual void getCachedBitmap(void ** gdiplusBitmap) = 0;

	/** 获取坐标系统 
	* @return 坐标系统指针
	*/
	virtual Coordsys2dSharedPtr& getCoordsys() = 0;

	/** 显示二维图形对象。 
	* @param [in] pObj:二维图形对象
	* @param [in] env: 设备环境
	* @param [in] nDispMode-显示模式：
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_NORMAL,         正常显示
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_DEFINE_COLOR    指定颜色显示 
	* @param [in] pColor: 自定义颜色
	* @return 
	*/	
	virtual void display2DGeometry(GeometrySharedPtr& pObj,GraphicsDevice& env,int nDispMode,Color4f* pColor) = 0;

	/** 设置打印状态 
	* @param [in] bPrinting: 是否处于打印状态
	* @return 
	*/
	virtual void setPrintingStatus(bool bPrinting) = 0;

	/** 获取是否处于打印状态 
	* @return 是否处于打印状态 
	*/
	virtual bool getPrintingStatus() = 0;

	/** 设置线条的连接方式 
	* @param [in] mode:线条的连接方式
	* @return 
	*/
	virtual void setLineJoinMode(int mode) = 0;

	/** 获取线条的连接方式 
	* @return 线条的连接方式
	*/
	virtual int  getLineJoinMode() = 0;

	/** 设置 图形的全局显示模式 
	* @param [in] mode: 图形的全局显示模式
	*			  RENDER2D_GLOBAL_DISPLAY_MODE_REAL:显示真实线型、花纹
	*			  RENDER2D_GLOBAL_DISPLAY_MODE_EDIT:图形编辑模式，不显示线型、填充花纹
	* @return 
	*/
	virtual void setMapGlobalDisplayMode(int mode) = 0;

	/** 获取，设置 图形的全局显示模式 
	* @return mode: 图形的全局显示模式
	*   RENDER2D_GLOBAL_DISPLAY_MODE_REAL:显示真实线型、花纹
	*   RENDER2D_GLOBAL_DISPLAY_MODE_EDIT:图形编辑模式，不显示线型、填充花纹 
	*/
	virtual int  getMapGlobalDisplayMode() = 0;

	/** 计算世界坐标系下，当前窗口的范围 
	* @return 
	*/
	virtual void calculateWpWndRange() = 0;

	virtual void setGeometricTrack(GeometrySharedPtr p) =0;
	virtual void addNode(FeatureSharedPtr fsp)=0;
};
/** @}*/


/** @addtogroup Render  二维符号显示接口
*  @{
*/
class GdiplusRenderSymbol2d
{
public:
	/** 设置坐标系  
	* @param [in] pCoordSys:坐标系
	* @return 
	*/
	virtual void setCoordSys(Coordsys2dSharedPtr& pCoordSys) = 0;

	/**是否显示点符号的插入点  
	* @return 是否显示点符号的插入点
	*/
	virtual bool isDisplaySymPointInsertPt() = 0;

	/**设置是否显示点符号的插入点  
	* @param [in] b:是否显示点符号的插入点
	* @return 
	*/
	virtual void setDisplaySymPointInsertPt(bool b) = 0;

	/**是否显示真实的线宽  
	* @return 是否显示真实的线宽 
	*/
	virtual bool isDisplayRealLineWidth() = 0;

	/**设置是否显示真实的线宽  
	* @param [in] b 是否显示真实的线宽
	* @return 
	*/
	virtual void setDisplayRealLineWidth(bool b) = 0;

	/**线宽是否根据显示的比例进行相应的放大或缩小 
	* @return 线宽是否根据显示的比例进行相应的放大或缩小
	*/
	virtual bool isLineWidthFitToScale() = 0;

	/**线宽是否根据显示的比例进行相应的放大或缩小 
	* @param [in] b线宽是否根据显示的比例进行相应的放大或缩小
	* @return 
	*/
	virtual void setLineWidthFitToScale(bool b) = 0;

	/**是否处于打印状态  
	* @return 是否处于打印状态 
	*/
	virtual bool isPrinting() = 0;

	/**设置是否处于打印状态  
	* @param [in] b 是否处于打印状态
	* @return 
	*/
	virtual void setPrinting(bool b) = 0;

	/**是否横向打印  
	* @return 是否横向打印  
	*/
	virtual bool isTransversePrinting() = 0;

	/**设置是否横向打印  
	* @param [in] b 是否横向打印
	* @return 
	*/
	virtual void setTransversePrinting(bool b) = 0;

	/**显示点符号
	* @param [in] pPoint: 点符号对象指针
	* @param [in] env: 设备环境
	* @param [in] nSpecialMode: 显示模式
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_NORMAL-代表正常显示，此时pColor为空
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_DEFINE_COLOR-以特殊颜色显示，此时pColor不能为空
	* @param [in] pColor - 指定颜色
	* @param [in] orgX,orgY - 插入点设备坐标
	* @param [in] dZoomFactor - 指定点符号显示比例
	* @param [in] dAngRad - 符号旋转角度（弧度单位）
	* @return 
	*/
	virtual void drawSymbolPoint( Symbol2dSharedPtr& pPoint, GraphicsDevice& env,int nSpecialMode,
		Color4f *pColor, double orgX, double orgY, double dZoomFactor, double dAngRad ) = 0;
	
	/**	将点符号绘制到指定的矩形区域中, rct: 设备坐标
	* @param [in] pPoint: 点符号对象指针
	* @param [in] env: 设备环境
	* @param [in] nSpecialMode: 显示模式
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_NORMAL-代表正常显示，此时pColor为空
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_DEFINE_COLOR-以特殊颜色显示，此时pColor不能为空
	* @param [in] pColor - 指定颜色
	* @param [in] rct: 设备坐标下的矩形区域
	* @return 
	*/
	virtual void drawSymbolPointInRect( Symbol2dSharedPtr& pPoint,
		GraphicsDevice& env,int nSpecialMode,Color4f *pColor,Rect2d &rct) = 0;
	
	/**在指定的坐标,绘制单个的区符号，或线符号，(类似于点符号的绘制)
	* @param [in] pRgn: 区符号，或线符号对象指针
	* @param [in] env: 设备环境
	* @param [in] nSpecialMode: 显示模式
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_NORMAL-代表正常显示，此时pColor为空
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_DEFINE_COLOR-以特殊颜色显示，此时pColor不能为空
	* @param [in] pColor - 指定颜色
	* @param [in] orgX,orgY - 插入点设备坐标
	* @param [in] dZoomFactor - 指定点符号显示比例
	* @param [in] dAngRad - 符号旋转角度（弧度单位）
	* @return 
	*/
	virtual void drawSymbol( Symbol2dSharedPtr& pRgn, GraphicsDevice& env,int nSpecialMode,
		Color4f *pColor, double orgX, double orgY, double dZoomFactor, double dAngRad ) = 0;
	
	/**	将区填充符号，或线符号, 绘制到指定的矩形区域中(充满整个区域), rct: 设备坐标
	* @param [in] pRgn: 区填充符号，或线符号对象指针
	* @param [in] env: 设备环境
	* @param [in] rct: 设备坐标下的矩形区域
	* @param [in] bKeepHZScale: 是否保持横纵比例，如果为FALSE，则充满区域
	* @return 
	*/
	virtual void drawSymbolInRect( Symbol2dSharedPtr& pRgn, GraphicsDevice& env, Rect2d &rct,bool bKeepHZScale) = 0;
	
	/**	绘制具有ptCount个点的点阵pts 组成的边界的填充区, 此函数不绘制区边界, 此处用到花纹旋转
	* @param [in] pRgn: 区符号，或线符号对象指针
	* @param [in] env: 设备环境
	* @param [in] pts: 点阵数组的首地址
	* @param [in] ptCount: 点阵的个数
	* @param [in] pcrBK:填充区背景色，如果不需要背景填充，则pcrBK指针为空
	* @param [in] dZoomFactor - 指定点符号显示比例
	* @param [in] dAngDeg-填充角度，单位为度
	* @return 
	*/
	virtual void fillPathWithSymbol( Symbol2dSharedPtr& pRgn, GraphicsDevice& env, 
		Vertex2d* pts, long ptCount, Color4f* pcrBK,double dZoomFactor,double dAngDeg ) = 0;
	
	/** 显示组成符号的原子 
	* @param [in] geom: 符号的原子对象指针
	* @param [in] env: 设备环境
	* @param [in] nSpecialMode: 显示模式
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_NORMAL-代表正常显示，此时pColor为空
	*			  RENDER2D_GEOMETRY_DISPLAY_MODE_DEFINE_COLOR-以特殊颜色显示，此时pColor不能为空
	* @param [in] pColor - 指定颜色
	* @param [in] orgX,orgY - 插入点设备坐标
	* @param [in] dScaleX - 指定点符号X轴显示比例
	* @param [in] dScaleY - 指定点符号Y轴显示比例
	* @param [in] dAngRad - 符号旋转角度（弧度单位）
	* @return 
	*/
	virtual void drawAtom( GeometrySharedPtr& geom, GraphicsDevice& env,int nSpecialMode,Color4f *pColor,
		double orgX, double orgY, double dScaleX,double dScaleY, double dAngRad) = 0;
};
/** @}*/

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
