#include "internal_spatialfuns.h"
#include <vector>
#include "envelope3d.h"


begin_gtl_namespace
begin_gdb_namespace
int GetPlaneEquation(Vertex3d &p1, Vertex3d &p2, Vertex3d &p3, PLANE3D& plane)
{
	Vertex3d vNormal = (p2 - p1) ^ (p3 - p2);

	if (vNormal == Vertex3d(0, 0, 0))
		return 0;
	vNormal.normalize(); // 最好进行归一化处理  否则后面的一些判断不容易进行
	plane.A = vNormal.x;
	plane.B = vNormal.y;
	plane.C = vNormal.z;
	plane.D = -(plane.A * p1.x + plane.B * p1.y + plane.C * p1.z);
	return 1;
}

int GetPlaneEquationEx(Vertex3d &p1, Vertex3d &p2, Vertex3d &p3, PLANE3D& plane)
{
	 //此函数暂时无效
	Vertex3d vNormal = (p2 - p1) ^ (p3 - p2);
	if (vNormal == Vertex3d(0, 0, 0))
		return 0;
	vNormal.normalize();

	plane.A = vNormal.x;
	plane.B = vNormal.y;
	plane.C = vNormal.z;

	plane.D = -(plane.A * p1.x + plane.B * p1.y + plane.C * p1.z);
	return 1;
}

int IsPlaneValid(PLANE3D& plane)
{
	if (plane.A == 0 && plane.B == 0 && plane.C == 0)
		return 0;
	return 1;
}

int GetPlaneEquation(Vertex3d &vNormal, Vertex3d &pt, PLANE3D& plane)
{
	if (vNormal == Vertex3d(0, 0, 0))
		return 0;
	plane.A = vNormal.x;
	plane.B = vNormal.y;
	plane.C = vNormal.z;
	plane.D = -(plane.A * pt.x + plane.B * pt.y + plane.C * pt.z);
	return 1;
}


void GetPlaneNormal(PLANE3D& plane, Vertex3d& pt)
{
	pt.x = plane.A;
	pt.y = plane.B;
	pt.z = plane.C;
}

void GetPlaneNormalEx(PLANE3D& plane, Vertex3d& pt, Vertex3d& vNormal)
{
	GetPlaneNormal(plane, vNormal);
	//	if(vNormal * pt + plane.D < 0)
	if ((vNormal * pt + plane.D) < -MathEx::TOL)
		vNormal = -vNormal;
}

void GetOnePointInPlane(PLANE3D& plane, Vertex3d& pt)
{
	double temp = rand() % 100 * 1.0;
	pt.x = temp;
	temp = rand() % 100 * 1.0;
	pt.y = temp;
	temp = rand() % 100 * 1.0;
	pt.z = temp;

	if (fabs(plane.C) > MathEx::TOL)
		pt.z = -(plane.D + plane.A * pt.x + plane.B * pt.y) / plane.C;
	else
	{
		if (fabs(plane.B) > MathEx::TOL)
		{
			pt.y = -(plane.D + plane.A * pt.x + plane.C * pt.z) / plane.B;
		}
		else
		{
			pt.x = -(plane.D + plane.B * pt.y + plane.C * pt.z) / plane.A;
		}
	}
}

void GetOnePointInPlane(PLANE3D& plane, Vertex3d& pt, Vertex3d& Diffpt)
{
	// 这样的点肯定不同于点Diffpt
	double temp = rand() % 100 * 1.0 + 1.0;
	pt.x = Diffpt.x - temp;
	temp = rand() % 100 * 1.0 + 1.0;
	pt.y = Diffpt.y - temp;
	temp = rand() % 100 * 1.0 + 1.0;
	pt.z = Diffpt.z - temp;

	if (fabs(plane.C) > MathEx::TOL)
		pt.z = -(plane.D + plane.A * pt.x + plane.B * pt.y) / plane.C;
	else
	{
		if (fabs(plane.B) > MathEx::TOL)
		{
			pt.y = -(plane.D + plane.A * pt.x + plane.C * pt.z) / plane.B;
		}
		else
		{
			pt.x = -(plane.D + plane.B * pt.y + plane.C * pt.z) / plane.A;
		}
	}
}

/* Return a random ray endpoint */
void GetRandomRay(Vertex3d& ray, double radius)
{
	double x, y, z, w, t;

	const int MAX_INT = 32767;
	/* Generate a random point on a sphere of radius 1. */
	/* the sphere is sliced at z, and a random point at angle t
	generated on the circle of intersection. */
	z = 2.0 * (double)rand() / MAX_INT - 1.0;
	t = 2.0 * MathEx::ONE_PI * (double)rand() / MAX_INT;
	w = sqrt(1 - z*z);
	x = w * cos(t);
	y = w * sin(t);

	ray.x = (radius * x);
	ray.y = (radius * y);
	ray.z = (radius * z);

	/*printf( "RandomRay returns %6d %6d %6d\n", ray[X], ray[Y], ray[Z] );*/
}

void GetRandomRayInPlane(Vertex3d& ray, double radius, PLANE3D& plane)
{
	Vertex3d vPt1, vPt2;
	GetOnePointInPlane(plane, vPt1);
	GetOnePointInPlane(plane, vPt2, vPt1);
	ray = vPt2 - vPt1;
	ray.normalize();
	ray = ray * radius;
}

int GetPointsNormal(vector<Vertex3d>& ptArray, Vertex3d& vNormal)
{
	int nPtNum = ptArray.size();
	int nRecordIndex = -1;

	vNormal = Vertex3d(0, 0, 0);

	//用于遍历最后一个边界 i-1 -- 0 
	Vertex3d vert = ptArray[nPtNum - 1];

	bool isFoundRecord = false;
	for (int i = 0; i<nPtNum; i++)
	{
		if (!isFoundRecord)
		{
			if (i >= 2)
			{
				if (0 == IsPointInLine(ptArray[i], ptArray[i - 2], ptArray[i - 1]))
				{
					nRecordIndex = i;
					isFoundRecord = true;
				}
			}
			else if (i == 0)
			{
				if (0 == IsPointInLine(ptArray[i], ptArray[nPtNum - 2], ptArray[nPtNum - 1]))
				{
					nRecordIndex = i;
					isFoundRecord = true;
				}
			}
			else
			{
				if (0 == IsPointInLine(ptArray[i], ptArray[nPtNum - 1], ptArray[0]))
				{
					nRecordIndex = i;
					isFoundRecord = true;
				}
			}
		}

		//Newill's Algorithm (计算平面向量法线的和，能够处理凹凸轻微不共面三维多边形，并通过增项减少一次乘法)
		vNormal.x += (vert.z + ptArray[i].z) * (vert.y - ptArray[i].y);
		vNormal.y += (vert.x + ptArray[i].x) * (vert.z - ptArray[i].z);
		vNormal.z += (vert.y + ptArray[i].y) * (vert.x - ptArray[i].x);

		vert = ptArray[i];
	}

	vNormal.normalize();

	if (Vertex3d(0, 0, 0) == vNormal)
	{
		return -1;
	}

	//判断点集是否位于同一个平面上
	PLANE3D refPlane;
	GetPlaneEquation(vNormal, ptArray[0], refPlane);
	for (int i = 1; i<nPtNum; i++)
	{
		if (DPointToPlaneEx(ptArray[i], refPlane) > MathEx::TOL)
		{
			vNormal = Vertex3d(0, 0, 0);
			return -1;
		}
	}

	return nRecordIndex;
}

// 增量法，有待检验
void GetConvexHull(vector<Vertex3d>& ptArray, vector<Vertex3d>& CHArray) // xx_add_0619
{
	// ydcl
	//（1）重复点没有考虑
	//（2）斜率为无穷大（垂直）没有考虑
	Vertex3d a[999999];

	int t, i = 0, m, q, count;
	int n = 3;
	double k, kk, kkk, tmp;

	count = ptArray.size();

	/*初始化三个点，在首与尾各加上另一边的邻接点*/
	a[0] = ptArray.at(2);
	a[1] = ptArray.at(0);
	a[2] = ptArray.at(1);
	a[3] = ptArray.at(2);
	a[4] = ptArray.at(0);

	/*不断加入新的点*/
	for (t = 3; t<count; t++)
	{
		q = 0;

		/*求各边斜率*/
		for (i = 0; i <= n - 1; i++)
		{
			k = (ptArray.at(t).y - a[i].y) / (ptArray.at(t).x - a[i].x);
			kk = (ptArray.at(t).y - a[i + 1].y) / (ptArray.at(t).x - a[i + 1].x);
			kkk = (ptArray.at(t).y - a[i + 2].y) / (ptArray.at(t).x - a[i + 2].x);

			if (k>kkk)
			{
				tmp = k; k = kkk; kkk = tmp;
			}

			if (kk >= kkk || kk <= k)    // 此处找到了经过加入点的，并且与原有多边形相切的一条直线
			{
				break;
			}

		}

		while (1)
		{
			k = (ptArray.at(t).y - a[i + 1].y) / (ptArray.at(t).x - a[i + 1].x);
			kk = (ptArray.at(t).y - a[i + 2].y) / (ptArray.at(t).x - a[i + 2].x);
			kkk = (ptArray.at(t).y - a[i + 3].y) / (ptArray.at(t).x - a[i + 3].x);

			if (k>kkk)
			{
				tmp = k; k = kkk; kkk = tmp;
			}

			if (kk >= k&&kk <= kkk)  // 从相切直线上交点的下一个点算起，观察之后点是否该删除
			{
				q++;    i++;
			}

			if (kk >= kkk || kk <= k)
				break;
		}

		if (q == 0)    // 若之后没有要删除的点，则将原来要加入的点真正加入多边形中 
		{
			for (m = n + 1; m >= i + 2; m--)
				a[m + 1] = a[m];

			a[i + 2] = ptArray.at(t);

			a[0] = a[n + 1];
			a[n + 2] = a[1];
		}

		if (q != 0)   // 若之后又要删除的点，则先删除这些点，然后再将原来要加入的点正式加入
		{
			a[i - q + 2] = ptArray.at(t);

			for (m = i + 2; m <= n + 1; m++)
				a[m + 1 - q] = a[m];

			a[0] = a[n - q + 1];
			a[n - q + 2] = a[1];
		}

		n = n + 1 - q;
	}

	for (int l = 0; l<n; l++)
		CHArray.push_back(a[l]);

}
int GetPointsNormalEx(vector<Vertex3d>& ptArray, Vertex3d& vNormal)
{
	int nPtNum = ptArray.size();
	int nRecordIndex = -1;

	vNormal = Vertex3d(0, 0, 0);

	//用于遍历最后一个边界 i-1 -- 0 
	Vertex3d vert = ptArray[nPtNum - 1];

	bool isFoundRecord = false;
	for (int i = 0; i<nPtNum; i++)
	{
		if (!isFoundRecord)
		{
			if (i >= 2)
			{
				if (0 == IsPointInLine(ptArray[i], ptArray[i - 2], ptArray[i - 1]))
				{
					nRecordIndex = i;
					isFoundRecord = true;
				}
			}
			else if (i == 0)
			{
				if (0 == IsPointInLine(ptArray[i], ptArray[nPtNum - 2], ptArray[nPtNum - 1]))
				{
					nRecordIndex = i;
					isFoundRecord = true;
				}
			}
			else
			{
				if (0 == IsPointInLine(ptArray[i], ptArray[nPtNum - 1], ptArray[0]))
				{
					nRecordIndex = i;
					isFoundRecord = true;
				}
			}
		}

		//Newill's Algorithm (计算平面向量法线的和，能够处理凹凸轻微不共面三维多边形，并通过增项减少一次乘法)
		vNormal.x += (vert.z + ptArray[i].z) * (vert.y - ptArray[i].y);
		vNormal.y += (vert.x + ptArray[i].x) * (vert.z - ptArray[i].z);
		vNormal.z += (vert.y + ptArray[i].y) * (vert.x - ptArray[i].x);
		vert = ptArray[i];
	}

	vNormal.normalize();

	if (Vertex3d(0, 0, 0) == vNormal)
	{
		return -1;
	}
	return nRecordIndex;
}

int EnsureAntiClock(vector<Vertex3d>& Point3dArray, Vertex3d &vDirection)
{
	EnsureClose(Point3dArray); // 保证封闭

	int nPt = Point3dArray.size();
	double Area2 = 0.0;

	Vertex3d pt = Point3dArray[0]; //选择一个固定点

	for (int i = 0; i<nPt - 1; i++)
	{
		Vertex3d v1, v2;
		v1 = pt - Point3dArray[i];
		v2 = pt - Point3dArray[i + 1];
		Vertex3d v3 = v1 ^ v2;

		if (v3 * vDirection < -1e-6)
			Area2 -= v3.length(); // v3的模
		else
			Area2 += v3.length();
	}

	if (Area2 < -MathEx::TOL)
	{
		// 数组逆序算法
		for (int j = 0; j<(nPt + 1) / 2; j++)
		{
			Vertex3d temp = Point3dArray[j];
			Point3dArray[j] = Point3dArray[nPt - 1 - j];
			Point3dArray[nPt - 1 - j] = temp;
		}
		return 0;
	}
	return 1;
}

void EnsureClose(vector<Vertex3d>& pointArray)//使数组首尾点相同
{
	int pointNum = pointArray.size();
	if (pointNum <= 2)
		return;
	///封闭	
	if (pointArray[0] != pointArray[pointNum - 1])
		pointArray.push_back(pointArray[0]);
	/*else
	pointArray[pointNum-1] = pointArray[0];*/ // xx_modify_1011
}

void EnsureOpen(vector<Vertex3d>& ptArray)
{
	int pointNum = ptArray.size();
	if (pointNum < 2)
		return;

	EnsureNoRedunant(ptArray);

	if (ptArray[0] == ptArray[pointNum - 1])
		ptArray.erase(ptArray.begin() + pointNum - 1);
}

void EnsureNoRedunant(vector<Vertex3d>& pointArray)
{
	int pointNum = pointArray.size();
	if (pointNum < 2)  // xx_modify_1011	
		return;

	vector<Vertex3d> tempPointArray;
	tempPointArray = pointArray;
	pointArray.clear();

	for (int i = 0; i<pointNum - 1; i++)
	{
		if (tempPointArray[i] != tempPointArray[i + 1])
		{
			pointArray.push_back(tempPointArray[i]);
		}
	}
	pointArray.push_back(tempPointArray[pointNum - 1]);
}

void EnsureNoRedunantWithTOL(vector<Vertex3d>& pointArray, double dTOL)
{
	int pointNum = pointArray.size();
	if (pointNum < 2)	// xx_modify_1011
		return;

	vector<Vertex3d> tempPointArray;
	tempPointArray = pointArray;
	pointArray.clear();

	for (int i = 0; i<pointNum - 1; i++)
	{
		if ((tempPointArray[i] - tempPointArray[i + 1]).length() > dTOL)
		{
			pointArray.push_back(tempPointArray[i]);
		}
	}
	pointArray.push_back(tempPointArray[pointNum - 1]);
}

void GetPlaneEquationByVerticalPlane(Vertex3d& pt1, Vertex3d& pt2, PLANE3D& plane1, PLANE3D& plane2)
{
	Vertex3d Vn1(plane1.A, plane1.B, plane1.C);//平面1的法向量

	Vertex3d Vn = Vn1 ^ (pt2 - pt1); // 平面2的法向量
	GetPlaneEquation(Vn, pt1, plane2);
}

void GetPlaneEquationByParallelPlane(Vertex3d& pt, PLANE3D& plane1, PLANE3D& plane2)
{
	Vertex3d Vn1(plane1.A, plane1.B, plane1.C);
	GetPlaneEquation(Vn1, pt, plane2);
}

void GetPlaneEquationByParallelLine(Vertex3d& pt1, Vertex3d& pt2, Vertex3d& pt3, Vertex3d& pt4, PLANE3D& plane)
{
	Vertex3d vLine1 = pt2 - pt1;
	Vertex3d vLine2 = pt4 - pt3;
	Vertex3d vVec = vLine1 ^ vLine2;

	if (vVec == Vertex3d(0, 0, 0))
	{
		GetPlaneEquation(pt1, pt2, pt3, plane);	// 如果平行且重合，该平面将无效
	}
	else
	{
		GetPlaneEquation(vVec, pt1, plane);
	}
}
//////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
double DPointToPoint(Vertex3d& pt1, Vertex3d& pt2)
{
	return sqrt((pt1.x - pt2.x) * (pt1.x - pt2.x) +
		(pt1.y - pt2.y) * (pt1.y - pt2.y) +
		(pt1.z - pt2.z) * (pt1.z - pt2.z));
}


double DPointToPointEx(Vertex3d& pt1, Vertex3d& pt2)
{
	return (pt1.x - pt2.x) * (pt1.x - pt2.x) +
		(pt1.y - pt2.y) * (pt1.y - pt2.y) +
		(pt1.z - pt2.z) * (pt1.z - pt2.z);
}

double DPointToPlaneEx(Vertex3d& pt3d, PLANE3D& plane)
{
	double temp = plane.A * pt3d.x + plane.B * pt3d.y + plane.C * pt3d.z + plane.D;
	return temp * temp / (plane.A * plane.A + plane.B * plane.B + plane.C * plane.C);
}

double DPointToPlane(Vertex3d& pt3d, PLANE3D& plane)
{
	return sqrt(DPointToPlaneEx(pt3d, plane));
}

double DPointToLineSeg(Vertex3d& pPoint, Vertex3d& pStartPoint, Vertex3d& pEndPoint, Vertex3d* pResultPoint)
{
	return sqrt(DPointToLineSegEx(pPoint, pStartPoint, pEndPoint, pResultPoint));
}
double DPointToRay(Vertex3d& pPoint, Vertex3d& pStartPoint, Vertex3d& pEndPoint, Vertex3d* pResultPoint)
{
	return sqrt(DPointToRayEx(pPoint, pStartPoint, pEndPoint, pResultPoint));
}

double DPointToRayEx(Vertex3d& pPoint, Vertex3d& pStartPoint, Vertex3d& pEndPoint, Vertex3d* pResultPoint)
{
	Vertex3d temp1, temp2;
	temp1 = pEndPoint - pStartPoint;
	temp2 = pPoint - pStartPoint;

	double c1, c2;
	c1 = temp1 * temp2;
	c2 = temp1 * temp1;

	if (c1 < MathEx::TOL)
	{
		if (pResultPoint)
			*pResultPoint = pStartPoint;
		return DPointToPointEx(pPoint, pStartPoint);
	}
	// 		 if(c2 <= c1)
	// 		 {
	// 			 if(pResultPoint)
	// 				 *pResultPoint = pEndPoint;
	// 			 return DPointToPointEx(pPoint,pEndPoint);
	// 		 }

	double b = c1 / c2;
	temp2 = pStartPoint + temp1 * b;

	if (pResultPoint)
		*pResultPoint = temp2;

	return DPointToPointEx(pPoint, temp2);
}

//三维点到线段的空间距离(平方值)
double DPointToLineSegEx(Vertex3d& pPoint, Vertex3d& pStartPoint, Vertex3d& pEndPoint, Vertex3d* pResultPoint)
{
	Vertex3d temp1, temp2;
	temp1 = pEndPoint - pStartPoint;
	temp2 = pPoint - pStartPoint;

	double c1, c2;
	c1 = temp1 * temp2;
	c2 = temp1 * temp1;

	if (c1 < MathEx::TOL)
	{
		if (pResultPoint)
			*pResultPoint = pStartPoint;
		return DPointToPointEx(pPoint, pStartPoint);
	}
	//	 if(c2 <= c1)
	if ((c2 - c1) < MathEx::TOL)
	{
		if (pResultPoint)
			*pResultPoint = pEndPoint;
		return DPointToPointEx(pPoint, pEndPoint);
	}

	double b = c1 / c2;
	temp2 = pStartPoint + temp1 * b;

	if (pResultPoint)
		*pResultPoint = temp2;

	return DPointToPointEx(pPoint, temp2);
}
//空间中三角形到三角形的最小距离的平方值
double DTriangleToTriangleEx(Vertex3d& tri1Pt1, Vertex3d& tri1Pt2, Vertex3d& tri1Pt3, Vertex3d& planePt1, Vertex3d& planePt2, Vertex3d& planePt3, Vertex3d* pPoint1, Vertex3d* pPoint2)
{
	Vertex3d resPoints[12];
	double dDis[6];
	double dTempDis = 1e+14;

	// 一个三角形各边到另一个三角形的最小距离，取最小值
	if (pPoint1 == NULL || pPoint2 == NULL)
	{
		dDis[0] = DLineSegToTriangleEx(tri1Pt1, tri1Pt2, planePt1, planePt2, planePt3);
		dDis[1] = DLineSegToTriangleEx(tri1Pt2, tri1Pt3, planePt1, planePt2, planePt3);
		dDis[2] = DLineSegToTriangleEx(tri1Pt1, tri1Pt3, planePt1, planePt2, planePt3);

		dDis[3] = DLineSegToTriangleEx(planePt1, planePt2, tri1Pt1, tri1Pt2, tri1Pt3);
		dDis[4] = DLineSegToTriangleEx(planePt2, planePt3, tri1Pt1, tri1Pt2, tri1Pt3);
		dDis[5] = DLineSegToTriangleEx(planePt1, planePt3, tri1Pt1, tri1Pt2, tri1Pt3);

		dTempDis = dDis[0];
		for (int i = 1; i < 6; i++)
		{
			//	 if(dTempDis > dDis[i])
			if ((dTempDis - dDis[i]) > MathEx::TOL)
			{
				dTempDis = dDis[i];
			}
		}
	}
	else
	{
		dDis[0] = DLineSegToTriangleEx(tri1Pt1, tri1Pt2, planePt1, planePt2, planePt3, &resPoints[0], &resPoints[1]);
		dDis[1] = DLineSegToTriangleEx(tri1Pt2, tri1Pt3, planePt1, planePt2, planePt3, &resPoints[2], &resPoints[3]);
		dDis[2] = DLineSegToTriangleEx(tri1Pt1, tri1Pt3, planePt1, planePt2, planePt3, &resPoints[4], &resPoints[5]);

		dDis[3] = DLineSegToTriangleEx(planePt1, planePt2, tri1Pt1, tri1Pt2, tri1Pt3, &resPoints[6], &resPoints[7]);
		dDis[4] = DLineSegToTriangleEx(planePt2, planePt3, tri1Pt1, tri1Pt2, tri1Pt3, &resPoints[8], &resPoints[9]);
		dDis[5] = DLineSegToTriangleEx(planePt1, planePt3, tri1Pt1, tri1Pt2, tri1Pt3, &resPoints[10], &resPoints[11]);

		dTempDis = dDis[0];
		*pPoint1 = resPoints[0];
		*pPoint2 = resPoints[1];

		for (int i = 1; i < 6; i++)
		{
			//	 if(dTempDis > dDis[i])
			if ((dTempDis - dDis[i]) > MathEx::TOL)
			{
				dTempDis = dDis[i];
				*pPoint1 = resPoints[i * 2];
				*pPoint2 = resPoints[i * 2 + 1];
			}
		}
	}
	return dTempDis;
}

//空间中三角形到三角形的最小距离
double DTriangleToTriangle(Vertex3d& tri1Pt1, Vertex3d& tri1Pt2, Vertex3d& tri1Pt3, Vertex3d& planePt1, Vertex3d& planePt2, Vertex3d& planePt3, Vertex3d* pPoint1, Vertex3d* pPoint2)
{
	return sqrt(DTriangleToTriangleEx(tri1Pt1, tri1Pt2, tri1Pt3, planePt1, planePt2, planePt3, pPoint1, pPoint2));
}

