// gtl_test_interval4dtrees.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <vector>
#include <string>
#include "timer.h"
#include "console.h"
#include "TriTree.h"
#include "interval4dtrees.h"
#include "rtree.h"
#include "RStarTree.h"
#include "decompositiontree.h" 
#include "spatiotemporalgenerator.h"

std::string _database = "d:\\gtl\\data\\";

gtl::Console<char> g_console;

void generate_new_data(int n, std::vector<gtl::Interval4d*> & vss)
{
	double minv[4];
	double maxv[4];

	gtl::Interval4d::calculateEnvelope(vss,minv,maxv);

	unsigned int s = vss.size();
	for(int i=0;i<s;i++)
	{
		for(int j=1;j<n;j++)
		{
			gtl::Interval4d* pi4d = new gtl::Interval4d(*(vss[i]));
			for(int k=0;k<4;k++)
			{
				pi4d->get(k)+=((maxv[k]-minv[k])*j);
			}
			vss.push_back(pi4d);
		}
	}
}

void test_interval4dtrees()
{
	time_t now ;
	::time(&now);
	tm   t;
	::localtime_s(&t,&now); 
	std::string sz = gtl::numberToString<char,long>(t.tm_year+1900)+"-"+
		gtl::numberToString<char,long>(t.tm_mon+1)+"-"+
		gtl::numberToString<char,long>(t.tm_mday)+"-"+
		gtl::numberToString<char,long>(t.tm_hour)+"-"+
		gtl::numberToString<char,long>(t.tm_min)+"-"+
		gtl::numberToString<char,long>(t.tm_sec);

	std::ofstream ofs(sz+".txt"); 



	gtl::Timer<char> timer;
	gtl::Envelope2d e2d4[4];

	e2d4[0]=gtl::Envelope2d(1000,3000,1000,3000);
	e2d4[1]=gtl::Envelope2d(1000,3000,1000,3000);
	e2d4[2]=gtl::Envelope2d(1000,3000,1000,3000);
	e2d4[3]=gtl::Envelope2d(1000,3000,1000,3000);

	double minv[4];
	double maxv[4];
	std::vector<gtl::Interval4d*> vss;

	int search_operation_repeated_number=10000;


	for(unsigned int i=1000;i<31000;i+=1000)
	{

		gtl::RTree<gtl::Interval4d*,double> rtree;

		gtl::RStarTree<gtl::Interval4d*,4,4,8> rstree;

		gtl::Interval4dTrees<gtl::TriTree> itree(
			0,10240,20,0,10240,20,0,10240,20,0,10240,20);


		ofs<<i;//Êý¾Ý¸öÊý

		g_console.print("generate %d intervals\n",i);
		gtl::Interval4d::random(i,1,10000,vss);

		g_console.print("Interval4dTrees insertion\n");
		timer.start("");//IntervalTrees¹¹ÔìÊ±¼ä		 
		itree.insert(vss);		 
		timer.end();

		g_console.print("Interval4dTrees searching operation \n");
		timer.start("");		
		itree.search(e2d4,search_operation_repeated_number);
		timer.end();
		////////////////////////////////////////////////////////////////////////////////////////////////
		g_console.print("RStarTrees insertion \n");
		timer.start("");
		for(std::vector<gtl::Interval4d*>::iterator it =vss.begin();it!=vss.end();it++ )
		{
			(*it)->get(minv,maxv);
			rstree.insert(minv,maxv,*it);
		}
		timer.end();


		minv[0]=1000;minv[1]=1000;minv[2]=1000;minv[3]=1000;
		maxv[0]=3000;maxv[1]=3000;maxv[2]=3000;maxv[3]=3000;

		g_console.print("RStarTrees searching operation \n");
		timer.start(" "); 		
		rstree.search(minv,maxv,search_operation_repeated_number);
		timer.end();

		////////////////////////////////////////////////////////////////////////////////////////////////
		g_console.print("RTrees insertion \n");
		timer.start("");
		for(std::vector<gtl::Interval4d*>::iterator it =vss.begin();it!=vss.end();it++ )
		{
			(*it)->get(minv,maxv);
			rtree.insert(minv,maxv,*it);
		}
		timer.end();

		vss.clear();

		minv[0]=1000;minv[1]=1000;minv[2]=1000;minv[3]=1000;
		maxv[0]=3000;maxv[1]=3000;maxv[2]=3000;maxv[3]=3000;

		g_console.print("RTrees searching operation \n");
		timer.start(" "); 		
		rtree.search(minv,maxv,search_operation_repeated_number);
		timer.end();

		timer.write(ofs);
		timer.clear();

		ofs<<std::endl;
	}

	ofs.close();
}

