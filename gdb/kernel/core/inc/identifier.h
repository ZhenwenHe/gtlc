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
#include <memory>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <set>
#include "time.h"
#include "config.h"



begin_gtl_namespace
begin_gdb_namespace

/** @defgroup Identifier Identifier-唯一标识
* 其实质是一个64位长整数
*  @{
*/
class CORE_API Identifier{
	/** @name ULONGNumber 32位整数联合体
	*  @{
	*/
	union ULONGNumber
	{
		unsigned long ns;
		unsigned char bs[4];
	};
	/** @} */

	/** @name 64位整数联合体
	*  @{
	*/
	union {
		struct 	{
			/** 用户ID区域 */
			unsigned char user_id;
			/** 自动增加区域 */
			unsigned char numbers[3];
			/** ID生成时间 */
			unsigned long time_stamp;
		}_sid;
		/** 64位原始值 */
		unsigned long long  _id;
		/** 以BYTE表示的64位值 */
		unsigned char _bid[8];
	};
	/** @} */

public:
	/** @name 原始值类型定义
	*  @{
	*/
	typedef unsigned long long raw_type;
	/** @} */
public:
	/** @name 构造与析构函数集
	*  @{
	*/
	/** 采用随机用户ID生成Identifier */
	Identifier();
	Identifier(int l) { _id = l; }
	Identifier(unsigned long l) { _id = l; }
	Identifier(unsigned int l) { _id = l; }
	Identifier(unsigned long long l) { _id = l; }
	Identifier(double l) { _id = (unsigned long long)l; }
	Identifier(unsigned char user_id);
	Identifier(const Identifier & ids);
	/** @} */
public:
	/** @name 静态生成函数集
	*  @{
	*/
	/**  生成一个值为0的ID对象
	*	 @param 无
	*   @return Identifier对象
	*/
	static Identifier zero() { return Identifier(0); }
	/**  生成一个唯一的原始ID值，为一个64位整数
	*	 @param 无
	*   @return 返回原始64位ID
	*/
	static raw_type  generate();
	/** @} */
public:
	/** @name 获取Identifier的值
	*  @{
	*/
	inline unsigned long long  get() { return _id; }
	inline const unsigned long long  get() const { return _id; }
	inline unsigned long getTimestamp(){ return _sid.time_stamp; }
	inline unsigned char getUserID() { return _sid.user_id; }
	inline unsigned char * getBytes(){ return _bid; }
	/** @} */
public:
	/** @name 赋值等号重载
	*  @{
	*/
	Identifier operator = (const Identifier&  ids);
	Identifier operator = (unsigned long long  ids);
	Identifier operator = (int  ids);
	Identifier operator = (unsigned int  ids);
	Identifier operator = (unsigned long   ids);
	/** @} */

	/** @name 类型转换操作重载
	*  @{
	*/
	operator unsigned long long() { return _id; }
	operator unsigned long() { return (unsigned long)_id; }
	operator unsigned int() { return (unsigned int)_id; }
	operator int() { return (int)_id; }
	unsigned long long * operator &() { return &_id; }
	/** @} */
public:

	/** @name 静态友元符号重载
	*  @{
	*/
	friend  bool operator==(const Identifier & id1, const Identifier & id2);
	friend  bool operator==(const Identifier & id1, unsigned long long  id2);

	friend  bool operator!=(const Identifier & id1, const Identifier & id2);
	friend  bool operator!=(const Identifier & id1, unsigned long long  id2);

	friend  bool operator<(const Identifier & id1, const Identifier & id2);
	friend  bool operator<(const Identifier & id1, unsigned long long  id2);

	friend  bool operator>(const Identifier & id1, const Identifier & id2);
	friend  bool operator>(const Identifier & id1, unsigned long long  id2);


	friend  Identifier operator-(const Identifier & id1, const Identifier & id2);
	friend  Identifier operator-(const Identifier & id1, unsigned long long  id2);

	friend  Identifier operator+(const Identifier & id1, const Identifier & id2);
	friend  Identifier operator+(const Identifier & id1, unsigned long long  id2);


	friend  std::ostream & operator <<(std::ostream & s, const Identifier & id1);
	friend  std::istream & operator >>(std::istream & s, const Identifier & id1);

	/** @} */
public:
	/** @name 流读写操作
	*  @{
	*/
	virtual void  write(std::ostream & f);
	virtual void  read(std::istream & f);
	/** @} */
};

/** @name 全局的0值Identifer对象
*  @{
*/
static const Identifier ZEROID = Identifier::zero();
/** @} */

/** @} */


/** @addtogroup Identifier  Identifier类的外部静态符号重载函数集
*  @{
*/
static bool operator==(const Identifier & id1, const Identifier & id2){
	return id1._id == id2._id;
}

static bool operator==(const Identifier & id1, unsigned long long  id2){
	return id1._id == id2;
}

static bool operator!=(const Identifier & id1, const Identifier & id2){
	return id1._id != id2._id;
}

static bool operator!=(const Identifier & id1, unsigned long long  id2){
	return id1._id != id2;
}

static bool operator>(const Identifier & id1, const Identifier & id2){
	return id1._id>id2._id;
}

static bool operator>(const Identifier & id1, unsigned long long  id2){
	return id1._id>id2;
}

static bool operator<(const Identifier & id1, const Identifier & id2){
	return id1._id<id2._id;
}

static bool operator<(const Identifier & id1, unsigned long long  id2){
	return id1._id<id2;
}

static Identifier operator+(const Identifier & id1, const Identifier & id2){
	return Identifier(id1._id + id2._id);
}

static Identifier operator+(const Identifier & id1, unsigned long long  id2){
	return Identifier(id1._id + id2);
}

static Identifier operator-(const Identifier & id1, const Identifier & id2){
	return Identifier(id1._id - id2._id);
}

static Identifier operator-(const Identifier & id1, unsigned long long  id2){
	return Identifier(id1._id - id2);
}


static std::ostream & operator <<(std::ostream & s, const Identifier & id1){
	s << id1._id;
	return s;
}

static std::istream & operator >>(std::istream & s, const Identifier & id1){
	s >> id1._id;
	return s;
}

/** @} */


/** @addtogroup Identifier  系统常用ID类型定义
*  @{
*/
/** 工程ID */
typedef unsigned long long PRJID;
/** 分区ID */
typedef unsigned long long REGID;
/** 工作区ID */
typedef REGID             WKSPID;
/** 要素类型ID */
typedef unsigned long long FCLSID;
/** 要素ID */
typedef unsigned long long FID;
/** 要素集ID */
typedef unsigned long long FSID;
/** 要素LOD的ID */
typedef unsigned long long FLOD;
/** 纹理ID */
typedef unsigned long long TEXID;
/** 图层ID */
typedef unsigned long long LYRID;
/** 共用模型的ID */
typedef unsigned long long MDLID;
/** 空间参考系统ID */
typedef unsigned long long SREFID;
/** 用户记录数据更新时间 */
typedef unsigned long long GDBTIME;
/** 材质ID */
typedef unsigned long long MATID;
/** 注记ID */
typedef unsigned long long ANNOID;
/** 语义专题集ID */
typedef unsigned long long SEMID;
/** 关系类ID */
typedef unsigned long long RELID;
/** 语义拓扑对象ID; */
typedef unsigned long long TID;
/** @} */

end_gdb_namespace
end_gtl_namespace


