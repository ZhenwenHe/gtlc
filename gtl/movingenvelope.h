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
#include "temporalenvelope.h"
#include "movingvertex.h" 
begin_gtl_namespace
template<
		typename DataType=double,
		int DIMS=4,
		typename CHARTYPE=char,
		typename TRAITS=std::char_traits<CHARTYPE>,
		typename ALLOC=std::allocator<CHARTYPE> 
> 
class MovingEnvelope:  public TemporalEnvelope<DataType,DIMS,CHARTYPE,TRAITS,ALLOC>{
public:
	typedef TemporalEnvelope<DataType,DIMS,CHARTYPE,TRAITS,ALLOC>    spatiotemporalenvelope_type;
	typedef typename spatiotemporalenvelope_type::time_type                       time_type; ;
	typedef typename spatiotemporalenvelope_type::position_type                   position_type;
	typedef typename spatiotemporalenvelope_type::interval_type                   interval_type;	
	typedef typename spatiotemporalenvelope_type::vertex_type                     vertex_type;
	typedef typename spatiotemporalenvelope_type::spatiotemporalvertex_type             spatiotemporalvertex_type;
	typedef typename MovingVertex<DataType,DIMS,CHARTYPE,TRAITS,ALLOC>        movingvertex_type;      ;
	typedef typename spatiotemporalenvelope_type::spatioenvelope_type             spatioenvelope_type;
public:
	virtual std::basic_string<CHARTYPE,TRAITS,ALLOC> getClassName(){
		return "MovingEnvelope";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object<CHARTYPE,TRAITS,ALLOC> * clone() {
		return 0;
	}
protected:
	DataType _low_velocities[DIMS-1];
	DataType _high_velocities[DIMS-1];
public:
	inline void getVelocities(DataType vlow[] ,DataType vhigh[]){
		memcpy(vlow,_low_velocities,sizeof(DataType)*(DIMS-1));
		memcpy(vhigh,_high_velocities,sizeof(DataType)*(DIMS-1));
	}
	inline const DataType * getLowVelocites() const {return _low_velocities;}
	inline const  DataType * getHighVelocites() const  {return _high_velocities;}

	inline void setVelocities(const DataType vlow[] ,const DataType vhigh[]){
		memcpy(_low_velocities,vlow,sizeof(DataType)*(DIMS-1));
		memcpy(_high_velocities,vhigh,sizeof(DataType)*(DIMS-1));
	}
	inline bool isValid() const {
		for (std::size_t axis = 0; axis < DIMS-1; axis++)
			if(_high_velocities[axis] < _low_velocities[axis])
				return false;
		return spatiotemporalenvelope_type::isValid();
	}
public:
	MovingEnvelope()
	{
		memset(_low_velocities,0,sizeof(DataType)*(DIMS-1));
		memset(_high_velocities,0,sizeof(DataType)*(DIMS-1));
	}

	
	MovingEnvelope(time_type tStart, time_type tEnd,DataType minv[DIMS-1], DataType maxv[DIMS-1],DataType vlow[DIMS-1], DataType vhigh[DIMS-1])
		: spatiotemporalenvelope_type(tStart,tEnd,minv,maxv) 
	{
		memcpy(_low_velocities,vlow,sizeof(DataType)*(DIMS-1));
		memcpy(_high_velocities,vhigh,sizeof(DataType)*(DIMS-1));
	}

	MovingEnvelope(const MovingEnvelope & p)
		: spatiotemporalenvelope_type((const spatiotemporalenvelope_type &)p )
	{
		memcpy(_low_velocities,p._low_velocities,sizeof(DataType)*(DIMS-1));
		memcpy(_high_velocities,p._high_velocities,sizeof(DataType)*(DIMS-1));
	}

	MovingEnvelope(const movingvertex_type& low,const movingvertex_type& high)
	{
		_start =min(low.start(),high.end());
		_end = max(low.start(),high.end());
		if(_start<=_end)
			_end+=TOLERANCE;

		const DataType * dmin = low.getPosition();
		const DataType * dmax = high.getPosition();
		int i;
		for( i=0;i<DIMS-1;i++){
			minData[i] = min(dmin[i],dmax[i]); 
			maxData[i] = max(dmin[i],dmax[i]); 

			if(minData[i]==maxData[i]){
				maxData[i]+=TOLERANCE;
				minData[i]-=TOLERANCE;
			}
		}
		
		dmin = low.getVelocity();
		dmax = high.getVelocity();
		for( i=0;i<DIMS-1;i++){
			_low_velocities[i] = min(dmin[i],dmax[i]); 
			_high_velocities[i] = max(dmin[i],dmax[i]); 
		} 
	}

	virtual ~MovingEnvelope()
	{
	}

	MovingEnvelope& operator=(const MovingEnvelope & p)
	{

		memcpy(minData, p.minData, (DIMS-1) * sizeof(DataType));
		memcpy(maxData, p.maxData, (DIMS-1) * sizeof(DataType));
		_start = p._start;
		_end = p._end;
		memcpy(_low_velocities,p._low_velocities,sizeof(DataType)*(DIMS-1));
		memcpy(_high_velocities,p._high_velocities,sizeof(DataType)*(DIMS-1));
		return *this;
	}

};


end_gtl_namespace