void test_tritree(){
	std::vector<gtl::Interval1d> vss;  
	gtl::Interval1dOperators op;
	op.random(100,1,10000,vss);

	gtl::TriTree itree(0,10240,20); 

	for(std::vector<gtl::Interval1d>::iterator it =vss.begin();it!=vss.end();it++ )
	{
		itree.insert(&(*it));
	} 

	gtl::Envelope2d e2d (100,10000,100,10000);
	std::vector<gtl::Interval1d*> results; 
	itree.search(e2d,results);
	int k = results.size();
}

void test_tdtree(){


	std::vector<gtl::Interval1d> vss;  
	gtl::Interval1dOperators op;
	op.random(100, 1, 10000, vss);

	gtl::TDTree itree(0,10240,20); 

	for(std::vector<gtl::Interval1d>::iterator it =vss.begin();it!=vss.end();it++ )
	{
		itree.insert(&(*it));
	} 

	gtl::Envelope2d e2d (100,10000,100,10000);
	std::vector<gtl::Interval1d*> results; 
	itree.search(e2d,results);
	int k = results.size();
}

void test_interval4dtrees_with_real_dataset()
{



	gtl::Timer<char> timer;
	gtl::Envelope2d e2d4[4];


	double minv[4];
	double maxv[4];

	std::vector<gtl::Interval4d*> vss; 
	gtl::Interval4d::readIndexFile("D:\\DATA\\wu_08_new2\\wu_08_new\\feature.fdx",vss);
	gtl::Interval4d::calculateEnvelope(vss,minv,maxv);


	unsigned int s = vss.size();
	for(int i=0;i<s;i++)
	{
		for(int j=1;j<10;j++)
		{
			gtl::Interval4d* pi4d = new gtl::Interval4d(*(vss[i]));
			for(int k=0;k<4;k++)
			{
				pi4d->get(k)+=((maxv[k]-minv[k])*j);
			}
			vss.push_back(pi4d);
		}
	}

	{
		std::ofstream ofs("real_data.txt"); 
		for(std::vector<gtl::Interval4d*>::iterator it =vss.begin();it!=vss.end();it++ )
		{
			ofs<<(*it)->get(0).start()<<" "<<	(*it)->get(0).end()<<std::endl;
		}
		ofs.close();
		return;
	}


	time_t now ;
	::time(&now);
	tm   t;
	::localtime_s(&t,&now); 
	std::string sz = gtl::numberToString<char,long>(t.tm_year+1900)+"-"+
		gtl::numberToString<char,long>(t.tm_mon+1)+"-"+
		gtl::numberToString<char,long>(t.tm_mday)+"-"+
		gtl::numberToString<char,long>(t.tm_hour)+"-"+
		gtl::numberToString<char,long>(t.tm_min)+"-"+
		gtl::numberToString<char,long>(t.tm_sec);

	std::ofstream ofs(sz+"_real_data.txt"); 



	gtl::Interval4d::calculateEnvelope(vss,minv,maxv);
	int search_operation_repeated_number=10000;
	double begins[4],lens[4];
	for(int i=0;i<4;i++)
	{
		begins[i] = minv[i];
		lens[i] = (maxv[i]-minv[i])/1000; 
	}


	gtl::RTree<gtl::Interval4d*,double> rtree;

	gtl::RStarTree<gtl::Interval4d*,4,4,8> rstree;

	gtl::Interval4dTrees<gtl::TriTree> itree(
		minv[0]-100,maxv[0]+100,20,
		minv[1]-100,maxv[1]+100,20,
		minv[2]-100,maxv[2]+100,20,
		minv[3]-100,maxv[3]+100,20);


	ofs<<vss.size();//Êý¾Ý¸öÊý



	g_console.print("Interval4dTrees insertion\n");
	timer.start("");//IntervalTrees¹¹ÔìÊ±¼ä
	for(std::vector<gtl::Interval4d*>::iterator it =vss.begin();it!=vss.end();it++ )
	{
		itree.insert(*it);		
	}
	timer.end();

	////////////////////////////////////////////////////////////////////////////////////////////////
	g_console.print("RStarTrees insertion \n");
	timer.start("");
	for(std::vector<gtl::Interval4d*>::iterator it =vss.begin();it!=vss.end();it++ )
	{
		(*it)->get(minv,maxv);
		rstree.insert(minv,maxv,*it);
	}
	timer.end();
	///////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	g_console.print("RTrees insertion \n");
	timer.start("");
	for(std::vector<gtl::Interval4d*>::iterator it =vss.begin();it!=vss.end();it++ )
	{
		(*it)->get(minv,maxv);
		rtree.insert(minv,maxv,*it);
	}
	timer.end();

	vss.clear();

	timer.write(ofs);
	timer.clear();

	ofs<<std::endl;	

	//////////////////////////////////////////////////////////////////////////////////////////////////

	for(int k=1;k<=101;k+=10)
	{

		for(int i=0;i<4;i++)
		{
			e2d4[i]=gtl::Envelope2d(
				begins[i]+lens[i],
				begins[i]+lens[i]*k,
				begins[i]+lens[i],
				begins[i]+lens[i]*k);
		}


		g_console.print("Interval4dTrees searching operation \n");
		timer.start("");		
		itree.search(e2d4,search_operation_repeated_number);
		timer.end();



		for(int i=0;i<4;i++)
		{
			minv[i]=begins[i]+lens[i];
			maxv[i]=begins[i]+lens[i]*k;
		}

		g_console.print("RStarTrees searching operation \n");
		timer.start(" "); 		
		rstree.search(minv,maxv,search_operation_repeated_number);
		timer.end();

		g_console.print("RTrees searching operation \n");
		timer.start(" "); 		
		rtree.search(minv,maxv,search_operation_repeated_number);
		timer.end();

		timer.write(ofs);
		timer.clear();

		ofs<<std::endl;	
	}

	ofs.close();
}

