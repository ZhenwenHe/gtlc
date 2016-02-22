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
#include "quaternion.h"
#include "matrix4x4.h"
#include "mathex.h"
#include "internal_frustumimpl.h"
#include "internal_rayimpl.h"
#include "internal_aabboximpl.h"
#include <vector>
#include <algorithm>
#include <limits>
#include <deque>
#include <set>
using namespace  std;


begin_gtl_namespace
begin_gdb_namespace

/***********************************************/
// 通常为Get系列或者Ensure系列函数
// 注意求平面函数，判断其返回值是否为0
/***********************************************/

/** 求通过三点的平面
* @param [in] p1,p2,p3	空间三点
* @param [out] plane	所求平面
* @return 1 - 三点不共线；0 - 三点共线
*/
 int GetPlaneEquation(Vertex3d& p1, Vertex3d &p2, Vertex3d &p3, PLANE3D& plane);

/** 求已知法向量和通过一点的平面
* @param [in] vNormal	已知法向量
* @param [in] pt	通过一点
* @param [out] plane	所求平面
* @return 1 - 三点不共线；0 - 三点共线
*/
 int GetPlaneEquation(Vertex3d &vNormal, Vertex3d &pt, PLANE3D& plane);

/** 求通过三点的平面,并且法向量会被归一化
* @param [in] p1,p2,p3	空间三点
* @param [out] plane	所求平面
* @return 1 - 三点不共线；0 - 三点共线
*/
 int GetPlaneEquationEx(Vertex3d &p1, Vertex3d &p2, Vertex3d &p3, PLANE3D& plane);

/** 求已知平面的法向量
* @param [in] plane	已知平面
* @param [out] vNormal	所求法向量
* @return
*/
 void GetPlaneNormal(PLANE3D& plane, Vertex3d& vNormal);

//功能：求在一点另一侧的平面的法向量
//参数：plane为平面，pt为一点，vNormal为所求法向量

 void GetPlaneNormalEx(PLANE3D& plane, Vertex3d& pt, Vertex3d& vNormal);
/** 求已知平面上的任意一点（使用前需要提供随机种子）
* @param [in] plane	已知平面
* @param [out] pt	所求一点
* @return
*/
 void GetOnePointInPlane(PLANE3D& plane, Vertex3d& pt);

/** 求已知平面上的区别于某点的任意一点（使用前需要提供随机种子）
* @param [in] plane	已知平面
* @param [out] pt	所求一点
* @param [in] Diffpt 区别点
* @return
*/
 void GetOnePointInPlane(PLANE3D& plane, Vertex3d& pt, Vertex3d& Diffpt);

/** 求已知长度的任意射线向量（使用前需要提供随机种子）
* @param [out] ray	所求射线向量
* @param [in] radius	已知向量长度
* @return
*/
 void GetRandomRay(Vertex3d& ray, double radius);

/** 求某平面内已知长度的任意射线向量（使用前需要提供随机种子）
* @param [out] ray	所求射线向量
* @param [in] radius	已知向量长度
* @param [in] plane	已知平面
* @return
*/
 void GetRandomRayInPlane(Vertex3d& ray, double radius, PLANE3D& plane);

/** 获取一组逆时针排序且非自相交共面点集的法向量
* @param [in] ptArray	已知点集
* @param [out] vNormal	所求法向量
* @return 已知平面上第一个不共线点的索引号（-1 - 所有点共线 或点集不共面）
*/
 int GetPointsNormal(vector<Vertex3d>& ptArray, Vertex3d& vNormal);

/** 获取一组逆时针排序且非自相交点集的法向量,允许点集不共面
* @param [in] ptArray	已知点集
* @param [out] vNormal	所求法向量
* @return 已知平面上第一个不共线点的索引号（-1 - 所有点共线）
*/
 int GetPointsNormalEx(vector<Vertex3d>& ptArray, Vertex3d& vNormal);

/** 增量法求点集凸壳(2D),ydcl
* @param [in] ptArray	已知点集
* @param [out] CHArray	所求凸壳
* @return
*/
 void GetConvexHull(vector<Vertex3d>& ptArray, vector<Vertex3d>& CHArray); // xx_add_0619
/** 确保点集封闭
* @param [in] ptArray	已知点集
* @return
*/
 void EnsureClose(vector<Vertex3d>& ptArray);

/** 确保点集开放
* @param [in] ptArray	已知点集
* @return
*/
 void EnsureOpen(vector<Vertex3d>& ptArray);

 int EnsureAntiClock(vector<Vertex3d>& Point3dArray, Vertex3d &vDirection);