double DLineSegToTriangleEx(Vertex3d& linePt1, Vertex3d& linePt2, Vertex3d& planePt1, Vertex3d& planePt2, Vertex3d& planePt3, Vertex3d* pPoint1, Vertex3d* pPoint2)
{
	Vertex3d resPoints[10];
	double dDis[5];
	double dTempDis = 0.0;

	if (RLineSegCrossTri(linePt1, linePt2, planePt1, planePt2, planePt3) != '0')
		return 0.0;

	// 线段到三角形每条边的最短距离以及线段端点到三角形的最短距离
	if (pPoint1 == NULL || pPoint2 == NULL)
	{
		dDis[0] = DLineSegToLineSegEx(linePt1, linePt2, planePt1, planePt2);
		dDis[1] = DLineSegToLineSegEx(linePt1, linePt2, planePt2, planePt3);
		dDis[2] = DLineSegToLineSegEx(linePt1, linePt2, planePt1, planePt3);
		dDis[3] = DPointToTriangleEx(linePt1, planePt1, planePt2, planePt3);
		dDis[4] = DPointToTriangleEx(linePt2, planePt1, planePt2, planePt3);

		dTempDis = dDis[0];
		for (int i = 1; i < 5; i++)
		{
			//	 if(dTempDis > dDis[i])
			if ((dTempDis - dDis[i]) > MathEx::TOL)
			{
				dTempDis = dDis[i];
			}
		}
	}
	else
	{
		dDis[0] = DLineSegToLineSegEx(linePt1, linePt2, planePt1, planePt2, &resPoints[0], &resPoints[1]);
		dDis[1] = DLineSegToLineSegEx(linePt1, linePt2, planePt2, planePt3, &resPoints[2], &resPoints[3]);
		dDis[2] = DLineSegToLineSegEx(linePt1, linePt2, planePt1, planePt3, &resPoints[4], &resPoints[5]);

		resPoints[6] = linePt1;
		dDis[3] = DPointToTriangleEx(linePt1, planePt1, planePt2, planePt3, &resPoints[7]);
		resPoints[8] = linePt2;
		dDis[4] = DPointToTriangleEx(linePt2, planePt1, planePt2, planePt3, &resPoints[8]);

		dTempDis = dDis[0];
		int iIndex = 0;

		for (int i = 1; i < 5; i++)
		{
			//  if(dTempDis > dDis[i])
			if ((dTempDis - dDis[i]) > MathEx::TOL)
			{
				dTempDis = dDis[i];
				iIndex = i;
			}
		}

		*pPoint1 = resPoints[iIndex * 2];
		*pPoint2 = resPoints[iIndex * 2 + 1];
	}
	return dTempDis;
}

double DLineSegToPolygon(Vertex3d& linePt1, Vertex3d& linePt2, vector<Vertex3d>& ptArray, Vertex3d* pPoint1, Vertex3d* pPoint2)
{
	return sqrt(DLineSegToPolygonEx(linePt1, linePt2, ptArray, pPoint1, pPoint2));
}

double DLineSegToPolygonEx(Vertex3d& linePt1, Vertex3d& linePt2, vector<Vertex3d>& ptArray, Vertex3d* pPoint1, Vertex3d* pPoint2)
{
	EnsureClose(ptArray);
	int nCount = ptArray.size();

	double dDis = MathEx::dDoubleMax;

	double dTempDis = 0.0;
	if (pPoint1 && pPoint2)
	{
		Vertex3d ptTemp1, ptTemp2;
		dDis = DLineSegToLineSegEx(linePt1, linePt2, ptArray[0], ptArray[1], &ptTemp1, &ptTemp2);
		*pPoint1 = ptTemp1;
		*pPoint2 = ptTemp2;

		for (int i = 1; i < nCount - 1; i++)
		{
			dTempDis = DLineSegToLineSegEx(linePt1, linePt2, ptArray[i], ptArray[i + 1], &ptTemp1, &ptTemp2);
			//	 if(dTempDis < dDis)
			if ((dTempDis - dDis) < -MathEx::TOL)
			{
				dDis = dTempDis;
				*pPoint1 = ptTemp1;
				*pPoint2 = ptTemp2;
			}
		}

		ptTemp1 = linePt1;
		dTempDis = DPointToPolygonEx(linePt1, ptArray, &ptTemp2);
		//	 if(dTempDis < dDis)
		if ((dTempDis - dDis) < -MathEx::TOL)
		{
			dDis = dTempDis;
			*pPoint1 = ptTemp1;
			*pPoint2 = ptTemp2;
		}

		ptTemp1 = linePt2;
		dTempDis = DPointToPolygonEx(linePt2, ptArray, &ptTemp2);
		//	 if(dTempDis < dDis)
		if ((dTempDis - dDis) < -MathEx::TOL)
		{
			dDis = dTempDis;
			*pPoint1 = ptTemp1;
			*pPoint2 = ptTemp2;
		}
	}
	else
	{
		for (int i = 0; i < nCount - 1; i++)
		{
			dTempDis = DLineSegToLineSegEx(linePt1, linePt2, ptArray[i], ptArray[i + 1], NULL, NULL); // 不求最短距离点对是可以提高效率的
			//	 if(dTempDis < dDis)
			if ((dTempDis - dDis) < -MathEx::TOL)
			{
				dDis = dTempDis;
			}
		}

		dTempDis = DPointToPolygonEx(linePt1, ptArray, NULL);
		//	 if(dTempDis < dDis)
		if ((dTempDis - dDis) < -MathEx::TOL)
		{
			dDis = dTempDis;
		}

		dTempDis = DPointToPolygonEx(linePt2, ptArray, NULL);
		//	 if(dTempDis < dDis)
		if ((dTempDis - dDis) < -MathEx::TOL)
		{
			dDis = dTempDis;
		}
	}

	return dDis;
}

double DLineSegToTriangle(Vertex3d& linePt1, Vertex3d& linePt2, Vertex3d& planePt1, Vertex3d& planePt2, Vertex3d& planePt3, Vertex3d* pPoint1, Vertex3d* pPoint2)
{
	return sqrt(DLineSegToTriangleEx(linePt1, linePt2, planePt1, planePt2, planePt3, pPoint1, pPoint2));
}

double DPointToTriangle(Vertex3d& pt3d, Vertex3d& planePt1, Vertex3d& planePt2, Vertex3d& planePt3, Vertex3d* pResultPoint)
{
	return sqrt(DPointToTriangleEx(pt3d, planePt1, planePt2, planePt3, pResultPoint));
}

double DPointToTriangleEx(Vertex3d& pt3d, Vertex3d& planePt1, Vertex3d& planePt2, Vertex3d& planePt3, Vertex3d* pResultPoint)
{
	Vertex3d tar;

	IsFourPointsInOnePlane(planePt1, planePt2, planePt3, pt3d, &tar);

	if (IsPointInTri(planePt1, planePt2, planePt3, tar))
	{
		if (pResultPoint)
			*pResultPoint = tar;
		return DPointToPointEx(pt3d, tar);
	}

	else
	{
		double d[3];
		Vertex3d pt[3];

		d[0] = DPointToLineSegEx(pt3d, planePt1, planePt2, &pt[0]);
		d[1] = DPointToLineSegEx(pt3d, planePt1, planePt3, &pt[1]);
		d[2] = DPointToLineSegEx(pt3d, planePt2, planePt3, &pt[2]);

		int iIndex = 0;
		double dDis = d[0];
		//		 if(dDis > d[1])
		if ((dDis - d[1]) > MathEx::TOL)
		{
			dDis = d[1];
			iIndex = 1;
		}
		//		 if(dDis > d[2])
		if ((dDis - d[2]) > MathEx::TOL)
		{
			dDis = d[2];
			iIndex = 2;
		}

		if (pResultPoint)
		{
			*pResultPoint = pt[iIndex];
		}

		return dDis;
	}

}

double DPointToPolygon(Vertex3d& pt3d, vector<Vertex3d>& vPts, Vertex3d* pResultPoint)
{
	return sqrt(DPointToPolygonEx(pt3d, vPts, pResultPoint));
}

double DPointToPolygonEx(Vertex3d& pt3d, vector<Vertex3d>& vPts, Vertex3d* pResultPoint)
{
	EnsureClose(vPts);
	int nCount = vPts.size();
	if (nCount < 3)
		return -1;

	Vertex3d vNormal;
	int nIndex = GetPointsNormal(vPts, vNormal);
	if (nIndex == -1)	// 所有点共线
		return -1;

	PLANE3D plane;
	GetPlaneEquation(vNormal, vPts[0], plane);

	Vertex3d tar;
	IsPointInPlane(pt3d, plane, &tar);

	if (IsPointInPolygon(tar, vPts))
	{
		if (pResultPoint)
			*pResultPoint = tar;
		return DPointToPointEx(pt3d, tar);
	}
	else
	{
		double dMin;
		for (int i = 0; i<nCount - 1; i++)
		{
			double dDis = DPointToLineSegEx(pt3d, vPts[i], vPts[i + 1], &tar);
			if (i == 0)
			{
				if (pResultPoint)
					*pResultPoint = tar;
				dMin = dDis;
			}
			else
			{
				//	  if(dMin > dDis)
				if ((dMin - dDis) > MathEx::TOL)
				{
					dMin = dDis;
					if (pResultPoint)
						*pResultPoint = tar;
				}
			}
		}
		return (dMin);
	}
}

double DPointToLine(Vertex3d& pt, Vertex3d& LineStr, Vertex3d& LineEnd, Vertex3d* pFootPt)
{
	return sqrt(DPointToLineEx(pt, LineStr, LineEnd, pFootPt));
}

double DPointToLineEx(Vertex3d& pt, Vertex3d& LineStr, Vertex3d& LineEnd, Vertex3d* pFootPt)
{
	Vertex3d temp1 = LineEnd - LineStr;
	Vertex3d temp2 = pt - LineStr;

	double c1 = temp1 * temp2;
	double c2 = temp1 * temp1;

	if (fabs(c2) < 1e-6)	// 直线起点和终点相同
	{
		if (pFootPt)
			*pFootPt = LineStr;
		return DPointToPointEx(pt, LineStr);
	}

	double para = c1 / c2;
	Vertex3d footPt = LineStr + para * temp1;

	if (pFootPt)
		*pFootPt = footPt;

	return DPointToPointEx(pt, footPt);
}

double DLineSegToLineSegEx(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, Vertex3d *pPair1, Vertex3d *pPair2)
{
	Vertex3d PQ, AB, AP, AQ;
	PQ = Line1End - Line1Str;
	AB = Line2End - Line2Str;
	AP = Line1Str - Line2Str;
	AQ = Line1End - Line2Str;
	Vertex3d n = AB ^ PQ;
	double dis;

	if (Vertex3d(0, 0, 0) != n)	// 先求距离
	{
		// 如果两条线段共面不相交，那么AP*n=0，距离为0
		double temp = (AP * n) *  (AP * n);
		dis = temp / (n * n);

		Vertex3d AA, BB;
		if (fabs(dis) < MathEx::TOL)
		{
			AA = Line2Str;
			BB = Line2End;
		}
		else
		{
			PLANE3D plane;
			GetPlaneEquationByParallelLine(Line1Str, Line1End, Line2Str, Line2End, plane);
			IsPointInPlane(Line2Str, plane, &AA);
			IsPointInPlane(Line2End, plane, &BB);
		}

		Vertex3d intsectPt;
		char cRes = RLineSegWithLineSeg(Line1Str, Line1End, AA, BB, 1, &intsectPt);	// 线段求交

		if (cRes == '1')
		{
			if (pPair1 && pPair2)
			{
				*pPair1 = intsectPt;
				Vertex3d vVec = Line2Str - AA;
				vVec.normalize();

				*pPair2 = *pPair1 + vVec * sqrt(dis);
			}
			return dis;
		}
	}

	// 如果两条线段平行，那么也是比较任意一个端点到另一条线段的最短距离
	double dDis[4];
	Vertex3d ptPoints1[4];
	Vertex3d ptPoints2[4];

	ptPoints1[0] = Line1Str;
	dDis[0] = DPointToLineSegEx(ptPoints1[0], Line2Str, Line2End, &ptPoints2[0]);

	ptPoints1[1] = Line1End;
	dDis[1] = DPointToLineSegEx(ptPoints1[1], Line2Str, Line2End, &ptPoints2[1]);

	ptPoints2[2] = Line2Str;
	dDis[2] = DPointToLineSegEx(ptPoints2[2], Line1Str, Line1End, &ptPoints1[2]);

	ptPoints2[3] = Line2End;
	dDis[3] = DPointToLineSegEx(ptPoints2[3], Line1Str, Line1End, &ptPoints1[3]);

	dis = dDis[0];
	int iIndex = 0;

	for (int i = 1; i < 4; i++)
	{
		if (dDis[i] < dis)
		{
			dis = dDis[i];
			iIndex = i;
		}
	}

	if (pPair1 && pPair2)
	{
		*pPair1 = ptPoints1[iIndex];
		*pPair2 = ptPoints2[iIndex];
	}

	return dis;

}

double DLineSegToLineSeg(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, Vertex3d *pPair1, Vertex3d *pPair2)
{
	return sqrt(DLineSegToLineSegEx(Line1Str, Line1End, Line2Str, Line2End, pPair1, pPair2));
}

double DLineToLine(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, Vertex3d *pPair1, Vertex3d *pPair2)
{
	Vertex3d PQ, AB, AP, AQ;
	PQ = Line1End - Line1Str;
	AB = Line2End - Line2Str;
	AP = Line1Str - Line2Str;
	AQ = Line1End - Line2Str;
	Vertex3d n = AB ^ PQ;
	double dis;

	if (Vertex3d(0, 0, 0) != n)
	{
		double temp = (AP * n) *  (AP * n);
		dis = temp / (n * n);
		dis = sqrt(dis);

		if (pPair1 && pPair2)
		{
			PLANE3D plane;
			GetPlaneEquation(n, Line1Str, plane);
			Vertex3d AA, BB;
			IsPointInPlane(Line2Str, plane, &AA);
			IsPointInPlane(Line2End, plane, &BB);

			RLineWithLine(Line1Str, Line1End, AA, BB, 1, pPair1);

			Vertex3d vVec = Line2Str - AA;
			vVec.normalize();
			*pPair2 = *pPair1 + vVec * dis;
		}
	}
	else // 平行的情况下，则随意取点对上的端点
	{
		Vertex3d vPt;
		dis = DPointToLine(Line2Str, Line1End, Line1Str, &vPt);
		if (pPair1 && pPair2)
		{
			*pPair1 = Line1Str;
			*pPair2 = Line2Str;
		}
	}

	return dis;
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
char RPointWithPlane(Vertex3d& pt, PLANE3D& plane)
{
	// 注意，这里的意思是指点是否在平面朝向的一侧，这里的朝向就是ABC，若在，表示，点在面内，否则在面内
	Vertex3d vNor(plane.A, plane.B, plane.C);
	double dRes = pt * vNor + plane.D;
	if (fabs(dRes) < MathEx::TOL)
		return '2';
	else if (dRes > MathEx::TOL)
		return '1';
	else
		return '0';
}

char RFrustumWithPolygon(FrustumImpl& frustum, vector<Vertex3d>& PtsArray)
{
	EnsureOpen(PtsArray);
	if (PtsArray.size() < 3) return '0';

	int i = 0;
	for (i = 1; i < PtsArray.size() - 1; i++)
		if (RFrustumWithTriangle(frustum, PtsArray.at(0), PtsArray.at(i), PtsArray.at(i + 1)) == '1')
			return '1';

	return '0';
}

char RFrustumWithTriangle(FrustumImpl& frustum, Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3)
{
	Vertex3d triVertices[3] = { triPt1, triPt2, triPt3 };

	// box判断问题
	Envelope3d env;
	for (int i = 0; i < 3; i++)
	{
		env.merge(triVertices[i]);
	}

	double radius = env.getDiagonal() / 2;
	Vertex3d boxCen = env.center();

	// 最外层的包围球相交情况
	if (0 == IsSphereIntersectSphere(boxCen, radius, frustum.sphCen, frustum.radius))
		return '0';

	// frustum的外包cone与sphere相交情况
	if ('0' == RConeWithSphere(frustum.coneFov, frustum.coneCen, frustum.coneDir, boxCen, radius))
		return '0';

	// frustum开始
	if ('0' == RFrustumWithSphere(frustum, boxCen, radius))
		return '0';

	// 与AABB判断
	if ('0' == RFrustumWithEnvelope(frustum, env))
		return '0';

	//return '1';

	Vertex3d triVec[3];
	triVec[0] = triPt2 - triPt1;
	triVec[1] = triPt3 - triPt2;
	triVec[2] = triPt1 - triPt3;

	Vertex3d fruVec[4];
	fruVec[0] = frustum.vPlaneVertices[0] - frustum.vPlaneVertices[1]; // 平行方向 
	fruVec[1] = frustum.vPlaneVertices[0] - frustum.vPlaneVertices[3];
	fruVec[2] = frustum.vPlaneVertices[0] - frustum.vPlaneVertices[4];
	fruVec[3] = frustum.vPlaneVertices[1] - frustum.vPlaneVertices[1];

	int nAxisCount = 19;
	vector<Vertex3d> axisArray;
	axisArray.reserve(nAxisCount);

	Vertex3d vTriNor = triVec[0] ^ triVec[1];
	vTriNor.normalize();

	axisArray.push_back(vTriNor);

	for (int i = 0; i < 6; i++)
		axisArray.push_back(frustum.vPlaneNor[i]);

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 4; j++)
		{
			Vertex3d vNor = triVec[i] ^ fruVec[j];
			vNor.normalize();
			axisArray.push_back(vNor);
		}

	double min1, min2, max1, max2;


	for (int i = 0; i < nAxisCount; i++)
	{
		// 对于每个轴，只要能够确认其是分离轴，那么就是不相交的
		max1 = min1 = axisArray[i] * triVertices[0];
		for (int j = 1; j<3; j++)
		{
			double pos = axisArray[i] * triVertices[j];
			//	if(min1 > pos)
			if ((min1 - pos) > MathEx::TOL)
				min1 = pos;
			//	if(max1 < pos)
			if ((max1 - pos) < -MathEx::TOL)
				max1 = pos;
		}

		max2 = min2 = axisArray[i] * frustum.vPlaneVertices[0];
		for (int j = 1; j<8; j++)
		{
			double pos = axisArray[i] * frustum.vPlaneVertices[j];
			//	if(min2 > pos)
			if ((min2 - pos) > MathEx::TOL)
				min2 = pos;
			//	if(max2 < pos)
			if ((max2 - pos) < -MathEx::TOL)
				max2 = pos;
		}

		if ((min1 - max2) > MathEx::TOL || (max1 - min2) < -MathEx::TOL)
			return '0';
	}

	return '1';
}

char RFrustumWithEnvelope(FrustumImpl& frusum, Envelope3d& env)
{
	Vertex3d ptMin = env.getMin();
	Vertex3d ptVertexP;
	Vertex3d ptVertexN;

	double xSize, ySize, zSize;
	xSize = env.lengthX();
	ySize = env.lengthY();
	zSize = env.lengthZ();

	if (env.recPlaneIndex == -1)
	{
		for (int i = 0; i < 6; i++)
		{
			ptVertexN.x = ptMin.x + xSize * frusum.vPlaneNorSign[i][0];
			ptVertexN.y = ptMin.y + ySize * frusum.vPlaneNorSign[i][1];
			ptVertexN.z = ptMin.z + zSize * frusum.vPlaneNorSign[i][2];

			if ('0' == RPointWithPlane(ptVertexN, frusum.plane[i]))
			{
				env.recPlaneIndex = i;
				return '0';
			}
			ptVertexP.x = ptMin.x + xSize * (!frusum.vPlaneNorSign[i][0]);
			ptVertexP.y = ptMin.y + ySize * (!frusum.vPlaneNorSign[i][1]);
			ptVertexP.z = ptMin.z + zSize * (!frusum.vPlaneNorSign[i][2]);

			if ('0' == RPointWithPlane(ptVertexP, frusum.plane[i]))
			{
				return '2';
			}
		}

		return '1';
	}
	else
	{
		int nRec[6] = { 0, 1, 2, 3, 4, 5 };
		nRec[0] = env.recPlaneIndex;
		for (int i = 0; i < env.recPlaneIndex; i++)
			nRec[i + 1] = i;

		for (int j = 0; j < 6; j++)
		{
			int i = nRec[j];
			ptVertexN.x = ptMin.x + xSize * frusum.vPlaneNorSign[i][0];
			ptVertexN.y = ptMin.y + ySize * frusum.vPlaneNorSign[i][1];
			ptVertexN.z = ptMin.z + zSize * frusum.vPlaneNorSign[i][2];

			if ('0' == RPointWithPlane(ptVertexN, frusum.plane[i]))
			{
				env.recPlaneIndex = i;
				return '0';
			}

			ptVertexP.x = ptMin.x + xSize * (!frusum.vPlaneNorSign[i][0]);
			ptVertexP.y = ptMin.y + ySize * (!frusum.vPlaneNorSign[i][1]);
			ptVertexP.z = ptMin.z + zSize * (!frusum.vPlaneNorSign[i][2]);

			if ('0' == RPointWithPlane(ptVertexP, frusum.plane[i]))
			{
				env.recPlaneIndex = -1;
				return '2';// 相交
			}
		}

		env.recPlaneIndex = -1;
		return '1';
	}


	// ===========================================================
	// = 算法一

	// 	Vertex3d ptCorner;
	// 	env.GetVertices(ptCorner);
	// 	int iTotalIn = 0;
	// 	for(int i=0; i<6; i++)
	// 	{
	// 		int iInCount = 8;
	// 		int iPtIn = 1;		
	// 		for(int j = 0; j < 8; ++j) 
	// 		{			// test this point against the planes
	// 			if('0' == RPointWithPlane(ptCorner[j],frusum.plane[i]))
	// 			{
	// 				iPtIn = 0;
	// 				--iInCount;
	// 			}
	// 		}		// were all the points outside of plane p?
	// 		if(iInCount == 0)
	// 			return '0';		// check if they were all on the right side of the plane
	// 		iTotalIn += iPtIn;
	// 	}
	// 
	// 	if(iTotalIn == 6)
	// 		return '1';	// 在其内部
	// 
	// 	return '2'; // 相交 
}

char RFrustumWithSphere(FrustumImpl& frusum, Vertex3d& cen, double radius)
{
	// various distances
	double fDistance;	// calculate our distances to each of the planes
	for (int i = 0; i < 6; ++i)
	{
		// find the distance to this plane
		PLANE3D plane = frusum.plane[i];
		// 计算有向距离   注意plane的法向量已经被归一化；减小运算量
		fDistance = plane.A * cen.x + plane.B * cen.y + plane.C * cen.z + plane.D;

		//	if(fDistance < -radius)
		if ((fDistance + radius) < -MathEx::TOL)
			return '0';		// else if the distance is between +- radius, then we intersect
		//	if(fabs(fDistance) < radius)
		if ((fabs(fDistance) - radius) < -MathEx::TOL)
			return '2';
	}	// otherwise we are fully in view

	return '1';
}

int IsSphereIntersectSphere(Vertex3d& cen1, double radius1, Vertex3d& cen2, double radius2)
{
	double dCen = DPointToPointEx(cen1, cen2);
	double dRad = radius1 + radius2;
	if (dCen - dRad * dRad < -MathEx::TOL)
		return 1;
	else
		return 0;
}

int IsConeIntersectSphere(double dRadian, Vertex3d& vStart, Vertex3d& vDir, Vertex3d& shpCen, double radius)
{
	Vertex3d vVec = shpCen - vStart;
	vDir.normalize();
	double a = vVec * vDir;
	double temp = a * a - (vVec * vVec) * cos(dRadian) * cos(dRadian);
	if (temp < -MathEx::TOL)
		return 1;
	else
		return 0;
}

char RConeWithSphere(double dRadian, Vertex3d& vStart, Vertex3d& vDir, Vertex3d& shpCen, double radius)
{
	// 为了提高速度，这里的vDir需要预先就已经处理过了
	Vertex3d vVec = shpCen - vStart;
	//vDir.normalize();
	double a = vVec * vDir;
	double b = a * tan(dRadian);
	double c = sqrt(vVec*vVec - a*a); // 球心到圆锥轴线的距离
	double d = c - b;
	double e = d * cos(dRadian);

	if (e - radius > MathEx::TOL)
		return '0';	// 在之外
	else if (e - radius < -MathEx::TOL)
		return '1'; // 在之内
	else
		return '2'; // 相交
}