void test_interval4dtrees_for_large_data()
{

	time_t now ;
	::time(&now);
	tm   t;
	::localtime_s(&t,&now); 
	std::string sz = gtl::numberToString<char,long>(t.tm_year+1900)+"-"+
		gtl::numberToString<char,long>(t.tm_mon+1)+"-"+
		gtl::numberToString<char,long>(t.tm_mday)+"-"+
		gtl::numberToString<char,long>(t.tm_hour)+"-"+
		gtl::numberToString<char,long>(t.tm_min)+"-"+
		gtl::numberToString<char,long>(t.tm_sec);

	std::ofstream ofs(sz+"_large.txt"); 



	gtl::Timer<char> timer;
	gtl::Envelope2d e2d4[4];

	e2d4[0]=gtl::Envelope2d(1000,3000,1000,3000);
	e2d4[1]=gtl::Envelope2d(1000,3000,1000,3000);
	e2d4[2]=gtl::Envelope2d(1000,3000,1000,3000);
	e2d4[3]=gtl::Envelope2d(1000,3000,1000,3000);

	double minv[4];
	double maxv[4];
	std::vector<gtl::Interval4d*> vss;

	int search_operation_repeated_number=10000;


	gtl::RTree<gtl::Interval4d*,double> rtree;

	gtl::RStarTree<gtl::Interval4d*,4,4,8> rstree;

	gtl::Interval4dTrees<gtl::TriTree> itree(
		0,102400,20,0,102400,20,0,102400,20,0,102400,20);


	gtl::Interval4d::random(30000,1,10000,vss);
	generate_new_data(10,vss);
	g_console.print("generate %ld intervals\n",vss.size());
	ofs<<vss.size();//Êý¾Ý¸öÊý



	g_console.print("Interval4dTrees insertion\n");
	timer.start("");//IntervalTrees¹¹ÔìÊ±¼ä		 
	itree.insert(vss);		 
	timer.end();

	g_console.print("Interval4dTrees searching operation \n");
	timer.start("");		
	itree.search(e2d4,search_operation_repeated_number);
	timer.end();
	////////////////////////////////////////////////////////////////////////////////////////////////
	g_console.print("RStarTrees insertion \n");
	timer.start("");
	for(std::vector<gtl::Interval4d*>::iterator it =vss.begin();it!=vss.end();it++ )
	{
		(*it)->get(minv,maxv);
		rstree.insert(minv,maxv,*it);
	}
	timer.end();


	minv[0]=1000;minv[1]=1000;minv[2]=1000;minv[3]=1000;
	maxv[0]=3000;maxv[1]=3000;maxv[2]=3000;maxv[3]=3000;

	g_console.print("RStarTrees searching operation \n");
	timer.start(" "); 		
	rstree.search(minv,maxv,search_operation_repeated_number);
	timer.end();

	////////////////////////////////////////////////////////////////////////////////////////////////
	g_console.print("RTrees insertion \n");
	timer.start("");
	for(std::vector<gtl::Interval4d*>::iterator it =vss.begin();it!=vss.end();it++ )
	{
		(*it)->get(minv,maxv);
		rtree.insert(minv,maxv,*it);
	}
	timer.end();

	vss.clear();

	minv[0]=1000;minv[1]=1000;minv[2]=1000;minv[3]=1000;
	maxv[0]=3000;maxv[1]=3000;maxv[2]=3000;maxv[3]=3000;

	g_console.print("RTrees searching operation \n");
	timer.start(" "); 		
	rtree.search(minv,maxv,search_operation_repeated_number);
	timer.end();

	timer.write(ofs);
	timer.clear();

	ofs<<std::endl;


	ofs.close();
}

