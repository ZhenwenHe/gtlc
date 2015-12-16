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

#include "gtl.h"
begin_gtl_namespace
struct TypeInfo
{
public:
	TypeInfo() { 
		class Nil {};
		pInfo_ = &typeid(Nil);
	}
	TypeInfo(const std::type_info& ti):pInfo_(&ti){ 
	}
	const std::type_info& get() const {
		return *pInfo_;
	};
	bool before(const TypeInfo& rhs) const{
		return pInfo_->before(*rhs.pInfo_) != 0;
	}
	const char* name() const{
		return pInfo_->name();
	}
	const char*  raw_name() const {
		return pInfo_->raw_name();
	}
private:
	const std::type_info* pInfo_;
};

inline bool operator==(const TypeInfo& lhs, const TypeInfo& rhs)
{ return (lhs.get() == rhs.get()) != 0; }

inline bool operator<(const TypeInfo& lhs, const TypeInfo& rhs)
{ return lhs.before(rhs); }

inline bool operator!=(const TypeInfo& lhs, const TypeInfo& rhs)
{ return !(lhs == rhs); }    

inline bool operator>(const TypeInfo& lhs, const TypeInfo& rhs)
{ return rhs < lhs; }

inline bool operator<=(const TypeInfo& lhs, const TypeInfo& rhs)
{ return !(lhs > rhs); }

inline bool operator>=(const TypeInfo& lhs, const TypeInfo& rhs)
{ return !(lhs < rhs); }

template  < typename T >  TypeInfo getTypeInfo( T ){
	return TypeInfo(typeid(T));
}

end_gtl_namespace