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

#include "object.h"

begin_gtl_namespace 
template <
	typename T=char,
	typename TRAITS=std::char_traits<T>,
	typename ALLOC=std::allocator<T> >
class Buffer : public Object <T,TRAITS,ALLOC> {
public:
	virtual std::basic_string<T,TRAITS,ALLOC> getClassName(){
		return "Buffer";
	}
	virtual void writeBuffer(void*  & buffer, size_t & count){
		count =  sizeof(uint32_t)  // _size
			+sizeof(uint32_t)//_capacity
			+sizeof(uint32_t)//_growth
			+sizeof(uint32_t)//_curpos
			+sizeof(uint8_t);//_alloc

		if(_alloc){
			count+=_capacity;//
		}
		else{
			count+=sizeof(uint64_t);//
		}
			
		if(buffer==0)
			buffer = new uint8_t[count];

		uint32_t * p = (uint32_t *) buffer;
		*p=_size;
		p++;
		*p=_capacity;
		p++;
		*p=_growth;
		p++;
		*p=_curpos;
		p++;
		uint8_t *p8 =  (uint8_t*)p;

		if(_alloc)
			*p8= 1 ;
		else
			*p8 = 0;

		p8++;

		if(_alloc){
			memcpy(p8,_data,_capacity);
		}
		else{
			uint64_t* p64= (uint64_t*) p8;
			*p64 = (uint64_t) _data;
		}

		
	}
	virtual void readBuffer(const void*  buffer, size_t count=0){
		clear();
		count =  sizeof(uint32_t)  // _size
			+sizeof(uint32_t)//_capacity
			+sizeof(uint32_t)//_growth
			+sizeof(uint32_t)//_curpos
			+sizeof(uint8_t);//_alloc

		if(_alloc){
			count+=_capacity;//
		}
		else{
			count+=sizeof(uint64_t);//
		}
	
		uint32_t * p = (uint32_t *)buffer;
		*p=_size;
		p++;
		*p=_capacity;
		p++;
		*p=_growth;
		p++;
		*p=_curpos;
		p++;
		uint8_t *p8 =  (uint8_t*)p;

		if(*p8 ==1)
			_alloc=true;
		else 
			_alloc=false;

		p8++;

		if(_alloc){
			_data = new uint8_t[_capacity];
			memcpy(_data,p8,_capacity);
		}
		else{
			uint64_t* p64= (uint64_t*) p8;
			if(sizeof(void*)<sizeof(uint64_t)){
				uint32_t address = (uint32_t)p64;
				memcpy(&_data,&address,sizeof(uint32_t));
			}
			else{
				memcpy(&_data,&p64,sizeof(uint64_t));				
			}			
		}
	}
	virtual Object * clone() {
		Buffer * buf = new Buffer();
		buf->copy(*this);
		return buf;
	}
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
	inline void seek(size_t pos){	
		if(_size>0){
			_curpos = pos;
			assert(pos < _size && pos >= 0);
		}			
	}
	/** 获取缓冲区中当前指针位置
	* @param 无
	* @return 内存块的指针位置，从头地址开始计算，单位为字节
	*/
	inline const size_t tell() const {	return _curpos;	}
	/** 获取当前位置指针
	* @param 无
	* @return 内存块的当前读写指针
	*/
	inline void * tell() { return (void*)(((char *)_data)+_curpos);}
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
	size_t read(void* cdata, size_t count);
	/** 在Buffer对象默认的位置，向后写入字符串
	* @param [in] sz 将被写入的字符串
	* @return
	*/
	void write(const std::basic_string<T,TRAITS,ALLOC> & sz);
	/** 在Buffer对象默认的位置，读出字符串
	* @param [in] sz 将被写入的字符串
	* @return
	*/
	void read(std::basic_string<T,TRAITS,ALLOC> & sz);
	/** 在Buffer对象默认的位置，读出字符串
	* @return 字符串
	*/
	std::basic_string<T,TRAITS,ALLOC> readString();
		
	/** 将指定的文件内容读入Buffer中
	* @param [in] pathName 文件全名
	* @return
	*/
	bool loadFile(const std::basic_string<T,TRAITS,ALLOC> & pathName);

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
};

/** @} */ 
////////////////////////////////////////////////////////////////////////////////////////////////////

end_gtl_namespace

#include "buffer.inl"