int IsFourPointsInOnePlane(Vertex3d &p1, Vertex3d &p2, Vertex3d &p3, Vertex3d& p4, Vertex3d* pFootPt)
{
	if (p1 == p4 || p2 == p4 || p3 == p4)
	{
		if (pFootPt)
			*pFootPt = p4;
		return 1;
	}

	PLANE3D plane1;
	int bRes = GetPlaneEquation(p1, p2, p3, plane1);
	if (!bRes)	// 存在共线情况
	{
		IsPointInLine(p4, p1, p2, pFootPt);	// 求出垂足
		return 1;
	}
	if (!IsPointInPlane(p4, plane1, pFootPt))	// 不在同一平面,求该点在平面1的垂足
		return 0;

	return 1;
}

int IsPointInTri(Vertex3d& pt1, Vertex3d& pt2, Vertex3d& pt3, Vertex3d& vPt, int bOnPlane)
{
	Vertex3d tempPt;

	if (!bOnPlane)
		if (!IsFourPointsInOnePlane(pt1, pt2, pt3, vPt, &tempPt)) return 0;

	Vertex3d temp1 = (vPt - pt1) ^ (pt2 - pt1);
	Vertex3d temp2 = (vPt - pt2) ^ (pt3 - pt2);
	Vertex3d temp3 = (vPt - pt3) ^ (pt1 - pt3);

	if (temp1 * temp2 > -MathEx::TOL && temp2 * temp3 > -MathEx::TOL  && temp1 * temp3 > -MathEx::TOL)
		return 1;

	return 0;
}

char RPointWithTri(Vertex3d& pt1, Vertex3d& pt2, Vertex3d& pt3, Vertex3d& vPt, bool bOnPlane)
{
	Vertex3d pt;
	Vertex3d tempPt;
	if (vPt == pt1 || vPt == pt2 || vPt == pt3)
		return 'V';
	// 	else if(IsPointInLineSeg(vPt,pt1,pt2,tempPt) || IsPointInLineSeg(vPt,pt2,pt3,tempPt) || IsPointInLineSeg(vPt,pt1,pt3,tempPt))
	// 		return 'E';

	if (!bOnPlane)
		if (!IsFourPointsInOnePlane(pt1, pt2, pt3, vPt, &pt)) return '0'; // out

	Vertex3d temp1 = (vPt - pt1) ^ (pt2 - pt1);
	Vertex3d temp2 = (vPt - pt2) ^ (pt3 - pt2);
	Vertex3d temp3 = (vPt - pt3) ^ (pt1 - pt3);

	double dRes1 = temp1 * temp2;
	double dRes2 = temp2 * temp3;
	double dRes3 = temp1 * temp3;
	//	if(dRes1 >= 0 && dRes2 >= 0  && dRes3 >= 0)
	if (dRes1 > -MathEx::TOL && dRes2 > -MathEx::TOL && dRes3 > -MathEx::TOL)
	{
		if (fabs(dRes1) < 1e-6 || fabs(dRes2) < 1e-6 || fabs(dRes3) < 1e-6)
			return 'E';
		else
			return 'F';
	}

	return '0';
}


int IsPointInLineSeg(Vertex3d& pt, Vertex3d& pt1, Vertex3d& pt2, Vertex3d* pFootPt)
{
	double dd = DPointToLineSegEx(pt, pt1, pt2, pFootPt);
	if (sqrt(dd) < MathEx::TOL)
		return 1;
	else
		return 0;
}

int IsPointInRay(Vertex3d& pt, Vertex3d& pt1, Vertex3d& pt2, Vertex3d* pFootPt)
{
	double dd = DPointToRayEx(pt, pt1, pt2, pFootPt);
	if (sqrt(dd) < MathEx::TOL)
		return 1;
	else
		return 0;
}

int IsPointInLine(Vertex3d& pt, Vertex3d& pt1, Vertex3d& pt2, Vertex3d* pFootPt)
{
	double dd = DPointToLineEx(pt, pt1, pt2, pFootPt);
	if (sqrt(dd) < MathEx::TOL)
		return 1;
	else
		return 0;
}

int IsPointInPolygon(Vertex3d& ptPoint, vector<Vertex3d>& vPts)
{
	char cRes = RPointWithPolygon(ptPoint, vPts);
	if (cRes == 'V' || cRes == 'E' || cRes == 'i')
		return 1;
	else
		return 0;
}
int RPolygonWithPolygon2D(vector<Vertex3d>& ptAs, vector<Vertex3d>& ptBs)
{
	// 判断包容矩形相交情况
	Envelope3d envA, envB, IntersectBox;
	long i = 0, j = 0;
	envA.merge(ptAs);
	envB.merge(ptBs);
	if (envA.test(envB, 'z', &IntersectBox) == 0) // 不相交
		return 0; // 相离
	else // 精细判断
	{
		EnsureClose(ptAs);
		EnsureClose(ptBs);
		vector<Envelope3d> envAs, envBs;
		for (i = 0; i < ptAs.size() - 1; i++)
		{
			Envelope3d tempEnv;
			tempEnv.merge(ptAs[i]);
			tempEnv.merge(ptAs[i + 1]);
			envAs.push_back(tempEnv);
		}
		for (i = 0; i < ptBs.size() - 1; i++)
		{
			Envelope3d tempEnv;
			tempEnv.merge(ptBs[i]);
			tempEnv.merge(ptBs[i + 1]);
			envBs.push_back(tempEnv);
		}

		// 边界相交检测：如有交边，则为相交
		for (i = 0; i < envAs.size(); i++)
			for (j = 0; j < envBs.size(); j++)
			{
				if (envAs[i].test(envBs[j]) == 0)
					continue;
				else
				{
					if ('1' == RLineSegWithLineSeg2d(ptAs[i], ptAs[i + 1], ptBs[j], ptBs[j + 1])) // 相交 // xx_modify
						return 1; // 相交
					else
						continue;
				}
			}

		// 顶点检测：无交边情况下，相互检测顶点位置

		// 暂时用三维函数，需要预处理
		vector<Vertex3d> CopyptAs, CopyptBs;
		CopyptAs.resize(ptAs.size());
		CopyptBs.resize(ptBs.size());
		for (i = 0; i < ptAs.size(); i++)
		{
			CopyptAs[i] = ptAs[i];
			CopyptAs[i].z = 0.0;
		}
		for (i = 0; i < ptBs.size(); i++)
		{
			CopyptBs[i] = ptBs[i];
			CopyptBs[i].z = 0.0;
		}

		if (IsPointInPolygon(CopyptAs[0], CopyptBs) == 1) // 内
			return 2; // 包含
		else
		{
			if (IsPointInPolygon(CopyptBs[0], CopyptAs) == 1)
				return 2; // 包含
			else
				return 0;
		}
	}
}

char  RPointWithPolygon(Vertex3d& ptPoint, vector<Vertex3d>& vPts, Vertex3d *pRay)
{
	vector<Vertex3d> vTempPts;
	vTempPts.insert(vTempPts.end(), vPts.begin(), vPts.end());
	//vTempPts = vPts;

	EnsureClose(vTempPts);
	EnsureNoRedunant(vTempPts);
	int nCount = vTempPts.size();
	if (nCount < 3)
		return '?';

	Vertex3d vNormal;
	int nIndex = GetPointsNormal(vTempPts, vNormal);
	if (nIndex == -1)	// 所有点共线
		return '?';

	Envelope3d env;
	env.merge(vTempPts);
	if (!env.test(ptPoint))
		return 'o';

	PLANE3D plane;
	GetPlaneEquation(vNormal, vTempPts[0], plane);
	Vertex3d tempPt;
	if (!IsPointInPlane(ptPoint, plane, &tempPt))
		return 'o';

	char code = '?';
	int crossings = 0;
	Vertex3d r;
	double radius = env.getDiagonal();

	Vertex3d tempPt1;
	for (int k = 0; k < nCount - 1; k++)
	{
		Vertex3d vPtS = vTempPts[k];
		Vertex3d vPtE = vTempPts[k + 1];
		if (vPtS == ptPoint || vPtE == ptPoint)
			return 'V';
		else if (IsPointInLineSeg(ptPoint, vPtS, vPtE, &tempPt1))
			return 'E';
	}

loop:
	while (1)
	{
		crossings = 0;
		GetRandomRayInPlane(r, radius, plane);
		r = ptPoint + r;

		for (int k = 0; k < nCount - 1; k++)
		{
			Vertex3d vPtS = vTempPts[k];
			Vertex3d vPtE = vTempPts[k + 1];

			code = RRayWithLineSeg(ptPoint, r, vPtS, vPtE, 0, &tempPt);

			if (code == '1')		// 有交点
			{
				if (tempPt == vPtS || tempPt == vPtE)
					code = 'v';
				else
					code = 'f';
			}
			else if (code == '3')
				goto loop;

			// 对各种code进行判断
			if (code == 'v')
				goto loop;
			else if (code == 'f')
				crossings++;

		}
		break;
	}
	if (pRay)
		*pRay = r - ptPoint;

	if ((crossings % 2) == 1)	// 奇数次相交
		return   'i';
	else return 'o';
}

char RPointWithPolygon2d(Vertex3d& ptPoint, vector<Vertex3d>& vPts)
{
	// 暂时用3d方法——xx!!

	//EnsureClose(vPts);
	//int nCount = vPts.size();
	//if(nCount < 3)
	//	return '?';
	//
	//// 确保polygon已经是逆时针的	
	//for(int i=0; i<nCount-1; i++)
	//{
	//	Vertex3d pt1 = vPts[i] - ptPoint;
	//	Vertex3d pt2 = vPts[i+1] - ptPoint;
	//	if((pt1.x * pt2.y - pt1.y * pt2.x) < -1e-6) // 不在多边形内
	//		return 'o';
	//}

	//return 'i'; // 在多边形内或者边界上

	Vertex3d propt;
	std::vector<Vertex3d> provPts;

	propt.x = ptPoint.x;
	propt.y = ptPoint.y;
	propt.z = 0.0;

	provPts.resize(vPts.size());
	for (long i = 0; i < vPts.size(); i++)
	{
		provPts[i].x = vPts[i].x;
		provPts[i].y = vPts[i].y;
		provPts[i].z = 0.0;
	}

	char rrr = RPointWithPolygon(propt, provPts);
	return rrr;
}
char RRectWithPolygon2d(Vertex3d& ptMin, Vertex3d& ptMax, vector<Vertex3d>& vPts)
{
	EnsureClose(vPts);
	int nCount = vPts.size();
	if (nCount < 3)
		return '?';

	double xSize = ptMax.x - ptMin.x;
	double ySize = ptMax.y - ptMin.y;
	double dx1, dy1, dx2, dy2;

	// 确保polygon已经是逆时针的	
	for (int i = 0; i < nCount - 1; i++)
	{
		double signNormalX = vPts[i].y - vPts[i + 1].y;
		double signNormalY = vPts[i + 1].x - vPts[i].x;
		Vertex3d ptPointN;
		ptPointN.x = ptMin.x + xSize * (signNormalX > -MathEx::TOL);
		ptPointN.y = ptMin.y + ySize * (signNormalY > -MathEx::TOL);

		dx1 = vPts[i].x - ptPointN.x;
		dy1 = vPts[i].y - ptPointN.y;
		dx2 = vPts[i + 1].x - ptPointN.x;
		dy2 = vPts[i + 1].y - ptPointN.y;

		if ((dx1 * dy2 - dx2 * dy1) < -1e-6)
			return '0';

		Vertex3d ptPointP;
		ptPointP.x = ptMin.x + xSize * !(signNormalX > -MathEx::TOL);
		ptPointP.y = ptMin.y + ySize * !(signNormalY > -MathEx::TOL);

		dx1 = vPts[i].x - ptPointP.x;
		dy1 = vPts[i].y - ptPointP.y;
		dx2 = vPts[i + 1].x - ptPointP.x;
		dy2 = vPts[i + 1].y - ptPointP.y;

		if ((dx1 * dy2 - dx2 * dy1) < -1e-6)
			return '2'; //  相交
	}

	return '1'; // 在其内部

	// 劣级算法
	// 	 EnsureClose(vPts);
	// 	 int nCount = vPts.size();
	// 	 if(nCount < 3)
	// 		 return '?';
	// 
	// 	 double xSize = ptMax.x - ptMin.x;
	// 	 double ySize = ptMax.y - ptMin.y;
	// 	 vector<Vertex3d> vRectPts;
	// 	 vRectPts.push_back(ptMin);
	// 	 vRectPts.push_back(Vertex3d(ptMin.x + xSize,ptMin.y,0));
	// 	 vRectPts.push_back(Vertex3d(ptMin.x ,ptMin.y + ySize,0));
	// 	 vRectPts.push_back(Vertex3d(ptMin.x + xSize,ptMin.y + ySize,0));
	// 
	// 	 double dx1,dy1,dx2,dy2;
	// 	 
	// 	 
	// 	 // 确保polygon已经是逆时针的	
	// 	 int nTotalIn = 0;
	// 	 for(int i=0; i<nCount-1; i++)
	// 	 {
	// 		int nRec = 0;
	// 		int nInCount = 1;
	// 		for(int j=0; j<4; j++)
	// 		{
	// 			dx1 = vPts[i].x - vRectPts[j].x;
	// 			dy1 = vPts[i].y - vRectPts[j].y;
	// 			dx2 = vPts[i+1].x - vRectPts[j].x;
	// 			dy2 = vPts[i+1].y - vRectPts[j].y;
	// 			if((dx1 * dy2 - dx2 * dy1) < -1e-6) 
	// 			{
	// 				nInCount = 0;
	// 				nRec ++;
	// 			}
	// 		}
	// 		if(nRec == 4) // 四点都在外面
	// 			return '0';
	// 
	// 		nTotalIn += nInCount;
	// 	 }
	// 	
	// 	if(nTotalIn == nCount - 1)
	// 	    return '1'; // 在其内部
	// 	else 
	// 		return '2';
}

char RLineSegCrossTri(Vertex3d& LineStr, Vertex3d& LineEnd, Vertex3d& pt1, Vertex3d& pt2, Vertex3d& pt3)
{
	PLANE3D plane;
	int bRes = GetPlaneEquation(pt1, pt2, pt3, plane);
	if (!bRes)
		return '?';

	Vertex3d tempPt;
	char cRes = RLineSegWithPlane(LineStr, LineEnd, plane, &tempPt);

	if (cRes == '1' || cRes == 'e' || cRes == 's')	// 与平面的交点和三角形有点或者线重合
	{
		return RPointWithTri(pt1, pt2, pt3, tempPt);
	}
	return '0';
}

char RRayCrossTri(Vertex3d& LineStr, Vertex3d& LineEnd, Vertex3d& pt1, Vertex3d& pt2, Vertex3d& pt3)
{
	PLANE3D plane;
	int bRes = GetPlaneEquation(pt1, pt2, pt3, plane);
	if (!bRes)
		return '?';

	Vertex3d tempPt;
	char cRes = RRayWithPlane(LineStr, LineEnd, plane, &tempPt);
	char cResEx = RPointWithTri(pt1, pt2, pt3, tempPt);
	if (cRes == 's')	// 射线起点与三角形可能有重合
		return cResEx;
	else if (cRes == '1' || cRes == 'e')	// 射线的另一点或者与平面的交点，可能和三角形有点或线重合
	{
		if (cResEx == 'V')
			cRes = 'v';
		else if (cResEx == 'E')
			cRes = 'e';
		else if (cResEx == 'F')
			cRes = 'f';
		else if (cResEx == '0')
			cRes = '0';
	}
	else
		cRes = '0';
	return cRes;
}

int IsRayCrossEnvelope(RayImpl& r, const Envelope3d& env, double t0, double t1)
{
	// 算法1：与6个面进行相交检测 效率低下

	// ======================================================
	// 	PLANE3D plane[6] = {{0,0,1,-env.MaxZ()},{0,0,1,-env.MinZ()},{0,1,0,-env.MaxY()},{0,1,0,env.MinY()},
	// 	{1,0,0,env.MinX()},{1,0,0,env.MaxX()}};
	// 
	// 	double planeScope[3][4] = {{env.MinX(),env.MaxX(),env.MinY(),env.MaxY()}, 
	// 				{env.MinX(),env.MaxX(),env.MinZ(),env.MaxZ()},  
	// 				{env.MinY(),env.MaxY(),env.MinZ(),env.MaxZ()}};
	// 	
	// 	// 只判断0，1情况，其他复杂情况不判断，因为只用在点选里面
	// 	for(int i=0; i<6; i++)
	// 	{
	// 		Vertex3d tempPt;
	// 		char cRes = RLineSegWithPlane(LineStr,LineEnd,plane[i],&tempPt);
	// 		if(cRes != '0')
	// 		{
	// 			double tempPtCoors[6] = {tempPt.x,tempPt.y, tempPt.x,tempPt.z, tempPt.y,tempPt.z};
	// 			int j = i / 2;
	// 			double Tol = MathEx::TOL;
	// 			if((tempPtCoors[j] - planeScope[j][0]) > -Tol && (tempPtCoors[j] - planeScope[j][1]) < Tol &&
	// 				(tempPtCoors[j+1] - planeScope[j][2]) > -Tol && (tempPtCoors[j+1] - planeScope[j][3]) < Tol)
	// 
	// 				return 1;
	// 		}
	// 	}
	// 
	// 	return 0;

	// 算法2：超高效算法

	double tmin, tmax, tymin, tymax, tzmin, tzmax;
	Vertex3d parameters[2] = { env.getMin(), env.getMax() };
	tmin = (parameters[r.sign[0]].x - r.origin.x) * r.inv_direction.x;
	tmax = (parameters[1 - r.sign[0]].x - r.origin.x) * r.inv_direction.x;
	tymin = (parameters[r.sign[1]].y - r.origin.y) * r.inv_direction.y;
	tymax = (parameters[1 - r.sign[1]].y - r.origin.y) * r.inv_direction.y;
	if ((tmin > tymax) || (tymin > tmax))
		return 0;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;
	tzmin = (parameters[r.sign[2]].z - r.origin.z) * r.inv_direction.z;
	tzmax = (parameters[1 - r.sign[2]].z - r.origin.z) * r.inv_direction.z;
	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;
	return ((tmin < t1) && (tmax > t0));

}

char RLineSegWithLineSeg2d(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, Vertex3d* pCrossPt)
{
	// 漂亮的线段求交算法
	double dx1 = Line1End.x - Line1Str.x;
	double dx2 = Line2End.x - Line2Str.x;
	double dy1 = Line1End.y - Line1Str.y;
	double dy2 = Line2End.y - Line2Str.y;
	double cx = Line2Str.x - Line1Str.x;
	double cy = Line2Str.y - Line1Str.y;

	double temp = dx1 * dy2 - dy1 * dx2;
	if (fabs(temp) < 1e-6) // 平行或者重合
		return '3';

	double u = (cx * dy2 - cy * dx2) / temp;
	double v = (cx * dy1 - cy * dx1) / temp;

	if (u > -1e-6 && (1 - u) > -1e-6 && v > -1e-6 && (1 - v) > -1e-6)
	{
		if (pCrossPt)
		{
			pCrossPt->x = Line1Str.x + dx1 * u;
			pCrossPt->y = Line1Str.y + dy1 * u;
			pCrossPt->z = 0;
		}
		return '1'; // 相交
	}
	return '2'; // 不相交
}

char RLineSegWithLineSeg(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, int bOnPlane, Vertex3d* pCrossPt)
{
	Vertex3d v1 = Line1End - Line1Str;
	Vertex3d v2 = Line2End - Line2Str;
	Vertex3d v = v1 ^ v2;

	//平行，显然共面
	if (Vertex3d(0, 0, 0) == v)
		return '3';

	Vertex3d tempPt;

	if (!bOnPlane)
	{
		//异面，不相交
		if (!IsFourPointsInOnePlane(Line1Str, Line1End, Line2End, Line2Str, &tempPt))
			return '0';
	}

	Vertex3d v3 = Line2Str - Line1Str;
	Vertex3d v4 = v3 ^ v2;
	Vertex3d v5 = v1 ^ v2;
	Vertex3d v6 = v3 ^ v1;

	double u = v4.length() / v5.length();
	double x = v6.length() / v5.length();
	if (v4 * v5 < -(MathEx::TOL))
		u = -u;
	if (v6 * v5 < -(MathEx::TOL))
		x = -x;

	// 共面相交
	// 不要写成if(u>=0 && u<=1)
	if (u > -MathEx::TOL && (1 - u) > -MathEx::TOL && x > -MathEx::TOL && (1 - x) > -MathEx::TOL)
	{
		if (pCrossPt)
		{
			Vertex3d crossPt = Line1Str + v1 * u;
			*pCrossPt = crossPt;
		}
		return '1';
	}
	return '2';//共面不相交也不平行
}

char RRayWithLineSeg(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, int bOnPlane, Vertex3d* pCrossPt)
{
	Vertex3d v1 = Line1End - Line1Str;
	Vertex3d v2 = Line2End - Line2Str;
	Vertex3d v = v1 ^ v2;

	//平行，显然共面
	if (Vertex3d(0, 0, 0) == v)
		return '3';

	Vertex3d tempPt;
	if (!bOnPlane)
	{
		//异面，不相交
		if (!IsFourPointsInOnePlane(Line1Str, Line1End, Line2End, Line2Str, &tempPt))
			return '0';
	}
	Vertex3d v3 = Line2Str - Line1Str;
	Vertex3d v4 = v3 ^ v2;
	Vertex3d v5 = v1 ^ v2;
	Vertex3d v6 = v3 ^ v1;

	double u = v4.length() / v5.length();
	double x = v6.length() / v5.length();
	if (v4 * v5 < -(MathEx::TOL))
		u = -u;
	if (v6 * v5 < -(MathEx::TOL))
		x = -x;

	// 共面相交
	// 不要写成if(u>=0 && u<=1) // 注意u的区间
	if (u > -MathEx::TOL && x > -MathEx::TOL && (1 - x) > -MathEx::TOL)
	{
		if (pCrossPt)
		{
			Vertex3d crossPt = Line1Str + v1 * u;
			*pCrossPt = crossPt;
		}
		return '1';
	}
	return '2';//共面不相交也不平行
}

char RRayWithLine(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, int bOnPlane, Vertex3d* pCrossPt)
{
	Vertex3d v1 = Line1End - Line1Str;
	Vertex3d v2 = Line2End - Line2Str;
	Vertex3d v = v1 ^ v2;

	//平行，显然共面
	if (Vertex3d(0, 0, 0) == v)
		return '3';

	Vertex3d tempPt;
	if (!bOnPlane)
	{
		//异面，不相交
		if (!IsFourPointsInOnePlane(Line1Str, Line1End, Line2End, Line2Str, &tempPt))
			return '0';
	}
	Vertex3d v3 = Line2Str - Line1Str;
	Vertex3d v4 = v3 ^ v2;
	Vertex3d v5 = v1 ^ v2;
	Vertex3d v6 = v3 ^ v1;

	double u = v4.length() / v5.length();
	double x = v6.length() / v5.length();
	if (v4 * v5 < -(MathEx::TOL))
		u = -u;
	if (v6 * v5 < -(MathEx::TOL))
		x = -x;

	// 共面相交
	// 不要写成if(u>=0 && u<=1) // 注意u的区间
	if (u > -MathEx::TOL)
	{
		if (pCrossPt)
		{
			Vertex3d crossPt = Line1Str + v1 * u;
			*pCrossPt = crossPt;
		}
		return '1';
	}
	return '2';//共面不相交也不平行
}

