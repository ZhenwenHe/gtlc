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
#include "movingpoints.h"
#include "movingsegments.h"
begin_gtl_namespace
template<
		typename DataType=double,
		int DIMS=4,
		typename ELEMENT=char,
		typename TRAITS=std::char_traits<ELEMENT>,
		typename ALLOC=std::allocator<ELEMENT> 
> 
class Trajectory : public MovingShape<DataType,DIMS,ELEMENT,TRAITS,ALLOC> {
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
	typedef  MovingPoints<DataType,DIMS,ELEMENT,TRAITS,ALLOC>  movingpoints_type;
	typedef  MovingSegment<DataType,DIMS,ELEMENT,TRAITS,ALLOC>  movingsegment_type;
	typedef  MovingSegments<DataType,DIMS,ELEMENT,TRAITS,ALLOC>  movingsegments_type;
	typedef typename std::vector< movingvertex_type *>          movingvertexptr_vector_type;
	typedef typename std::vector< movingvertex_type * >::iterator iterator;
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "Trajectory";
	}

	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}

	virtual void readBuffer(const void *  buffer, size_t count=0){

	}

	virtual Object * clone() {
		return 0;
	}

protected:
	std::basic_string<ELEMENT,TRAITS,ALLOC> _name;
	movingvertexptr_vector_type _points; 	
public:
	Trajectory(){
		_shapeType = ST_TRAJECTORY;
	}
	virtual ~Trajectory(){
		 clear();
	}
	inline void clear() {
		for(iterator it=_points.begin();it!=_points.end();it++)
		{
			delete (*it);
		}
		_points.clear();
		_name="";
	}
	

	inline void add(const movingvertex_type & p){
		_points.push_back( new movingvertex_type(p));
	}
	inline void add(movingvertex_type * p){
		_points.push_back(p);
	}
	inline size_t size() const {return _points.size();}

	inline std::basic_string<ELEMENT,TRAITS,ALLOC> & getName(){ return _name;}

	inline void setName(const std::basic_string<ELEMENT,TRAITS,ALLOC> & sz) { _name =sz;}

	inline movingvertexptr_vector_type & operator[](size_t i) { return * _points[i];}
	inline movingvertex_type * get(size_t i) {return _points[i];}
	inline movingvertexptr_vector_type & getVertices() {return _points;}

	
	inline void split(movingsegments_type & segments){
		size_t n = _points.size()-1;
		for(size_t i=0;i<n;i++)
			segments.add(new movingsegment_type(_identifier,* _points[i],* _points[i+1]));
	}

	inline void split(movingpoints_type & pnts){
		size_t n = _points.size();
		for(size_t i=0;i<n;i++)
			pnts.add(new movingpoint_type(* _points[i]));
	}
 
	inline void recalculateVelocities(){
		movingvertexptr_vector_type::iterator it1 = _points.begin();
		auto it2 = it1+1;
		DataType * v1,* p1, * p2, during;
		int i =0;
		for(; it2!= _points.end();it1++,it2++){
			v1 = (*it1)->getVelocity();
			p1 = (*it1)->getPosition();
			p2  =(*it2)->getPosition();
			during = (*it2)->start()-(*it1)->start();
			(*it1)->end((*it2)->start());
			for(i=0;i<DIMS-1;i++){
				v1[i] = (p2[i]-p1[i])/during;
			}
		}
		// the last vertex, same as the second last vertex or unknown
		//(*it1)->setVelocity(v1);
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

		iterator it = _points.begin();
		minv[0]=(*it)->start();
		maxv[0]=(*it)->end();
		for(int i=1;i<DIMS;i++)	{
			minv[i]= (*it)->getPosition(i-1);
			maxv[i]= minv[i];
		}

		for(;it!=_points.end();it++){
			minv[0]=min(minv[0],(*it)->start());
			maxv[0]=max(maxv[0],(*it)->end());
			for(int i=1;i<DIMS;i++)	{
				minv[i]= min(minv[i],(*it)->getPosition(i-1));
				maxv[i]= max(maxv[i],(*it)->getPosition(i-1));
			}
		}

		return e;
	}

	virtual movingenvelope_type  getMovingEnvelope( ) {
		return movingenvelope_type();
	}
};
end_gtl_namespace
