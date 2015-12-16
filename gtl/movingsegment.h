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
#include "movingpoint.h"
begin_gtl_namespace
template<
		typename DataType=double,
		int DIMS=4,
		typename ELEMENT=char,
		typename TRAITS=std::char_traits<ELEMENT>,
		typename ALLOC=std::allocator<ELEMENT> 
> 
class MovingSegment : public MovingShape<DataType,DIMS,ELEMENT,TRAITS,ALLOC> {
public:
	typedef MovingShape<DataType,DIMS,ELEMENT,TRAITS,ALLOC>   base_class_type; 
	typedef typename base_class_type::time_type               time_type;
	typedef typename base_class_type::position_type           position_type;
	typedef typename base_class_type::interval_type           interval_type;
	typedef typename base_class_type::envelope_type           envelope_type;
	typedef typename base_class_type::vertex_type             vertex_type;
	typedef typename base_class_type::spatiotemporalvertex_type     spatiotemporalvertex_type;
	typedef typename base_class_type::movingvertex_type       movingvertex_type;
	typedef typename base_class_type::movingenvelope_type     movingenvelope_type;
 	typedef  MovingPoint<DataType,DIMS,ELEMENT,TRAITS,ALLOC>  movingpoint_type;
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "MovingSegment";
	}

	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}

	virtual void readBuffer(const void *  buffer, size_t count=0){

	}

	virtual Object * clone() {
		return 0;
	}

protected:
	movingvertex_type _startpoint;
	movingvertex_type _endpoint;
public:	
	inline movingvertex_type & getStartVertex()  { return _startpoint;}
	inline movingvertex_type & getEndVertex()  { return _endpoint;}
public:
	MovingSegment(){
		_shapeType = ST_SEGMENT;
	}

	MovingSegment(const movingvertex_type &s,const movingvertex_type &e ){
		_shapeType = ST_SEGMENT;
		_startpoint = s;
		_endpoint = e;
	}

	MovingSegment(const uint64_t   id, const movingvertex_type &s,const movingvertex_type &e ){
		_shapeType = ST_SEGMENT;
		_identifier = id;
		_startpoint = s;
		_endpoint = e;
	}

	inline void set(const uint64_t id, const movingvertex_type &s,const movingvertex_type &e ){
		_shapeType = ST_SEGMENT;
		_identifier = id;
		_startpoint = s;
		_endpoint = e;
	}

	MovingSegment(const MovingSegment& ms){
		_identifier = ms._identifier;
		_startpoint = ms._startpoint;
		_endpoint = ms._endpoint;
		_shapeType = ST_SEGMENT;
	}

	MovingSegment operator = (const MovingSegment& ms){
		_identifier = ms._identifier;
		_startpoint = ms._startpoint;
		_endpoint = ms._endpoint;
		_shapeType = ST_SEGMENT;
		return *this;
	}
public:
	virtual vertex_type getCenter()  {
		vertex_type v;
		getEnvelope().getCenter(v);
		return v;
	}

	virtual envelope_type getEnvelope() {
		envelope_type e;
		DataType * minv = e.getLowerBounds();
		DataType * maxv = e.getUpperBounds();
		minv[0]=_startpoint.start();
		maxv[0]=_endpoint.end();
		for(int i=1;i<DIMS;i++)	{
			minv[i]= min(_startpoint.getPosition(i-1),_endpoint.getPosition(i-1));
			maxv[i]= max(_startpoint.getPosition(i-1),_endpoint.getPosition(i-1));
		}
		return e;
	}

	virtual movingenvelope_type  getMovingEnvelope( ) {
		return movingenvelope_type(_startpoint,_endpoint);
	}
};
end_gtl_namespace
