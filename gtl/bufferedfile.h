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
 #include "object.h"
begin_gtl_namespace
template<
	typename  ELEMENT,
	typename TRAITS=std::char_traits<ELEMENT>,
	typename ALLOC=std::allocator<ELEMENT> 
>
class BufferedFile : public Object<ELEMENT,TRAITS,ALLOC>
{
public: 
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "BufferedFile";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
public:
	enum FileMode
	{
		APPEND = 0x0,
		CREATE
	};
public:
	BufferedFile(uint32_t u32BufferSize = 16384);
	virtual ~BufferedFile();

	virtual void close();
	virtual bool eof();
	virtual void rewind()=0;
	virtual void seek(std::size_t offset) =0;

protected:
	std::basic_fstream<ELEMENT, TRAITS>  m_file;
	ELEMENT* m_buffer;
	uint32_t m_u32BufferSize;
	bool m_bEOF;
};

template<
	typename  ELEMENT,
	typename TRAITS=std::char_traits<ELEMENT>,
	typename ALLOC=std::allocator<ELEMENT> 
>
class BufferedFileReader : virtual public BufferedFile< ELEMENT,TRAITS,ALLOC>
{
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "BufferedFileReader";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){

	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
public:
	BufferedFileReader();
	BufferedFileReader(const std::basic_string<ELEMENT,TRAITS,ALLOC> & sFileName, uint32_t u32BufferSize = 32768);
	virtual ~BufferedFileReader();

	virtual void open(const std::basic_string<ELEMENT,TRAITS,ALLOC>& sFileName);
	virtual void rewind();
	virtual void seek(std::size_t offset);

	virtual uint8_t readUInt8();
	virtual uint16_t readUInt16();
	virtual uint32_t readUInt32();
	virtual uint64_t readUInt64();
	virtual float readFloat();
	virtual double readDouble();
	virtual bool readBoolean();
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> readString();
	virtual void readBytes(uint32_t u32Len, ELEMENT ** pData);
};

template<
	typename  ELEMENT,
	typename TRAITS=std::char_traits<ELEMENT>,
	typename ALLOC=std::allocator<ELEMENT> 
>
class BufferedFileWriter : virtual public BufferedFile< ELEMENT, TRAITS  >
{
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "BufferedFileWriter";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){

	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
public:
	BufferedFileWriter();
	BufferedFileWriter(const std::basic_string<ELEMENT,TRAITS,ALLOC>& sFileName, 
		BufferedFile<ELEMENT,TRAITS>::FileMode mode = BufferedFile<ELEMENT,TRAITS>::CREATE, uint32_t u32BufferSize = 32768);
	virtual ~BufferedFileWriter();

	virtual void open(const std::basic_string<ELEMENT,TRAITS,ALLOC>& sFileName, 
		BufferedFile<ELEMENT,TRAITS>::FileMode mode = BufferedFile<ELEMENT,TRAITS>::CREATE);
	virtual void rewind();
	virtual void seek(std::size_t offset);

	virtual void write(uint8_t i);
	virtual void write(uint16_t i);
	virtual void write(uint32_t i);
	virtual void write(uint64_t i);
	virtual void write(float i);
	virtual void write(double i);
	virtual void write(bool b);
	virtual void write(const std::basic_string<ELEMENT,TRAITS,ALLOC>& s);
	virtual void write(uint32_t u32Len, ELEMENT * pData);
};

template<
	typename  ELEMENT,
	typename TRAITS=std::char_traits<ELEMENT>,
	typename ALLOC=std::allocator<ELEMENT> 
>
class TemporaryFile : public Object<ELEMENT,TRAITS,ALLOC>
{
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "TemporaryFile";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){

	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
public:
	TemporaryFile();
	virtual ~TemporaryFile();

	void rewindForReading();
	void rewindForWriting();
	bool eof();
	std::basic_string<ELEMENT,TRAITS,ALLOC> getFileName() const;

	uint8_t readUInt8();
	uint16_t readUInt16();
	uint32_t readUInt32();
	uint64_t readUInt64();
	float readFloat();
	double readDouble();
	std::basic_string<ELEMENT,TRAITS,ALLOC> readString();
	void readBytes(uint32_t u32Len, ELEMENT ** pData);

	void write(uint8_t i);
	void write(uint16_t i);
	void write(uint32_t i);
	void write(uint64_t i);
	void write(float i);
	void write(double i);
	void write(const std::basic_string<ELEMENT,TRAITS,ALLOC>& s);
	void write(uint32_t u32Len, ELEMENT * pData);

private:
	std::basic_string<ELEMENT,TRAITS,ALLOC> m_sFile;
	BufferedFile<ELEMENT,TRAITS>* m_pFile;
};


end_gtl_namespace

#include "bufferedfile.inl"