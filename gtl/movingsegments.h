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
#include "movingsegment.h"
begin_gtl_namespace
template<
		typename DataType=double,
		int DIMS=4,
		typename ELEMENT=char,
		typename TRAITS=std::char_traits<ELEMENT>,
		typename ALLOC=std::allocator<ELEMENT> 
> 
class MovingSegments : public MovingShape<DataType,DIMS,ELEMENT,TRAITS,ALLOC> {
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
	typedef  MovingSegment<DataType,DIMS,ELEMENT,TRAITS,ALLOC>  movingsegment_type;
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "MovingSegments";
	}

	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}

	virtual void readBuffer(const void *  buffer, size_t count=0){

	}

	virtual Object * clone() {
		return 0;
	}

protected:
	std::vector<movingsegment_type*>  _objects;

	struct CompareKey 
	{
		bool operator ()(movingsegment_type*  i, movingsegment_type* j){
			if(i->getStartVertex().start() <j->getStartVertex().start())
				return true;
			else
				return false;
		}
	} compare_time;
public:
	void sortByStartTime(){
		std::sort(_objects.begin(),_objects.end(),compare_time);
	}
public:
	MovingSegments(){ 
		_shapeType = ST_SEGMENTS;
	}
	virtual ~MovingSegments(){ 
		clear();
	}
	inline void clear(){ 
		for(std::vector<movingsegment_type*>::iterator it = _objects.begin();
			it!=_objects.end();it++){
				delete (*it);
		}
		_objects.clear();
	}
	inline movingsegment_type& operator[](size_t i) { return * _objects[i];} 
	inline movingsegment_type* get(size_t i) { return _objects[i];} 
	inline const movingsegment_type* get(size_t i) const { return _objects[i];} 
	inline size_t size() const {return _objects.size();}
	inline void add(movingsegment_type * p){
		_objects.push_back(p);
	}

	//void write (const std::string & filename ){
	//	assert(DIMS==4);
	//	//1. Objects Number
	//	//2. Object ID
	//	//3. Time, X,Y,Z,During,VX,VY,VZ [startpoint]
	//	//4. Time, X,Y,Z,During,VX,VY,VZ [endpoint]

	//	std::locale _loc = std::locale::global(std::locale(""));

	//	std::basic_ofstream<ELEMENT,TRAITS> ofs;
	//	ofs.open(filename.c_str(),std::ios_base::out|std::ios_base::trunc);

	//	movingsegment_type * ms=0; 
	//	movingvertex_type * mv=0;
	//	int k,i;
	//	if (ofs)
	//	{				
	//		ofs<< _objects.size() << std::endl;

	//		for( i=0;i<_objects.size();i++)
	//		{
	//			ms = (movingsegment_type *) _objects[i];
	//			ofs<<i<<std::endl;//Object ID

	//			mv = &(ms->getStartVertex());
	//			ofs<<mv->start();
	//			for( k=0;k<DIMS-1;k++){
	//				ofs<<" "<<mv->getPosition(k);//x,y,z
	//			}
	//			ofs<<" " <<mv->during() ;//During

	//			for( k=0;k<DIMS-1;k++){
	//				ofs <<" " <<mv->getVelocity(k) ;//VX,VY,VZ
	//			}
	//			ofs<<std::endl;

	//			mv = &(ms->getEndVertex());
	//			ofs<<mv->start();
	//			for( k=0;k<DIMS-1;k++){
	//				ofs<<" "<<mv->getPosition(k);//x,y,z
	//			}
	//			ofs<<" " <<mv->during() ;//During

	//			for( k=0;k<DIMS-1;k++){
	//				ofs <<" " <<mv->getVelocity(k) ;//VX,VY,VZ
	//			}
	//			ofs<<std::endl;
	//		}
	//	}

	//	ofs.close();
	//	std::locale::global(_loc); 

	//}

	//void read(const std::string & filename){
	//	//1. Objects Number
	//	//2. Object ID
	//	//3. Time, X,Y,Z,During,VX,VY,VZ [startpoint]
	//	//4. Time, X,Y,Z,During,VX,VY,VZ [endpoint]
	//	assert(DIMS==3 || DIMS==4);
	//	std::locale _loc = std::locale::global(std::locale(""));//设置环境为系统环境  

	//	std::basic_ifstream<ELEMENT,TRAITS> ifs;

	//	ifs.open(filename.c_str(),std::ios_base::in|std::ios_base::_Nocreate);
	//	char sz[1024];	

	//	if (ifs)
	//	{				
	//		clear();
	//		ifs.getline(sz,1024);
	//		int objectsNumber=stringToNumber<int,char>(sz);
	//		unsigned long long ObjectID;
	//		movingsegment_type * seg=0;
	//		movingvertex_type *mvt =0;
	//		int i,k;

	//		for(i=0;i<objectsNumber;i++)
	//		{
	//			seg = new movingsegment_type();

	//			ifs.getline(sz,1024);
	//			ObjectID=stringToNumber<unsigned long long,char>(sz);
	//			seg->setIdentifier(ObjectID);

	//			if(DIMS==4){
	//				mvt = &(seg->getStartVertex());

	//				ifs.getline(sz,1024,' ');
	//				mvt->start(stringToNumber<double,char>(sz));

	//				for(k=0;k<DIMS-1;k++){
	//					ifs.getline(sz,1024,' ');
	//					mvt->setPosition(k,stringToNumber<double,char>(sz));
	//				}

	//				ifs.getline(sz,1024,' ');// During
	//				mvt->end(stringToNumber<double,char>(sz)+mvt->start());

	//				for(k=0;k<DIMS-2;k++){
	//					ifs.getline(sz,1024,' ');
	//					mvt->setVelocity(k,stringToNumber<double,char>(sz));
	//				} 
	//				ifs.getline(sz,1024);// VZ
	//				mvt->setVelocity(k,stringToNumber<double,char>(sz));

	//				mvt = &(seg->getEndVertex());

	//				ifs.getline(sz,1024,' ');
	//				mvt->start(stringToNumber<double,char>(sz));
	//				for(k=0;k<DIMS-1;k++){
	//					ifs.getline(sz,1024,' ');
	//					mvt->setPosition(k,stringToNumber<double,char>(sz));
	//				}

	//				ifs.getline(sz,1024,' ');// During
	//				mvt->end(stringToNumber<double,char>(sz)+mvt->start());

	//				for(k=0;k<DIMS-2;k++){
	//					ifs.getline(sz,1024,' ');
	//					mvt->setVelocity(k,stringToNumber<double,char>(sz));
	//				} 
	//				ifs.getline(sz,1024);// VZ
	//				mvt->setVelocity(k,stringToNumber<double,char>(sz));	
	//			}
	//			else{
	//				mvt = &(seg->getStartVertex());

	//				ifs.getline(sz,1024,' ');
	//				mvt->start(stringToNumber<double,char>(sz));

	//				for(k=0;k<DIMS-1;k++){
	//					ifs.getline(sz,1024,' ');
	//					mvt->setPosition(k,stringToNumber<double,char>(sz));
	//				}

	//				//skip Z
	//				ifs.getline(sz,1024,' ');

	//				ifs.getline(sz,1024,' ');// During
	//				mvt->end(stringToNumber<double,char>(sz)+mvt->start());
	//				//VX,VY
	//				for(k=0;k<DIMS-1;k++){
	//					ifs.getline(sz,1024,' ');
	//					mvt->setVelocity(k,stringToNumber<double,char>(sz));
	//				} 
	//				//skip VZ
	//				ifs.getline(sz,1024);// VZ					

	//				mvt = &(seg->getEndVertex());

	//				//Start
	//				ifs.getline(sz,1024,' ');
	//				mvt->start(stringToNumber<double,char>(sz));
	//				//X,Y
	//				for(k=0;k<DIMS-1;k++){
	//					ifs.getline(sz,1024,' ');
	//					mvt->setPosition(k,stringToNumber<double,char>(sz));
	//				}
	//				ifs.getline(sz,1024,' ');//skip Z

	//				ifs.getline(sz,1024,' ');// During
	//				mvt->end(stringToNumber<double,char>(sz)+mvt->start());
	//				//VX,VY
	//				for(k=0;k<DIMS-1;k++){
	//					ifs.getline(sz,1024,' ');
	//					mvt->setVelocity(k,stringToNumber<double,char>(sz));
	//				} 
	//				ifs.getline(sz,1024);// skip VZ 	
	//			}	

	//			add(seg);
	//		}		

	//	}
	//	ifs.close();
	//	std::locale::global(_loc);//还原环境  
	//}
public:
	virtual envelope_type getEnvelope() {
		envelope_type v;
		for(int i=0;i<_objects.size();i++)	{
			v.extend(_objects[i]->getEnvelope()); 
		}
		return v;
	}

	virtual vertex_type getCenter()  {
		vertex_type v;
		getEnvelope().getCenter(v);
		return v;
	}
	 
	virtual movingenvelope_type  getMovingEnvelope( ) {
		return movingenvelope_type();
	}
};
end_gtl_namespace
