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
 
#include "identifier.h"
#include "timer.h"
#include "randomnumbers.h"
#include "pointerarray.h"
begin_gtl_namespace

#define  MAX_INTERVAL_NUMBER_PRENODE 20

template <
	typename T, 
	typename CHARTYPE=char,
	typename TRAITS=std::char_traits<CHARTYPE>,
	typename ALLOC=std::allocator<CHARTYPE>  >
class Interval:  public virtual Object<CHARTYPE,TRAITS,ALLOC> {//间隔对象 
public:
	virtual std::basic_string<CHARTYPE,TRAITS,ALLOC> getClassName(){
		return "Interval";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		count = sizeof(T)* 2 + sizeof(int);
		buffer =(void*) new char[count];
		int * p = (int*)buffer;
		*p = _type;
		++p;
		T * t = (T*)p;
		*t = _start;
		++t;
		*t = _end;
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){
		int * p = (int*)buffer;
		_type = *p ;
		++p;
		T * t = (T*)p;
		_start = *t ;
		++t;
		_end = *t ; 
	}
	virtual Object * clone() {
		return new Interval<T, CHARTYPE, TRAITS, ALLOC>(_start,_end,_type);
	}
public:
	enum IntervalType {
		IT_RIGHTOPEN = 0x0,
		IT_LEFTOPEN,
		IT_OPEN,
		IT_CLOSED
	};
protected:
	T _start;//间隔开始点
	T _end;  //间隔结束点
	int    _type;
protected:
	void * _parent;
public:

	//获取开始点的值
	inline T start() { 
		return _start;
	}
	//获取结束点的值
	inline T end() { 
		return _end;
	}
	//获取开始点的值
	inline const T start() const { 
		return _start;
	} 
	//获取结束点的值
	inline const T end() const { 
		return _end;
	}
	//get interval value,equal to )end-start)
	inline T during() {
		return _end-_start;
	}	

	inline void reset(const T & s, const T & e, int type= IT_CLOSED){
		_start=s;
		_end=e;
		_type = type;
	}

	//设置开始点的值
	inline void start(T s) {  
		_start=s;
	}
	//获取结束点的值
	inline void end(T e) {
		_end=e;
	}

	inline void * parent(){
		return _parent;
	}

	inline void parent(void *p){ 
		_parent=p;
	}

	inline T center() {
		return (_start+_end)/2.0;
	}

	inline int getType()const {
		return _type;
	}

	inline void setType(int t) { 
		_type=t;
	}

	

	inline void copy(const Interval  & i) {
		_start =i._start;
		_end =i._end;
		_type=i._type;
	}

	inline Interval  operator+=(const T & t) {
		_start+=t;
		_end+=t;
		return *this;
	}
public:
	Interval (){
		_start=0;
		_end=0;
		_type=IT_CLOSED;
	}

	//带参数构造函数，s为开始点，e为结束点
	Interval (T s, T e ):_start(s),_end(e){
		_type=IT_CLOSED;
	}

	Interval (T s, T e,int type ):_start(s),_end(e),_type(type){
	}

	Interval(const Interval & i) {
		_start =i._start;
		_end =i._end;
		_type = i._type;
	}

	Interval  operator = (const Interval  & i)	{
		_start =i._start;
		_end =i._end; 
		_type = i._type;
		return *this;
	}
		
public://operators
	inline bool operator == (const Interval  & i)	{
		if (fabs(i._start - i._start) < TOLERANCE && fabs(_end - i._end) < TOLERANCE && _type == i._type)
			return true;
		else
			return false;
	}

