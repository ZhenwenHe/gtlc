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
#include "console.h"
#include "randomnumbers.h"
#include "envelopes.h"
#include "trajectories.h"
#include "spatiotemporalreader.h"
#include "spatiotemporalwriter.h"

begin_gtl_namespace
template<
		typename DataType=double,
		int DIMS=4,
		typename ELEMENT=char,
		typename TRAITS=std::char_traits<ELEMENT>,
		typename ALLOC=std::allocator<ELEMENT> 
> 
class SpatiotemporalGenerator: public SpatiotemporalWriter<DataType,DIMS,ELEMENT,TRAITS,ALLOC> ,
	public SpatiotemporalReader<DataType,DIMS,ELEMENT,TRAITS,ALLOC>{
public:
	typedef MovingShape<DataType,DIMS,ELEMENT,TRAITS,ALLOC>   movingshape_type; 
	typedef typename movingshape_type::time_type               time_type;
	typedef typename movingshape_type::position_type           position_type;
	typedef typename movingshape_type::interval_type           interval_type;
	typedef typename movingshape_type::envelope_type           envelope_type;
	typedef typename movingshape_type::vertex_type             vertex_type;
	typedef typename movingshape_type::spatiotemporalvertex_type     spatiotemporalvertex_type;
	typedef typename movingshape_type::movingvertex_type       movingvertex_type;
	typedef typename movingshape_type::movingenvelope_type     movingenvelope_type;
	typedef typename movingshape_type::spatioenvelope_type     spatioenvelope_type; 
 	typedef  MovingPoint<DataType,DIMS,ELEMENT,TRAITS,ALLOC>  movingpoint_type;
	typedef  MovingPoints<DataType,DIMS,ELEMENT,TRAITS,ALLOC>  movingpoints_type;
	typedef  MovingSegment<DataType,DIMS,ELEMENT,TRAITS,ALLOC>  movingsegment_type;
	typedef  MovingSegments<DataType,DIMS,ELEMENT,TRAITS,ALLOC>  movingsegments_type;
	typedef typename std::vector< movingvertex_type *>            movingvertexptr_vector_type;
	typedef typename std::vector< movingvertex_type >             movingvertex_vector;
	typedef typename std::vector< movingvertex_type * >::iterator iterator;
	typedef  Trajectory<DataType,DIMS,ELEMENT,TRAITS,ALLOC>       trajectory_type;
	typedef  Trajectories<DataType,DIMS,ELEMENT,TRAITS,ALLOC>       trajectories_type;
 
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "SpatiotemporalGenerator";
	}

	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}

	virtual void readBuffer(const void *  buffer, size_t count=0){

	}

	virtual Object * clone() {
		return 0;
	}
 
