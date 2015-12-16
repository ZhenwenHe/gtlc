// gtl_test_dtree.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h" 
#include <vector>
#include <string>
 
#include "decompositiontree.h"
#include "cbplustree.h"
#include "rstartree.h"

 
#include "timer.h"  
#include "bufferedfile.h"
#include "cachemanager.h"
#include "tprtree.h"
#include "spatiotemporalgenerator.h"

extern std::string _database;
extern void generate_random_points(const int maxpoints, const double minv[], const double maxv[] ,gtl::MovingPoints<double,4> & pnts);
extern unsigned long long  calculateNodes(int n);
extern gtl::Console<char> g_console;
extern gtl::Envelope<double,4> _cubeenvelope;

struct dtree_traits{
	static const int  capacity = 128;//maximum number permitted in each leaf
	static const int  growth = 1;//growth number of object array of leaf
	static const int  shape = gtl::DT_POINTS; 
	static const int velocity=0;// not use velocity
};

template <typename _Key>
struct btree_traits
{
	/// If true, the tree will self verify it's invariants after each insert()
	/// or erase(). The header must have been compiled with BTREE_DEBUG defined.
	static const bool   selfverify = false;

	/// If true, the tree will print out debug information and a tree dump
	/// during insert() or erase() operation. The header must have been
	/// compiled with BTREE_DEBUG defined and key_type must be std::ostream
	/// printable.
	static const bool   debug = false;

	/// Number of slots in each leaf of the tree. Estimated so that each node
	/// has a size of about 256 bytes.
	static const int    leafslots = 128;

	/// Number of slots in each inner node of the tree. Estimated so that each node
	/// has a size of about 256 bytes.
	static const int    innerslots = 128;
};

typedef gtl::DecompositionTree<gtl::MovingVertex<double,4>,double,4,dtree_traits> DTree;
typedef gtl::STCBPlusTrees<double,4,btree_traits<double> > STCBPlusTree;
typedef gtl::RStarTree<gtl::MovingSegment<double,4>*,4,dtree_traits::capacity/2,dtree_traits::capacity> RSTree ;

