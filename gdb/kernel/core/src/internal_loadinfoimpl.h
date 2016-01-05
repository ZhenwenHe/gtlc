#pragma once
#include "feature.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace 

/** @defgroup LODInfoImpl Lod信息管理类
*  @{
*/
class LODInfoImpl :public LODInfo {
public:
	/*
	CameraPosition ----------------D0-----------------D1-------------------D2-------------------D3-------------------D4-----------------
	G0                 G1                   G2                   G3                   G4
	*/

	/** @name 保护成员变量说明
	*  @{
	*/
protected:
	/**要素对象ID*/
	FID             _fid;
	/**当前级别*/
	int             _cur_level;
	/**Lod信息集*/
	std::vector<Info>  _infos;
	/**buffer的大小*/
	unsigned long _buffersize;

	/** @} */
public:
	/** @name 构造与析构函数
	*  @{
	*/
	LODInfoImpl();
	~LODInfoImpl();
	/** @} */


	/** @name 属性设置获得操作集
	*  @{
	*/

	/**获取当前级别
	* @param  无
	* @return   当前级别
	*/
	inline int getCurLevel(){
		return _cur_level;
	}
	/**根据距离判断LOD级别
	* @param  [in] dis 距离
	* @return   LOD级别,-1不显示
	*/
	inline int getLevel(double dis){//
		for (size_t j = 0; j<_infos.size(); j++){
			if (dis < _infos[j].first.lodparam.dSwitchOut) {
				return _infos[j].first.lodLevel;
			}
		}
		return -1;
	}
	/**设置要素对象ID
	* @param [in] id 要素对象ID
	* @return  无
	*/
	inline void setID(FID id) { _fid = id; }
	/**获取要素对象ID
	* @param  无
	* @return   要素对象ID
	*/
	inline FID getID() { return _fid; }

	/**获取buffer的大小
	* @param  无
	* @return   buffer的大小
	*/
	inline unsigned long getBufferSize(){ return _buffersize; }
	/**设置buffer的大小
	* @param [in] buffersize buffer的大小
	* @return 无
	*/
	inline void  setBufferSize(unsigned long buffersize){ _buffersize = buffersize; }

	/**获取总的级别数
	* @param  无
	* @return   总的级别数
	*/
	inline int getLevels(){
		return	_infos.size();
	}
	/**获取LOD信息
	* @param  无
	* @return   LOD信息
	*/
	inline std::vector<Info> & getLods(){
		return _infos;
	}

	/**添加几何对象
	* @param  [in] lodinfo LOD信息
	* @param  [in] g 几何对象指针
	* @return  成功true，失败false
	*/
	bool addInfo(const LOD_INFO &lodinfo, GeometrySharedPtr g);




	/**供feature使用，只将LOD的级别及每个级别的对应的参数数据写入文件:该信息随着Geometry对象走
	* @param  无
	* @return  需要的buffer大小
	*/
	virtual size_t sizeBuffer();
	/** @} */

	/** @name 文件IO操作函数集
	*  @{
	*/

	/**将lod信息写入Buffer
	* @param  [in] buf
	* @return  成功true，失败false
	*/
	virtual bool writeBuffer(Buffer& buf);
	/**从Bufferlod读取lod信息
	* @param  [in] buf
	* @return  成功true，失败false
	*/
	virtual bool readBuffer(const Buffer & buf);

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
};
/** @} */ // End of the group block


 
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