void test_tritree_tdtree(){

	gtl::Timer<char> timer;
	gtl::Interval1dOperators op;
	time_t now ;
	::time(&now);
	tm   t;
	::localtime_s(&t,&now); 
	std::string sz = gtl::numberToString<char,long>(t.tm_year+1900)+"-"+
		gtl::numberToString<char,long>(t.tm_mon+1)+"-"+
		gtl::numberToString<char,long>(t.tm_mday)+"-"+
		gtl::numberToString<char,long>(t.tm_hour)+"-"+
		gtl::numberToString<char,long>(t.tm_min)+"-"+
		gtl::numberToString<char,long>(t.tm_sec);

	std::ofstream ofs(sz+"_tri_td.txt"); 

	for (int j=10000;j<31000;j+=500)
	{

		ofs<<j;//Êý¾Ý¸öÊý 
		std::vector<gtl::Interval1d> vss;  
	
		op.random(j, 1, 10000, vss);
		gtl::Envelope2d e2d (0,100,0,100);
		e2d.moveTo(rand()%10000,rand()%10000);

		{
			gtl::TriTree itree(0,10240,100); 
			timer.start(" ");
			for(std::vector<gtl::Interval1d>::iterator it =vss.begin();it!=vss.end();it++ )
			{
				itree.insert(&(*it));
			} 
			timer.end();

			//ofs<< " " <<itree.getMap().size();

			std::vector<gtl::Interval1d*> results; 
			timer.start(" ");
			for(int k=0;k<1000;k++)
			{
				itree.search(e2d,results);
			}
			timer.end();
			results.clear();


		}

		{
			gtl::TDTree itree(0,10240,100); 
			timer.start(" ");
			for(std::vector<gtl::Interval1d>::iterator it =vss.begin();it!=vss.end();it++ )
			{
				itree.insert(&(*it));
			} 
			timer.end();

			//ofs<< " " <<itree.getMap().size();


			std::vector<gtl::Interval1d*> results; 
			timer.start(" ");
			for(int k=0;k<1000;k++)
			{
				itree.search(e2d);

			}		
			timer.end(); 
			results.clear();
		}


		timer.write(ofs);
		timer.clear();
		ofs<<std::endl;
	}

	ofs.close();
}