char RRayWithRay(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, int bOnPlane, Vertex3d* pCrossPt)
{
	Vertex3d v1 = Line1End - Line1Str;
	Vertex3d v2 = Line2End - Line2Str;
	Vertex3d v = v1 ^ v2;

	//平行，显然共面
	if (Vertex3d(0, 0, 0) == v)
		return '3';

	Vertex3d tempPt;
	if (!bOnPlane)
	{
		//异面，不相交
		if (!IsFourPointsInOnePlane(Line1Str, Line1End, Line2End, Line2Str, &tempPt))
			return '0';
	}
	Vertex3d v3 = Line2Str - Line1Str;
	Vertex3d v4 = v3 ^ v2;
	Vertex3d v5 = v1 ^ v2;
	Vertex3d v6 = v3 ^ v1;

	double u = v4.length() / v5.length();
	double x = v6.length() / v5.length();
	if (v4 * v5 < -(MathEx::TOL))
		u = -u;
	if (v6 * v5 < -(MathEx::TOL))
		x = -x;

	// 共面相交
	// 不要写成if(u>=0 && u<=1) // 注意u的区间
	if (u > -MathEx::TOL && x > -MathEx::TOL)
	{
		if (pCrossPt)
		{
			Vertex3d crossPt = Line1Str + v1 * u;
			*pCrossPt = crossPt;
		}
		return '1';
	}
	return '2';//共面不相交也不平行
}
char RLineSegWithLine(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, int bOnPlane, Vertex3d* pCrossPt)
{
	Vertex3d v1 = Line1End - Line1Str;
	Vertex3d v2 = Line2End - Line2Str;

	if (v1.length() == 0 || v2.length() == 0)
	{
		return '3';
	}
	// xx_0505_modify_for_test:防止线段退化
	for (;;)
	{
		if (v1.length() < MathEx::TOL)
			v1 *= 100;
		else
			break;
	}
	for (;;)
	{
		if (v2.length() < MathEx::TOL)
			v2 *= 100;
		else
			break;
	}

	// xx_0505_modify_for_test

	Vertex3d v = v1 ^ v2;

	//平行，显然共面
	if (Vertex3d(0, 0, 0) == v)
	{
		return '3';
	}

	Vertex3d tempPt;
	if (!bOnPlane)
	{
		//异面，不相交
		if (!IsFourPointsInOnePlane(Line1Str, Line1End, Line2End, Line2Str, &tempPt))
			return '0';
	}
	Vertex3d v3 = Line2Str - Line1Str;
	Vertex3d v4 = v3 ^ v2;
	Vertex3d v5 = v1 ^ v2;
	Vertex3d v6 = v3 ^ v1;

	double u = v4.length() / v5.length();
	double x = v6.length() / v5.length();
	if (v4 * v5 < -(MathEx::TOL))
		u = -u;
	if (v6 * v5 < -(MathEx::TOL))
		x = -x;

	// 共面相交
	// 不要写成if(u>=0 && u<=1)
	if (u > -MathEx::TOL && (1 - u) > -MathEx::TOL)
	{
		if (pCrossPt)
		{
			Vertex3d crossPt = Line1Str + v1 * u;
			*pCrossPt = crossPt;
		}
		return '1';
	}
	return '2';//共面不相交也不平行

}

int IsPointInPlane(Vertex3d& pt, PLANE3D& plane, Vertex3d* pFootPt)
//判断点是否在某一个平面内.
{
	double temp = plane.A * pt.x + plane.B * pt.y + plane.C * pt.z + plane.D;
	if (fabs(temp) > MathEx::TOL)
	{
		double t = (-1) * temp / (plane.A * plane.A + plane.B * plane.B + plane.C * plane.C);
		if (pFootPt)
		{
			pFootPt->x = t * plane.A + pt.x;
			pFootPt->y = t * plane.B + pt.y;
			pFootPt->z = t * plane.C + pt.z;
		}
		return 0;
	}
	else
	{
		if (pFootPt)
			*pFootPt = pt;
		return 1;
	}
}

// char RLineSegWithPlane( Vertex3d& LineStr,Vertex3d& LineEnd,PLANE3D& plane,Vertex3d& pt)
// {
// 	Vertex3d Vn;
// 	GetPlaneNormal(plane,Vn);//平面的法向量
// 	Vertex3d tempPt;
// 	GetOnePointInPlane(plane,tempPt);
// 
// 	double d1,d2;
// 	d1 = (LineStr - tempPt) * Vn;
// 	d2 = (LineEnd - tempPt) * Vn;		//平面上任意一点与线段的起、终点构成的向量
// 
// 	if(fabs(d1) < MathEx::TOL && fabs(d2) < MathEx::TOL)//表示线段在平面上
// 		return '2';
// 
// 	// 若点乘结果异号，则表示线段起点与终点分居平面异侧，即线段与平面相交；
// 	// 若为0，则表示至少线段的起点或者终点位于平面上
// 	// if(d1 * d2 <= 0)
// 	if(d1 * d2 < MathEx::TOL) 
// 	{
// 		RLineWithPlane(LineStr,LineEnd,plane,pt);
// 		return '1';
// 	}
// 	return '0';
// }

char RRayWithPlane(Vertex3d& LineStr, Vertex3d& LineEnd, PLANE3D& plane, Vertex3d* pCrossPt)
{
	Vertex3d Vn;
	GetPlaneNormal(plane, Vn);//平面的法向量
	Vertex3d tempPt;
	GetOnePointInPlane(plane, tempPt);

	double d1, d2;
	d1 = (LineStr - tempPt) * Vn;
	d2 = (LineEnd - tempPt) * Vn;		//平面上任意一点与线段的起、终点构成的向量

	if (fabs(d1) < MathEx::TOL && fabs(d2) < MathEx::TOL)//表示线段在平面上
		return 'p';
	else if (fabs(d1) < MathEx::TOL)
	{
		if (pCrossPt)
			*pCrossPt = LineStr;
		return 's';
	}
	// 	else if(fabs(d2) < MathEx::TOL)
	// 	{
	// 		if(pCrossPt)
	// 			*pCrossPt = LineEnd;
	// 		return 'e';
	// 	}

	// 若点乘结果异号，则表示线段起点与终点分居平面异侧，即线段与平面相交；
	// 若为0，则表示至少线段的起点或者终点位于平面上
	// if(d1 * d2 <= 0)
	//	if(d1 * d2 < MathEx::TOL) 
	{
		Vertex3d tempPt;
		RLineWithPlane(LineStr, LineEnd, plane, &tempPt);
		if (IsPointInRay(tempPt, LineStr, LineEnd, pCrossPt)) // 如果点在射线上
		{
			return '1';
		}

	}
	return '0';
}

char RLineSegWithPlane(Vertex3d& LineStr, Vertex3d& LineEnd, PLANE3D& plane, Vertex3d* pCrossPt)
{
	Vertex3d Vn;
	GetPlaneNormal(plane, Vn);//平面的法向量
	Vertex3d tempPt;
	GetOnePointInPlane(plane, tempPt);

	double d1, d2;
	d1 = (LineStr - tempPt) * Vn;
	d2 = (LineEnd - tempPt) * Vn;		//平面上任意一点与线段的起、终点构成的向量

	if (fabs(d1) < MathEx::TOL && fabs(d2) < MathEx::TOL)//表示线段在平面上
		return 'p';
	else if (fabs(d1) < MathEx::TOL)
	{
		if (pCrossPt)
			*pCrossPt = LineStr;
		return 's';
	}
	else if (fabs(d2) < MathEx::TOL)
	{
		if (pCrossPt)
			*pCrossPt = LineEnd;
		return 'e';
	}

	// 若点乘结果异号，则表示线段起点与终点分居平面异侧，即线段与平面相交；
	// 若为0，则表示至少线段的起点或者终点位于平面上
	// if(d1 * d2 <= 0)
	if (d1 * d2 < MathEx::TOL * MathEx::TOL)
	{
		RLineWithPlane(LineStr, LineEnd, plane, pCrossPt);
		return '1';
	}
	return '0';
}

char RLineWithPlane(Vertex3d& LineStr, Vertex3d& LineEnd, PLANE3D& plane, Vertex3d* pCrossPt)
{
	Vertex3d vLine = LineEnd - LineStr;//直线的方向向量

	double temp = plane.A * vLine.x + plane.B * vLine.y + plane.C * vLine.z;
	// 判断平行或者在面上
	if (fabs(temp) < MathEx::TOL)
	{
		if (IsPointInPlane(LineStr, plane, pCrossPt))
			return '2';
		else
			return '0';
	}

	//根据平面方程Ax+By+Cz+D=0与直线方程(x-x1)/t = (y-y1)/m = (z-z1)/n = u来求解，
	//其中(x1,y1,z1)是直线一点,(t,m,n)是直线的方向向量
	if (pCrossPt)
	{
		double u = -(plane.D + plane.A * LineStr.x + plane.B * LineStr.y + plane.C * LineStr.z) / temp;
		Vertex3d crossPt = vLine * u + LineStr;
		*pCrossPt = crossPt;
	}
	return '1';
}

int IsLineSegCrossTri(Vertex3d& LineStr, Vertex3d& LineEnd, Vertex3d& pt1, Vertex3d& pt2, Vertex3d& pt3, Vertex3d* pCrossPt)
{
	PLANE3D plane;
	int bRes = GetPlaneEquation(pt1, pt2, pt3, plane);
	if (!bRes)
		return 0;

	Vertex3d tempPt;
	//首先判断线段与平面是否相交
	char cRes = RLineSegWithPlane(LineStr, LineEnd, plane, pCrossPt);
	if (cRes == '0' || cRes == 'p')
		return 0;

	//求得线段与平面的交点

	//验证该交点是否在三角形内
	Vertex3d pt4;
	if (IsPointInTri(pt1, pt2, pt3, pt4))
		return 1;
	else
		return 0;
	if (pCrossPt)
		*pCrossPt = pt4;
}

int IsLineSegCrossPlane(Vertex3d& LineStr, Vertex3d& LineEnd, PLANE3D& plane)
{
	Vertex3d vPt;
	char cRes = RLineSegWithPlane(LineStr, LineEnd, plane, &vPt);
	if (cRes == '0')
		return 0;
	else
		return 1;
}

char RLineWithLine(Vertex3d& Line1Str, Vertex3d& Line1End, Vertex3d& Line2Str, Vertex3d& Line2End, int bOnPlane, Vertex3d* pIntersect)
{
	Vertex3d v1 = Line1End - Line1Str;
	Vertex3d v2 = Line2End - Line2Str;
	Vertex3d v = v1 ^ v2;

	//平行，显然共面
	if (Vertex3d(0, 0, 0) == v)
		return '2';

	Vertex3d tempPt;

	//异面，不相交
	if (!bOnPlane)
	{
		if (!IsFourPointsInOnePlane(Line1Str, Line1End, Line2End, Line2Str, &tempPt))
			return '0';
	}

	Vertex3d v3 = Line2Str - Line1Str;
	Vertex3d v4 = v3 ^ v2;
	Vertex3d v5 = v1 ^ v2;

	double u = v4.length() / v5.length();
	if (v4 * v5 < -(1e-6))
		u = -u;

	if (pIntersect)
		*pIntersect = Line1Str + v1 * u;

	return '1';
}

char RPlaneWithPlane(PLANE3D& plane1, PLANE3D& plane2, Vertex3d& pt1, Vertex3d& pt2)
{
	Vertex3d vNormal, vNormal1, vNormal2;
	GetPlaneNormal(plane1, vNormal1);
	GetPlaneNormal(plane2, vNormal2);
	vNormal = vNormal1 ^ vNormal2;

	//面面平行
	if (Vertex3d(0, 0, 0) == vNormal)
	{
		Vertex3d tempPt;
		GetOnePointInPlane(plane1, tempPt);
		Vertex3d pt;
		if (IsPointInPlane(tempPt, plane2, &pt))
			return '2';
		else
			return '0';
	}
	double temp1, temp2, temp3;

	temp1 = plane1.B * plane2.C - plane1.C * plane2.B;
	temp2 = plane1.A * plane2.C - plane1.C * plane2.A;
	temp3 = plane1.A * plane2.B - plane1.B * plane2.A;

	if (fabs(temp1) > 1e-6)
	{
		pt1.x = 0;
		pt1.y = -(plane1.D * plane2.C - plane2.D * plane1.C) / temp1;
		pt1.z = (plane1.D * plane2.B - plane2.D * plane1.B) / temp1;
	}
	else if (fabs(temp2) > 1e-6)
	{
		pt1.y = 0;
		pt1.x = -(plane1.D * plane2.C - plane2.D * plane1.C) / temp2;
		pt1.z = (plane1.D * plane2.A - plane2.D * plane1.A) / temp2;
	}
	else if (fabs(temp3) > 1e-6)
	{
		pt1.z = 0;
		pt1.x = -(plane1.D * plane2.B - plane2.D * plane1.B) / temp3;
		pt1.y = (plane1.D * plane2.A - plane2.D * plane1.A) / temp3;
	}
	pt2 = pt1 + vNormal;
	return '1';
}


// 数据精度处理函数 : 擦除小数点n位以后数据
void FuzzyPrecision_base(Vertex3d& pos, int nMedian)
{
	FuzzyPrecision_base(pos.x, nMedian);
	FuzzyPrecision_base(pos.y, nMedian);
	FuzzyPrecision_base(pos.z, nMedian);
}

// 数据精度处理函数 : 擦除小数点n位以后数据
void FuzzyPrecision_base(double& dData, int nMedian)
{
	if (dData < 0) return; // xx_modify_1011
	double dMedian = (double)nMedian;
	double copyData = dData;
	long lData = (long)copyData;
	double miniData = dData - lData;
	miniData *= pow(10.0, dMedian);
	long minidata = (long)miniData;
	dData = lData + minidata / pow(10.0, dMedian);
}

// 数据精度处理函数 : 擦除小数点n位以后数据
void FuzzyPrecision_base(float& dData, int nMedian)
{
	if (dData < 0) return; // xx_modify_1011
	double dMedian = (double)nMedian;
	double copyData = dData;
	long lData = (long)copyData;
	double miniData = dData - lData;
	miniData *= pow(10.0, dMedian);
	long minidata = (long)miniData;
	dData = lData + (float)(minidata / pow(10.0, dMedian));
}


//////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
void	InpByKochanekBartels(std::vector<Vertex3d>& ptArray, double dStepLength, std::vector<Vertex3d>& allPtArray, std::vector<int>& indexArray)
{
	int nSize = ptArray.size();
	assert(nSize >= 4);

	allPtArray.clear();

	// 通过插值点数实现分段插值
	for (int i = 1; i < nSize - 2; i++)
	{
		double dist_std = DPointToPoint(ptArray[i], ptArray[i + 1]);
		int nKeyNum = dist_std / dStepLength;

		if (fabs(dStepLength * nKeyNum - dist_std) < MathEx::TOL)
			nKeyNum--;

		int nRecIndex = allPtArray.size();
		allPtArray.push_back(ptArray[i]);
		indexArray.push_back(nRecIndex);

		for (int j = 0; j < nKeyNum; j++)
		{
			double t = (1 + j) * 1.0 / nKeyNum;
			Vertex3d ptInp = InpByKochanekBartels(ptArray, i, t);
			allPtArray.push_back(ptInp);
		}
	}

	// 加入倒数第二个控制点   省略最后一个控制点
	int nRecIndex = allPtArray.size();
	allPtArray.push_back(ptArray[nSize - 2]);
	indexArray.push_back(nRecIndex);
}


Vertex3d InpByCubicHermite(Vertex3d& P0, Vertex3d& D0, Vertex3d& P1, Vertex3d& D1, double t)
{
	double t2 = t * t;											// t2 = t^2;
	double t3 = t2 * t;											// t3 = t^3;
	return P0*(2 * t3 - 3 * t2 + 1) + D0*(t3 - 2 * t2 + t) + P1 * (-2 * t3 + 3 * t2) + D1 * (t3 - t2);
}

Vertex3d InpByCubicBezier(Vertex3d& P0, Vertex3d& P1, Vertex3d& P2, Vertex3d& P3, double t)
{
	double t2 = t * t;											// t2 = t^2;
	double t3 = t2 * t;											// t3 = t^3;
	return 1.0 / 6 * ((-t3 + 3 * t2 - 3 * t + 1) * P0 + (3 * t3 - 6 * t2 + 3 * t) * P1 + (-3 * t3 + 3 * t2) * P2 + t3 * P3);
}

Vertex3d InpByCubicBezier(std::vector<Vertex3d>& ptArray, int iIndex, double t)
{
	assert(iIndex + 2 < ptArray.size());

	// 注意iIndex的含义 相当于上面的P2点下标号
	return InpByCubicBezier(ptArray[iIndex - 1], ptArray[iIndex], ptArray[iIndex + 1], ptArray[iIndex + 2], t);
}

void	InpByCubicBezier(std::vector<Vertex3d>& ptArray, double dStepLength, std::vector<Vertex3d>& allPtArray, std::vector<int>& indexArray)
{
	int nSize = ptArray.size();
	assert(nSize >= 4);

	allPtArray.clear();

	Vertex3d lastPt;
	// 通过插值点数实现分段插值
	for (int i = 1; i < nSize - 2; i++)
	{
		double dist_std = DPointToPoint(ptArray[i], ptArray[i + 1]);
		int nKeyNum = dist_std / dStepLength;
		if (fabs(dStepLength * nKeyNum - dist_std) < MathEx::TOL)
			nKeyNum--;

		// 控制点发生改变
		int nRecIndex = allPtArray.size();
		Vertex3d ptInp = InpByCubicBezier(ptArray, i, 0.0);
		allPtArray.push_back(ptInp);
		indexArray.push_back(nRecIndex);

		for (int j = 0; j < nKeyNum; j++)
		{
			double t = (1 + j) * 1.0 / nKeyNum;
			Vertex3d ptInp = InpByCubicBezier(ptArray, i, t);
			allPtArray.push_back(ptInp);
		}

		if (i == nSize - 3)
		{
			// 加入倒数第二个控制点   省略最后一个控制点
			int nRecIndex = allPtArray.size();
			Vertex3d ptInp = InpByCubicBezier(ptArray, i, 1.0);
			allPtArray.push_back(ptInp);
			indexArray.push_back(nRecIndex);
		}
	}
}

Vertex3d InpByCubicBSpline(Vertex3d& P0, Vertex3d& P1, Vertex3d& P2, Vertex3d& P3, double t)
{
	double t2 = t * t;											// t2 = t^2;
	double t3 = t2 * t;											// t3 = t^3;
	return 1.0 / 6 * ((-t3 + 3 * t2 - 3 * t + 1) * P0 + (3 * t3 - 6 * t2 + 4) * P1 + (-3 * t3 + 3 * t2 + 3 * t + 1) * P2 + t3 * P3);
}

Vertex3d InpByCubicBSpline(std::vector<Vertex3d>& ptArray, int iIndex, double t)
{
	assert(iIndex + 2 < ptArray.size());

	// 注意iIndex的含义 相当于上面的P2点下标号
	return InpByCubicBSpline(ptArray[iIndex - 1], ptArray[iIndex], ptArray[iIndex + 1], ptArray[iIndex + 2], t);
}

void	InpByCubicBSpline(std::vector<Vertex3d>& ptArray, double dStepLength, std::vector<Vertex3d>& allPtArray, std::vector<int>& indexArray)
{
	int nSize = ptArray.size();
	assert(nSize >= 4);

	allPtArray.clear();

	Vertex3d lastPt;
	// 通过插值点数实现分段插值
	for (int i = 1; i < nSize - 2; i++)
	{
		double dist_std = DPointToPoint(ptArray[i], ptArray[i + 1]);
		int nKeyNum = dist_std / dStepLength;
		if (fabs(dStepLength * nKeyNum - dist_std) < MathEx::TOL)
			nKeyNum--;

		// 控制点发生改变
		int nRecIndex = allPtArray.size();
		Vertex3d ptInp = InpByCubicBSpline(ptArray, i, 0.0);
		allPtArray.push_back(ptInp);
		indexArray.push_back(nRecIndex);

		for (int j = 0; j < nKeyNum; j++)
		{
			double t = (1 + j) * 1.0 / nKeyNum;
			Vertex3d ptInp = InpByCubicBSpline(ptArray, i, t);
			allPtArray.push_back(ptInp);
		}

		if (i == nSize - 3)
		{
			// 加入倒数第二个控制点   省略最后一个控制点
			int nRecIndex = allPtArray.size();
			Vertex3d ptInp = InpByCubicBSpline(ptArray, i, 1.0);
			allPtArray.push_back(ptInp);
			indexArray.push_back(nRecIndex);
		}
	}
}


Vertex3d InpByKochanekBartels(Vertex3d& P0, Vertex3d& P1, Vertex3d& P2, Vertex3d& P3, double t)
{
	// 注意t的含义，t是指占P1P2的距离比例
	Vertex3d D1, D2;
	double dDis1 = DPointToPoint(P1, P0);
	double dDis2 = DPointToPoint(P2, P1);
	double dDis3 = DPointToPoint(P3, P2);

	double dsAdjust1 = (dDis1) / (dDis2 + dDis1);
	double dsAdjust2 = (dDis2) / (dDis2 + dDis3);

	D1 = dsAdjust1 * (P2 - P0);
	D2 = dsAdjust2 * (P3 - P1);

	return InpByCubicHermite(P1, D1, P2, D2, t);
}

Vertex3d InpByKochanekBartels(std::vector<Vertex3d>& ptArray, int iIndex, double t)
{
	assert(iIndex + 2 < ptArray.size());

	// 注意iIndex的含义 相当于上面的P2点下标号
	return InpByKochanekBartels(ptArray[iIndex - 1], ptArray[iIndex], ptArray[iIndex + 1], ptArray[iIndex + 2], t);
}

Vertex3d InpByLinearDis(std::vector<Vertex3d>& ptArray, int &nCurPathNode, Vertex3d vTrans, double& dDistanceAccumulation)
{
	double dist_std = 0;

	// 这里假设vTrans的开始是在nCurPathNode和nCurPathNode+1之间
	Vertex3d vVec = ptArray[nCurPathNode + 1] - ptArray[nCurPathNode];

	int nLimit = ptArray.size() - 1;
	double dDir = 1.0;
	//	if(vVec * vTrans < 0)
	if (vVec * vTrans < -MathEx::TOL)
	{
		dDir = -1;
		nLimit = 0;
	}
	Vertex3d vTransE = vTrans;
	vTransE.normalize();

	double dDisAtti = dDistanceAccumulation;

	while (1)
	{
		//	if(dDir > 0)
		if (dDir > MathEx::TOL)
		{
			if (nCurPathNode >= ptArray.size() - 1)
			{
				dDistanceAccumulation = 0;
				Vertex3d nextPos = ptArray[nCurPathNode];
				return nextPos;
			}
		}
		else
		{
			if (nCurPathNode < 0)
			{
				nCurPathNode = 0;
				dDistanceAccumulation = 0;
				Vertex3d nextPos = ptArray[0];
				return nextPos;
			}
		}

		Vertex3d vVec = ptArray[nCurPathNode + 1] - ptArray[nCurPathNode];
		dDistanceAccumulation = dDisAtti + vTrans.getProjectLength(vVec); // 可能为负值

		if (fabs(dDisAtti) < MathEx::TOL && dDistanceAccumulation * dDir < -MathEx::TOL)	// 已经到端点的情况下并且反向受力
		{
			if (dDir < -MathEx::TOL)
			{
				dDistanceAccumulation = 0;
			}
			else
			{
				dDistanceAccumulation = 0;
				nCurPathNode += 1;
			}
			Vertex3d nextPos = ptArray[nCurPathNode];
			return nextPos;
		}

		dist_std = DPointToPoint(ptArray[nCurPathNode + 1], ptArray[nCurPathNode]);

		//if((dDir * dDistanceAccumulation) < dist_std)
		if (((dDir * dDistanceAccumulation) - dist_std) < -MathEx::TOL)
			break;

		dDistanceAccumulation = dDir * dDistanceAccumulation - dist_std;
		vTrans = vTransE * fabs(dDistanceAccumulation);

		//if(dDir > 0)
		if (dDir > MathEx::TOL)
			nCurPathNode++;
		else
			nCurPathNode--;

		dDisAtti = 0;
	}

	Vertex3d nextPos;
	nextPos = ptArray[nCurPathNode + 1] - ptArray[nCurPathNode];
	if (dDir < -MathEx::TOL)
		dDistanceAccumulation = dist_std - dDistanceAccumulation;

	nextPos = nextPos * dDistanceAccumulation / dist_std + ptArray[nCurPathNode];
	return nextPos;

}

