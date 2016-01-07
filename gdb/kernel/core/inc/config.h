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
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <set>
#include <list>
#include <memory>
#include <cmath> 
#include "time.h"
#include "assert.h"

/*
 * 0-windows, include config_windows.h
 * 1-linux, include config_linux.h
 * 2-maxcos, include config_maxcos.h
 */
#define USING_OS_TYPE  1 //0-windows 1-linux 2-macxos

/*
 * 1-enable console window and display information 
 * 0-disable
 */
#define ENABLE_CONSOLE_OUTPUT 0 
/*
 * 0-MFC , the USING_OS_TYPE must be 0
 * 1-Qt 
 * 2-wxWidget
 */
#define USING_UI_TYPE  1 
/*
 * If the ui library is 0, the os type must be 0.
 */
#if(USING_UI_TYPE==0 && USING_OS_TYPE!=0)
#error operation system does not match the UI library you choose
#endif

/*
 * 0-use geometry plugin library implemented by gv3dgemtry
 * 1-use default geometry library implemented in Core project
 * 2-some other geometry libraries implemented by user
*/
#define USING_3DGEOMETRY_TYPE 1

/*
 * 0-use geometry plugin library;
 * 1-use default geometry library implemented in core project
 */
#define USING_2DGEOMETRY_TYPE 0

/*
 * 0-use geometry plugin library;
 * 1-use default geometry library implemented in core project
 */
#define USING_MESH_TYPE     1


#if(USING_OS_TYPE==0)
#include "config_windows.h"    
#elif(USING_OS_TYPE==1)
#include "config_linux.h"          
#else //MacX OS
#include "config_macxos.h"        
#endif //USING_OS_TYPE
        
#ifdef CORE_EXPORTS
    #ifdef _USRDLL
        #define CORE_API __declspec(dllexport)
    #else
        #define CORE_API  
    #endif
#else
    #define CORE_API __declspec(dllimport)
#endif


#ifndef begin_gtl_namespace
#define begin_gtl_namespace namespace gtl{
#endif

#ifndef end_gtl_namespace
#define end_gtl_namespace  }
#endif

#ifndef using_gtl_namespace
#define using_gtl_namespace  using namespace gtl;
#endif

#ifndef begin_cn_namespace
#define begin_cn_namespace namespace cn{
#endif

#ifndef end_cn_namespace
#define end_cn_namespace }
#endif

#ifndef using_cn_namespace
#define using_cn_namespace using namespace cn;
#endif


#define begin_edu_namespace namespace edu{
#define end_edu_namespace }
#define using_edu_namespace using namespace edu;

#define begin_cug_namespace namespace cug{
#define end_cug_namespace }
#define using_cug_namespace using namespace cug;

#define begin_gdb_namespace namespace gdb{
#define end_gdb_namespace }
#define using_gdb_namespace using namespace gdb;


#define begin_catalog_namespace namespace catalog{
#define end_catalog_namespace }
#define using_catalog_namespace using namespace catalog;


#define begin_sve_namespace namespace sve{
#define end_sve_namespace }
#define using_sve_namespace using namespace sve; 


#define begin_plugin_namespace namespace plugin{   
#define end_plugin_namespace                   }   
#define useing_plugin_namespace using namespace plugin ;  


#define begin_details_namespace namespace details{   
#define end_details_namespace                   }   
#define useing_details_namespace using namespace details ;

using namespace std;

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace


const double  TOLERANCE = 0.0000001;
const double  PI        =  3.14159265358979323846264338327950288419716939937510582;

#ifndef INITMAX
#define INITMAX -9999999999999999999.0
#endif

#ifndef INITMIN
#define INITMIN  9999999999999999999.0
#endif

#ifndef SMALL_NUMBER
#define SMALL_NUMBER 0.0000001
#endif

#include "config_env.h"

class Envelope3d;
/** @defgroup Config  Config-全局配置类
*  @{
*/
class GV3DCORE_API Config {

