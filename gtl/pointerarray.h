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

#pragma  once
#include <string>
#include <vector>
#include <iostream>
#include "object.h"
begin_gtl_namespace 
/////////////////////////////////////////////////////////
template<typename OBJECTTYPE, 
	bool AUTODESTORY=true,
	typename ELEMENT=char,
	typename TRAITS=std::char_traits<ELEMENT>,
	typename ALLOC=std::allocator<ELEMENT>  >
class PointerArray:public Object<ELEMENT,TRAITS,ALLOC>{
public:		
	typedef OBJECTTYPE *   ObjectPtr;
	typedef ObjectPtr      object_ptr;
	typedef ObjectPtr * iterator;
private:
	ObjectPtr                          *_vertices;
	unsigned int                        _verticesNumber;
	unsigned int                        _capacity;
	unsigned int                        _growth;
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "PointerArray";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
public:
	PointerArray(unsigned int maxsize=32, unsigned int growth=1){ 
		_capacity = maxsize;
		_vertices=new ObjectPtr[_capacity];
		_verticesNumber=0;
		_growth=growth;
	}
	~PointerArray(){
		clear();
	}
	PointerArray(const PointerArray & ia){
		_capacity = ia._capacity;
		_vertices=new ObjectPtr[_capacity];
		_verticesNumber=ia._verticesNumber;
		for(int i=0;i<_capacity;i++)
			_vertices[i]=ia._vertices[i];
		_growth=ia._growth;
	}
	inline iterator begin() { return (iterator) _vertices;}
	inline iterator end() { return (iterator) _vertices + _verticesNumber;}
	inline size_t size() {return _verticesNumber;}
	inline void push_back(ObjectPtr p) {
		if(_verticesNumber<_capacity){
			_vertices[_verticesNumber]=p;
			_verticesNumber++;
		}
		else{
			_capacity+=_growth;
			ObjectPtr * pp = new ObjectPtr[_capacity];
			for(unsigned int i=0;i<_verticesNumber;i++)
				pp[i]=_vertices[i];
			delete [] _vertices;
			_vertices=pp;
			_vertices[_verticesNumber]=p;
			_verticesNumber++;
		}
	}
	inline unsigned int capacity() {return _capacity;}
	inline ObjectPtr  operator[] (unsigned int i){
		return _vertices[i];
	}
	inline ObjectPtr get(unsigned int i){
		return _vertices[i];
	}
	inline void set(unsigned int i,ObjectPtr p){
		_vertices[i]=p;
	}
	inline void clear() {
		
		if(_vertices )
		{
			if(AUTODESTORY){
				for(int i=0;i<_verticesNumber;i++)
					delete _vertices[i];
			}

			delete [] _vertices;
			_vertices=0;
		}
		_capacity=0;
		_verticesNumber=0;
	}
	inline void removeAll(){
		_verticesNumber=0;
		if(_vertices)
		{
			delete [] _vertices;
			_vertices=0;
		}
		_capacity=0;
	}
	inline void resize(unsigned int s){ 
		if(_capacity<s){
			_capacity =s;
			ObjectPtr * pp = new ObjectPtr[_capacity];
			for(unsigned int i=0;i<_verticesNumber;i++)
				pp[i]=_vertices[i];
			delete [] _vertices;
			_vertices=pp;
		}
		_verticesNumber=s;
	}
};

end_gtl_namespace