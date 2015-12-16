#pragma  once
#include "object.h"

begin_gtl_namespace
 
template<
	typename ELEMENT=char,
	typename TRAITS=std::char_traits<ELEMENT>,
	typename ALLOC=std::allocator<ELEMENT> >
class  Console:public Object<ELEMENT,TRAITS,ALLOC>
{
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "Console";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
public:
	Console();
	virtual ~Console();

public:
	bool print(const ELEMENT * fmt,...);
	bool print(unsigned int Attrs,const ELEMENT * fmt,...);
	bool setTile(const ELEMENT * lpTile);

public:	

	bool writeString(const std::basic_string<ELEMENT,TRAITS,ALLOC> &  lpString);
	bool writeString(unsigned int Attrs,const std::basic_string<ELEMENT,TRAITS,ALLOC> &  lpString);
private:
	void * m_hConsole;
	bool   m_bCreate;
	bool   m_bAttrs;
	unsigned int   m_oldColorAttrs; 
};




end_gtl_namespace
#include "console.inl"
