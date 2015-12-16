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
#include <stack>
#include "object.h"

begin_gtl_namespace

template<
	typename CHARTYPE=char,
	typename TRAITS=std::char_traits<CHARTYPE>,
	typename ALLOC=std::allocator<CHARTYPE> >
class Bitset :  public Object<CHARTYPE,TRAITS,ALLOC>{
	/*
		_validbitposition=19, the valid bit count is 20, bytecount = 19/8+1=3, 	bitindex = 7-19%8=7-3=4, byteindex=19/8
	                     |
	00000001 00000011 00110000 bitset
	01234567 01234567 01234567 bitindex
	_byte[0] _byte[1] _byte[2]
	0        1        2        byteindex
	1        3        48       bytevalue
	00000001 00000011 00110000 represent number(66352)
	*/
	unsigned char * _bytes; //[_validbitpos/8+1]
	unsigned int  _validbitposition;

	inline unsigned int byteIndex(unsigned int pos) const {return pos/8;}
	inline unsigned int byteCount() const {return _validbitposition/8+1;}
	inline unsigned int bitIndex(unsigned int pos) const {return (8-1)-pos%8;}
	inline unsigned int bitCount()const {return _validbitposition+1;}
	inline void ull2bin(unsigned long long ull, std::stack<unsigned char> & binstack)
	{
		//transfer to binary string
		while(ull!=0)
		{
			binstack.push((unsigned char)(ull & 0x01));
			ull = ull >> 1;
		}
		//delete the invalid zero 
		while((!binstack.empty()))
		{
			if(binstack.top()==0)
				binstack.pop();
			else
				break;
		}
	}
public:
	virtual std::basic_string<CHARTYPE,TRAITS,ALLOC> getClassName(){
		return "Bitset";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		count = byteCount() + sizeof(unsigned int);
		buffer =(void*) new char[count];
		unsigned int * p = (unsigned int *)buffer;
		*p = _validbitposition;
		p++;
		memcpy(p, _bytes, byteCount());
	}
	virtual void readBuffer(const void *  buffer, size_t count ){
		unsigned int * p = (unsigned int *)buffer;
		_validbitposition = *p;
		p++;
		if (_bytes){
			delete _bytes;
		}
		_bytes = new unsigned char[byteCount()];
		memcpy(_bytes,p, byteCount());		
	}
	virtual Object<CHARTYPE,TRAITS,ALLOC> * clone() {
		return new Bitset(*this);
	}
public:
	//返回已经设置的位数
	size_t count() {
		if(_bytes)
			return bitCount();
		else
			return 0;
	}

	bool get(size_t pos) const
	{
		if(pos <= _validbitposition)
		{
			unsigned char t=1;
			t = t<< bitIndex(pos);
			return (_bytes[byteIndex(pos)]&t) !=0;
		}
		else
			return false;
	}
	//设置指定位置为0或1，true表示1，false表示0，如果pos大于数组长度，则自动扩展
	void set(size_t pos, bool b = true ){

		if(_bytes==0) 
		{
			_validbitposition = pos;
			_bytes = new unsigned char [byteCount()];
			memset(_bytes,0,byteCount()); 
		}
		else
		{
			if(pos>=byteCount()*8)
			{
				unsigned int oldbytecount = byteCount();
				_validbitposition = pos;
				unsigned char * newbytes = new unsigned char [byteCount()];
				memset(newbytes,0,byteCount());
				memcpy(newbytes,_bytes,oldbytecount);
				delete [] _bytes;
				_bytes= newbytes;
			}
			else
			{
				if(_validbitposition < pos)
					_validbitposition=pos;
			}
		}
		

		unsigned char t=1;
		t = t<< bitIndex(pos);
		if(b)
		{
			_bytes[byteIndex(pos)] |= t;
		}
		else
		{
			t = ~t;
			_bytes[byteIndex(pos)] &= t;
		}
	}
	
	void set(const char * binstring)
	{
		if(_bytes) delete [] _bytes;
		_validbitposition = strlen(binstring)-1;
		_bytes = new unsigned char [byteCount()];
		memset(_bytes,0,byteCount());
		for(int i=0;i<=_validbitposition;i++)
			if(binstring[i]=='1')
				set(i,true);
			else
				set(i,false);
	}

