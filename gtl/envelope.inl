#pragma  once

#include "vertex.h"
begin_gtl_namespace
template <class T>
class Envelope<T,2,char,std::char_traits<char>,std::allocator<char> >
:virtual public Object<char,std::char_traits<char>,std::allocator<char>>{
public:
	virtual std::basic_string<char,std::char_traits<char>,std::allocator<char> > getClassName(){
		return "Envelope";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
public: 
	T minData[2];//记录各维分量的最小值
	T maxData[2];//记录各维分量的最大值
public:
	Envelope(){
		minData[1]=minData[0]=DBL_MAX;	
		maxData[1]=maxData[0]=DBL_MIN;
	}
	Envelope( double x1, T x2, T y1, T y2){
		minData[0]=x1;
		minData[1]=y1;
		maxData[1]=y2;
		maxData[0]=x2;
	}
	Envelope(const Envelope<T,2,char,std::char_traits<char>,std::allocator<char> > & e){
		minData[0]=e.minData[0];
		minData[1]=e.minData[1];
		maxData[1]=e.maxData[1];
		maxData[0]=e.maxData[0];
	}
	Envelope<T,2,char,std::char_traits<char>,std::allocator<char> > operator = (const Envelope<T,2,char,std::char_traits<char>,std::allocator<char> > & e){
		minData[0]=e.minData[0];
		minData[1]=e.minData[1];
		maxData[1]=e.maxData[1];
		maxData[0]=e.maxData[0];
		return *this;
	}
public:
	inline void moveTo(T x, T y){
		T w = maxData[0]-minData[0];
		minData[0] = x-w/2;
		maxData[0] = x+w/2;
		w = maxData[1]-minData[1];
		minData[1] = y-w/2;
		maxData[1] = y+w/2;
	}
	inline void moveTo(Vertex2d & v){
		moveTo(v.getX(),v.getY());
	}
	inline Vertex2d center(){
		return Vertex2d ((minData[0]+maxData[0])/2,(minData[1]+maxData[1])/2);
	}
	inline Vertex2d getMin() {
		return Vertex2d (minData[0],minData[1]);   
	}
	inline Vertex2d getMax() { 
		return Vertex2d (maxData[0],maxData[1]);  
	}
	inline void get(T v[4]){
		v[0]=minData[0];		v[1]=maxData[0];
		v[2]=minData[1];		v[3]=maxData[1];
	}
	inline void set(T v[4]){
		minData[0]=v[0];		maxData[0]=v[1];
		minData[1]=v[2];		maxData[1]=v[3];
	}
	inline void get(T minv[2],T maxv[2]){
		minv[0]=minData[0];minv[1]=minData[1];
		maxv[0]=maxData[0];maxv[1]=maxData[1];
	}
	inline void set(T minv[2],T maxv[2]){
		minData[0]=minv[0];minData[1]=minv[1];
		maxData[0]=maxv[0];maxData[1]=maxv[1];
	}	
	inline void set( T x1, T x2, T y1, T y2) {minData[0]=x1;maxData[0]=x2;minData[1]=y1,maxData[1]=y2;}
	inline T lengthX()const{return maxData[0]-minData[0];}
	inline T lengthY()const {return maxData[1]-minData[1];}
	/*忽略v.z判断点是否在包围盒内0-表示在外面，1-表示在里面，2-表示在边上
	*/
	inline int test (const Vertex3d & v)const
	{
		if(v.getX()>maxData[0] || v.getX()<minData[0] || v.getY()>maxData[1] || v.getY()<minData[1])
			return 0;
		else
			return 1;
	}
	/*判断点是否在包围盒内0-表示在外面，1-表示在里面，2-表示在边上
	*/
	inline int test (const Vertex2d & v)const 
	{
		if(v.getX()>maxData[0] || v.getX()<minData[0] || v.getY()>maxData[1] || v.getY()<minData[1])
			return 0;
		else
			return 1;
	}
	/*判断点是否在包围盒内0-表示在外面，1-表示在里面，2-表示在边上
	*/
	inline int test (const T & x, const T  & y)const 
	{
		if(x>maxData[0] || x<minData[0] || y>maxData[1] || y<minData[1])
			return 0;
		else
			return 1;
	}

public:

	inline void extend(const Vertex2d & v){
		if(valid()){
			if(minData[0]>v.getX()) minData[0] = v.getX();
			if(minData[1]>v.getY()) minData[1] = v.getY();

			if(maxData[0]<v.getX()) maxData[0] = v.getX();
			if(maxData[1]<v.getY()) maxData[1] = v.getY();
		}
		else{
			minData[0] = v.getX() - TOLERANCE*2;
			minData[1] = v.getY() - TOLERANCE*2;

			maxData[0] = v.getX() + TOLERANCE*2;
			maxData[1] = v.getY() + TOLERANCE*2;
		}
	}
	inline void extend(const Envelope<T,2,char,std::char_traits<char>,std::allocator<char> > & v){
		if(valid() && v.valid() ){
			if(minData[0]>v.minData[0]) minData[0] = v.minData[0];
			if(minData[1]>v.minData[1]) minData[1] = v.minData[1];

			if(maxData[0]<v.maxData[0]) maxData[0] = v.maxData[0];
			if(maxData[1]<v.maxData[1]) maxData[1] = v.maxData[1];
			return ;
		}
		if(v.valid()){
			minData[0] = v.minData[0];
			minData[1] = v.minData[1];

			maxData[0] = v.maxData[0];
			maxData[1] = v.maxData[1];
		
			return ;
		}
	}

	inline void extend(const T v[4]){
		if(valid()){
			if(minData[0]>v[0]) minData[0] = v[0];
			if(minData[1]>v[2]) minData[1] = v[2];
		
			if(maxData[0]<v[1]) maxData[0] = v[1];
			if(maxData[1]<v[3]) maxData[1] = v[3];
		
			return ;
		}
		else{
			minData[0] = v[0];
			minData[1] = v[2];
		 
			maxData[0] = v[1];
			maxData[1] = v[3];
		 
			return ;
		}
	}

	inline bool valid( ) const{

		if(minData[0]>=maxData[0] || minData[1]>=maxData[1] )
			return false;
		else
			return true;

	}
	
	/*
	* 0-表示两个包围盒相离
	* 1-表示两个包围盒a和b相交
	* 2-表示包围盒a在b中
	* 3-表示包围盒b在a中
	*/
	static int test(const Envelope<T,2,char,std::char_traits<char>,std::allocator<char> > & a, const Envelope<T,2,char,std::char_traits<char>,std::allocator<char> > & b){
		if(a.minData[0]>b.maxData[0] || a.maxData[0]<b.minData[0]|| a.minData[1]>b.maxData[1] || a.maxData[1]<b.minData[1])
			return 0;
		if((a.minData[0]>=b.minData[0] && a.minData[0]<=b.maxData[0])&&((a.maxData[0]>=b.minData[0] && a.maxData[0]<=b.maxData[0]))
			&&(a.minData[1]>=b.minData[1] && a.minData[1]<=b.maxData[1])&&((a.maxData[1]>=b.minData[1] && a.maxData[1]<=b.maxData[1]))){
				return 2;
		}
		if((b.minData[0]>=a.minData[0] && b.minData[0]<=a.maxData[0])&&((b.maxData[0]>=a.minData[0] && b.maxData[0]<=a.maxData[0]))
			&&(b.minData[1]>=a.minData[1] && b.minData[1]<=a.maxData[1])&&((b.maxData[1]>=a.minData[1] && b.maxData[1]<=a.maxData[1]))){
				return 3;
		}
	
		return 1;
	}
};
 
typedef Envelope<double,2,char,std::char_traits<char>,std::allocator<char> > Envelope2d;
typedef Envelope<float,2,char,std::char_traits<char>,std::allocator<char> > Envelope2f;
typedef Envelope<int,2,char,std::char_traits<char>,std::allocator<char> > Envelope2i;
end_gtl_namespace