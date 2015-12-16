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
class MovingPoints : public MovingShape<DataType,DIMS,ELEMENT,TRAITS,ALLOC> {
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
		return "MovingPoints";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
protected:
	std::vector<movingpoint_type*>  _objects;
	struct CompareKey 
	{
		bool operator ()(movingpoint_type*  i, movingpoint_type* j){
			if(i->start()<j->start())
				return true;
			else
				return false;
		}
	} compare_time;
public:
	MovingPoints(){ 
		_shapeType = ST_POINTS; 
	}

	void sortByTime(){
		std::sort(_objects.begin(),_objects.end(),compare_time);
	}
	
	virtual ~MovingPoints(){ 
		clear();
	}

	inline void clear(){ 
		for(std::vector<movingpoint_type*>::iterator it = _objects.begin();
			it!=_objects.end();it++){
				delete (*it);
		}
		_objects.clear();
	}

	inline movingpoint_type& operator[](size_t i) { return * _objects[i];} 
	inline movingpoint_type* get(size_t i) { return _objects[i];} 
	inline const movingpoint_type* get(size_t i) const { return _objects[i];} 
	inline size_t size() const {return _objects.size();}
	inline void add(movingpoint_type * p){
		_objects.push_back(p);
	}
	inline void extend(size_t newsize){
		if(newsize> _objects.size())
			_objects.resize(newsize);
	}
	inline void set(size_t i,movingpoint_type*p) { _objects[i]=p;} 
	//void write (const std::basic_string<ELEMENT,TRAITS,ALLOC> & filename ){
	//	//1. Objects Number
	//	//2. Object ID,Time, X,Y,Z,During,VX,VY,VZ
	//	std::locale _loc = std::locale::global(std::locale(""));

	//	std::basic_ofstream<ELEMENT,TRAITS> ofs;
	//	ofs.open(filename.c_str(),std::ios_base::out|std::ios_base::trunc); 
	//	movingpoint_type * mv=0;
	//	int k,i;
	//	if (ofs)
	//	{				
	//		ofs<< _objects.size() << std::endl;

	//		for( i=0;i<_objects.size();i++)
	//		{
	//			mv =   _objects[i];
	//			ofs<<i<<std::endl;//Object ID 
	//			ofs<<mv->start();
	//			for( k=0;k<DIMS-1;k++){
	//				ofs<<" "<<mv->getPosition(k);//x,y,z
	//			}
	//			ofs<<" "<<mv->during() ;//During

	//			for( k=0;k<DIMS-1;k++){
	//				ofs <<" " <<mv->getVelocity(k) ;//VX,VY,VZ
	//			}
	//			ofs<<std::endl;
	//		}
	//	}

	//	ofs.close();
	//	std::locale::global(_loc); 

	//}

	//void read(const std::basic_string<ELEMENT,TRAITS,ALLOC> & filename){
	//	//1. Objects Number
	//	//2. Object ID, Time, X,Y,Z,During,VX,VY,VZ

	//	std::locale _loc = std::locale::global(std::locale(""));//设置环境为系统环境  

	//	std::basic_ifstream<ELEMENT,TRAITS> ifs;

	//	ifs.open(filename.c_str(),std::ios_base::in|std::ios_base::_Nocreate);
	//	char sz[1024];	

	//	if (ifs)
	//	{				
	//		clear();
	//		ifs.getline(sz,1024);
	//		int objectsNumber=stringToNumber<int,char>(sz);
	//		uint64_t ObjectID;
	//		movingpoint_type *mvt =0;
	//		int i,k;

	//		for(i=0;i<objectsNumber;i++)
	//		{
	//			mvt = new movingpoint_type();

	//			ifs.getline(sz,1024);
	//			ObjectID=stringToNumber<uint64_t,char>(sz);
	//			mvt->setIdentifier(ObjectID);
	//			 
	//			ifs.getline(sz,1024,' ');
	//			mvt->start(stringToNumber<double,char>(sz));
	//			for(k=0;k<DIMS-1;k++){
	//				ifs.getline(sz,1024,' ');
	//				mvt->setPosition(k,stringToNumber<double,char>(sz));
	//			}

	//			ifs.getline(sz,1024,' ');// During
	//			mvt->end(stringToNumber<double,char>(sz)+mvt->start());

	//			for(k=0;k<DIMS-2;k++){
	//				ifs.getline(sz,1024,' ');
	//				mvt->setVelocity(k,stringToNumber<double,char>(sz));
	//			} 
	//			ifs.getline(sz,1024);// VZ
	//			mvt->setVelocity(k,stringToNumber<double,char>(sz));

	//			add(mvt);
	//		}		

	//	}
	//	ifs.close();
	//	std::locale::global(_loc);//还原环境  
	//}

	virtual envelope_type getEnvelope() {
		envelope_type v;
		for( int i=0;i<_objects.size();i++)	{
			v.extend(_objects[i]->getEnvelope()); 
		}
		return v;
	}

	virtual movingenvelope_type getMovingEnvelope() {
		movingenvelope_type me;
		return me;
	} 

	virtual vertex_type getCenter(){
		return getEnvelope().getCenter();
	}
};
end_gtl_namespace