	void set(const unsigned char * flags,unsigned int len)
	{
		if(_bytes) delete [] _bytes;
		_validbitposition = len-1;
		_bytes = new unsigned char [byteCount()];
		memset(_bytes,0,byteCount());
		for(int i=0;i<=_validbitposition;i++)
			if(flags[i]=='1' || flags[i]==1)
				set(i,true);
			else
				set(i,false);
	}


	std::size_t get(std::basic_string<CHARTYPE,TRAITS,ALLOC> & sz)
	{
		sz.resize(_validbitposition+1);
		for(int i=0;i<=_validbitposition;i++)
		{
			if(get(i))
				sz[i]='1';
			else
				sz[i]='0';
		}
		return _validbitposition+1;
	}
	std::size_t get(std::stack<char> & sz)
	{
		sz.resize(_validbitposition+1);
		for(int i=0;i<=_validbitposition;i++)
		{
			if(get(i))
				sz.push('1');
			else
				sz.push('0');
		}
		return _validbitposition+1;
	}
	//将位数组转换成整数，最高位放在[0]位置
	unsigned long long ullong()
	{
		if(_bytes==0) 
			return 0;		
		unsigned long long s = get(0)? 1:0;
		unsigned int i=1;
		for(i ;i<= _validbitposition; ++i)//unsigned long long has 8 bytes
			s=(s<<1) + (get(i)?1:0);
		return s;
	}
	//返回指定位置的值，如果pos大于位数组长度，自动拓展
	bool operator [] (const size_t pos)
	{
		return get(pos);
	}

	//直接整数转化成二进制，赋值给Bitset，最高位放在[0]位置
	Bitset operator = (unsigned long long ull)
	{
		std::stack<unsigned char> binstack;
		ull2bin(ull,binstack);
		int i=0;
		while((!binstack.empty()))
		{
			set(i,binstack.top()==1); 
			binstack.pop();
			i++;
		}
		return *this;
	}
	//默认构造
	Bitset()
	{
		_validbitposition=0;
		_bytes=0;
	}
	//直接整数转化成二进制，赋值给Bitset，最高位放在[0]位置
	Bitset(unsigned long long ull)
	{
		_validbitposition=0;
		_bytes=0;

		std::stack<unsigned char> binstack;
		ull2bin(ull,binstack);
		int i=0;
		while((!binstack.empty()))
		{
			set(i,binstack.top()==1); 
			binstack.pop();
			i++;
		}
	}
	//拷贝构造函数
	Bitset(const Bitset & b)
	{
		_validbitposition = b._validbitposition;
		unsigned int bc = byteCount();
		_bytes = new unsigned char [bc];
		memset(_bytes,0,bc);
		memcpy(_bytes,b._bytes,bc);
	}

	Bitset operator = (const Bitset & b)
	{
		_validbitposition = b._validbitposition;
		unsigned int bc = byteCount();
		_bytes = new unsigned char[bc];
		memset(_bytes, 0, bc);
		memcpy(_bytes, b._bytes, bc);
		return *this;
	}

