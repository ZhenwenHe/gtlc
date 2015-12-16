#pragma  once
#include "vertex.h"
begin_gtl_namespace
//NUMDIMS维的边界矩形定义
template<typename ElemType,
	int NUMDIMS,
	typename CHARTYPE=char,
	typename TRAITS=std::char_traits<CHARTYPE>,
	typename ALLOC=std::allocator<CHARTYPE> >
class Envelope : public virtual Object<CHARTYPE,TRAITS,ALLOC>{
public:
	typedef Vertex<ElemType,NUMDIMS,CHARTYPE,TRAITS,ALLOC>  vertex_type; 
public:
	virtual std::basic_string<CHARTYPE,TRAITS,ALLOC> getClassName(){
		return "Envelope";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){		
		count = sizeof(ElemType)*NUMDIMS ;
		buffer = new unsigned char[count*2];
		memcpy(buffer, (const void*)minData, count);
		memcpy(((unsigned char*)buffer) + count, (const void*)maxData, count);
		count *= 2;
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){
		size_t c = sizeof(ElemType)*NUMDIMS;
		memcpy( (void*)minData, buffer,c);
		memcpy((void*)maxData, ((unsigned char*)buffer) +  c, c);
	}
	virtual Object * clone() {
		return new Envelope<ElemType, NUMDIMS, CHARTYPE, TRAITS, ALLOC>(minData, maxData);
	}
public:
	ElemType minData[NUMDIMS];//记录各维分量的最小值
	ElemType maxData[NUMDIMS];//记录各维分量的最大值
public:
	Envelope(){//默认构造函数
		for(int i=0;i<NUMDIMS;i++){
			minData[i]=DBL_MAX;
			maxData[i]=-DBL_MAX;
		}
	}
	Envelope(ElemType xmin, ElemType xmax, ElemType ymin, ElemType ymax){
		assert(NUMDIMS>=2);
		minData[0]=xmin;
		minData[1]=ymin;
		maxData[0]=xmax;
		maxData[1]=ymax;
	}
	Envelope(ElemType xmin, ElemType xmax, ElemType ymin, ElemType ymax,ElemType zmin, ElemType zmax){
		assert(NUMDIMS>=3);
		minData[0]=xmin;
		minData[1]=ymin;
		minData[2]=zmin;
		maxData[0]=xmax;
		maxData[1]=ymax;
		maxData[2]=zmax;
	}
	
	Envelope(const ElemType t, const ElemType x,const ElemType y,const ElemType z,ElemType r){
		assert(NUMDIMS>=4);
		minData[0]=t-r;
		minData[1]=x-r;
		minData[2]=y-r; 
		minData[3]=z-r; 
		maxData[0]=t+r;
		maxData[1]=x+r;
		maxData[2]=y+r;
		maxData[3]=z+r;
	}
	inline bool empty(){
		for(int i=0;i<NUMDIMS;i++){
			if(minData[i]==DBL_MAX || minData[i]==-DBL_MAX
				||maxData[i]==-DBL_MAX || maxData[i]==DBL_MAX)
				return true;
		}
		return false;
	}
	void makeInfinitesimal(const ElemType centerPos[NUMDIMS]){
		for(int i=0;i<NUMDIMS;i++){
			minData[i]=centerPos[i]-TOLERANCE;
			maxData[i]=centerPos[i]+TOLERANCE;
		}
	}

	void makeIndentity(){
		for(int i=0;i<NUMDIMS;i++){
			minData[i]=-1.0;
			maxData[i]=1.0;
		}		
	}
	void reset(){
		for(int i=0;i<NUMDIMS;i++){
			minData[i]=DBL_MAX;
			maxData[i]=-DBL_MAX;
		}
	}
	~Envelope(){
	}
	Envelope(const ElemType centerPos[NUMDIMS],ElemType r){
		for(int i=0;i<NUMDIMS;i++){
			minData[i]=centerPos[i]-r;
			maxData[i]=centerPos[i]+r;
		}
	}
	
