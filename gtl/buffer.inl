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

begin_gtl_namespace

template<typename T,typename TRAITS,typename ALLOC>
Buffer<T,TRAITS,ALLOC>::Buffer(size_t size ,size_t growth){//默认构造函数
	_data=0;	
	_curpos=0;	
	_growth=growth;
	if(size>0){
		_size=size;
		_alloc=true;
		_capacity=_size;
		_data = new T [_capacity];
	}
	else{
		_capacity=0;
		_size=0;
		_alloc=false;
	}
}
template<typename T,typename TRAITS,typename ALLOC>
void Buffer<T,TRAITS,ALLOC>::clear(){//清空Buffer
	if(_alloc && _data){
		delete  [] ((T*)_data);
	}
	_data = 0;
	_size=0;
	_alloc=false;
	_curpos=0;
	_capacity=0;
	_growth=512;
}
template<typename T,typename TRAITS,typename ALLOC>
Buffer<T,TRAITS,ALLOC>::~Buffer(){//默认析构函数
	clear();
}
template<typename T,typename TRAITS,typename ALLOC>
Buffer<T,TRAITS,ALLOC>::Buffer(void * d,size_t s,bool b_alloc,size_t growth){//构造函数，如果b_alloc为真，则会新申请一块大小为s的内存块，并复制d的内容，反之，则指向d
	_size=s;

	_capacity=s;

	_growth=growth;

	_alloc=b_alloc;
	if(_alloc){
		_data = new T[_size];
		memcpy(_data,d,_size);
	}
	else{
		_data=d;
	}
	_curpos=0;
}
template<typename T,typename TRAITS,typename ALLOC>
void Buffer<T,TRAITS,ALLOC>::copy(const Buffer & b){//拷贝Buffer函数，如果b的_alloc成员为假，则拷贝后的Buffer对象与b公用一个内存块，否则为内存块拷贝
	set(b._data,b._size,b._alloc);
	_curpos=0;
}
template<typename T,typename TRAITS,typename ALLOC>
Buffer<T,TRAITS,ALLOC>::Buffer(const Buffer & b){//拷贝构造函数
	_data = 0;
	_size=0;
	_alloc=false;
	_curpos=0;
	_capacity=0;
	_growth=512;
	copy(b);
}
template<typename T,typename TRAITS,typename ALLOC>
Buffer<T,TRAITS,ALLOC> Buffer<T,TRAITS,ALLOC>::operator = (const Buffer & b){//等号赋值操作
	copy(b);
	return * this;
}
template<typename T,typename TRAITS,typename ALLOC>
void Buffer<T,TRAITS,ALLOC>::set(void * pv, size_t ss,bool b_alloc){//设置缓冲区，如果b_alloc为真，则会新申请一块大小为ss的内存块，并复制pv的内容，反之，则指向pv
	clear();
	_alloc = b_alloc;
	if(_alloc){
		_data = new T[ss];
		_size = ss;
		_capacity=ss; 
		if(pv)
			memcpy(_data,pv,ss);
	}
	else{
		_data = pv;
		_size = ss;
		_capacity=ss;
	}
	_curpos=0;
}

