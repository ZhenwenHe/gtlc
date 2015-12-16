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
#include <map>
#include <stack>
#include <queue>
#include "gtl.h" 
#include "stdint.h"
#include <iosfwd>
#include <fstream>
// For checking if a file exists  
#include <sys/stat.h>
#include "object.h"
begin_gtl_namespace
template<typename  CHARTYPE,typename TRAITS=std::char_traits<CHARTYPE>,typename ALLOC=std::allocator<CHARTYPE>  >
class StorageManager: public Object<CHARTYPE,TRAITS,ALLOC>
{
public:
	virtual std::basic_string<CHARTYPE,TRAITS,ALLOC> getClassName(){
		return "StorageManager";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
public:
	enum StorageManagerConstants
	{
		EmptyPage = -0x1,
		NewPage = -0x1
	};
public:
	virtual void load(const int64_t id, uint32_t& len, unsigned char** data) = 0;
	virtual void store(int64_t& id, const uint32_t len, const unsigned char* const data) = 0;
	virtual void erase(const int64_t id) = 0;
	virtual ~StorageManager() {}
};  

///////////////////////////////////////////////////////////////////////////////
template<typename  CHARTYPE,typename TRAITS=std::char_traits<CHARTYPE>,typename ALLOC=std::allocator<CHARTYPE>  >
class MemoryStorageManager : public  StorageManager<CHARTYPE,TRAITS,ALLOC>
{
public:
	virtual std::basic_string<CHARTYPE,TRAITS,ALLOC> getClassName(){
		return "MemoryStorageManager";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		assert(false);
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){
		assert(false);
	}
	virtual Object * clone() {
		assert(false);
		return 0;
	}
public:
	MemoryStorageManager();

	virtual ~MemoryStorageManager();

	virtual void load(const int64_t page, uint32_t& len, unsigned char** data);
	virtual void store(int64_t& page, const uint32_t len, const unsigned char* const data);
	virtual void erase(const int64_t page);

private:
	class Entry
	{
	public:
		unsigned char* m_pData;
		uint32_t m_length;

		Entry(uint32_t l, const unsigned char* const d) : m_pData(0), m_length(l)
		{
			m_pData = new unsigned char[m_length];
			memcpy(m_pData, d, m_length);
		}

		~Entry() { delete[] m_pData; }
	}; // Entry

	std::vector<Entry*> m_buffer;
	std::stack<uint64_t> m_emptyPages;
}; // MemoryStorageManager
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////
template<typename  CHARTYPE,typename TRAITS=std::char_traits<CHARTYPE>,typename ALLOC=std::allocator<CHARTYPE>  >
class DiskStorageManager : public StorageManager<CHARTYPE,TRAITS,ALLOC>
{
public:
	virtual std::basic_string<CHARTYPE,TRAITS,ALLOC> getClassName(){
		return "DiskStorageManager";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		assert(false);
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){
		assert(false);
	}
	virtual Object * clone() {
		assert(false);
		return 0;
	}
public:
	DiskStorageManager(const std::basic_string<CHARTYPE,TRAITS,ALLOC> & sIndexFile,
		const std::basic_string<CHARTYPE,TRAITS,ALLOC> & sDataFile,
		const uint32_t PageSize,
		bool bOverwrite=false);
	virtual ~DiskStorageManager();

	void flush();

	virtual void load(const int64_t page, uint32_t& len, unsigned char** data);
	virtual void store(int64_t& page, const uint32_t len, const unsigned char* const data);
	virtual void erase(const int64_t page);

private:
	class Entry
	{
	public:
		uint32_t m_length;
		std::vector<uint64_t> m_pages;
	};

	std::basic_fstream<CHARTYPE, TRAITS> m_dataFile;
	std::basic_fstream<CHARTYPE, TRAITS> m_indexFile;
	uint32_t m_pageSize;
	uint64_t m_nextPage;
	std::priority_queue<uint64_t, std::vector<uint64_t>, std::greater<uint64_t> > m_emptyPages;
	std::map<uint64_t, Entry*> m_pageIndex;

	unsigned char* m_buffer;
}; // DiskStorageManager
//////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////

end_gtl_namespace
#include "storagemanager.inl"