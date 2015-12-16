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
class SpatiotemporalWriter: virtual public Object<ELEMENT,TRAITS,ALLOC> {
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
		return "SpatiotemporalWriter";
	}

	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}

	virtual void readBuffer(const void *  buffer, size_t count=0){

	}

	virtual Object * clone() {
		return 0;
	}	 
public:
	 
	/* mvoing object file [mtf]
	*1. Objects Number
	*2. Object ID, Points Number,R,G,B
	*3. Time, X,Y,Z,During,VX,VY,VZ
	*/
	static void writeTrajectories(const std::basic_string<ELEMENT,TRAITS,ALLOC> & filename,const trajectories_type & trajectories ){
		//1. Objects Number
		//2. Object ID, Points Number,R,G,B
		//3. Time, X,Y,Z,During,VX,VY,VZ
		//trajectories.write(filename);
		assert(DIMS==4);

		//1. Objects Number
		//2. Object ID, Points Number,R,G,B
		//3. Time, X,Y,Z,During,VX,VY,VZ

		std::locale _loc = std::locale::global(std::locale("")); 

		std::basic_ofstream<ELEMENT,TRAITS> ofs;
		ofs.open(filename.c_str(),std::ios_base::out|std::ios_base::trunc);
		trajectory_type * trj=0; 
		if (ofs)
		{				
			ofs<< trajectories.size() << std::endl;

			for(size_t i=0;i< trajectories.size();i++)
			{
				trj = (trajectory_type *) trajectories.get(i);
				ofs<<i<<" "//Object ID
					<<trj->size()<<" "//Points Number
					<<(rand()%255)/255.0<<" "//R
					<<(rand()%255)/255.0<<" "//G
					<<(rand()%255)/255.0<<std::endl;//B

				for(size_t j=0;j<trj->size();j++)
				{
					ofs<<trj->get(j)->start();
					for(int k=0;k<DIMS-1;k++){
						ofs<<" "<<trj->get(j)->getPosition(k);//x,y,z
					}
					ofs<<" " <<trj->get(j)->during() ;//During
					for(int k=0;k<DIMS-1;k++){
						ofs <<" " <<trj->get(j)->getVelocity(k) ;//VX,VY,VZ
					}
					ofs<<std::endl;
				}
			}
		}

		ofs.close();
		std::locale::global(_loc); 
	}

	 
	/*  mvoing segment file [msf]
		//1. Objects Number
		//2. Object ID
		//3. Time, X,Y,Z,During,VX,VY,VZ [startpoint]
		//4. Time, X,Y,Z,During,VX,VY,VZ [endpoint]
	*/
	static void writeSegments(const std::basic_string<ELEMENT,TRAITS,ALLOC> & filename,const MovingSegments<DataType,DIMS> & segments ){
		
		//segments.write(filename);		

		assert(DIMS==4);
		//1. Objects Number
		//2. Object ID
		//3. Time, X,Y,Z,During,VX,VY,VZ [startpoint]
		//4. Time, X,Y,Z,During,VX,VY,VZ [endpoint]

		std::locale _loc = std::locale::global(std::locale(""));

		std::basic_ofstream<ELEMENT,TRAITS> ofs;
		ofs.open(filename.c_str(),std::ios_base::out|std::ios_base::trunc);

		movingsegment_type * ms=0; 
		movingvertex_type * mv=0;
		int k,i;
		if (ofs)
		{				
			ofs<< segments.size() << std::endl;

			for( i=0;i<segments.size();i++)
			{
				ms = (movingsegment_type *) segments.get(i);
				ofs<<i<<std::endl;//Object ID

				mv = &(ms->getStartVertex());
				ofs<<mv->start();
				for( k=0;k<DIMS-1;k++){
					ofs<<" "<<mv->getPosition(k);//x,y,z
				}
				ofs<<" " <<mv->during() ;//During

				for( k=0;k<DIMS-1;k++){
					ofs <<" " <<mv->getVelocity(k) ;//VX,VY,VZ
				}
				ofs<<std::endl;

				mv = &(ms->getEndVertex());
				ofs<<mv->start();
				for( k=0;k<DIMS-1;k++){
					ofs<<" "<<mv->getPosition(k);//x,y,z
				}
				ofs<<" " <<mv->during() ;//During

				for( k=0;k<DIMS-1;k++){
					ofs <<" " <<mv->getVelocity(k) ;//VX,VY,VZ
				}
				ofs<<std::endl;
			}
		}

		ofs.close();
		std::locale::global(_loc); 
	}

	static void writePoints (const std::basic_string<ELEMENT,TRAITS,ALLOC> & filename ,const MovingPoints<DataType,DIMS> & points){
		//1. Objects Number
		//2. Object ID,Time, X,Y,Z,During,VX,VY,VZ
		std::locale _loc = std::locale::global(std::locale(""));

		std::basic_ofstream<ELEMENT,TRAITS> ofs;
		ofs.open(filename.c_str(),std::ios_base::out|std::ios_base::trunc); 
		movingpoint_type * mv=0;
		int k,i;
		if (ofs)
		{				
			ofs<< points.size() << std::endl;

			for( i=0;i<points.size();i++)
			{
				mv =  (movingpoint_type *) points.get(i);
				ofs<<i<<std::endl;//Object ID 
				ofs<<mv->start();
				for( k=0;k<DIMS-1;k++){
					ofs<<" "<<mv->getPosition(k);//x,y,z
				}
				ofs<<" "<<mv->during() ;//During

				for( k=0;k<DIMS-1;k++){
					ofs <<" " <<mv->getVelocity(k) ;//VX,VY,VZ
				}
				ofs<<std::endl;
			}
		}

		ofs.close();
		std::locale::global(_loc); 

	}

	static void exportXYZ (const std::basic_string<ELEMENT,TRAITS,ALLOC> & filename ,const MovingPoints<DataType,DIMS> & points){
		//1. Objects Number
		//2. Object ID,Time, X,Y,Z,During,VX,VY,VZ
		std::locale _loc = std::locale::global(std::locale(""));

		std::basic_ofstream<ELEMENT,TRAITS> ofs;
		ofs.open(filename.c_str(),std::ios_base::out|std::ios_base::trunc); 
		movingpoint_type * mv=0;
		int k,i;
		if (ofs)
		{				
			ofs<< points.size() << std::endl;

			for( i=0;i<points.size();i++)
			{
				mv =  (movingpoint_type *) points.get(i);
				for( k=0;k<DIMS-1;k++){
					ofs<<" "<<mv->getPosition(k);//x,y,z
				}
				ofs<<std::endl;
			}
		}

		ofs.close();
		std::locale::global(_loc); 

	}
};

end_gtl_namespace
