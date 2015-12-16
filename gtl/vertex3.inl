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
 
template<class T>
class Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >
:public Object<char,std::char_traits<char>,std::allocator<char> >{
protected:
	T  _data[3];  
public:
	virtual std::basic_string<char,std::char_traits<char>,std::allocator<char> > getClassName(){
		return "Vertex3";
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
		memcpy(_data,data,sizeof(T)*3);
	}
	void copy(const  Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > & p){
		for(int i=0;i<3;i++)
			_data[i]=p._data[i];
	}

	Vertex( ){ 
	}

	Vertex(T xx, T yy, T zz){		 
			_data[0]=xx;
			_data[1]=yy;
			_data[2]=zz;
	}
	Vertex(const T * data ){
		memcpy(_data,data,sizeof(T)*3);
	}
	Vertex(const  Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > & p){
		copy(p);
	}
	
	virtual ~Vertex(){
	}
	 Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > operator=(const   Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > & p){
		copy(p);
		return *this;
	}
	 Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > operator+=(const   Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > & p){
		for(int i=0;i<3;i++)
			_data[i]+=p._data[i];
		return *this;
	}
	 Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > operator-=(const   Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > & p){
		for(int i=0;i<3;i++)
			_data[i]-=p._data[i];
		return *this;
	}
	 Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > operator*=(double rhd)
	{ 
		for(int i=0;i<3;i++)
			_data[i] *= rhd; 
		return *this;
	}
	/** 内联函数。重载二元操作符"^="，用于对向量的复合赋值（叉乘）
	* @param [in] vt3d	当前用于复合赋值的向量
	* @return 
	*/
	inline  Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > operator^=(const  Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >& vt3d)
	{
		T dTempX,dTempY,dTempZ;
		dTempX = getY() * vt3d.getZ() - getZ() * vt3d.getY();
		dTempY = getZ() * vt3d.getX() - getX() * vt3d.getZ();
		dTempZ = getX() * vt3d.getY() - getY() * vt3d.getX();
		set(dTempX,dTempY,dTempZ);
		return *this;
	}

	inline const T  & getX() const { return _data[0];}
	inline const T  & getY() const { return _data[1];}
	inline const T  & getZ() const { return _data[2];}

	inline void setX(const T  & t)   {  _data[0]=t;}
	inline void setY(const T  & t)   {  _data[1]=t;}
	inline void setZ(const T  & t)   {  _data[2]=t;}
	inline void set(const T  & x, const T & y, const T & z)   { 
		_data[0]=x;
		_data[1]=y;
		_data[2]=z;
	}

	T &  operator[]( size_t  i){
		return _data[i];
	}
	const T &  operator[]( const size_t  i) const {
		return _data[i];
	}
	void zero(){
		for(int i=0;i<3;i++)
			_data[i]=0;
	}
	inline void getEnvelope(T minv[3],T maxv[3])
	{
		for(int i=0;i<3;i++)
		{
			maxv[i]=_data[i]+TOLERANCE;
			minv[i]=_data[i]-TOLERANCE;
		}
	}

public:
	static double calculateDistance(const  Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > & p1,const  Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > & p2 ){
		return sqrt(calculateDistance2(p1,p2));
	}
	static double calculateDistance2(const  Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > & p1,const  Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > & p2){		
		double s = 0;
		for(size_t i =0;i<3;i++){
			s+=(p1._data[i]-p2._data[i])*(p1._data[i]-p2._data[i]);
		}
		return s;
	}
public:
	/** 内联函数。重载一元操作符"-"，用于取得向量或坐标的相反值。
	* @param 
	* @return 向量或坐标的相反值
	*/
	inline  Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  operator-() const
	{
		return VertVertex<T,3>(-getX(),-getY(),-getZ());
	}

	/** Length of the vector = sqrt( vec . vec ) */
	inline double  length() const  {
		return sqrt(getX()*getX() + getY()*getY() + getZ()*getZ());
	}

	/** Length squared of the vector = vec . vec */
	inline double length2() const {
		return getX()*getX() + getY()*getY() + getZ()*getZ();
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
			_data[2] *= inv;
		}                
		return( norm );
	}
	inline double  dotProduct(const  Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >& vec) const
	{
		return get * vec.getX() + getY() * vec.getY() + getZ() * vec.getZ();
	}
	inline   Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  crossProduct( const   Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >& vt3d ) const
	{		
        double dTempX = getY() * vt3d.getZ() - getZ() * vt3d.getY();
        double dTempY = getZ() * vt3d.getX() - getX() * vt3d.getZ();
        double dTempZ = getX() * vt3d.getY() - getY() * vt3d.getX();
		return  Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > (dTempX,dTempY,dTempZ); 
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////
template <class T> 
static bool operator==(const  Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  & t1,const  Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > & t2){	
	for(int i=0;i<3;i++){
		if(t1[i]!=t2[i])
			return false;
	}
	return true;
}

template <class T> 
static   Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > operator*(const   Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >   & t11,double a ){
	 Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  & t1=( Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  &) t11;
	 Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > t;
	for(int i=0;i<t1.size();i++){
		t[i]=t1[i]*a;
	}
	return t;
}

template <class T> 
static   Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > operator*(double a,const   Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >   & t11 ){
	 Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  & t1=( Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  &) t11;
	 Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > t;
	for(int i=0;i<t1.size();i++){
		t[i]=t1[i]*a;
	}
	return t;
}

template <class T> 
static T  operator*(const   Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >   & t1,const   Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  & t2){

	T t=0;
	for(int i=0;i<3;i++){
		t+=(t1[i]*t2[i]);
	}
	return t;
}

template <class T> 
static  Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  operator+(const   Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >   & t11,const   Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  & t22){
	 Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  & t1=( Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  &) t11;
	 Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  & t2=( Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  &) t22;

	 Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > t; 
	for(int i=0;i<3;i++){
		t[i]=t1[i]+t2[i];
	}
	return t;
}

template <class T> 
static  Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  operator / (const   Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >   & t11,const T & d){
	 Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  & t1=( Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  &) t11;
	 Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > t; 
	for(int i=0;i<3;i++){
		t[i]=t1[i]/d;
	}
	return t;
}

template <class T> 
static   Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  operator-(const   Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >   & t11,const   Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  & t22){
	 Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  & t1=( Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  &) t11;
	 Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  & t2=( Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  &) t22;

	 Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > t; 
	for(int i=0;i<3;i++){
		t[i]=t1[i]-t2[i];
	}
	return t;
}


template <class T> 
static bool operator!=(const   Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >   & t1,const   Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >  & t2){
	return  !(t1==t2);
}

//////////////////////////////////////////////////////////////////////////////////////////
template <class T> 
static const   Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > operator^(const   Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >& lhv, const   Vertex<T,3,char,std::char_traits<char>,std::allocator<char> >& rhv)
{
	 Vertex<T,3,char,std::char_traits<char>,std::allocator<char> > vRes = lhv;
	vRes ^= rhv;
	return vRes;
}


typedef  Vertex<double,3,char,std::char_traits<char>,std::allocator<char> >  Vertex3d;
typedef  Vertex<float,3,char,std::char_traits<char>,std::allocator<char> >  Vertex3f;
typedef  Vertex<int,3,char,std::char_traits<char>,std::allocator<char> >  Vertex3i;
 
typedef  Vertex<float,3,char,std::char_traits<char>,std::allocator<char> >  Color3f;
typedef  Vertex<uint8_t,3,char,std::char_traits<char>,std::allocator<char> >  Color3b;

end_gtl_namespace
