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
#include "movingenvelope.h"
begin_gtl_namespace 

template<
		typename DataType=double,
		int DIMS=4,
		typename ELEMENT=char,
		typename TRAITS=std::char_traits<ELEMENT>,
		typename ALLOC=std::allocator<ELEMENT> 
> 
class MovingShape: public Object<ELEMENT,TRAITS,ALLOC> 
{
public:
	typedef DataType                                           time_type;
	typedef Vertex<DataType,DIMS-1,ELEMENT,TRAITS,ALLOC>       position_type;
	typedef Interval<time_type,ELEMENT,TRAITS,ALLOC>           interval_type;
	typedef Envelope<DataType,DIMS,ELEMENT,TRAITS,ALLOC>       envelope_type;
	typedef Vertex<DataType,DIMS,ELEMENT,TRAITS,ALLOC>         vertex_type;
	typedef TemporalVertex<DataType,DIMS,ELEMENT,TRAITS,ALLOC> spatiotemporalvertex_type;
	typedef MovingVertex<DataType,DIMS,ELEMENT,TRAITS,ALLOC>   movingvertex_type;
	typedef MovingEnvelope<DataType,DIMS,ELEMENT,TRAITS,ALLOC> movingenvelope_type;
	typedef Envelope<DataType,DIMS-1,ELEMENT,TRAITS,ALLOC>     spatioenvelope_type;
public:
	enum 
	{
		ST_UNKNOWN,
		ST_POINT,
		ST_SEGMENT,
		ST_POINTS,
		ST_SEGMENTS,
		ST_TRAJECTORY,
		ST_TRAJECTORIES
	};
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "MovingShape";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
protected: 
	int       _shapeType;
	uint64_t  _identifier;

	MovingShape(){
		_identifier = Identifier64::generate();
		_shapeType=ST_UNKNOWN;
	}
public:
	virtual ~MovingShape(){
	}
public:
	inline int getType() const {return _shapeType;}
	inline int getType()  {return _shapeType;}
	inline const uint64_t getIdentifier() const {return _identifier;}
	inline void setIdentifier(const uint64_t & id)   { _identifier=id;}
	inline uint64_t  getIdentifier() {return _identifier;}
public:
	virtual movingenvelope_type getMovingEnvelope() =0;
	virtual envelope_type getEnvelope() =0;
	virtual vertex_type getCenter()=0;
};


end_gtl_namespace
