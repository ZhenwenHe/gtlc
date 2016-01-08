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
#include "vertex2f.h"
#include "vertex3f.h" 

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace 

class VersatileVertex3f : public Vertex3f
{
	
public:
	/** 法线 */
	float nx, ny, nz;

	/** 纹理坐标 */
	float u, v;

	/** 颜色 */
	float cR, cG, cB;

public:

	/** 构造函数1
	*/
	VersatileVertex3f(float fx = 0.0, float fy = 0.0, float fz = 0.0, float fnx = 0.0, float fny = 0.0, float fnz = 0.0, float fu = 0.0, float fv = 0.0, float fcr = 0.0, float fcg = 0.0, float fcb = 0.0) 
		:Vertex3f(fx, fy, fz), nx(fnx), ny(fny), nz(fnz), u(fu), v(fv), cR(fcr), cG(fcg), cB(fcb)
	{

	}

	/** 构造函数2
	*/
	VersatileVertex3f(Vertex3f& vPos, Vertex3f& vNor, float fu, float fv, float fcr, float fcg, float fcb) :
		Vertex3f(vPos), nx(vNor.x), ny(vNor.y), nz(vNor.z), u(fu), v(fv), cR(fcr), cG(fcg), cB(fcb)
	{

	}

	/** 构造函数3
	*/
	VersatileVertex3f(Vertex3f& vPos, Vertex3f& vNor, Vertex2f& vTex, Vertex3f& vClr) :
		Vertex3f(vPos), nx(vNor.x), ny(vNor.y), nz(vNor.z), u(vTex.x), v(vTex.y), cR(vClr.x), cG(vClr.y), cB(vClr.z)
	{

	}

  
	/** 获得顶点法向量
	* @param
	* @return 顶点法向量
	*/
	Vertex3f getNormal() const
	{
		return Vertex3f(nx, ny, nz);
	}

	/** 设置顶点法向量
	* @param [in]  vNor 顶点法向量
	* @return
	*/
	void setNormal(const Vertex3f& vNor)
	{
		nx = vNor.x;
		ny = vNor.y;
		nz = vNor.z;
	}

	/** 获得顶点纹理坐标
	* @param
	* @return 顶点纹理坐标
	*/
	Vertex2f getTexCoor() const
	{
		return Vertex2f(u, v);
	}

	/** 设置顶点纹理坐标
	* @param [in] vTex 顶点纹理坐标
	* @return
	*/
	void setTexCoor(const Vertex2f& vTex)
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
