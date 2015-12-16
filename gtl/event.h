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

enum EVENTTYPE{
	ATTRIBUTE_CREATE,	
	ATTRIBUTE_CHANGE,
	ATTRIBUTE_DESTORY,
	GEOMTRY_CREATE,
	GEOMTRY_TRANSLATE,
	GEOMTRY_ROTATE,
	GEOMTRY_SCALE,
	GEOMTRY_DEFORMATE,
	GEOMTRY_DESTORY,
	RELATIONSHIP_CREATE,
	RELATIONSHIP_CHANGE,
	RELATIONSHIP_DESTORY
};

begin_gtl_namespace
template<
	typename ELEMENT=char,
	typename TRAITS=std::char_traits<ELEMENT>,
	typename ALLOC=std::allocator<ELEMENT> >
class Event :  public Object<ELEMENT,TRAITS,ALLOC>{ 
protected:
	int _type;// EVENTTYPE
	Object<ELEMENT,TRAITS,ALLOC> * _emitter;
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "Event";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
public:
	inline int getType() const {return _type;}
	inline int getType()  {return _type;}
	Object<ELEMENT,TRAITS,ALLOC>* getEmitter() {return _emitter;}
};
end_gtl_namespace
 