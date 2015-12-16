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
#include <string>
#include <vector>
#include <iostream>
#include "object.h"

begin_gtl_namespace
//用于表达任意位数的空间节点
template<typename T,int NUMDIMS,
	typename ELEMENT=char,
	typename TRAITS=std::char_traits<ELEMENT>,
	typename ALLOC=std::allocator<ELEMENT>  >
class Vertex:public Object<ELEMENT,TRAITS,ALLOC>{
protected:
	T  _data[NUMDIMS];  
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "Vertex";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
    virtual Object<ELEMENT,TRAITS,ALLOC> * clone() {
		return 0;
	}
public:
	typedef T value_type;
	inline const T * get() const {return _data;}
	inline T * get() {return (T*)_data;}
	inline const T get(int i)const {return _data[i];} 
	inline void set(int i, const T & t) {_data[i]=t;}
	inline void set(const T * data){
		memcpy(_data,data,sizeof(T)*NUMDIMS);
	}
	void copy(const Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC> & p){
		for(int i=0;i<NUMDIMS;i++)
			_data[i]=p._data[i];
	}

	Vertex( ){ 
	}
	Vertex(const T * data ){
		memcpy(_data,data,sizeof(T)*NUMDIMS);
	}
	Vertex(const Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC> & p){
		copy(p);
	}
	
	virtual ~Vertex(){
	}
	Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC> operator=(const  Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC> & p){
		copy(p);
		return *this;
	}
	Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC> operator+=(const  Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC> & p){
		for(int i=0;i<NUMDIMS;i++)
			_data[i]+=p._data[i];
		return *this;
	}
	Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC> operator-=(const  Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC> & p){
		for(int i=0;i<NUMDIMS;i++)
			_data[i]-=p._data[i];
		return *this;
	}

	T &  operator[]( size_t  i){
		return _data[i];
	}
	const T &  operator[]( const size_t  i) const {
		return _data[i];
	}
	void zero(){
		for(int i=0;i<NUMDIMS;i++)
			_data[i]=0;
	}
	inline void getEnvelope(T minv[NUMDIMS],T maxv[NUMDIMS])
	{
		for(int i=0;i<NUMDIMS;i++)
		{
			maxv[i]=_data[i]+TOLERANCE;
			minv[i]=_data[i]-TOLERANCE;
		}
	}
public:
	static double calculateDistance(const Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC> & p1,const Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC> & p2 ){
		return sqrt(calculateDistance2(p1,p2));
	}
	static double calculateDistance2(const Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC> & p1,const Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC> & p2){		
		double s = 0;
		for(size_t i =0;i<NUMDIMS;i++){
			s+=(p1._data[i]-p2._data[i])*(p1._data[i]-p2._data[i]);
		}
		return s;
	}

};

template<typename T,int NUMDIMS,typename ELEMENT,typename TRAITS,typename ALLOC > 
static bool operator==(const Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>  & t1,const Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC> & t2){	
	for(int i=0;i<NUMDIMS;i++){
		if(t1[i]!=t2[i])
			return false;
	}
	return true;
}

template<typename T,int NUMDIMS,typename ELEMENT,typename TRAITS,typename ALLOC > 
static  Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC> operator*(const  Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>   & t11,double a ){
	Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>  & t1=(Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>  &) t11;
	Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC> t;
	for(int i=0;i<t1.size();i++){
		t[i]=t1[i]*a;
	}
	return t;
}

template<typename T,int NUMDIMS,typename ELEMENT,typename TRAITS,typename ALLOC > 
static T  operator*(const  Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>   & t1,const  Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>  & t2){

	T t=0;
	for(int i=0;i<NUMDIMS;i++){
		t+=(t1[i]*t2[i]);
	}
	return t;
}

template<typename T,int NUMDIMS,typename ELEMENT,typename TRAITS,typename ALLOC > 
static Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>  operator+(const  Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>   & t11,const  Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>  & t22){
	Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>  & t1=(Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>  &) t11;
	Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>  & t2=(Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>  &) t22;

	Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC> t; 
	for(int i=0;i<NUMDIMS;i++){
		t[i]=t1[i]+t2[i];
	}
	return t;
}

template<typename T,int NUMDIMS,typename ELEMENT,typename TRAITS,typename ALLOC > 
static Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>  operator / (const  Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>   & t11,const T & d){
	Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>  & t1=(Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>  &) t11;
	Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC> t; 
	for(int i=0;i<NUMDIMS;i++){
		t[i]=t1[i]/d;
	}
	return t;
}

template<typename T,int NUMDIMS,typename ELEMENT,typename TRAITS,typename ALLOC > 
static  Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>  operator-(const  Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>   & t11,const  Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>  & t22){
	Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>  & t1=(Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>  &) t11;
	Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>  & t2=(Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>  &) t22;

	Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC> t; 
	for(int i=0;i<NUMDIMS;i++){
		t[i]=t1[i]-t2[i];
	}
	return t;
}


template<typename T,int NUMDIMS,typename ELEMENT,typename TRAITS,typename ALLOC > 
static bool operator!=(const  Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>   & t1,const  Vertex<T,NUMDIMS,ELEMENT,TRAITS,ALLOC>  & t2){
	return  !(t1==t2);
}


typedef  Vertex<double,4,char,std::char_traits<char>,std::allocator<char> >  Vertex4d;
typedef  Vertex<float,4,char,std::char_traits<char>,std::allocator<char> >  Vertex4f;
typedef  Vertex<int,4,char,std::char_traits<char>,std::allocator<char> >  Vertex4i;


 
typedef  Vertex<float,4,char,std::char_traits<char>,std::allocator<char> >  Color4f;
typedef  Vertex<uint8_t,4,char,std::char_traits<char>,std::allocator<char> >  Color4b;

end_gtl_namespace
#include "vertex3.inl"
#include "vertex2.inl"