	Envelope(const ElemType centerPos[NUMDIMS],std::vector<ElemType> rs){
		assert(rs.size()>=NUMDIMS);
		for(int i=0;i<NUMDIMS;i++){
			minData[i]=centerPos[i]-rs[i];
			maxData[i]=centerPos[i]+rs[i];
		}
	}
	//带参构造函数
	Envelope(const ElemType minV[NUMDIMS],const ElemType maxV[NUMDIMS]){
		set(minV,maxV);
	}
	//拷贝构造函数
	Envelope(const Envelope & br){
		set(br.minData,br.maxData);
	}
	//拷贝构造函数
	void copy(const Envelope & br){
		set(br.minData,br.maxData);
	}
	//设置边界矩形
	void set (const ElemType minV[NUMDIMS],const ElemType maxV[NUMDIMS]){
		for(int i=0;i<NUMDIMS;i++){
			minData[i]=minV[i];
			maxData[i]=maxV[i];
		}
	}
	//设置边界矩形
	void set (const ElemType & s, const ElemType & e, const ElemType minV[NUMDIMS-1],const ElemType maxV[NUMDIMS-1]){
		minData[0]=s;
		maxData[0]=e;
		for(int i=1;i<NUMDIMS;i++){
			minData[i]=minV[i-1];
			maxData[i]=maxV[i-1];
		}
	}

	void extend (const std::vector< vertex_type > & vl){	
        for(std::vector<vertex_type>::const_iterator it = vl.begin();it!=vl.end();it++){
			for(size_t i=0;i<NUMDIMS;i++){
				maxData[i]=max(maxData[0],(*it)[i]);
				minData[i]=min(minData[0],(*it)[i]);
			}
		}
	}

	void extend (const std::vector< vertex_type > & vl,unsigned long long offset, unsigned long long count){	
		for(unsigned long long j=0;j<count;j++){
			for(size_t i=0;i<NUMDIMS;i++){
				maxData[i]=max(maxData[0], vl[j+offset][i]);
				minData[i]=min(minData[0],vl[j+offset][i]);
			}
		}
	}

	void reset (const std::vector< vertex_type > & vl){
		for(int i=0;i<NUMDIMS;i++){
			minData[i]=DBL_MAX;
			maxData[i]=-DBL_MAX;
		}
		extend(vl);
	}

	//测试一个NUMDIMS点是否在边界矩形内
	//0表示不在矩形内
	int test (const ElemType v [NUMDIMS]) const{
		for(int i=0;i<NUMDIMS;i++){
			if(v[i]> maxData[i]|| v[i]< minData[i])
				return 0;
		}
		return 1;
	}

	int test (const Envelope  & e ) const{
		if(overlaps(e))
		{
			if(encloses(e))//e在envelope中
			{
				return 3;
			}
			if(e.encloses(*this))//envelope在e中
			{
				return 2;
			}
			return 1;//相交
		}
		else
			return 0;//相离
	}
	int test (const Envelope  & e ,double & percent) const{
		if(overlaps(e))
		{
			if(encloses(e))//e在envelope中
			{
				return 3;
			}
			if(e.encloses(*this))//envelope在e中
			{
				return 2;
			}
			return 1;//相交
		}
		else
			return 0;//相离
	}
	int test (const vertex_type & v)const {
		return test(v.get());
	}

	// calculates the area of a bounding box
	inline double volume() const
	{
		double area = 1;
		for (std::size_t axis = 0; axis < NUMDIMS; axis++)
			area *= (double)(maxData[axis] - minData[axis]);
		return area;
	}

