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
#include "config.h"
#include "trajectory.h"
#include "datetime.h"

begin_gtl_namespace
template<
		typename DataType=double,
		int DIMS=4,
		typename ELEMENT=char,
		typename TRAITS=std::char_traits<ELEMENT>,
		typename ALLOC=std::allocator<ELEMENT> 
> 
class Trajectories : public MovingShape<DataType,DIMS,ELEMENT,TRAITS,ALLOC> {
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
	typedef typename std::vector< movingvertex_type *>            movingvertexptr_vector_type;
	typedef typename std::vector< movingvertex_type >             movingvertex_vector;
	typedef typename std::vector< movingvertex_type * >::iterator iterator;
	typedef  Trajectory<DataType,DIMS,ELEMENT,TRAITS,ALLOC>       trajectory_type;
	
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
	std::vector<trajectory_type*>               _trajectories;
public:
	Trajectories(){ 
		_shapeType = ST_TRAJECTORIES;
	}
	virtual ~Trajectories(){ 
		clear();
	}
	inline void clear(){
		for(std::vector<trajectory_type*>::iterator it = _trajectories.begin();
			it!=_trajectories.end();it++){
				delete (*it);
		}
		_trajectories.clear();
	}

	
	inline trajectory_type& operator[](size_t i) { return * _trajectories[i];} 
	inline trajectory_type* get(size_t i) { return _trajectories[i];} 
	inline const trajectory_type* get(size_t i) const { return _trajectories[i];}
	inline movingvertex_type* get(size_t i, size_t j) {return _trajectories[i]->get(j);}
	inline size_t size() const {return _trajectories.size();}
	inline void add(trajectory_type * p){
		_trajectories.push_back(p);
	}	
	inline void add(uint64_t trjid, const movingvertex_type & pnt){ 
		auto it = std::find_if(_trajectories.begin(),_trajectories.end(),
			[&trjid](trajectory_type * tj)->bool {
				if(tj->getIdentifier()==trjid)
					return true;
				else 
					return false;
		});
		if(it != _trajectories.end())
		{
			(*it)->add(pnt);
		}
		else{// create a new trajectory object
			trajectory_type* p = new trajectory_type;
			p->setIdentifier(trjid);
			p->add(pnt);
			_trajectories.push_back(p);
		}
	}

	inline void split(movingsegments_type & segments){
		for(std::vector<trajectory_type*>::iterator it = _trajectories.begin();
			it!=_trajectories.end();it++)
			(*it)->split(segments);
	}
	inline void recalculateVelocities(){
		for(auto it = _trajectories.begin();it!=_trajectories.end();it++)
			(*it)->recalculateVelocities();
	}
	inline void check(){//remove the objects which is invalid
		for(auto it = _trajectories.begin();it!=_trajectories.end();it++){
			if((*it)->size()<2){
				delete (*it);
				*it =0;
			}
		}
		std::remove_if(_trajectories.begin(),_trajectories.end(),[](trajectory_type * it)->bool{
			if(it==0) return true;
			else return false;
		});
	}
	inline void split(MovingPoints<DataType,DIMS> & pnts){
		for(std::vector<trajectory_type*>::iterator it = _trajectories.begin();
			it!=_trajectories.end();it++)
			(*it)->split(pnts);
	}
	void normalize(double minval, double maxval){//½«t,x,y,z±ä»»µ½[0,1]
		envelope_type total;
		double temp=0;
		for(std::vector<trajectory_type*>::iterator it = _trajectories.begin();
			it!=_trajectories.end();it++){
				total.extend((*it)->getEnvelope());
		}
		movingvertex_type  * mvt=0;
		DataType * minv = total.getLowerBounds();
		DataType * maxv = total.getUpperBounds();
		for(std::vector<trajectory_type*>::iterator it = _trajectories.begin();
			it!=_trajectories.end();it++){
			for(size_t i=0;i<(*it)->size();i++){
				mvt = (*it)->get(i);
				temp = (mvt->start()-minv[0])/(maxv[0]-minv[0]);
				temp = (maxval-minval)*temp+minval;
				mvt->start(temp);
				mvt->end(temp);
				for(int j=0;j<DIMS-1;j++)
				{
					temp = (mvt->getPosition(j)-minv[j+1])/(maxv[j+1]-minv[j+1]);
					temp = (maxval-minval)*temp+minval;
					mvt->setPosition(j,temp);
				}
			}	
		}
	}

