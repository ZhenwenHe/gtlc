
#include "stdafx.h" 
#include <vector>
#include <string>
#include "spatiotemporalgenerator.h"
extern std::string _database;

void test_import_brinkhoff_count(const std::string& filename, uint64_t count){
	gtl::Trajectories<double,4> trjs;
	gtl::SpatiotemporalReader<double,4>::importBrinkhoff(_database+filename,trjs,count);
	std::string sz = filename.substr(0,filename.rfind('-'));
	gtl::SpatiotemporalWriter<double,4>::writeTrajectories(_database+sz+gtl::numberToString<char,uint64_t>(count)+".mtf",trjs);
	
	{
		gtl::MovingSegments<double,4> segs;
		trjs.split(segs);
		gtl::SpatiotemporalWriter<double,4>::writeSegments(_database+sz+gtl::numberToString<char,uint64_t>(count)+".msf",segs);
	}

	{
		gtl::MovingPoints<double,4> pnts;
		trjs.split(pnts);
		gtl::SpatiotemporalWriter<double,4>::writePoints(_database+sz+gtl::numberToString<char,uint64_t>(count)+".mpf",pnts);
	}
}

void test_import_brinkhoff( )
{
	gtl::MovingPoints<double,4> pnts;
	gtl::SpatiotemporalReader<double,4>::readPoints(_database+"oldenburg0.mpf",pnts);
	gtl::SpatiotemporalWriter<double,4>::exportXYZ(_database+"oldenburg0.xyz",pnts);

	//test_import_brinkhoff_count("oldenburg-50000.dat",0);
	/*
	std::cout<<100000<<std::endl;
	test_import_brinkhoff_count("oldenburg-1000.dat",100000);
	std::cout<<200000<<std::endl;
	test_import_brinkhoff_count("oldenburg-2000.dat",200000);
	std::cout<<300000<<std::endl;
	test_import_brinkhoff_count("oldenburg-2000.dat",300000);
	std::cout<<400000<<std::endl;
	test_import_brinkhoff_count("oldenburg-5000.dat",400000);	
	std::cout<<500000<<std::endl;
	test_import_brinkhoff_count("oldenburg-5000.dat",500000);	
	std::cout<<600000<<std::endl;
	test_import_brinkhoff_count("oldenburg-5000.dat",600000);
	std::cout<<700000<<std::endl;
	test_import_brinkhoff_count("oldenburg-5000.dat",700000);
	std::cout<<800000<<std::endl;
	test_import_brinkhoff_count("oldenburg-5000.dat",800000);
	std::cout<<900000<<std::endl;
	test_import_brinkhoff_count("oldenburg-10000.dat",900000);
	std::cout<<1000000<<std::endl;
	test_import_brinkhoff_count("oldenburg-10000.dat",1000000);
	
	std::cout<<1100000<<std::endl;
	test_import_brinkhoff_count("oldenburg-10000.dat",1100000);
	std::cout<<1200000<<std::endl;
	test_import_brinkhoff_count("oldenburg-10000.dat",1200000);
	std::cout<<1300000<<std::endl;
	test_import_brinkhoff_count("oldenburg-10000.dat",1300000);
	std::cout<<1400000<<std::endl;
	test_import_brinkhoff_count("oldenburg-10000.dat",1400000);
	std::cout<<1500000<<std::endl;
	test_import_brinkhoff_count("oldenburg-10000.dat",1500000);
	
	std::cout<<1600000<<std::endl;
	test_import_brinkhoff_count("oldenburg-20000.dat",1600000);
	std::cout<<1700000<<std::endl;
	test_import_brinkhoff_count("oldenburg-20000.dat",1700000);
	std::cout<<1800000<<std::endl;
	test_import_brinkhoff_count("oldenburg-20000.dat",1800000);
	std::cout<<1900000<<std::endl;
	test_import_brinkhoff_count("oldenburg-20000.dat",1900000);
	std::cout<<2000000<<std::endl;
	test_import_brinkhoff_count("oldenburg-20000.dat",2000000);
	for(uint64_t i=2100000;i<=3200000;i+=100000)
	{
		std::cout<<i<<std::endl;
		test_import_brinkhoff_count("oldenburg-50000.dat",i);
	}*/
}