Vertex3d InpByLinearDis(std::vector<Vertex3d>& ptArray, double dStepLength, int &nCurPathNode, double &dDistanceAccumulation, double &dist_std)
{
	dDistanceAccumulation += dStepLength;
	dist_std = 0;

	//	if(dStepLength > 0)
	if (dStepLength > MathEx::TOL)
	{
		while (1)
		{
			if (nCurPathNode >= ptArray.size() - 1)
			{
				Vertex3d nextPos = ptArray[nCurPathNode];
				return nextPos;
			}

			dist_std = DPointToPoint(ptArray[nCurPathNode + 1], ptArray[nCurPathNode]);

			//	if(dDistanceAccumulation < dist_std)
			if ((dDistanceAccumulation - dist_std) < -MathEx::TOL)
				break;

			dDistanceAccumulation -= dist_std;
			nCurPathNode++;
		}

		Vertex3d nextPos;
		nextPos = ptArray[nCurPathNode + 1] - ptArray[nCurPathNode];
		nextPos = nextPos * dDistanceAccumulation / dist_std + ptArray[nCurPathNode];
		return nextPos;
	}

	else
	{
		while (1)
		{
			if (nCurPathNode <= 0)
			{
				Vertex3d nextPos = ptArray[0];
				return nextPos;
			}

			dist_std = DPointToPoint(ptArray[nCurPathNode - 1], ptArray[nCurPathNode]);

			double dist_std2 = -dist_std;
			//	if(fabs(dDistanceAccumulation) < dist_std)
			if ((fabs(dDistanceAccumulation) - dist_std) < -MathEx::TOL)
				break;

			dDistanceAccumulation = dist_std2 - dDistanceAccumulation;
			nCurPathNode--;
		}

		Vertex3d nextPos;
		nextPos = ptArray[nCurPathNode - 1] - ptArray[nCurPathNode];
		nextPos = nextPos * dDistanceAccumulation / dist_std + ptArray[nCurPathNode];
		return nextPos;
	}
}

Vertex3d InpByCatmullRom(Vertex3d& P0, Vertex3d& P1, Vertex3d& P2, Vertex3d& P3, double t)
{
	// 插值特点：
	// (1) 需要4个点来内插一个点,该点介于P1和P2之间，因此位于两端P0P1 和 P2P3段的点是不能进行插值的,一种做法是在两端增加控制一个控制点，该控制点通过线性插值获得
	// (2) 内插后的曲线经过所有控制点
	// (3) 曲线一次连续，二次不连续

	// 注意t的含义，假设Q是插值点，t = Dis(P1Q) / Dis(P1P2) > 0
	assert(t > MathEx::TOL);

	Vertex3d D1 = 0.5 * (P2 - P0);
	Vertex3d D2 = 0.5 * (P3 - P1);
	return InpByCubicHermite(P1, D1, P2, D2, t);
}

Vertex3d InpByCatmullRom(std::vector<Vertex3d>& ptArray, int iIndex, double t)
{
	assert(iIndex + 2 < ptArray.size());

	// 注意iIndex的含义 相当于上面的P2点下标号
	return InpByCatmullRom(ptArray[iIndex - 1], ptArray[iIndex], ptArray[iIndex + 1], ptArray[iIndex + 2], t);
}

void	InpByCatmullRom(std::vector<Vertex3d>& ptArray, double dStepLength, std::vector<Vertex3d>& allPtArray, std::vector<int>& indexArray)
{
	int nSize = ptArray.size();
	assert(nSize >= 4);

	allPtArray.clear();

	// 本身第一个控制点是应该加入的，但是是人为引入的，因此可以取消
	//allPtArray.push_back(ptArray[0]);
	// 		allPtArray.push_back(ptArray[1]);
	// 		indexArray.push_back(0);
	//indexArray.push_back(1);

	int nCurPathNode = 1;
	int nOldCurPathNode = nCurPathNode;
	double dDistanceAcc = 0;
	double dist_std;

	// 这种做法会保留上一次留下来的比例
	// 		while(1)
	// 		{
	// 			InpByLinearDis(ptArray, dStepLength, nCurPathNode, dDistanceAcc, dist_std);
	// 			if(nCurPathNode < nSize - 2)
	// 			{
	// 				if(nOldCurPathNode != nCurPathNode) // 说明控制点开始发生改变
	// 				{
	// 					int nRecIndex = allPtArray.size();
	// 					allPtArray.push_back(ptArray[nCurPathNode]);
	// 					indexArray.push_back(nRecIndex);
	// 					nOldCurPathNode = nCurPathNode;
	// 				}
	// 
	// 				double t = dDistanceAcc / dist_std;
	// 				Vertex3d ptInp = InpByCatmullRom(ptArray,nCurPathNode,t);
	// 				allPtArray.push_back(ptInp);
	// 			}
	// 			else
	// 				break;
	// 		}

	// 通过插值点数实现分段插值
	for (int i = 1; i < nSize - 2; i++)
	{
		dist_std = DPointToPoint(ptArray[i], ptArray[i + 1]);
		int nKeyNum = dist_std / dStepLength;
		if (fabs(dStepLength * nKeyNum - dist_std) < MathEx::TOL)
			nKeyNum--;

		int nRecIndex = allPtArray.size();
		allPtArray.push_back(ptArray[i]);
		indexArray.push_back(nRecIndex);

		for (int j = 0; j < nKeyNum; j++)
		{
			double t = (1 + j) * 1.0 / nKeyNum;
			Vertex3d ptInp = InpByCatmullRom(ptArray, i, t);
			allPtArray.push_back(ptInp);
		}
	}
	// 加入倒数第二个控制点   省略最后一个控制点
	int nRecIndex = allPtArray.size();
	allPtArray.push_back(ptArray[nSize - 2]);
	indexArray.push_back(nRecIndex);
}


char RFrustumWithEnvelope(FrustumImpl& frusum, AABBoxImpl & env)
{
	Vertex3d ptMin = env.GetMin();
	Vertex3d ptVertexP;
	Vertex3d ptVertexN;

	double xSize, ySize, zSize;
	xSize = env.XSize();
	ySize = env.YSize();
	zSize = env.ZSize();

	if (env.m_nRecPlaneIndex == -1)
	{
		for (int i = 0; i < 6; i++)
		{
			ptVertexN.x = ptMin.x + xSize * frusum.vPlaneNorSign[i][0];
			ptVertexN.y = ptMin.y + ySize * frusum.vPlaneNorSign[i][1];
			ptVertexN.z = ptMin.z + zSize * frusum.vPlaneNorSign[i][2];

			if ('0' == RPointWithPlane(ptVertexN, frusum.plane[i]))
			{
				env.m_nRecPlaneIndex = i;
				return '0';
			}
			ptVertexP.x = ptMin.x + xSize * (!frusum.vPlaneNorSign[i][0]);
			ptVertexP.y = ptMin.y + ySize * (!frusum.vPlaneNorSign[i][1]);
			ptVertexP.z = ptMin.z + zSize * (!frusum.vPlaneNorSign[i][2]);

			if ('0' == RPointWithPlane(ptVertexP, frusum.plane[i]))
			{
				return '2';
			}
		}

		return '1';
	}
	else
	{
		int nRec[6] = { 0, 1, 2, 3, 4, 5 };
		nRec[0] = env.m_nRecPlaneIndex;
		for (int i = 0; i < env.m_nRecPlaneIndex; i++)
			nRec[i + 1] = i;

		for (int j = 0; j < 6; j++)
		{
			int i = nRec[j];
			ptVertexN.x = ptMin.x + xSize * frusum.vPlaneNorSign[i][0];
			ptVertexN.y = ptMin.y + ySize * frusum.vPlaneNorSign[i][1];
			ptVertexN.z = ptMin.z + zSize * frusum.vPlaneNorSign[i][2];

			if ('0' == RPointWithPlane(ptVertexN, frusum.plane[i]))
			{
				env.m_nRecPlaneIndex = i;
				return '0';
			}

			ptVertexP.x = ptMin.x + xSize * (!frusum.vPlaneNorSign[i][0]);
			ptVertexP.y = ptMin.y + ySize * (!frusum.vPlaneNorSign[i][1]);
			ptVertexP.z = ptMin.z + zSize * (!frusum.vPlaneNorSign[i][2]);

			if ('0' == RPointWithPlane(ptVertexP, frusum.plane[i]))
			{
				env.m_nRecPlaneIndex = -1;
				return '2';// 相交
			}
		}

		env.m_nRecPlaneIndex = -1;
		return '1';
	}


	// ===========================================================
	// = 算法一

	// 	Vertex3d ptCorner;
	// 	env.GetVertices(ptCorner);
	// 	int iTotalIn = 0;
	// 	for(int i=0; i<6; i++)
	// 	{
	// 		int iInCount = 8;
	// 		int iPtIn = 1;		
	// 		for(int j = 0; j < 8; ++j) 
	// 		{			// test this point against the planes
	// 			if('0' == RPointWithPlane(ptCorner[j],frusum.plane[i]))
	// 			{
	// 				iPtIn = 0;
	// 				--iInCount;
	// 			}
	// 		}		// were all the points outside of plane p?
	// 		if(iInCount == 0)
	// 			return '0';		// check if they were all on the right side of the plane
	// 		iTotalIn += iPtIn;
	// 	}
	// 
	// 	if(iTotalIn == 6)
	// 		return '1';	// 在其内部
	// 
	// 	return '2'; // 相交 
}


int IsRayCrossEnvelope(RayImpl& r, const AABBoxImpl & env, double t0, double t1)
{
	// 算法1：与6个面进行相交检测 效率低下

	// ======================================================
	// 	PLANE3D plane[6] = {{0,0,1,-env.MaxZ()},{0,0,1,-env.MinZ()},{0,1,0,-env.MaxY()},{0,1,0,env.MinY()},
	// 	{1,0,0,env.MinX()},{1,0,0,env.MaxX()}};
	// 
	// 	double planeScope[3][4] = {{env.MinX(),env.MaxX(),env.MinY(),env.MaxY()}, 
	// 				{env.MinX(),env.MaxX(),env.MinZ(),env.MaxZ()},  
	// 				{env.MinY(),env.MaxY(),env.MinZ(),env.MaxZ()}};
	// 	
	// 	// 只判断0，1情况，其他复杂情况不判断，因为只用在点选里面
	// 	for(int i=0; i<6; i++)
	// 	{
	// 		Vertex3d tempPt;
	// 		char cRes = RLineSegWithPlane(LineStr,LineEnd,plane[i],&tempPt);
	// 		if(cRes != '0')
	// 		{
	// 			double tempPtCoors[6] = {tempPt.x,tempPt.y, tempPt.x,tempPt.z, tempPt.y,tempPt.z};
	// 			int j = i / 2;
	// 			double Tol = MathEx::TOL;
	// 			if((tempPtCoors[j] - planeScope[j][0]) > -Tol && (tempPtCoors[j] - planeScope[j][1]) < Tol &&
	// 				(tempPtCoors[j+1] - planeScope[j][2]) > -Tol && (tempPtCoors[j+1] - planeScope[j][3]) < Tol)
	// 
	// 				return 1;
	// 		}
	// 	}
	// 
	// 	return 0;

	// 算法2：超高效算法

	double tmin, tmax, tymin, tymax, tzmin, tzmax;
	Vertex3d parameters[2] = { env.GetMin(), env.GetMax() };
	tmin = (parameters[r.sign[0]].x - r.origin.x) * r.inv_direction.x;
	tmax = (parameters[1 - r.sign[0]].x - r.origin.x) * r.inv_direction.x;
	tymin = (parameters[r.sign[1]].y - r.origin.y) * r.inv_direction.y;
	tymax = (parameters[1 - r.sign[1]].y - r.origin.y) * r.inv_direction.y;
	if ((tmin > tymax) || (tymin > tmax))
		return 0;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;
	tzmin = (parameters[r.sign[2]].z - r.origin.z) * r.inv_direction.z;
	tzmax = (parameters[1 - r.sign[2]].z - r.origin.z) * r.inv_direction.z;
	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;
	return ((tmin < t1) && (tmax > t0));

}

////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//NVTriStrip
//////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
// EnableRestart()
//
// For GPUs that support primitive restart, this sets a value as the restart index
//
// Restart is meaningless if strips are not being stitched together, so enabling restart
//  makes NvTriStrip forcing stitching.  So, you'll get back one strip.
//
// Default value: disabled
//
void EnableRestart(const unsigned long restartVal);

////////////////////////////////////////////////////////////////////////////////////////
// DisableRestart()
//
// For GPUs that support primitive restart, this disables using primitive restart
//
void DisableRestart();


////////////////////////////////////////////////////////////////////////////////////////
// SetCacheSize()
//
// Sets the cache size which the stripfier uses to optimize the data.
// Controls the length of the generated individual strips.
// This is the "actual" cache size, so 24 for GeForce3 and 16 for GeForce1/2
// You may want to play around with this number to tweak performance.
//
// Default value: 16
//
void SetCacheSize(const unsigned long cacheSize);


////////////////////////////////////////////////////////////////////////////////////////
// SetStitchStrips()
//
// bool to indicate whether to stitch together strips into one huge strip or not.
// If set to true, you'll get back one huge strip stitched together using degenerate
//  triangles.
// If set to false, you'll get back a large number of separate strips.
//
// Default value: true
//
void SetStitchStrips(const bool bStitchStrips);


////////////////////////////////////////////////////////////////////////////////////////
// SetMinStripSize()
//
// Sets the minimum acceptable size for a strip, in triangles.
// All strips generated which are shorter than this will be thrown into one big, separate list.
//
// Default value: 0
//
void SetMinStripSize(const unsigned long minSize);


////////////////////////////////////////////////////////////////////////////////////////
// SetListsOnly()
//
// If set to true, will return an optimized list, with no strips at all.
//
// Default value: false
//
void SetListsOnly(const bool bListsOnly);


////////////////////////////////////////////////////////////////////////////////////////
// GenerateStrips()
//
// in_indices: input index list, the indices you would use to render
// in_numIndices: number of entries in in_indices
// primGroups: array of optimized/stripified PrimitiveGroups
// numGroups: number of groups returned
//
// Be sure to call delete[] on the returned primGroups to avoid leaking mem
//
bool GenerateStrips(const unsigned long* in_indices, const unsigned long in_numIndices,
	PrimitiveGroup** primGroups, unsigned long* numGroups, bool validateEnabled = false);


////////////////////////////////////////////////////////////////////////////////////////
// RemapIndices()
//
// Function to remap your indices to improve spatial locality in your vertex buffer.
//
// in_primGroups: array of PrimitiveGroups you want remapped
// numGroups: number of entries in in_primGroups
// numVerts: number of vertices in your vertex buffer, also can be thought of as the range
//  of acceptable values for indices in your primitive groups.
// remappedGroups: array of remapped PrimitiveGroups
//
// Note that, according to the remapping handed back to you, you must reorder your 
//  vertex buffer.
//
// Credit goes to the MS Xbox crew for the idea for this interface.
//
void RemapIndices(const PrimitiveGroup* in_primGroups, const unsigned long numGroups,
	const unsigned long numVerts, PrimitiveGroup** remappedGroups);

//=======================================Declarence=====================================
//////////////////////////////////////////////////////////////////////////CacheSimulator
class VertexCache
{

public:

	VertexCache(int size)
	{
		numEntries = size;

		entries = new int[numEntries];

		for (int i = 0; i < numEntries; i++)
			entries[i] = -1;
	}

	VertexCache() { VertexCache(16); }
	~VertexCache() { delete[] entries; entries = 0; }

	bool InCache(int entry)
	{
		bool returnVal = false;
		for (int i = 0; i < numEntries; i++)
		{
			if (entries[i] == entry)
			{
				returnVal = true;
				break;
			}
		}

		return returnVal;
	}

	int AddEntry(int entry)
	{
		int removed;

		removed = entries[numEntries - 1];

		//push everything right one
		for (int i = numEntries - 2; i >= 0; i--)
		{
			entries[i + 1] = entries[i];
		}

		entries[0] = entry;

		return removed;
	}

	void Clear()
	{
		memset(entries, -1, sizeof(int) * numEntries);
	}

	void Copy(VertexCache* inVcache)
	{
		for (int i = 0; i < numEntries; i++)
		{
			inVcache->Set(i, entries[i]);
		}
	}

	int At(int index) { return entries[index]; }
	void Set(int index, int value) { entries[index] = value; }

private:

	int *entries;
	int numEntries;

};
//////////////////////////////////////////////////////////////////////////NVTriStripe

/////////////////////////////////////////////////////////////////////////////////
//
// Types defined for stripification
//
/////////////////////////////////////////////////////////////////////////////////

struct MyVertex {
	float x, y, z;
	float nx, ny, nz;
};

typedef MyVertex MyVector;

struct MyFace {
	int v1, v2, v3;
	float nx, ny, nz;
};


class NvFaceInfo {
public:

	// vertex indices
	NvFaceInfo(int v0, int v1, int v2, bool bIsFake = false){
		m_v0 = v0; m_v1 = v1; m_v2 = v2;
		m_stripId = -1;
		m_testStripId = -1;
		m_experimentId = -1;
		m_bIsFake = bIsFake;
	}

	// data members are left public
	int   m_v0, m_v1, m_v2;
	int   m_stripId;      // real strip Id
	int   m_testStripId;  // strip Id in an experiment
	int   m_experimentId; // in what experiment was it given an experiment Id?
	bool  m_bIsFake;      //if true, will be deleted when the strip it's in is deleted
};

// nice and dumb edge class that points knows its
// indices, the two faces, and the next edge using
// the lesser of the indices
class NvEdgeInfo {
public:

	// constructor puts 1 ref on us
	NvEdgeInfo(int v0, int v1){
		m_v0 = v0;
		m_v1 = v1;
		m_face0 = NULL;
		m_face1 = NULL;
		m_nextV0 = NULL;
		m_nextV1 = NULL;

		// we will appear in 2 lists.  this is a good
		// way to make sure we delete it the second time
		// we hit it in the edge infos
		m_refCount = 2;

	}

	// ref and unref
	void Unref() { if (--m_refCount == 0) delete this; }

	// data members are left public
	UINT         m_refCount;
	NvFaceInfo  *m_face0, *m_face1;
	int          m_v0, m_v1;
	NvEdgeInfo  *m_nextV0, *m_nextV1;
};


// This class is a quick summary of parameters used
// to begin a triangle strip.  Some operations may
// want to create lists of such items, so they were
// pulled out into a class
class NvStripStartInfo {
public:
	NvStripStartInfo(NvFaceInfo *startFace, NvEdgeInfo *startEdge, bool toV1){
		m_startFace = startFace;
		m_startEdge = startEdge;
		m_toV1 = toV1;
	}
	NvFaceInfo    *m_startFace;
	NvEdgeInfo    *m_startEdge;
	bool           m_toV1;
};


typedef std::vector<NvFaceInfo*>     NvFaceInfoVec;
typedef std::list  <NvFaceInfo*>     NvFaceInfoList;
typedef std::list  <NvFaceInfoVec*>  NvStripList;
typedef std::vector<NvEdgeInfo*>     NvEdgeInfoVec;

typedef std::vector<unsigned short> WordVec;
typedef std::vector<int> IntVec;
typedef std::vector<MyVertex> MyVertexVec;
typedef std::vector<MyFace> MyFaceVec;

template<class T>
inline void SWAP(T& first, T& second)
{
	T temp = first;
	first = second;
	second = temp;
}

// This is a summary of a strip that has been built
class NvStripInfo {
public:

	// A little information about the creation of the triangle strips
	NvStripInfo(const NvStripStartInfo &startInfo, int stripId, int experimentId = -1) :
		m_startInfo(startInfo)
	{
		m_stripId = stripId;
		m_experimentId = experimentId;
		visited = false;
		m_numDegenerates = 0;
	}

	// This is an experiment if the experiment id is >= 0
	inline bool IsExperiment() const { return m_experimentId >= 0; }

	inline bool IsInStrip(const NvFaceInfo *faceInfo) const
	{
		if (faceInfo == NULL)
			return false;

		return (m_experimentId >= 0 ? faceInfo->m_testStripId == m_stripId : faceInfo->m_stripId == m_stripId);
	}

	bool SharesEdge(const NvFaceInfo* faceInfo, NvEdgeInfoVec &edgeInfos);

	// take the given forward and backward strips and combine them together
	void Combine(const NvFaceInfoVec &forward, const NvFaceInfoVec &backward);

	//returns true if the face is "unique", i.e. has a vertex which doesn't exist in the faceVec
	bool Unique(NvFaceInfoVec& faceVec, NvFaceInfo* face);

	// mark the triangle as taken by this strip
	bool IsMarked(NvFaceInfo *faceInfo);
	void MarkTriangle(NvFaceInfo *faceInfo);

	// build the strip
	void Build(NvEdgeInfoVec &edgeInfos, NvFaceInfoVec &faceInfos);

	// public data members
	NvStripStartInfo m_startInfo;
	NvFaceInfoVec    m_faces;
	int              m_stripId;
	int              m_experimentId;

	bool visited;

	int m_numDegenerates;
};

typedef std::vector<NvStripInfo*>    NvStripInfoVec;


//The actual stripifier
class NvStripifier {
public:

	// Constructor
	NvStripifier();
	~NvStripifier();

	//the target vertex cache size, the structure to place the strips in, and the input indices
	void Stripify(const WordVec &in_indices, const int in_cacheSize, const int in_minStripLength,
		const unsigned long maxIndex, NvStripInfoVec &allStrips, NvFaceInfoVec &allFaces);
	void CreateStrips(const NvStripInfoVec& allStrips, IntVec& stripIndices, const bool bStitchStrips, unsigned long& numSeparateStrips, const bool bRestart, const unsigned long restartVal);

	static int GetUniqueVertexInB(NvFaceInfo *faceA, NvFaceInfo *faceB);
	//static int GetSharedVertex(NvFaceInfo *faceA, NvFaceInfo *faceB);
	static void GetSharedVertices(NvFaceInfo *faceA, NvFaceInfo *faceB, int* vertex0, int* vertex1);

	static bool IsDegenerate(const NvFaceInfo* face);
	static bool IsDegenerate(const unsigned long v0, const unsigned long v1, const unsigned long v2);

protected:

	WordVec indices;
	int cacheSize;
	int minStripLength;
	float meshJump;
	bool bFirstTimeResetPoint;

	/////////////////////////////////////////////////////////////////////////////////
	//
	// Big mess of functions called during stripification
	//
	/////////////////////////////////////////////////////////////////////////////////

	//********************
	bool IsMoneyFace(const NvFaceInfo& face);
	bool FaceContainsIndex(const NvFaceInfo& face, const unsigned long index);

	bool IsCW(NvFaceInfo *faceInfo, int v0, int v1);
	bool NextIsCW(const int numIndices);

	static int  GetNextIndex(const WordVec &indices, NvFaceInfo *face);
	static NvEdgeInfo *FindEdgeInfo(NvEdgeInfoVec &edgeInfos, int v0, int v1);
	static NvFaceInfo *FindOtherFace(NvEdgeInfoVec &edgeInfos, int v0, int v1, NvFaceInfo *faceInfo);
	NvFaceInfo *FindGoodResetPoint(NvFaceInfoVec &faceInfos, NvEdgeInfoVec &edgeInfos);

	void FindAllStrips(NvStripInfoVec &allStrips, NvFaceInfoVec &allFaceInfos, NvEdgeInfoVec &allEdgeInfos, int numSamples);
	void SplitUpStripsAndOptimize(NvStripInfoVec &allStrips, NvStripInfoVec &outStrips, NvEdgeInfoVec& edgeInfos, NvFaceInfoVec& outFaceList);
	void RemoveSmallStrips(NvStripInfoVec& allStrips, NvStripInfoVec& allBigStrips, NvFaceInfoVec& faceList);

	bool FindTraversal(NvFaceInfoVec &faceInfos, NvEdgeInfoVec &edgeInfos, NvStripInfo *strip, NvStripStartInfo &startInfo);
	int  CountRemainingTris(std::list<NvStripInfo*>::iterator iter, std::list<NvStripInfo*>::iterator  end);

