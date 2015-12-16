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
	///////////////////////////////////////////////////////////////////////////////
	//
	// BufferedFile
	//
template<typename ELEMENT,typename TRAITS, typename ALLOC >
BufferedFile<ELEMENT,TRAITS,ALLOC>::BufferedFile(uint32_t u32BufferSize)
	: m_buffer(new ELEMENT[u32BufferSize]), m_u32BufferSize(u32BufferSize), m_bEOF(true)
{
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
BufferedFile<ELEMENT,TRAITS,ALLOC>::~BufferedFile()
{
	m_file.close();
	delete[] m_buffer;
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void BufferedFile<ELEMENT,TRAITS,ALLOC>::close()
{
	m_file.close();
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
bool BufferedFile<ELEMENT,TRAITS,ALLOC>::eof()
{
	return m_bEOF;
}

//
// BufferedFileReader
//
template<typename ELEMENT,typename TRAITS, typename ALLOC >
BufferedFileReader<ELEMENT,TRAITS,ALLOC>::BufferedFileReader()
{
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
BufferedFileReader<ELEMENT,TRAITS,ALLOC>::BufferedFileReader(const std::basic_string<ELEMENT,TRAITS,ALLOC>& sFileName, uint32_t u32BufferSize)
	: BufferedFile<ELEMENT,TRAITS,ALLOC>(u32BufferSize)
{
	open(sFileName);
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void BufferedFileReader<ELEMENT,TRAITS,ALLOC>::open(const std::basic_string<ELEMENT,TRAITS,ALLOC>& sFileName)
{
	m_bEOF = false;
	m_file.close(); m_file.clear();


	m_file.open(sFileName.c_str(), std::ios_base::in | std::ios_base::binary);
	if (! m_file.good())
		throw std::ios_base::failure("BufferedFileReader<ELEMENT,TRAITS,ALLOC>::BufferedFileReader: Cannot open file.");

	m_file.rdbuf()->pubsetbuf(m_buffer, m_u32BufferSize);
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
BufferedFileReader<ELEMENT,TRAITS,ALLOC>::~BufferedFileReader()
{
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void BufferedFileReader<ELEMENT,TRAITS,ALLOC>::rewind()
{
	m_file.clear();
	m_file.seekg(0, std::ios_base::beg);
	if (! m_file.good())
		throw std::ios_base::failure("BufferedFileReader<ELEMENT,TRAITS,ALLOC>::rewind: seek failed.");

	m_bEOF = false;
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void BufferedFileReader<ELEMENT,TRAITS,ALLOC>::seek(std::size_t offset)
{
	m_bEOF = false;
	m_file.clear();
	m_file.seekg(offset, std::ios_base::beg);
	if (! m_file.good())
		throw std::ios_base::failure("BufferedFileReader<ELEMENT,TRAITS,ALLOC>::seek: seek failed.");
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
uint8_t BufferedFileReader<ELEMENT,TRAITS,ALLOC>::readUInt8()
{
	if (m_bEOF) throw "End of file";

	uint8_t ret;
	m_file.read(reinterpret_cast<ELEMENT*>(&ret), sizeof(uint8_t));
	if (! m_file.good())
	{
		m_bEOF = true;
		throw   "End of file";
	}
	return ret;
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
uint16_t BufferedFileReader<ELEMENT,TRAITS,ALLOC>::readUInt16()
{
	if (m_bEOF) throw "End of file";

	uint16_t ret;
	m_file.read(reinterpret_cast<ELEMENT*>(&ret), sizeof(uint16_t));
	if (! m_file.good())
	{
		m_bEOF = true;
		throw "End of file";
	}
	return ret;
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
uint32_t BufferedFileReader<ELEMENT,TRAITS,ALLOC>::readUInt32()
{
	if (m_bEOF) throw "End of file";

	uint32_t ret;
	m_file.read(reinterpret_cast<ELEMENT*>(&ret), sizeof(uint32_t));
	if (! m_file.good())
	{
		m_bEOF = true;
		throw "End of file";
	}
	return ret;
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
uint64_t BufferedFileReader<ELEMENT,TRAITS,ALLOC>::readUInt64()
{
	if (m_bEOF) throw "End of file";

	uint64_t ret;
	m_file.read(reinterpret_cast<ELEMENT*>(&ret), sizeof(uint64_t));
	if (! m_file.good())
	{
		m_bEOF = true;
		throw "End of file";
	}
	return ret;
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
float BufferedFileReader<ELEMENT,TRAITS,ALLOC>::readFloat()
{
	if (m_bEOF) throw "End of file";

	float ret;
	m_file.read(reinterpret_cast<ELEMENT*>(&ret), sizeof(float));
	if (! m_file.good())
	{
		m_bEOF = true;
		throw "End of file";
	}
	return ret;
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
double BufferedFileReader<ELEMENT,TRAITS,ALLOC>::readDouble()
{
	if (m_bEOF) throw "End of file";

	double ret;
	m_file.read(reinterpret_cast<ELEMENT*>(&ret), sizeof(double));
	if (! m_file.good())
	{
		m_bEOF = true;
		throw "End of file";
	}
	return ret;
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
bool BufferedFileReader<ELEMENT,TRAITS,ALLOC>::readBoolean()
{
	if (m_bEOF) throw "End of file";

	bool ret;
	m_file.read(reinterpret_cast<ELEMENT*>(&ret), sizeof(bool));
	if (! m_file.good())
	{
		m_bEOF = true;
		throw "End of file";
	}
	return ret;
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
std::basic_string<ELEMENT,TRAITS,ALLOC> BufferedFileReader<ELEMENT,TRAITS,ALLOC>::readString()
{
	if (m_bEOF) throw "End of file";

	uint32_t len;
	m_file.read(reinterpret_cast<ELEMENT*>(&len), sizeof(uint32_t));
	if (! m_file.good())
	{
		m_bEOF = true;
		throw "End of file";
	}

	std::basic_string<ELEMENT,TRAITS,ALLOC>::value_type* buf = new std::basic_string<ELEMENT,TRAITS,ALLOC>::value_type[len];
	m_file.read(reinterpret_cast<ELEMENT*>(buf), len * sizeof(std::basic_string<ELEMENT,TRAITS,ALLOC>::value_type));
	if (! m_file.good())
	{
		delete[] buf;
		m_bEOF = true;
		throw "End of file";
	}

	std::basic_string<ELEMENT,TRAITS,ALLOC> ret(buf, len);
	delete[] buf;

	return ret;
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void BufferedFileReader<ELEMENT,TRAITS,ALLOC>::readBytes(uint32_t u32Len,  ELEMENT** pData)
{
	if (m_bEOF) throw "End of file";

	*pData = new unsigned ELEMENT[u32Len];
	m_file.read(reinterpret_cast<ELEMENT*>(*pData), u32Len);
	if (! m_file.good())
	{
		delete[] *pData;
		m_bEOF = true;
		throw "End of file";
	}
}

//
// BufferedFileWriter
//
template<typename ELEMENT,typename TRAITS, typename ALLOC >
BufferedFileWriter<ELEMENT, TRAITS,ALLOC>::BufferedFileWriter()
{
	open("");
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
BufferedFileWriter<ELEMENT, TRAITS,ALLOC>::BufferedFileWriter(const std::basic_string<ELEMENT,TRAITS,ALLOC>& sFileName, FileMode mode, uint32_t u32BufferSize)
	: BufferedFile<ELEMENT,TRAITS,ALLOC>(u32BufferSize)
{
	open(sFileName, mode);
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
BufferedFileWriter<ELEMENT, TRAITS,ALLOC>::~BufferedFileWriter()
{
	m_file.flush();
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void BufferedFileWriter<ELEMENT, TRAITS,ALLOC>::open(const std::basic_string<ELEMENT,TRAITS,ALLOC>& sFileName, FileMode mode)
{
	m_bEOF = false;
	m_file.close(); m_file.clear();

	if (mode == CREATE)
	{
		m_file.open(sFileName.c_str(), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
		if (! m_file.good())
			throw std::ios_base::failure("BufferedFileWriter<ELEMENT, TRAITS,ALLOC>::open: Cannot open file.");
	}
	else if (mode == APPEND)
	{
		// Idiotic fstream::open truncates an existing file anyway, if it is only opened
		// for output (no ios_base::in flag)!! On the other hand, if a file does not exist
		// and the ios_base::in flag is specified, then the open fails!!

		m_file.open(sFileName.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
		if (! m_file.good())
		{
			m_file.clear();
			m_file.open(sFileName.c_str(), std::ios_base::out | std::ios_base::binary);
			if (! m_file.good())
				throw std::ios_base::failure("BufferedFileWriter<ELEMENT, TRAITS,ALLOC>::open: Cannot open file.");
		}
		else
		{
			m_file.seekp(0, std::ios_base::end);
			if (! m_file.good())
				throw std::ios_base::failure("BufferedFileWriter<ELEMENT, TRAITS,ALLOC>::open: Cannot open file.");
		}
	}
	else
		throw "BufferedFileWriter<ELEMENT, TRAITS,ALLOC>::open: Unknown mode.";
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void BufferedFileWriter<ELEMENT, TRAITS,ALLOC>::rewind()
{
	m_bEOF = false;
	m_file.clear();
	m_file.seekp(0, std::ios_base::beg);
	if (! m_file.good())
		throw std::ios_base::failure("BufferedFileWriter<ELEMENT, TRAITS,ALLOC>::rewind: seek failed.");
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void BufferedFileWriter<ELEMENT, TRAITS,ALLOC>::seek(std::size_t offset)
{
	m_bEOF = false;
	m_file.clear();
	m_file.seekp(offset, std::ios_base::beg);
	if (! m_file.good())
		throw std::ios_base::failure("BufferedFileWriter<ELEMENT, TRAITS,ALLOC>::seek: seek failed.");
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void BufferedFileWriter<ELEMENT, TRAITS,ALLOC>::write(uint8_t i)
{
	m_file.write(reinterpret_cast<const ELEMENT*>(&i), sizeof(uint8_t));
	if (! m_file.good()) throw std::ios_base::failure("");
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void BufferedFileWriter<ELEMENT, TRAITS,ALLOC>::write(uint16_t i)
{
	m_file.write(reinterpret_cast<const ELEMENT*>(&i), sizeof(uint16_t));
	if (! m_file.good()) throw std::ios_base::failure("");
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void BufferedFileWriter<ELEMENT, TRAITS,ALLOC>::write(uint32_t i)
{
	m_file.write(reinterpret_cast<const ELEMENT*>(&i), sizeof(uint32_t));
	if (! m_file.good()) throw std::ios_base::failure("");
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void BufferedFileWriter<ELEMENT, TRAITS,ALLOC>::write(uint64_t i)
{
	m_file.write(reinterpret_cast<const ELEMENT*>(&i), sizeof(uint64_t));
	if (! m_file.good()) throw std::ios_base::failure("");
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void BufferedFileWriter<ELEMENT, TRAITS,ALLOC>::write(float i)
{
	m_file.write(reinterpret_cast<const ELEMENT*>(&i), sizeof(float));
	if (! m_file.good()) throw std::ios_base::failure("");
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void BufferedFileWriter<ELEMENT, TRAITS,ALLOC>::write(double i)
{
	m_file.write(reinterpret_cast<const ELEMENT*>(&i), sizeof(double));
	if (! m_file.good()) throw std::ios_base::failure("");
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void BufferedFileWriter<ELEMENT, TRAITS,ALLOC>::write(bool b)
{
	m_file.write(reinterpret_cast<const ELEMENT*>(&b), sizeof(bool));
	if (! m_file.good()) throw std::ios_base::failure("");
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void BufferedFileWriter<ELEMENT, TRAITS,ALLOC>::write(const std::basic_string<ELEMENT,TRAITS,ALLOC>& s)
{
	uint32_t len = static_cast<uint32_t>(s.size());
	m_file.write(reinterpret_cast<const ELEMENT*>(&len), sizeof(uint32_t));
	if (! m_file.good()) throw std::ios_base::failure("");
	m_file.write(reinterpret_cast<const ELEMENT*>(s.c_str()), len * sizeof(std::basic_string<ELEMENT,TRAITS,ALLOC>::value_type));
	if (! m_file.good()) throw std::ios_base::failure("");
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void BufferedFileWriter<ELEMENT, TRAITS,ALLOC>::write(uint32_t u32Len,  ELEMENT* pData)
{
	m_file.write(reinterpret_cast<const ELEMENT*>(pData), u32Len);
	if (! m_file.good()) throw std::ios_base::failure("");
}

//
// TemporaryFile
//
template<typename ELEMENT,typename TRAITS, typename ALLOC >
TemporaryFile<ELEMENT, TRAITS,ALLOC>::TemporaryFile()
{

#ifdef _MSC_VER

#ifndef L_tmpnam_s
	// MSVC 2003 doesn't have tmpnam_s, so we'll have to use the old functions

	ELEMENT* tmpName = NULL;
	tmpName = tmpnam( NULL );

	if (tmpName == NULL)
		throw std::ios_base::failure("TemporaryFile: Cannot create temporary file name.");

#else 
	ELEMENT tmpName[L_tmpnam_s];
	errno_t err = tmpnam_s(tmpName, L_tmpnam_s);
	if (err)
		throw std::ios_base::failure("TemporaryFile: Cannot create temporary file name.");

#endif
	if (tmpName[0] == '\\')
		m_sFile = std::basic_string<ELEMENT,TRAITS,ALLOC>(tmpName + 1);
	else
		m_sFile = std::basic_string<ELEMENT,TRAITS,ALLOC>(tmpName);

#else
	ELEMENT tmpName[7] = "XXXXXX";
	if (mktemp(tmpName) == 0)
		throw std::ios_base::failure("TemporaryFile: Cannot create temporary file name.");
	m_sFile = tmpName;
#endif

	m_pFile = new BufferedFileWriter<ELEMENT,TRAITS,ALLOC>(m_sFile, BufferedFile<ELEMENT,TRAITS,ALLOC>::CREATE);
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
TemporaryFile<ELEMENT, TRAITS,ALLOC>::~TemporaryFile()
{
	delete m_pFile;

#ifdef _MSC_VER
	_unlink(m_sFile.c_str());
#else
	std::remove(m_sFile.c_str());
#endif
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void TemporaryFile<ELEMENT, TRAITS,ALLOC>::rewindForReading()
{
	BufferedFileReader<ELEMENT, TRAITS> * br = dynamic_cast<BufferedFileReader<ELEMENT, TRAITS>*>(m_pFile);
	if (br != 0)
		m_pFile->rewind();
	else
	{
		delete m_pFile;
		m_pFile = new BufferedFileReader<ELEMENT, TRAITS>(m_sFile);
	}
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void TemporaryFile<ELEMENT, TRAITS,ALLOC>::rewindForWriting()
{
	BufferedFileWriter<ELEMENT, TRAITS,ALLOC>* bw = dynamic_cast<BufferedFileWriter<ELEMENT, TRAITS,ALLOC>*>(m_pFile);
	if (bw != 0)
		m_pFile->rewind();
	else
	{
		delete m_pFile;
		m_pFile = new BufferedFileWriter<ELEMENT, TRAITS,ALLOC>(m_sFile);
	}
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
bool TemporaryFile<ELEMENT, TRAITS,ALLOC>::eof()
{
	return m_pFile->eof();
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
std::basic_string<ELEMENT,TRAITS,ALLOC> TemporaryFile<ELEMENT, TRAITS,ALLOC>::getFileName() const
{
	return m_sFile;
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
uint8_t TemporaryFile<ELEMENT, TRAITS,ALLOC>::readUInt8()
{
	BufferedFileReader<ELEMENT, TRAITS>* br = dynamic_cast<BufferedFileReader<ELEMENT, TRAITS>*>(m_pFile);
	if (br == 0)
		throw std::ios_base::failure("TemporaryFile<ELEMENT, TRAITS,ALLOC>::readUInt8: file not open for reading.");

	return br->readUInt8();
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
uint16_t TemporaryFile<ELEMENT, TRAITS,ALLOC>::readUInt16()
{
	BufferedFileReader<ELEMENT, TRAITS>* br = dynamic_cast<BufferedFileReader<ELEMENT, TRAITS>*>(m_pFile);
	if (br == 0)
		throw std::ios_base::failure("TemporaryFile<ELEMENT, TRAITS,ALLOC>::readUInt16: file not open for reading.");

	return br->readUInt16();
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
uint32_t TemporaryFile<ELEMENT, TRAITS,ALLOC>::readUInt32()
{
	BufferedFileReader<ELEMENT, TRAITS>* br = dynamic_cast<BufferedFileReader<ELEMENT, TRAITS>*>(m_pFile);
	if (br == 0)
		throw std::ios_base::failure("TemporaryFile<ELEMENT, TRAITS,ALLOC>::readUInt32: file not open for reading.");

	return br->readUInt32();
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
uint64_t TemporaryFile<ELEMENT, TRAITS,ALLOC>::readUInt64()
{
	BufferedFileReader<ELEMENT, TRAITS>* br = dynamic_cast<BufferedFileReader<ELEMENT, TRAITS>*>(m_pFile);
	if (br == 0)
		throw std::ios_base::failure("TemporaryFile<ELEMENT, TRAITS,ALLOC>::readUInt64: file not open for reading.");

	return br->readUInt64();
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
float TemporaryFile<ELEMENT, TRAITS,ALLOC>::readFloat()
{
	BufferedFileReader<ELEMENT, TRAITS>* br = dynamic_cast<BufferedFileReader<ELEMENT, TRAITS>*>(m_pFile);
	if (br == 0)
		throw std::ios_base::failure("TemporaryFile<ELEMENT, TRAITS,ALLOC>::readFloat: file not open for reading.");

	return br->readFloat();
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
double TemporaryFile<ELEMENT, TRAITS,ALLOC>::readDouble()
{
	BufferedFileReader<ELEMENT, TRAITS>* br = dynamic_cast<BufferedFileReader<ELEMENT, TRAITS>*>(m_pFile);
	if (br == 0)
		throw std::ios_base::failure("TemporaryFile<ELEMENT, TRAITS,ALLOC>::readDouble: file not open for reading.");

	return br->readDouble();
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
std::basic_string<ELEMENT,TRAITS,ALLOC> TemporaryFile<ELEMENT, TRAITS,ALLOC>::readString()
{
	BufferedFileReader<ELEMENT, TRAITS>* br = dynamic_cast<BufferedFileReader<ELEMENT, TRAITS>*>(m_pFile);
	if (br == 0)
		throw std::ios_base::failure("TemporaryFile<ELEMENT, TRAITS,ALLOC>::readString: file not open for reading.");

	return br->readString();
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void TemporaryFile<ELEMENT, TRAITS,ALLOC>::readBytes(uint32_t u32Len, ELEMENT** pData)
{
	BufferedFileReader<ELEMENT, TRAITS>* br = dynamic_cast<BufferedFileReader<ELEMENT, TRAITS>*>(m_pFile);
	if (br == 0)
		throw std::ios_base::failure("TemporaryFile<ELEMENT, TRAITS,ALLOC>::readString: file not open for reading.");

	return br->readBytes(u32Len, pData);
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void TemporaryFile<ELEMENT, TRAITS,ALLOC>::write(uint8_t i)
{
	BufferedFileWriter<ELEMENT, TRAITS,ALLOC>* bw = dynamic_cast<BufferedFileWriter<ELEMENT, TRAITS,ALLOC>*>(m_pFile);
	if (bw == 0)
		throw std::ios_base::failure("TemporaryFile<ELEMENT, TRAITS,ALLOC>::write: file not open for writing.");

	return bw->write(i);
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void TemporaryFile<ELEMENT, TRAITS,ALLOC>::write(uint16_t i)
{
	BufferedFileWriter<ELEMENT, TRAITS,ALLOC>* bw = dynamic_cast<BufferedFileWriter<ELEMENT, TRAITS,ALLOC>*>(m_pFile);
	if (bw == 0)
		throw std::ios_base::failure("TemporaryFile<ELEMENT, TRAITS,ALLOC>::write: file not open for writing.");

	return bw->write(i);
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void TemporaryFile<ELEMENT, TRAITS,ALLOC>::write(uint32_t i)
{
	BufferedFileWriter<ELEMENT, TRAITS,ALLOC>* bw = dynamic_cast<BufferedFileWriter<ELEMENT, TRAITS,ALLOC>*>(m_pFile);
	if (bw == 0)
		throw std::ios_base::failure("TemporaryFile<ELEMENT, TRAITS,ALLOC>::write: file not open for writing.");

	return bw->write(i);
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void TemporaryFile<ELEMENT, TRAITS,ALLOC>::write(uint64_t i)
{
	BufferedFileWriter<ELEMENT, TRAITS,ALLOC>* bw = dynamic_cast<BufferedFileWriter<ELEMENT, TRAITS,ALLOC>*>(m_pFile);
	if (bw == 0)
		throw std::ios_base::failure("TemporaryFile<ELEMENT, TRAITS,ALLOC>::write: file not open for writing.");

	return bw->write(i);
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void TemporaryFile<ELEMENT, TRAITS,ALLOC>::write(float i)
{
	BufferedFileWriter<ELEMENT, TRAITS,ALLOC>* bw = dynamic_cast<BufferedFileWriter<ELEMENT, TRAITS,ALLOC>*>(m_pFile);
	if (bw == 0)
		throw std::ios_base::failure("TemporaryFile<ELEMENT, TRAITS,ALLOC>::write: file not open for writing.");

	return bw->write(i);
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void TemporaryFile<ELEMENT, TRAITS,ALLOC>::write(double i)
{
	BufferedFileWriter<ELEMENT, TRAITS,ALLOC>* bw = dynamic_cast<BufferedFileWriter<ELEMENT, TRAITS,ALLOC>*>(m_pFile);
	if (bw == 0)
		throw std::ios_base::failure("TemporaryFile<ELEMENT, TRAITS,ALLOC>::write: file not open for writing.");

	return bw->write(i);
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void TemporaryFile<ELEMENT, TRAITS,ALLOC>::write(const std::basic_string<ELEMENT,TRAITS,ALLOC>& s)
{
	BufferedFileWriter<ELEMENT, TRAITS,ALLOC>* bw = dynamic_cast<BufferedFileWriter<ELEMENT, TRAITS,ALLOC>*>(m_pFile);
	if (bw == 0)
		throw std::ios_base::failure("TemporaryFile<ELEMENT, TRAITS,ALLOC>::write: file not open for writing.");

	return bw->write(s);
}
template<typename ELEMENT,typename TRAITS, typename ALLOC >
void TemporaryFile<ELEMENT, TRAITS,ALLOC>::write(uint32_t u32Len, ELEMENT* pData)
{
	BufferedFileWriter<ELEMENT, TRAITS,ALLOC>* bw = dynamic_cast<BufferedFileWriter<ELEMENT, TRAITS,ALLOC>*>(m_pFile);
	if (bw == 0)
		throw std::ios_base::failure("TemporaryFile<ELEMENT, TRAITS,ALLOC>::write: file not open for writing.");

	return bw->write(u32Len, pData);
}

end_gtl_namespace
