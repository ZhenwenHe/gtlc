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
#include <list>
#include <map>
#include "bitset.h"
#include "buffer.h"
#include "vertex.h"
#include "IsoscelesRightTriangle.h"
#include "interval.h"
#include "intervals.h"
begin_gtl_namespace 
class TDTree
{
public:
typedef IsoscelesRightTriangle<char> triangle_type;
typedef Bitset<char>                 bitset_type;
private:
	double   _minmumValue;//最小值
	double   _maximumValue;//最大值	
	//表示树的叶子节点包含Interval的最大个数为_blocking_factor，大于这个数则该节点会分裂
	unsigned int  _blockingFactor;
	short    _dimensionalOrder;//指向的维度序号；

	std::map< unsigned long long, IntervalArray > _directory;
	int      _depth;//树的深度【1，64】

	triangle_type _total;//保存下来，减少计算量
	typedef std::map< unsigned long long, IntervalArray >::iterator map_iterator;
	typedef std::list< map_iterator >::reference list_reference;
	typedef std::map< unsigned long long, IntervalArray >  map_type;

public:
	TDTree(double minValue, double maxValue, unsigned int maxNumberInBlock)
	{
		_maximumValue=maxValue;
		_minmumValue=minValue;
		_blockingFactor=maxNumberInBlock;
		_dimensionalOrder=0;
		_depth=1;
		_total.set(Vertex3d(minValue,maxValue,0),Vertex3d(maxValue,maxValue,0),Vertex3d(minValue,minValue,0));
	}
	bool insert(Interval1d * pi) {
		/*
			find maximum envelope at maximum depth where the object for insertion would belong;

			the target envelope equals the region in directory with longest number in common 
			left to right with the maximum envelope;
			find target envelope population;

			if ( the population of target < maximum population ) then 
				increase the population in directory in directory of target envelope;
				update region with target envelope;
            else 
			    split the target envelope;
			endif 
		*/
	
		unsigned long long code = calculateTriangleIdentifer(pi,_depth);
		map_iterator it = _directory.find(code);
		if(it!=_directory.end()) 
		{
			if(it->second.size()<_blockingFactor)
			{
				it->second.push_back(pi);
				return true;
			}
			else
			{
				it->second.push_back(pi);
				split(it);
				return true;
			}
		}
		else
		{
			//查看上层节点,如果上层为空，则直接在跟节点添加pi
			if((code>>1)==0)
			{
				_directory[1].push_back(pi);		
				return true;
			}
			//如果上层节点标识不为空，
			int i=1;
			std::string sz;
			numberToString(code,sz);
			std::string prefix;
			do
			{
				prefix = sz.substr(0,i);
				if(hasPrefix(prefix))
				{
					i++;
				}
				else
					break;
			}while(i<=sz.size());

			code = stringToNumber(prefix.c_str());
			_directory[code].push_back(pi);
			return true;
		}
		return true;
	}
	inline void search(const Envelope2d & e2d)
	{
		std::list< map_iterator > ls;
		for(map_iterator it = _directory.begin();it!=_directory.end();it++)
			ls.push_back(it);

		unsigned long long parent_identifier = 1;
		triangle_type         parent_envelope;
		int test_result=0;
		std::list< map_iterator >::iterator ls_old_it,ls_it;
		IntervalArray::iterator interval_it ;
		Interval1d * interval_ptr=0;
		Interval4d * interval4d_ptr=0;

		while(!ls.empty()){
			std::list< map_iterator >::reference it = ls.front();
			parent_identifier = parentIdentifer(it->first);
			calculateTriangle(parent_identifier,parent_envelope);

			if(intersect(e2d,parent_envelope) ==false)//不相交，则移除list中所有前缀为parent_identifier的节点 
			{ 
				for( ls_it = ls.begin();ls_it!=ls.end();)
				{
					if(isPrefix(parent_identifier,(*ls_it)->first))
					{
						ls_old_it = ls_it;
						ls_it++;
						ls.erase(ls_old_it);
					}
					else
						ls_it++;
				}
			}
			else{//两个包围盒a和b相交
				for(ls_it = ls.begin();ls_it!=ls.end();)
				{
					if(isPrefix(parent_identifier,(*ls_it)->first))
					{
						for( interval_it = (*ls_it)->second.begin();
							interval_it!=(*ls_it)->second.end();interval_it++){
								if(e2d.test((*interval_it)->start(),(*interval_it)->end())!=0)
								{
									interval4d_ptr =(Interval4d*)( (*interval_it)->parent());
									if(interval4d_ptr)
										interval4d_ptr->flags(_dimensionalOrder,1);
									
								}
						}
						ls_old_it = ls_it;
						ls_it++;
						ls.erase(ls_old_it);
					}
					else
						ls_it++;
				}
			}			 
			if(!ls.empty() ) 
				ls.pop_front();
		}
	}
	int search(const Envelope2d & e2d,std::vector<Interval1d*> & results)
	{
		std::list< map_iterator > ls;
		for(map_iterator it = _directory.begin();it!=_directory.end();it++)
			ls.push_back(it);

		unsigned long long parent_identifier = 1;
		triangle_type         parent_envelope;
		int test_result=0;
		std::list< map_iterator >::iterator ls_old_it,ls_it;
		IntervalArray::iterator interval_it ;
		Interval1d * interval_ptr=0;
		Interval4d * interval4d_ptr=0;

		while(!ls.empty()){
			std::list< map_iterator >::reference it = ls.front();
			parent_identifier = parentIdentifer(it->first);
			calculateTriangle(parent_identifier,parent_envelope);

			if(intersect(e2d,parent_envelope) ==false)//不相交，则移除list中所有前缀为parent_identifier的节点 
			{ 
				for( ls_it = ls.begin();ls_it!=ls.end();)
				{
					if(isPrefix(parent_identifier,(*ls_it)->first))
					{
						ls_old_it = ls_it;
						ls_it++;
						ls.erase(ls_old_it);
					}
					else
						ls_it++;
				}
			}
			else{//两个包围盒a和b相交
				for(ls_it = ls.begin();ls_it!=ls.end();)
				{
					if(isPrefix(parent_identifier,(*ls_it)->first))
					{
						for( interval_it = (*ls_it)->second.begin();
							interval_it!=(*ls_it)->second.end();interval_it++){
								if(e2d.test((*interval_it)->start(),(*interval_it)->end())!=0)
								{
									interval4d_ptr = (Interval4d*)( (*interval_it)->parent());
									if(interval4d_ptr)
										interval4d_ptr->flags(_dimensionalOrder,1);
									results.push_back(*interval_it);
								}
						}
						ls_old_it = ls_it;
						ls_it++;
						ls.erase(ls_old_it);
					}
					else
						ls_it++;
				}
			}			 
			if(!ls.empty() ) 
				ls.pop_front();
		}
		return results.size();
	}
public:
	inline short getDimensionalOrder() const {return _dimensionalOrder;}
	inline void setDimensionalOrder(short d)  { _dimensionalOrder=d;}
	inline map_type & getMap() {return _directory;}
protected:
	/*
		divide the triangle p into two triangles
	*/
	inline void decompose(triangle_type & p, triangle_type& leftTriangle, triangle_type & rightTriangle)
	{
		p.decompose(leftTriangle,rightTriangle);
	}
	
