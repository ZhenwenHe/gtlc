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
#include "assert.h"
#include <cmath>
#include "vertex2f.h"
#include "vertex2d.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace 

/** @} */
/** @addtogroup vertex Vertex3d 坐标点的结构体定义
*  @{
*/
class CORE_API Vertex3d{
public:
	double x, y, z;
	Vertex3d(double xx, double yy, double zz);
	Vertex3d();
	inline void operator*=(double rhd);
	

	Vertex3d operator = (const Vertex3d & c);

	/** 内联函数。重载二元操作符"^="，用于对向量的复合赋值（叉乘）
	* @param [in] vt3d	当前用于复合赋值的向量
	* @return
	*/
	inline void operator^=(const Vertex3d& vt3d);

	/** 内联函数。重载二元操作符"+="，用于对向量的复合赋值（加法）。
	* @param [in] vt3d	当前用于复合赋值的向量
	* @return
	*/
	inline void operator+=(const Vertex3d& vt3d);

	/** 内联函数。重载二元操作符"-="，用于对向量的复合赋值（减法）
	* @param [in] vt3d	当前用于复合赋值的向量
	* @return
	*/
	inline void operator-=(const Vertex3d& vt3d);
	/** 内联函数。重载二元操作符"/="，用于对向量的复合赋值（点乘）。
	* @param [in] rhd	当前用于复合赋值的除数
	* @return
	*/
	void operator/=(double rhd);

	/** 内联函数。重载一元操作符"-"，用于取得向量或坐标的相反值。
	* @param
	* @return 向量或坐标的相反值
	*/
	inline Vertex3d operator-() const;

	/** Length of the vector = sqrt( vec . vec ) */
	inline double length() const;

	/** Length squared of the vector = vec . vec */
	inline double length2() const;

	/*distance to another vertex*/
	inline double distance(const Vertex3d & v)const ;

	/** 对向量置空
	* @param
	* @return
	*/
	inline void zero()
	{
		x = y = z = 0.0;
	}
	/** Normalize the vector so that it has length unity.
	* Returns the previous length of the vector.
	* If the vector is zero length, it is left unchanged and zero is returned.
	*/
	inline double normalize();
	inline double  dotProduct(const Vertex3d& vec) const;
	inline Vertex3d  crossProduct(const Vertex3d& rkVector) const;
	/*               this
	                /|
	               / |
	              /  |  Heigh
                 /   |
				0-----vVec
				  Lenght
	*/
	inline double getProjectLength(Vertex3d& vVec) const;
	inline double getProjectHeight(Vertex3d& vVec) const;
};
CORE_API const Vertex3d operator*(const Vertex3d& lhv, double rhd);
CORE_API const Vertex3d operator*(double lhd, const Vertex3d& rhv);
CORE_API double operator*(const Vertex3d& lhv, const Vertex3d& rhv);
CORE_API Vertex3d operator-(const Vertex3d & c1, const Vertex3d & c2);
CORE_API Vertex3d operator+(const Vertex3d & c1, const Vertex3d & c2);
CORE_API bool operator==(const Vertex3d& lhv, const Vertex3d& rhv);
CORE_API bool operator!=(const Vertex3d& lhv, const Vertex3d& rhv);
CORE_API const Vertex3d operator /(const Vertex3d& lhv, double rhd);
CORE_API const Vertex3d operator^(const Vertex3d& lhv, const Vertex3d& rhv);
typedef Vertex3d Vertex;
typedef Vertex3d& Vertex3dRef;
typedef vector<Vertex3d> Vertex3dVector;
typedef std::vector<Vertex3d> Vertex3dList; 
typedef double Vector3d[3]; 


inline void Vertex3d::operator*=(double rhd)
{
	x *= rhd;
	y *= rhd;
	z *= rhd;
}

/*distance to another vertex*/
inline double Vertex3d::distance(const Vertex3d & v) const 
{
	return (*this - v).length();
}

/** 内联函数。重载二元操作符"^="，用于对向量的复合赋值（叉乘）
* @param [in] vt3d	当前用于复合赋值的向量
* @return
*/
inline void Vertex3d::operator^=(const Vertex3d& vt3d)
{
	double dTempX, dTempY, dTempZ;
	dTempX = y * vt3d.z - z * vt3d.y;
	dTempY = z * vt3d.x - x * vt3d.z;
	dTempZ = x * vt3d.y - y * vt3d.x;
	x = dTempX; y = dTempY; z = dTempZ;
}
/** 内联函数。重载二元操作符"+="，用于对向量的复合赋值（加法）。
* @param [in] vt3d	当前用于复合赋值的向量
* @return
*/
inline void Vertex3d::operator+=(const Vertex3d& vt3d)
{
	x += vt3d.x;
	y += vt3d.y;
	z += vt3d.z;
}

/** 内联函数。重载二元操作符"-="，用于对向量的复合赋值（减法）
* @param [in] vt3d	当前用于复合赋值的向量
* @return
*/
inline void Vertex3d::operator-=(const Vertex3d& vt3d)
{
	x -= vt3d.x;
	y -= vt3d.y;
	z -= vt3d.z;
}

/** 内联函数。重载一元操作符"-"，用于取得向量或坐标的相反值。
* @param
* @return 向量或坐标的相反值
*/
inline Vertex3d Vertex3d::operator-() const
{
	return Vertex3d(-x, -y, -z);
}

/** Length of the vector = sqrt( vec . vec ) */
inline double Vertex3d::length() const  {
	return sqrt(x*x + y*y + z*z);
}

/** Length squared of the vector = vec . vec */
inline double Vertex3d::length2() const {
	return x*x + y*y + z*z;
}

/** Normalize the vector so that it has length unity.
* Returns the previous length of the vector.
* If the vector is zero length, it is left unchanged and zero is returned.
*/
inline double Vertex3d::normalize() {
	double norm = length();
	if (norm>0.0) {
		double inv = 1.0 / norm;
		x *= inv;
		y *= inv;
		z *= inv;
	}
	return(norm);
}
inline double  Vertex3d::dotProduct(const Vertex3d& vec) const
{
	return x * vec.x + y * vec.y + z * vec.z;
}
inline Vertex3d  Vertex3d::crossProduct(const Vertex3d& rkVector) const
{
	Vertex3d kCross;
	kCross.x = y * rkVector.z - z * rkVector.y;
	kCross.y = z * rkVector.x - x * rkVector.z;
	kCross.z = x * rkVector.y - y * rkVector.x;
	return kCross;
}

// 在vVec上的投影长度
inline double Vertex3d::getProjectLength(Vertex3d& vVec) const
{
	// 在vVec上的投影长度
	if (vVec == Vertex3d(0, 0, 0))
		return 0;
	return ((*this) *vVec) / vVec.length();
}
// 在vVec上的投影Height度
inline double Vertex3d::getProjectHeight(Vertex3d& vVec) const
{
	double dHeightEx = 0.0;
	if (vVec == Vertex3d(0, 0, 0))
		dHeightEx = length2();
	else
	{
		double crossResult = (*this) * vVec;
		dHeightEx = length2() - crossResult * crossResult / vVec.length2();
	}
	return sqrt(dHeightEx);
}

//////////////////////////////////////////////////////////////////////////////

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace

