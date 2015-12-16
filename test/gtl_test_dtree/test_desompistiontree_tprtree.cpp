// gtl_test_dtree.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h" 
#include <vector>
#include <string>
#include "timer.h"
 
#include "decompositiontree.h"
#include "bufferedfile.h"
#include "cachemanager.h"
#include "tprtree.h"
#include "spatiotemporalgenerator.h"

extern std::string _database;
extern void generate_random_points(const int maxpoints, const double minv[], const double maxv[] ,gtl::MovingPoints<double,4> & pnts);
extern unsigned long long  calculateNodes(int n);

struct dtree_traits{
	static const int  capacity = 64;//maximum number permitted in each leaf
	static const int  growth = 1;//growth number of object array of leaf
	static const int  shape = gtl::DT_POINTS; 
	static const int velocity=1;// not use velocity
};
void test_desompistiontree_tprtree(const std::string& filename)
{
	gtl::MovingPoints<double,4> vss;
	//vss.read(_database+filename);
	gtl::SpatiotemporalReader<double,4>::readPoints(_database+filename,vss);
	vss.sortByTime();

	

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
	qe=gtl::Envelope<double,4>(curpos,100);

	gtl::Timer<char> timer;
	std::ofstream ofs(_database+"dtree_tprtree_"+filename+".txt"); 

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
		//gtl::TPRTree<gtl::MovingPoint<double,4>,double,4> tprtree(_database);
		gtl::TPRTree<gtl::MovingPoint<double,4>,double,4> tprtree;
		gtl::MovingEnvelope<double,4> me;
		gtl::MovingPoint<double,4> * pnt =0;
		

		timer.start(" ");
		for(int it =0;it<vss.size()-1;it++ )
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


		std::vector<gtl::MovingPoint<double,4>*> results;
		int mm=0; 
		me.set(qe);

		double current_time = vss.get(vss.size()-1)->start();
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
	ofs.close();
}


