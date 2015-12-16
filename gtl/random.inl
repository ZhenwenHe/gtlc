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
 
begin_gtl_namespace

#ifndef RAND48_SEED_0
#define	RAND48_SEED_0	(0x330e)
#endif
#ifndef RAND48_SEED_1
#define	RAND48_SEED_1	(0xabcd)
#endif
#ifndef RAND48_SEED_2
#define	RAND48_SEED_2	(0x1234)
#endif
#ifndef RAND48_MULT_0
#define	RAND48_MULT_0	(0xe66d)
#endif
#ifndef RAND48_MULT_1
#define	RAND48_MULT_1	(0xdeec)
#endif
#ifndef RAND48_MULT_2
#define	RAND48_MULT_2	(0x0005)
#endif

#ifndef RAND48_ADD
#define	RAND48_ADD	(0x000b)
#endif


 
/* Internal function to compute next state of the generator.  */
template<typename ELEMENT,typename TRAITS,	typename ALLOC>
void		Random<ELEMENT,TRAITS, ALLOC>::_dorand48(unsigned short xseed[3])
{
	unsigned long accu;
	unsigned short temp[2];

	accu = (unsigned long) __rand48_Mult[0] * (unsigned long) xseed[0] +
		(unsigned long) __rand48_Add;
	temp[0] = (unsigned short) accu;	/* lower 16 bits */
	accu >>= sizeof(unsigned short) * 8;
	accu += (unsigned long) __rand48_Mult[0] * (unsigned long) xseed[1] +
		(unsigned long) __rand48_Mult[1] * (unsigned long) xseed[0];
	temp[1] = (unsigned short) accu;	/* middle 16 bits */
	accu >>= sizeof(unsigned short) * 8;
	accu += __rand48_Mult[0] * xseed[2] + __rand48_Mult[1] * xseed[1] +
		__rand48_Mult[2] * xseed[0];
	xseed[0] = temp[0];
	xseed[1] = temp[1];
	xseed[2] = (unsigned short) accu;
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
void	Random<ELEMENT,TRAITS, ALLOC>::srand48(long seed) 
{
	__rand48_Seed[0] = RAND48_SEED_0;
	__rand48_Seed[1] = (unsigned short) seed;
	__rand48_Seed[2] = (unsigned short) (seed >> 16);
	__rand48_Mult[0] = RAND48_MULT_0;
	__rand48_Mult[1] = RAND48_MULT_1;
	__rand48_Mult[2] = RAND48_MULT_2;
	__rand48_Add = RAND48_ADD;
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
unsigned short *	Random<ELEMENT,TRAITS, ALLOC>::seed48(unsigned short xseed[3]) 
{
	static unsigned short sseed[3];

	sseed[0] = __rand48_Seed[0];
	sseed[1] = __rand48_Seed[1];
	sseed[2] = __rand48_Seed[2];
	__rand48_Seed[0] = xseed[0];
	__rand48_Seed[1] = xseed[1];
	__rand48_Seed[2] = xseed[2];
	__rand48_Mult[0] = RAND48_MULT_0;
	__rand48_Mult[1] = RAND48_MULT_1;
	__rand48_Mult[2] = RAND48_MULT_2;
	__rand48_Add = RAND48_ADD;
	return sseed;
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
long	Random<ELEMENT,TRAITS, ALLOC>::nrand48(unsigned short xseed[3]) 
{
	_dorand48(xseed);
	return ((long) xseed[2] << 15) + ((long) xseed[1] >> 1);
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
long	Random<ELEMENT,TRAITS, ALLOC>::mrand48(void) 
{
	_dorand48(__rand48_Seed);
	return ((long) __rand48_Seed[2] << 16) + (long) __rand48_Seed[1];
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
long	Random<ELEMENT,TRAITS, ALLOC>::lrand48(void) 
{
	_dorand48(__rand48_Seed);
	return ((long) __rand48_Seed[2] << 15) + ((long) __rand48_Seed[1] >> 1);
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
void	Random<ELEMENT,TRAITS, ALLOC>::lcong48(unsigned short p[7]) 
{
	__rand48_Seed[0] = p[0];
	__rand48_Seed[1] = p[1];
	__rand48_Seed[2] = p[2];
	__rand48_Mult[0] = p[3];
	__rand48_Mult[1] = p[4];
	__rand48_Mult[2] = p[5];
	__rand48_Add = p[6];
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
long	Random<ELEMENT,TRAITS, ALLOC>::jrand48(unsigned short xseed[3]) 
{
	_dorand48(xseed);
	return ((long) xseed[2] << 16) + (long) xseed[1];
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
double	Random<ELEMENT,TRAITS, ALLOC>::erand48(unsigned short xseed[3]) 
{
	_dorand48(xseed);
	return ldexp((double) xseed[0], -48) +
		ldexp((double) xseed[1], -32) +
		ldexp((double) xseed[2], -16);
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
double	Random<ELEMENT,TRAITS, ALLOC>::drand48(void) 
{
	return erand48(__rand48_Seed);
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
Random<ELEMENT,TRAITS, ALLOC>::Random()
{
	__rand48_Seed[0]=RAND48_SEED_0;
	__rand48_Seed[1]=RAND48_SEED_1;
	__rand48_Seed[2]=RAND48_SEED_2; 

	
	__rand48_Mult[0] =RAND48_MULT_0;
	__rand48_Mult[1] =RAND48_MULT_1;
	__rand48_Mult[2] =RAND48_MULT_2; 

	__rand48_Add = RAND48_ADD;

	m_pBuffer = 0;
	initDrand(static_cast<unsigned long>(time(0)), 0xD31A);
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
Random<ELEMENT,TRAITS, ALLOC>::Random(unsigned long seed, unsigned short xsubi0)
{
	__rand48_Seed[0]=RAND48_SEED_0;
	__rand48_Seed[1]=RAND48_SEED_1;
	__rand48_Seed[2]=RAND48_SEED_2; 

	
	__rand48_Mult[0] =RAND48_MULT_0;
	__rand48_Mult[1] =RAND48_MULT_1;
	__rand48_Mult[2] =RAND48_MULT_2; 

	__rand48_Add = RAND48_ADD;

	m_pBuffer = 0;
	initDrand(seed, xsubi0);
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
Random<ELEMENT,TRAITS, ALLOC>::~Random()
{
	delete[] m_pBuffer;
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
void Random<ELEMENT,TRAITS, ALLOC>::initDrand(unsigned long seed, unsigned short xsubi0)
{
	

	m_pBuffer = new unsigned short[3];
	m_pBuffer[0] = static_cast<unsigned short>(xsubi0);
	unsigned long mask = 0xFFFF;
	m_pBuffer[1] = static_cast<unsigned short>(seed & mask);
	mask = mask << 16;
	m_pBuffer[2] = static_cast<unsigned short>((seed & mask) >> 16);
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
long Random<ELEMENT,TRAITS, ALLOC>::nextLong()
{
	return jrand48(m_pBuffer);
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
unsigned long Random<ELEMENT,TRAITS, ALLOC>::nextUnsignedLong()
{
	return static_cast<unsigned long>(nextLong());
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
long Random<ELEMENT,TRAITS, ALLOC>::nextLong(long low, long high)
{
	return low + static_cast<long>((high - low) * nextDouble());
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
unsigned long Random<ELEMENT,TRAITS, ALLOC>::nextUnsignedLong(unsigned long low, unsigned long high)
{
	return low + static_cast<unsigned long>((high - low) * nextDouble());
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
long long Random<ELEMENT,TRAITS, ALLOC>::nextLongLong()
{
	return static_cast<long long>(nextUnsignedLongLong());
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
unsigned long long Random<ELEMENT,TRAITS, ALLOC>::nextUnsignedLongLong()
{
	unsigned long long lh = static_cast<unsigned long long>(nextLong());
	unsigned long long ll = static_cast<unsigned long long>(nextLong());
	unsigned long long ret = (lh << 32) | ll;
	return ret;
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
long long Random<ELEMENT,TRAITS, ALLOC>::nextLongLong(long long low, long long high)
{
	return low + static_cast<long long>((high - low) * nextDouble());
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
unsigned long long Random<ELEMENT,TRAITS, ALLOC>::nextUnsignedLongLong(unsigned long long low, unsigned long long high)
{
	return low + static_cast<unsigned long long>((high - low) * nextDouble());
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
short Random<ELEMENT,TRAITS, ALLOC>::nextShort()
{
	return static_cast<short>(nextUnsignedShort());
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
unsigned short Random<ELEMENT,TRAITS, ALLOC>::nextUnsignedShort()
{
	return nextUnsignedLong() >> 16;
	// retain the high order bits.
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
double Random<ELEMENT,TRAITS, ALLOC>::nextDouble()
{
	unsigned short* xsubi = reinterpret_cast<unsigned short*>(m_pBuffer);
	return erand48(xsubi);
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
double Random<ELEMENT,TRAITS, ALLOC>::nextDouble(double low, double high)
{
	return (high - low) * nextDouble() + low;
}
template<typename ELEMENT,typename TRAITS, typename ALLOC>
bool Random<ELEMENT,TRAITS, ALLOC>::flipCoin()
{
	if (nextDouble() < 0.5) return true;
	return false;
}



end_gtl_namespace