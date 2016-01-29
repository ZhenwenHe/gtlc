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

end_gtl_namespace