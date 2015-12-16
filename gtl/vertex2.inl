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

begin_gtl_namespace

template<class T >
class Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >
: public Object<char,std::char_traits<char>,std::allocator<char> >{
protected:
	T  _data[2];
public:
	virtual std::basic_string<char,std::char_traits<char>,std::allocator<char> > getClassName(){
		return "Vertex2";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	} 
public:
	typedef T value_type;
	inline const T * get() const {return _data;}
	inline T * get() {return (T*)_data;}
	inline const T get(int i)const {return _data[i];} 
	inline void set(int i, const T & t) {_data[i]=t;}
	inline void set(const T * data){
		memcpy(_data,data,sizeof(T)*2);
	}
	void copy(const Vertex<T,2,char,std::char_traits<char>,std::allocator<char> > & p){
		for(int i=0;i<2;i++)
			_data[i]=p._data[i];
	}

	Vertex( ){ 
	}

	Vertex(T xx, T yy){		 
			_data[0]=xx;
			_data[1]=yy;
            _data[2]=0;
	}
	Vertex(const T * data ){
		memcpy(_data,data,sizeof(T)*2);
	}
	Vertex(const Vertex<T,2,char,std::char_traits<char>,std::allocator<char> > & p){
		copy(p);
	}
	
	virtual ~Vertex(){
	}
	Vertex<T,2,char,std::char_traits<char>,std::allocator<char> > operator=(const  Vertex<T,2,char,std::char_traits<char>,std::allocator<char> > & p){
		copy(p);
		return *this;
	}
	Vertex<T,2,char,std::char_traits<char>,std::allocator<char> > operator+=(const  Vertex<T,2,char,std::char_traits<char>,std::allocator<char> > & p){
		for(int i=0;i<2;i++)
			_data[i]+=p._data[i];
		return *this;
	}
	Vertex<T,2,char,std::char_traits<char>,std::allocator<char> > operator-=(const  Vertex<T,2,char,std::char_traits<char>,std::allocator<char> > & p){
		for(int i=0;i<2;i++)
			_data[i]-=p._data[i];
		return *this;
	}
	Vertex<T,2,char,std::char_traits<char>,std::allocator<char> > operator*=(double rhd)
	{ 
		for(int i=0;i<2;i++)
			_data[i] *= rhd; 
		return *this;
	}
	 

	inline const T  & getX() const { return _data[0];}
	inline const T  & getY() const { return _data[1];} 

	inline void setX(const T  & t)   { return _data[0]=t;}
	inline void setY(const T  & t)   { return _data[1]=t;} 

	inline void set(const T  & x, const T & y )   { 
		_data[0]=x;
		_data[1]=y; 
	}

	T &  operator[]( size_t  i){
		return _data[i];
	}
	const T &  operator[]( const size_t  i) const {
		return _data[i];
	}
	void zero(){
		for(int i=0;i<2;i++)
			_data[i]=0;
	}
	inline void getEnvelope(T minv[2],T maxv[2])
	{
		for(int i=0;i<2;i++)
		{
			maxv[i]=_data[i]+TOLERANCE;
			minv[i]=_data[i]-TOLERANCE;
		}
	}

public:
	static double calculateDistance(const Vertex<T,2,char,std::char_traits<char>,std::allocator<char> > & p1,const Vertex<T,2,char,std::char_traits<char>,std::allocator<char> > & p2 ){
		return sqrt(calculateDistance2(p1,p2));
	}
	static double calculateDistance2(const Vertex<T,2,char,std::char_traits<char>,std::allocator<char> > & p1,const Vertex<T,2,char,std::char_traits<char>,std::allocator<char> > & p2){		
		double s = 0;
		for(size_t i =0;i<2;i++){
			s+=(p1._data[i]-p2._data[i])*(p1._data[i]-p2._data[i]);
		}
		return s;
	}
public:
	/** 内联函数。重载一元操作符"-"，用于取得向量或坐标的相反值。
	* @param 
	* @return 向量或坐标的相反值
	*/
	inline Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >  operator-() const
	{
        return Vertex<T,2>(-getX(),-getY(),-getZ());
	}

	/** Length of the vector = sqrt( vec . vec ) */
	inline double  length() const  {
		return sqrt(getX()*getX() + getY()*getY() );
	}

	/** Length squared of the vector = vec . vec */
	inline double length2() const {
		return getX()*getX() + getY()*getY()  ;
	}

	/** Normalize the vector so that it has length unity.
	* Returns the previous length of the vector.
	* If the vector is zero length, it is left unchanged and zero is returned.
	*/
	inline double  normalize() {
		double norm =  length();
		if (norm>0.0) {
			double inv = 1.0/norm;
			_data[0] *= inv;
			_data[1] *= inv;
		}                
		return( norm );
	}
	inline double  dotProduct(const Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >& vec) const
	{
		return get * vec.getX() + getY() * vec.getY() ;
	}
	
};
//////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
static bool operator==(const Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >  & t1,const Vertex<T,2,char,std::char_traits<char>,std::allocator<char> > & t2){	
	for(int i=0;i<2;i++){
		if(t1[i]!=t2[i])
			return false;
	}
	return true;
}

