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
#include <vector>
#include <sstream>
#include <limits>
#include <cmath>
#include "identifier.h"
#include "valutils.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

/** @addtogroup value Value 
*  @{
*/
class CORE_API Value {
protected:	
	ValUtils::VALUE * m_data;
	bool _need_release;
public:
	typedef ValUtils::VALUE value_type;
public:
	
	Value();
	void copy (const Value & v);
	void copy (const ValUtils::VALUE & v);
	Value(const Value & v);
	Value( ValUtils::VALUE * pv, bool auto_release=true);
	
	virtual ~Value();
	void clear();
	void attach( ValUtils::VALUE * pv);
	ValUtils::VALUE * detach() ;

	virtual void write(Buffer & bs);
	virtual void read(Buffer & bs);
public:
	Value operator = (const Value & v);
	Value operator = (char v);
	Value operator = (unsigned char v);
	Value operator = (short  v);
	Value operator = (unsigned short v);
	Value operator = (int v);
	Value operator = (long v);
	Value operator = (unsigned long v);
	Value operator = (unsigned long long v);
	Value operator = ( Identifier & v);
	Value operator = (float v);
	Value operator = (double v);
	Value operator = (string v);
	Value operator = (const char *  v);
public:
	inline int getTypeSize( ){	return ValUtils::getTypeSize(m_data->type);	}
	inline long getCount(){return m_data->count;}
	inline long getSize(){ return m_data->count * getTypeSize();}

	inline ValUtils::VALUE & get(){ return *m_data;}	
	inline bool isDecimal(){return ValUtils::isDecimal(*m_data);}
	inline bool isInteger(){return ValUtils::isInteger(*m_data);}
	inline bool isNumber(){return ValUtils::isNumber(*m_data);	}
	inline bool isChar(){return ValUtils::isChar(*m_data);	}
	inline bool isArray(){return ValUtils::isArray(*m_data);	}
	inline operator double (){return ValUtils::toNumber(*m_data);	}
	inline std::string toString(){return ValUtils::toString(*m_data);	}
	inline std::wstring toWString(){return ValUtils::toWString(*m_data);	}
	inline int compare( Value & v)  {return ValUtils::compare(*m_data,*v.m_data);	}

	inline void initialChar(char * cc,int n=1){ValUtils::initialChar(m_data,cc,n);	}
	inline void initialChar(char  cc ){initialChar(&cc,1);	}
	inline void initialUChar(unsigned char * cc,int n=1){ValUtils::initialUChar(m_data,cc,n);	}
	inline void initialUChar(unsigned char  cc ){initialUChar(&cc,1);	}

	inline void initialWChar(wchar_t * cc,int n=1){ValUtils::initialWChar(m_data,cc,n);	}
	inline void initialWChar(wchar_t  cc ){initialWChar(&cc,1);	}
	inline void initialShort(short * cc,int n=1){ValUtils::initialShort(m_data,cc,n);	}
	inline void initialShort(short  cc ){initialShort(&cc,1);	}

	inline void initialUShort(unsigned short * cc,int n=1){ValUtils::initialUShort(m_data,cc,n);	}
	inline void initialUShort(unsigned short  cc ){initialUShort(&cc,1);	}

	inline void initialInt(int * cc,int n=1){ValUtils::initialInt(m_data,cc,n);	}
	inline void initialInt(int  cc ){initialInt(&cc,1);	}

	inline void initialUInt(unsigned int * cc,int n=1){ValUtils::initialUInt(m_data,cc,n);	}
	inline void initialUInt(unsigned int  cc ){initialUInt(&cc,1);	}

	inline void initialLong(long * cc,int n=1){ValUtils::initialLong(m_data,cc,n);	}
	inline void initialLong(long  cc ){initialLong(&cc,1);	}

	inline void initialULong(unsigned long * cc,int n=1){ValUtils::initialULong(m_data,cc,n);	}
	inline void initialULong(unsigned long  cc ){initialULong(&cc,1);	}

	inline void initialFloat(float * cc,int n=1){ValUtils::initialFloat(m_data,cc,n);	}
	inline void initialFloat(float  cc ){initialFloat(&cc,1);	}

	inline void initialDouble(double * cc,int n=1){ValUtils::initialDouble(m_data,cc,n);	}
	inline void initialDouble(double  cc ){(&cc,1);	}	
};
/** @} */ 

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
