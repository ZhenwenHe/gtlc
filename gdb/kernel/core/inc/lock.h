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
#include <atomic>
#include <mutex>
#include <thread>
#include "feature.h"

begin_gtl_namespace
begin_gdb_namespace

typedef   std::mutex                         Mutex;
typedef   std::unique_lock<std::mutex>  UniqueLock;

/** @defgroup Thread  Thread-多线程相关类
*  @{
*/
class CORE_API Lock{
public:
	static void lock();
	static void unlock();
	static bool tryLock();
	static UniqueLock & getUniqueLock();
};
/** @} */

/** @addtogroup Thread  Mutex
*  @{
*/
//class CORE_API Mutex:public Lock{
//	void *    _hMutex;            // used to lock/unlock object
//public:
//	Mutex();
//	virtual ~Mutex();
//	static Mutex * create();
//	static void release(Mutex *);
//	void Lock::lock();
//	void Lock::unlock();
//};
/** @} */

/** @addtogroup Thread  CriticalSection
*  @{
*/
//class CORE_API CriticalSection  {
//public:
//	typedef CRITICAL_SECTION  critical_section_type;
//protected:
//	critical_section_type   _sect;
//public:
//	CriticalSection();
//	virtual ~CriticalSection();
//	void Lock::lock();
//	void Lock::unlock();
//};
/** @} */

/** @addtogroup Thread  Semaphore
*  @{
*/
//class CORE_API Semaphore:public Lock {
//	void * _sema;
//public:
//	Semaphore( int init = 0 );
//	virtual ~Semaphore();
//	void wait() const;
//	int post() const;
//	int getValue() const;
//	void reset( int init = 0 );
//};
/** @} */
//class  ThreadPool;
//class Thread;
//
///** @addtogroup Thread  ThreadProcedure
//*  @{
//*/
//class CORE_API ThreadProcedure{
//public:
//	virtual  long run( )=0;
//};
//typedef std::shared_ptr<ThreadProcedure> ThreadProcSharedPtr;
///** @} */
//
///** @addtogroup Thread  Thread
//*  @{
//*/
//class CORE_API Thread{
//protected:
//	unsigned _id;
//	string   _name;
//	void *   _handle;
//	ThreadProcSharedPtr _proc;
//public:
//	Thread(string name = "");
//	Thread(string name ,ThreadProcSharedPtr  p );
//	virtual ~ Thread();
//	unsigned getID();
//	string getName();
//	void setName(string name);
//	void setProcedure(ThreadProcSharedPtr p);
//	ThreadProcSharedPtr getProcedure();
//	unsigned long resume();
//
//public:
//	virtual long run();
//public:
//	static unsigned __stdcall threadEntryPoint (void * p);
//
//	friend  ThreadPool;
//};
//typedef std::shared_ptr<Thread> ThreadSharedPtr;
///** @} */
//
///** @addtogroup Thread  ThreadPool
//*  @{
//*/
//class CORE_API ThreadPool{
//protected:
//	std::vector<ThreadSharedPtr> _pool;
//public:
//	ThreadPool();
//	virtual ~ThreadPool();
//	std::vector<ThreadSharedPtr> & getPool();
//	ThreadSharedPtr createThread(string name,  ThreadProcSharedPtr p,bool bSuspended=false);
//};
//
//typedef std::shared_ptr<ThreadPool> ThreadPoolSharedPtr;
/** @} */
//
///** @@defgroup  Console  Console
//*  @{
//*/
//#if(ENABLE_CONSOLE_OUTPUT!=0)
//class CORE_API Console
//{
//public:
//	Console();
//	virtual ~Console();
//
//public:
//	static bool print(const char * fmt,...);
//	static bool print(unsigned int Attrs,const char * fmt,...);
//	static bool setTile(const char * lpTile);
//public:
//
//	bool writeString(const string&  lpString);
//	bool writeString(unsigned int Attrs,const string &  lpString);
//private:
//	void * m_hConsole;
//	bool   m_bCreate;
//	bool   m_bAttrs;
//	unsigned int   m_oldColorAttrs;
//};
//#define  CONSOLE_OUTPUT  Console::print
//#else
//#define  CONSOLE_OUTPUT
//#endif
///** @}*/
end_gdb_namespace
end_gtl_namespace

