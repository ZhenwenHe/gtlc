#include "stdafx.h" 
#include <vector>
#include <string>
#include "timer.h"
#include "spatiotemporalgenerator.h"
#include "RStarTree.h"
#include "decompositiontree.h"
#include "console.h"

extern std::string _database;
extern gtl::Console<char> g_console;
extern void generate_random_points(const int maxpoints, const double minv[], const double maxv[] ,gtl::MovingPoints<double,4> & pnts);
extern unsigned long long  calculateNodes(int n);

struct dtree_traits{
	static const int  capacity = 32;//maximum number permitted in each leaf
	static const int  growth = 1;//growth number of object array of leaf
	static const int  shape = gtl::DT_POINTS; 
	static const int velocity=0;// not use velocity
};


void test_dtree_rstartree_random_points_500_35000()
{
	gtl::Timer<char> timer;
	std::ofstream ofs(_database+"test_dtree_rstartree_random_points_500_35000.txt"); 
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

	for (int j=500;j<32768;j+=500)
	{
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

		timer.write(ofs);
		timer.clear();
		ofs<<std::endl;

		vss.clear();
	}

	ofs.close();
}

void test_dtree_rstartree_points(const std::string& filename)
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
	qe=gtl::Envelope<double,4>(curpos,100);

	gtl::Timer<char> timer;
	std::ofstream ofs(_database+"dtree_rstartree"+filename+".txt"); 

	ofs<<vss.size();	

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

	timer.write(ofs);
	timer.clear();
	ofs<<std::endl;

	vss.clear();	
	ofs.close();
}


void test_desompistiontree_rstartree(){
	test_dtree_rstartree_random_points_500_35000();
	test_dtree_rstartree_points("random.mpf");
	test_dtree_rstartree_points("delft.mpf");
}