protected:
	// the range of velocity in each axis;
	DataType _low_velocity[DIMS-1];
	DataType _high_velocity[DIMS-1];

	spatioenvelope_type     _spatialRanges;// position range in each axis

	std::pair<DataType,DataType>  _temporalRange;// time range 

	DataType                      _frequency;//how ofen will a point be recorded
 public:
	SpatiotemporalGenerator(
		time_type startTime,
		time_type endTime,
		
		DataType low_position[DIMS-1],
		DataType high_position[DIMS-1],

		DataType low_velocity[DIMS-1],
		DataType high_velocity[DIMS-1],

		DataType frequency
		): _spatialRanges(low_position,high_position),_frequency(frequency),_temporalRange(startTime,endTime)
	{
		memcpy(_low_velocity,low_velocity,sizeof(DataType)*(DIMS-1));
		memcpy(_high_velocity,high_velocity,sizeof(DataType)*(DIMS-1)); 
	}

	SpatiotemporalGenerator(
		time_type startTime,
		time_type endTime,

		DataType xlow,
		DataType ylow,
		DataType zlow,

		DataType xhigh,
		DataType yhigh,
		DataType zhigh,
				
		DataType vxlow,
		DataType vylow,
		DataType vzlow,

		DataType vxhigh,
		DataType vyhigh,
		DataType vzhigh,
		
		DataType frequency=60 //60 seconds a point 
		): _frequency(frequency),_temporalRange(startTime,endTime)
	{
		_low_velocity[0]=vxlow;
		_low_velocity[1]=vylow;
		_low_velocity[2]=vzlow;

		_high_velocity[0]=vxhigh;
		_high_velocity[1]=vyhigh;
		_high_velocity[2]=vzhigh;

		DataType * p = _spatialRanges.getLowBounds();
		p[0]=xlow;
		p[1]=ylow;
		p[2]=zlow;
		p = _spatialRanges.getUpperBounds();
		p[0]=xhigh;
		p[1]=yhigh;
		p[2]=zhigh;
	}

	SpatiotemporalGenerator(
		time_type startTime,
		time_type endTime,

		DataType xlow,
		DataType ylow,

		DataType xhigh,
		DataType yhigh,

		DataType vxlow,
		DataType vylow,

		DataType vxhigh,
		DataType vyhigh,

		DataType frequency=60 //60 seconds a point 
		): _frequency(frequency),_temporalRange(startTime,endTime)
	{
		_low_velocity[0]=vxlow;
		_low_velocity[1]=vylow;

		_high_velocity[0]=vxhigh;
		_high_velocity[1]=vyhigh;

		DataType * p = _spatialRanges.getLowBounds();
		p[0]=xlow;
		p[1]=ylow;
		p = _spatialRanges.getUpperBounds();
		p[0]=xhigh;
		p[1]=yhigh;
	}

	unsigned long generateTrajectory(const position_type & startPosition, const time_type & startTime,const size_t & maxpoints, trajectory_type & path){
		Random r;
		movingvertex_type p , p2;
		movingvertex_type::position_type pp = startPosition;
		DataType * pv=0;
		time_type endTime = startTime+_frequency;

		p.setPosition(pp);
		p.start(startTime);
		p.end(endTime);

		DataType * pv = p.getVelocity();
		for(int i=0;i<DIMS-1;i++)
		{
			pv[i]=r.nextDouble(_low_velocity[i],_high_velocity[i]);
		}
						
		do{
			path.push_back(p);
			p.getProjectedPosition(endTime,pp);	
			p2.setPosition(pp);
			pv = p2.getVelocity();
			for(int i=0;i<DIMS-1;i++)
			{
				pv[i]=r.nextDouble(_low_velocity[i],_high_velocity[i]);
			}
			p2.start(endTime);
			endTime  += _frequency;
			p2.end(endTime);
			p = p2; 
		}while(_spatialRanges.test(pp) != 0 && endTime < _temporalRange.second  && path.size()<maxpoints);

		return path.size();
	}

	unsigned long generateTrajectory(const size_t & maxpoints, trajectory_type & path, bool ignoreSpatialRange=true,bool ignoreTemporalRange=true){
		Random<double> r(rand(),rand());
		movingvertex_type p , p2;
		movingvertex_type::position_type pp;
		std::pair<DataType,DataType> range;
		
		for(int i=0;i<DIMS-1;i++){
			range = _spatialRanges.getRange(i);

#ifdef _GTL_DEBUG_
			double randnumber=0;
			randnumber=r.nextDouble(range.first,range.second);
			pp.set(i,randnumber);
			g_console.print("random number = %lf\n",randnumber);
#else
			pp.set(i,r.nextDouble(range.first,range.second));
#endif
		}
		DataType * pv=0;
		const time_type & startTime = r.nextDouble(_temporalRange.first,_temporalRange.second);
		time_type endTime = startTime+_frequency;

		p.setPosition(pp);
		p.start(startTime);
		p.end(startTime);

		pv = p.getVelocity();
		for(int i=0;i<DIMS-1;i++)
		{
			pv[i]=r.nextDouble(_low_velocity[i],_high_velocity[i]);
		}

		bool repeated = false;

		do{
			path.add(p);
			p.getProjectedPosition(endTime,pp);	
			p2.setPosition(pp);
			pv = p2.getVelocity();
			for(int i=0;i<DIMS-1;i++)
			{
				pv[i]=r.nextDouble(_low_velocity[i],_high_velocity[i]);
				if(rand()%2) 
					pv[i]=-pv[i];
			}

			p2.start(endTime);			
			p2.end(endTime);
			endTime  += _frequency;
			p = p2; 

			repeated =  path.size()<maxpoints;

			if(!ignoreSpatialRange)
				repeated = repeated && (_spatialRanges.test(p)!=0);

			if(!ignoreTemporalRange)
				repeated = repeated && (endTime < _temporalRange.second);


		}while(repeated);

		return path.size();
	}
	
	unsigned long generateTrajectories(
		const size_t & maxpoints_of_trajectory, //每个路径的最大点数
		const size_t & maxjtrajectories,//最多生成的路径条数
		trajectories_type & trajectories,
		bool ignoreSpatialRange=true,bool ignoreTemporalRange=true)	{

			size_t objnumber=0;			
			while(trajectories.size()<maxjtrajectories){	
				trajectory_type * trj = new trajectory_type;
				generateTrajectory(maxpoints_of_trajectory,*trj,ignoreSpatialRange,ignoreTemporalRange);
				objnumber++;
				trajectories.add(trj);
			}
			return trajectories.size();
	}
 
};

end_gtl_namespace
