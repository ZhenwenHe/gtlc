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
#include "object.h"
begin_gtl_namespace

template <
	typename  T,
	typename ELEMENT=char,
	typename TRAITS=std::char_traits<ELEMENT>,
	typename ALLOC=std::allocator<ELEMENT> >
class List : public Object<ELEMENT,TRAITS,ALLOC>
{
	struct Node 
	{
		Node * left;
		Node * right;
		T      data;
	} ;
	Node * _head;
	Node * _tail;
public:
	typedef  Node * iterator ;
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "List";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	} 
public:
	List(){
		_head=0;
		_tail=0;
	}
	inline void push_back(const T & v){
		if(_tail)
		{
			Node * p = new Node;
			p->right=0;
			p->data=v;			
			p->left=_tail;
			_tail->right=p;
			_tail =p;
		}
		else
		{
			_tail = new Node ;
			_tail->left=0;
			_tail->right=0;
			_tail->data=v;
			_head =_tail;
		}
	}
	inline bool empty() {
		if(_head==0)
			return true;
		else
			return false;
	}
	inline T&  front(){ return _head->data;}

	inline void erase(iterator &it){ 
		if(it==_head && it==_tail)
		{
			delete _head;
			_head=0;
			_tail=0;
		}
		else if(it==_head)
		{
			iterator p = _head;
			_head=_head->right;
			_head->left=0;
			delete p;
		}
		else if(it==_tail)
		{
			iterator p = _tail;
			_tail=_tail->left;
			_tail->right=0;
			delete p;
		}
		else{
			it->left->right = it->right;
			it->right->left = it->left;
			delete it;
		}		
	}
	inline iterator begin() {return _head;}
	inline iterator end() {return 0;}

	inline void right(iterator & it) {
		it=it->right; 
	}
	inline void left(iterator & it) {
		it=it->left; 
	}
	inline T& get(iterator & it){
		return it->data;
	}
	inline void pop_front(){
		Node * p = _head;
		_head = _head->right;
		_head->left=0;
		delete p;
	}
};
end_gtl_namespace