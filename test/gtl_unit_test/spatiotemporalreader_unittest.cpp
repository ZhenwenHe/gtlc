#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../gtl/spatiotemporalreader.h"
#include "../../gtl/spatiotemporalwriter.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace gtl_unit_test
{		
	TEST_CLASS(UnitTest_SpatiotemporalReader)
	{
	public:
		
		TEST_METHOD(importBrinkhoff)
		{
			
			gtl::Trajectories<double,4> trjs;
			gtl::SpatiotemporalReader<double,4>::importBrinkhoff(
				"D:\\Netherlands\\Brinkhoff\\oldenburgGen\\oldenburgGen_1000.dat",
				trjs);
			gtl::SpatiotemporalWriter<double,4>::writeTrajectories("oldenburgGen_1000.mtf",trjs);
		}

	};
}