	//void write(const std::basic_string<ELEMENT,TRAITS,ALLOC> & filename) const
	//{
	//	assert(DIMS==4);

	//	//1. Objects Number
	//	//2. Object ID, Points Number,R,G,B
	//	//3. Time, X,Y,Z,During,VX,VY,VZ

	//	std::locale _loc = std::locale::global(std::locale("")); 

	//	std::basic_ofstream<ELEMENT,TRAITS> ofs;
	//	ofs.open(filename.c_str(),std::ios_base::out|std::ios_base::trunc);
	//	trajectory_type * trj=0; 
	//	if (ofs)
	//	{				
	//		ofs<< _trajectories.size() << std::endl;

	//		for(size_t i=0;i< _trajectories.size();i++)
	//		{
	//			trj = (trajectory_type *) _trajectories[i];
	//			ofs<<i<<" "//Object ID
	//				<<trj->size()<<" "//Points Number
	//				<<(rand()%255)/255.0<<" "//R
	//				<<(rand()%255)/255.0<<" "//G
	//				<<(rand()%255)/255.0<<std::endl;//B

	//			for(size_t j=0;j<trj->size();j++)
	//			{
	//				ofs<<trj->get(j)->start();
	//				for(int k=0;k<DIMS-1;k++){
	//					ofs<<" "<<trj->get(j)->getPosition(k);//x,y,z
	//				}
	//				ofs<<" " <<trj->get(j)->during() ;//During
	//				for(int k=0;k<DIMS-1;k++){
	//					ofs <<" " <<trj->get(j)->getVelocity(k) ;//VX,VY,VZ
	//				}
	//				ofs<<std::endl;
	//			}
	//		}
	//	}

	//	ofs.close();
	//	std::locale::global(_loc);  

	//}

	//void read(const std::basic_string<ELEMENT,TRAITS,ALLOC> & filename){
	//	//1. Objects Number
	//	//2. Object ID, Points Number,R,G,B
	//	//3. Time, X,Y,Z,During,VX,VY,VZ

	//	assert(DIMS== 3 || DIMS==4);

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
	//		unsigned int pointsNumber =0;
	//		trajectory_type * trj=0;
	//		movingvertex_type *mvt =0;
	//		int i,k,j;
	//		for(i=0;i<objectsNumber;i++)
	//		{
	//			trj = new trajectory_type;

	//			ifs.getline(sz,1024,' ');
	//			ObjectID=stringToNumber<unsigned long long,char>(sz);
	//			trj->setIdentifier(ObjectID);

	//			ifs.getline(sz,1024,' ');
	//			pointsNumber=stringToNumber<unsigned int,char>(sz);
	//			ifs.getline(sz,1024,' ');//igonre r
	//			//r=stringToNumber<float,char>(sz);
	//			ifs.getline(sz,1024,' ');//igonre g
	//			//g=stringToNumber<float,char>(sz);
	//			ifs.getline(sz,1024);//igonre b
	//			//b=stringToNumber<float,char>(sz);	
	//			if(DIMS==4)
	//			{
	//				for( j =0;j<pointsNumber;j++)
	//				{
	//					mvt = new movingvertex_type;
	//					ifs.getline(sz,1024,' ');
	//					mvt->start(stringToNumber<double,char>(sz));
	//					//X,Y,Z
	//					for(k=0;k<DIMS-1;k++){
	//						ifs.getline(sz,1024,' ');
	//						mvt->setPosition(k,stringToNumber<double,char>(sz));
	//					}

