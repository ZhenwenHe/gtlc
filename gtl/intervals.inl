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
#include "assert.h"
begin_gtl_namespace
 
//Interval1d::Interval1d(){_start=0;_end=0;_parent=0;_start=IT_CLOSED;}
////带参数构造函数，s为开始点，e为结束点
//Interval1d::Interval1d(double s, double e,Intervals* p):_start(s),_end(e),_parent(p){
//	_start=IT_CLOSED;
//}
//
//Interval1d::Interval1d(const Interval1d & i) {
//	_start =i._start;
//	_end =i._end;
//	_parent=i._parent;
//	_type = i._type;
//}
//Interval1d Interval1d::operator = (const Interval1d & i)
//{
//	_start =i._start;
//	_end =i._end;
//	_parent=i._parent;
//	_type = i._type;
//	return *this;
//}
//bool Interval1d::operator == (const Interval1d & i)
//{
//	if(fabs(i._start-i._start)<TOLERANCE && fabs(_end-i._end)<TOLERANCE && _type==i._type)
//		return true;
//	else
//		return false;
//}
// 
//void Interval1d::random(int n,double mind, double maxd, std::vector<Interval1d> & vs)
//{
//	vs.resize(n);
//	unsigned int k =0;
//	bool repeat =false;
//	while(k<n)
//	{
//		RandomNumbers s(n,mind,maxd),e(n,mind,maxd);
//		Interval1d v;
//		for(int i=0;i<n;i++)
//		{
//			if(s[i]<e[i]){
//				v.start(s[i]);
//				v.end(e[i]);
//			}
//			else
//			{
//				v.start(e[i]);
//				v.end(s[i]);
//			}
//			//查看是否有重复
//			repeat =false;
//			for(int j=0;j<k;j++)
//			{
//				if(vs[j]==v)
//				{
//					repeat=true;
//					break;
//				}
//			}
//			if(repeat == false)
//			{
//				if(k==n)
//					return;
//
//				vs[k]=v;
//				k++;
//			}
//		}			
//	}
//}
// 
//bool Interval1d::intersects(IntervalType type, const double low, const double high) const
//{
//
//	if (_start > high || _end < low) return false;
//	if ((_start > low && _start < high) || (_end > low && _end < high)) return true;
//
//	switch (_type)
//	{
//	case IT_CLOSED:
//		if (_start == high)
//		{
//			if (type == IT_CLOSED || type == IT_LEFTOPEN) return true;
//			else return false;
//		}
//		else if (_end == low)
//		{
//			if (type == IT_CLOSED || type == IT_RIGHTOPEN) return true;
//			else return false;
//		}
//		break;
//	case IT_OPEN:
//		if (_start == high || _end == low) return false;
//		break;
//	case IT_RIGHTOPEN:
//		if (_start == high)
//		{
//			if (type == IT_CLOSED || type == IT_LEFTOPEN) return true;
//			else return false;
//		}
//		else if (_end == low)
//		{
//			return false;
//		}
//		break;
//	case IT_LEFTOPEN:
//		if (_start == high)
//		{
//			return false;
//		}
//		else if (_end == low)
//		{
//			if (type == IT_CLOSED || type == IT_RIGHTOPEN) return true;
//			else return false;
//		}
//		break;
//	}
//
//	return true;
//}
//bool Interval1d::intersects(const Interval1d& i) const
//{
//	return intersects((enum IntervalType)i._type, i._start, i._end);
//}
//
//bool Interval1d::contains(const Interval1d& i) const
//{
//
//	double low = i._start;
//	double high = i._end;
//	IntervalType type = (IntervalType) i._type;
//
//	if (_start < low && _end > high) return true;
//	if (_start > low || _end < high) return false;
//
//	switch (_type)
//	{
//	case IT_CLOSED:
//		break;
//	case IT_OPEN:
//		if ((_start == low && _end == high && type != IT_OPEN) ||
//			(_start == low && (type == IT_CLOSED || type == IT_RIGHTOPEN)) ||
//			(_end == high && ( type == IT_CLOSED || type == IT_LEFTOPEN)))
//			return false;
//		break;
//	case IT_RIGHTOPEN:
//		if (_end == high && (type == IT_CLOSED || type == IT_LEFTOPEN))
//			return false;
//		break;
//	case IT_LEFTOPEN:
//		if (_start == low && (type == IT_CLOSED || type == IT_RIGHTOPEN))
//			return false;
//		break;
//	}
//
//	return true;
//}
 
