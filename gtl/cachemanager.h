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
/*
缓存默认采用的是随机剔除替换算法，如果需要更换替换算法，可以重载removeEntry虚函数
*/
#pragma  once
#include <map>
#include <stack>
#include <queue>
#include "random.h"
#include <assert.h>
#include "StorageManager.h"  

begin_gtl_namespace
template<typename  ELEMENT,typename TRAITS=std::char_traits<ELEMENT> ,typename ALLOC=std::allocator<ELEMENT> >
class CacheManager: public StorageManager<ELEMENT,TRAITS,ALLOC>
{	
public:
	CacheManager( StorageManager<ELEMENT,TRAITS,ALLOC> * sm, unsigned int capacity=10, bool write_through=false){
		m_u64Hits=0;
		m_capacity=capacity;
		m_bWriteThrough =write_through;
		m_pStorageManager=sm;
	}
	virtual ~CacheManager(){
		for (std::map<int64_t, Entry*>::iterator it = m_buffer.begin(); it != m_buffer.end(); ++it)
		{
			if ((*it).second->m_bDirty)
			{
				int64_t page = (*it).first;
				m_pStorageManager->store(page, (*it).second->m_length, (*it).second->m_pData);
			}
			delete (*it).second;
		}
	}
	virtual void load(const int64_t page, unsigned int& len, unsigned char** data) 
	{
		std::map<int64_t, Entry*>::iterator it = m_buffer.find(page);

		if (it != m_buffer.end())
		{
			++m_u64Hits;
			len = (*it).second->m_length;
			*data = new unsigned char[len];
			memcpy(*data, (*it).second->m_pData, len);
		}
		else
		{
			m_pStorageManager->load(page, len, data);
			addEntry(page, new Entry(len, static_cast<const unsigned char*>(*data)));
		}
	}
	virtual void store(int64_t& page, const unsigned int len, const unsigned char* const data)
	{
		if (page == NewPage)
		{
			m_pStorageManager->store(page, len, data);
			assert(m_buffer.find(page) == m_buffer.end());
			addEntry(page, new Entry(len, data));
		}
		else
		{
			if (m_bWriteThrough)
			{
				m_pStorageManager->store(page, len, data);
			}

			Entry* e = new Entry(len, data);
			if (m_bWriteThrough == false) e->m_bDirty = true;

			std::map<int64_t, Entry*>::iterator it = m_buffer.find(page);
			if (it != m_buffer.end())
			{
				delete (*it).second;
				(*it).second = e;
				if (m_bWriteThrough == false) ++m_u64Hits;
			}
			else
			{
				addEntry(page, e);
			}
		}
	}
	virtual void erase(const int64_t page) 
	{
		std::map<int64_t, Entry*>::iterator it = m_buffer.find(page);
		if (it != m_buffer.end())
		{
			delete (*it).second;
			m_buffer.erase(it);
		}

		m_pStorageManager->erase(page);
	}

	virtual void clear()
	{
		for (std::map<int64_t, Entry*>::iterator it = m_buffer.begin(); it != m_buffer.end(); ++it)
		{
			if ((*it).second->m_bDirty)
			{
				int64_t page = (*it).first;
				m_pStorageManager->store(page, ((*it).second)->m_length, static_cast<const unsigned char*>(((*it).second)->m_pData));
			}

			delete (*it).second;
		}

		m_buffer.clear();
		m_u64Hits = 0;
	}

	virtual uint64_t getHits()
	{
		return m_u64Hits;
	}

protected:
	class Entry
	{
	public:
		Entry(unsigned int l, const unsigned char* const d) : m_pData(0), m_length(l), m_bDirty(false)
		{
			m_pData = new unsigned char[m_length];
			memcpy(m_pData, d, m_length);
		}

		~Entry() { delete[] m_pData; }

		unsigned char* m_pData;
		unsigned int m_length;
		bool m_bDirty;
	}; // Entry
 
	unsigned int m_capacity;
	bool m_bWriteThrough;
	StorageManager<ELEMENT,TRAITS,ALLOC>* m_pStorageManager;
	std::map<int64_t, Entry*> m_buffer;
	uint64_t m_u64Hits; 
protected:
	virtual void addEntry(int64_t page, Entry* e)
	{
		assert(m_buffer.size() <= m_capacity);
		if (m_buffer.size() == m_capacity) removeEntry();
		assert(m_buffer.find(page) == m_buffer.end());
		m_buffer.insert(std::pair<int64_t, Entry*>(page, e));
	}

	virtual void removeEntry()
	{
		if (m_buffer.size() == 0) return;

		double random;

		random = Random::drand48();

		unsigned int entry = static_cast<unsigned int>(floor(((double) m_buffer.size()) * random));

		std::map<int64_t, Entry*>::iterator it = m_buffer.begin();
		for (unsigned int cIndex = 0; cIndex < entry; cIndex++) ++it;

		if ((*it).second->m_bDirty)
		{
			int64_t page = (*it).first;
			m_pStorageManager->store(page, ((*it).second)->m_length, (const unsigned char *) ((*it).second)->m_pData);
		}

		delete (*it).second;
		m_buffer.erase(it);
	}

};  
end_gtl_namespace
