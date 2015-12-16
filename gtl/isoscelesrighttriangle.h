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

#include "vertex.h" 
begin_gtl_namespace
template<
	typename ELEMENT=char,
	typename TRAITS=std::char_traits<ELEMENT>,
	typename ALLOC=std::allocator<ELEMENT> 
>
class IsoscelesRightTriangle :public Object<ELEMENT,TRAITS,ALLOC>
{
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "IsoscelesRightTriangle";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
protected:
	Identifier64 _identifier;
	Vertex3d  _points[3];//0-直角顶点，1-直角顺时针的第1个点，2-1-直角顺时针的第2个点
public:
	IsoscelesRightTriangle()
	{
		_identifier = Identifier64::generate();
	}

	IsoscelesRightTriangle(const Vertex3d & point0, const Vertex3d & point1,const Vertex3d & point2 )
	{
		_points[0]=point0;
		_points[1]=point1;
		_points[2]=point2;
		_identifier = Identifier64::generate();
	}

	IsoscelesRightTriangle(const IsoscelesRightTriangle& irt)
	{
		_identifier=irt._identifier;
		for(int i=0;i<3;i++)
			_points[i]=irt._points[i];//0-直角顶点，1-直角顺时针的第1个点，2-1-直角顺时针的第2个点
	}
	
	virtual ~IsoscelesRightTriangle(void)
	{
	}

public:
	inline void set(const Vertex3d & point0, const Vertex3d & point1,const Vertex3d & point2)
	{
		_points[0]=point0;
		_points[1]=point1;
		_points[2]=point2;
	}

	inline void decompose(	IsoscelesRightTriangle & t1, //包含点p1的三角形
		IsoscelesRightTriangle & t2)//包含点p1的三角形
	{
		t1._points[0] = (_points[2]+_points[1])/2.0;
		t2._points[0] = t1._points[0];

		t1._points[1] = _points[0];
		t1._points[2] = _points[1];

		t2._points[1] = _points[2];
		t2._points[2] = _points[0];
	}

	
	//function PointInTriangle(p, a,b,c)
	//    if SameSide(p,a, b,c) and SameSide(p,b, a,c)
	//        and SameSide(p,c, a,b) then return true
	//    else return false
	//
	inline int test(const Vertex3d & p)const //判断点p与三角形的关系，0-表示在三角形外，1-表示在三角形内，2-表示在三角形的边上
	{
		if(sameSide(p,_points[0],_points[1],_points[2]) && 
			sameSide(p,_points[1],_points[0],_points[2])&&
			sameSide(p,_points[2],_points[0],_points[1]) )
			return 1;
		else
			return 0;
	}

	inline bool equals(const IsoscelesRightTriangle& irt)//判断是否相等
	{
		return irt._identifier==_identifier;
	}


	inline Vertex3d & get(int i) {return _points[i];}
	inline const Vertex3d & get(int i) const {return _points[i];}
	inline void subtriangle(unsigned char index/*0-left,1-right*/,IsoscelesRightTriangle & t)
	{
		if(index==0){
			t._points[0] = (_points[2]+_points[1])/2.0;
			t._points[1] = _points[0];
			t._points[2] = _points[1];
		}
		else{
			t._points[0] = (_points[2]+_points[1])/2.0;
			t._points[1] = _points[2];
			t._points[2] = _points[0];
		}		
	}
public:
	//function SameSide(p1,p2, a,b)
	//    cp1 = CrossProduct(b-a, p1-a)
	//    cp2 = CrossProduct(b-a, p2-a)
	//    if DotProduct(cp1, cp2) >= 0 then return true
	//    else return false
	//	
	static inline bool sameSide(const Vertex3d & p1,const Vertex3d & p2, const Vertex3d & a, const Vertex3d & b )
	{
		Vertex3d cp1 = (b-a)^(p1-a); 
		Vertex3d cp2 = (b-a)^(p2-a); 
		if(cp1*cp2>0)
			return true;
		else
			return false;
	}
	
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////

end_gtl_namespace 