///////////////////////////////////////////////////////////////////////////////////////////////////////

//IntervalArray::IntervalArray(unsigned int maxsize){ 
//	_capacity = maxsize;
//	_intervals=new Interval1dPtr[_capacity];
//	_intervalsNumber=0;
//}
//IntervalArray::~IntervalArray(){
//	clear();
//}
//IntervalArray::IntervalArray(const IntervalArray & ia){
//	_capacity = ia._capacity;
//	_intervals=new Interval1dPtr[_capacity];
//	_intervalsNumber=ia._intervalsNumber;
//	for(int i=0;i<_capacity;i++)
//		_intervals[i]=ia._intervals[i];
//}
//void IntervalArray::push_back(Interval1dPtr p) {
//	if(_intervalsNumber<_capacity){
//		_intervals[_intervalsNumber]=p;
//		_intervalsNumber++;
//	}
//	else{
//		_capacity++;
//		Interval1dPtr * pp = new Interval1dPtr[_capacity];
//		for(unsigned int i=0;i<_intervalsNumber;i++)
//			pp[i]=_intervals[i];
//		delete [] _intervals;
//		_intervals=pp;
//		_intervals[_intervalsNumber]=p;
//		_intervalsNumber++;
//	}
//} 
//void IntervalArray::clear() {
//	_intervalsNumber=0;
//	for(int i=0;i<_capacity;i++)
//		_intervals[i]=0;
//	delete [] _intervals;
//	_intervals=0;
//}
//void IntervalArray::resize(unsigned int s){ 
//	if(_capacity<s){
//		_capacity =s;
//		Interval1dPtr * pp = new Interval1dPtr[_capacity];
//		for(unsigned int i=0;i<_intervalsNumber;i++)
//			pp[i]=_intervals[i];
//		delete [] _intervals;
//		_intervals=pp;
//	}
//	_intervalsNumber=s;
//}
	 