template<typename T,typename TRAITS,typename ALLOC>
void Buffer<T,TRAITS,ALLOC>::attach(void * pv, size_t ss){
	clear();
	_data = pv;
	_size = ss;
	_capacity=ss;
	_alloc=true;
}
template<typename T,typename TRAITS,typename ALLOC>
void* Buffer<T,TRAITS,ALLOC>::detach(size_t & s){		
	s=_size;
	_alloc=false;
	void* p =_data;
	_data = 0;
	_curpos=0;
	_size=0;
	_capacity=0;
	return p;
}
template<typename T,typename TRAITS,typename ALLOC>
void Buffer<T,TRAITS,ALLOC>::write(size_t beginpos,void * cdata,size_t count)//在指定的位置beginpos处向后写入count个字符
{
	if(count==0||cdata==0)return ;

	if(beginpos+count<=_capacity){
		memcpy(((T*)_data)+beginpos,cdata,count);
		//扩展使用过的内存大小计数
		if(_size<beginpos+count)
			_size = beginpos+count;
	}
	else{//需要申请扩大内存容量
		size_t new_capacity = ((beginpos+count)/_growth+1)*_growth;
		T * p = new T[new_capacity];
		memcpy(p,_data,beginpos);
		delete [] ((T*)_data);
		memcpy(p+beginpos,cdata,count);
		//T * p = new T [beginpos+count];
		//memcpy(p,_data,beginpos);
		//memcpy(p+beginpos,cdata,count);		
		_data = p;
		_size = beginpos+count;
		_capacity = new_capacity;

		// 由于调用了delete [] ((T*)_data); _data = p;  因此，如果内存是以attach方式得到的，
		//则外部的管理者无法实现新内存的析构,故需添加下面的代码或者外面调用clear函数 [7-29-2010 孙卡]
		_alloc = true;
	}
	//移动当前指针位置
	_curpos = beginpos + count;
}
template<typename T,typename TRAITS,typename ALLOC>
void Buffer<T,TRAITS,ALLOC>::write(void * cdata, size_t count)
{
	if(count==0||cdata==0)return ;

	if(_curpos+count<=_capacity){
		memcpy(((T*)_data)+_curpos,cdata,count);
		//移动当前指针位置
		_curpos += count;
		//扩展使用内存的大小
		if(_size<=_curpos)
			_size = _curpos;
	}
	else{
		size_t new_capacity = ((_curpos+count)/_growth+1)*_growth;
		T * p = new T[new_capacity];
		memcpy(p,_data,_curpos);
		delete [] ((T*)_data);
		memcpy(p+_curpos,cdata,count);

		///**修改_curpos增加为新值[2009-09-17cuiyt]*/
		//_size = _curpos+count;
		//T * p = new T [_size];
		//memcpy(p,_data,_curpos);
		//memcpy(p+_curpos,cdata,count);
		//delete [] ((T*)_data);
		_data = p;
		_curpos += count;	
		//扩展使用内存的大小
		if(_size<=_curpos)
			_size = _curpos;

		_capacity = new_capacity;

		// 由于调用了delete [] ((T*)_data); _data = p;  因此，如果内存是以attach方式得到的，
		//则外部的管理者无法实现新内存的析构,故需添加下面的代码或者外面调用clear函数 [7-29-2010 孙卡]
		_alloc = true;
	}
}
template<typename T,typename TRAITS,typename ALLOC>
size_t Buffer<T, TRAITS, ALLOC>::read(void * cdata, size_t count)
{
	if(_curpos+count<=_size){
		memcpy(cdata,((T*)_data)+_curpos,count);
		_curpos += count;
		return count;
	}
	else{
		count = _size - _curpos;
		memcpy(cdata,((T*)_data)+_curpos,count); 
		return count;
	}
}
template<typename T,typename TRAITS,typename ALLOC>
void Buffer<T,TRAITS,ALLOC>::write(const std::basic_string<T,TRAITS,ALLOC> & sz){
	int s = sz.size();
	write((void*)&s,sizeof(int));
	if(s>0){
		const T * sc = sz.c_str();
		write((void*)sc,s*sizeof(T));
	}
}
template<typename T,typename TRAITS,typename ALLOC>
std::basic_string<T,TRAITS,ALLOC> Buffer<T,TRAITS,ALLOC>::readString(){
	int s =0;
	read((void*)&s,sizeof(int));
	std::basic_string<T, TRAITS, ALLOC> sz;
	if(s>0){
		T * cc = new T[s];
		read((void*)cc,s*sizeof(T));
		sz.assign(cc,s);
		delete [] cc;
	}
	return sz;
}
template<typename T,typename TRAITS,typename ALLOC>
void Buffer<T,TRAITS,ALLOC>::read(std::basic_string<T,TRAITS,ALLOC> & sz){
	int s =0;
	read((void*)&s,sizeof(int));
	if(s>0){
		T * cc = new T[s];
		read((void*)cc,s*sizeof(T));
		sz.assign(cc,s);
		delete [] cc;
	}
}
template<typename T,typename TRAITS,typename ALLOC>
void Buffer<T,TRAITS,ALLOC>::resize(size_t newsize){
	if(_capacity==0 && _data==0)
	{
		_data =(void*) new T[newsize];
		_capacity=newsize;
		_size=newsize;
		return;
	}
	if(newsize>_capacity){
		T * p = new T[newsize];
		memcpy(p,_data,_capacity);
		delete [] ((T*)_data);
		_data = p;
		_capacity=newsize;
		_size=newsize;
	}
	else{
		if(newsize>=_size){
			_size = newsize;
		}
		else{
			_size=newsize;
			_curpos = min(_curpos,_size);
		}
	}
}

/** 将指定的文件内容读入Buffer中
	* @param [in] pathName 文件全名
	* @return
	*/
template<typename T,typename TRAITS,typename ALLOC>
bool Buffer<T,TRAITS,ALLOC>::loadFile(const std::basic_string<T,TRAITS,ALLOC> & pathName)
{

	try{
		FILE * fp = fopen(pathName.c_str(), "rb");
		fseek(fp,0,2);
		size_t s= ftell(fp);		
		resize(s);
		fseek(fp,0,0);
		fread(_data,1,s,fp);
		fclose(fp);
		return true;
	}
	catch(...)
	{
		return false;
	}
}

end_gtl_namespace

