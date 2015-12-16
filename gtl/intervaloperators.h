/*
* Geosciences Template Library
*
* Copyright (c) 2008
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

#include "interval.h" 
begin_gtl_namespace 

template <
typename T,
typename CHARTYPE = char,
typename TRAITS = std::char_traits<CHARTYPE>,
typename ALLOC = std::allocator<CHARTYPE>  >
class IntervalOperators : public virtual Object < CHARTYPE, TRAITS, ALLOC > { 
	typedef Interval<T, CHARTYPE, TRAITS, ALLOC> IntervalClass;
public:
	virtual std::basic_string<CHARTYPE, TRAITS, ALLOC> getClassName(){
		return "IntervalOperators";
	}

	virtual void writeBuffer(void* & buffer, size_t & count){
	}

	virtual void readBuffer(const void *  buffer, size_t count = 0){
	}

	virtual Object * clone() {
		return nullptr;
	}
public:
	//(a)	Equals Query: Is=Qs and Ie=Qe.
	bool equals(const IntervalClass & i1, const IntervalClass & i2){
		return false;
	}
	//(b)	Starts Query: Is=Qs and Qs<Ie<Qe; as shown in Fig.3(a)
	bool starts(const IntervalClass & i1, const IntervalClass & i2){
		return false;
	}
	//(c)StartedBy Query : Is = Qs and Ie>Qe; as shown in Fig.3(b).
	bool startedBy(const IntervalClass & i1, const IntervalClass & i2){
		return false;
	}
	//(d)	Meets Query: Is<Ie=Qs<Qe; as shown in Fig.3(c).
	bool meets(const IntervalClass & i1, const IntervalClass & i2){
		return false;
	}
	//(e)	MetBy Query: Qs<Qe=Is<Ie; as shown in Fig.3(d).
	bool metBy(const IntervalClass & i1, const IntervalClass & i2){
		return false;
	}
	//(f)	Finishes Query: Qs<Is<Qe and Ie=Qe; as shown in Fig.3(e).
	bool finishes(const IntervalClass & i1, const IntervalClass & i2){
		return false;
	}
	//(g)	FinishedBy Query: Is<Qs and Ie=Qe; as shown in Fig.3(f).
	bool finishedBy(const IntervalClass & i1, const IntervalClass & i2){
		return false;
	}
	//(h)	Before Query: Is<Ie<Qs<Qe; as shown in Fig.4(a).
	bool before(const IntervalClass & i1, const IntervalClass & i2){
		return false;
	}
	//(i)	After Query: Qs<Qe<Is<Ie; as shown in Fig.4(b).
	bool after(const IntervalClass & i1, const IntervalClass & i2){
		return false;
	}
	//(j)	Overlaps Query: Is<Qs and Qs<Ie<Qe; as shown in Fig.4(c).
	bool overlaps(const IntervalClass & i1, const IntervalClass & i2){
		return false;
	}
	//(k)	OverlappedBy  Query: Qs<Is<Qe and Ie>Qe; as shown in Fig.4(d).
	bool overlapedBy(const IntervalClass & i1, const IntervalClass & i2){
		return false;
	}
	//(l)	During Query: Qs<Is<Ie<Qe; as shown in Fig.4(e).
	bool during(const IntervalClass & i1, const IntervalClass & i2){
		return false;
	}
	//(m) Contains Query: Is<Qs<Qe<Ie; as shown in Fig.4(f).
	bool contains(const IntervalClass & i1, const IntervalClass & i2){
		T low = i2.start();
		T high = i2.end();
		int type =  i2.getType();

		T _start = i1.start();
		T _end = i1.end();
		int _type = i1.getType();

		if (_start < low && _end > high)
			return true;

		if (_start > low || _end < high) 
			return false;

		switch (_type)
		{
		case IT_CLOSED:
			break;
		case IT_OPEN:
			if ((_start == low && _end == high && type != IT_OPEN) ||
				(_start == low && (type == IT_CLOSED || type == IT_RIGHTOPEN)) ||
				(_end == high && (type == IT_CLOSED || type == IT_LEFTOPEN)))
				return false;
			break;
		case IT_RIGHTOPEN:
			if (_end == high && (type == IT_CLOSED || type == IT_LEFTOPEN))
				return false;
			break;
		case IT_LEFTOPEN:
			if (_start == low && (type == IT_CLOSED || type == IT_RIGHTOPEN))
				return false;
			break;
		}

		return true; 
	}
	//(n ) Covers Query: Is=Qs , Ie=Qe, Q is close and U is open.
	bool covers(const IntervalClass & i1, const IntervalClass & i2){
		return false;
	}
	//(o) CoveredBy Query: Is=Qs , Ie=Qe, Q is open and U is close.
	bool coveredBy(const IntervalClass & i1, const IntervalClass & i2){
		return false;
	}
public:
	bool intersects(const IntervalClass & i1, const IntervalClass & i2, IntervalClass & it){
		return false;
	}

	bool intersects(const IntervalClass & i1, const IntervalClass & i2){
		T _start = i1.start();
		T _end = i1.end();
		int _type = i1.getType();

		T low = i2.start();
		T high = i2.end();
		int type = i2.getType();
		
		if (_start > high || _end < low) 
			return false;

		if ((_start > low && _start < high) || (_end > low && _end < high)) 
			return true;

		switch (_type)
		{
		case IT_CLOSED:
			if (_start == high)	{
				if (type == IT_CLOSED || type == IT_LEFTOPEN) 
					return true;
				else 
					return false;
			}
			else if (_end == low) {
				if (type == IT_CLOSED || type == IT_RIGHTOPEN) 
					return true;
				else
					return false;
			}
			break;
		case IT_OPEN:
			if (_start == high || _end == low)
				return false;
			break;
		case IT_RIGHTOPEN:
			if (_start == high)	{
				if (type == IT_CLOSED || type == IT_LEFTOPEN) 
					return true;
				else 
					return false;
			}
			else if (_end == low) {
				return false;
			}
			break;
		case IT_LEFTOPEN:
			if (_start == high)	{
				return false;
			}
			else if (_end == low) {
				if (type == IT_CLOSED || type == IT_RIGHTOPEN) 
					return true;
				else 
					return false;
			}
			break;
		}

		return true;
	}

public:
	void random(int n, T mind, T maxd, std::vector<IntervalClass> & vs)	{
		vs.resize(n);
		unsigned int k = 0;
		bool repeat = false;
		while (k < n) {
			RandomNumbers<T> s(n, (double)mind, (double)maxd), e(n, (double)mind, (double)maxd);
			IntervalClass  v;
			for (int i = 0; i < n; i++)	{
				if (s[i]<e[i]){
					v.start(s[i]);
					v.end(e[i]);
				}
				else if (s[i] == e[i]){
					double t = -1;//rand()%100/100.0*(maxd-mind)+mind;
					while (t == e[i])
						t = rand() % 100 / 100.0*(maxd - mind) + mind;
					if (t>s[i])
						e[i] = t;
					else{
						e[i] = s[i];
						s[i] = t;
					}
				}
				else {
					v.start(e[i]);
					v.end(s[i]);
				}
				//查看是否有重复
				repeat = false;
				for (int j = 0; j < k; j++)	{
					if (vs[j] == v)
					{
						repeat = true;
						break;
					}
				}
				if (repeat == false) {
					if (k == n)
						return;

					vs[k] = v;
					k++;
				}
			}
		}
	}
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef IntervalOperators<double, char, std::char_traits<char>, std::allocator<char>> Interval1dOperators;
typedef Interval1dOperators* Interval1dOperatorsPtr;
end_gtl_namespace