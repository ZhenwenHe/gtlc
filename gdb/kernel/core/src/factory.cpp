#include "factory.h"


begin_gtl_namespace
begin_gdb_namespace

std::map<int,FactorySharedPtr> g_Factories;


Factory::Factory(){

}
Factory::~Factory(){

}
///////////////////////////////////////////////////////////////
void Factory::setFactory(FactorySharedPtr p){
	g_Factories[p->getType()]=p;
}
Factory * Factory::getFactory( int t){
	return g_Factories[t].get();
}
std::map<int,FactorySharedPtr> &   Factory::getFactoryMap(){
	return g_Factories;
}
//void*  Factory::loadModule(const char * render_lib){
//	return (void*) ::LoadLibrary(render_lib);
//}
//
//void Factory::freeModule(void * module)
//{
//	if(module)
//		::FreeLibrary((HMODULE)module);
//}

end_gdb_namespace
end_gtl_namespace


