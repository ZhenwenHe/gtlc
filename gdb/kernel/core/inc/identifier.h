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
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <set>
#include "time.h"
#include "config.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

/** @defgroup Identifier Identifier-Ψһ��ʶ
* ��ʵ����һ��64λ������  
*  @{
*/
class CORE_API Identifier{
	/** @name ULONGNumber 32λ����������  
	*  @{
	*/
	union ULONGNumber
	{
		unsigned long ns;
		unsigned char bs[4];
	};
	/** @} */ 

	/** @name 64λ����������  
	*  @{
	*/
	union {
		struct 	{			
			/** �û�ID���� */
			unsigned char user_id;
			/** �Զ��������� */
			unsigned char numbers[3];
			/** ID����ʱ�� */
			unsigned long time_stamp;
		}_sid;
		/** 64λԭʼֵ */
		unsigned long long  _id;
		/** ��BYTE��ʾ��64λֵ */
		unsigned char _bid[8]; 
	} ;
	/** @} */ 

public:
	/** @name ԭʼֵ���Ͷ���  
	*  @{
	*/
	typedef unsigned long long raw_type;
	/** @} */ 
public:
	/** @name ����������������  
	*  @{
	*/
	/** ��������û�ID����Identifier */
	Identifier();
	Identifier(int l ) {_id =l;}
	Identifier(unsigned long l ) {_id =l;}
	Identifier(unsigned int l ) {_id =l;}
	Identifier(unsigned long long l ) {_id =l;}
	Identifier(double l ) {_id = (unsigned long long)l;}
	Identifier(unsigned char user_id );
	Identifier(const Identifier & ids);
	/** @} */ 	
public:
	/** @name ��̬���ɺ�����  
	*  @{
	*/
	/**  ����һ��ֵΪ0��ID����
	*	 @param ��
	*   @return Identifier����     
	*/
	static Identifier zero() {	return Identifier(0);}
	/**  ����һ��Ψһ��ԭʼIDֵ��Ϊһ��64λ����
	*	 @param ��
	*   @return ����ԭʼ64λID    
	*/
	static raw_type  generate() ;
	/** @} */  
public:
	/** @name ��ȡIdentifier��ֵ  
	*  @{
	*/
	inline unsigned long long  get() { return _id; }
	inline const unsigned long long  get() const { return _id; }
	inline unsigned long getTimestamp(){ return _sid.time_stamp;}
	inline unsigned char getUserID() { return _sid.user_id;}
	inline unsigned char * getBytes(){return _bid;}
	/** @} */ 
public:
	/** @name ��ֵ�Ⱥ�����  
	*  @{
	*/
	Identifier operator = (const Identifier&  ids);
	Identifier operator = (unsigned long long  ids);
	Identifier operator = (int  ids);
	Identifier operator = (unsigned int  ids);
	Identifier operator = (unsigned long   ids);
	/** @} */ 

	/** @name ����ת����������  
	*  @{
	*/
	operator unsigned long long  () { return _id; }
	operator unsigned long  () { return (unsigned long)_id; }
	operator unsigned int () { return (unsigned int )_id; }
	operator int() { return (int)_id; }
	unsigned long long * operator &() { return &_id; }
	/** @} */ 	
public:

	/** @name ��̬��Ԫ��������  
	*  @{
	*/
	friend bool operator==(const Identifier & id1,const Identifier & id2);
	friend bool operator==(const Identifier & id1,unsigned long long  id2);

	friend bool operator!=(const Identifier & id1,const Identifier & id2);
	friend bool operator!=(const Identifier & id1,unsigned long long  id2);

	friend bool operator<(const Identifier & id1,const Identifier & id2);
	friend bool operator<(const Identifier & id1,unsigned long long  id2);

	friend bool operator>(const Identifier & id1,const Identifier & id2);
	friend bool operator>(const Identifier & id1,unsigned long long  id2);


	friend Identifier operator-(const Identifier & id1,const Identifier & id2);
	friend Identifier operator-(const Identifier & id1,unsigned long long  id2);

	friend Identifier operator+(const Identifier & id1,const Identifier & id2);
	friend Identifier operator+(const Identifier & id1,unsigned long long  id2);


	friend std::ostream & operator <<( std::ostream & s,const Identifier & id1);
	friend std::istream & operator >>( std::istream & s,const Identifier & id1);

	/** @} */ 
public:
	/** @name ����д����
	*  @{
	*/
	virtual void  write(std::ostream & f);
	virtual void  read(std::istream & f);
	/** @} */ 
};

/** @name ȫ�ֵ�0ֵIdentifer����  
*  @{
*/
static const Identifier ZEROID = Identifier::zero();
/** @} */ 

/** @} */ 


/** @addtogroup Identifier  Identifier����ⲿ��̬�������غ�����
*  @{
*/
bool operator==(const Identifier & id1,const Identifier & id2){
	return id1._id==id2._id;
}

bool operator==(const Identifier & id1,unsigned long long  id2){
	return id1._id==id2;
}

bool operator!=(const Identifier & id1,const Identifier & id2){
	return id1._id!=id2._id;
}

bool operator!=(const Identifier & id1,unsigned long long  id2){
	return id1._id!=id2;
}

bool operator>(const Identifier & id1,const Identifier & id2){
	return id1._id>id2._id;
}

bool operator>(const Identifier & id1,unsigned long long  id2){
	return id1._id>id2;
}

bool operator<(const Identifier & id1,const Identifier & id2){
	return id1._id<id2._id;
}

bool operator<(const Identifier & id1,unsigned long long  id2){
	return id1._id<id2;
}

Identifier operator+(const Identifier & id1,const Identifier & id2){
	return Identifier(id1._id+id2._id);
}

Identifier operator+(const Identifier & id1,unsigned long long  id2){
	return Identifier(id1._id+id2);
}

Identifier operator-(const Identifier & id1,const Identifier & id2){
	return Identifier(id1._id-id2._id);
}

Identifier operator-(const Identifier & id1,unsigned long long  id2){
	return Identifier(id1._id-id2);
}


std::ostream & operator <<( std::ostream & s,const Identifier & id1){
	s<< id1._id;
	return s;
}

std::istream & operator >>( std::istream & s,const Identifier & id1){
	s>> id1._id;
	return s;
}

/** @} */ 


/** @addtogroup Identifier  ϵͳ����ID���Ͷ���
*  @{
*/
/** ����ID */
typedef unsigned long long PRJID; 
/** ����ID */
typedef unsigned long long REGID;
/** ������ID */
typedef REGID             WKSPID ;
/** Ҫ������ID */
typedef unsigned long long FCLSID;
/** Ҫ��ID */
typedef unsigned long long FID; 
/** Ҫ�ؼ�ID */
typedef unsigned long long FSID;
/** Ҫ��LOD��ID */
typedef unsigned long long FLOD;
/** ����ID */
typedef unsigned long long TEXID; 
/** ͼ��ID */
typedef unsigned long long LYRID;
/** ����ģ�͵�ID */
typedef unsigned long long MDLID; 
/** �ռ�ο�ϵͳID */
typedef unsigned long long SREFID;
/** �û���¼���ݸ���ʱ�� */
typedef unsigned long long GDBTIME;
/** ����ID */
typedef unsigned long long MATID;
/** ע��ID */
typedef unsigned long long ANNOID;
/** ����ר�⼯ID */
typedef unsigned long long SEMID;
/** ��ϵ��ID */
typedef unsigned long long RELID;
/** �������˶���ID; */
typedef unsigned long long TID;
/** @} */ 

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
