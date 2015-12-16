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

extern gtl::Console<char> g_console;

extern void generate_random_points(const int maxpoints, const double minv[], const double maxv[] ,gtl::MovingPoints<double,4> & pnts);
extern unsigned long long  calculateNodes(int n);

struct dtree_traits{
	static const int  capacity = 128;//maximum number permitted in each leaf
	static const int  growth = 1;//growth number of object array of leaf
	static const int  shape = gtl::DT_POINTS; 
	static const int  velocity=0;// not use velocity
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

 
void test_dtree_bplustree_brinkhoff_100000_1000000()
{
 
	std::ofstream ofs(_database+"test_dtree_bplustree_brinkhoff_100000_1000000.txt"); 
	 
	gtl::Envelope<double,4> total; 
 
	unsigned long long nodenumber=0;
	unsigned long long pointnumber=0;

 
	unsigned long long identifier=0;
	unsigned long long order=0;

	for (uint64_t j=100000;j<=1000000;j+=100000)
	{
		Sleep(1000*5);

		g_console.print("%ld\n",j);

		ofs<<j;//the number of the objects

		gtl::MovingPoints<double,4> vss;
		gtl::SpatiotemporalReader<double,4>::readPoints(_database+"oldenburg"+gtl::numberToString<char,uint64_t>(j)+".mpf", vss);
		total = vss.getEnvelope();
		total.setLowerBound(3,0.0);
		total.setUpperBound(3,100.0);

		{
			gtl::DecompositionTree<gtl::MovingPoint<double,4>,double,4,dtree_traits> itree(total); 

			for(size_t it =0;it<vss.size();it++ )
			{
				order=it;
				itree.insert(identifier,order,vss.get(it)->getCenter(),vss.get(it),0);
			} 
			itree.getObjectNumber(pointnumber,nodenumber);
			ofs<< " " <<pointnumber<<" " <<nodenumber<<" "<<itree.getDepth()<<" "<<calculateNodes(itree.getDepth());
		}
	
		{

			std::vector<gtl::MovingPoint<double,4>*> results; 
			gtl::CompressedBPlusTrees<double,gtl::MovingPoint<double,4>*,4,btree_traits<double> > btrees;
			
			std::vector< std::pair<double,gtl::MovingPoint<double,4>* > >timed_vss(vss.size());
			for(size_t it =0;it<vss.size();it++ )
			{
				timed_vss[it].first = vss.get(it)->start();
				timed_vss[it].second = vss.get(it);
			}
			btrees.insert(0,timed_vss);
			for(size_t it =0;it<vss.size();it++ )
			{
				//btrees.insert(0,vss.get(it)->start(),vss.get(it));
				for(int d =0;d<3;d++)
					btrees.insert(d+1,vss.get(it)->getPosition(d),vss.get(it));
			}
			
			btrees.getObjectNumber(pointnumber,nodenumber);
			ofs<< " " <<pointnumber<<" "<<nodenumber;
		}		

		ofs<<std::endl;
		vss.clear();		
	}

	ofs.close();
}

 
