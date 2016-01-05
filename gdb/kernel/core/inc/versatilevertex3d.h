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
#include "config.h" 
#include "vertex2d.h"
#include "vertex3d.h"
#include "vertex4d.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace 

class VersatileVertex3d : public Vertex3d
{ 
public:
	 
	/** 法线 */
	double nx, ny, nz;

	/** 纹理坐标 */
	double u, v;

	/** 颜色 */
	float cR, cG, cB;
public:

	/** 构造函数1
	*/
	VersatileVertex3d(double fx = 0.0, double fy = 0.0, double fz = 0.0, double fnx = 0.0, double fny = 0.0, double fnz = 0.0, double fu = 0.0, double fv = 0.0, float fcr = 0.0, float fcg = 0.0, float fcb = 0.0) :
		Vertex3d(fx, fy, fz), nx(fnx), ny(fny), nz(fnz), u(fu), v(fv), cR(fcr), cG(fcg), cB(fcb)
	{

	}

	/** 构造函数2
	*/
	VersatileVertex3d(Vertex3d& vPos, Vertex3d& vNor, double fu, double fv, float fcr, float fcg, float fcb) :
		Vertex3d(vPos), nx(vNor.x), ny(vNor.y), nz(vNor.z), u(fu), v(fv), cR(fcr), cG(fcg), cB(fcb)
	{

	}

	/** 构造函数3
	*/
	VersatileVertex3d(Vertex3d& vPos, Vertex3d& vNor, Vertex2d& vTex, Vertex3f& vClr) :
		Vertex3d(vPos), nx(vNor.x), ny(vNor.y), nz(vNor.z), u(vTex.x), v(vTex.y), cR(vClr.x), cG(vClr.y), cB(vClr.z)
	{

	}


	/** 获得顶点法向量
	* @param
	* @return 顶点法向量
	*/
	Vertex3d getNormal() const
	{
		return Vertex3d(nx, ny, nz);
	}

	/** 设置顶点法向量
	* @param [in]  vNor 顶点法向量
	* @return
	*/
	void setNormal(const Vertex3d& vNor)
	{
		nx = vNor.x;
		ny = vNor.y;
		nz = vNor.z;
	}

	/** 获得顶点纹理坐标
	* @param
	* @return 顶点纹理坐标
	*/
	Vertex2d getTexCoor() const
	{
		return Vertex2d(u, v);
	}

	/** 设置顶点纹理坐标
	* @param [in] vTex 顶点纹理坐标
	* @return
	*/
	void setTexCoor(const Vertex2d& vTex)
	{
		u = vTex.x;
		v = vTex.y;
	}

	/** 获得顶点颜色
	* @param
	* @return 顶点颜色
	*/
	Vertex3f getColor() const
	{
		return Vertex3f(cR, cG, cB);
	}


	/** 设置顶点颜色
	* @param [in] vClr	 顶点颜色
	* @return
	*/
	void setColor(const Vertex3f& vClr)
	{
		cR = vClr.x;
		cG = vClr.y;
		cB = vClr.z;
	} 
};
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