/** 从世界坐标系（WCS），转换到局部坐标（LCS）下
* @param [in] CPoint3d &pt : 需要转换的点坐标
* @param [in] CPoint3d& origin : LCS坐标系的原点在WCS坐标系中的值
* @param [in] CPoint3d& vx,CPoint3d& vy,CPoint3d& vz : LCS坐标系的轴在WCS坐标系中的值
* @return
*/
 inline void ChangeXYZAxis(Vertex3d &pt, Vertex3d& origin, Vertex3d& vx, Vertex3d& vy, Vertex3d& vz)
{
	pt -= origin;
	Vertex3d temp;
	temp.x = pt * vx;
	temp.y = pt * vy;
	temp.z = pt * vz;
	pt = temp;
}

/** 从局部坐标（LCS），转换到世界坐标系（WCS）下
* @param [in] CPoint3d &pt : 需要转换的点坐标
* @param [in] CPoint3d& origin : LCS坐标系的原点在WCS坐标系中的值
* @param [in] CPoint3d& vx,CPoint3d& vy,CPoint3d& vz : LCS坐标系的轴在WCS坐标系中的值
* @return
*/
 inline void ChangeXYZAxisReverse(Vertex3d &pt, Vertex3d& origin, Vertex3d& vx, Vertex3d& vy, Vertex3d& vz)
{
	pt = vx * pt.x + vy * pt.y + vz * pt.z + origin;
}
//功能：求过给定直线与一平面垂直的平面方程
//参数：pt1,pt2为直线的两个端点，plane1为平面1，返回垂直平面plane2
 void GetPlaneEquationByVerticalPlane(Vertex3d& pt1, Vertex3d& pt2, PLANE3D& plane1, PLANE3D& plane2);

//功能：求过给定点与一平面平行的平面方程
//参数：pt为端点，plane1为平面1，返回平行平面plane2
 void GetPlaneEquationByParallelPlane(Vertex3d& pt, PLANE3D& plane1, PLANE3D& plane2);

//功能：求过给定直线与另一直线平行的平面方程
//参数：pt1,pt2,pt3,pt4为两条直线的4个端点，plane1为平面1，返回平行平面plane2
 void GetPlaneEquationByParallelLine(Vertex3d& pt1, Vertex3d& pt2, Vertex3d& pt3, Vertex3d& pt4, PLANE3D& plane);

// 功能：检查平面是否有效（想法来自于检查四维矩阵是否为变换矩阵）
 int IsPlaneValid(PLANE3D& plane);
 void EnsureNoRedunant(vector<Vertex3d>& pointArray);

/** 根据制定的精度消除顶点序列中相邻点的冗余
* @param [in] pointArray : 需要检查的顶点序列
* @param [in] dTOL : 检查冗余时采用的绝对误差
* @return
*/
 void EnsureNoRedunantWithTOL(vector<Vertex3d>& pointArray, double dTOL);

/***********************************************/
// 对于后缀名为Ex的函数，在求距离系列函数中，即D系列函数中，都表示求得距离平方值，没有其他扩展含义
// 所有求距离函数都提供对应的Ex函数
/***********************************************/

/** 求已知空间两点的距离
* @param [in] p1,p2  已知空间两点
* @return  所求距离
*/
 double DPointToPoint(Vertex3d& pt1, Vertex3d& pt2);

/** 求已知空间两点的距离的平方值
* @param [in] p1,p2  已知空间两点
* @return  所求距离的平方值
*/
 double DPointToPointEx(Vertex3d& pt3d, Vertex3d& pOrigin);

/**　求某点到平面的距离值。
@param pt3d  三维空间中的给定点。
@param plane  平面。
@return  返回点构成平面的距离值
*/
 double DPointToPlane(Vertex3d& pt3d, PLANE3D& plane);

/**　求某点到平面的距离的平方值
@param pt3d  三维空间中的给定点
@param plane  平面。
@return  返回点构成平面的距离的平方值
*/
 double DPointToPlaneEx(Vertex3d& pt3d, PLANE3D& plane);


/** 求点到直线段的最短距离
* @param [in] pt3d  已知点
* @param [in] pStartPoint,pEndPoint  已知直线段的端点
* @param [out] resultPoint			 距离连线的另一端点（一端是已知点）
* @return  所求距离
*/
 double DPointToLineSeg(Vertex3d& pt3d, Vertex3d& pStartPoint, Vertex3d& pEndPoint, Vertex3d* pResultPoint = NULL);//xxy//

