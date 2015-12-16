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
 
begin_gtl_namespace
template<typename CHARTYPE,typename TRAITS, typename ALLOC >
MemoryStorageManager<CHARTYPE, TRAITS, ALLOC>::MemoryStorageManager()
{
}
template<typename CHARTYPE,typename TRAITS, typename ALLOC >
MemoryStorageManager<CHARTYPE, TRAITS, ALLOC>::~MemoryStorageManager()
{
	for (std::vector<Entry*>::iterator it = m_buffer.begin(); it != m_buffer.end(); ++it) delete *it;
}
template<typename CHARTYPE,typename TRAITS, typename ALLOC >
void MemoryStorageManager<CHARTYPE, TRAITS, ALLOC>::load(const int64_t page, uint32_t& len, unsigned char** data)
{
	Entry* e;
	try
	{
		e = m_buffer.at((size_t)page);
		if (e == 0) throw "Invalid Page Exception";
	}
	catch (std::out_of_range)
	{
		throw "Invalid Page Exception";
	}

	len = e->m_length;
	*data = new unsigned char[len];

	memcpy(*data, e->m_pData, len);
}
template<typename CHARTYPE,typename TRAITS, typename ALLOC >
void MemoryStorageManager<CHARTYPE, TRAITS, ALLOC>::store(int64_t& page, const uint32_t len, const unsigned char* const data)
{
	if (page == StorageManager::NewPage)
	{
		Entry* e = new Entry(len, data);

		if (m_emptyPages.empty())
		{
			m_buffer.push_back(e);
			page = m_buffer.size() - 1;
		}
		else
		{
			page = m_emptyPages.top(); m_emptyPages.pop();
			m_buffer[page] = e;
		}
	}
	else
	{
		Entry* e_old;
		try
		{
			e_old = m_buffer.at(page);
			if (e_old == 0) throw "Invalid Page Exception";
		}
		catch (std::out_of_range)
		{
			throw "Invalid Page Exception";
		}

		Entry* e = new Entry(len, data);

		delete e_old;
		m_buffer[page] = e;
	}
}
template<typename CHARTYPE,typename TRAITS, typename ALLOC >
void MemoryStorageManager<CHARTYPE, TRAITS, ALLOC>::erase(const int64_t page)
{
	Entry* e;
	try
	{
		e = m_buffer.at(page);
		if (e == 0) throw "Invalid Page Exception";
	}
	catch (std::out_of_range)
	{
		throw "Invalid Page Exception";
	}

	m_buffer[page] = 0;
	m_emptyPages.push(page);

	delete e;
}

