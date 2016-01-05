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

#include <iosfwd>
#include <ios>
#include <istream>
#include <ostream>
#include <string>    // char traits            
#include <cstddef>   // ptrdiff_t
#include <cmath>
#include "assert.h"

#include "bufferstream.h" 

#include "color.h"
#include "vertex2d.h"
#include "vertex3f.h"
#include "vertex3d.h"
#include "vertex4d.h"
#include "vertexvisitor.h"
#include "vertexcollection2d.h"
#include "sphere3d.h"
#include "globalfuns.h"
#include "quaternion.h"
#include "matrix4x4.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

/** @defgroup Buffer Buffer-内存缓冲区管理类
*  Buffer是一个内存缓冲区管理类,主要用于表示一块内存块。在空间信息入库过程中要经常用到此类。
* 该类和纯虚类IBufferIO一起构建了对象到内存缓冲区的读写机制。所有需要写入数据库的对象，均需要实现IBufferIO。
* 如所有几何对象Geometry需要实现IBufferIO。在数据库中Geometry的读写操作过程如下:
* Buffer buf;
* Geometry g;
* g.writeBuffer(buf);
* writeBlob(buf);//写入数据库
* 读出过程：
* buf.clear();
* readBlob(buf);
* g.readBuffer(buf);  
*  @{
*/
class CORE_API Buffer{
protected:
	/*breif 指向内存块头地址 */
	void * _data;
	/*breif 使用过的内存块大小 */
	size_t _size;
	/*breif 内存块大小 */
	size_t _capacity;
	/*breif 内存增长大小 */
	size_t _growth;
	/*breif 是否是Buffer类自己分配的内存，如果是则为真，反之为假 */
	bool _alloc;
	/*breif 当前位置，默认为0 */
	size_t _curpos;
public:
	/** 获取内存块的头地址
	* @return 内存块的头地址指针
	*/
	inline  void const *  data() const {return _data;}
	/** 获取内存块的头地址
	* @return 内存块的头地址指针
	*/
	inline void *  data(){return _data;}
	/** 获取内存块大小
	* @return 内存块的大小，单位为字节
	*/
	inline size_t size() const {return _size;}
public:
	/** 默认构造函数
	* @return 无
	*/
	Buffer(size_t size=0,size_t growth=512);
	
	/** 清空Buffer
	* @return 无
	*/
	void clear();
	/** 默认析构函数
	* @return 无
	*/
	~Buffer();
	/** 构造函数，如果alloc为真，则会新申请一块大小为size的内存块，并复制data的内容，反之，则指向data
	* @param [in] data 内存块的首地址指针  
	* @param [in] size 将被写入的数据大小
	* @param [in] alloc 是否需要分配新的内存
	* @return 无
	*/
	Buffer(void * data,size_t size,bool alloc=false,size_t growth=512);
	/** 拷贝构造函数
	* @return 无
	*/
	Buffer(const Buffer & b);
	/** 等号赋值操作
	* @return 无
	*/
	Buffer operator = (const Buffer & b);
	/** /拷贝Buffer函数，如果b的_alloc成员为假，则拷贝后的Buffer对象与b公用一个内存块，否则为内存块拷贝
	* @param [in] buf Buffer 
	* @return 无
	*/
	void copy(const Buffer & buf);	
	/** //设置缓冲区，如果alloc为真，则会新申请一块大小为size的内存块，并复制data的内容，反之，则指向data
	* @param [in] data 内存块的首地址指针  
	* @param [in] size 将被写入的数据大小
	* @param [in] alloc 是否需要分配新的内存
	* @return 无
	*/
	void set(void * data, size_t ss,bool b_alloc=false);
	/** 设置缓冲区中当前指针位置
	* @param [in] pos 内存块的指针位置，从头地址开始计算，单位为字节
	* @return 无
	*/
	inline void setPos(size_t pos){	
		if(_size>0){
			_curpos = pos;
			assert(pos < _size && pos >= 0);
		}			
	}
	/** 获取缓冲区中当前指针位置
	* @param 无
	* @return 内存块的指针位置，从头地址开始计算，单位为字节
	*/
	inline size_t getPos() {	return _curpos;	}
	/** 获取当前位置指针
	* @param 无
	* @return 内存块的当前读写指针
	*/
	inline void * getCurPtr() { return (void*)(((char *)_data)+_curpos);}
	/** 重新设置Buffer大小
	* @param [in] newsize    新的Buffer大小
	* @return 
	*/
	void resize(size_t newsize);
	/** 将已经存在的内存块交付给Buffer来进行管理，外部不能对这个内存块进行销毁
	* @param [in] pBlock    内存块首地址指针
	* @param [in] blockSize 内存块大小，单位为字节
	* @return 
	*/
	void attach(void *pBlock,size_t blockSize);
	/** 将Buffer管理的内存块的管理权限释放出来，，用完后需要自行释放内存
	* @param [out] s 返回内存块的大小，单位返回该内存块的指针为字节
	* @return 返回该内存块的指针
	*/
	void* detach(size_t & s);
	/** 设置内存值，在指定的位置beginpos处向后写入count个字符
	* @param [in] beginpos 起始写入位置  
	* @param [in] cdata 将被写入的数据指针
	* @param [in] count 将被写入的数据大小
	* @return
	*/
	void write(size_t beginpos,void * cdata,size_t count);