/** 求点到直线段的最短距离的平方值
* @param [in] pt3d  已知点
* @param [in] pStartPoint,pEndPoint  已知直线段的端点
* @param [out] resultPoint			 距离连线的另一端点（一端是已知点）
* @return  所求距离的平方值
*/
 double DPointToLineSegEx(Vertex3d& pt3d, Vertex3d& pStartPoint, Vertex3d& pEndPoint, Vertex3d* pResultPoint = NULL);//xxy//

/** 求点到三角形的最短距离
* @param [in] pt3d  已知点
* @param [in] triPt1,triPt2,triPt3  已知三角形的三个顶点
* @param [out] resultPoint			 距离连线的另一端点（一端是已知点）
* @return  所求距离
*/
 double DPointToTriangle(Vertex3d& pt3d, Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3, Vertex3d* pResultPoint = NULL);//xxy//

/** 求点到三角形的最短距离的平方值
* @param [in] pt3d  已知点
* @param [in] triPt1,triPt2,triPt3  已知三角形的三个顶点
* @param [out] resultPoint			 距离连线的另一端点（一端是已知点）
* @return  所求距离的平方值
*/
 double DPointToTriangleEx(Vertex3d& pt3d, Vertex3d& planePt1, Vertex3d& planePt2, Vertex3d& planePt3, Vertex3d* pResultPoint = NULL);//xxy//

/** 求线段到三角形的最短距离
* @param [in] pt3d  线段2端点
* @param [in] triPt1,triPt2,triPt3  已知三角形的三个顶点
* @param [out] resultPoint			 距离连线的另一端点（一端是已知点）
* @return  所求距离
*/
 double DLineSegToTriangle(Vertex3d& linePt1, Vertex3d& linePt2, Vertex3d& planePt1, Vertex3d& planePt2, Vertex3d& planePt3, Vertex3d* pPoint1 = NULL, Vertex3d* pPoint2 = NULL);//xxy//

/** 求线段到三角形的最短距离的平方值
* @param [in] pt3d  线段2端点
* @param [in] triPt1,triPt2,triPt3  已知三角形的三个顶点
* @param [out] resultPoint			 距离连线的另一端点（一端是已知点）
* @return  所求距离的平方值
*/
 double DLineSegToTriangleEx(Vertex3d& linePt1, Vertex3d& linePt2, Vertex3d& planePt1, Vertex3d& planePt2, Vertex3d& planePt3, Vertex3d* pPoint1 = NULL, Vertex3d* pPoint2 = NULL);//xxy//
//空间中三角形到三角形的最小距离/平方
 double DTriangleToTriangleEx(Vertex3d& tri1Pt1, Vertex3d& tri1Pt2, Vertex3d& tri1Pt3, Vertex3d& planePt1, Vertex3d& planePt2, Vertex3d& planePt3, Vertex3d* pPoint1 = NULL, Vertex3d* pPoint2 = NULL);
 double DTriangleToTriangle(Vertex3d& tri1Pt1, Vertex3d& tri1Pt2, Vertex3d& tri1Pt3, Vertex3d& planePt1, Vertex3d& planePt2, Vertex3d& planePt3, Vertex3d* pPoint1 = NULL, Vertex3d* pPoint2 = NULL);

/** 求点到多边形的最短距离
* @param [in] pt3d  已知点
* @param [in] vPts  已知多边形的顶点集
* @param [out] resultPoint			 距离连线的另一端点（一端是已知点）
* @return  所求距离
*/
 double DPointToPolygon(Vertex3d& pt3d, vector<Vertex3d>& vPts, Vertex3d* pResultPoint = NULL); //xxy//

/** 求点到多边形的最短距离的平方值
* @param [in] pt3d  已知点
* @param [in] vPts  已知多边形的顶点集
* @param [out] resultPoint			 距离连线的另一端点（一端是已知点）
* @return  所求距离的平方值
*/
 double DPointToPolygonEx(Vertex3d& pt3d, vector<Vertex3d>& vPts, Vertex3d* pResultPoint = NULL); //xxy//

 double DPointToLine(Vertex3d& pt, Vertex3d& LineStr, Vertex3d& LineEnd, Vertex3d* pFootPt = NULL); //xxy//
 double DPointToLineEx(Vertex3d& pt, Vertex3d& LineStr, Vertex3d& LineEnd, Vertex3d* pFootPt = NULL); //xxy//

