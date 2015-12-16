/*
* Geosciences Template Library
*
* Copyright (c) 2008-2018
* Zhenwen He (zwhe@hotmail.com)
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Zhenwen He  makes no  representations
* about the suitability of this software for any purpose.
* It is provided "as is" without express or implied warranty.
*/
#pragma once
#include "gtl.h"
#include "bitset.h"
begin_gtl_namespace
/*
对于给定的矩形，每次分解为2^NUMDIMS个均等的矩形，
当NUMDIMS=1时，为二叉树
当NUMDIMS=2时，为四叉树
当NUMDIMS=3时，为八叉数
当NUMDIMS=4时，为十六叉数
以此类推，
如果给定一个初始矩形，则分解过程中的每一个子矩形都会和一个唯一的识别码一一对应。
*/
template<typename  COORDTYPE,
	int NUMDIMS,
	typename CHARTYPE = char,
	typename TRAITS = std::char_traits<CHARTYPE>,
	typename ALLOC = std::allocator<CHARTYPE>
	>
class EnvelopeDecomposer : public Object<CHARTYPE, TRAITS, ALLOC>
{
public:
	typedef Envelope<COORDTYPE, NUMDIMS, CHARTYPE, TRAITS, ALLOC>                     envelope;
	typedef Vertex<COORDTYPE, NUMDIMS, CHARTYPE, TRAITS, ALLOC>                       vertex;
	typedef Bitset<CHARTYPE, TRAITS, ALLOC>                                           identifier;
public:
	virtual std::basic_string<CHARTYPE, TRAITS, ALLOC> getClassName(){
		return "EnvelopeDecomposer";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){

	}
	virtual void readBuffer(const void *  buffer, size_t count = 0){

	}
	virtual Object * clone() {
		return 0;
	}
public:
	EnvelopeDecomposer(const envelope & e) :_rootEnvelope(e), _rootIdentifier(1)
	{
		_powofDimensions = std::pow(2, NUMDIMS);
	}
public:
	void calculateEnvelope(identifier code, envelope & e)
	{
		if (code == _rootIdentifier)
		{
			e2d = _rootEnvelope; 
			return;
		}
		std::stack<unsigned char> sk;
		envelope total = _rootEnvelope;
		identifier constcode;
		for (int i = 0i < NUMDIMS; i++)
		{
			constcode.set(i, true);
		}
		while (code != 1)
		{
			sk.push((unsigned char)(code & constcode));
			code = code >> NUMDIMS;
		}
		while (!sk.empty())
		{
			decompose(total, sk.top(), e2d);
			total = e2d;
			sk.pop();
		}
	}
	inline identifier calculateEnvelope(const vertex& r, const int depth)
	{
		envelope total = _total;
		envelope e2d;
		int i = 1;
		identifier s = 1;
		identifier temp;
		while (i<depth){
			for (int j = 0; j<_powofDimensions; j++){
				decompose(total, j, e2d);
				if (e2d.test(r) != 0)	{
					total = e2d;
					s = s << NUMDIMS;
					temp = j;
					s =s | temp;
					break;
				}
			}
			i++;
		}
		return s;
	}

	/*
	divide the envelope p into 2^NUMDIMS envelopes, and get the envelope located in index
	*/
	inline void decompose(const envelope& p, int index, envelope & e)
	{
		uint64_t mark = 1;
		for (int i = 0; i<NUMDIMS; i++)
		{
			if (index & mark)
			{
				e.minData[i] = (p.minData[i] + p.maxData[i]) / 2;
				e.maxData[i] = p.maxData[i];
			}
			else{
				e.minData[i] = p.minData[i];
				e.maxData[i] = (p.minData[i] + p.maxData[i]) / 2;
			}
			mark = mark << 1;
		}
	}
	/*
	divide the envelope p into 2^NUMDIMS envelopes
	*/
	inline void  decompose(const envelope& p, envelope es[])
	{
	 
		for (int i = 0; i<_powofDimensions; i++){
			decompose(p, i, es[i]);
		}
	}
protected:
	envelope   _rootEnvelope;
	identifier _rootIdentifier;
	int        _powofDimensions; 
};

end_gtl_namespace

 