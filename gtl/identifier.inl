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

#include "identifier.h"

begin_gtl_namespace
 /** @addtogroup Identifier  Identifier类的外部静态符号重载函数集
*  @{
*/

template<	int BITS ,	typename ELEMENT,	typename TRAITS,	typename ALLOC >
bool operator==(const Identifier< BITS , ELEMENT, TRAITS, ALLOC > & id1,const Identifier< BITS , ELEMENT, TRAITS, ALLOC > & id2){
	return id1.get()==id2.get();
}
template< int BITS , typename ELEMENT, typename TRAITS, typename ALLOC >
bool operator==(const Identifier< BITS , ELEMENT, TRAITS, ALLOC > & id1,uint64_t  id2){
    return id1.get()==id2;
}
template< int BITS , typename ELEMENT, typename TRAITS, typename ALLOC >
bool operator!=(const Identifier< BITS , ELEMENT, TRAITS, ALLOC > & id1,const Identifier< BITS , ELEMENT, TRAITS, ALLOC > & id2){
	return id1.get()!=id2.get();
}
template< int BITS , typename ELEMENT, typename TRAITS, typename ALLOC >
bool operator!=(const Identifier< BITS , ELEMENT, TRAITS, ALLOC > & id1,uint64_t  id2){
	return id1.get()!=id2;
}
template< int BITS , typename ELEMENT, typename TRAITS, typename ALLOC >
bool operator>(const Identifier< BITS , ELEMENT, TRAITS, ALLOC > & id1,const Identifier< BITS , ELEMENT, TRAITS, ALLOC > & id2){
	return id1.get()>id2.get();
}
template< int BITS , typename ELEMENT, typename TRAITS, typename ALLOC >
bool operator>(const Identifier< BITS , ELEMENT, TRAITS, ALLOC > & id1,uint64_t  id2){
	return id1.get()>id2;
}
template< int BITS , typename ELEMENT, typename TRAITS, typename ALLOC >
bool operator<(const Identifier< BITS , ELEMENT, TRAITS, ALLOC > & id1,const Identifier< BITS , ELEMENT, TRAITS, ALLOC > & id2){
	return id1.get()<id2.get();
}
template< int BITS , typename ELEMENT, typename TRAITS, typename ALLOC >
bool operator<(const Identifier< BITS , ELEMENT, TRAITS, ALLOC > & id1,uint64_t  id2){
	return id1.get()<id2;
}
template< int BITS , typename ELEMENT, typename TRAITS, typename ALLOC >
Identifier< BITS , ELEMENT, TRAITS, ALLOC > operator+(const Identifier< BITS , ELEMENT, TRAITS, ALLOC > & id1,const Identifier< BITS , ELEMENT, TRAITS, ALLOC > & id2){
	return Identifier< BITS , ELEMENT, TRAITS, ALLOC >(id1.get()+id2.get());
}
template< int BITS , typename ELEMENT, typename TRAITS, typename ALLOC >
Identifier< BITS , ELEMENT, TRAITS, ALLOC > operator+(const Identifier< BITS , ELEMENT, TRAITS, ALLOC > & id1,uint64_t  id2){
	return Identifier< BITS , ELEMENT, TRAITS, ALLOC >(id1.get()+id2);
}
template< int BITS , typename ELEMENT, typename TRAITS, typename ALLOC >
Identifier< BITS , ELEMENT, TRAITS, ALLOC > operator-(const Identifier< BITS , ELEMENT, TRAITS, ALLOC > & id1,const Identifier< BITS , ELEMENT, TRAITS, ALLOC > & id2){
	return Identifier< BITS , ELEMENT, TRAITS, ALLOC >(id1.get()-id2.get());
}
template< int BITS , typename ELEMENT, typename TRAITS, typename ALLOC >
Identifier< BITS , ELEMENT, TRAITS, ALLOC > operator-(const Identifier< BITS , ELEMENT, TRAITS, ALLOC > & id1,uint64_t  id2){
	return Identifier< BITS , ELEMENT, TRAITS, ALLOC >(id1.get()-id2);
}

template< int BITS , typename ELEMENT, typename TRAITS, typename ALLOC >
std::ostream & operator <<( std::ostream & s,const Identifier< BITS , ELEMENT, TRAITS, ALLOC > & id1){
	s<< id1.get();
	return s;
}

template< int BITS , typename ELEMENT, typename TRAITS, typename ALLOC >
std::istream & operator >>( std::istream & s,const Identifier< BITS , ELEMENT, TRAITS, ALLOC > & id1){
	s>> id1.get();
	return s;
}
////////////////////////////////////////////////////////////////////////////////
static unsigned long global_number_id=0;

template< int BITS , typename ELEMENT, typename TRAITS, typename ALLOC >
Identifier< BITS , ELEMENT, TRAITS, ALLOC >::Identifier(){//采用随机用户ID生成Identifier
	_id = 0;

	global_number_id=global_number_id % 0xffffff;
	ULONGNumber n;
	n.ns=global_number_id++;
	_sid.numbers[0]=n.bs[0];
	_sid.numbers[1]=n.bs[1];
	_sid.numbers[2]=n.bs[2];
	_sid.time_stamp = (unsigned long) ::time(NULL);

#ifdef USE_WINDOWS_OS
	srand(GetTickCount() );	
#endif

	_sid.user_id = rand()%255;		
}



template< int BITS , typename ELEMENT, typename TRAITS, typename ALLOC >
Identifier< BITS , ELEMENT, TRAITS, ALLOC >::Identifier(unsigned char user_id/*=0*/){
	_id = 0;
	_sid.user_id = user_id;		

	global_number_id=global_number_id % 0xffffff;
	ULONGNumber n;
	n.ns=global_number_id++;
	_sid.numbers[0]=n.bs[0];
	_sid.numbers[1]=n.bs[1];
	_sid.numbers[2]=n.bs[2];
	_sid.time_stamp = (unsigned long) ::time(NULL);
}
template< int BITS , typename ELEMENT, typename TRAITS, typename ALLOC >
Identifier< BITS , ELEMENT, TRAITS, ALLOC >::Identifier(const Identifier< BITS , ELEMENT, TRAITS, ALLOC > & ids){	
	_id = ids._id;
}
template< int BITS , typename ELEMENT, typename TRAITS, typename ALLOC >
Identifier< BITS , ELEMENT, TRAITS, ALLOC > Identifier< BITS , ELEMENT, TRAITS, ALLOC >::operator= (const Identifier & ids) { 
	_id = ids._id; 
	return *this;
}
template< int BITS , typename ELEMENT, typename TRAITS, typename ALLOC >
Identifier< BITS , ELEMENT, TRAITS, ALLOC > Identifier< BITS , ELEMENT, TRAITS, ALLOC >::operator = (uint64_t  ids){
	_id = ids; 
	return *this;
}
template< int BITS , typename ELEMENT, typename TRAITS, typename ALLOC >
Identifier< BITS , ELEMENT, TRAITS, ALLOC > Identifier< BITS , ELEMENT, TRAITS, ALLOC >::operator = (int  ids){
	_id = ids; 
	return *this;
}
 
template< int BITS , typename ELEMENT, typename TRAITS, typename ALLOC >
Identifier< BITS , ELEMENT, TRAITS, ALLOC > Identifier< BITS , ELEMENT, TRAITS, ALLOC >::operator = (uint32_t  ids){
	_id = ids; 
	return *this;
}
 

end_gtl_namespace