// 求解空间直线的最短距离（若相交则为0，则为异面则为公垂线距离，若平行则为间距），并求出最短距离点对
 double DLineToLine(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, Vertex3d *pPair1 = NULL, Vertex3d *pPair2 = NULL);


// 求解空间线段的最短距离（若相交则为0，则为异面则为公垂线距离，若平行则为间距），并求出最短距离点对
 double DLineSegToLineSeg(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, Vertex3d *pPair1 = NULL, Vertex3d *pPair2 = NULL);

 double DLineSegToLineSegEx(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, Vertex3d *pPair1 = NULL, Vertex3d *pPair2 = NULL);

 double DLineSegToPolygon(Vertex3d& linePt1, Vertex3d& linePt2, vector<Vertex3d>& ptArray, Vertex3d* pPoint1 = NULL, Vertex3d* pPoint2 = NULL);

 double DLineSegToPolygonEx(Vertex3d& linePt1, Vertex3d& linePt2, vector<Vertex3d>& ptArray, Vertex3d* pPoint1 = NULL, Vertex3d* pPoint2 = NULL);
 double DPointToRay(Vertex3d& pPoint, Vertex3d& pStartPoint, Vertex3d& pEndPoint, Vertex3d* pResultPoint);

 double DPointToRayEx(Vertex3d& pPoint, Vertex3d& pStartPoint, Vertex3d& pEndPoint, Vertex3d* pResultPoint);


 /***********************************************/
 // 空间关系的判断的返回值包括is系列和R系列
 // 只需要知道粗略的判断结果，即只需要简单知道1或者0可以用Is系列，如判断包含关系，函数名有In，Cross，Intersect等
 // 若需要知道精细的判断结果，即比0、1更精细的结果，可以用R系列，返回值都用char表示，减小int返回值空间,函数名多包含With
 // 另外，有些时候不需要知道完整的判断结果，只需要知道某一方面的关系，那么可以从R系列中提取Is系列,采用快速算法提高效率
 /***********************************************/

 /**判断空间四点是否共面,若不共面，则求出第四点在平面上的垂足。
 @param : [IN] p1	给定的三维空间点一（只与p4区分给出顺序）。
 @param : [IN] p2　	给定的三维空间点二（只与p4区分给出顺序）。
 @param : [IN] p3	给定的三维空间点三（只与p4区分给出顺序）。
 @param : [IN] p4	给定的三维空间点四（只与p4区分给出顺序）。
 @param : [OUT] pt	p4在p1、p2、p3确定的一平面上的垂足
 @return : 1 - 共面; 0 - 不共面。
 */
  int IsFourPointsInOnePlane(Vertex3d& p1, Vertex3d& p2, Vertex3d& p3, Vertex3d& p4, Vertex3d* pFootPt = NULL);//xxy//


 /**判断空间一点S是否位于某平面上，若不在平面上，求得其在屏幕上的垂足
 @brief :
 @param : [IN] pt		空间一点
 @param : [IN] plane		平面
 @param : [OUT] footPt	垂足
 @return : 1 - 点在平面上 ; 0 - 点不在平面上
 */
  int IsPointInPlane(Vertex3d& pt, PLANE3D& plane, Vertex3d* pFootPt = NULL);//xxy//

 /**判断点落在三角形的内部（包括边上)，还是外部 。
 @param : [in] pt1  三角形的顶点一（无排序要求）。
 @param : [in] pt2  三角形的顶点二（无排序要求）。
 @param : [in] pt3  三角形的顶点三（无排序要求）。
 @param : [in] result  待判断点。
 @param : [in] bOnPlane  是否点已经在平面上。
 @return:  1  -  点落在三角形内部（包括边上)； 0 -  点落在三角形外部。
 */
  int IsPointInTri(Vertex3d& pt1, Vertex3d& pt2, Vertex3d& pt3, Vertex3d& vPt, int bOnPlane = 1);

 /**判断点落在三角形的内部（包括边上)，还是外部。
 @param : [in] pt1  三角形的顶点一（无排序要求）。
 @param : [in] pt2  三角形的顶点二（无排序要求）。
 @param : [in] pt3  三角形的顶点三（无排序要求）。
 @param : [in] result  待判断点。
 @param : [in] bOnPlane  是否点已经在平面上。
 @return:  'V'  - 点与三角形一个顶点重合; 'E' - 点落在三角形边上； 'F' -  点落在三角形内部; '0' -  点落在三角形外部
 */
  char RPointWithTri(Vertex3d& pt1, Vertex3d& pt2, Vertex3d& pt3, Vertex3d& vPt, bool bOnPlane = true);


 /**判断点在线段上或者线段外
 @param : [in] pt  待判断点
 @param : [in] pt1  线段起点
 @param : [in] pt2  线段终点
 @param : [out] footPt  点在线段上的垂足
 @return:  1  -  点在线段上； 0 -  点在线段外。
 */
  int IsPointInLineSeg(Vertex3d& pt, Vertex3d& pt1, Vertex3d& pt2, Vertex3d* pFootPt = NULL);//xxy//

 /**判断点在直线上或者直线外
 @param : [in] pt  待判断点
 @param : [in] pt1  直线一点
 @param : [in] pt2  直线一点
 @param : [out] footPt  点在直线上的垂足
 @return:  1  -  点在直线上； 0 -  点在直线外。
 */
  int IsPointInLine(Vertex3d& pt, Vertex3d& pt1, Vertex3d& pt2, Vertex3d* pFootPt = NULL);//xxy//

 /**判断点是否在空间多边形（同一平面内）内
 @param : [in] pt  待判断点
 @param : [in] vPts  直线一点
 @return:  1  -  点在多边形内； 0 -  点在多边形外。
 */
  int IsPointInPolygon(Vertex3d& pt, vector<Vertex3d>& vPts);

 /**判断点与空间多边形（同一平面内）的关系（射线法）
 @param : [in] ptPoint  待判断点
 @param : [in] vPts  多边形
 @param : [in] vPts  指定的射线
 @return:  'o' - 在多边形外； 'i' - 在多边形内；'E' - 在多边形边界上； 'V' - 在多边形点上
 */
  char RPointWithPolygon(Vertex3d& ptPoint, vector<Vertex3d>& vPts, Vertex3d *pRay = NULL);

 /**判断点投影与空间多边形投影的关系（射线法）
 @param : [in] ptPoint  待判断点
 @param : [in] vPts  直线一点
 @return:  'o' - 在多边形外； 'i' - 在多边形内；'E' - 在多边形边界上； 'V' - 在多边形点上
 */
  char RPointWithPolygon2d(Vertex3d& ptPoint, vector<Vertex3d>& vPts);

 /**判断多边形与多边形在XOY投影上的关系：包含、相交、相离
 @param : [in] pt  待判断点
 @param : [in] vPts  直线一点
 @return:  0 - 相离；1 - 相交；2 - 包含；
 */
  int RPolygonWithPolygon2D(vector<Vertex3d>& ptAs, vector<Vertex3d>& ptBs); //xx_add_0619, // ydcl
 /**判断线段与空间平面的关系
 @param : [in] LineStr  线段起点
 @param : [in] LineEnd  线段终点
 @param : [in] plane	   平面
 @param : [out] pt	   线段与平面的交点
 @return:  '0' - The segment lies strictly to one side or the other of the plane；
 '1' - The segment intersects the plane
 'p' - The segment lies wholly within the plane.；
 's' - The startpoint is on the plane
 'e' - The endpoint is on the plane
 */
  char RLineSegWithPlane(Vertex3d& LineStr, Vertex3d& LineEnd, PLANE3D& plane, Vertex3d* pCrossPt = NULL);//xxy//

 /**判断线段是否与空间平面相交
 @param : [in] LineStr  线段起点
 @param : [in] LineEnd  线段终点
 @param : [in] plane	   平面
 @return:  1 - 线段与空间平面相交
 0 - 线段与空间平面不相交
 */
  int IsLineSegCrossPlane(Vertex3d& LineStr, Vertex3d& LineEnd, PLANE3D& plane);

 /**（三维空间中）判断直线与平面的关系
 @param : [IN] LineStr  直线一点。
 @param : [IN] LineEnd  直线一点。
 @param : [IN] plane    平面
 @param : [OUT] pt	  直线与平面的交点
 @return : '1'	- 线段与平面相交；'0' - 线段与平面平行；'2' - 线段在平面上
 */
  char RLineWithPlane(Vertex3d& LineStr, Vertex3d& LineEnd, PLANE3D& plane, Vertex3d* pCrossPt = NULL);//xxy//

 /**（三维空间中）判断线段是否穿过三角形所在平面，并且有一个交点在三角形内
 @param ： [IN] LineStr			线段起点。
 @param ： [IN] LineEnd			线段终点。
 @param ： [IN] pt1，pt2，pt3	确定三角形的三个点。
 @param ： [OUT] pt				交点
 @return：  1  - 线段穿过三角形平面，并交点在三角形； 0 -  其他情况。
 */
  int IsLineSegCrossTri(Vertex3d& LineStr, Vertex3d& LineEnd, Vertex3d& pt1, Vertex3d& pt2, Vertex3d& pt3, Vertex3d* pCrossPt = NULL);//xxy//

 /**（三维空间中）判断线段与三角形的关系
 @param ： [IN] LineStr			线段起点。
 @param ： [IN] LineEnd			线段终点。
 @param ： [IN] pt1，pt2，pt3	确定三角形的三个点。
 @param ： [OUT] pt				交点//??
 @return：
 'V': the open segment includes a vertex of T.
 'E': the open segment includes a point in the relative interior of an edge of T.
 'F': the open segment includes a point in the relative interior of a face of T.
 '0': the open segment does not intersect triangle T.
 */
  char RLineSegCrossTri(Vertex3d& LineStr, Vertex3d& LineEnd, Vertex3d& pt1, Vertex3d& pt2, Vertex3d& pt3);

 /**判断Line1Str、Line1End和Line2Str、Line2End分别构成的两条线段的空间关系。
 @param ： [IN] Line1Str  线段1起点。
 @param ： [IN] Line1End  线段1终点。
 @param ： [IN] Line2Str  线段2起点。
 @param ： [IN] Line2End  线段2终点。
 @param ： [IN] bOnPlane	 给定的共面标记。
 @param ： [OUT] pt       求得的两线段交点。
 @return ： '1'- 两线段有交点;  '0'- 两线段异面; '3'	- 两线段平行; '2'-共面不相交也不平行。
 */
  char RLineSegWithLineSeg(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, int bOnPlane = 0, Vertex3d* pCrossPt = NULL);//xxy//


 /**判断射线与三角形的相交情况。
 @param ：  [IN] LineStr  射线起点。
 @param ： [IN] LineEnd  射线终点。
 @param ： [IN] pt1,pt2,pt3  三角形三个顶点。
 @return ： 'V'- 射线起点与三角形顶点重合;  'E'- 射线起点与三角形边界重合; 'F'- 射线起点在三角形内;
 'v'- 射线和平面的交点与三角形顶点重合; 'e'- 射线和平面的交点与三角形顶点重合; 'f'- 射线和平面的交点穿过三角形;
 '0'- 其他情况
 */
  char RRayCrossTri(Vertex3d& LineStr, Vertex3d& LineEnd, Vertex3d& pt1, Vertex3d& pt2, Vertex3d& pt3);

 // 1 - 相交 0 - 不相交
  int IsRayCrossEnvelope(RayImpl& ray, const Envelope3d& env, double t0 = 0.0, double t1 = MathEx::dDoubleMax);
  int IsRayCrossEnvelope(RayImpl& r, const AABBoxImpl & env, double t0 = 0.0, double t1 = MathEx::dDoubleMax);

  char RLineSegWithLine(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, int bOnPlane = 0, Vertex3d* pCrossPt = NULL);

 /**判断Line1Str、Line1End和Line2Str、Line2End分别构成的两条直线的空间关系。
 @param ： [IN] Line1Str  线段1起点。
 @param ： [IN] Line1End  线段1终点。
 @param ： [IN] Line2Str  线段2起点。
 @param ： [IN] Line2End  线段2终点。
 @param ： [OUT] pt    求得的两线段交点。
 @return ： '1'- 两直线有交点;  '0'- 两直线异面; '2'	- 两直线平行;
 */
  char RLineWithLine(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, int bOnPlane = 0, Vertex3d* pIntersect = NULL);
 /** @} */ // 模块结尾

 // return : '0' - 两面平行但不重合  '2' 两面重合 '1' - 两面相交
  char RPlaneWithPlane(PLANE3D& plane1, PLANE3D& plane2, Vertex3d& pt1, Vertex3d& pt2);

 // return : '0' - 点在面内  '2' - 点在面上 '1' - 点在面外
  char RPointWithPlane(Vertex3d& pt, PLANE3D& plane);

 // return : '0' - box在视椎体外  '2' - 相交 '1' - 在视椎体内
  char RFrustumWithEnvelope(FrustumImpl& frusum, Envelope3d& env);
  char RFrustumWithEnvelope(FrustumImpl& frusum, AABBoxImpl & env);

 // return : '0' - 球在视椎体外  '2' - 相交 '1' - 在视椎体内
  char RFrustumWithSphere(FrustumImpl& frusum, Vertex3d& cen, double radius);

  char RFrustumWithTriangle(FrustumImpl& frustum, Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3);

  char RFrustumWithPolygon(FrustumImpl& frustum, vector<Vertex3d>& PtsArray);

 // return : 1 - 两球相交；0 - 两球相离； 这里除了相离之外，其他都算相交 // 这里还可以写个函数具体判断sphere之间的各种关系
  int IsSphereIntersectSphere(Vertex3d& cen1, double radius1, Vertex3d& cen2, double radius2);

 // return : '0' - 球在圆椎体外  '2' - 相交 '1' - 在圆椎体内
  char RConeWithSphere(double dAngle, Vertex3d& vStart, Vertex3d& vDir, Vertex3d& shpCen, double radius);

 // return : 1 - 球在圆锥相交（圆锥包含球也算）；0 - 球在圆锥外
  int IsConeIntersectSphere(double dAngle, Vertex3d& vStart, Vertex3d& vDir, Vertex3d& shpCen, double radius);

 // 判断二维线段相交情况
  char RLineSegWithLineSeg2d(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, Vertex3d* pCrossPt = NULL);//xxy//

  char RRectWithPolygon2d(Vertex3d& ptMin, Vertex3d& ptMax, vector<Vertex3d>& vPts);
  char RRayWithLineSeg(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, int bOnPlane, Vertex3d* pCrossPt);

  char RRayWithLine(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, int bOnPlane, Vertex3d* pCrossPt);

  char RRayWithRay(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, int bOnPlane, Vertex3d* pCrossPt);

  char RRayWithPlane(Vertex3d& LineStr, Vertex3d& LineEnd, PLANE3D& plane, Vertex3d* pCrossPt);

  int IsPointInRay(Vertex3d& pt, Vertex3d& pt1, Vertex3d& pt2, Vertex3d* pFootPt);
 // xx_add_for_test_20100321
  void FuzzyPrecision_base(Vertex3d& pos, int nMedian);
  void FuzzyPrecision_base(double& dData, int nMedian);
  void FuzzyPrecision_base(float& dData, int nMedian);
 // xx_add_for_test_20100321



  /***********************************************/
  // 对于后缀名为Ex的函数，在求插值系列函数中，即Inp系列函数中，都表示对整个点集进行插值,得到的集合只有插值点，不包括原来的控制点

  // 注意不同的插值方式，可能控制点会有所改变，例如BSpline
  // 所有的插值方式都不使用拉格朗日多项式插值，因为高次不可取，出现龙格，另外局部控制性不强，所以采用的都是分段等距低次插值。
  // 由于是分段低次插值，所以需要考虑不同曲线段之间的连续性问题。C0表示位置连续，C1表示一阶导数连续
  // 各种分段三次多项式没有最好的表达方法，都是根据实际应用情况来选择。一般来说，都能具备下面性质中的3个性质，而不是所有的性质：
  // （1）曲线的每一片段都是三次的
  // （2）曲线对控制点插值。（曲线通过所有的型值点）
  // （3）曲线有局部控制性
  // （4）曲线有C2连续性
  /***********************************************/

  // 线性插值
  // 返回值：插值点；插值点离上一个控制点的距离；插值点之间2个控制点的距离；上一个控制点的索引号