void test_query_performace(const std::string& filename)
{
	unsigned long long nodenumber=0;
	unsigned long long pointnumber=0;
	unsigned int runtimes =10;

	unsigned long long identifier=0;
	unsigned long long order=0;

	size_t i,j,c,n;

	gtl::Envelope<double,4> total ;

	gtl::MovingVertex<double,4> * pmv =0;
	gtl::MovingSegment<double,4>* pms=0;

	gtl::MovingEnvelope<double,4> me;

	gtl::Timer<char> timer;
	std::ofstream ofs(_database+"test_query_performace"+filename+".txt");

	gtl::Envelopes<double,4> queryEnvelopes;
	int tn =500;
	int ts=10;

	//1. setup the d-tree
	{

		gtl::Trajectories<double,4> trajectories;
		//trajectories.read(_database+filename+".mtf");
		gtl::SpatiotemporalReader<double,4>::readTrajectories(_database+filename+".mtf",trajectories);
		total = trajectories.getEnvelope();
		total.setLowerBound(3,0.0);
		total.setUpperBound(3,100.0);
		DTree itree(total); 
		g_console.print( "setup the d-tree\n");
		timer.start(" ");
		n = trajectories.size();
		for(i=0;i<n;i++)
		{
			c = trajectories.get(i)->size();
			identifier = i;
			for(j=0;j<c;j++){
				order = j;
				pmv = trajectories.get(i,j);
				itree.insert(identifier,order,pmv->getCenter(),pmv,0);
			}
		} 
		timer.end();
		itree.prepare();
		itree.getObjectNumber(pointnumber,nodenumber);
		
		ofs<< " " <<pointnumber<<" " <<nodenumber;
		timer.write(ofs);
		timer.clear();

		//query times
		for(int t = ts;t<=tn;t+=10)
		{
			g_console.print("%d\n",t);
			queryEnvelopes.read(_database+"envelopes"+gtl::numberToString<char,int>(t)+".ef");
			queryEnvelopes.transform(_cubeenvelope,total);
			//DTree::object_ptr_vector results;
			std::map<unsigned long long ,std::vector<gtl::MovingVertex<double,4> *> > results;
			DTree::envelope_vector::iterator it = queryEnvelopes.begin();
			timer.start("\n");
			for(;it!=queryEnvelopes.end();it++)
			{
				itree.search(*it,results);
				results.clear();
			}
			timer.end();
		}
		timer.write(ofs);
		timer.clear();
		ofs<<std::endl;

	}

	{
			
		gtl::MovingSegments<double,4> segments;
		//segments.read(_database+filename+".msf");
		gtl::SpatiotemporalReader<double,4>::readSegments(_database+filename+".msf",segments);
		segments.sortByStartTime();

		//2.setup the STCBPlusTrees 
		{
			STCBPlusTree btree;	
			g_console.print( "setup the STCBPlusTrees\n");
			timer.start(" ");
			n = segments.size();
			for(i=0;i<n;i++)
			{
				pms = segments.get(i);
				btree.insert(pms);
			} 
			timer.end();			
			btree.getObjectNumber(pointnumber,nodenumber);			
			ofs<< " " <<pointnumber<<" " <<nodenumber;
			timer.write(ofs);
			timer.clear();

			//query times
			for(int t = ts;t<=tn;t+=10)
			{
				g_console.print("%d\n",t);
				queryEnvelopes.read(_database+"envelopes"+gtl::numberToString<char,int>(t)+".ef");
				queryEnvelopes.transform(_cubeenvelope,total);

				STCBPlusTree::moving_sement_ptr_vector results;
				gtl::Envelopes<double,4>::iterator it = queryEnvelopes.begin();
				timer.start("\n");
				for(;it!=queryEnvelopes.end();it++)
				{					
					btree.search(*it,results);
					results.clear();
				}
				timer.end();				
			}
			timer.write(ofs);
			timer.clear();
			ofs<<std::endl;
		}
	
		//3. setup rstartree 	
		{
			//RSTree rstree;
			//gtl::Console<char>::print( "setup rstartree\n");
			//timer.start("");
			//n = segments.size();
			//for(i =0;i<n;i++ )
			//{
			//	pms = segments.get(i);
			//	pms->getEnvelope().getData(minv,maxv);
			//	rstree.insert(minv,maxv,pms);
			//}
			//timer.end();
			//
			//rstree.getObjectNumber(pointnumber,nodenumber);
			//ofs<< " " <<pointnumber<<" "<<nodenumber;
			//timer.write(ofs);
			//timer.clear();

			////query times 
			//for(int t = ts;t<=tn;t+=10)
			//{
			//	gtl::Console<char>::print("%d\n",t);
			//	queryEnvelopes.read(_database+"envelopes"+gtl::numberToString<char,int>(t)+".ef");
			//	std::vector<gtl::MovingSegment<double,4>* > results;
			//	gtl::Envelopes<double,4>::iterator it = queryEnvelopes.begin();
			//	double minv[4],maxv[4];
			//	timer.start("\n");
			//	for(;it!=queryEnvelopes.end();it++)
			//	{
			//		it->getLowerBounds(minv);
			//		it->getUpperBounds(maxv); 
			//		rstree.search(minv,maxv,results);
			//		results.clear();
			//	}
			//	timer.end();
			//}

			//timer.write(ofs); 
			//timer.clear();

			//ofs<<std::endl;

		}
	
		//4.setup tpr-tree
		{
			/*gtl::TPRTree<gtl::MovingSegment<double,4>,double,4> tprtree;
			gtl::Console<char>::print( "setup tpr-tree\n");
			timer.start(" ");
			for(i =0;i<segments.size();i++ )
			{
			pms = segments.get(i);
			me=pms->getMovingEnvelope();
			tprtree.insert(me,pms);
			} 
			timer.end();
			tprtree.getObjectNumber(pointnumber,nodenumber);
			ofs<< " " <<pointnumber<<" "<<nodenumber;*/
		}
	}

	ofs.close();
}