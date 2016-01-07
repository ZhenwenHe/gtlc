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

#include <iostream>
#include "config.h"


begin_gtl_namespace
begin_gdb_namespace
//!A streambuf class that controls the transmission of elements to and from
//!a basic_xbufferstream. The elements are transmitted from a to a fixed
//!size buffer
template <class ELEMENT, class TRAITS = std::char_traits<ELEMENT>>
class basic_bufferbuf
	: public std::basic_streambuf < ELEMENT, TRAITS >
{
public:
	typedef ELEMENT                                         char_type;
	typedef typename TRAITS::int_type                 int_type;
	typedef typename TRAITS::pos_type                 pos_type;
	typedef typename TRAITS::off_type                 off_type;
	typedef TRAITS                                    traits_type;
	typedef std::basic_streambuf<char_type, traits_type>  base_t;

public:
	//!Constructor.
	//!Does not throw.
	explicit basic_bufferbuf(std::ios_base::openmode mode
		= std::ios_base::in | std::ios_base::out)
		: base_t(), m_mode(mode), m_buffer(0), m_length(0)
	{}

	//!Constructor. Assigns formatting buffer.
	//!Does not throw.
	explicit basic_bufferbuf(ELEMENT *buffer, std::size_t length,
		std::ios_base::openmode mode
		= std::ios_base::in | std::ios_base::out)
		: base_t(), m_mode(mode), m_buffer(buffer), m_length(length)
	{
		this->set_pointers();
	}

	virtual ~basic_bufferbuf(){}

public:
	//!Returns the pointer and size of the internal buffer. 
	//!Does not throw.
	std::pair<ELEMENT *, std::size_t> buffer() const
	{
		return std::pair<ELEMENT *, std::size_t>(m_buffer, m_length);
	}

	//!Sets the underlying buffer to a new value
	//!Does not throw.
	void buffer(ELEMENT *buffer, std::size_t length)
	{
		m_buffer = buffer;   m_length = length;   this->set_pointers();
	}

	/// @cond
private:
	void set_pointers()
	{
		// The initial read position is the beginning of the buffer.
		if (m_mode & std::ios_base::in)
			this->setg(m_buffer, m_buffer, m_buffer + m_length);

		// The initial write position is the beginning of the buffer.
		if (m_mode & std::ios_base::out)
			this->setp(m_buffer, m_buffer + m_length);
	}

protected:
	virtual int_type underflow()
	{
		// Precondition: gptr() >= egptr(). Returns a character, if available.
		return this->gptr() != this->egptr() ?
			TRAITS::to_int_type(*this->gptr()) : TRAITS::eof();
	}

	virtual int_type pbackfail(int_type c = TRAITS::eof())
	{
		if (this->gptr() != this->eback()) {
			if (!TRAITS::eq_int_type(c, TRAITS::eof())) {
				if (TRAITS::eq(TRAITS::to_char_type(c), this->gptr()[-1])) {
					this->gbump(-1);
					return c;
				}
				else if (m_mode & std::ios_base::out) {
					this->gbump(-1);
					*this->gptr() = c;
					return c;
				}
				else
					return TRAITS::eof();
			}
			else {
				this->gbump(-1);
				return TRAITS::not_eof(c);
			}
		}
		else
			return TRAITS::eof();
	}

	virtual int_type overflow(int_type c = TRAITS::eof())
	{
		if (m_mode & std::ios_base::out) {
			if (!TRAITS::eq_int_type(c, TRAITS::eof())) {
				//            if(!(m_mode & std::ios_base::in)) {
				//               if(this->pptr() != this->epptr()) {
				//                  *this->pptr() = TRAITS::to_char_type(c);
				//                  this->pbump(1);
				//                  return c;
				//               }
				//               else
				//                  return TRAITS::eof();
				//            }
				//            else {
				if (this->pptr() == this->epptr()) {
					//We can't append to a static buffer
					return TRAITS::eof();
				}
				else {
					*this->pptr() = TRAITS::to_char_type(c);
					this->pbump(1);
					return c;
				}
				//            }
			}
			else  // c is EOF, so we don't have to do anything
				return TRAITS::not_eof(c);
		}
		else     // Overflow always fails if it's read-only.
			return TRAITS::eof();
	}

	virtual pos_type seekoff(off_type off, std::ios_base::seekdir dir,
		std::ios_base::openmode mode
		= std::ios_base::in | std::ios_base::out)
	{
		bool in = false;
		bool out = false;

		const std::ios_base::openmode inout =
			std::ios_base::in | std::ios_base::out;

		if ((mode & inout) == inout) {
			if (dir == std::ios_base::beg || dir == std::ios_base::end)
				in = out = true;
		}
		else if (mode & std::ios_base::in)
			in = true;
		else if (mode & std::ios_base::out)
			out = true;

		if (!in && !out)
			return pos_type(off_type(-1));
		else if ((in && (!(m_mode & std::ios_base::in) || this->gptr() == 0)) ||
			(out && (!(m_mode & std::ios_base::out) || this->pptr() == 0)))
			return pos_type(off_type(-1));

		std::streamoff newoff;
		switch (dir) {
		case std::ios_base::beg:
			newoff = 0;
			break;
		case std::ios_base::end:
			newoff = static_cast<std::streamoff>(m_length);
			break;
		case std::ios_base::cur:
			newoff = in ? static_cast<std::streamoff>(this->gptr() - this->eback())
				: static_cast<std::streamoff>(this->pptr() - this->pbase());
			break;
		default:
			return pos_type(off_type(-1));
		}

		off += newoff;

		if (in) {
			std::ptrdiff_t n = this->egptr() - this->eback();

			if (off < 0 || off > n)
				return pos_type(off_type(-1));
			else
				this->setg(this->eback(), this->eback() + off, this->eback() + n);
		}

		if (out) {
			std::ptrdiff_t n = this->epptr() - this->pbase();

			if (off < 0 || off > n)
				return pos_type(off_type(-1));
			else {
				this->setp(this->pbase(), this->pbase() + n);
				this->pbump(off);
			}
		}

		return pos_type(off);
	}

	virtual pos_type seekpos(pos_type pos, std::ios_base::openmode mode
		= std::ios_base::in | std::ios_base::out)
	{
		return seekoff(pos - pos_type(off_type(0)), std::ios_base::beg, mode);
	}

private:
	std::ios_base::openmode m_mode;
	ELEMENT *                 m_buffer;
	std::size_t             m_length;
	/// @endcond
};