Vertex3d InpByLinearDis(std::vector<Vertex3d>& ptArray, double dStepLength, int &nCurPathNode, double &dDistanceAccumulation, double &dist_std);

  // 刚体弹性插值
  // 假设有一个拉力vTrans，拉动camPos，最后的拉到位置
Vertex3d InpByLinearDis(std::vector<Vertex3d>& ptArray, int &nCurPathNode, Vertex3d vTrans, double& dDistanceAccumulation);

  // 三次Hermite插值（也称为三次参数样条）： 对控制点插值，有局部控制性，但是没有C2连续性
Vertex3d InpByCubicHermite(Vertex3d& P0, Vertex3d& D0, Vertex3d& P1, Vertex3d& D1, double t);

  // 三次Hermite插值特例：KochanekBartels样条
Vertex3d InpByKochanekBartels(Vertex3d& P0, Vertex3d& P1, Vertex3d& P2, Vertex3d& P3, double t);

Vertex3d InpByKochanekBartels(std::vector<Vertex3d>& ptArray, int iIndex, double t);

void	InpByKochanekBartels(std::vector<Vertex3d>& ptArray, double dInpDis, std::vector<Vertex3d>& allPtArray, std::vector<int>& indexArray);

  // 三次Hermite插值特例：基数样条中的特例Catmull-Rom样条
