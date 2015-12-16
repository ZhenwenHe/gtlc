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
#include "temporalvertex.h"
begin_gtl_namespace
 
template<
		typename DataType=double,
		int DIMS=4,
		typename ELEMENT=char,
		typename TRAITS=std::char_traits<ELEMENT>,
		typename ALLOC=std::allocator<ELEMENT> 
> 
class MovingVertex: public TemporalVertex<DataType,DIMS,ELEMENT,TRAITS,ALLOC> 
{
public:
	typedef DataType                                        time_type;
	typedef Vertex<DataType,DIMS-1,ELEMENT,TRAITS,ALLOC>    position_type;
	typedef Interval<time_type,ELEMENT,TRAITS,ALLOC>        interval_type;
	typedef Envelope<DataType,DIMS,ELEMENT,TRAITS,ALLOC>    envelope_type;
	typedef Vertex<DataType,DIMS,ELEMENT,TRAITS,ALLOC>      vertex_type;
	typedef TemporalVertex<DataType,DIMS,ELEMENT,TRAITS,ALLOC> spatiotemporalvertex_type;
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "MovingVertex";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object<ELEMENT,TRAITS,ALLOC> * clone() {
		return 0;
	}
protected:
	DataType  _velocity[DIMS-1];
public:
	MovingVertex() 
	{
		 memset(_velocity,0,sizeof(DataType)*(DIMS-1));
	}

	MovingVertex(const DataType * pCoords, const interval_type& ti, const DataType * pVelocity)
		: TemporalVertex(pCoords,ti)
	{
		if(pVelocity)
			memcpy((void*)_velocity,pVelocity,sizeof(DataType)*(DIMS-1));
		else
			memset(_velocity,0,sizeof(DataType)*(DIMS-1));
		
	}

	MovingVertex(const DataType* pCoords, time_type tStart, time_type tEnd,const DataType * pVelocity)
		: spatiotemporalvertex_type(pCoords,tStart,tEnd)
	{
		if(pVelocity)
			memcpy((void*)_velocity,pVelocity,sizeof(DataType)*(DIMS-1));
		else
			memset(_velocity,0,sizeof(DataType)*(DIMS-1));
	}

	MovingVertex(const Vertex<DataType,DIMS-1>& p, const interval_type& ti,const Vertex<DataType,DIMS-1>& velocity)
		: spatiotemporalvertex_type(p,ti)
	{
		memcpy((void*)_velocity,velocity.get(),sizeof(DataType)*(DIMS-1));
	}

	MovingVertex(const Vertex<DataType,DIMS-1> & p,  time_type tStart, time_type tEnd,const Vertex<DataType,DIMS-1>& velocity)
		: spatiotemporalvertex_type(p,tStart,tEnd)
	{
		memcpy((void*)_velocity,velocity.get(),sizeof(DataType)*(DIMS-1));
	}

	MovingVertex(const spatiotemporalvertex_type & p, const Vertex<DataType,DIMS-1>& velocity)
		: spatiotemporalvertex_type(p)
	{
		memcpy((void*)_velocity,velocity.get(),sizeof(DataType)*(DIMS-1));
	}

	MovingVertex(const spatiotemporalvertex_type & p, const DataType* velocity)
		: spatiotemporalvertex_type(p)
	{
		if(velocity) 
			memcpy((void*)_velocity,velocity,sizeof(DataType)*(DIMS-1));
		else
			memset(_velocity,0,sizeof(DataType)*(DIMS-1));
	}

	MovingVertex(const MovingVertex& p)
	{
		memcpy(_data, p._data, (DIMS-1) * sizeof(DataType));
		_start = p._start;
		_end = p._end;	
		memcpy((void*)_velocity,p._velocity,sizeof(DataType)*(DIMS-1));
	}
	void copy(const MovingVertex& p)
	{
		memcpy(_data, p._data, (DIMS-1) * sizeof(DataType));
		_start = p._start;
		_end = p._end;	
		memcpy((void*)_velocity,p._velocity,sizeof(DataType)*(DIMS-1));
	}
	virtual ~MovingVertex()
	{
	}

	MovingVertex& operator=(const MovingVertex& p)
	{		
		memcpy(_data, p._data, (DIMS-1) * sizeof(DataType));
		_start = p._start;
		_end = p._end;	
		memcpy((void*)_velocity,p._velocity,sizeof(DataType)*(DIMS-1));
		return *this;
	}

	inline const DataType * getVelocity()const {return _velocity;}

	inline DataType * getVelocity() {return (DataType *)_velocity;}

	inline const DataType  getVelocity(int i) const {return _velocity[i];}

	inline void setVelocity(int i,const DataType & v) {_velocity[i]=v;}

	inline void setVelocity( const DataType * v) { 
		memcpy(_velocity,v,sizeof(DataType)*(DIMS-1));
	}
 
	position_type getProjectedPosition(time_type t) const
	{
		position_type pos;
		DataType * p = pos.get();
		for(int i=0;i<DIMS-1;i++){
			p[i]= _data[i] + _velocity[i] * (t - _start);
		}		
		return pos;
	}
	 
	void getProjectedPosition(time_type t,position_type & pos) 
	{
		DataType * p = pos.get();
		for(int i=0;i<DIMS-1;i++){
			p[i]= _data[i] + _velocity[i] * (t - _start);
		}		
	}

};


end_gtl_namespace
