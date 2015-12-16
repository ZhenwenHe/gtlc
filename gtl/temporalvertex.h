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
#include "identifier.h"
#include "vertex.h" 
#include "interval.h"
#include "envelope.h" 
begin_gtl_namespace

template<
		typename DataType=double,
		int DIMS=4,
		typename ELEMENT=char,
		typename TRAITS=std::char_traits<ELEMENT>,
		typename ALLOC=std::allocator<ELEMENT> > 
class TemporalVertex: 
	public Vertex<DataType,DIMS-1,ELEMENT,TRAITS,ALLOC>, 
	public Interval<DataType,ELEMENT,TRAITS,ALLOC> {
public:
	typedef DataType                                      time_type;
	typedef Vertex<DataType,DIMS-1,ELEMENT,TRAITS,ALLOC>  position_type;
	typedef Interval<DataType,ELEMENT,TRAITS,ALLOC>       interval_type; 
	typedef Vertex<DataType,DIMS,ELEMENT,TRAITS,ALLOC>    vertex_type;
	typedef Envelope<DataType,DIMS,ELEMENT,TRAITS,ALLOC>  envelope_type;
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "TemporalVertex";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object<ELEMENT,TRAITS,ALLOC> * clone() {
		return 0;
	}
public:
	TemporalVertex()
		:position_type(),
		interval_type(-DBL_MAX,DBL_MAX)
	{
	}

	TemporalVertex(const DataType * pCoords, const Interval<time_type>& ti)
		: position_type(pCoords),
		interval_type(ti)
	{
	}
	TemporalVertex(const DataType* pCoords, time_type tStart, time_type tEnd)
		:  position_type(pCoords),
		interval_type(tStart,tEnd)
	{

	}

	TemporalVertex(const position_type& p, const Interval<time_type>& ti)
		:position_type(p),
		interval_type(ti)
	{
	}

	TemporalVertex(const position_type & p,  time_type tStart, time_type tEnd)
		:position_type(), 
		interval_type(tStart,tEnd)
	{
	}

	TemporalVertex(const TemporalVertex& p)
		: position_type(p._data),
		interval_type(p._start,p._end,p._type)
	{
	}

	virtual ~TemporalVertex()
	{
	}

	TemporalVertex& operator=(const TemporalVertex& p)
	{		
		memcpy(_data, p._data, (DIMS-1) * sizeof(DataType));
		_start = p._start;
		_end = p._end;		
		return *this;
	}

	bool operator==(const TemporalVertex& p) const
	{
		if (
			_start < p._start - std::numeric_limits<DataType>::epsilon() ||
			_start > p._start + std::numeric_limits<DataType>::epsilon() ||
			_end < p._end - std::numeric_limits<DataType>::epsilon() ||
			_end > p._end + std::numeric_limits<DataType>::epsilon())
			return false;

		for (uint32_t cDim = 0; cDim < (DIMS-1); ++cDim)
		{
			if (
				_data[cDim] < p._data[cDim] - std::numeric_limits<DataType>::epsilon() ||
				_data[cDim] > p._data[cDim] + std::numeric_limits<DataType>::epsilon()) 
				return false;
		}

		return true;
	}

	inline void setPosition(int i, const DataType& v ){
		position_type::set(i,v);
	}
	inline void setPosition(const position_type& v ){
		position_type::set(v.get());
	}
	inline DataType getPosition(int i){
		return *(position_type::get()+i);
	}
	inline const DataType *getPosition() const {
		return  (const DataType *)(position_type::get());
	}
	inline DataType *getPosition() {
		return  ( DataType *)(position_type::get());
	}
	inline vertex_type getCenter()  {
		vertex_type c;
		DataType * p = c.get();
		p[0]=(_start+_end)/2;
		memcpy((void*)(p+1),_data,sizeof(DataType)*(DIMS-1));		
		return c;
	}

	inline envelope_type getEnvelope() {
		envelope_type v;
		DataType * d = v.getLowerBounds();
		d[0]=_start;
		memcpy((void*)(d+1),_data,sizeof(DataType)*(DIMS-1));
		for(int i=1;i<DIMS;i++)
		{
			d[i] -=std::numeric_limits<DataType>::epsilon();
		}
		d = v.getUpperBounds();
		d[0]=_end;
		memcpy((void*)(d+1),_data,sizeof(DataType)*(DIMS-1));
		for(int i=1;i<DIMS;i++)
		{
			d[i] +=std::numeric_limits<DataType>::epsilon();
		}
		return v;
	}

};

end_gtl_namespace