void test_for_access_info(){
	
	std::string sz = "test_for_access_info";
	std::ofstream ofs(sz+".txt"); 
 
	gtl::Envelope2d e2d4[4];

	double minv[4];
	double maxv[4];
	std::vector<gtl::Interval4d*> vss;

	int search_operation_repeated_number=1;
	std::pair<unsigned long long , unsigned long long> accessedInfo;

	gtl::Envelopes<double,4> queryEnvelopes;
	queryEnvelopes.read(_database+"envelopes100"+".ef");

	unsigned long long leafNumber=0;
	unsigned long long innerNumber=0;
	unsigned long long splitNumber=0;
	for(unsigned int i=1000;i<31000;i+=1000)
	{

		gtl::RTree<gtl::Interval4d*,double> rtree;

		//gtl::RStarTree<gtl::Interval4d*,4,4,8> rstree;

		gtl::Interval4dTrees<gtl::TriTree> itree(
			0,10240,20,0,10240,20,0,10240,20,0,10240,20);


		ofs<<i << " ";
		///////////////////////////////////////////////////////////////////////
		g_console.print("generate %d intervals\n",i);
		gtl::Interval4d::random(i,1,10000,vss);

		g_console.print("Interval4dTrees insertion\n");		 
		itree.insert(vss);	

		/*g_console.print("RStarTrees insertion \n"); 
		for(std::vector<gtl::Interval4d*>::iterator it =vss.begin();it!=vss.end();it++ )
		{
			(*it)->get(minv,maxv);
			rstree.insert(minv,maxv,*it);
		} */
		g_console.print("RTrees insertion \n"); 
		for(std::vector<gtl::Interval4d*>::iterator it =vss.begin();it!=vss.end();it++ )
		{
			(*it)->get(minv,maxv);
			rtree.insert(minv,maxv,*it);
		} 


		for (int k=0;k<4;k++){
			gtl::TriTree* sub = itree.getTree(k);
			splitNumber+=sub->splitNumber; 
		}

		ofs<<splitNumber<<" "<<rtree.splitNumber<<" ";
		///////////////////////////////////////////////////////////////////////////////////////////////	
		
		g_console.print("Interval4dTrees searching operation \n"); 
		leafNumber=0;
		innerNumber=0;
		for(auto it = queryEnvelopes.begin();it!=queryEnvelopes.end(); it++)
		{		
			for (int k=0;k<4;k++){
				e2d4[k]=gtl::Envelope2d(it->getLowerBound(k),it->getUpperBound(k),it->getLowerBound(k),it->getUpperBound(k));
			}

			itree.search(e2d4,search_operation_repeated_number); 
			for (int k=0;k<4;k++){
				gtl::TriTree* sub = itree.getTree(k);
				innerNumber+=sub->accessedInnerNodeNumber;
				leafNumber+=sub->accessedLeafNodeNumber;
			}
		} 
		ofs<<innerNumber<<" "<<leafNumber<<" ";
		/*g_console.print("RStarTrees searching operation \n"); 
		leafNumber=0;
		innerNumber=0;
		for(auto it = queryEnvelopes.begin();it!=queryEnvelopes.end(); it++)
		{
	
			for (int k=0;k<4;k++){
				minv[k]= it->getLowerBound(k);
				maxv[k]=it->getUpperBound(k);
			}

			accessedInfo = rstree.search(minv,maxv,search_operation_repeated_number); 
			innerNumber+=accessedInfo.first;
			leafNumber+=accessedInfo.second;
		}
		ofs<<innerNumber<<" "<<leafNumber<<" ";*/
		g_console.print("RTrees searching operation \n"); 
		leafNumber=0;
		innerNumber=0;
		for(auto it = queryEnvelopes.begin();it!=queryEnvelopes.end(); it++)
		{
			for (int k=0;k<4;k++){
				minv[k]= it->getLowerBound(k);
				maxv[k]=it->getUpperBound(k);
			}
		
			accessedInfo=rtree.search(minv,maxv,search_operation_repeated_number); 
			innerNumber+=accessedInfo.first;
			leafNumber+=accessedInfo.second;
		}
		ofs<<innerNumber<<" "<<leafNumber<<std::endl;
	}

	ofs.close();
};

int _tmain(int argc, _TCHAR* argv[])
{	
	//test_tritree();	
	//test_interval4dtrees();
	//test_tdtree();
	//test_interval4dtrees_with_real_dataset();
	//test_interval4dtrees_for_large_data();	
	//test_tritree_tdtree();
	test_for_access_info();
	return 0;
}