//!A basic_istream class that uses a fixed size character buffer
//!as its formatting buffer.
template <class ELEMENT, class TRAITS = std::char_traits<ELEMENT> >
class basic_ibufferstream
	: public std::basic_istream < ELEMENT, TRAITS >
{
public:                         // Typedefs
	typedef typename std::basic_ios
		<ELEMENT, TRAITS>::char_type          char_type;
	typedef typename std::basic_ios<char_type, TRAITS>::int_type     int_type;
	typedef typename std::basic_ios<char_type, TRAITS>::pos_type     pos_type;
	typedef typename std::basic_ios<char_type, TRAITS>::off_type     off_type;
	typedef typename std::basic_ios<char_type, TRAITS>::traits_type  traits_type;

private:
	typedef std::basic_ios<char_type, TRAITS>                basic_ios_t;
	typedef std::basic_istream<char_type, TRAITS>            base_t;

public:
	//!Constructor.
	//!Does not throw.
	basic_ibufferstream(std::ios_base::openmode mode = std::ios_base::in)
		: basic_ios_t(), base_t(0), m_buf(mode | std::ios_base::in)
	{
		basic_ios_t::init(&m_buf);
	}

	//!Constructor. Assigns formatting buffer.
	//!Does not throw.
	basic_ibufferstream(const ELEMENT *buffer, std::size_t length,
		std::ios_base::openmode mode = std::ios_base::in)
		: basic_ios_t(), base_t(0),
		m_buf(const_cast<ELEMENT*>(buffer), length, mode | std::ios_base::in)
	{
		basic_ios_t::init(&m_buf);
	}

	~basic_ibufferstream(){};

public:
	//!Returns the address of the stored
	//!stream buffer.
	basic_bufferbuf<ELEMENT, TRAITS>* rdbuf() const
	{
		return const_cast<basic_bufferbuf<ELEMENT, TRAITS>*>(&m_buf);
	}

	//!Returns the pointer and size of the internal buffer. 
	//!Does not throw.
	std::pair<const ELEMENT *, std::size_t> buffer() const
	{
		return m_buf.buffer();
	}

	//!Sets the underlying buffer to a new value. Resets 
	//!stream position. Does not throw.
	void buffer(const ELEMENT *buffer, std::size_t length)
	{
		m_buf.buffer(const_cast<ELEMENT*>(buffer), length);
	}

	/// @cond
private:
	basic_bufferbuf<ELEMENT, TRAITS> m_buf;
	/// @endcond
};

