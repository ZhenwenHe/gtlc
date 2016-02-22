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
#include "internal_linearring3dimpl.h"
#include "internal_trianglemesh3dimpl.h"


begin_gtl_namespace
begin_gdb_namespace
//实现二维三角化的功能,已计算包围盒，需要自行维护 TriangleMesh3dImpl* mesh

/** 根据同一平面上的三维离散点Vertex3d数据构建二维三角网
* @param [in] bIsClose -- 输入的特征边是否封闭
* @param [in] normal -- 指定平面的法向，传入NULL则自动计算
* @param [in] featurelines -- 输入的特征边（顶点索引），没有则传入NULL
* @param [in] holemarks -- 输入内环中的种子点用于消除内环，没有则传入NULL
* @param [in] verts -- 输入的二维离散点
* @param [in out] mesh -- 输出的三角网(逆时针排列)
* @return true -- 构建成功; false -- 构建错误
*/
bool TriangulationFrmVerts(bool bIsClose, Vertex3d* normal, vector<int>* featurelines, vector<Vertex3d>* holemarks, vector<Vertex3d> &verts, TriangleMesh3dImpl* mesh);

/** 根据同一平面上的三维离散点VersatileVertex3d数据构建二维三角网
* @param [in] bIsClose -- 输入的特征边是否封闭
* @param [in] normal -- 指定平面的法向，传入NULL则自动计算
* @param [in] featurelines -- 输入的特征边（顶点索引），没有则传入NULL
* @param [in] holemarks -- 输入内环中的种子点用于消除内环，没有则传入NULL
* @param [in] verts -- 输入的二维离散点
* @param [in] m_iVertexType -- 输出点类型
* @param [in out] mesh -- 输出的三角网(逆时针排列)
* @return true -- 构建成功; false -- 构建错误
*/
bool TriangulationFrmVertsEx(bool bIsClose, Vertex3d* normal, vector<int>* featurelines, vector<Vertex3d>* holemarks, vector<VersatileVertex3d> &verts, unsigned char m_iVertexType, TriangleMesh3dImpl* mesh);

/** 根据三维线性环构建带有约束的二维三角网
* @param [in] bIsClose -- true
* @param [in] holemarks -- 输入内环中的种子点用于消除内环，没有则传入NULL
* @param [in] rings -- 输入的二维线性环
* @param [in] iVertexType -- 输出点类型
* @param [in out] mesh -- 输出的三角网(逆时针排列)
* @param [in] normal -- 环的法线
* @return true -- 构建成功; false -- 构建错误
*/
bool TriangulationFrmRings(bool bIsClose, vector<Vertex3d> *holemarks, vector<LinearRing3dImpl*>& rings, unsigned char iVertexType, TriangleMesh3dImpl* mesh, Vertex3d* normal = NULL);

/** 根据单个线性环快速剖分为三角网
* @param [in] ring -- 输入的线性环
* @param [in] iVertexType -- 输出点类型
* @param [in out] mesh -- 输出的三角网(逆时针排列)
* @param [in] normal -- 环的法线
* @return true -- 构建成功; false -- 构建错误
*/
bool FastTriangulationFrmSimpleRing(LinearRing3dImpl* ring, unsigned char iVertexType, TriangleMesh3dImpl*& mesh, Vertex3d* normal = NULL);

/** 将单个线性环根据TOL进行冗余性检查
* @param [in] ring -- 输入的线性环
* @param [in] TOL -- 冗余阈值
* @return
*/
void eliminateRedundancy(LinearRing3dImpl* ring, double TOL = MathEx::TOL);

end_gdb_namespace
end_gtl_namespace