	//测试两个Bitset是否相等
	bool operator == (const Bitset & b)
	{
		if(_validbitposition != b._validbitposition) 
			return false;
		else{
			if(_validbitposition==0)
				return true;

		}
		return memcmp(_bytes,b._bytes,byteCount())==0;
	}
	bool operator == (const unsigned long long  & bl)
	{
		Bitset b(bl);
		if (_validbitposition != b._validbitposition)
			return false;
		else{
			if (_validbitposition == 0)
				return true;

		}
		return memcmp(_bytes, b._bytes, byteCount()) == 0;
	}
	bool operator != (const Bitset & b)
	{
		if (_validbitposition != b._validbitposition)
			return true;
		else{
			if (_validbitposition == 0)
				return false;

		}
		return memcmp(_bytes, b._bytes, byteCount()) != 0;
	}
	bool operator != (const unsigned long long  & bl)
	{
		Bitset b(bl);
		if (_validbitposition != b._validbitposition)
			return true;
		else{
			if (_validbitposition == 0)
				return false;

		}
		return memcmp(_bytes, b._bytes, byteCount()) != 0;
	}
	//将高位与低位互相，如数组存放的是1011，则本函数执行后为1101
	void reverse()
	{
		std::stack<char> sk;
		get(sk);
		int i=0;
		while((!sk.empty()))
		{
			set(i,sk.top()=='1'); 
			sk.pop();
			i++;
		}
	}
	//左移动n位，返回新的Bitset，不影响本类的值
	Bitset leftShift(size_t n)
	{
		Bitset bs(*this);
		for(int i=1;i<=n;++i)
			bs.set(i+_validbitposition,0);
		return bs;
	}
	//右移动n位，返回新的Bitset，不影响本类的值
	Bitset rightShift(size_t n)
	{
		Bitset bs(*this);
		for(int i=0;i<n;++i){
			bs.set(bs._validbitposition,0);
			-- bs._validbitposition;
		}
		return bs;
	}	
	//获取左边n位，返回新的Bitset 
	Bitset left(size_t n)const 
	{
		
		if (n < _validbitposition+1)
		{
			Bitset bs;
			for (int i = 0; i <n; ++i)
				bs.set(i, get(i));
			return bs;
		}
		else //如果n大于有效位数，则则直接返回本类的值
		{			
			return *this;
		}
	}
	//获取右边n位，返回新的Bitset 
	Bitset right(size_t n)const
	{
		if (n < _validbitposition + 1)
		{
			Bitset bs;
			int j = 0;
			for (int i = _validbitposition + 1 - n; i <= _validbitposition; ++i){
				bs.set(j, get(i));
				++j;
			}
			return bs;
		}
		else
		{			 
			return *this;
		}
	}
 
	Bitset  operator << ( int c)
	{
			return leftShift(c);
	}
	Bitset  operator >> (int c)
	{
		return rightShift(c);
	} 
	bool  operator > (const Bitset & c)
	{
		if ( _validbitposition == c._validbitposition)
		{
			return memcmp(_bytes, c._bytes, byteCount()) > 0; 
		}
		else if(_validbitposition > c._validbitposition)
		{
			Bitset b1 = left(_validbitposition - c._validbitposition);
			int s = bitCount();
			for (int i = 0; i < s; i++)
				if (b1.get(i))
					return true;
			b1 = right(c._validbitposition);
			return memcmp(b1._bytes, c._bytes, byteCount()) > 0;
		}
		else
		{
			Bitset b1 = c.left(c._validbitposition - _validbitposition);
			int s = bitCount();
			for (int i = 0; i < s; i++)
				if (b1.get(i))
					return false;
			b1 = b1.right(_validbitposition);
			return memcmp(b1._bytes, c._bytes, byteCount()) < 0;
		}
	}
	bool  operator < (const Bitset & c)
	{
		if (_validbitposition == c._validbitposition)
		{
			return memcmp(_bytes, c._bytes, byteCount()) < 0;
		}
		else if (_validbitposition > c._validbitposition)
		{
			Bitset b1 = left(_validbitposition - c._validbitposition);
			int s = bitCount();
			for (int i = 0; i < s; i++)
				if (b1.get(i))
					return false;
			b1 = right(c._validbitposition);
			return memcmp(b1._bytes, c._bytes, byteCount()) < 0;
		}
		else
		{
			Bitset b1 = c.left(c._validbitposition - _validbitposition);
			int s = bitCount();
			for (int i = 0; i < s; i++)
				if (b1.get(i))
					return true;
			b1 = b1.right(_validbitposition);
			return memcmp(b1._bytes, c._bytes, byteCount()) > 0;
		}
	}
	Bitset  operator & (const Bitset & c)
	{
		Bitset bs;
		if (c._validbitposition > _validbitposition)
		{
			bs = c.right(_validbitposition);
			for (int i = 0; i < _validbitposition; i++)
			{
				bs.set(i, get(i)&&bs.get(i));
			}
		}
		else if (c._validbitposition  == _validbitposition)
		{
			bs = *this; 
			for (int i = 0; i < c._validbitposition; i++)
			{
				bs.set(i, c.get(i)&&bs.get(i));
			}
		}
		else
		{
			bs = right(c._validbitposition);
			for (int i = 0; i < c._validbitposition; i++)
			{
				bs.set(i, c.get(i)&&bs.get(i));
			}
		}
		return bs;
	}
	Bitset  operator | (const Bitset & c)
	{
		Bitset bs;
		if (c._validbitposition > _validbitposition)
		{
			bs = c;
			for (int i = 0; i < _validbitposition; i++)
			{
				bs.set(i, get(i)||bs.get(i));
			}
		}
		else
		{
			bs = *this;
			for (int i = 0; i < c._validbitposition; i++)
			{
				bs.set(i, c.get(i) || bs.get(i));
			}
		}
		return bs;
	}
	Bitset  operator ~ ()
	{
		Bitset bs;
		int s = bitCount();
		for (int i = 0; i < s; i++)
		{
			bs.set(i, !get(i));
		}
		return bs;
	}
	void clear(){
		_validbitposition =0;
		if(_bytes) delete [] _bytes;
		_bytes=0;
	}
	//判断本类的位数组是否是b的前缀
	bool isPrefix(const Bitset & b)
	{
		if(_validbitposition>b._validbitposition) return false;
		unsigned int v= min(b._validbitposition, _validbitposition);
		for(int i=0;i<=v;i++)
			if(b.get(i)!=get(i))
				return false;
		return true;
	}
	//判断本类的位数组是否是b的前缀
	bool isPrefix(unsigned long long & b)
	{
		return isPrefix(Bitset(b));
	}
	
