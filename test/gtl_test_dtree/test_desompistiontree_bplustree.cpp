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
	static const int  capacity = 32;//maximum number permitted in each leaf
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
	static const int    leafslots = 32;

	/// Number of slots in each inner node of the tree. Estimated so that each node
	/// has a size of about 256 bytes.
	static const int    innerslots = 32;
};

void test_dtree_rstartree_stbtree_points(const std::string& filename)
{
	gtl::MovingPoints<double,4> vss;
	//vss.read(_database+filename);
	gtl::SpatiotemporalReader<double,4>::readPoints(_database+filename,vss);
	gtl::Envelope<double,4> total = vss.getEnvelope();

	unsigned long long nodenumber=0;
	unsigned long long pointnumber=0;
	unsigned int runtimes =1000;

	unsigned long long identifier=0;
	unsigned long long order=0;

	double curpos[4];
	gtl::Envelope<double,4> qe;
	for(int i=0;i<4;i++)
		curpos[i]=rand()%10000;
	qe=gtl::Envelope<double,4>(curpos,1000);

	gtl::Timer<char> timer;
	std::ofstream ofs(_database+"dtree_rstartree_bplustree_"+filename+".txt"); 

	ofs<<vss.size();	

	{
		gtl::DecompositionTree<gtl::MovingPoint<double,4>,double,4,dtree_traits> itree(total); 
		timer.start(" ");
		for(int it =0;it<vss.size();it++ )
		{
			order = it;
			itree.insert(identifier,order,vss.get(it)->getCenter(),vss.get(it),0);
		} 
		timer.end();
		itree.prepare();
		itree.getObjectNumber(pointnumber,nodenumber);

		ofs<< " " <<pointnumber<<" " <<nodenumber<<" "<<itree.getDepth()<<" "<<calculateNodes(itree.getDepth());


		std::vector< gtl::MovingPoint<double,4>* > results;
		int mm=0;

		timer.start(" ");
		for(int k=0;k<runtimes;k++)
		{
			itree.search(qe,results);
			if(k==0)
				mm=results.size();
			results.clear();
		}		
		timer.end(); 

		ofs<<" "<<mm;

	}

	{
		double minv[4];
		double maxv[4];
		std::vector<gtl::MovingPoint<double,4>*> results; 
		gtl::RStarTree<gtl::MovingPoint<double,4>*,4,dtree_traits::capacity/2,dtree_traits::capacity> rstree;			 
		timer.start("");
		for(int it =0;it<vss.size();it++ )
		{
			vss.get(it)->getEnvelope().getData(minv,maxv);
			rstree.insert(minv,maxv,vss.get(it));
		}
		timer.end();

		rstree.getObjectNumber(pointnumber,nodenumber);

		ofs<< " " <<pointnumber<<" "<<nodenumber;

		qe.getLowerBounds(minv);
		qe.getUpperBounds(maxv); 
		int mm=0;
		timer.start(" ");			 
		for(int k=0;k<runtimes;k++)
		{
			rstree.search(minv,maxv,results);
			if(k==0)
				mm=results.size();
			results.clear();
		} 
		timer.end(); 

		ofs<<" "<<mm;

	}


	{
		
		std::vector<gtl::MovingPoint<double,4>*> results; 
		gtl::CompressedBPlusTrees<double,gtl::MovingPoint<double,4>*,4, btree_traits<double> > btrees;			 
		
		timer.start("");
		std::vector< std::pair<double,gtl::MovingPoint<double,4>* > >timed_vss(vss.size());
		for(int it =0;it<vss.size();it++ )
		{
			timed_vss[it].first = vss.get(it)->start();
			timed_vss[it].second = vss.get(it);
		}
		btrees.insert(0,timed_vss);
		for(int it =0;it<vss.size();it++ )
		{
			for(int d =0;d<3;d++)
				btrees.insert(d+1,vss.get(it)->getPosition(d),vss.get(it));
		}
		timer.end();

		int mm=0;
		timer.start(" ");			 
		for(int k=0;k<runtimes;k++)
		{
			btrees.search(qe,results);
			if(k==0)
				mm=results.size();
			results.clear();
		} 
		timer.end(); 

		ofs<<" "<<mm;

	}

	timer.write(ofs);
	timer.clear();
	ofs<<std::endl;

	vss.clear();	
	ofs.close();
}