	inline bool isValid() const {
		for (std::size_t axis = 0; axis < NUMDIMS; axis++)
			if(maxData[axis]<=minData[axis])
				return false;
		return true;
	}
	inline void makeValid()  {
		ElemType t;
		for (int axis = 0; axis < NUMDIMS; axis++){
			if(maxData[axis]<minData[axis]){
				t = maxData[axis];
				*(maxData+axis)=minData[axis];
				*(minData+axis)= t;
			}
			else if(maxData[axis]== minData[axis]){
				maxData[axis] = maxData[axis]+(ElemType)TOLERANCE;
				minData[axis] = minData[axis]-(ElemType)TOLERANCE;
			}
			else 
				continue;
		}
	}
	//获取最小分量向量
	inline ElemType * getLowerBounds(){return (ElemType *)minData;}
	//获取最小分量向量
	inline ElemType * getUpperBounds(){return (ElemType *)maxData;}

	//获取最小分量向量
	inline ElemType  getLowerBound(int i){return  minData[i];}
	//获取最小分量向量
	inline ElemType  getUpperBound(int i){return  maxData[i];}

	//获取最小分量向量
	inline const ElemType&  getLowerBound(int i)const {return  minData[i];}
	//获取最小分量向量
	inline const ElemType&  getUpperBound(int i)const {return  maxData[i];}

	//设置最小分量向量
	inline void  setLowerBound(int i,ElemType e){ minData[i]=e;}
	//设置最小分量向量
	inline void  setUpperBound(int i,ElemType e){ maxData[i]=e;}

	//获取最小分量向量
	inline const ElemType * getLowerBounds()const {return (ElemType *)minData;}
	//获取最小分量向量
	inline const ElemType * getUpperBounds()const {return (ElemType *)maxData;}

	//获取最小分量向量
	void getLowerBounds(ElemType v[NUMDIMS]) const {
		for(int i=0;i<NUMDIMS;i++)
			v[i] = minData[i];
	}
	//获取最小分量向量
	void getUpperBounds(ElemType v[NUMDIMS]) const {
		for(int i=0;i<NUMDIMS;i++)
			v[i] = maxData[i];
	}

	//设置最小分量向量
	inline void setLowerBounds(ElemType v[NUMDIMS])  {
		for(int i=0;i<NUMDIMS;i++)
			minData[i]=v[i] ;
	}

	//设置最小分量向量
	inline void setUpperBounds(ElemType v[NUMDIMS])  {
		for(int i=0;i<NUMDIMS;i++)
			maxData[i]=v[i];
	}

	//设置最小分量向量
	inline void setLowerBounds(const vertex_type & v)  {
		for(int i=0;i<NUMDIMS;i++)
			minData[i]=v.get(i) ;
	}

	//设置最小分量向量
	inline void setUpperBounds(const vertex_type & v)  {
		for(int i=0;i<NUMDIMS;i++)
			maxData[i]=v.get(i);
	}

	inline void getData(ElemType minv[NUMDIMS],ElemType maxv[NUMDIMS]){
		for(int i=0;i<NUMDIMS;i++){
			maxv[i] = maxData[i];
			minv[i] = minData[i];
		}
	};

	inline std::pair<ElemType,ElemType> getRange(int d){
		return std::pair<ElemType,ElemType>(minData[d],maxData[d]);
	}

	//重载+号，实现两个矩形的并操作
	inline Envelope<ElemType,NUMDIMS,CHARTYPE,TRAITS,ALLOC>  & extend( Envelope<ElemType,NUMDIMS,CHARTYPE,TRAITS,ALLOC>  & b1){
		extend(b1.minData);
		extend(b1.maxData);
		return  *this;
	}


	//使得矩形包含点pnt
	inline void extend (const ElemType pnt[NUMDIMS] ){

		for(int i=0;i<NUMDIMS;i++){
			if(pnt[i]==DBL_MAX || pnt[i]==-DBL_MAX)
				continue;
			maxData[i]=max(maxData[i],pnt[i]);
			minData[i]=min(minData[i],pnt[i]);
		}
	}

	//使得矩形包含点pnt
	inline void extend (const vertex_type & pnt ){
		for(int i=0;i<NUMDIMS;i++){ 
			maxData[i]=max(maxData[i],pnt[i]);
			minData[i]=min(minData[i],pnt[i]);
		}
	}

