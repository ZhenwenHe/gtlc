// gtl_test_number.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../../gtl/number.h"

void test_gtl_bitset()
{
	int i = sizeof(unsigned char);
	unsigned long long ull = 66352;
	gtl::Bitset<char> bs1(ull);
	std::string sz;
	bs1.get(sz);
	std::cout<<sz<<std::endl;
	unsigned long long ull2=bs1.ullong();
	std::cout<<ull2<<std::endl;

	gtl::Bitset<char> bs2;
	bs2.set(sz.c_str());
	ull2=bs2.ullong();
	std::cout<<ull2<<std::endl;

	bool equal = (bs1==bs2);
	 
	bs2.leftShift(2).get(sz);
	std::cout<<sz<<std::endl;
	bs1.set(sz.c_str());
	ull2 = bs1.ullong();
	std::cout<<ull2<<std::endl;
	bool b1 = bs1.isPrefix(bs2);
	bool b2 = bs2.isPrefix(bs1);

	bs2.rightShift(5).get(sz);
	std::cout<<sz<<std::endl;
	bs1.set(sz.c_str());
	ull2 = bs1.ullong();
	std::cout<<ull2<<std::endl;
	b1 = bs1.isPrefix(bs2);
	b2 = bs2.isPrefix(bs1);
}
int _tmain(int argc, _TCHAR* argv[])
{
	
	test_gtl_bitset();

	return 0;
}

