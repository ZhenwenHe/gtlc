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
#include "temporalvertex.h"
begin_gtl_namespace
 
template<
		typename DataType=double,
		int DIMS=4,
		typename ELEMENT=char,
		typename TRAITS=std::char_traits<ELEMENT>,
		typename ALLOC=std::allocator<ELEMENT> > 
class TemporalEnvelope:
	public Envelope<DataType,DIMS-1,ELEMENT,TRAITS,ALLOC>, 
	public Interval<DataType,ELEMENT,TRAITS,ALLOC> {
public:
	typedef DataType                                                time_type;
	typedef Vertex<DataType,DIMS-1,ELEMENT,TRAITS,ALLOC>            position_type;
	typedef Interval<time_type,ELEMENT,TRAITS,ALLOC>                interval_type;
	typedef Envelope<DataType,DIMS-1,ELEMENT,TRAITS,ALLOC>          spatioenvelope_type;
	typedef Envelope<DataType,DIMS,ELEMENT,TRAITS,ALLOC>            envelope_type;
	typedef Vertex<DataType,DIMS,ELEMENT,TRAITS,ALLOC>              vertex_type;
	typedef TemporalVertex<DataType,DIMS,ELEMENT,TRAITS,ALLOC>      spatiotemporalvertex_type;
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "TemporalEnvelope";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object<ELEMENT,TRAITS,ALLOC> * clone() {
		return 0;
	}
public:
	TemporalEnvelope():interval_type(-DBL_MAX,DBL_MAX)
	{
	}

	TemporalEnvelope(const interval_type& ti,const spatioenvelope_type & e)
		: spatioenvelope_type(e),interval_type(ti)
	{
	}
	TemporalEnvelope(time_type tStart, time_type tEnd,DataType minv[DIMS-1], DataType maxv[DIMS-1])
		: spatioenvelope_type(minv,maxv),interval_type(tStart,tEnd)
	{
	}

	TemporalEnvelope(const TemporalEnvelope & p)
		: spatioenvelope_type(p.minData,p.maxData),interval_type(p._start,p._end,p._type)
	{
	}

	virtual ~TemporalEnvelope()
	{
	}

	inline const DataType * getLowPosition() const { return minData;}

	inline const DataType * getHighPosition() const { return maxData;}

	inline void set(const envelope_type & e) {
		const DataType * minv = e.getLowerBounds();
		const DataType * maxv = e.getUpperBounds();
		memcpy(minData, minv+1, (DIMS-1) * sizeof(DataType));
		memcpy(maxData, maxv+1, (DIMS-1) * sizeof(DataType));
		_start = *minv;
		_end = *maxv;
	}


	TemporalEnvelope& operator=(const TemporalEnvelope& p)
	{

		memcpy(minData, p.minData, (DIMS-1) * sizeof(DataType));
		memcpy(maxData, p.maxData, (DIMS-1) * sizeof(DataType));
		_start = p._start;
		_end = p._end;

		return *this;
	}

	inline vertex_type getCenter()  {
		vertex_type v;
		getEnvelope().getCenter(v);
		return v;
	}

	inline envelope_type getEnvelope() {
		envelope_type e = _startpoint.getEnvelope();
		e.extend(_endpoint.getEnvelope());
		return e;
	}

	inline bool isValid() const {
		if (_start<= _end && spatioenvelope_type::isValid())
				return true;
		return false;
	}
};
 

end_gtl_namespace
