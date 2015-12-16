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
#include <limits>
#include "movingshape.h"
begin_gtl_namespace
///////////////////////////////////////////////////////////////////////////////////
template<
		typename DataType=double,
		int DIMS=4,
		typename ELEMENT=char,
		typename TRAITS=std::char_traits<ELEMENT>,
		typename ALLOC=std::allocator<ELEMENT> 
> 
class MovingPoint: public MovingShape<DataType,DIMS,ELEMENT,TRAITS,ALLOC>,
	public MovingVertex<DataType,DIMS,ELEMENT,TRAITS,ALLOC>
{	
public:
	typedef MovingShape<DataType,DIMS,ELEMENT,TRAITS,ALLOC> base_class_type; 
	typedef typename base_class_type::time_type             time_type;
	typedef typename base_class_type::position_type         position_type;
	typedef typename base_class_type::interval_type         interval_type;
	typedef typename base_class_type::envelope_type         envelope_type;
	typedef typename base_class_type::vertex_type           vertex_type;
	typedef typename base_class_type::spatiotemporalvertex_type   spatiotemporalvertex_type;
	typedef typename base_class_type::movingvertex_type     movingvertex_type;
	typedef typename base_class_type::movingenvelope_type   movingenvelope_type;
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "MovingPoint";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object<ELEMENT,TRAITS,ALLOC> * clone() {
		return 0;
	}
public:	
	MovingPoint(){
		_shapeType = ST_POINT;
	}
	MovingPoint(const movingvertex_type &p ){
		movingvertex_type::copy(p);
		_shapeType = ST_POINT;
	}
	MovingPoint(const MovingPoint  &p ){
		movingvertex_type::copy((const movingvertex_type &)p);
		_shapeType = ST_POINT;
	}
	MovingPoint operator=(const MovingPoint &p ){
		movingvertex_type::copy((const movingvertex_type &)p);
		_identifier = p._identifier;
		_shapeType = ST_POINT;
		return *this;
	}
	virtual movingenvelope_type getMovingEnvelope() {
		movingenvelope_type me;
		return me;
	}
	virtual envelope_type getEnvelope() {
		return movingvertex_type::getEnvelope();
	}
	virtual vertex_type getCenter(){
		return movingvertex_type::getCenter();
	}
};

end_gtl_namespace