Vertex3d InpByCatmullRom(Vertex3d& P0, Vertex3d& P1, Vertex3d& P2, Vertex3d& P3, double t);

Vertex3d InpByCatmullRom(std::vector<Vertex3d>& ptArray, int iIndex, double t);

  // 返回值：插值后所有的点集，包含控制点；并且返回控制点在其中的索引号集合
void	InpByCatmullRom(std::vector<Vertex3d>& ptArray, double dInpDis, std::vector<Vertex3d>& allPtArray, std::vector<int>& indexArray);

  // 返回值：插值后所有插值得到的点集，不包含控制点；并且返回每一段插值点的个数集合
  //void	InpByCatmullRomEx(std::vector<Vertex3d>& ptArray, double dInpDis,std::vector<Vertex3d>& inpPtArray,std::vector<int>& indexArray);

  // 均匀三次B样条：有局部控制性，有C2连续性，但是不对控制点插值
Vertex3d InpByCubicBSpline(Vertex3d& P0, Vertex3d& P1, Vertex3d& P2, Vertex3d& P3, double t);

Vertex3d InpByCubicBSpline(std::vector<Vertex3d>& ptArray, int iIndex, double t);

void	InpByCubicBSpline(std::vector<Vertex3d>& ptArray, double dStepLength, std::vector<Vertex3d>& allPtArray, std::vector<int>& indexArray);

  // 三次Bezier样条：有C2连续性，通过第一和最后一个控制点，没有局部控制性
