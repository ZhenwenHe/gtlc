// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "src/imageimpl.h"
#include "factory.h"
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		gtl::gdb::Factory::setFactory(gtl::gdb::FactorySharedPtr(
			(gtl::gdb::Factory*) new ImageFactoryImpl(),
			ImageFactoryDeletor()));
		break;
	}
	case DLL_THREAD_ATTACH:
	{
		break;
	}
	case DLL_THREAD_DETACH:
	{
		break;
	}
	case DLL_PROCESS_DETACH:
		gtl::gdb::Factory::getFactoryMap()[gtl::gdb::Factory::FACTORYTYPE_IMAGE] =
			gtl::gdb::FactorySharedPtr();

		break;
	}
	return TRUE;
}