	void CommitStrips(NvStripInfoVec &allStrips, const NvStripInfoVec &strips);

	float AvgStripSize(const NvStripInfoVec &strips);
	int FindStartPoint(NvFaceInfoVec &faceInfos, NvEdgeInfoVec &edgeInfos);

	void UpdateCacheStrip(VertexCache* vcache, NvStripInfo* strip);
	void UpdateCacheFace(VertexCache* vcache, NvFaceInfo* face);
	float CalcNumHitsStrip(VertexCache* vcache, NvStripInfo* strip);
	int CalcNumHitsFace(VertexCache* vcache, NvFaceInfo* face);
	int NumNeighbors(NvFaceInfo* face, NvEdgeInfoVec& edgeInfoVec);

	void BuildStripifyInfo(NvFaceInfoVec &faceInfos, NvEdgeInfoVec &edgeInfos, const unsigned long maxIndex);
	bool AlreadyExists(NvFaceInfo* faceInfo, NvFaceInfoVec& faceInfos);

	// let our strip info classes and the other classes get
	// to these protected stripificaton methods if they want
	friend NvStripInfo;
};


//===========================================Implementation==========================================
#define CACHE_INEFFICIENCY 6

NvStripifier::NvStripifier()
{

}

NvStripifier::~NvStripifier()
{

}


///////////////////////////////////////////////////////////////////////////////////////////
// FindEdgeInfo()
//
// find the edge info for these two indices
//
NvEdgeInfo * NvStripifier::FindEdgeInfo(NvEdgeInfoVec &edgeInfos, int v0, int v1){

	// we can get to it through either array
	// because the edge infos have a v0 and v1
	// and there is no order except how it was
	// first created.
	NvEdgeInfo *infoIter = edgeInfos[v0];
	while (infoIter != NULL){
		if (infoIter->m_v0 == v0){
			if (infoIter->m_v1 == v1)
				return infoIter;
			else
				infoIter = infoIter->m_nextV0;
		}
		else {
			assert(infoIter->m_v1 == v0);
			if (infoIter->m_v0 == v1)
				return infoIter;
			else
				infoIter = infoIter->m_nextV1;
		}
	}
	return NULL;
}


///////////////////////////////////////////////////////////////////////////////////////////
// FindOtherFace
//
// find the other face sharing these vertices
// exactly like the edge info above
//
NvFaceInfo * NvStripifier::FindOtherFace(NvEdgeInfoVec &edgeInfos, int v0, int v1, NvFaceInfo *faceInfo){
	NvEdgeInfo *edgeInfo = FindEdgeInfo(edgeInfos, v0, v1);

	if ((edgeInfo == NULL) && (v0 == v1))
	{
		//we've hit a degenerate
		return NULL;
	}

	assert(edgeInfo != NULL);
	return (edgeInfo->m_face0 == faceInfo ? edgeInfo->m_face1 : edgeInfo->m_face0);
}