	/*
		divide the envelope p into two triangles, and get the triangle located in index
	*/
	inline void decompose(triangle_type& p,int index, triangle_type & e)
	{
		p.subtriangle(index,e);
	}
	/*
		calculate the number identifer of the envelope where the interval belongs in the depath 
	*/
	inline unsigned long long calculateTriangleIdentifer(Interval1d * pt,int depth)
	{
		
		int i=1;
		unsigned long long s =1;
		triangle_type left, right;
		triangle_type total(_total);
		while(i<depth ){
			decompose(total,left,right);
			if(left.test(Vertex3d(pt->start(),pt->end(),0))!=0){
				s=s<<1;
				total = left;
			}
			else
			{
				s=s<<1;
				s+=1;
				total = right;
			}
			i++;
		}
		return s;
	}
	/*
		calculate the characteristic identifer of the envelope where the interval belongs in the depath 
	*/
	inline unsigned long long calculateTriangleIdentifer(Interval1d * pt,std::string & sz,int depth)
	{
		unsigned long long s = calculateTriangleIdentifer(pt,depth);
		numberToString(s,sz);
		return s;
	}
	/*
		calculate the depth of the characteristic identifer  
	*/
	inline int calculateIdentiferDepth(std::string & sz)
	{
		return sz.size();
	}
	/*
		calculate the depth of the number identifer  
	*/
	inline int calculateIdentiferDepth(unsigned long long code)
	{
		int i=0;
		while(code !=0)
		{
			code = code >>1;
			i++;
		}
		return i;
	}
	/*
		calculate the maximum envelope at maximum depth 
	*/
	inline void calculateTriangle(unsigned long long code , triangle_type & e2d)
	{
		if(code == 1) 
		{
			e2d=_total;
			return;
		}
		bitset_type bs(code);//最高位放在0位置
		triangle_type total = _total;
		for(int i=1;i<bs.count();i++){
			total.subtriangle(bs[i]?1:0,e2d);
			total  = e2d;
		}
	}
	inline int numberToString(const unsigned long long & code,std::string& sz){
		bitset_type bs(code);
		//sz = bs.to_string();
		bs.get(sz);
		return bs.count();
	}
	inline unsigned long long stringToNumber(const  char * sz){
		int s = strlen(sz);
		bitset_type bs;
		bs.resize(s);
		for(int i=0;i<s;i++){
			if(sz[i]=='0')
				bs.set(i,false);
			else
				bs.set(i,true);
		}
		//return bs.to_ullong();
		return bs.ullong();
	}
	//判断a是否是b的前缀
	inline bool isPrefix(const unsigned long long & a, const unsigned long long & b){	
		unsigned long long c= b;
		while(c!=0)
		{
			if(c==a)
				return true;
			c=c>>1;			
		}
		return false;
	}
	//判断map中是否存在前缀为sz的对象存在
	inline bool hasPrefix(const std::string & sz){
		unsigned long long prefix = stringToNumber(sz.c_str());
		for(map_iterator it = _directory.begin();it!=_directory.end();it++)
		{
			if(isPrefix(prefix,it->first))
				return true;
		}
		return false;
	}
	//判断map中是否存在前缀为prefix的对象存在
	inline bool hasPrefix(const unsigned long long & prefix){
		for(map_iterator it = _directory.begin();it!=_directory.end();it++)
		{
			if(isPrefix(prefix,it->first))
				return true;
		}
		return false;
	}
	inline unsigned long long parentIdentifer(const unsigned long long &  code)
	{
		return code>>1;
	}
	inline void split(map_iterator it){
		unsigned long long code=1;
		triangle_type total,e2d;
		bool repeated = false;
		int k_old ;
		do {		
			repeated = true;
			k_old =-1;
			code = it->first;
			//分解Envelope		
			calculateTriangle(code,total);
			decompose(total,0,e2d);
			for(IntervalArray::iterator i =it->second.begin();i!=it->second.end();i++)
			{				
				if(e2d.test(Vertex3d((*i)->start(),(*i)->end(),0))!=0)
				{
					if(k_old==1) repeated=false;
					_directory[code<<1].push_back(*i);
					k_old=0;
				}
				else{
					if(k_old==0) repeated = false;
					_directory[(code<<1)+1].push_back(*i);
					k_old=1;
				}
			}
			it = _directory.find(code);
			_directory.erase(it);
			_depth= max(calculateIdentiferDepth(code<<1),_depth);

			if(repeated)
			{ 
				code = (code<<1)+k_old;
				it = _directory.find(code);

				/*for(int j =0;j<3;j++)
				{
					Console::print("%lf,%lf,%lf\n",e2d.get(j).x,e2d.get(j).y,e2d.get(j).z);
				}
				for(IntervalArray::iterator t =it->second.begin();t!=it->second.end();t++)
				{
					Console::print("%lf,%lf\n",(*t)->start(),(*t)->end());
				}*/
			}
		}while(repeated );			
	}
	inline bool intersect(const Envelope2d & e2d, const triangle_type & irt)
	{
		if(e2d.test(irt.get(0))!=0)
			return true;
		if(e2d.test(irt.get(1))!=0)
			return true;
		if(e2d.test(irt.get(2))!=0)
			return true;

		Vertex3d v (e2d.minData[0],e2d.minData[1],0); 

		if(irt.test(v)!=0)
			return true;
		 
		v.set(e2d.minData[0],e2d.maxData[1],0);
		if(irt.test(v)!=0)
			return true;

	 
		v.set(e2d.maxData[0],e2d.maxData[1],0);
		if(irt.test(v)!=0)
			return true;

 
		v.set(e2d.maxData[0],e2d.minData[1],0);
		if(irt.test(v)!=0)
			return true;

		return false;
	}
};

end_gtl_namespace 