	void resize(size_t newSize){
		if(newSize<_validbitposition+1)
		{
			for(int i=_validbitposition;i>=newSize;--i)
				set(i,false);
			_validbitposition=newSize-1;
		}
		else
		{
			set(newSize-1,false);
		}
	} 
};



#if  GTL_VERSION<2
template<
	typename CHARTYPE=char,
	typename TRAITS=std::char_traits<CHARTYPE>,
	typename ALLOC=std::allocator<CHARTYPE> >
class Bitset :  public Object<CHARTYPE,TRAITS,ALLOC>{
	//最高位放在[0]位置，每位的默认值为0
	std::vector<uint8_t> _data;
public:
	virtual std::basic_string<CHARTYPE,TRAITS,ALLOC> getClassName(){
		return "Bitset";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
public:
	//返回已经设置的位数
	size_t count();

	bool get(size_t pos);
	//设置指定位置为0或1，true表示1，false表示0，如果pos大于数组长度，则自动扩展
	void set(size_t pos, bool b = true );
	//将位数组转换成整数，最高位放在[0]位置
	//例如数组中存放的1011，则返回11，而不是返回13
	unsigned long long to_ullong();
	//返回指定位置的值，如果pos大于位数组长度，自动拓展
	bool operator [] (const size_t pos);
	//直接整数转化成二进制，赋值给Bitset，最高位放在[0]位置
	Bitset operator = (unsigned long long ull);
	//默认构造
	Bitset();
	//直接整数转化成二进制，赋值给Bitset，最高位放在[0]位置
	Bitset(unsigned long long n);
	//拷贝构造函数
	Bitset(const Bitset & b);
	//将高位与低位互相，如数组存放的是1011，则本函数执行后为1101
	void reverse();
	//返回左边n位，构成新的Bitset
	Bitset left(size_t n);
	//返回右边n位，构成新的Bitset
	Bitset right(size_t n);

	//测试两个Bitset是否相等
	bool operator == (const Bitset & b);
	//判断b包含的位数组是否是本类的位数组的前缀
	bool has_prefix(const Bitset & b);
	//判断本类的位数组是否是b的前缀
	bool is_prefix(unsigned long long & b);
	//
	void clear();

	void resize(size_t newSize);

	void set(const unsigned char * flags,unsigned long long s); 

	std::basic_string<CHARTYPE,TRAITS,ALLOC> to_string();
};
#endif	//BITSET_VERSION_1

end_gtl_namespace

#if GTL_VERSION<2
#include "bitset.inl"
#endif	//