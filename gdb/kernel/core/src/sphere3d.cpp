#include "config.h"
#include "sphere3d.h"
#include <limits>


begin_gtl_namespace
begin_gdb_namespace
Sphere3d::Sphere3d()
{
	_center.x=0;
	_center.y=0;
	_center.z=0;
	_radius=0;
}
Sphere3d::Sphere3d(const Vertex3d & c, double r)
{
	_center=c;
	_radius=r;
}
Sphere3d::Sphere3d(const Sphere3d & s)
{
	_center=s._center;
	_radius=s._radius;
}
Sphere3d Sphere3d::operator = (const Sphere3d & s)
{
	_center=s._center;
	_radius=s._radius;
	return * this;
}
Sphere3d Sphere3d::operator+=(const Vertex3d &c)
{
	_center=_center+c;
	return *this;
}
Sphere3d Sphere3d::operator-=(const Vertex3d &c)
{
	_center=_center-c;
	return *this;
}

/*
* 0-表示两个球相离
* 1-表示两个球相切
* 2-表示两个球相交
* 3-表示球a在b中
* 4-表示球b在a中
*/
int Sphere3d::test(const Sphere3d&  a, const Sphere3d&  b)
{
	double r = (a._center-b._center).length();
	if(r<a._radius+b._radius)//两球相交
	{
		if(r<a._radius)
			return 4;
		if(r<b._radius)
			return 3;

		return 1;
	}
	else //两球不相交或相切
	{
		if(fabs(r-a._radius-b._radius)<TOLERANCE)
			return  2;
		else
			return 0;
	}
}
/*
* 0-表示点在球外
* 1-表示点在球内
* 2-表示点在球上
*/
int Sphere3d::test(const Sphere3d&  a,const Vertex3d &  v)
{
	double r = (a._center-v).length();
	if(r<a._radius)//表示点在球内
	{
		return 1;
	}
	else 
	{
		if(fabs(r-a._radius)<TOLERANCE)
			return  2;
		else
			return 0;
	}
}

Sphere3d& Sphere3d::extend(const Sphere3d&   sp)
{
	/*
	* 0-表示两个球相离
	* 1-表示两个球相切
	* 2-表示两个球相交
	* 3-表示球a在b中
	* 4-表示球b在a中
	*/
	switch(Sphere3d::test(*this,sp))
	{
	case 0:
	case 1:
	case 2:
		{
			double r = (_center-sp._center).length();
			_radius = (sp._radius+_radius+r)/2;
			_center.x=(_center.x+sp._center.x)/2;
			_center.y=(_center.y+sp._center.y)/2;
			_center.z=(_center.z+sp._center.z)/2;
			break;
		}
	case 3:
		{
			_radius = sp._radius;
			_center = sp._center;
			break;
		}
	case 4:
		{
			break;
		}
	}
	return *this;
}
Sphere3d& Sphere3d::extend(const Vertex3d&    v)
{
	/*
	* 0-表示点在球外
	* 1-表示点在球内
	* 2-表示点在球上
	*/
	switch(Sphere3d::test(*this,v))
	{
	case 0:
		{
			_radius = (_center-v).length();
			break;
		}
	case 1:
	case 2:
		{
			break;
		}
	}
	return *this;
}
Sphere3d& Sphere3d::extend(const Envelope3d& sp)
{
	Vertex3d v;
	for(int i=0;i<8;i++)
	{
		sp.getCorner(i,v);
		extend(v);
	}
	return *this;
}
end_gdb_namespace
end_gtl_namespace


