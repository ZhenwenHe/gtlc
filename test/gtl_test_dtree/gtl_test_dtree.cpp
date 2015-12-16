// gtl_test_dtree.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h" 
#include <vector>
#include <string>
#include "console.h"
#include "spatiotemporalgenerator.h"

extern void prepare_test_data() ;

extern void test_dtree_rstartree_for_points();
extern void test_dtree_rstartree_for_segments();
extern void test_dtree_rstartree_for_trajectories();

extern void test_desompistiontree_tprtree(const std::string& filename);
extern void test_desompistiontree_bplustree();
extern void test_for_trajectories(const std::string& filename);
extern void test_for_trajectories_3d(const std::string& filename);
extern void test_for_future(const std::string& filename);
extern void test_import_brinkhoff( );
extern void test_dtree_bplustree_brinkhoff_100000_1000000();
extern void test_query_performace(const std::string& filename);
std::string _database = "d:\\gtl\\data\\";
gtl::Console<char> g_console;
gtl::Envelope<double,4> _cubeenvelope;

int _cube_size_length ;

int _tmain(int argc, _TCHAR* argv[])
{
	_cube_size_length=10000;
	for(int i=0;i<4;i++)
	{
		_cubeenvelope.setLowerBound(i,0);
		_cubeenvelope.setUpperBound(i,_cube_size_length);
	}




	
	//prepare_test_data();
	//test_dtree_rstartree_for_points();
	//test_dtree_rstartree_for_segments();
	//test_dtree_rstartree_for_trajectories();
	//test_desompistiontree_tprtree("delft.mpf");
	//test_desompistiontree_bplustree();
	//test_for_trajectories("random");
	//test_for_trajectories_3d("delft");
	//test_for_future("random");


	test_import_brinkhoff();
	//test_dtree_bplustree_brinkhoff_100000_1000000();
	/*test_query_performace("oldenburg100000");
	test_query_performace("oldenburg200000");
	test_query_performace("oldenburg300000");
	test_query_performace("oldenburg400000");
	test_query_performace("oldenburg500000");
	test_query_performace("oldenburg600000");
	test_query_performace("oldenburg700000");
	test_query_performace("oldenburg800000");
	test_query_performace("oldenburg900000");
	test_query_performace("oldenburg1000000");*/
	return 0;
}

