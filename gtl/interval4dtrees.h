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
#pragma once
#include "omp.h" 
#include "buffer.h"
#include "vertex.h"
#include "envelope.h"
#include "interval.h"
#include "tdtree.h"
#include "TriTree.h" 

begin_gtl_namespace 

template< typename IntervalTreeType> 
class Interval4dTrees
{
	std::vector<Interval4d*>      _objects;
	IntervalTreeType * _trees[4];
public:
	inline IntervalTreeType * getTree(int dim) {return _trees[dim];}

	std::vector<Interval4d*> & getObjects(){return _objects;}
	Interval4dTrees(double minValueT, double maxValueT, unsigned int maxNumberInBlockT,
		double minValueX, double maxValueX, unsigned int maxNumberInBlockX,
		double minValueY, double maxValueY, unsigned int maxNumberInBlockY,
		double minValueZ, double maxValueZ, unsigned int maxNumberInBlockZ)
	{
		_trees[0] = new IntervalTreeType(minValueT,maxValueT,maxNumberInBlockT);
		_trees[1] = new IntervalTreeType(minValueX,maxValueX,maxNumberInBlockX);
		_trees[2] = new IntervalTreeType(minValueY,maxValueY,maxNumberInBlockY);
		_trees[3] = new IntervalTreeType(minValueZ,maxValueZ,maxNumberInBlockZ);

		for(short i=0;i<4;i++)
			_trees[i]->setDimensionalOrder(i);

		omp_set_num_threads(4);
	}

	~Interval4dTrees()
	{
		for(int i=0;i<4;i++) 
			delete _trees[i];

		for(std::vector<Interval4d*>::iterator it = _objects.begin();it!=_objects.end();it++)
		{	
			if(*it)
				delete (*it);
		}
		_objects.clear();
	}
	inline void insert(std::vector<Interval4d*> & vs)
	{
		size_t s = _objects.size();
		size_t c = vs.size();
		_objects.resize(s+c);
		int i,j;

#pragma omp parallel for
		for( i=0;i<c;i++)
			_objects[i+s]=vs[i];

		
#pragma omp parallel for 
		for( i=0;i<4;i++)
		{
			for(j=0;j<c;j++)
				_trees[i]->insert(&(vs[j]->get(i)));
		}

	}
	void insert(Interval4d * p)
	{
		_objects.push_back(p);
#pragma omp parallel for 
		for(int i=0;i<4;i++)
		{
			//printf("section %d=%d\n",i,omp_get_thread_num());
			_trees[i]->insert(&(p->get(i)));
		}
		//#pragma omp parallel 
		//	{
		//#pragma opm section
		//		{
		//			printf("section 0=%d\n",omp_get_thread_num());
		//			_trees[0]->insert(&(p->get(0)));
		//		}
		//#pragma opm section
		//		{
		//			printf("section 1=%d\n",omp_get_thread_num());
		//			_trees[1]->insert(&(p->get(1)));
		//		}
		//#pragma opm section
		//		{
		//			printf("section 2=%d\n",omp_get_thread_num());
		//			_trees[2]->insert(&(p->get(2)));
		//		}
		//#pragma opm section
		//		{
		//			printf("section 3=%d\n",omp_get_thread_num());
		//			_trees[3]->insert(&(p->get(3)));
		//		}
		//	}
	}

	
	int search(Envelope2d e2d[4],std::vector<Interval4d*> & results)
	{
#pragma omp parallel for 
		for(int i=0;i<4;i++){ 
			_trees[i]->search(e2d[i]); 			
		}

		for(std::vector<Interval4d*>::iterator it = _objects.begin();it!=_objects.end();it++)
			if((*it)->flags()==0x01010101)
				results.push_back(*it);
		return results.size();
	}

	void search(Envelope2d e2d[4], int runtimes=1)
	{
		int j=0;
		int i=0;

#pragma omp parallel for 
		for( i=0;i<4;i++)
		{
			//while(j<runtimes)
			//{
				_trees[i]->search(e2d[i]);
			//	j++;
			//}
		}

		//size_t s = _objects.size();

//#pragma omp parallel for
//		for( i=0;i<s;i++)
//		{
//			if(_objects[i]->flags()==0x01010101)
//			{
//				// output the result;
//			}
//		}
	}
};
end_gtl_namespace 