	//					ifs.getline(sz,1024,' ');// During
	//					mvt->end(stringToNumber<double,char>(sz)+mvt->start());
	//					//VX,VZ
	//					for(k=0;k<DIMS-2;k++){
	//						ifs.getline(sz,1024,' ');
	//						mvt->setVelocity(k,stringToNumber<double,char>(sz));
	//					} 					
	//					ifs.getline(sz,1024);// VZ					
	//					mvt->setVelocity(k,stringToNumber<double,char>(sz));

	//					trj->add(mvt);
	//				}
	//			}
	//			else{//DIMS==3
	//				for( j =0;j<pointsNumber;j++)
	//				{
	//					mvt = new movingvertex_type;
	//					ifs.getline(sz,1024,' ');
	//					mvt->start(stringToNumber<double,char>(sz));
	//					//X,Y
	//					for(k=0;k<DIMS-1;k++){
	//						ifs.getline(sz,1024,' ');
	//						mvt->setPosition(k,stringToNumber<double,char>(sz));
	//					}

	//					// skip Z
	//					ifs.getline(sz,1024,' ');

	//					ifs.getline(sz,1024,' ');// During
	//					mvt->end(stringToNumber<double,char>(sz)+mvt->start());

	//					//VX,VZ
	//					for(k=0;k<DIMS-1;k++){
	//						ifs.getline(sz,1024,' ');
	//						mvt->setVelocity(k,stringToNumber<double,char>(sz));
	//					} 		

	//					//skip VZ
	//					ifs.getline(sz,1024);// VZ	

	//					trj->add(mvt);
	//				}
	//			}

	//			add(trj);

	//		}		

	//	}

	//	ifs.close();
	//	std::locale::global(_loc);//还原环境  
	//}

	/*
	format :  102_218[string]	2009-11-18T10:11:38Z[string]	52.0100686[double]	4.3641556[double]	84.3[double]
	*/
	//void import(const std::basic_string<ELEMENT,TRAITS,ALLOC> & filename){
	//	std::locale _loc = std::locale::global(std::locale(""));  

	//	std::basic_ifstream<ELEMENT,TRAITS> ifs;

	//	ifs.open(filename.c_str(),std::ios_base::in|std::ios_base::_Nocreate);
	//	char sz[1024];	
	//	trajectory_type * trj= 0;//new trajectory_type;
	//	double t,x,y,z;
	//	 
	//	while(!ifs.eof())
	//	{				
	//		
	//		ifs.getline(sz,1024,'	');
	//		if(strlen(sz)==0) 
	//			break;
	//		else{
	//			if(trj==0) // the first point
	//			{
	//				trj= new trajectory_type;
	//				trj->setName(sz);
	//				add(trj);
	//			}
	//			else{
	//				if(sz!=trj->getName())
	//				{
	//					trj= new trajectory_type;
	//					trj->setName(sz);
	//					add(trj);
	//				}
	//			}
	//		}
	//		
	//		movingvertex_type * mvt= new movingvertex_type;
	//		//time 
	//		ifs.getline(sz,1024,'	');
	//		DateTime<ELEMENT,TRAITS,ALLOC> time(sz);
	//		t = (double) time.get();
	//		mvt->start(t);
	//		mvt->end(t);

	//		//x
	//		ifs.getline(sz,1024,'	');
	//		x=  stringToNumber<double,char>(sz);
	//		mvt->setPosition(0,x);

	//		//y
	//		ifs.getline(sz,1024,'	');
	//		y=  stringToNumber<double,char>(sz);
	//		mvt->setPosition(1,y);

	//		//z
	//		ifs.getline(sz,1024);
	//		z=  stringToNumber<double,char>(sz);
	//		mvt->setPosition(2,z);

	//		trj->add(mvt);
	//	}

	//	ifs.close();
	//	std::locale::global(_loc);

	//}
public:
	virtual envelope_type getEnvelope() {
		envelope_type v;
		for( int i=0;i<_trajectories.size();i++)	{
			v.extend(_trajectories[i]->getEnvelope()); 
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
