#include "console.h"


begin_gtl_namespace
begin_gdb_namespace
#if(ENABLE_CONSOLE_OUTPUT!=0)

Console::Console()
{
#if (USING_UILIB_TYPE==0)//MFC
	m_hConsole=0;
	m_bCreate=false;
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
#elif(USING_UILIB_TYPE==1)//QT


#else//wxWidget


#endif
}
Console::~Console()
{
#if (USING_UILIB_TYPE==0)//MFC

	if(m_bCreate)
		FreeConsole();
#elif(USING_UILIB_TYPE==1)//QT


#else//wxWidget


#endif
}

bool Console::setTile(const char *  lpTile)
{
#if (USING_UILIB_TYPE==0)//MFC

	return SetConsoleTitle(lpTile)==TRUE?true:false;
#elif(USING_UILIB_TYPE==1)//QT


#else//wxWidget


#endif
}

bool Console::writeString(const string &  lpString)
{
#if (USING_UILIB_TYPE==0)//MFC
	bool ret=false;
	if(m_hConsole)
	{
		ret=WriteConsole(m_hConsole,lpString.c_str(),_tcslen(lpString.c_str()),0,0)==TRUE?true:false;
	}
	return ret;
#elif(USING_UILIB_TYPE==1)//QT


#else//wxWidget


#endif
}

bool Console::writeString(unsigned int Attrs,const string & lpString)
{
#if(USING_UILIB_TYPE==0)//MFC
	bool ret=false;
	if(m_hConsole)
	{
		if(m_bAttrs)SetConsoleTextAttribute(m_hConsole,Attrs);
		ret=WriteConsole(m_hConsole,lpString.c_str(),_tcslen(lpString.c_str()),0,0)==TRUE?true:false;
		if(m_bAttrs)
			SetConsoleTextAttribute(m_hConsole,m_oldColorAttrs);
	}
	return ret; 
#elif(USING_UILIB_TYPE==1)//QT


#else//wxWidget


#endif
}

const int  MAX_BUF_LEN = 4096;
Console g_console;


bool CONSOLE_OUTPUT(const char* fmt,...)
{
#if(USING_UILIB_TYPE==0)//MFC
	TCHAR   message[MAX_BUF_LEN];
	va_list cur_arg;
	va_start(cur_arg, fmt);
	_vsntprintf(message, MAX_BUF_LEN, fmt, cur_arg);
	va_end(cur_arg);
	return g_console.writeString(message);
#elif(USING_UILIB_TYPE==1)//QT


#else//wxWidget


#endif	
}

bool CONSOLE_OUTPUT(unsigned int Attrs,const char * fmt,...)
{
#if(USING_UILIB_TYPE==0)//MFC
	TCHAR   message[MAX_BUF_LEN];
	va_list cur_arg;
	va_start(cur_arg,fmt);
	_vsntprintf(message,MAX_BUF_LEN,fmt,cur_arg);
	va_end(cur_arg);
	return g_console.writeString(Attrs,message); 
#elif(USING_UILIB_TYPE==1)//QT


#else//wxWidget


#endif
}

#else //ENABLE_CONSOLE_OUTPUT==0
bool CONSOLE_OUTPUT(const char* fmt, ...){ return false; }
bool CONSOLE_OUTPUT(unsigned int Attrs, const char * fmt, ...){
	return false;
}
#endif

end_gdb_namespace
end_gtl_namespace