	inline bool equals(const Interval  & i){
		if (fabs(i._start - i._start) < TOLERANCE && fabs(_end - i._end) < TOLERANCE && _type == i._type)
			return true;
		else
			return false;
	}
	/* this function moves to IntervalOperators
	inline bool intersects(IntervalType type, const double low, const double high) const {

		if (_start > high || _end < low) return false;
		if ((_start > low && _start < high) || (_end > low && _end < high)) return true;

		switch (_type)
		{
		case IT_CLOSED:
			if (_start == high)
			{
				if (type == IT_CLOSED || type == IT_LEFTOPEN) return true;
				else return false;
			}
			else if (_end == low)
			{
				if (type == IT_CLOSED || type == IT_RIGHTOPEN) return true;
				else return false;
			}
			break;
		case IT_OPEN:
			if (_start == high || _end == low) return false;
			break;
		case IT_RIGHTOPEN:
			if (_start == high)
			{
				if (type == IT_CLOSED || type == IT_LEFTOPEN) return true;
				else return false;
			}
			else if (_end == low)
			{
				return false;
			}
			break;
		case IT_LEFTOPEN:
			if (_start == high)
			{
				return false;
			}
			else if (_end == low)
			{
				if (type == IT_CLOSED || type == IT_RIGHTOPEN) return true;
				else return false;
			}
			break;
		}

		return true;
	}
	*/
	/* this function moves to IntervalOperators
	inline bool intersects(const Interval& i) const {
		return intersects((enum IntervalType)i._type, i._start, i._end);
	}

	inline bool contains(const Interval& i) const 	{

		double low = i._start;
		double high = i._end;
		IntervalType type = (IntervalType) i._type;

		if (_start < low && _end > high) return true;
		if (_start > low || _end < high) return false;

		switch (_type)
		{
		case IT_CLOSED:
			break;
		case IT_OPEN:
			if ((_start == low && _end == high && type != IT_OPEN) ||
				(_start == low && (type == IT_CLOSED || type == IT_RIGHTOPEN)) ||
				(_end == high && ( type == IT_CLOSED || type == IT_LEFTOPEN)))
				return false;
			break;
		case IT_RIGHTOPEN:
			if (_end == high && (type == IT_CLOSED || type == IT_LEFTOPEN))
				return false;
			break;
		case IT_LEFTOPEN:
			if (_start == low && (type == IT_CLOSED || type == IT_RIGHTOPEN))
				return false;
			break;
		}

		return true;
	}
	*/

	/* this function moves to IntervalOperators
	static void random(int n,T mind, T maxd, std::vector<Interval<T,CHARTYPE,TRAITS,ALLOC> > & vs)
	{
		vs.resize(n);
		unsigned int k =0;
		bool repeat =false;
		while(k<n)
		{
			RandomNumbers<T> s(n,(double)mind,(double)maxd),e(n,(double)mind,(double)maxd);
			Interval<T,CHARTYPE,TRAITS,ALLOC> v;
			for(int i=0;i<n;i++)
			{
				if(s[i]<e[i]){
					v.start(s[i]);
					v.end(e[i]);
				}
				else if(s[i]==e[i]){
					double t = -1;//rand()%100/100.0*(maxd-mind)+mind;
					while(t==e[i])
						t = rand()%100/100.0*(maxd-mind)+mind;
					if(t>s[i])
						e[i]=t;
					else{
						e[i]=s[i];
						s[i]=t;
					}
				}
				else
				{
					v.start(e[i]);
					v.end(s[i]);
				}
				//查看是否有重复
				repeat =false;
				for(int j=0;j<k;j++)
				{
					if(vs[j]==v)
					{
						repeat=true;
						break;
					}
				}
				if(repeat == false)
				{
					if(k==n)
						return;

					vs[k]=v;
					k++;
				}
			}			
		}
	}
	*/
};
//////////////////////////////////////////////////////////////////////////////////////////////
//class Interval1d{//间隔对象，可以表示时间段等
//public:
//	enum IntervalType
//	{
//		IT_RIGHTOPEN = 0x0,
//		IT_LEFTOPEN,
//		IT_OPEN,
//		IT_CLOSED
//	};
//private:
//	double _start;//间隔开始点
//	double _end;  //间隔结束点
//	int    _type;
//
//	Interval4d * _parent;
//public:
//	
//	//获取开始点的值
//	inline double start() { return _start;}
//	//获取结束点的值
//	inline double end() { return _end;}
//	
//	//设置开始点的值
//	inline void start(double s) {  _start=s;}
//	//获取结束点的值
//	inline void end(double e) {  _end=e;}
//
//	inline Interval4d * parent(){return _parent;}
//
//	inline void parent(Interval4d *p){ _parent=p;}
//
//	inline bool equals(const Interval1d & i){
//		if(fabs(i._start-i._start)<TOLERANCE && fabs(_end-i._end)<TOLERANCE)
//			return true;
//		else
//			return false;
//	}
//	inline void copy(const Interval1d & i)
//	{
//		_start =i._start;
//		_end =i._end;
//		_parent=i._parent;
//	}
//	inline Interval1d operator+=(const double & t)
//	{
//		_start+=t;
//		_end+=t;
//		return *this;
//	}
//public:
//	Interval1d();
//	//带参数构造函数，s为开始点，e为结束点
//	Interval1d(double s, double e,Interval4d* p);
//	Interval1d(const Interval1d & i);
//	Interval1d operator = (const Interval1d & i);
//	bool operator == (const Interval1d & i);
//public:
//	static void random(int n,double mind, double maxd, std::vector<Interval1d> & vs);
//public:
//	bool intersects(IntervalType type, const double low, const double high) const;
//	bool intersects(const Interval1d& i) const;
//	bool contains(const Interval1d& i) const;
//};

