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

template <class OBJECTTYPE,int NUMDIMS> 
struct default_decomposition_tree_trait
{	
	static const int  velocity=1;// not use velocity
	static const int  capacity = 32;//maximum number permitted in each leaf
	static const int  growth = 1;//growth number of object array of leaf
	static const int  shape = gtl::DT_POINTS; 
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
	static const int    leafslots = 32;

	/// Number of slots in each inner node of the tree. Estimated so that each node
	/// has a size of about 256 bytes.
	static const int    innerslots = 32;
};

typedef default_decomposition_tree_trait<gtl::MovingVertex<double,4>,4> dtree_traits;
typedef gtl::DecompositionTree<gtl::MovingVertex<double,4>,double,4,dtree_traits> DTree;
typedef gtl::STCBPlusTrees<double,4,btree_traits<double> > STCBPlusTree;
typedef gtl::RStarTree<gtl::MovingSegment<double,4>*,4,dtree_traits::capacity/2,dtree_traits::capacity> RSTree ;
typedef gtl::TPRTree<gtl::MovingPoint<double,4>,double,4> TPRtree;

void test_for_future(const std::string& filename)
{
	unsigned long long nodenumber=0;
	unsigned long long pointnumber=0;
	unsigned int runtimes =10;

	unsigned long long identifier=0;
	unsigned long long order=0;

	size_t i,j,c,n;

	assert(dtree_traits::velocity==1);

	double curpos[4],minv[4],maxv[4];

	double currentTime=0;
	
	gtl::Envelope<double,4> total ;

	gtl::MovingVertex<double,4> * pmv =0;
	gtl::MovingSegment<double,4>* pms=0;

	gtl::MovingEnvelope<double,4> me;

	gtl::Timer<char> timer;
	std::ofstream ofs(_database+"test_for_future"+filename+".txt");

	gtl::Envelopes<double,4> queryEnvelopes;
	int tn =500;
	int ts=10;

	//1. setup the d-tree
	{

		gtl::Trajectories<double,4> trajectories;
		//trajectories.read(_database+filename+".mtf");
		gtl::SpatiotemporalReader<double,4>::readTrajectories( _database+filename+".mtf",trajectories);
		total = trajectories.getEnvelope();
		gtl::DecompositionTree<gtl::MovingVertex<double,4>,double,4,dtree_traits>   itree(total); 
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
				itree.insert(identifier,order,pmv->getCenter(),pmv->getVelocity(),pmv,0);
			}
		} 
		timer.end();
		itree.prepare();
		itree.getObjectNumber(pointnumber,nodenumber);
		
		ofs<< " " <<pointnumber<<" " <<nodenumber;
		timer.write(ofs);
		timer.clear();
		currentTime= itree.getCurrentTime();
		//query times
		for(int t = ts;t<=tn;t+=10)
		{
			g_console.print("%d\n",t);
			queryEnvelopes.read(_database+"envelopes"+gtl::numberToString<char,int>(t)+".ef");
			//DTree::object_ptr_vector results;
			//std::map<unsigned long long ,std::vector<gtl::MovingVertex<double,4> *> > results;
			DTree::entry_ptr_vector results;
			DTree::envelope_vector::iterator it = queryEnvelopes.begin();
			timer.start("\n");
			for(;it!=queryEnvelopes.end();it++)
			{
				it->setLowerBound(0,currentTime+1);
				it->setUpperBound(0,currentTime+19);

				itree.search(*it,results);
				results.clear();
			}
			timer.end();
		}
		timer.write(ofs);
		timer.clear();
		ofs<<std::endl;

	}

	//2.setup tpr-tree
	{
		TPRtree tprtree;
		gtl::MovingEnvelope<double,4> me;
		gtl::MovingPoint<double,4> * pnt =0;
		gtl::MovingPoints<double,4> vss;
		//vss.read(_database+filename+".mpf");
		gtl::SpatiotemporalReader<double,4>::readPoints(_database+filename+".mpf",vss);
		vss.sortByTime();

		int s = vss.size();
		timer.start(" ");
		for(int it =0;it<s;it++ )
		{
			pnt = vss.get(it);
			me.start(pnt->start());
			//me.end(vss.get(it+1)->start());
			me.end(DBL_MAX);
			memcpy((void*)(me.getLowPosition()),pnt->getPosition(),sizeof(double)*3);
			memcpy((void*)(me.getHighPosition()),pnt->getPosition(),sizeof(double)*3);
			memcpy((void*)(me.getLowVelocites()),pnt->getVelocity(),sizeof(double)*3);
			memcpy((void*)(me.getHighVelocites()),pnt->getVelocity(),sizeof(double)*3);
			tprtree.insert(me,pnt);
		} 
		timer.end();
		tprtree.getObjectNumber(pointnumber,nodenumber);
		
		ofs<< " " <<pointnumber<<" " <<nodenumber;
		timer.write(ofs);
		timer.clear();
		
		//query times
		for(int t = ts;t<=tn;t+=10)
		{
			g_console.print("%d\n",t);
			queryEnvelopes.read(_database+"envelopes"+gtl::numberToString<char,int>(t)+".ef");
			std::vector<gtl::MovingPoint<double,4>*> results;

			DTree::envelope_vector::iterator it = queryEnvelopes.begin();
			timer.start("\n");
			for(;it!=queryEnvelopes.end();it++)
			{
				it->setLowerBound(0,currentTime+1);
				it->setUpperBound(0,currentTime+19);
				me.set(*it);
				tprtree.search(me,results);
				results.clear();
			}
			timer.end();
		}
		timer.write(ofs);
		timer.clear();
		ofs<<std::endl;
	}
	

	ofs.close();
}