bool NvStripifier::AlreadyExists(NvFaceInfo* faceInfo, NvFaceInfoVec& faceInfos)
{
	for (int i = 0; i < faceInfos.size(); ++i)
	{
		if ((faceInfos[i]->m_v0 == faceInfo->m_v0) &&
			(faceInfos[i]->m_v1 == faceInfo->m_v1) &&
			(faceInfos[i]->m_v2 == faceInfo->m_v2))
			return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////
// BuildStripifyInfo()
//
// Builds the list of all face and edge infos
//
void NvStripifier::BuildStripifyInfo(NvFaceInfoVec &faceInfos, NvEdgeInfoVec &edgeInfos,
	const unsigned long maxIndex)
{
	// reserve space for the face infos, but do not resize them.
	int numIndices = indices.size();
	faceInfos.reserve(numIndices / 3);

	// we actually resize the edge infos, so we must initialize to NULL
	edgeInfos.resize(maxIndex + 1);
	int i;
	for (i = 0; i < maxIndex + 1; i++)
		edgeInfos[i] = NULL;

	// iterate through the triangles of the triangle list
	int numTriangles = numIndices / 3;
	int index = 0;
	bool bFaceUpdated[3];

	for (i = 0; i < numTriangles; i++)
	{
		bool bMightAlreadyExist = true;
		bFaceUpdated[0] = false;
		bFaceUpdated[1] = false;
		bFaceUpdated[2] = false;

		// grab the indices
		int v0 = indices[index++];
		int v1 = indices[index++];
		int v2 = indices[index++];

		//we disregard degenerates
		if (IsDegenerate(v0, v1, v2))
			continue;

		// create the face info and add it to the list of faces, but only if this exact face doesn't already 
		//  exist in the list
		NvFaceInfo *faceInfo = new NvFaceInfo(v0, v1, v2);

		// grab the edge infos, creating them if they do not already exist
		NvEdgeInfo *edgeInfo01 = FindEdgeInfo(edgeInfos, v0, v1);
		if (edgeInfo01 == NULL)
		{
			//since one of it's edges isn't in the edge data structure, it can't already exist in the face structure
			bMightAlreadyExist = false;

			// create the info
			edgeInfo01 = new NvEdgeInfo(v0, v1);

			// update the linked list on both 
			edgeInfo01->m_nextV0 = edgeInfos[v0];
			edgeInfo01->m_nextV1 = edgeInfos[v1];
			edgeInfos[v0] = edgeInfo01;
			edgeInfos[v1] = edgeInfo01;

			// set face 0
			edgeInfo01->m_face0 = faceInfo;
		}
		else
		{
			if (edgeInfo01->m_face1 != NULL)
			{
				printf("BuildStripifyInfo: > 2 triangles on an edge... uncertain consequences\n");
			}
			else
			{
				edgeInfo01->m_face1 = faceInfo;
				bFaceUpdated[0] = true;
			}
		}

		// grab the edge infos, creating them if they do not already exist
		NvEdgeInfo *edgeInfo12 = FindEdgeInfo(edgeInfos, v1, v2);
		if (edgeInfo12 == NULL)
		{
			bMightAlreadyExist = false;

			// create the info
			edgeInfo12 = new NvEdgeInfo(v1, v2);

			// update the linked list on both 
			edgeInfo12->m_nextV0 = edgeInfos[v1];
			edgeInfo12->m_nextV1 = edgeInfos[v2];
			edgeInfos[v1] = edgeInfo12;
			edgeInfos[v2] = edgeInfo12;

			// set face 0
			edgeInfo12->m_face0 = faceInfo;
		}
		else
		{
			if (edgeInfo12->m_face1 != NULL)
			{
				printf("BuildStripifyInfo: > 2 triangles on an edge... uncertain consequences\n");
			}
			else
			{
				edgeInfo12->m_face1 = faceInfo;
				bFaceUpdated[1] = true;
			}
		}

		// grab the edge infos, creating them if they do not already exist
		NvEdgeInfo *edgeInfo20 = FindEdgeInfo(edgeInfos, v2, v0);
		if (edgeInfo20 == NULL)
		{
			bMightAlreadyExist = false;

			// create the info
			edgeInfo20 = new NvEdgeInfo(v2, v0);

			// update the linked list on both 
			edgeInfo20->m_nextV0 = edgeInfos[v2];
			edgeInfo20->m_nextV1 = edgeInfos[v0];
			edgeInfos[v2] = edgeInfo20;
			edgeInfos[v0] = edgeInfo20;

			// set face 0
			edgeInfo20->m_face0 = faceInfo;
		}
		else
		{
			if (edgeInfo20->m_face1 != NULL)
			{
				printf("BuildStripifyInfo: > 2 triangles on an edge... uncertain consequences\n");
			}
			else
			{
				edgeInfo20->m_face1 = faceInfo;
				bFaceUpdated[2] = true;
			}
		}

		if (bMightAlreadyExist)
		{
			if (!AlreadyExists(faceInfo, faceInfos))
				faceInfos.push_back(faceInfo);
			else
			{
				delete faceInfo;

				//cleanup pointers that point to this deleted face
				if (bFaceUpdated[0])
					edgeInfo01->m_face1 = NULL;
				if (bFaceUpdated[1])
					edgeInfo12->m_face1 = NULL;
				if (bFaceUpdated[2])
					edgeInfo20->m_face1 = NULL;
			}
		}
		else
		{
			faceInfos.push_back(faceInfo);
		}

	}
}


///////////////////////////////////////////////////////////////////////////////////////////
// FindStartPoint()
//
// Finds a good starting point, namely one which has only one neighbor
//
int NvStripifier::FindStartPoint(NvFaceInfoVec &faceInfos, NvEdgeInfoVec &edgeInfos)
{
	int bestCtr = -1;
	int bestIndex = -1;

	for (int i = 0; i < faceInfos.size(); i++)
	{
		int ctr = 0;

		if (FindOtherFace(edgeInfos, faceInfos[i]->m_v0, faceInfos[i]->m_v1, faceInfos[i]) == NULL)
			ctr++;
		if (FindOtherFace(edgeInfos, faceInfos[i]->m_v1, faceInfos[i]->m_v2, faceInfos[i]) == NULL)
			ctr++;
		if (FindOtherFace(edgeInfos, faceInfos[i]->m_v2, faceInfos[i]->m_v0, faceInfos[i]) == NULL)
			ctr++;
		if (ctr > bestCtr)
		{
			bestCtr = ctr;
			bestIndex = i;
			//return i;
		}
	}
	//return -1;

	if (bestCtr == 0)
		return -1;
	else
		return bestIndex;
}


///////////////////////////////////////////////////////////////////////////////////////////
// FindGoodResetPoint()
//  
// A good reset point is one near other commited areas so that
// we know that when we've made the longest strips its because
// we're stripifying in the same general orientation.
//
NvFaceInfo* NvStripifier::FindGoodResetPoint(NvFaceInfoVec &faceInfos, NvEdgeInfoVec &edgeInfos){
	// we hop into different areas of the mesh to try to get
	// other large open spans done.  Areas of small strips can
	// just be left to triangle lists added at the end.
	NvFaceInfo *result = NULL;

	if (result == NULL)
	{
		int numFaces = faceInfos.size();
		int startPoint;
		if (bFirstTimeResetPoint)
		{
			//first time, find a face with few neighbors (look for an edge of the mesh)
			startPoint = FindStartPoint(faceInfos, edgeInfos);
			bFirstTimeResetPoint = false;
		}
		else
			startPoint = (int)(((float)numFaces - 1) * meshJump);

		if (startPoint == -1)
		{
			startPoint = (int)(((float)numFaces - 1) * meshJump);

			//meshJump += 0.1f;
			//if (meshJump > 1.0f)
			//	meshJump = .05f;
		}

		int i = startPoint;
		do {

			// if this guy isn't visited, try him
			if (faceInfos[i]->m_stripId < 0){
				result = faceInfos[i];
				break;
			}

			// update the index and clamp to 0-(numFaces-1)
			if (++i >= numFaces)
				i = 0;

		} while (i != startPoint);

		// update the meshJump
		meshJump += 0.1f;
		if (meshJump > 1.0f)
			meshJump = .05f;
	}

	// return the best face we found
	return result;
}


///////////////////////////////////////////////////////////////////////////////////////////
// GetUniqueVertexInB()
//
// Returns the vertex unique to faceB
//
int NvStripifier::GetUniqueVertexInB(NvFaceInfo *faceA, NvFaceInfo *faceB){

	int facev0 = faceB->m_v0;
	if (facev0 != faceA->m_v0 &&
		facev0 != faceA->m_v1 &&
		facev0 != faceA->m_v2)
		return facev0;

	int facev1 = faceB->m_v1;
	if (facev1 != faceA->m_v0 &&
		facev1 != faceA->m_v1 &&
		facev1 != faceA->m_v2)
		return facev1;

	int facev2 = faceB->m_v2;
	if (facev2 != faceA->m_v0 &&
		facev2 != faceA->m_v1 &&
		facev2 != faceA->m_v2)
		return facev2;

	// nothing is different
	return -1;
}


///////////////////////////////////////////////////////////////////////////////////////////
// GetSharedVertices()
//
// Returns the (at most) two vertices shared between the two faces
//
void NvStripifier::GetSharedVertices(NvFaceInfo *faceA, NvFaceInfo *faceB, int* vertex0, int* vertex1)
{
	*vertex0 = -1;
	*vertex1 = -1;

	int facev0 = faceB->m_v0;
	if (facev0 == faceA->m_v0 ||
		facev0 == faceA->m_v1 ||
		facev0 == faceA->m_v2)
	{
		if (*vertex0 == -1)
			*vertex0 = facev0;
		else
		{
			*vertex1 = facev0;
			return;
		}
	}

	int facev1 = faceB->m_v1;
	if (facev1 == faceA->m_v0 ||
		facev1 == faceA->m_v1 ||
		facev1 == faceA->m_v2)
	{
		if (*vertex0 == -1)
			*vertex0 = facev1;
		else
		{
			*vertex1 = facev1;
			return;
		}
	}

	int facev2 = faceB->m_v2;
	if (facev2 == faceA->m_v0 ||
		facev2 == faceA->m_v1 ||
		facev2 == faceA->m_v2)
	{
		if (*vertex0 == -1)
			*vertex0 = facev2;
		else
		{
			*vertex1 = facev2;
			return;
		}
	}

}


///////////////////////////////////////////////////////////////////////////////////////////
// GetNextIndex()
//
// Returns vertex of the input face which is "next" in the input index list
//
inline int NvStripifier::GetNextIndex(const WordVec &indices, NvFaceInfo *face){

	int numIndices = indices.size();
	assert(numIndices >= 2);

	int v0 = indices[numIndices - 2];
	int v1 = indices[numIndices - 1];

	int fv0 = face->m_v0;
	int fv1 = face->m_v1;
	int fv2 = face->m_v2;

	if (fv0 != v0 && fv0 != v1){
		if ((fv1 != v0 && fv1 != v1) || (fv2 != v0 && fv2 != v1)){
			printf("GetNextIndex: Triangle doesn't have all of its vertices\n");
			printf("GetNextIndex: Duplicate triangle probably got us derailed\n");
		}
		return fv0;
	}
	if (fv1 != v0 && fv1 != v1){
		if ((fv0 != v0 && fv0 != v1) || (fv2 != v0 && fv2 != v1)){
			printf("GetNextIndex: Triangle doesn't have all of its vertices\n");
			printf("GetNextIndex: Duplicate triangle probably got us derailed\n");
		}
		return fv1;
	}
	if (fv2 != v0 && fv2 != v1){
		if ((fv0 != v0 && fv0 != v1) || (fv1 != v0 && fv1 != v1)){
			printf("GetNextIndex: Triangle doesn't have all of its vertices\n");
			printf("GetNextIndex: Duplicate triangle probably got us derailed\n");
		}
		return fv2;
	}

	// shouldn't get here, but let's try and fail gracefully
	if ((fv0 == fv1) || (fv0 == fv2))
		return fv0;
	else if ((fv1 == fv0) || (fv1 == fv2))
		return fv1;
	else if ((fv2 == fv0) || (fv2 == fv1))
		return fv2;
	else
		return -1;
}


///////////////////////////////////////////////////////////////////////////////////////////
// IsMarked()
//
// If either the faceInfo has a real strip index because it is
// already assign to a committed strip OR it is assigned in an
// experiment and the experiment index is the one we are building
// for, then it is marked and unavailable
inline bool NvStripInfo::IsMarked(NvFaceInfo *faceInfo){
	return (faceInfo->m_stripId >= 0) || (IsExperiment() && faceInfo->m_experimentId == m_experimentId);
}


///////////////////////////////////////////////////////////////////////////////////////////
// MarkTriangle()
//
// Marks the face with the current strip ID
//
inline void NvStripInfo::MarkTriangle(NvFaceInfo *faceInfo){
	assert(!IsMarked(faceInfo));
	if (IsExperiment()){
		faceInfo->m_experimentId = m_experimentId;
		faceInfo->m_testStripId = m_stripId;
	}
	else{
		assert(faceInfo->m_stripId == -1);
		faceInfo->m_experimentId = -1;
		faceInfo->m_stripId = m_stripId;
	}
}


bool NvStripInfo::Unique(NvFaceInfoVec& faceVec, NvFaceInfo* face)
{
	bool bv0, bv1, bv2; //bools to indicate whether a vertex is in the faceVec or not
	bv0 = bv1 = bv2 = false;

	for (int i = 0; i < faceVec.size(); i++)
	{
		if (!bv0)
		{
			if ((faceVec[i]->m_v0 == face->m_v0) ||
				(faceVec[i]->m_v1 == face->m_v0) ||
				(faceVec[i]->m_v2 == face->m_v0))
				bv0 = true;
		}

		if (!bv1)
		{
			if ((faceVec[i]->m_v0 == face->m_v1) ||
				(faceVec[i]->m_v1 == face->m_v1) ||
				(faceVec[i]->m_v2 == face->m_v1))
				bv1 = true;
		}

		if (!bv2)
		{
			if ((faceVec[i]->m_v0 == face->m_v2) ||
				(faceVec[i]->m_v1 == face->m_v2) ||
				(faceVec[i]->m_v2 == face->m_v2))
				bv2 = true;
		}

		//the face is not unique, all it's vertices exist in the face vector
		if (bv0 && bv1 && bv2)
			return false;
	}

	//if we get out here, it's unique
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////
// Build()
//
// Builds a strip forward as far as we can go, then builds backwards, and joins the two lists
//
void NvStripInfo::Build(NvEdgeInfoVec &edgeInfos, NvFaceInfoVec &faceInfos)
{
	// used in building the strips forward and backward
	WordVec scratchIndices;

	// build forward... start with the initial face
	NvFaceInfoVec forwardFaces, backwardFaces;
	forwardFaces.push_back(m_startInfo.m_startFace);

	MarkTriangle(m_startInfo.m_startFace);

	int v0 = (m_startInfo.m_toV1 ? m_startInfo.m_startEdge->m_v0 : m_startInfo.m_startEdge->m_v1);
	int v1 = (m_startInfo.m_toV1 ? m_startInfo.m_startEdge->m_v1 : m_startInfo.m_startEdge->m_v0);

	// easiest way to get v2 is to use this function which requires the
	// other indices to already be in the list.
	scratchIndices.push_back(v0);
	scratchIndices.push_back(v1);
	int v2 = NvStripifier::GetNextIndex(scratchIndices, m_startInfo.m_startFace);
	scratchIndices.push_back(v2);

	//
	// build the forward list
	//
	int nv0 = v1;
	int nv1 = v2;

	NvFaceInfo *nextFace = NvStripifier::FindOtherFace(edgeInfos, nv0, nv1, m_startInfo.m_startFace);
	while (nextFace != NULL && !IsMarked(nextFace))
	{
		//check to see if this next face is going to cause us to die soon
		int testnv0 = nv1;
		int testnv1 = NvStripifier::GetNextIndex(scratchIndices, nextFace);

		NvFaceInfo* nextNextFace = NvStripifier::FindOtherFace(edgeInfos, testnv0, testnv1, nextFace);

		if ((nextNextFace == NULL) || (IsMarked(nextNextFace)))
		{
			//uh, oh, we're following a dead end, try swapping
			NvFaceInfo* testNextFace = NvStripifier::FindOtherFace(edgeInfos, nv0, testnv1, nextFace);

			if (((testNextFace != NULL) && !IsMarked(testNextFace)))
			{
				//we only swap if it buys us something

				//add a "fake" degenerate face
				NvFaceInfo* tempFace = new NvFaceInfo(nv0, nv1, nv0, true);

				forwardFaces.push_back(tempFace);
				MarkTriangle(tempFace);

				scratchIndices.push_back(nv0);
				testnv0 = nv0;

				++m_numDegenerates;
			}

		}

		// add this to the strip
		forwardFaces.push_back(nextFace);

		MarkTriangle(nextFace);

		// add the index
		//nv0 = nv1;
		//nv1 = NvStripifier::GetNextIndex(scratchIndices, nextFace);
		scratchIndices.push_back(testnv1);

		// and get the next face
		nv0 = testnv0;
		nv1 = testnv1;

		nextFace = NvStripifier::FindOtherFace(edgeInfos, nv0, nv1, nextFace);

	}

	// tempAllFaces is going to be forwardFaces + backwardFaces
	// it's used for Unique()
	NvFaceInfoVec tempAllFaces;
	for (int i = 0; i < forwardFaces.size(); i++)
		tempAllFaces.push_back(forwardFaces[i]);

	//
	// reset the indices for building the strip backwards and do so
	//
	scratchIndices.resize(0);
	scratchIndices.push_back(v2);
	scratchIndices.push_back(v1);
	scratchIndices.push_back(v0);
	nv0 = v1;
	nv1 = v0;
	nextFace = NvStripifier::FindOtherFace(edgeInfos, nv0, nv1, m_startInfo.m_startFace);
	while (nextFace != NULL && !IsMarked(nextFace))
	{
		//this tests to see if a face is "unique", meaning that its vertices aren't already in the list
		// so, strips which "wrap-around" are not allowed
		if (!Unique(tempAllFaces, nextFace))
			break;

		//check to see if this next face is going to cause us to die soon
		int testnv0 = nv1;
		int testnv1 = NvStripifier::GetNextIndex(scratchIndices, nextFace);

		NvFaceInfo* nextNextFace = NvStripifier::FindOtherFace(edgeInfos, testnv0, testnv1, nextFace);

		if ((nextNextFace == NULL) || (IsMarked(nextNextFace)))
		{
			//uh, oh, we're following a dead end, try swapping
			NvFaceInfo* testNextFace = NvStripifier::FindOtherFace(edgeInfos, nv0, testnv1, nextFace);
			if (((testNextFace != NULL) && !IsMarked(testNextFace)))
			{
				//we only swap if it buys us something

				//add a "fake" degenerate face
				NvFaceInfo* tempFace = new NvFaceInfo(nv0, nv1, nv0, true);

				backwardFaces.push_back(tempFace);
				MarkTriangle(tempFace);
				scratchIndices.push_back(nv0);
				testnv0 = nv0;

				++m_numDegenerates;
			}

		}

		// add this to the strip
		backwardFaces.push_back(nextFace);

		//this is just so Unique() will work
		tempAllFaces.push_back(nextFace);

		MarkTriangle(nextFace);

		// add the index
		//nv0 = nv1;
		//nv1 = NvStripifier::GetNextIndex(scratchIndices, nextFace);
		scratchIndices.push_back(testnv1);

		// and get the next face
		nv0 = testnv0;
		nv1 = testnv1;
		nextFace = NvStripifier::FindOtherFace(edgeInfos, nv0, nv1, nextFace);
	}

	// Combine the forward and backwards stripification lists and put into our own face vector
	Combine(forwardFaces, backwardFaces);
}


///////////////////////////////////////////////////////////////////////////////////////////
// Combine()
//
// Combines the two input face vectors and puts the result into m_faces
//
void NvStripInfo::Combine(const NvFaceInfoVec &forward, const NvFaceInfoVec &backward){

	// add backward faces
	int numFaces = backward.size();
	int i;
	for (i = numFaces - 1; i >= 0; i--)
		m_faces.push_back(backward[i]);

	// add forward faces
	numFaces = forward.size();
	for (i = 0; i < numFaces; i++)
		m_faces.push_back(forward[i]);
}


///////////////////////////////////////////////////////////////////////////////////////////
// SharesEdge()
//
// Returns true if the input face and the current strip share an edge
//
bool NvStripInfo::SharesEdge(const NvFaceInfo* faceInfo, NvEdgeInfoVec &edgeInfos)
{
	//check v0->v1 edge
	NvEdgeInfo* currEdge = NvStripifier::FindEdgeInfo(edgeInfos, faceInfo->m_v0, faceInfo->m_v1);

	if (IsInStrip(currEdge->m_face0) || IsInStrip(currEdge->m_face1))
		return true;

	//check v1->v2 edge
	currEdge = NvStripifier::FindEdgeInfo(edgeInfos, faceInfo->m_v1, faceInfo->m_v2);

	if (IsInStrip(currEdge->m_face0) || IsInStrip(currEdge->m_face1))
		return true;

	//check v2->v0 edge
	currEdge = NvStripifier::FindEdgeInfo(edgeInfos, faceInfo->m_v2, faceInfo->m_v0);

	if (IsInStrip(currEdge->m_face0) || IsInStrip(currEdge->m_face1))
		return true;

	return false;

}


///////////////////////////////////////////////////////////////////////////////////////////
// CommitStrips()
//
// "Commits" the input strips by setting their m_experimentId to -1 and adding to the allStrips
//  vector
//
void NvStripifier::CommitStrips(NvStripInfoVec &allStrips, const NvStripInfoVec &strips)
{
	// Iterate through strips
	int numStrips = strips.size();
	for (int i = 0; i < numStrips; i++){

		// Tell the strip that it is now real
		NvStripInfo *strip = strips[i];
		strip->m_experimentId = -1;

		// add to the list of real strips
		allStrips.push_back(strip);

		// Iterate through the faces of the strip
		// Tell the faces of the strip that they belong to a real strip now
		const NvFaceInfoVec &faces = strips[i]->m_faces;
		int numFaces = faces.size();

		for (int j = 0; j < numFaces; j++)
		{
			strip->MarkTriangle(faces[j]);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////
// FindTraversal()
//
// Finds the next face to start the next strip on.
//
bool NvStripifier::FindTraversal(NvFaceInfoVec &faceInfos,
	NvEdgeInfoVec    &edgeInfos,
	NvStripInfo      *strip,
	NvStripStartInfo &startInfo){

	// if the strip was v0->v1 on the edge, then v1 will be a vertex in the next edge.
	int v = (strip->m_startInfo.m_toV1 ? strip->m_startInfo.m_startEdge->m_v1 : strip->m_startInfo.m_startEdge->m_v0);

	NvFaceInfo *untouchedFace = NULL;
	NvEdgeInfo *edgeIter = edgeInfos[v];
	while (edgeIter != NULL){
		NvFaceInfo *face0 = edgeIter->m_face0;
		NvFaceInfo *face1 = edgeIter->m_face1;
		if ((face0 != NULL && !strip->IsInStrip(face0)) && face1 != NULL && !strip->IsMarked(face1))
		{
			untouchedFace = face1;
			break;
		}
		if ((face1 != NULL && !strip->IsInStrip(face1)) && face0 != NULL && !strip->IsMarked(face0)){
			untouchedFace = face0;
			break;
		}

		// find the next edgeIter
		edgeIter = (edgeIter->m_v0 == v ? edgeIter->m_nextV0 : edgeIter->m_nextV1);
	}

	startInfo.m_startFace = untouchedFace;
	startInfo.m_startEdge = edgeIter;
	if (edgeIter != NULL)
	{
		if (strip->SharesEdge(startInfo.m_startFace, edgeInfos))
			startInfo.m_toV1 = (edgeIter->m_v0 == v);  //note! used to be m_v1
		else
			startInfo.m_toV1 = (edgeIter->m_v1 == v);
	}
	return (startInfo.m_startFace != NULL);
}


////////////////////////////////////////////////////////////////////////////////////////
// RemoveSmallStrips()
//
// allStrips is the whole strip vector...all small strips will be deleted from this list, to avoid leaking mem
// allBigStrips is an out parameter which will contain all strips above minStripLength
// faceList is an out parameter which will contain all faces which were removed from the striplist
//
void NvStripifier::RemoveSmallStrips(NvStripInfoVec& allStrips, NvStripInfoVec& allBigStrips, NvFaceInfoVec& faceList)
{
	faceList.clear();
	allBigStrips.clear();  //make sure these are empty
	NvFaceInfoVec tempFaceList;

	for (int i = 0; i < allStrips.size(); i++)
	{
		if (allStrips[i]->m_faces.size() < minStripLength)
		{
			//strip is too small, add faces to faceList
			for (int j = 0; j < allStrips[i]->m_faces.size(); j++)
				tempFaceList.push_back(allStrips[i]->m_faces[j]);

			//and free memory
			delete allStrips[i];
		}
		else
		{
			allBigStrips.push_back(allStrips[i]);
		}
	}

	if (tempFaceList.size())
	{
		bool *bVisitedList = new bool[tempFaceList.size()];
		memset(bVisitedList, 0, tempFaceList.size()*sizeof(bool));

		VertexCache* vcache = new VertexCache(cacheSize);

		int bestNumHits = -1;
		int numHits;
		int bestIndex;

		while (1)
		{
			bestNumHits = -1;

			//find best face to add next, given the current cache
			for (int i = 0; i < tempFaceList.size(); i++)
			{
				if (bVisitedList[i])
					continue;

				numHits = CalcNumHitsFace(vcache, tempFaceList[i]);
				if (numHits > bestNumHits)
				{
					bestNumHits = numHits;
					bestIndex = i;
				}
			}

			if (bestNumHits == -1.0f)
				break;
			bVisitedList[bestIndex] = true;
			UpdateCacheFace(vcache, tempFaceList[bestIndex]);
			faceList.push_back(tempFaceList[bestIndex]);
		}

		delete vcache;
		delete[] bVisitedList;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////
// NextIsCW()
//
// Returns true if the next face should be ordered in CW fashion
//
bool NvStripifier::NextIsCW(const int numIndices)
{
	return ((numIndices % 2) == 0);
}


///////////////////////////////////////////////////////////////////////////////////////////
// IsCW()
//
// Returns true if the face is ordered in CW fashion
//
bool NvStripifier::IsCW(NvFaceInfo *faceInfo, int v0, int v1)
{
	if (faceInfo->m_v0 == v0)
		return (faceInfo->m_v1 == v1);

	else if (faceInfo->m_v1 == v0)
		return (faceInfo->m_v2 == v1);

	else
		return (faceInfo->m_v0 == v1);

	// shouldn't get here
	assert(0);
	return false;
}

bool NvStripifier::FaceContainsIndex(const NvFaceInfo& face, const unsigned long index)
{
	return ((face.m_v0 == index) || (face.m_v1 == index) || (face.m_v2 == index));
}

bool NvStripifier::IsMoneyFace(const NvFaceInfo& face)
{
	if (FaceContainsIndex(face, 800) &&
		FaceContainsIndex(face, 812) &&
		FaceContainsIndex(face, 731))
		return true;

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////
// CreateStrips()
//
// Generates actual strips from the list-in-strip-order.
//
void NvStripifier::CreateStrips(const NvStripInfoVec& allStrips, IntVec& stripIndices,
	const bool bStitchStrips, unsigned long& numSeparateStrips,
	const bool bRestart, const unsigned long restartVal)
{
	assert(numSeparateStrips == 0);

	NvFaceInfo tLastFace(0, 0, 0);
	NvFaceInfo tPrevStripLastFace(0, 0, 0);
	int nStripCount = allStrips.size();
	assert(nStripCount > 0);

	//we infer the cw/ccw ordering depending on the number of indices
	//this is screwed up by the fact that we insert -1s to denote changing strips
	//this is to account for that
	int accountForNegatives = 0;

	for (int i = 0; i < nStripCount; i++)
	{
		NvStripInfo *strip = allStrips[i];
		int nStripFaceCount = strip->m_faces.size();
		assert(nStripFaceCount > 0);

		// Handle the first face in the strip
		{
			NvFaceInfo tFirstFace(strip->m_faces[0]->m_v0, strip->m_faces[0]->m_v1, strip->m_faces[0]->m_v2);

			// If there is a second face, reorder vertices such that the
			// unique vertex is first
			if (nStripFaceCount > 1)
			{
				int nUnique = NvStripifier::GetUniqueVertexInB(strip->m_faces[1], &tFirstFace);
				if (nUnique == tFirstFace.m_v1)
				{
					SWAP(tFirstFace.m_v0, tFirstFace.m_v1);
				}
				else if (nUnique == tFirstFace.m_v2)
				{
					SWAP(tFirstFace.m_v0, tFirstFace.m_v2);
				}

				// If there is a third face, reorder vertices such that the
				// shared vertex is last
				if (nStripFaceCount > 2)
				{
					if (IsDegenerate(strip->m_faces[1]))
					{
						int pivot = strip->m_faces[1]->m_v1;
						if (tFirstFace.m_v1 == pivot)
						{
							SWAP(tFirstFace.m_v1, tFirstFace.m_v2);
						}
					}
					else
					{
						int nShared0, nShared1;
						GetSharedVertices(strip->m_faces[2], &tFirstFace, &nShared0, &nShared1);
						if ((nShared0 == tFirstFace.m_v1) && (nShared1 == -1))
						{
							SWAP(tFirstFace.m_v1, tFirstFace.m_v2);
						}
					}
				}
			}

			if ((i == 0) || !bStitchStrips || bRestart)
			{
				if (!IsCW(strip->m_faces[0], tFirstFace.m_v0, tFirstFace.m_v1))
					stripIndices.push_back(tFirstFace.m_v0);
			}
			else
			{
				// Double tap the first in the new strip
				stripIndices.push_back(tFirstFace.m_v0);

				// Check CW/CCW ordering
				if (NextIsCW(stripIndices.size() - accountForNegatives) != IsCW(strip->m_faces[0], tFirstFace.m_v0, tFirstFace.m_v1))
				{
					stripIndices.push_back(tFirstFace.m_v0);
				}
			}

			stripIndices.push_back(tFirstFace.m_v0);
			stripIndices.push_back(tFirstFace.m_v1);
			stripIndices.push_back(tFirstFace.m_v2);

			// Update last face info
			tLastFace = tFirstFace;
		}

		for (int j = 1; j < nStripFaceCount; j++)
		{
			int nUnique = GetUniqueVertexInB(&tLastFace, strip->m_faces[j]);
			if (nUnique != -1)
			{
				stripIndices.push_back(nUnique);

				// Update last face info
				tLastFace.m_v0 = tLastFace.m_v1;
				tLastFace.m_v1 = tLastFace.m_v2;
				tLastFace.m_v2 = nUnique;
			}
			else
			{
				//we've hit a degenerate
				stripIndices.push_back(strip->m_faces[j]->m_v2);
				tLastFace.m_v0 = strip->m_faces[j]->m_v0;//tLastFace.m_v1;
				tLastFace.m_v1 = strip->m_faces[j]->m_v1;//tLastFace.m_v2;
				tLastFace.m_v2 = strip->m_faces[j]->m_v2;//tLastFace.m_v1;

			}
		}

		// Double tap between strips.
		if (bStitchStrips && !bRestart)
		{
			if (i != nStripCount - 1)
				stripIndices.push_back(tLastFace.m_v2);
		}
		else if (bRestart)
		{
			stripIndices.push_back(restartVal);
		}
		else
		{
			//-1 index indicates next strip
			stripIndices.push_back(-1);
			accountForNegatives++;
			numSeparateStrips++;
		}

		// Update last face info
		tLastFace.m_v0 = tLastFace.m_v1;
		tLastFace.m_v1 = tLastFace.m_v2;
		tLastFace.m_v2 = tLastFace.m_v2;
	}

	if (bStitchStrips || bRestart)
		numSeparateStrips = 1;
}


///////////////////////////////////////////////////////////////////////////////////////////
// Stripify()
//
//
// in_indices are the input indices of the mesh to stripify
// in_cacheSize is the target cache size 
//
void NvStripifier::Stripify(const WordVec &in_indices, const int in_cacheSize,
	const int in_minStripLength, const unsigned long maxIndex,
	NvStripInfoVec &outStrips, NvFaceInfoVec& outFaceList)
{
	meshJump = 0.0f;
	bFirstTimeResetPoint = true; //used in FindGoodResetPoint()

	//the number of times to run the experiments
	int numSamples = 10;

	//the cache size, clamped to one
	cacheSize = max(1, in_cacheSize - CACHE_INEFFICIENCY);

	minStripLength = in_minStripLength;  //this is the strip size threshold below which we dump the strip into a list

	indices = in_indices;

	// build the stripification info
	NvFaceInfoVec allFaceInfos;
	NvEdgeInfoVec allEdgeInfos;

	BuildStripifyInfo(allFaceInfos, allEdgeInfos, maxIndex);

	NvStripInfoVec allStrips;

	// stripify
	FindAllStrips(allStrips, allFaceInfos, allEdgeInfos, numSamples);

	//split up the strips into cache friendly pieces, optimize them, then dump these into outStrips
	SplitUpStripsAndOptimize(allStrips, outStrips, allEdgeInfos, outFaceList);

	//clean up
	int i;
	for (i = 0; i < allStrips.size(); i++)
	{
		delete allStrips[i];
	}

	for (i = 0; i < allEdgeInfos.size(); i++)
	{
		NvEdgeInfo *info = allEdgeInfos[i];
		while (info != NULL)
		{
			NvEdgeInfo *next = (info->m_v0 == i ? info->m_nextV0 : info->m_nextV1);
			info->Unref();
			info = next;
		}
	}

}


bool NvStripifier::IsDegenerate(const NvFaceInfo* face)
{
	if (face->m_v0 == face->m_v1)
		return true;
	else if (face->m_v0 == face->m_v2)
		return true;
	else if (face->m_v1 == face->m_v2)
		return true;
	else
		return false;
}

bool NvStripifier::IsDegenerate(const unsigned long v0, const unsigned long v1, const unsigned long v2)
{
	if (v0 == v1)
		return true;
	else if (v0 == v2)
		return true;
	else if (v1 == v2)
		return true;
	else
		return false;
}

///////////////////////////////////////////////////////////////////////////////////////////
// SplitUpStripsAndOptimize()
//
// Splits the input vector of strips (allBigStrips) into smaller, cache friendly pieces, then
//  reorders these pieces to maximize cache hits
// The final strips are output through outStrips
//
void NvStripifier::SplitUpStripsAndOptimize(NvStripInfoVec &allStrips, NvStripInfoVec &outStrips,
	NvEdgeInfoVec& edgeInfos, NvFaceInfoVec& outFaceList)
{
	int threshold = cacheSize;
	NvStripInfoVec tempStrips;

	//split up strips into threshold-sized pieces
	for (int i = 0; i < allStrips.size(); i++)
	{
		NvStripInfo* currentStrip;
		NvStripStartInfo startInfo(NULL, NULL, false);

		int actualStripSize = 0;
		for (int j = 0; j < allStrips[i]->m_faces.size(); ++j)
		{
			if (!IsDegenerate(allStrips[i]->m_faces[j]))
				actualStripSize++;
		}

		if (actualStripSize /*allStrips[i]->m_faces.size()*/ > threshold)
		{

			int numTimes = actualStripSize /*allStrips[i]->m_faces.size()*/ / threshold;
			int numLeftover = actualStripSize /*allStrips[i]->m_faces.size()*/ % threshold;

			int degenerateCount = 0;
			int j;
			for (j = 0; j < numTimes; j++)
			{
				currentStrip = new NvStripInfo(startInfo, 0, -1);

				int faceCtr = j*threshold + degenerateCount;
				bool bFirstTime = true;
				while (faceCtr < threshold + (j*threshold) + degenerateCount)
				{
					if (IsDegenerate(allStrips[i]->m_faces[faceCtr]))
					{
						degenerateCount++;

						//last time or first time through, no need for a degenerate
						if ((((faceCtr + 1) != threshold + (j*threshold) + degenerateCount) ||
							((j == numTimes - 1) && (numLeftover < 4) && (numLeftover > 0))) &&
							!bFirstTime)
						{
							currentStrip->m_faces.push_back(allStrips[i]->m_faces[faceCtr++]);
						}
						else
						{
							//but, we do need to delete the degenerate, if it's marked fake, to avoid leaking
							if (allStrips[i]->m_faces[faceCtr]->m_bIsFake)
							{
								delete allStrips[i]->m_faces[faceCtr], allStrips[i]->m_faces[faceCtr] = NULL;
							}
							++faceCtr;
						}
					}
					else
					{
						currentStrip->m_faces.push_back(allStrips[i]->m_faces[faceCtr++]);
						bFirstTime = false;
					}
				}
				/*
				for(int faceCtr = j*threshold; faceCtr < threshold+(j*threshold); faceCtr++)
				{
				currentStrip->m_faces.push_back(allStrips[i]->m_faces[faceCtr]);
				}
				*/
				if (j == numTimes - 1) //last time through
				{
					if ((numLeftover < 4) && (numLeftover > 0)) //way too small
					{
						//just add to last strip
						int ctr = 0;
						while (ctr < numLeftover)
						{
							IsDegenerate(allStrips[i]->m_faces[faceCtr]) ? ++degenerateCount : ++ctr;
							currentStrip->m_faces.push_back(allStrips[i]->m_faces[faceCtr++]);
						}
						numLeftover = 0;
					}
				}
				tempStrips.push_back(currentStrip);
			}

			int leftOff = j * threshold + degenerateCount;

			if (numLeftover != 0)
			{
				currentStrip = new NvStripInfo(startInfo, 0, -1);

				int ctr = 0;
				bool bFirstTime = true;
				while (ctr < numLeftover)
				{
					if (!IsDegenerate(allStrips[i]->m_faces[leftOff]))
					{
						ctr++;
						bFirstTime = false;
						currentStrip->m_faces.push_back(allStrips[i]->m_faces[leftOff++]);
					}
					else if (!bFirstTime)
						currentStrip->m_faces.push_back(allStrips[i]->m_faces[leftOff++]);
					else
					{
						//don't leak
						if (allStrips[i]->m_faces[leftOff]->m_bIsFake)
						{
							delete allStrips[i]->m_faces[leftOff], allStrips[i]->m_faces[leftOff] = NULL;
						}

						leftOff++;
					}
				}
				/*
				for(int k = 0; k < numLeftover; k++)
				{
				currentStrip->m_faces.push_back(allStrips[i]->m_faces[leftOff++]);
				}
				*/

				tempStrips.push_back(currentStrip);
			}
		}
		else
		{
			//we're not just doing a tempStrips.push_back(allBigStrips[i]) because
			// this way we can delete allBigStrips later to free the memory
			currentStrip = new NvStripInfo(startInfo, 0, -1);

			for (int j = 0; j < allStrips[i]->m_faces.size(); j++)
				currentStrip->m_faces.push_back(allStrips[i]->m_faces[j]);

			tempStrips.push_back(currentStrip);
		}
	}

	//add small strips to face list
	NvStripInfoVec tempStrips2;
	RemoveSmallStrips(tempStrips, tempStrips2, outFaceList);

	outStrips.clear();
	//screw optimization for now
	//	for(i = 0; i < tempStrips.size(); ++i)
	//    outStrips.push_back(tempStrips[i]);

	if (tempStrips2.size() != 0)
	{
		//Optimize for the vertex cache
		VertexCache* vcache = new VertexCache(cacheSize);

		float bestNumHits = -1.0f;
		float numHits;
		int bestIndex;
		bool done = false;

		int firstIndex = 0;
		float minCost = 10000.0f;
		int i;
		for (i = 0; i < tempStrips2.size(); i++)
		{
			int numNeighbors = 0;

			//find strip with least number of neighbors per face
			for (int j = 0; j < tempStrips2[i]->m_faces.size(); j++)
			{
				numNeighbors += NumNeighbors(tempStrips2[i]->m_faces[j], edgeInfos);
			}

			float currCost = (float)numNeighbors / (float)tempStrips2[i]->m_faces.size();
			if (currCost < minCost)
			{
				minCost = currCost;
				firstIndex = i;
			}
		}

		UpdateCacheStrip(vcache, tempStrips2[firstIndex]);
		outStrips.push_back(tempStrips2[firstIndex]);

		tempStrips2[firstIndex]->visited = true;

		bool bWantsCW = (tempStrips2[firstIndex]->m_faces.size() % 2) == 0;

		//this n^2 algo is what slows down stripification so much....
		// needs to be improved
		while (1)
		{
			bestNumHits = -1.0f;

			//find best strip to add next, given the current cache
			for (int i = 0; i < tempStrips2.size(); i++)
			{
				if (tempStrips2[i]->visited)
					continue;

				numHits = CalcNumHitsStrip(vcache, tempStrips2[i]);
				if (numHits > bestNumHits)
				{
					bestNumHits = numHits;
					bestIndex = i;
				}
				else if (numHits >= bestNumHits)
				{
					//check previous strip to see if this one requires it to switch polarity
					NvStripInfo *strip = tempStrips2[i];
					int nStripFaceCount = strip->m_faces.size();

					NvFaceInfo tFirstFace(strip->m_faces[0]->m_v0, strip->m_faces[0]->m_v1, strip->m_faces[0]->m_v2);

					// If there is a second face, reorder vertices such that the
					// unique vertex is first
					if (nStripFaceCount > 1)
					{
						int nUnique = NvStripifier::GetUniqueVertexInB(strip->m_faces[1], &tFirstFace);
						if (nUnique == tFirstFace.m_v1)
						{
							SWAP(tFirstFace.m_v0, tFirstFace.m_v1);
						}
						else if (nUnique == tFirstFace.m_v2)
						{
							SWAP(tFirstFace.m_v0, tFirstFace.m_v2);
						}

						// If there is a third face, reorder vertices such that the
						// shared vertex is last
						if (nStripFaceCount > 2)
						{
							int nShared0, nShared1;
							GetSharedVertices(strip->m_faces[2], &tFirstFace, &nShared0, &nShared1);
							if ((nShared0 == tFirstFace.m_v1) && (nShared1 == -1))
							{
								SWAP(tFirstFace.m_v1, tFirstFace.m_v2);
							}
						}
					}

					// Check CW/CCW ordering
					if (bWantsCW == IsCW(strip->m_faces[0], tFirstFace.m_v0, tFirstFace.m_v1))
					{
						//I like this one!
						bestIndex = i;
					}
				}
			}

			if (bestNumHits == -1.0f)
				break;
			tempStrips2[bestIndex]->visited = true;
			UpdateCacheStrip(vcache, tempStrips2[bestIndex]);
			outStrips.push_back(tempStrips2[bestIndex]);
			bWantsCW = (tempStrips2[bestIndex]->m_faces.size() % 2 == 0) ? bWantsCW : !bWantsCW;
		}

		delete vcache;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////
// UpdateCacheStrip()
//
// Updates the input vertex cache with this strip's vertices
//
void NvStripifier::UpdateCacheStrip(VertexCache* vcache, NvStripInfo* strip)
{
	for (int i = 0; i < strip->m_faces.size(); ++i)
	{
		if (!vcache->InCache(strip->m_faces[i]->m_v0))
			vcache->AddEntry(strip->m_faces[i]->m_v0);

		if (!vcache->InCache(strip->m_faces[i]->m_v1))
			vcache->AddEntry(strip->m_faces[i]->m_v1);

		if (!vcache->InCache(strip->m_faces[i]->m_v2))
			vcache->AddEntry(strip->m_faces[i]->m_v2);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
// UpdateCacheFace()
//
// Updates the input vertex cache with this face's vertices
//
void NvStripifier::UpdateCacheFace(VertexCache* vcache, NvFaceInfo* face)
{
	if (!vcache->InCache(face->m_v0))
		vcache->AddEntry(face->m_v0);

	if (!vcache->InCache(face->m_v1))
		vcache->AddEntry(face->m_v1);

	if (!vcache->InCache(face->m_v2))
		vcache->AddEntry(face->m_v2);
}


///////////////////////////////////////////////////////////////////////////////////////////
// CalcNumHitsStrip()
//
// returns the number of cache hits per face in the strip
//
float NvStripifier::CalcNumHitsStrip(VertexCache* vcache, NvStripInfo* strip)
{
	int numHits = 0;
	int numFaces = 0;

	for (int i = 0; i < strip->m_faces.size(); i++)
	{
		if (vcache->InCache(strip->m_faces[i]->m_v0))
			++numHits;

		if (vcache->InCache(strip->m_faces[i]->m_v1))
			++numHits;

		if (vcache->InCache(strip->m_faces[i]->m_v2))
			++numHits;

		numFaces++;
	}

	return ((float)numHits / (float)numFaces);
}


///////////////////////////////////////////////////////////////////////////////////////////
// CalcNumHitsFace()
//
// returns the number of cache hits in the face
//
int NvStripifier::CalcNumHitsFace(VertexCache* vcache, NvFaceInfo* face)
{
	int numHits = 0;

	if (vcache->InCache(face->m_v0))
		numHits++;

	if (vcache->InCache(face->m_v1))
		numHits++;

	if (vcache->InCache(face->m_v2))
		numHits++;

	return numHits;
}


///////////////////////////////////////////////////////////////////////////////////////////
// NumNeighbors()
//
// Returns the number of neighbors that this face has
//
int NvStripifier::NumNeighbors(NvFaceInfo* face, NvEdgeInfoVec& edgeInfoVec)
{
	int numNeighbors = 0;

	if (FindOtherFace(edgeInfoVec, face->m_v0, face->m_v1, face) != NULL)
	{
		numNeighbors++;
	}

	if (FindOtherFace(edgeInfoVec, face->m_v1, face->m_v2, face) != NULL)
	{
		numNeighbors++;
	}

	if (FindOtherFace(edgeInfoVec, face->m_v2, face->m_v0, face) != NULL)
	{
		numNeighbors++;
	}

	return numNeighbors;
}


///////////////////////////////////////////////////////////////////////////////////////////
// AvgStripSize()
//
// Finds the average strip size of the input vector of strips
//
float NvStripifier::AvgStripSize(const NvStripInfoVec &strips){
	int sizeAccum = 0;
	int numStrips = strips.size();
	for (int i = 0; i < numStrips; i++){
		NvStripInfo *strip = strips[i];
		sizeAccum += strip->m_faces.size();
		sizeAccum -= strip->m_numDegenerates;
	}
	return ((float)sizeAccum) / ((float)numStrips);
}


///////////////////////////////////////////////////////////////////////////////////////////
// FindAllStrips()
//
// Does the stripification, puts output strips into vector allStrips
//
// Works by setting runnning a number of experiments in different areas of the mesh, and
//  accepting the one which results in the longest strips.  It then accepts this, and moves
//  on to a different area of the mesh.  We try to jump around the mesh some, to ensure that
//  large open spans of strips get generated.
//
void NvStripifier::FindAllStrips(NvStripInfoVec &allStrips,
	NvFaceInfoVec &allFaceInfos,
	NvEdgeInfoVec &allEdgeInfos,
	int numSamples){
	// the experiments
	int experimentId = 0;
	int stripId = 0;
	bool done = false;

	int loopCtr = 0;

	while (!done)
	{
		loopCtr++;

		//
		// PHASE 1: Set up numSamples * numEdges experiments
		//
		NvStripInfoVec *experiments = new NvStripInfoVec[numSamples * 6];
		int experimentIndex = 0;
		std::set   <NvFaceInfo*>  resetPoints;
		int i;
		for (i = 0; i < numSamples; i++)
		{

			// Try to find another good reset point.
			// If there are none to be found, we are done
			NvFaceInfo *nextFace = FindGoodResetPoint(allFaceInfos, allEdgeInfos);
			if (nextFace == NULL){
				done = true;
				break;
			}
			// If we have already evaluated starting at this face in this slew
			// of experiments, then skip going any further
			else if (resetPoints.find(nextFace) != resetPoints.end()){
				continue;
			}

			// trying it now...
			resetPoints.insert(nextFace);

			// otherwise, we shall now try experiments for starting on the 01,12, and 20 edges
			assert(nextFace->m_stripId < 0);

			// build the strip off of this face's 0-1 edge
			NvEdgeInfo *edge01 = FindEdgeInfo(allEdgeInfos, nextFace->m_v0, nextFace->m_v1);
			NvStripInfo *strip01 = new NvStripInfo(NvStripStartInfo(nextFace, edge01, true), stripId++, experimentId++);
			experiments[experimentIndex++].push_back(strip01);

			// build the strip off of this face's 1-0 edge
			NvEdgeInfo *edge10 = FindEdgeInfo(allEdgeInfos, nextFace->m_v0, nextFace->m_v1);
			NvStripInfo *strip10 = new NvStripInfo(NvStripStartInfo(nextFace, edge10, false), stripId++, experimentId++);
			experiments[experimentIndex++].push_back(strip10);

			// build the strip off of this face's 1-2 edge
			NvEdgeInfo *edge12 = FindEdgeInfo(allEdgeInfos, nextFace->m_v1, nextFace->m_v2);
			NvStripInfo *strip12 = new NvStripInfo(NvStripStartInfo(nextFace, edge12, true), stripId++, experimentId++);
			experiments[experimentIndex++].push_back(strip12);

			// build the strip off of this face's 2-1 edge
			NvEdgeInfo *edge21 = FindEdgeInfo(allEdgeInfos, nextFace->m_v1, nextFace->m_v2);
			NvStripInfo *strip21 = new NvStripInfo(NvStripStartInfo(nextFace, edge21, false), stripId++, experimentId++);
			experiments[experimentIndex++].push_back(strip21);

			// build the strip off of this face's 2-0 edge
			NvEdgeInfo *edge20 = FindEdgeInfo(allEdgeInfos, nextFace->m_v2, nextFace->m_v0);
			NvStripInfo *strip20 = new NvStripInfo(NvStripStartInfo(nextFace, edge20, true), stripId++, experimentId++);
			experiments[experimentIndex++].push_back(strip20);

			// build the strip off of this face's 0-2 edge
			NvEdgeInfo *edge02 = FindEdgeInfo(allEdgeInfos, nextFace->m_v2, nextFace->m_v0);
			NvStripInfo *strip02 = new NvStripInfo(NvStripStartInfo(nextFace, edge02, false), stripId++, experimentId++);
			experiments[experimentIndex++].push_back(strip02);
		}

		//
		// PHASE 2: Iterate through that we setup in the last phase
		// and really build each of the strips and strips that follow to see how
		// far we get
		//
		int numExperiments = experimentIndex;
		for (i = 0; i < numExperiments; i++){

			// get the strip set

			// build the first strip of the list
			experiments[i][0]->Build(allEdgeInfos, allFaceInfos);
			int experimentId = experiments[i][0]->m_experimentId;

			NvStripInfo *stripIter = experiments[i][0];
			NvStripStartInfo startInfo(NULL, NULL, false);
			while (FindTraversal(allFaceInfos, allEdgeInfos, stripIter, startInfo)){

				// create the new strip info
				stripIter = new NvStripInfo(startInfo, stripId++, experimentId);

				// build the next strip
				stripIter->Build(allEdgeInfos, allFaceInfos);

				// add it to the list
				experiments[i].push_back(stripIter);
			}
		}

		//
		// Phase 3: Find the experiment that has the most promise
		//
		int bestIndex = 0;
		double bestValue = 0;
		for (i = 0; i < numExperiments; i++)
		{
			const float avgStripSizeWeight = 1.0f;
			const float numTrisWeight = 0.0f;
			const float numStripsWeight = 0.0f;
			float avgStripSize = AvgStripSize(experiments[i]);
			float numStrips = (float)experiments[i].size();
			float value = avgStripSize * avgStripSizeWeight + (numStrips * numStripsWeight);
			//float value = 1.f / numStrips;
			//float value = numStrips * avgStripSize;

			if (value > bestValue)
			{
				bestValue = value;
				bestIndex = i;
			}
		}

		//
		// Phase 4: commit the best experiment of the bunch
		//
		CommitStrips(allStrips, experiments[bestIndex]);

		// and destroy all of the others
		for (i = 0; i < numExperiments; i++)
		{
			if (i != bestIndex)
			{
				int numStrips = experiments[i].size();
				for (int j = 0; j < numStrips; j++)
				{
					NvStripInfo* currStrip = experiments[i][j];
					//delete all bogus faces in the experiments
					for (int k = 0; k < currStrip->m_faces.size(); ++k)
					{
						if (currStrip->m_faces[k]->m_bIsFake)
						{
							delete currStrip->m_faces[k], currStrip->m_faces[k] = NULL;
						}
					}
					delete currStrip, currStrip = NULL, experiments[i][j] = NULL;
				}
			}
		}

		// delete the array that we used for all experiments
		delete[] experiments;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////
// CountRemainingTris()
//
// This will count the number of triangles left in the
// strip list starting at iter and finishing up at end
//
int NvStripifier::CountRemainingTris(std::list<NvStripInfo*>::iterator iter,
	std::list<NvStripInfo*>::iterator  end){
	int count = 0;
	while (iter != end){
		count += (*iter)->m_faces.size();
		iter++;
	}
	return count;
}

//
////////////////////////////////////////////////////////////////////////////////////////
//private data
static unsigned long cacheSize = CACHESIZE;
static bool bStitchStrips = true;
static unsigned long minStripSize = 0;
static bool bListsOnly = false;
static unsigned long restartVal = 0;
static bool bRestart = false;

void EnableRestart(const unsigned long _restartVal)
{
	bRestart = true;
	restartVal = _restartVal;
}

void DisableRestart()
{
	bRestart = false;
}

////////////////////////////////////////////////////////////////////////////////////////
// SetListsOnly()
//
// If set to true, will return an optimized list, with no strips at all.
//
// Default value: false
//
void SetListsOnly(const bool _bListsOnly)
{
	bListsOnly = _bListsOnly;
}

////////////////////////////////////////////////////////////////////////////////////////
// SetCacheSize()
//
// Sets the cache size which the stripfier uses to optimize the data.
// Controls the length of the generated individual strips.
// This is the "actual" cache size, so 24 for GeForce3 and 16 for GeForce1/2
// You may want to play around with this number to tweak performance.
//
// Default value: 16
//
void SetCacheSize(const unsigned long _cacheSize)
{
	cacheSize = _cacheSize;
}


////////////////////////////////////////////////////////////////////////////////////////
// SetStitchStrips()
//
// bool to indicate whether to stitch together strips into one huge strip or not.
// If set to true, you'll get back one huge strip stitched together using degenerate
//  triangles.
// If set to false, you'll get back a large number of separate strips.
//
// Default value: true
//
void SetStitchStrips(const bool _bStitchStrips)
{
	bStitchStrips = _bStitchStrips;
}


////////////////////////////////////////////////////////////////////////////////////////
// SetMinStripSize()
//
// Sets the minimum acceptable size for a strip, in triangles.
// All strips generated which are shorter than this will be thrown into one big, separate list.
//
// Default value: 0
//
void SetMinStripSize(const unsigned long _minStripSize)
{
	minStripSize = _minStripSize;
}


////////////////////////////////////////////////////////////////////////////////////////
//Cleanup strips / faces, used by generatestrips
void Cleanup(NvStripInfoVec& tempStrips, NvFaceInfoVec& tempFaces)
{
	//delete strips
	int i;
	for (i = 0; i < tempStrips.size(); i++)
	{
		for (int j = 0; j < tempStrips[i]->m_faces.size(); j++)
		{
			delete tempStrips[i]->m_faces[j];
			tempStrips[i]->m_faces[j] = NULL;
		}
		tempStrips[i]->m_faces.resize(0);
		delete tempStrips[i];
		tempStrips[i] = NULL;
	}

	//delete faces
	for (i = 0; i < tempFaces.size(); i++)
	{
		delete tempFaces[i];
		tempFaces[i] = NULL;
	}
}


////////////////////////////////////////////////////////////////////////////////////////
//SameTriangle()
//
//Returns true if the two triangles defined by firstTri and secondTri are the same
// The "same" is defined in this case as having the same indices with the same winding order
//
bool SameTriangle(unsigned long firstTri0, unsigned long firstTri1, unsigned long firstTri2,
	unsigned long secondTri0, unsigned long secondTri1, unsigned long secondTri2)
{
	bool isSame = false;

	if (firstTri0 == secondTri0)
	{
		if (firstTri1 == secondTri1)
		{
			if (firstTri2 == secondTri2)
				isSame = true;
		}
	}
	else if (firstTri0 == secondTri1)
	{
		if (firstTri1 == secondTri2)
		{
			if (firstTri2 == secondTri0)
				isSame = true;
		}
	}
	else if (firstTri0 == secondTri2)
	{
		if (firstTri1 == secondTri0)
		{
			if (firstTri2 == secondTri1)
				isSame = true;
		}
	}

	return isSame;
}


bool TestTriangle(const unsigned long v0, const unsigned long v1, const unsigned long v2, const std::vector<NvFaceInfo>* in_bins, const int NUMBINS)
{
	//hash this triangle
	bool isLegit = false;
	int ctr = v0 % NUMBINS;
	for (int k = 0; k < in_bins[ctr].size(); ++k)
	{
		//check triangles in this bin
		if (SameTriangle(in_bins[ctr][k].m_v0, in_bins[ctr][k].m_v1, in_bins[ctr][k].m_v2,
			v0, v1, v2))
		{
			isLegit = true;
			break;
		}
	}
	if (!isLegit)
	{
		ctr = v1 % NUMBINS;
		for (int k = 0; k < in_bins[ctr].size(); ++k)
		{
			//check triangles in this bin
			if (SameTriangle(in_bins[ctr][k].m_v0, in_bins[ctr][k].m_v1, in_bins[ctr][k].m_v2,
				v0, v1, v2))
			{
				isLegit = true;
				break;
			}
		}

		if (!isLegit)
		{
			ctr = v2 % NUMBINS;
			for (int k = 0; k < in_bins[ctr].size(); ++k)
			{
				//check triangles in this bin
				if (SameTriangle(in_bins[ctr][k].m_v0, in_bins[ctr][k].m_v1, in_bins[ctr][k].m_v2,
					v0, v1, v2))
				{
					isLegit = true;
					break;
				}
			}

		}
	}

	return isLegit;
}


////////////////////////////////////////////////////////////////////////////////////////
// GenerateStrips()
//
// in_indices: input index list, the indices you would use to render
// in_numIndices: number of entries in in_indices
// primGroups: array of optimized/stripified PrimitiveGroups
// numGroups: number of groups returned
//
// Be sure to call delete[] on the returned primGroups to avoid leaking mem
//
bool GenerateStrips(const unsigned long* in_indices, const unsigned long in_numIndices,
	PrimitiveGroup** primGroups, unsigned long* numGroups, bool validateEnabled)
{
	//put data in format that the stripifier likes
	WordVec tempIndices;
	tempIndices.resize(in_numIndices);
	unsigned long maxIndex = 0;
	unsigned long minIndex = 0xFFFF;
	for (int i = 0; i < in_numIndices; i++)
	{
		tempIndices[i] = in_indices[i];
		if (in_indices[i] > maxIndex)
			maxIndex = in_indices[i];
		if (in_indices[i] < minIndex)
			minIndex = in_indices[i];
	}
	NvStripInfoVec tempStrips;
	NvFaceInfoVec tempFaces;

	NvStripifier stripifier;

	//do actual stripification
	stripifier.Stripify(tempIndices, cacheSize, minStripSize, maxIndex, tempStrips, tempFaces);

	//stitch strips together
	IntVec stripIndices;
	unsigned long numSeparateStrips = 0;

	if (bListsOnly)
	{
		//if we're outputting only lists, we're done
		*numGroups = 1;
		(*primGroups) = new PrimitiveGroup[*numGroups];
		PrimitiveGroup* primGroupArray = *primGroups;

		//count the total number of indices
		unsigned long numIndices = 0;
		int i;
		for (i = 0; i < tempStrips.size(); i++)
		{
			numIndices += tempStrips[i]->m_faces.size() * 3;
		}

		//add in the list
		numIndices += tempFaces.size() * 3;

		primGroupArray[0].type = PT_LIST;
		primGroupArray[0].numIndices = numIndices;
		primGroupArray[0].indices = new unsigned long[numIndices];

		//do strips
		unsigned long indexCtr = 0;
		for (i = 0; i < tempStrips.size(); i++)
		{
			for (int j = 0; j < tempStrips[i]->m_faces.size(); j++)
			{
				//degenerates are of no use with lists
				if (!NvStripifier::IsDegenerate(tempStrips[i]->m_faces[j]))
				{
					primGroupArray[0].indices[indexCtr++] = tempStrips[i]->m_faces[j]->m_v0;
					primGroupArray[0].indices[indexCtr++] = tempStrips[i]->m_faces[j]->m_v1;
					primGroupArray[0].indices[indexCtr++] = tempStrips[i]->m_faces[j]->m_v2;
				}
				else
				{
					//we've removed a tri, reduce the number of indices
					primGroupArray[0].numIndices -= 3;
				}
			}
		}

		//do lists
		for (i = 0; i < tempFaces.size(); i++)
		{
			primGroupArray[0].indices[indexCtr++] = tempFaces[i]->m_v0;
			primGroupArray[0].indices[indexCtr++] = tempFaces[i]->m_v1;
			primGroupArray[0].indices[indexCtr++] = tempFaces[i]->m_v2;
		}
	}
	else
	{
		stripifier.CreateStrips(tempStrips, stripIndices, bStitchStrips, numSeparateStrips, bRestart, restartVal);

		//if we're stitching strips together, we better get back only one strip from CreateStrips()
		assert((bStitchStrips && (numSeparateStrips == 1)) || !bStitchStrips);

		//convert to output format
		*numGroups = numSeparateStrips; //for the strips
		if (tempFaces.size() != 0)
			(*numGroups)++;  //we've got a list as well, increment
		(*primGroups) = new PrimitiveGroup[*numGroups];

		PrimitiveGroup* primGroupArray = *primGroups;

		//first, the strips
		int startingLoc = 0;
		for (int stripCtr = 0; stripCtr < numSeparateStrips; stripCtr++)
		{
			int stripLength = 0;

			if (!bStitchStrips)
			{
				//if we've got multiple strips, we need to figure out the correct length
				int i;
				for (i = startingLoc; i < stripIndices.size(); i++)
				{
					if (stripIndices[i] == -1)
						break;
				}

				stripLength = i - startingLoc;
			}
			else
				stripLength = stripIndices.size();

			primGroupArray[stripCtr].type = PT_STRIP;
			primGroupArray[stripCtr].indices = new unsigned long[stripLength];
			primGroupArray[stripCtr].numIndices = stripLength;

			int indexCtr = 0;
			for (int i = startingLoc; i < stripLength + startingLoc; i++)
				primGroupArray[stripCtr].indices[indexCtr++] = stripIndices[i];

			//we add 1 to account for the -1 separating strips
			//this doesn't break the stitched case since we'll exit the loop
			startingLoc += stripLength + 1;
		}

		//next, the list
		if (tempFaces.size() != 0)
		{
			int faceGroupLoc = (*numGroups) - 1;    //the face group is the last one
			primGroupArray[faceGroupLoc].type = PT_LIST;
			primGroupArray[faceGroupLoc].indices = new unsigned long[tempFaces.size() * 3];
			primGroupArray[faceGroupLoc].numIndices = tempFaces.size() * 3;
			int indexCtr = 0;
			for (int i = 0; i < tempFaces.size(); i++)
			{
				primGroupArray[faceGroupLoc].indices[indexCtr++] = tempFaces[i]->m_v0;
				primGroupArray[faceGroupLoc].indices[indexCtr++] = tempFaces[i]->m_v1;
				primGroupArray[faceGroupLoc].indices[indexCtr++] = tempFaces[i]->m_v2;
			}
		}
	}

	//validate generated data against input
	if (validateEnabled)
	{
		const int NUMBINS = 100;

		std::vector<NvFaceInfo> in_bins[NUMBINS];

		//hash input indices on first index
		int i;
		for (i = 0; i < in_numIndices; i += 3)
		{
			NvFaceInfo faceInfo(in_indices[i], in_indices[i + 1], in_indices[i + 2]);
			in_bins[in_indices[i] % NUMBINS].push_back(faceInfo);
		}

		for (i = 0; i < *numGroups; ++i)
		{
			switch ((*primGroups)[i].type)
			{
			case PT_LIST:
			{
				for (int j = 0; j < (*primGroups)[i].numIndices; j += 3)
				{
					unsigned long v0 = (*primGroups)[i].indices[j];
					unsigned long v1 = (*primGroups)[i].indices[j + 1];
					unsigned long v2 = (*primGroups)[i].indices[j + 2];

					//ignore degenerates
					if (NvStripifier::IsDegenerate(v0, v1, v2))
						continue;

					if (!TestTriangle(v0, v1, v2, in_bins, NUMBINS))
					{
						Cleanup(tempStrips, tempFaces);
						return false;
					}
				}
				break;
			}

			case PT_STRIP:
			{
				int brokenCtr = 0;
				bool flip = false;
				for (int j = 2; j < (*primGroups)[i].numIndices; ++j)
				{
					unsigned long v0 = (*primGroups)[i].indices[j - 2];
					unsigned long v1 = (*primGroups)[i].indices[j - 1];
					unsigned long v2 = (*primGroups)[i].indices[j];

					if (flip)
					{
						//swap v1 and v2
						unsigned long swap = v1;
						v1 = v2;
						v2 = swap;
					}

					//ignore degenerates
					if (NvStripifier::IsDegenerate(v0, v1, v2))
					{
						flip = !flip;
						continue;
					}

					if (!TestTriangle(v0, v1, v2, in_bins, NUMBINS))
					{
						Cleanup(tempStrips, tempFaces);
						return false;
					}

					flip = !flip;
				}
				break;
			}

			case PT_FAN:
			default:
				break;
			}
		}

	}

	//clean up everything
	Cleanup(tempStrips, tempFaces);

	return true;
}


////////////////////////////////////////////////////////////////////////////////////////
// RemapIndices()
//
// Function to remap your indices to improve spatial locality in your vertex buffer.
//
// in_primGroups: array of PrimitiveGroups you want remapped
// numGroups: number of entries in in_primGroups
// numVerts: number of vertices in your vertex buffer, also can be thought of as the range
//  of acceptable values for indices in your primitive groups.
// remappedGroups: array of remapped PrimitiveGroups
//
// Note that, according to the remapping handed back to you, you must reorder your 
//  vertex buffer.
//
void RemapIndices(const PrimitiveGroup* in_primGroups, const unsigned long numGroups,
	const unsigned long numVerts, PrimitiveGroup** remappedGroups)
{
	(*remappedGroups) = new PrimitiveGroup[numGroups];

	//caches oldIndex --> newIndex conversion
	int *indexCache;
	indexCache = new int[numVerts];
	memset(indexCache, -1, sizeof(int)*numVerts);

	//loop over primitive groups
	unsigned long indexCtr = 0;
	for (int i = 0; i < numGroups; i++)
	{
		unsigned long numIndices = in_primGroups[i].numIndices;

		//init remapped group
		(*remappedGroups)[i].type = in_primGroups[i].type;
		(*remappedGroups)[i].numIndices = numIndices;
		(*remappedGroups)[i].indices = new unsigned long[numIndices];

		for (int j = 0; j < numIndices; j++)
		{
			int cachedIndex = indexCache[in_primGroups[i].indices[j]];
			if (cachedIndex == -1) //we haven't seen this index before
			{
				//point to "last" vertex in VB
				(*remappedGroups)[i].indices[j] = indexCtr;

				//add to index cache, increment
				indexCache[in_primGroups[i].indices[j]] = indexCtr++;
			}
			else
			{
				//we've seen this index before
				(*remappedGroups)[i].indices[j] = cachedIndex;
			}
		}
	}

	delete[] indexCache;
}
////////////////////////////////////////////////////////////////////////////////////////
// GenerateStripsEx()
//
bool GenerateStripsEx(bool isStrip, const unsigned long* in_indices, const unsigned long in_numIndices, PrimitiveGroup** primGroups, unsigned long* numGroups)
{
	SetListsOnly(!isStrip);
	return GenerateStrips(in_indices, in_numIndices, primGroups, numGroups, false);
}

////////////////////////////////////////////////////////////////////////////////////////
// GenerateStripsExEx()
//
bool GenerateStripsExEx(bool isStrip, bool isStitch, const unsigned long in_MinStripSize,
	const unsigned long* in_indices, const unsigned long in_numIndices, PrimitiveGroup** primGroups, unsigned long* numGroups)
{
	SetStitchStrips(isStitch);

	if (!isStitch)
		SetMinStripSize(in_MinStripSize);

	SetListsOnly(!isStrip);
	return GenerateStrips(in_indices, in_numIndices, primGroups, numGroups, false);
}



///////////////////////////////////////////////////////////////////////////////

end_gdb_namespace
end_gtl_namespace



