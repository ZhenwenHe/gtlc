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
#include "trajectories.h"
begin_gtl_namespace
template<
		typename DataType=double,
		int DIMS=4,
		typename ELEMENT=char,
		typename TRAITS=std::char_traits<ELEMENT>,
		typename ALLOC=std::allocator<ELEMENT> 
> 
class SpatiotemporalReader: virtual public Object<ELEMENT,TRAITS,ALLOC> {
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
		return "SpatiotemporalReader";
	}

	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}

	virtual void readBuffer(const void *  buffer, size_t count=0){

	}

	virtual Object * clone() {
		return 0;
	}	 
public:
	/*        MOVINGPOINT_NAMR  TIMESTAMPE                      X                   Y                   Z
	format :  102_218[string]	2009-11-18T10:11:38Z[string]	52.0100686[double]	4.3641556[double]	84.3[double]
	*/
	static void importTrajectories(const std::basic_string<ELEMENT,TRAITS,ALLOC> & filename,trajectories_type & trajectories ){
		//trajectories.import(filename);
		std::locale _loc = std::locale::global(std::locale(""));  

		std::basic_ifstream<ELEMENT,TRAITS> ifs;

		ifs.open(filename.c_str(),std::ios_base::in|std::ios_base::_Nocreate);
		char sz[1024];	
		trajectory_type * trj= 0;//new trajectory_type;
		double t,x,y,z;
		 
		while(!ifs.eof())
		{				
			
			ifs.getline(sz,1024,'	');
			if(strlen(sz)==0) 
				break;
			else{
				if(trj==0) // the first point
				{
					trj= new trajectory_type;
					trj->setName(sz);
					trajectories.add(trj);
				}
				else{
					if(sz!=trj->getName())
					{
						trj= new trajectory_type;
						trj->setName(sz);
						trajectories.add(trj);
					}
				}
			}
			
			movingvertex_type * mvt= new movingvertex_type;
			//time 
			ifs.getline(sz,1024,'	');
			DateTime<ELEMENT,TRAITS,ALLOC> time(sz);
			t = (double) time.get();
			mvt->start(t);
			mvt->end(t);

			//x
			ifs.getline(sz,1024,'	');
			x=  stringToNumber<double,char>(sz);
			mvt->setPosition(0,x);

			//y
			ifs.getline(sz,1024,'	');
			y=  stringToNumber<double,char>(sz);
			mvt->setPosition(1,y);

			//z
			ifs.getline(sz,1024);
			z=  stringToNumber<double,char>(sz);
			mvt->setPosition(2,z);

			trj->add(mvt);
		}

		ifs.close();
		std::locale::global(_loc);
	}
	 
	/*  mvoing object file [mtf]
	*1. Objects Number
	*2. Object ID, Points Number,R,G,B
	*3. Time, X,Y,Z,During,VX,VY,VZ
	*/
	static void readTrajectories(const std::basic_string<ELEMENT,TRAITS,ALLOC> & filename, trajectories_type & trajectories ){
		//1. Objects Number
		//2. Object ID, Points Number,R,G,B
		//3. Time, X,Y,Z,During,VX,VY,VZ
		//trajectories.read(filename);
		//1. Objects Number
		//2. Object ID, Points Number,R,G,B
		//3. Time, X,Y,Z,During,VX,VY,VZ

		assert(DIMS== 3 || DIMS==4);

		std::locale _loc = std::locale::global(std::locale(""));//设置环境为系统环境  

		std::basic_ifstream<ELEMENT,TRAITS> ifs;

		ifs.open(filename.c_str(),std::ios_base::in|std::ios_base::_Nocreate);
		char sz[1024];	

		if (ifs)
		{				
			trajectories.clear();

			ifs.getline(sz,1024);
			int objectsNumber=stringToNumber<int,char>(sz);
			unsigned long long ObjectID;
			unsigned int pointsNumber =0;
			trajectory_type * trj=0;
			movingvertex_type *mvt =0;
			int i,k,j;
			for(i=0;i<objectsNumber;i++)
			{
				trj = new trajectory_type;

				ifs.getline(sz,1024,' ');
				ObjectID=stringToNumber<unsigned long long,char>(sz);
				trj->setIdentifier(ObjectID);

				ifs.getline(sz,1024,' ');
				pointsNumber=stringToNumber<unsigned int,char>(sz);
				ifs.getline(sz,1024,' ');//igonre r
				//r=stringToNumber<float,char>(sz);
				ifs.getline(sz,1024,' ');//igonre g
				//g=stringToNumber<float,char>(sz);
				ifs.getline(sz,1024);//igonre b
				//b=stringToNumber<float,char>(sz);	
				if(DIMS==4)
				{
					for( j =0;j<pointsNumber;j++)
					{
						mvt = new movingvertex_type;
						ifs.getline(sz,1024,' ');
						mvt->start(stringToNumber<double,char>(sz));
						//X,Y,Z
						for(k=0;k<DIMS-1;k++){
							ifs.getline(sz,1024,' ');
							mvt->setPosition(k,stringToNumber<double,char>(sz));
						}

						ifs.getline(sz,1024,' ');// During
						mvt->end(stringToNumber<double,char>(sz)+mvt->start());
						//VX,VZ
						for(k=0;k<DIMS-2;k++){
							ifs.getline(sz,1024,' ');
							mvt->setVelocity(k,stringToNumber<double,char>(sz));
						} 					
						ifs.getline(sz,1024);// VZ					
						mvt->setVelocity(k,stringToNumber<double,char>(sz));

						trj->add(mvt);
					}
				}
				else{//DIMS==3
					for( j =0;j<pointsNumber;j++)
					{
						mvt = new movingvertex_type;
						ifs.getline(sz,1024,' ');
						mvt->start(stringToNumber<double,char>(sz));
						//X,Y
						for(k=0;k<DIMS-1;k++){
							ifs.getline(sz,1024,' ');
							mvt->setPosition(k,stringToNumber<double,char>(sz));
						}

						// skip Z
						ifs.getline(sz,1024,' ');

						ifs.getline(sz,1024,' ');// During
						mvt->end(stringToNumber<double,char>(sz)+mvt->start());

						//VX,VZ
						for(k=0;k<DIMS-1;k++){
							ifs.getline(sz,1024,' ');
							mvt->setVelocity(k,stringToNumber<double,char>(sz));
						} 		

						//skip VZ
						ifs.getline(sz,1024);// VZ	

						trj->add(mvt);
					}
				}

				trajectories.add(trj);

			}		

		}

		ifs.close();
		std::locale::global(_loc);//还原环境 
	}
	 
	/*  mvoing segment file [msf]
		//1. Objects Number
		//2. Object ID
		//3. Time, X,Y,Z,During,VX,VY,VZ [startpoint]
		//4. Time, X,Y,Z,During,VX,VY,VZ [endpoint]
	*/
	static void readSegments(const std::basic_string<ELEMENT,TRAITS,ALLOC> & filename,MovingSegments<DataType,DIMS> & segments ){
		//segments.read(filename);

		//1. Objects Number
		//2. Object ID
		//3. Time, X,Y,Z,During,VX,VY,VZ [startpoint]
		//4. Time, X,Y,Z,During,VX,VY,VZ [endpoint]
		assert(DIMS==3 || DIMS==4);
		std::locale _loc = std::locale::global(std::locale(""));//ÉèÖÃ»·¾³ÎªÏµÍ³»·¾³  

		std::basic_ifstream<ELEMENT,TRAITS> ifs;

		ifs.open(filename.c_str(),std::ios_base::in|std::ios_base::_Nocreate);
		char sz[1024];	

		if (ifs)
		{				
			segments.clear();
			ifs.getline(sz,1024);
			int objectsNumber=stringToNumber<int,char>(sz);
			uint64_t ObjectID;
			movingsegment_type * seg=0;
			movingvertex_type *mvt =0;
			int i,k;

			for(i=0;i<objectsNumber;i++)
			{
				seg = new movingsegment_type();

				ifs.getline(sz,1024);
				ObjectID=stringToNumber<uint64_t,char>(sz);
				seg->setIdentifier(ObjectID);

				if(DIMS==4){
					mvt = &(seg->getStartVertex());

					ifs.getline(sz,1024,' ');
					mvt->start(stringToNumber<double,char>(sz));

					for(k=0;k<DIMS-1;k++){
						ifs.getline(sz,1024,' ');
						mvt->setPosition(k,stringToNumber<double,char>(sz));
					}

					ifs.getline(sz,1024,' ');// During
					mvt->end(stringToNumber<double,char>(sz)+mvt->start());

					for(k=0;k<DIMS-2;k++){
						ifs.getline(sz,1024,' ');
						mvt->setVelocity(k,stringToNumber<double,char>(sz));
					} 
					ifs.getline(sz,1024);// VZ
					mvt->setVelocity(k,stringToNumber<double,char>(sz));

					mvt = &(seg->getEndVertex());

					ifs.getline(sz,1024,' ');
					mvt->start(stringToNumber<double,char>(sz));
					for(k=0;k<DIMS-1;k++){
						ifs.getline(sz,1024,' ');
						mvt->setPosition(k,stringToNumber<double,char>(sz));
					}

					ifs.getline(sz,1024,' ');// During
					mvt->end(stringToNumber<double,char>(sz)+mvt->start());

					for(k=0;k<DIMS-2;k++){
						ifs.getline(sz,1024,' ');
						mvt->setVelocity(k,stringToNumber<double,char>(sz));
					} 
					ifs.getline(sz,1024);// VZ
					mvt->setVelocity(k,stringToNumber<double,char>(sz));	
				}
				else{
					mvt = &(seg->getStartVertex());

					ifs.getline(sz,1024,' ');
					mvt->start(stringToNumber<double,char>(sz));

					for(k=0;k<DIMS-1;k++){
						ifs.getline(sz,1024,' ');
						mvt->setPosition(k,stringToNumber<double,char>(sz));
					}

					//skip Z
					ifs.getline(sz,1024,' ');

					ifs.getline(sz,1024,' ');// During
					mvt->end(stringToNumber<double,char>(sz)+mvt->start());
					//VX,VY
					for(k=0;k<DIMS-1;k++){
						ifs.getline(sz,1024,' ');
						mvt->setVelocity(k,stringToNumber<double,char>(sz));
					} 
					//skip VZ
					ifs.getline(sz,1024);// VZ					

					mvt = &(seg->getEndVertex());

					//Start
					ifs.getline(sz,1024,' ');
					mvt->start(stringToNumber<double,char>(sz));
					//X,Y
					for(k=0;k<DIMS-1;k++){
						ifs.getline(sz,1024,' ');
						mvt->setPosition(k,stringToNumber<double,char>(sz));
					}
					ifs.getline(sz,1024,' ');//skip Z

					ifs.getline(sz,1024,' ');// During
					mvt->end(stringToNumber<double,char>(sz)+mvt->start());
					//VX,VY
					for(k=0;k<DIMS-1;k++){
						ifs.getline(sz,1024,' ');
						mvt->setVelocity(k,stringToNumber<double,char>(sz));
					} 
					ifs.getline(sz,1024);// skip VZ 	
				}	

				segments.add(seg);
			}		

		}
		ifs.close();
		std::locale::global(_loc);//»¹Ô­»·¾³  
	}

	/* moving point file [mpf]
	//1. Objects Number
	//2. Object ID, Time, X,Y,Z,During,VX,VY,VZ
	*/
	static void readPoints(const std::basic_string<ELEMENT,TRAITS,ALLOC> & filename,MovingPoints<DataType,DIMS> & points)
	{
		//1. Objects Number
		//2. Object ID, Time, X,Y,Z,During,VX,VY,VZ

		std::locale _loc = std::locale::global(std::locale(""));//ÉèÖÃ»·¾³ÎªÏµÍ³»·¾³  

		std::basic_ifstream<ELEMENT,TRAITS> ifs;

		ifs.open(filename.c_str(),std::ios_base::in|std::ios_base::_Nocreate);
		char sz[1024];	

		if (ifs)
		{				
			points.clear();
			ifs.getline(sz,1024);
			int objectsNumber=stringToNumber<int,char>(sz);
			uint64_t ObjectID;
			movingpoint_type *mvt =0;
			int i,k;
			points.extend(objectsNumber);
			for(i=0;i<objectsNumber;i++)
			{
				mvt = new movingpoint_type();

				ifs.getline(sz,1024);
				ObjectID=stringToNumber<uint64_t,char>(sz);
				mvt->setIdentifier(ObjectID);
				 
				ifs.getline(sz,1024,' ');
				mvt->start(stringToNumber<double,char>(sz));
				for(k=0;k<DIMS-1;k++){
					ifs.getline(sz,1024,' ');
					mvt->setPosition(k,stringToNumber<double,char>(sz));
				}

				ifs.getline(sz,1024,' ');// During
				mvt->end(stringToNumber<double,char>(sz)+mvt->start());

				for(k=0;k<DIMS-2;k++){
					ifs.getline(sz,1024,' ');
					mvt->setVelocity(k,stringToNumber<double,char>(sz));
				} 
				ifs.getline(sz,1024);// VZ
				mvt->setVelocity(k,stringToNumber<double,char>(sz));

				points.set(i,mvt);
			}		

		}
		ifs.close();
		std::locale::global(_loc);//restore the io environment
	}

	/* import a dataset generated by Brinkhoff spatiotemporal generator
		A line of the text file represents the generated position of an object; 
		it is described by the following fields (separated by tabulators):
			• newpoint (for the first position of a new moving object), 
				point (for the following positions of a moving object), 
				or disappearpoint (if a moving object has reached its destination)
			• the id of the point
			• the sequence number (starts with 1)
			• the id of the object class
			• the time stamp (as integer)
			• the x-coordinate (as floating-point number)
			• the y-coordinate (as floating-point number)
			• the current speed (in space units per time unit as a floating-point number; 
				note: the speed may change several times between two reported positions)
			• the x-coordinate of the next node that will be passed (as integer)
			• the y-coordinate of the next node that will be passed (as integer)
	*/
	static void importBrinkhoff(const std::basic_string<ELEMENT,TRAITS,ALLOC> & filename,Trajectories<DataType,DIMS> & trjs, uint64_t count = 0)
	{
		assert(DIMS>=3);

		std::locale _loc = std::locale::global(std::locale("")); 

		std::basic_ifstream<ELEMENT,TRAITS> ifs;

		ifs.open(filename.c_str(),std::ios_base::in|std::ios_base::_Nocreate);
		char sz[1024];	

		if (!ifs) return ;
		uint64_t trjid;
		double xyz[DIMS-1];
		int t;
		double vxyz[DIMS-1]; 
		double av;
		movingvertex_type mv;
		uint64_t i=0;

		trjs.clear();
		while(!ifs.eof()){
			//newpoint, point or disappearpoint
			ifs.getline(sz,1024,'	');			
			// id of the point
			ifs.getline(sz,1024,'	'); 
			trjid = stringToNumber<uint64_t,ELEMENT>(sz);
			// the sequence number
			ifs.getline(sz,1024,'	'); 
			//the id of the object class
			ifs.getline(sz,1024,'	');
			//the time stamp (as integer)
			ifs.getline(sz,1024,'	');
			t = stringToNumber<int32_t,ELEMENT>(sz);
			//the x-coordinate (as floating-point number)
			ifs.getline(sz,1024,'	');
			xyz[0] = stringToNumber<double,ELEMENT>(sz);
			//the y-coordinate (as floating-point number)
			ifs.getline(sz,1024,'	');
			xyz[1] = stringToNumber<double,ELEMENT>(sz);
			//the current speed (in space units per time unit as a floating-point number; 
			ifs.getline(sz,1024,'	');
			av = stringToNumber<double,ELEMENT>(sz);
			//the x-coordinate of the next node that will be passed (as integer)
			ifs.getline(sz,1024,'	');
			//dx = stringToNumber<double,ELEMENT>(sz);
			// the y-coordinate of the next node that will be passed (as integer)
			ifs.getline(sz,1024);
			//dy = stringToNumber<double,ELEMENT>(sz);

			// calculate the velocity and x,y,z values
			if(DIMS==4) xyz[DIMS-2] = rand()%100;
			//dz = rand()%100;
			
			//add the movingvertex to trjactories
			trjs.add(trjid,movingvertex_type(xyz,t,t,0));
			i++;

			if(i==count)
					break;
		}

		ifs.close();
		std::locale::global(_loc);

		trjs.check();

		trjs.recalculateVelocities();
	}
};

end_gtl_namespace