template <class T>
static  Vertex<T,2,char,std::char_traits<char>,std::allocator<char> > operator*(const  Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >   & t11,double a ){
	Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >  & t1=(Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >  &) t11;
	Vertex<T,2,char,std::char_traits<char>,std::allocator<char> > t;
	for(int i=0;i<t1.size();i++){
		t[i]=t1[i]*a;
	}
	return t;
}

template <class T>
static  Vertex<T,2,char,std::char_traits<char>,std::allocator<char> > operator*(double a,const  Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >   & t11 ){
	Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >  & t1=(Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >  &) t11;
	Vertex<T,2,char,std::char_traits<char>,std::allocator<char> > t;
	for(int i=0;i<t1.size();i++){
		t[i]=t1[i]*a;
	}
	return t;
}

template <class T> 
static T  operator*(const  Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >   & t1,const  Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >  & t2){

	T t=0;
	for(int i=0;i<2;i++){
		t+=(t1[i]*t2[i]);
	}
	return t;
}

template <class T> 
static Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >  operator+(const  Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >   & t11,const  Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >  & t22){
	Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >  & t1=(Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >  &) t11;
	Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >  & t2=(Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >  &) t22;

	Vertex<T,2,char,std::char_traits<char>,std::allocator<char> > t; 
	for(int i=0;i<2;i++){
		t[i]=t1[i]+t2[i];
	}
	return t;
}

template <class T> 
static Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >  operator / (const  Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >   & t11,const T & d){
	Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >  & t1=(Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >  &) t11;
	Vertex<T,2,char,std::char_traits<char>,std::allocator<char> > t; 
	for(int i=0;i<2;i++){
		t[i]=t1[i]/d;
	}
	return t;
}

template <class T> 
static  Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >  operator-(const  Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >   & t11,const  Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >  & t22){
	Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >  & t1=(Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >  &) t11;
	Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >  & t2=(Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >  &) t22;

	Vertex<T,2,char,std::char_traits<char>,std::allocator<char> > t; 
	for(int i=0;i<2;i++){
		t[i]=t1[i]-t2[i];
	}
	return t;
}


template <class T>
static bool operator!=(const  Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >   & t1,const  Vertex<T,2,char,std::char_traits<char>,std::allocator<char> >  & t2){
	return  !(t1==t2);
}

typedef  Vertex<double,2,char,std::char_traits<char>,std::allocator<char> >   Vertex2d;
typedef  Vertex<int,2,char,std::char_traits<char>,std::allocator<char> >   Vertex2i;
typedef  Vertex<float,2,char,std::char_traits<char>,std::allocator<char> >   Vertex2f;
end_gtl_namespace
