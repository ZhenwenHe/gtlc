#ifdef USE_WINSOWS_OS
#include "windows.h"
#include "tchar.h"
#endif
begin_gtl_namespace

template<typename ELEMENT,typename TRAITS,typename ALLOC>
Console<ELEMENT,TRAITS,ALLOC>::Console()
{
	m_hConsole=0;
	m_bCreate=false;

#ifdef USE_WINSOWS_OS 
	if(AllocConsole())
	{
		m_hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTitle("Console Window");
		SetConsoleMode(m_hConsole,ENABLE_PROCESSED_OUTPUT);
		m_bCreate=true;
	}
	else{
		m_hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
		if(m_hConsole==INVALID_HANDLE_VALUE)
			m_hConsole=0;
	}
	if(m_hConsole)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 
		if(GetConsoleScreenBufferInfo(m_hConsole, &csbiInfo))
		{
			m_bAttrs=true;
			m_oldColorAttrs = csbiInfo.wAttributes;      
		}
		else{
			m_bAttrs=false;
			m_oldColorAttrs = 0;
		}
	}
#endif
}
template<typename ELEMENT,typename TRAITS,typename ALLOC>
Console<ELEMENT,TRAITS,ALLOC>::~Console()
{
#ifdef USE_WINSOWS_OS 
	if(m_bCreate)
		FreeConsole();
#endif
}

template<typename ELEMENT,typename TRAITS,typename ALLOC>
bool Console<ELEMENT,TRAITS,ALLOC>::setTile(const ELEMENT *  lpTile)
{
#ifdef USE_WINSOWS_OS 
	return SetConsoleTitle(lpTile)==TRUE?true:false;
#endif
	return false;
}

template<typename ELEMENT,typename TRAITS,typename ALLOC>
bool Console<ELEMENT,TRAITS,ALLOC>::writeString(const std::basic_string<ELEMENT,TRAITS,ALLOC>  &  lpString)
{
	bool ret=false;
#ifdef USE_WINSOWS_OS 
	if(m_hConsole)
	{
		ret=WriteConsole(m_hConsole,lpString.c_str(),_tcslen(lpString.c_str()),0,0)==TRUE?true:false;
	}
#endif
	return ret;
}

template<typename ELEMENT,typename TRAITS,typename ALLOC>
bool Console<ELEMENT,TRAITS,ALLOC>::writeString(unsigned int Attrs,const std::basic_string<ELEMENT,TRAITS,ALLOC>  & lpString)
{
	bool ret=false;
#ifdef USE_WINSOWS_OS 
	if(m_hConsole)
	{
		if(m_bAttrs)SetConsoleTextAttribute(m_hConsole,Attrs);
		ret=WriteConsole(m_hConsole,lpString.c_str(),_tcslen(lpString.c_str()),0,0)==TRUE?true:false;
		if(m_bAttrs)
			SetConsoleTextAttribute(m_hConsole,m_oldColorAttrs);
	}
#endif
	return ret; 
}

const int  MAX_BUF_LEN = 10240;

template<typename ELEMENT,typename TRAITS,typename ALLOC>
bool Console<ELEMENT,TRAITS,ALLOC>::print(const ELEMENT* fmt,...)
{
#ifdef USE_WINSOWS_OS 
	TCHAR   message[MAX_BUF_LEN];
	va_list cur_arg;
	va_start(cur_arg,fmt);
	_vsntprintf(message,MAX_BUF_LEN,fmt,cur_arg);
	va_end(cur_arg);
	return writeString(message);
#endif
	return false;
}

template<typename ELEMENT,typename TRAITS,typename ALLOC>
bool Console<ELEMENT,TRAITS,ALLOC>::print(unsigned int Attrs,const ELEMENT * fmt,...)
{
#ifdef USE_WINSOWS_OS 
	TCHAR   message[MAX_BUF_LEN];
	va_list cur_arg;
	va_start(cur_arg,fmt);
	_vsntprintf(message,MAX_BUF_LEN,fmt,cur_arg);
	va_end(cur_arg);
	return writeString(Attrs,message); 
#endif
	return false;
}


end_gtl_namespace