	inline void getCenter(ElemType ptn[NUMDIMS]){
		for(int i=0;i<NUMDIMS;i++){
			ptn[i]=(maxData[i]+minData[i])/2;
		}
	}
	inline void getCenter(vertex_type& ptn){
		for(int i=0;i<NUMDIMS;i++){
			ptn.set(i,(maxData[i]+minData[i])/2);
		}
	}
	inline  ElemType getCenter(int d){		
		return  (maxData[d]+minData[d])/2;		
	} 

	inline vertex_type  getCenter( ){
		 vertex_type pnt;
		for(int i=0;i<NUMDIMS;i++){
			pnt.set(i,(maxData[i]+minData[i])/2);
		}
		return pnt;
	}

	inline vertex_type getMinVertex() {
		return vertex_type(minData);
	}
	inline vertex_type getMaxVertex() {
		return vertex_type(maxData);
	}
	//将本矩形从e1定义的空间变换到e2定义的空间
	inline void transform(const Envelope & e1, const Envelope & e2){
		for(int i=0;i<NUMDIMS;i++){
			minData[i]=(minData[i]-e1.minData[i])/(e1.maxData[i]-e1.minData[i])*(e2.maxData[i]-e2.minData[i])+e2.minData[i];
			maxData[i]=(maxData[i]-e1.minData[i])/(e1.maxData[i]-e1.minData[i])*(e2.maxData[i]-e2.minData[i])+e2.minData[i];
		}
	}
	//将本矩形从e1定义的空间变换到e2定义的空间
	inline void transform(const Envelope & e1, const Envelope & e2,Envelope & e){
		for(int i=0;i<NUMDIMS;i++){
			e.minData[i]=(minData[i]-e1.minData[i])/(e1.maxData[i]-e1.minData[i])*(e2.maxData[i]-e2.minData[i])+e2.minData[i];
			e.maxData[i]=(maxData[i]-e1.minData[i])/(e1.maxData[i]-e1.minData[i])*(e2.maxData[i]-e2.minData[i])+e2.minData[i];
		}
	}
	//赋值重载
	Envelope operator=(const Envelope & br){
		set(br.minData,br.maxData);
		return *this;
	}
	//重载+号，实现两个矩形的并操作
	Envelope<ElemType,NUMDIMS,CHARTYPE,TRAITS,ALLOC> operator+=( Envelope<ElemType,NUMDIMS,CHARTYPE,TRAITS,ALLOC>  & b1){
		extend(b1.minData);
		extend(b1.maxData);
		return  *this;
	}
public:
	// this determines if a bounding box is fully contained within this bounding box
	inline bool encloses(const Envelope<ElemType,NUMDIMS,CHARTYPE,TRAITS,ALLOC>& bb) const
	{
		// if (y1 < x1 || x2 < y2)
		for (std::size_t axis = 0; axis < NUMDIMS; axis++)
			if (bb.minData[axis]  < minData[axis]  || maxData[axis] < bb.maxData[axis])
				return false;

		return true;
	}

	// a quicker way to determine if two bounding boxes overlap
	inline bool overlaps(const Envelope<ElemType,NUMDIMS,CHARTYPE,TRAITS,ALLOC>& bb) const
	{
		// do it this way so theres no equal signs (in case of doubles)
		// if (!(x1 < y2) && !(x2 > y1))
		for (std::size_t axis = 0; axis < NUMDIMS; axis++)
		{		
			if (!(minData[axis] < bb.maxData[axis]) || !(bb.minData[axis] < maxData[axis]))
				return false;
		}

		return true;
	}