void test_dtree_rstartree_bplustree_random_points_500_35000()
{
	gtl::Timer<char> timer;
	std::ofstream ofs(_database+"dtree_rstartree_bplustree_random_points_500_35000.txt"); 
	double minData[4] = {0,0,0,0};
	double maxData[4] = {10240,10240,10240,10240};
	gtl::Envelope<double,4> total;
	total.set(minData,maxData);
	double curpos[4]={5120,5120,5120,5120};
	gtl::Envelope<double,4> qe;
	unsigned long long nodenumber=0;
	unsigned long long pointnumber=0;
	unsigned int runtimes =1000;
	unsigned long long identifier=0;
	unsigned long long order=0;

	for (int j=5000;j<32768;j+=5000)
	{
		Sleep(1000*5);

		g_console.print("%d\n",j);
		ofs<<j;//the number of the objects

		gtl::MovingPoints<double,4> vss;
		generate_random_points(j,minData,maxData,vss);


		for(int i=0;i<4;i++)
			curpos[i]=rand()%10240;

		qe=gtl::Envelope<double,4>(curpos,100);

		{
			gtl::DecompositionTree<gtl::MovingPoint<double,4>,double,4,dtree_traits> itree(total); 
			timer.start(" ");
			for(int it =0;it<vss.size();it++ )
			{
				order=it;
				itree.insert(identifier,order,vss.get(it)->getCenter(),vss.get(it),0);
			} 
			timer.end();
			itree.prepare();
			itree.getObjectNumber(pointnumber,nodenumber);

			ofs<< " " <<pointnumber<<" " <<nodenumber<<" "<<itree.getDepth()<<" "<<calculateNodes(itree.getDepth());


			std::vector< gtl::MovingPoint<double,4>* > results;
			int mm=0;

			timer.start(" ");
			for(int k=0;k<runtimes;k++)
			{
				itree.search(qe,results);
				if(k==0)
					mm=results.size();
				results.clear();
			}		
			timer.end(); 

			ofs<<" "<<mm;

		}

		{
			double minv[4];
			double maxv[4];
			std::vector<gtl::MovingPoint<double,4>*> results; 
			gtl::RStarTree<gtl::MovingPoint<double,4>*,4,dtree_traits::capacity/2,dtree_traits::capacity> rstree;			 
			timer.start("");
			for(int it =0;it<vss.size();it++ )
			{
				vss.get(it)->getEnvelope().getData(minv,maxv);
				rstree.insert(minv,maxv,vss.get(it));
			}
			timer.end();

			rstree.getObjectNumber(pointnumber,nodenumber);

			ofs<< " " <<pointnumber<<" "<<nodenumber;

			qe.getLowerBounds(minv);
			qe.getUpperBounds(maxv); 
			int mm=0;
			timer.start(" ");			 
			for(int k=0;k<runtimes;k++)
			{
				rstree.search(minv,maxv,results);
				if(k==0)
					mm=results.size();
				results.clear();
			} 
			timer.end(); 

			ofs<<" "<<mm;

		}

		{

			std::vector<gtl::MovingPoint<double,4>*> results; 
			gtl::CompressedBPlusTrees<double,gtl::MovingPoint<double,4>*,4,btree_traits<double> > btrees;
			
			timer.start("");
			std::vector< std::pair<double,gtl::MovingPoint<double,4>* > >timed_vss(vss.size());
			for(int it =0;it<vss.size();it++ )
			{
				timed_vss[it].first = vss.get(it)->start();
				timed_vss[it].second = vss.get(it);
			}
			btrees.insert(0,timed_vss);
			for(int it =0;it<vss.size();it++ )
			{
				//btrees.insert(0,vss.get(it)->start(),vss.get(it));
				for(int d =0;d<3;d++)
					btrees.insert(d+1,vss.get(it)->getPosition(d),vss.get(it));
			}
			timer.end(); 

			btrees.getObjectNumber(pointnumber,nodenumber);
			ofs<< " " <<pointnumber<<" "<<nodenumber;

			int mm=0;
			timer.start(" ");			 
			for(int k=0;k<runtimes;k++)
			{
				btrees.search(qe,results);
				if(k==0)
					mm=results.size();
				results.clear();
			} 
			timer.end(); 

			ofs<<" "<<mm;

		}


		{
			
			gtl::TPRTree<gtl::MovingPoint<double,4>,double,4> tprtree;
			gtl::MovingEnvelope<double,4> me;
			gtl::MovingPoint<double,4> * pnt =0;

			vss.sortByTime();

			timer.start(" ");
			for(int it =0;it<vss.size()-1;it++ )
			{
				pnt = vss.get(it);
				me.start(pnt->start()); 
				me.end(DBL_MAX);
				memcpy((void*)(me.getLowPosition()),pnt->getPosition(),sizeof(double)*3);
				memcpy((void*)(me.getHighPosition()),pnt->getPosition(),sizeof(double)*3);
				memcpy((void*)(me.getLowVelocites()),pnt->getVelocity(),sizeof(double)*3);
				memcpy((void*)(me.getHighVelocites()),pnt->getVelocity(),sizeof(double)*3);
				tprtree.insert(me,pnt);
			} 
			timer.end();


			tprtree.getObjectNumber(pointnumber,nodenumber);

			ofs<< " " <<pointnumber<<" "<<nodenumber;

			std::vector<gtl::MovingPoint<double,4>*> results;
			int mm=0; 
			me.set(qe);

			double current_time = vss.get(vss.size()-1)->start()+10;
			double h = 20;

			me.start(current_time+1);
			me.end(current_time + h-1);

			timer.start(" ");
			for(int k=0;k<runtimes;k++)
			{
			tprtree.search(me,results);
			if(k==0)
			mm=results.size();
			results.clear();
			}		
			timer.end(); 

			ofs<<" "<<mm;

		}


		timer.write(ofs);
		timer.clear();
		ofs<<std::endl;

		vss.clear();		
	}

	ofs.close();
}

void test_desompistiontree_bplustree()
{
	test_dtree_rstartree_bplustree_random_points_500_35000();
	test_dtree_rstartree_stbtree_points("random.mpf");
	test_dtree_rstartree_stbtree_points("delft.mpf");
}