////////////////////////////////////////////////////////////////////////////////////////
//template <typename T, int DIMS=4, typename IDENTIFIER = Identifier64 >
//class Intervals{//间隔对象，可以表示时间段等
//	IDENTIFIER         _identifier;
//	Interval<T,CHARTYPE,TRAITS,ALLOC>        _data[DIMS];
//	unsigned long long _offset;//文件中的偏移
//	unsigned char      _flags[DIMS];//记录对应的Interval的状态 
//public:
//	Intervals();
//	Intervals(const Intervals & i4d);
//	Intervals(unsigned long long id, T ts, T te, T boundingBox[6]);
//	inline Interval<T,CHARTYPE,TRAITS,ALLOC> & get(int i){
//		return _data[i];
//	}
//	inline void set(int i,const Interval<T,CHARTYPE,TRAITS,ALLOC> &iv){
//		_data[i]=iv;
//		_data[i].parent(this);
//	}
//	inline void set(int i,T s, T e){
//		_data[i].start(s);
//		_data[i].end(e);
//		_data[i].parent(this);
//	}
//	inline unsigned long long offset() const {return _offset;}
//
//	inline void offset(unsigned long long v)  { _offset=v;}
//
//	inline unsigned int flags(){ 
//		unsigned int i =0; 
//		unsigned int s=0;
//		for(int j=0;j<DIMS;i++){
//			i=_flags[j];
//			s+=(i<<(8*(DIMS-1-j)));
//		}
//		/*i=_flags[0];
//		s+=i<<24;
//		i = _flags[1];
//		s+=i<<16;
//		i = _flags[2];
//		s+=i<<8;
//		s+=_flags[3];*/
//		return s;
//	}
//	inline unsigned char flags(int i){ 
//		return _flags[i];
//	}
//	inline void flags(int i,unsigned char f){ 
//		_flags[i]=f;
//	}
//	inline void get(T minv[DIMS], T maxv[DIMS]){
//		for(int i=0;i<DIMS;i++){
//			minv[i]=_data[i].start();
//			maxv[i]=_data[i].end();
//		}
//	}
//
//	static void random(int n,T mind, T maxd, std::vector<Intervals<T, DIMS, IDENTIFIER>*> & vs);
//	static bool readIndexFile(const std::string & szFileName,std::vector<Intervals<T, DIMS, IDENTIFIER> *> & vs);
//	static void calculateEnvelope(const std::vector<Intervals<T, DIMS, IDENTIFIER>*> & vs, T minv[DIMS], T maxv[DIMS]);
//};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//class IntervalArray{ 
//	Interval1dPtr                   *_intervals;
//	unsigned int                   _intervalsNumber;
//	unsigned int                   _capacity;
//public:
//	typedef Interval1dPtr *   iterator;
//
//	IntervalArray(unsigned int maxsize=MAX_INTERVAL_NUMBER_PRENODE);
//	~IntervalArray();
//	IntervalArray(const IntervalArray & ia);
//
//	inline iterator begin() { return (iterator) _intervals;}
//	inline iterator end() { return (iterator) _intervals + _intervalsNumber;}
//	inline size_t size() {return _intervalsNumber;}
//	void push_back(Interval1dPtr p);
//	inline unsigned int capacity() {return _capacity;}
//	inline Interval1dPtr & operator[] (unsigned int i){
//		return _intervals[i];
//	}
//	inline void clear();
//	inline void resize(unsigned int s);
//};
////////////////////////////////////////////////////////////////////////////////////////////
typedef Interval<double,char,std::char_traits<char>,std::allocator<char>> Interval1d;
typedef Interval1d* Interval1dPtr; 
end_gtl_namespace