template<typename CHARTYPE,typename TRAITS, typename ALLOC >
bool CheckFilesExists(
	const std::basic_string<CHARTYPE, TRAITS, ALLOC> & sIndexFile,
	const std::basic_string<CHARTYPE, TRAITS, ALLOC> & sDataFile )
{
	bool bExists = false;
	struct stat stats;	
	int ret = stat(sDataFile.c_str(), &stats);
	if (ret == 0) bExists = true;	 
	ret = stat(sIndexFile.c_str(), &stats);
	if ((ret == 0) && (bExists == true)) bExists = true;
	return bExists;
}
template<typename CHARTYPE,typename TRAITS, typename ALLOC >
DiskStorageManager<CHARTYPE, TRAITS, ALLOC>::DiskStorageManager(
	const std::basic_string<CHARTYPE, TRAITS, ALLOC> & sIndexFile,
	const std::basic_string<CHARTYPE, TRAITS, ALLOC> & sDataFile,
	const uint32_t PageSize,
	bool bOverwrite
	) : m_pageSize(0), m_nextPage(-1), m_buffer(0)
{
	// check if file exists.
	bool bFileExists = CheckFilesExists(sIndexFile,sDataFile);

	// check if file can be read/written.
	if (bFileExists == true && bOverwrite == false)
	{
		m_indexFile.open(sIndexFile.c_str(), std::ios::in | std::ios::out | std::ios::binary);
		m_dataFile.open(sDataFile.c_str(), std::ios::in | std::ios::out | std::ios::binary);

		if (m_indexFile.fail() || m_dataFile.fail())
			throw "Index/Data file cannot be read/writen.";
	}
	else
	{
		m_indexFile.open(sIndexFile.c_str(), std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
		m_dataFile.open(sDataFile.c_str(), std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);

		if (m_indexFile.fail() || m_dataFile.fail())
			throw "Index/Data file cannot be created.";

	}


	// find page size.
	if (bOverwrite == true)
	{
		m_pageSize = PageSize;
		m_nextPage = 0;
	}
	else
	{
		m_indexFile.read(reinterpret_cast<char*>(&m_pageSize), sizeof(uint32_t));
		if (m_indexFile.fail())
			throw "Failed reading pageSize.";

		m_indexFile.read(reinterpret_cast<char*>(&m_nextPage), sizeof(uint64_t));
		if (m_indexFile.fail())
			throw "Failed reading nextPage.";
	}

	// create buffer.
	m_buffer = new unsigned char[m_pageSize];
	memset(m_buffer, 0, m_pageSize);

	if (bOverwrite == false)
	{
		uint32_t count;
		uint64_t page, id;

		// load empty pages in memory.
		m_indexFile.read(reinterpret_cast<char*>(&count), sizeof(uint32_t));
		if (m_indexFile.fail())
			throw "Corrupted storage manager index file.";

		for (uint32_t cCount = 0; cCount < count; ++cCount)
		{
			m_indexFile.read(reinterpret_cast<char*>(&page), sizeof(uint64_t));
			if (m_indexFile.fail())
				throw "Corrupted storage manager index file.";
			m_emptyPages.push(page);
		}

		// load index table in memory.
		m_indexFile.read(reinterpret_cast<char*>(&count), sizeof(uint32_t));
		if (m_indexFile.fail())
			throw "Corrupted storage manager index file.";

		for (uint32_t cCount = 0; cCount < count; ++cCount)
		{
			Entry* e = new Entry();

			m_indexFile.read(reinterpret_cast<char*>(&id), sizeof(uint64_t));
			if (m_indexFile.fail())
				throw "Corrupted storage manager index file.";

			m_indexFile.read(reinterpret_cast<char*>(&(e->m_length)), sizeof(uint32_t));
			if (m_indexFile.fail())
				throw "Corrupted storage manager index file.";

			uint32_t count2;
			m_indexFile.read(reinterpret_cast<char*>(&count2), sizeof(uint32_t));
			if (m_indexFile.fail())
				throw "Corrupted storage manager index file.";

			for (uint32_t cCount2 = 0; cCount2 < count2; ++cCount2)
			{
				m_indexFile.read(reinterpret_cast<char*>(&page), sizeof(uint64_t));
				if (m_indexFile.fail())
					throw "Corrupted storage manager index file.";
				e->m_pages.push_back(page);
			}
			m_pageIndex.insert(std::pair<uint64_t, Entry* >(id, e));
		}
	}
}
template<typename CHARTYPE,typename TRAITS, typename ALLOC >
DiskStorageManager<CHARTYPE, TRAITS, ALLOC>::~DiskStorageManager()
{
	flush();
	m_indexFile.close();
	m_dataFile.close();
	if (m_buffer != 0) delete[] m_buffer;

	std::map<uint64_t, Entry*>::iterator it;
	for (it = m_pageIndex.begin(); it != m_pageIndex.end(); ++it) delete (*it).second;
}
template<typename CHARTYPE,typename TRAITS, typename ALLOC >
void DiskStorageManager<CHARTYPE, TRAITS, ALLOC>::flush()
{
	m_indexFile.seekp(0, std::ios_base::beg);
	if (m_indexFile.fail())
		throw "Corrupted storage manager index file.";

	m_indexFile.write(reinterpret_cast<const char*>(&m_pageSize), sizeof(uint32_t));
	if (m_indexFile.fail())
		throw "Corrupted storage manager index file.";

	m_indexFile.write(reinterpret_cast<const char*>(&m_nextPage), sizeof(uint64_t));
	if (m_indexFile.fail())
		throw "Corrupted storage manager index file.";

	uint32_t count = static_cast<uint32_t>(m_emptyPages.size());
	uint64_t page, id;

	m_indexFile.write(reinterpret_cast<const char*>(&count), sizeof(uint32_t));
	if (m_indexFile.fail())
		throw "Corrupted storage manager index file.";

	while (! m_emptyPages.empty())
	{
		page = m_emptyPages.top(); m_emptyPages.pop();
		m_indexFile.write(reinterpret_cast<const char*>(&page), sizeof(uint64_t));
		if (m_indexFile.fail())
			throw "Corrupted storage manager index file.";
	}

	count = static_cast<uint32_t>(m_pageIndex.size());

	m_indexFile.write(reinterpret_cast<const char*>(&count), sizeof(uint32_t));
	if (m_indexFile.fail())
		throw "Corrupted storage manager index file.";

	std::map<uint64_t, Entry*>::iterator it;

	for (it = m_pageIndex.begin(); it != m_pageIndex.end(); ++it)
	{
		id = (*it).first;
		m_indexFile.write(reinterpret_cast<const char*>(&id), sizeof(uint64_t));
		if (m_indexFile.fail())
			throw "Corrupted storage manager index file.";

		uint32_t length = (*it).second->m_length;
		m_indexFile.write(reinterpret_cast<const char*>(&length), sizeof(uint32_t));
		if (m_indexFile.fail())
			throw "Corrupted storage manager index file.";

		count = static_cast<uint32_t>((*it).second->m_pages.size());
		m_indexFile.write(reinterpret_cast<const char*>(&count), sizeof(uint32_t));
		if (m_indexFile.fail())
			throw "Corrupted storage manager index file.";

		for (uint32_t cIndex = 0; cIndex < count; ++cIndex)
		{
			page = (*it).second->m_pages[cIndex];
			m_indexFile.write(reinterpret_cast<const char*>(&page), sizeof(uint64_t));
			if (m_indexFile.fail())
				throw "Corrupted storage manager index file.";
		}
	}

	m_indexFile.flush();
	m_dataFile.flush();
}
template<typename CHARTYPE,typename TRAITS, typename ALLOC >
void DiskStorageManager<CHARTYPE, TRAITS, ALLOC>::load(const int64_t page, uint32_t& len, unsigned char** data)
{
	std::map<uint64_t, Entry*>::iterator it = m_pageIndex.find(page);

	if (it == m_pageIndex.end())
		throw "InvalidPageException(page)";

	std::vector<uint64_t>& pages = (*it).second->m_pages;
	uint32_t cNext = 0;
	uint32_t cTotal = static_cast<uint32_t>(pages.size());

	len = (*it).second->m_length;
	*data = new unsigned char[len];

	unsigned char* ptr = *data;
	uint32_t cLen;
	uint32_t cRem = len;

	do
	{
		m_dataFile.seekg(pages[cNext] * m_pageSize, std::ios_base::beg);
		if (m_dataFile.fail())
			throw "Corrupted data file.";

		m_dataFile.read(reinterpret_cast<char*>(m_buffer), m_pageSize);
		if (m_dataFile.fail())
			throw "Corrupted data file.";

		cLen = (cRem > m_pageSize) ? m_pageSize : cRem;
		memcpy(ptr, m_buffer, cLen);

		ptr += cLen;
		cRem -= cLen;
		++cNext;
	}
	while (cNext < cTotal);
}
template<typename CHARTYPE,typename TRAITS, typename ALLOC >
void DiskStorageManager<CHARTYPE, TRAITS, ALLOC>::store(int64_t& page, const uint32_t len, const unsigned char* const data)
{
	if (page == StorageManager::NewPage)
	{
		Entry* e = new Entry();
		e->m_length = len;

		const unsigned char* ptr = data;
		uint64_t cPage;
		uint32_t cRem = len;
		uint32_t cLen;

		while (cRem > 0)
		{
			if (! m_emptyPages.empty())
			{
				cPage = m_emptyPages.top(); m_emptyPages.pop();
			}
			else
			{
				cPage = m_nextPage;
				++m_nextPage;
			}

			cLen = (cRem > m_pageSize) ? m_pageSize : cRem;
			memcpy(m_buffer, ptr, cLen);

			m_dataFile.seekp(cPage * m_pageSize, std::ios_base::beg);
			if (m_dataFile.fail())
				throw "Corrupted data file.";

			m_dataFile.write(reinterpret_cast<const char*>(m_buffer), m_pageSize);
			if (m_dataFile.fail())
				throw "Corrupted data file.";

			ptr += cLen;
			cRem -= cLen;
			e->m_pages.push_back(cPage);
		}

		page = e->m_pages[0];
		m_pageIndex.insert(std::pair<uint64_t, Entry*>(page, e));
	}
	else
	{
		// find the entry.
		std::map<uint64_t, Entry*>::iterator it = m_pageIndex.find(page);

		// check if it exists.
		if (it == m_pageIndex.end())
			throw "InvalidPageException(page)";

		Entry* oldEntry = (*it).second;

		m_pageIndex.erase(it);

		Entry* e = new Entry();
		e->m_length = len;

		const unsigned char* ptr = data;
		uint64_t cPage;
		uint32_t cRem = len;
		uint32_t cLen, cNext = 0;

		while (cRem > 0)
		{
			if (cNext < oldEntry->m_pages.size())
			{
				cPage = oldEntry->m_pages[cNext];
				++cNext;
			}
			else if (! m_emptyPages.empty())
			{
				cPage = m_emptyPages.top(); m_emptyPages.pop();
			}
			else
			{
				cPage = m_nextPage;
				++m_nextPage;
			}

			cLen = (cRem > m_pageSize) ? m_pageSize : cRem;
			memcpy(m_buffer, ptr, cLen);

			m_dataFile.seekp(cPage * m_pageSize, std::ios_base::beg);
			if (m_dataFile.fail())
				throw "Corrupted data file.";

			m_dataFile.write(reinterpret_cast<const char*>(m_buffer), m_pageSize);
			if (m_dataFile.fail())
				throw "Corrupted data file.";

			ptr += cLen;
			cRem -= cLen;
			e->m_pages.push_back(cPage);
		}

		while (cNext < oldEntry->m_pages.size())
		{
			m_emptyPages.push(oldEntry->m_pages[cNext]);
			++cNext;
		}

		m_pageIndex.insert(std::pair<uint64_t, Entry*>(page, e));
		delete oldEntry;
	}
}
template<typename CHARTYPE,typename TRAITS, typename ALLOC >
void DiskStorageManager<CHARTYPE, TRAITS, ALLOC>::erase(const int64_t page)
{
	std::map<uint64_t, Entry*>::iterator it = m_pageIndex.find(page);

	if (it == m_pageIndex.end())
		throw "InvalidPageException(page)";

	for (uint32_t cIndex = 0; cIndex < (*it).second->m_pages.size(); ++cIndex)
	{
		m_emptyPages.push((*it).second->m_pages[cIndex]);
	}

	delete (*it).second;
	m_pageIndex.erase(it);
}

end_gtl_namespace