	std::locale _loc;
	static string   shaderPath;
	static string   imagePath;
private:
	static int  textureFileModel;//	TextureReadModel //0代表IO流小纹理文件，1代表IO流大纹理文件，2代表内存映射纹理文件
	static int 	featureFileModel; //1代表IO流读写，0代表IO内存映射文件读写
private:
	static Envelope3d defaultQueryEnvelope;
	static double unloadDistance;//大于unloadDistance距离的时候，从内存中卸载；
	static double loadDistance;//小于loadDistance距离的时候，不从内存中卸载；	
	static unsigned int  numberOfAddFeaturesPreframe;//每一帧回调加入场景中的最大要素个数
	static unsigned int cachedTexturesOfRender;//渲染引擎中的纹理缓存大小，单位为纹理个数
	static unsigned int cachedSharedModelsOfRender;//渲染引擎中的共享模型缓存大小，单位为纹理个数
private:
	static unsigned int versionFileDB;//文件数据库版本
	static unsigned int versionRDB;//关系数据库版本
private:
	static unsigned  int cachedBlockNumber;//缓存中网格块的最大个数
	static unsigned int useSameBlockID4DomDem;//DEM和DOM块的ID是否对应
private://投影参数
	static double fovy;
	static double zNear;
	static double zFar;
private://Display Mode
	static unsigned int stereo;//0-disable 1-enable	 
	/*QUAD_BUFFER=0,
	ANAGLYPHIC=1,
	HORIZONTAL_SPLIT=2,
	VERTICAL_SPLIT=3,
	LEFT_EYE=4,
	RIGHT_EYE=5,
	HORIZONTAL_INTERLACE=6,
	VERTICAL_INTERLACE=7,
	CHECKERBOARD=8*/
	static unsigned int stereoMode;
private://Dynamic Dispaching
	static unsigned int dispachMode;
public:
	static int cameraLightStatus;
private:
	static unsigned int currentCommand;//当前按钮的ID	
public:
	Config();
public:
	static string   getShaderPath();
	static string   getImagePath();
	static void   setShaderPath(string);
	static void   setImagePath(string);
	static int getTextureFileModel();
	static int getFeatureFileModel();
	static Envelope3d&  getDefaultQueryEnvelope();
	static double getUnloadDistance();
	static double getLoadDistance();
	static double getFovy();
	static double getZNear();
	static double getZFar();
	static int getNumberOfAddFeaturesPreframe();
	static unsigned int getCachedTexturesOfRender();//渲染引擎中的纹理缓存大小，单位为纹理个数
	static unsigned int getCachedSharedModelsOfRender();//渲染引擎中的共享模型缓存大小，单位为纹理个数
	static unsigned int getCachedBlockNumber();//缓存中网格块的最大个数
	static unsigned int  getFDBVersion();//获取文件库版本
	static unsigned int  getRDBVersion();//获取数据库版本
	static unsigned int  sameID4DEMDOMBlock();
	static int getCameraLightStatus();
	static void enableStereo(bool s = true);
	static bool isEnableStereo();
	static unsigned int getStereoMode();
	static bool isDynamicDispatching();
	static std::vector<unsigned long long> & getCacheSizes();
public:
	enum{//应用程序中固定的操作命令，一般这些命令功能已经由Render封装了
		APP_COMMAND_UNKNOWN = 0,
		APP_COMMAND_SELECT,//选择
		APP_COMMAND_ROTATE,//左键旋转
		APP_COMMAND_ZOOM,//右键缩放
		APP_COMMAND_TRANSLATE,//中键平移
		APP_COMMAND_END
	};
	static unsigned int getCurrentCommand();
	static void setCurrentCommand(unsigned int cmd);
	static unsigned int beginPluginCommand();//获取所有插件集的最小CommandID
	static unsigned int endPluginCommand();//获取所有插件集的最大CommandID
};
/** @} */


end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
