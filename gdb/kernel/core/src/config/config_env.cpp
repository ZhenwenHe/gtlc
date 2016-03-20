#include <stdlib.h>
#include <stdio.h>
#include "config.h"

begin_gtl_namespace

std::string getHome(){
    char * s = getenv("GTL_HOME");
    if(s){
        return std::string((const char*)s);
    }
    else{
#if(USING_OS_TYPE==0)
        return std::string("H:\\gtl\\gtl");
#elif(USING_OS_TYPE==1)
        return std::string("/home/vincent/gtl");
#else
        return std::string("/home/vincent/gtl");
#endif
    }
}

std::string getDataHome(){
    std::string s = getHome();
#if(USING_OS_TYPE==0)
    s=s+"\\data";
#elif(USING_OS_TYPE==1)
    s=s+"/data";
#else
    s=s+"/data";
#endif
    return s;
}

/* get the full path name */
std::string getDataFile(const char * filename) {
	std::string s = getDataHome();
#if(USING_OS_TYPE==0)
	s = s + "\\";
#elif(USING_OS_TYPE==1)
	s = s + "/";
#else
	s = s + "/";
#endif
	s = s + std::string(filename);
	return s;
}
std::string getInstallHome(){
    char * s = getenv("GTL_INSTALL_HOME");
    if(s){
        return std::string((const char*)s);
    }
    else{
        std::string sz = getHome();
#if(USING_OS_TYPE==0)        //windows
        char path[512];
	int size = ::GetModuleFileName(NULL,path,512);
	std::string szModPath = string(path,size);
	size_t pos = szModPath.find_last_of("\\");
	sz = szModPath.substr(0,pos);
#elif(USING_OS_TYPE==1) //linux
        sz=sz+"/sdk";
#else                   //macX OS
        sz=sz+"/sdk";
#endif
        return sz;
    }
}


#if(USING_UI_TYPE==1)//Qt : use Qt QLibrary
/*Fetch a function pointer from a shared library / DLL.*/
void * getFunctionPointer(const char * pszLibrary, const char * pszSymbolName) {
	void        *pSymbol=0;
	//pSymbol = (void *)QLibrary::revolve(pszLibrary, pszSymbolName);
	return pSymbol;
}
#else
#if(USING_OS_TYPE==0)        //windows
#include <windows.h>
/*Fetch a function pointer from a shared library / DLL.*/
void * getFunctionPointer(const char * pszLibrary, const char * pszSymbolName) {
	void        *pLibrary;
	void        *pSymbol;

	pLibrary = LoadLibrary(pszLibrary);
	if (pLibrary == NULL)
	{
		LPVOID      lpMsgBuf = NULL;
		int         nLastError = GetLastError();

		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER
			| FORMAT_MESSAGE_FROM_SYSTEM
			| FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, nLastError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf, 0, NULL);

		return NULL;
	}

	pSymbol = (void *)GetProcAddress((HINSTANCE)pLibrary, pszSymbolName);

	if (pSymbol == NULL)
	{
		return NULL;
	}

	return(pSymbol);
}
#elif(USING_OS_TYPE==1) //linux
#define GOT_GETSYMBOL
#include <dlfcn.h>
/*Fetch a function pointer from a shared library / DLL.*/
void * getFunctionPointer(const char * pszLibrary, const char * pszSymbolName) {
	void        *pLibrary;
	void        *pSymbol;

	pLibrary = dlopen(pszLibrary, RTLD_LAZY);
	if (pLibrary == NULL)
	{
		return NULL;
	}

	pSymbol = dlsym(pLibrary, pszSymbolName);
	if (pSymbol == NULL)
	{
		return NULL;
	}

	return(pSymbol);
}
#else                   //macX OS
/*Fetch a function pointer from a shared library / DLL.*/
void * getFunctionPointer(const char * pszLibrary, const char * pszSymbolName) {
	void        *pLibrary;
	void        *pSymbol;

	pLibrary = dlopen(pszLibrary, RTLD_LAZY);
	if (pLibrary == NULL)
	{
		return NULL;
	}

	pSymbol = dlsym(pLibrary, pszSymbolName);

#if (defined(__APPLE__) && defined(__MACH__))
	/* On mach-o systems, C symbols have a leading underscore and depending
	* on how dlcompat is configured it may or may not add the leading
	* underscore.  So if dlsym() fails add an underscore and try again.
	*/
	if (pSymbol == NULL)
	{
		char withUnder[strlen(pszSymbolName) + 2];
		withUnder[0] = '_'; withUnder[1] = 0;
		strcat(withUnder, pszSymbolName);
		pSymbol = dlsym(pLibrary, withUnder);
	}
#endif

	if (pSymbol == NULL)
	{
		return NULL;
	}

	return(pSymbol);
}
#endif

#endif

end_gtl_namespace
