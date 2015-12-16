// gtl_test_bitset.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../gtl/bitset.h"

int _tmain(int argc, _TCHAR* argv[])
{
	unsigned long long a = 1,b=1026;
	unsigned long long c = a & b;
	unsigned long long d = a | b;
	unsigned long long e = a ^ b;

	gtl::Bitset<char> bs1, bs2, bs3, bs4,bs5,bs6;
	std::string sz = "1000001111000111111100111";
	std::cout << sz << std::endl;
	bs1.set("1000001111000111111100111");
	sz.clear();
	bs1.get(sz);
	std::cout << sz << std::endl;
	bs2 = bs1.left(10);
	sz.clear();
	bs2.get(sz);
	std::cout << sz << std::endl;
	sz.clear();
	bs1.get(sz);
	std::cout << sz << std::endl;
	bs3 = bs1.right(10);
	sz.clear();
	bs3.get(sz);
	std::cout << sz << std::endl;

	bs4 = bs1.leftShift(10);
	sz.clear();
	bs4.get(sz);
	std::cout << sz << std::endl;

	bs5 = bs1.rightShift(10);
	sz.clear();
	bs5.get(sz);
	std::cout << sz << std::endl;

	bs6 = bs1 >> 10;
	sz.clear();
	bs6.get(sz);
	std::cout << sz << std::endl;

	bool b1 = (bs6 == 10);
	bool b2 = (bs6 != 10);

	bs1 = 666666666;
	bs2 = 666666665;
	assert(bs2 < bs1);
	assert(bs1 > bs2);
	assert(bs1 != bs2);
	bs3 = ~bs1;
	sz.clear();
	bs1.get(sz);
	std::cout << sz << std::endl;
	sz.clear();
	bs3.get(sz);
	std::cout << sz << std::endl;
	bs4 = bs1&bs2;
	sz.clear();
	bs4.get(sz);
	std::cout << sz << std::endl;
	return 0;
}