//////////////////////////////////////////////////////////////////////////////////
//template <typename T, int DIMS, typename IDENTIFIER  >
//Intervals<T, DIMS, IDENTIFIER>::Intervals(){
//	_identifier = IDENTIFIER::generate();
//	for(int i=0;i<DIMS;i++){ 
//		_data[i].parent(this);
//		_flags[i]=0;
//	}
//	_offset=0;
//
//}
//template <typename T, int DIMS, typename IDENTIFIER  >
//Intervals<T, DIMS, IDENTIFIER>::Intervals(const Intervals & i4d)
//{
//	for(int i=0;i<DIMS;i++){
//		_data[i]=i4d._data[i];
//		_data[i].parent((void*) this);
//		_flags[i]=i4d._flags[i];
//	}
//	_identifier = i4d._identifier;
//	_offset=i4d._offset;
//}
//template <typename T, int DIMS, typename IDENTIFIER  >
//void Intervals<T, DIMS, IDENTIFIER>::random(int n,T mind, T maxd, std::vector<Intervals<T, DIMS, IDENTIFIER>*> & vs)
//{
//	std::vector<Interval<T> > intvs;
//	vs.resize(n);
//	for(int j=0;j<n;j++)
//		vs[j] = new Intervals<T, DIMS, IDENTIFIER>();
//	for(int i=0;i<4;i++)
//	{
//		intvs.clear();
//		Interval<T>::random(n,mind,maxd,intvs);
//		for(int j=0;j<n;j++)
//			vs[j]->set(i,intvs[j]);
//	}
//}
//template <typename T, int DIMS, typename IDENTIFIER  >
//Intervals<T, DIMS, IDENTIFIER>::Intervals(unsigned long long id, T ts, T te, T boundingBox[6])
//{
//	assert(DIMS==4);
//
//	_identifier=id;
//	_data[0].start(ts);
//	_data[0].end(te);
//
//	_data[1].start(boundingBox[0]);
//	_data[2].start(boundingBox[2]);
//	_data[3].start(boundingBox[4]);
//
//	_data[1].end(boundingBox[1]);	
//	_data[2].end(boundingBox[3]);
//	_data[3].end(boundingBox[5]);
//
//	for(int i=0;i<DIMS;i++)
//		_data[i].parent((void*)this);
//}
//
//template <typename T, int DIMS, typename IDENTIFIER  >
//bool Intervals<T, DIMS, IDENTIFIER>::readIndexFile(const std::string & szFileName,std::vector<Intervals<T, DIMS, IDENTIFIER>*> & vs)
//{
//	assert(DIMS==4);
//	struct IndexItem
//	{
//		unsigned long long    _fid; //对象的ID
//		unsigned long long _fclsid; //对象
//		int    _state; //标示记录记录的状态：0原始数据；1删除标示；2更新标示
//		unsigned long _offset; //记录文件指针的起始位置:相对于文件头进行偏移的长度
//		unsigned long _bufLength; //记录该块buf的长度
//		unsigned long _bufLengthNoPro;//记录feature没有属性信息的长度
//		unsigned long long  _writeTime;	//记录文件写入的时间
//		unsigned long long  _updateTime;	//记录文件写入的时间
//		double _bound[6];//对象的包围盒:供建立索引使用
//	};
//
//	std::locale loc = std::locale::global(std::locale(""));//设置环境为系统环境
//
//	std::ifstream iFsteram(szFileName.c_str(),std::ios::in|std::ios::binary);
//	if (!iFsteram){
//		return false;
//	}
//	iFsteram.seekg(0,std::ios_base::end); // 移动到文件尾 
//	std::size_t len = iFsteram.tellg();//取得当前位置的指针长度 即文件长度
//	if (len>0)
//	{
//		char *pv = new char[len];
//		iFsteram.seekg(0,std::ios_base::beg); // 移动到文件头 
//		iFsteram.read(pv,len);
//		int nUint = sizeof(IndexItem);
//		int nSize = len/nUint;
//		vs.resize(nSize);
//		std::vector<Interval1d> vi;
//		Interval1d::random(nSize,1000,10000,vi);
//
//		for (int i = 0; i < nSize;i++){
//
//			IndexItem * pii =  (IndexItem*)(pv+nUint*i);			
//			vs[i]=new Intervals<T, DIMS, IDENTIFIER>(pii->_fid,vi[i].start(),vi[i].end(),pii->_bound);
//		}
//
//		delete  [] pv;
//		pv = NULL;
//	}
//	iFsteram.close();
//	std::locale::global(loc);
//	return true;
//}
//template <typename T, int DIMS, typename IDENTIFIER  >
//void Intervals<T, DIMS, IDENTIFIER>::calculateEnvelope(const std::vector<Intervals<T, DIMS, IDENTIFIER>*> & vs, T minv[DIMS], T maxv[DIMS])
//{
//	for(int i=0;i<DIMS;i++)
//	{
//		minv[i]=DBL_MAX;
//		maxv[i]= - DBL_MAX;
//	}
//
//	for(std::vector<Intervals*>::const_iterator it = vs.begin();it!=vs.end();it++)
//	{
//		for(int i=0;i<DIMS;i++)
//		{
//			if(minv[i]> (*it)->get(i).start())
//				minv[i] = (*it)->get(i).start();
//			if(maxv[i]<(*it)->get(i).end())
//				maxv[i]=(*it)->get(i).end();
//		}
//	}
//}
end_gtl_namespace