Vertex3d InpByCubicBezier(Vertex3d& P0, Vertex3d& P1, Vertex3d& P2, Vertex3d& P3, double t);

Vertex3d InpByCubicBezier(std::vector<Vertex3d>& ptArray, int iIndex, double t);

void	InpByCubicBezier(std::vector<Vertex3d>& ptArray, double dStepLength, std::vector<Vertex3d>& allPtArray, std::vector<int>& indexArray);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// Public interface for Strip and VertexList Optimization based on NVTriStrip from nVidia
////////////////////////////////////////////////////////////////////////////////////////
#define CACHESIZE   32//32 for G80 24 for pre-G80
enum PrimType
{
	PT_LIST,
	PT_STRIP,
	PT_FAN
};
struct PrimitiveGroup //structure for strip
{
	PrimType type;
	unsigned long numIndices;
	unsigned long* indices;

	////////////////////////////////////////////////////////////////////////////////////////
	PrimitiveGroup() : type(PT_STRIP), numIndices(0), indices(NULL) {}
	~PrimitiveGroup()
	{
		if (indices)
			delete[] indices;
		indices = NULL;
	}
};
/** 根据给定的三角形列表对Mesh进行优化
* @param [in] isStrip -- true 生成一条条带 false 仅优化列表
* @param [in] in_indices -- 输入三角形顶点索引列表
* @param [in] in_numIndices -- 输入的顶点索引个数
* @param [out] primGroups -- 输出的条带(列表)
* @param [out] numGroups -- 输出的条带(列表)个数
* @return true -- 优化成功; false -- 优化错误
*/
bool GenerateStripsEx(bool isStrip, const unsigned long* in_indices, const unsigned long in_numIndices, PrimitiveGroup** primGroups, unsigned long* numGroups);
/** 根据给定的三角形列表对Mesh进行优化
* @param [in] isStrip -- true 生成条带 false 仅优化列表
* @param [in] isStitch -- true 生成一个条带(默认) false 生成多个条带
* @param [in] in_MinStripSize -- 生成多个条带时，限制最小条带中顶点个数(默认为0)
* @param [in] in_indices -- 输入三角形顶点索引列表
* @param [in] in_numIndices -- 输入的顶点索引个数
* @param [out] primGroups -- 输出的条带(列表)
* @param [out] numGroups -- 输出的条带(列表)个数
* @return true -- 优化成功; false -- 优化错误
*/
bool GenerateStripsExEx(bool isStrip, bool isStitch, const unsigned long in_MinStripSize, const unsigned long* in_indices, const unsigned long in_numIndices,
	PrimitiveGroup** primGroups, unsigned long* numGroups);

////////////////////////////////////////////////////////////////////////////////////////////////////


end_gdb_namespace
end_gtl_namespace



