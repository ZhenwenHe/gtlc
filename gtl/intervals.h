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
 
#include "interval.h"
#include "intervaloperators.h"

begin_gtl_namespace
template <typename T, 
	int DIMS=4, 
	typename IDENTIFIER = Identifier64,
	typename ELEMENT=char,
	typename TRAITS=std::char_traits<ELEMENT>,
	typename ALLOC=std::allocator<ELEMENT>  >
class Intervals : public Object <ELEMENT,TRAITS,ALLOC> {//间隔对象，可以表示时间段等
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "Intervals";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
private:
	IDENTIFIER         _identifier;
	Interval<T,ELEMENT,TRAITS,ALLOC>        _data[DIMS];
	unsigned long long _offset;//文件中的偏移
	unsigned char      _flags[DIMS];//记录对应的Interval的状态 
public:
	
	inline Interval<T,ELEMENT,TRAITS,ALLOC> & get(int i){
		return _data[i];
	}
	inline void set(int i,const Interval<T,ELEMENT,TRAITS,ALLOC> &iv){
		_data[i]=iv;
		_data[i].parent(this);
	}
	inline void set(int i,T s, T e){
		_data[i].start(s);
		_data[i].end(e);
		_data[i].parent(this);
	}
	inline unsigned long long offset() const {return _offset;}

	inline void offset(unsigned long long v)  { _offset=v;}

	inline unsigned int flags(){ 
		unsigned int i =0; 
		unsigned int s=0;
		for(int j=0;j<DIMS;i++){
			i=_flags[j];
			s+=(i<<(8*(DIMS-1-j)));
		}
		/*i=_flags[0];
		s+=i<<24;
		i = _flags[1];
		s+=i<<16;
		i = _flags[2];
		s+=i<<8;
		s+=_flags[3];*/
		return s;
	}
	inline unsigned char flags(int i){ 
		return _flags[i];
	}
	inline void flags(int i,unsigned char f){ 
		_flags[i]=f;
	}
	inline void get(T minv[DIMS], T maxv[DIMS]){
		for(int i=0;i<DIMS;i++){
			minv[i]=_data[i].start();
			maxv[i]=_data[i].end();
		}
	}

public: 
	Intervals(){
		_identifier = IDENTIFIER::generate();
		for(int i=0;i<DIMS;i++){ 
			_data[i].parent(this);
			_flags[i]=0;
		}
		_offset=0;
	} 
	Intervals(const Intervals & i4d)
	{
		for(int i=0;i<DIMS;i++){
			_data[i]=i4d._data[i];
			_data[i].parent((void*) this);
			_flags[i]=i4d._flags[i];
		}
		_identifier = i4d._identifier;
		_offset=i4d._offset;
	}
	static void random(int n,T mind, T maxd, std::vector<Intervals<T, DIMS, IDENTIFIER,ELEMENT,TRAITS,ALLOC>*> & vs)
	{
		IntervalOperators<T, ELEMENT, TRAITS, ALLOC> op;
		std::vector<Interval<T,ELEMENT,TRAITS,ALLOC> > intvs;
		vs.resize(n);
		for(int j=0;j<n;j++)
			vs[j] = new Intervals<T, DIMS, IDENTIFIER,ELEMENT,TRAITS,ALLOC>();
		for(int i=0;i<4;i++)
		{
			intvs.clear();
			op.random(n,mind,maxd,intvs);
			for(int j=0;j<n;j++)
				vs[j]->set(i,intvs[j]);
		}
	}
	
	Intervals(unsigned long long id, T ts, T te, T boundingBox[6])
	{
		assert(DIMS==4);

		_identifier=id;
		_data[0].start(ts);
		_data[0].end(te);

		_data[1].start(boundingBox[0]);
		_data[2].start(boundingBox[2]);
		_data[3].start(boundingBox[4]);

		_data[1].end(boundingBox[1]);	
		_data[2].end(boundingBox[3]);
		_data[3].end(boundingBox[5]);

		for(int i=0;i<DIMS;i++)
			_data[i].parent((void*)this);
	}
 
	static bool readIndexFile(const std::string & szFileName,std::vector<Intervals<T, DIMS, IDENTIFIER,ELEMENT,TRAITS,ALLOC>*> & vs)
	{
		assert(DIMS==4);
		struct IndexItem
		{
			unsigned long long    _fid; //对象的ID
			unsigned long long _fclsid; //对象
			int    _state; //标示记录记录的状态：0原始数据；1删除标示；2更新标示
			unsigned long _offset; //记录文件指针的起始位置:相对于文件头进行偏移的长度
			unsigned long _bufLength; //记录该块buf的长度
			unsigned long _bufLengthNoPro;//记录feature没有属性信息的长度
			unsigned long long  _writeTime;	//记录文件写入的时间
			unsigned long long  _updateTime;	//记录文件写入的时间
			double _bound[6];//对象的包围盒:供建立索引使用
		};

		std::locale loc = std::locale::global(std::locale(""));//设置环境为系统环境

		std::ifstream iFsteram(szFileName.c_str(),std::ios::in|std::ios::binary);
		if (!iFsteram){
			return false;
		}
		iFsteram.seekg(0,std::ios_base::end); // 移动到文件尾 
		std::size_t len = iFsteram.tellg();//取得当前位置的指针长度 即文件长度
		if (len>0)
		{
			char *pv = new char[len];
			iFsteram.seekg(0,std::ios_base::beg); // 移动到文件头 
			iFsteram.read(pv,len);
			int nUint = sizeof(IndexItem);
			int nSize = len/nUint;
			vs.resize(nSize);
			std::vector<Interval1d> vi;
			Interval1dOperators op1d;
			op1d.random(nSize, 1000, 10000, vi);

			for (int i = 0; i < nSize;i++){

				IndexItem * pii =  (IndexItem*)(pv+nUint*i);			
				vs[i]=new Intervals<T, DIMS, IDENTIFIER,ELEMENT,TRAITS,ALLOC>(pii->_fid,vi[i].start(),vi[i].end(),pii->_bound);
			}

			delete  [] pv;
			pv = NULL;
		}
		iFsteram.close();
		std::locale::global(loc);
		return true;
	}
	 
	static void  calculateEnvelope(const std::vector<Intervals<T, DIMS, IDENTIFIER,ELEMENT,TRAITS,ALLOC>*> & vs, T minv[DIMS], T maxv[DIMS])
	{
		for(int i=0;i<DIMS;i++)
		{
			minv[i]=DBL_MAX;
			maxv[i]= - DBL_MAX;
		}

		for(std::vector<Intervals*>::const_iterator it = vs.begin();it!=vs.end();it++)
		{
			for(int i=0;i<DIMS;i++)
			{
				if(minv[i]> (*it)->get(i).start())
					minv[i] = (*it)->get(i).start();
				if(maxv[i]<(*it)->get(i).end())
					maxv[i]=(*it)->get(i).end();
			}
		}
	}
};


typedef Intervals<double,4,Identifier<64,char,std::char_traits<char>,std::allocator<char>>,char,std::char_traits<char>,std::allocator<char> > Interval4d;
typedef PointerArray<Interval1d,false,char,std::char_traits<char>,std::allocator<char> > IntervalArray;
end_gtl_namespace
#include "intervals.inl"