//!A basic_ostream class that uses a fixed size character buffer
//!as its formatting buffer.
template <class ELEMENT, class TRAITS = std::char_traits<ELEMENT> >
class basic_obufferstream
	: public std::basic_ostream < ELEMENT, TRAITS >
{
public:
	typedef typename std::basic_ios
		<ELEMENT, TRAITS>::char_type          char_type;
	typedef typename std::basic_ios<char_type, TRAITS>::int_type     int_type;
	typedef typename std::basic_ios<char_type, TRAITS>::pos_type     pos_type;
	typedef typename std::basic_ios<char_type, TRAITS>::off_type     off_type;
	typedef typename std::basic_ios<char_type, TRAITS>::traits_type  traits_type;

	/// @cond
private:
	typedef std::basic_ios<char_type, TRAITS>      basic_ios_t;
	typedef std::basic_ostream<char_type, TRAITS>  base_t;
	/// @endcond
public:
	//!Constructor.
	//!Does not throw.
	basic_obufferstream(std::ios_base::openmode mode = std::ios_base::out)
		: basic_ios_t(), base_t(0), m_buf(mode | std::ios_base::out)
	{
		basic_ios_t::init(&m_buf);
	}

	//!Constructor. Assigns formatting buffer.
	//!Does not throw.
	basic_obufferstream(ELEMENT *buffer, std::size_t length,
		std::ios_base::openmode mode = std::ios_base::out)
		: basic_ios_t(), base_t(0),
		m_buf(buffer, length, mode | std::ios_base::out)
	{
		basic_ios_t::init(&m_buf);
	}

	~basic_obufferstream(){}

public:
	//!Returns the address of the stored
	//!stream buffer.
	basic_bufferbuf<ELEMENT, TRAITS>* rdbuf() const
	{
		return const_cast<basic_bufferbuf<ELEMENT, TRAITS>*>(&m_buf);
	}

	//!Returns the pointer and size of the internal buffer. 
	//!Does not throw.
	std::pair<ELEMENT *, std::size_t> buffer() const
	{
		return m_buf.buffer();
	}

	//!Sets the underlying buffer to a new value. Resets 
	//!stream position. Does not throw.
	void buffer(ELEMENT *buffer, std::size_t length)
	{
		m_buf.buffer(buffer, length);
	}

	/// @cond
private:
	basic_bufferbuf<ELEMENT, TRAITS> m_buf;
	/// @endcond
};


//!A basic_iostream class that uses a fixed size character buffer
//!as its formatting buffer.
template <class ELEMENT, class TRAITS = std::char_traits<ELEMENT> >
class basic_bufferstream
	: public std::basic_iostream < ELEMENT, TRAITS >

{
public:                         // Typedefs
	typedef typename std::basic_ios
		<ELEMENT, TRAITS>::char_type          char_type;
	typedef typename std::basic_ios<char_type, TRAITS>::int_type     int_type;
	typedef typename std::basic_ios<char_type, TRAITS>::pos_type     pos_type;
	typedef typename std::basic_ios<char_type, TRAITS>::off_type     off_type;
	typedef typename std::basic_ios<char_type, TRAITS>::traits_type  traits_type;

	/// @cond
private:
	typedef std::basic_ios<char_type, TRAITS>                 basic_ios_t;
	typedef std::basic_iostream<char_type, TRAITS>            base_t;
	/// @endcond

public:
	//!Constructor.
	//!Does not throw.
	basic_bufferstream(std::ios_base::openmode mode
		= std::ios_base::in | std::ios_base::out)
		: basic_ios_t(), base_t(0), m_buf(mode)
	{
		basic_ios_t::init(&m_buf);
	}

	//!Constructor. Assigns formatting buffer.
	//!Does not throw.
	basic_bufferstream(ELEMENT *buffer, std::size_t length,
		std::ios_base::openmode mode
		= std::ios_base::in | std::ios_base::out)
		: basic_ios_t(), base_t(0), m_buf(buffer, length, mode)
	{
		basic_ios_t::init(&m_buf);
	}

	~basic_bufferstream(){}

public:
	//!Returns the address of the stored
	//!stream buffer.
	basic_bufferbuf<ELEMENT, TRAITS>* rdbuf() const
	{
		return const_cast<basic_bufferbuf<ELEMENT, TRAITS>*>(&m_buf);
	}

	//!Returns the pointer and size of the internal buffer. 
	//!Does not throw.
	std::pair<ELEMENT *, std::size_t> buffer() const
	{
		return m_buf.buffer();
	}

	//!Sets the underlying buffer to a new value. Resets 
	//!stream position. Does not throw.
	void buffer(ELEMENT *buffer, std::size_t length)
	{
		m_buf.buffer(buffer, length);
	}

	/// @cond
private:
	basic_bufferbuf<ELEMENT, TRAITS> m_buf;
	/// @endcond
};

//Some typedefs to simplify usage
typedef basic_bufferbuf<char, std::char_traits<char> >        bufferbuf;
typedef basic_bufferstream<char, std::char_traits<char> >     bufferstream;
typedef basic_ibufferstream<char, std::char_traits<char> >    ibufferstream;
typedef basic_obufferstream<char, std::char_traits<char> >    obufferstream;

typedef basic_bufferbuf<wchar_t, std::char_traits<wchar_t> >     wbufferbuf;
typedef basic_bufferstream<wchar_t, std::char_traits<wchar_t>  >  wbufferstream;
typedef basic_ibufferstream<wchar_t, std::char_traits<wchar_t> > wibufferstream;
typedef basic_obufferstream<wchar_t, std::char_traits<wchar_t> > wobufferstream;


end_gdb_namespace
end_gtl_namespace