	/** 设置内存值，在Buffer对象默认的位置，向后写入count个字符
	* 写入操作后，当前指针相对内存块头的位置自动向后移动count位
	* @param [in] cdata 将被写入的数据指针
	* @param [in] count 将被写入的数据大小
	* @return
	*/
	void write(void * cdata, size_t count);

	/** 获取内存值，在Buffer对象默认的位置，向后获取count个字符
	* 获取操作后，当前指针相对内存块头的位置自动向后移动count位
	* @param [in] cdata 将被赋值的数据指针
	* @param [in] count	将获取的数据大小
	* @return
	*/
	void read(void* cdata, size_t count);
	/** 在Buffer对象默认的位置，向后写入字符串
	* @param [in] sz 将被写入的字符串
	* @return
	*/
	void write(const std::string & sz);
	/** 在Buffer对象默认的位置，读出字符串
	* @param [in] sz 将被写入的字符串
	* @return
	*/
	void read(std::string & sz);
	/** 在Buffer对象默认的位置，读出字符串
	* @return 字符串
	*/
	std::string readString();
	/** 在Buffer对象默认的位置，写入一个Envelope3d对象
	* @param [in] envelope 被写入的Envelope3d
	* @return
	*/
	void write(Envelope3d & envelope );
	/** 在Buffer对象默认的位置，读出一个Envelope3d对象
	* @param [in] envelope 读出的Envelope3d
	* @return
	*/
	void read(Envelope3d & envelope );
	/** 在Buffer对象默认的位置，写入一个Vertex3dList对象
	* @param [in] vlist 被写入的Vertex3dList
	* @return
	*/
	void write(Vertex3dList & vlist );
	/** 在Buffer对象默认的位置，读出一个Vertex3dList对象
	* @param [in] vlist 读出的Vertex3dList
	* @return
	*/
	void read(Vertex3dList & vlist );
	/** 在Buffer对象默认的位置，写入一个Color3bList对象
	* @param [in] clist 被写入的Color3bList
	* @return
	*/
	void write(Color3bList & clist );
	/** 在Buffer对象默认的位置，读出一个Color3bList对象
	* @param [in] clist 读出的Color3bList
	* @return
	*/
	void read(Color3bList & clist );	
	/** 将指定的文件内容读入Buffer中
	* @param [in] pathName 文件全名
	* @return
	*/
	bool loadFile(const string & pathName);
};

/** @} */ 
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace

