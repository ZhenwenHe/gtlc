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

#pragma  once
#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <algorithm>
#include "object.h"

begin_gtl_namespace

template<class KEYTYPE,class INDEXTYPE=int,class OBJTYPE=void *>
struct IndexItem{
	KEYTYPE k;
	INDEXTYPE i;
	OBJTYPE obj;
};

template<class KEYTYPE,class INDEXTYPE=int,class OBJTYPE=void * >
struct greater_index_item: 
	public std::binary_function <IndexItem<KEYTYPE,INDEXTYPE,OBJTYPE>,IndexItem<KEYTYPE,INDEXTYPE,OBJTYPE>,bool> {

	bool operator () (const IndexItem<KEYTYPE,INDEXTYPE,OBJTYPE> & x,const IndexItem<KEYTYPE,INDEXTYPE,OBJTYPE> & y){
		return x.k>y.k;
	}
};

template<class KEYTYPE,class INDEXTYPE=int,class OBJTYPE=void * >
struct less_index_item: 
	public std::binary_function <IndexItem<KEYTYPE,INDEXTYPE,OBJTYPE>,IndexItem<KEYTYPE,INDEXTYPE,OBJTYPE>,bool> {

		bool operator () (const IndexItem<KEYTYPE,INDEXTYPE,OBJTYPE> & x,const IndexItem<KEYTYPE,INDEXTYPE,OBJTYPE> & y){
			return x.k<y.k;
		}
};

template<
	typename KEYTYPE,
	typename INDEXTYPE=int,
	typename OBJTYPE=void *,
	typename  ELEMENT,
	typename TRAITS=std::char_traits<ELEMENT>,
	typename ALLOC=std::allocator<ELEMENT> 
>
class IndexSorter: public Object<ELEMENT,TRAITS,ALLOC> {
	typedef IndexItem<KEYTYPE,INDEXTYPE,OBJTYPE> ELETYPE;
	std::vector<ELETYPE> data;
public:
	IndexSorter(KEYTYPE *pk,INDEXTYPE*pi,int s){
		ELETYPE e;
		for(int i=0;i<s;i++){
			e.k=pk[i];
			e.i=pi[i];
			e.obj=0;
			data.push_back(e);
		}
	}
	void  sortAsce(){
		std::sort(data.begin(),data.end(),less_index_item <KEYTYPE,INDEXTYPE,OBJTYPE>());		
	}
	void  sortDesc(){
		std::sort(data.begin(),data.end(),greater_index_item<KEYTYPE,INDEXTYPE,OBJTYPE>());	
	}
	std::vector<ELETYPE> & getResult(){
		return data;
	}
	void getResult(INDEXTYPE * iv,int * count){
		*count=data.size();
		for(int i=0;i<*count;i++){
			iv[i]=data[i].i;
		}
	}
 
public: 
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "IndexSorter";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
};
end_gtl_namespace