	// calculates the total overlapping area of two boxes
	inline bool overlaps(const Envelope<ElemType,NUMDIMS,CHARTYPE,TRAITS,ALLOC>& bb, double & area) const
	{
		area = 1.0;
		for (std::size_t axis = 0; area && axis < NUMDIMS; axis++)
		{
			// this makes it easier to understand
			const double x1 = minData[axis];
			const double x2 = maxData[axis];
			const double y1 = bb.minData[axis];
			const double y2 = bb.maxData[axis];

			// left edge outside left edge
			if (x1 < y1)
			{
				// and right edge inside left edge
				if (y1 < x2)
				{
					// right edge outside right edge
					if (y2 < x2)
						area *= (double)( y2 - y1 );
					else
						area *= (double)( x2 - y1 );

					continue;
				}
			}
			// right edge inside left edge
			else if (x1 < y2)
			{
				// right edge outside right edge
				if (x2 < y2)
					area *= (double)( x2 - x1 );
				else
					area *= (double)( y2 - x1 );

				continue;
			}

			// if we get here, there is no overlap
			return false;
		}

		return true;
	}
};


/////////////////////////////////////////////////////////////////////////////////
//class Envelope2d{
//public:
//	double minx,maxx;
//	double miny,maxy;
//public:
//	Envelope2d(){
//		miny=minx=DBL_MAX;	
//		maxy=maxx=DBL_MIN;
//	}
//	Envelope2d( double x1, double x2, double y1, double y2){
//		minx=x1;
//		miny=y1;
//		maxy=y2;
//		maxx=x2;
//	}
//	Envelope2d(const Envelope2d & e){
//		minx=e.minx;
//		miny=e.miny;
//		maxy=e.maxy;
//		maxx=e.maxx;
//	}
//	Envelope2d operator = (const Envelope2d & e){
//		minx=e.minx;
//		miny=e.miny;
//		maxy=e.maxy;
//		maxx=e.maxx;
//		return *this;
//	}
//public:
//	inline void moveTo(double x, double y){
//		double w = maxx-minx;
//		minx = x-w/2;
//		maxx = x+w/2;
//		w = maxy-miny;
//		miny = y-w/2;
//		maxy = y+w/2;
//	}
//	inline void moveTo(Vertex2d & v){
//		moveTo(v.x,v.y);
//	}
//	inline Vertex2d center(){
//		Vertex2d v;
//		v.x=(minx+maxx)/2;
//		v.y=(miny+maxy)/2;
//		return v;
//	}
//	inline Vertex2d getMin() {
//		Vertex2d v;
//		v.x=minx;
//		v.y=miny;  
//		return v;
//	}
//	inline Vertex2d getMax() {
//		Vertex2d v;
//		v.x=maxx;
//		v.y=maxy;  
//		return v;
//	}
//	inline void get(double v[4]){
//		v[0]=minx;		v[1]=maxx;
//		v[2]=miny;		v[3]=maxy;
//	}
//	inline void set(double v[4]){
//		minx=v[0];		maxx=v[1];
//		miny=v[2];		maxy=v[3];
//	}
//	inline void get(double minv[2],double maxv[2]){
//		minv[0]=minx;minv[1]=miny;
//		maxv[0]=maxx;maxv[1]=maxy;
//	}
//	inline void set(double minv[2],double maxv[2]){
//		minx=minv[0];miny=minv[1];
//		maxx=maxv[0];maxy=maxv[1];
//	}	
//	inline void set( double x1, double x2, double y1, double y2) {minx=x1;maxx=x2;miny=y1,maxy=y2;}
//	inline double lengthX()const{return maxx-minx;}
//	inline double lengthY()const {return maxy-miny;}
//	/*忽略v.z判断点是否在包围盒内0-表示在外面，1-表示在里面，2-表示在边上
//	*/
//	inline int test (const Vertex3d & v)const
//	{
//		if(v.x>maxx || v.x<minx || v.y>maxy || v.y<miny)
//			return 0;
//		else
//			return 1;
//	}
//	/*判断点是否在包围盒内0-表示在外面，1-表示在里面，2-表示在边上
//	*/
//	inline int test (const Vertex2d & v)const 
//	{
//		if(v.x>maxx || v.x<minx || v.y>maxy || v.y<miny)
//			return 0;
//		else
//			return 1;
//	}
//	/*判断点是否在包围盒内0-表示在外面，1-表示在里面，2-表示在边上
//	*/
//	inline int test (const double & x, const double  & y)const 
//	{
//		if(x>maxx || x<minx || y>maxy || y<miny)
//			return 0;
//		else
//			return 1;
//	}
//
//public:
//
//	inline void extend(const Vertex2d & v){
//		if(valid()){
//			if(minx>v.x) minx = v.x;
//			if(miny>v.y) miny = v.y;
//
//			if(maxx<v.x) maxx = v.x;
//			if(maxy<v.y) maxy = v.y;
//		}
//		else{
//			minx = v.x - TOLERANCE*2;
//			miny = v.y - TOLERANCE*2;
//
//			maxx = v.x + TOLERANCE*2;
//			maxy = v.y + TOLERANCE*2;
//		}
//	}
//	inline void extend(const Envelope2d & v){
//		if(valid() && v.valid() ){
//			if(minx>v.minx) minx = v.minx;
//			if(miny>v.miny) miny = v.miny;
//
//			if(maxx<v.maxx) maxx = v.maxx;
//			if(maxy<v.maxy) maxy = v.maxy;
//			return ;
//		}
//		if(v.valid()){
//			minx = v.minx;
//			miny = v.miny;
//
//			maxx = v.maxx;
//			maxy = v.maxy;
//		
//			return ;
//		}
//	}
//
//	inline void extend(const double v[4]){
//		if(valid()){
//			if(minx>v[0]) minx = v[0];
//			if(miny>v[2]) miny = v[2];
//		
//			if(maxx<v[1]) maxx = v[1];
//			if(maxy<v[3]) maxy = v[3];
//		
//			return ;
//		}
//		else{
//			minx = v[0];
//			miny = v[2];
//		 
//			maxx = v[1];
//			maxy = v[3];
//		 
//			return ;
//		}
//	}
//
//	inline bool valid( ) const{
//
//		if(minx>=maxx || miny>=maxy )
//			return false;
//		else
//			return true;
//
//	}
//	
//	/*
//	* 0-表示两个包围盒相离
//	* 1-表示两个包围盒a和b相交
//	* 2-表示包围盒a在b中
//	* 3-表示包围盒b在a中
//	*/
//	static int test(const Envelope2d & a, const Envelope2d & b){
//		if(a.minx>b.maxx || a.maxx<b.minx|| a.miny>b.maxy || a.maxy<b.miny)
//			return 0;
//		if((a.minx>=b.minx && a.minx<=b.maxx)&&((a.maxx>=b.minx && a.maxx<=b.maxx))
//			&&(a.miny>=b.miny && a.miny<=b.maxy)&&((a.maxy>=b.miny && a.maxy<=b.maxy))){
//				return 2;
//		}
//		if((b.minx>=a.minx && b.minx<=a.maxx)&&((b.maxx>=a.minx && b.maxx<=a.maxx))
//			&&(b.miny>=a.miny && b.miny<=a.maxy)&&((b.maxy>=a.miny && b.maxy<=a.maxy))){
//				return 3;
//		}
//	
//		return 1;
//	}
//};


typedef Envelope<double,3,char,std::char_traits<char>,std::allocator<char> > Envelope3d;
typedef Envelope<float,3,char,std::char_traits<char>,std::allocator<char> > Envelope3f;
typedef Envelope<int,3,char,std::char_traits<char>,std::allocator<char> > Envelope3i;
typedef Envelope<double,4,char,std::char_traits<char>,std::allocator<char> > Envelope4d;
typedef Envelope<float,4,char,std::char_traits<char>,std::allocator<char> > Envelope4f;
typedef Envelope<int,4,char,std::char_traits<char>,std::allocator<char> > Envelope4i;
end_gtl_namespace
#include "envelope.inl"
