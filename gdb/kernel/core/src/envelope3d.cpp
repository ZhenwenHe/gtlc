#include "config.h"
#include "envelope3d.h" 
#include "mathex.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
//////////////////////////////////////////////////////////////
void Envelope3d::moveTo(double x, double y, double z){
	double w = maxx - minx;
	minx = x - w / 2;
	maxx = x + w / 2;
	w = maxy - miny;
	miny = y - w / 2;
	maxy = y + w / 2;
	w = maxz - minz;
	minz = z - w / 2;
	maxz = z + w / 2;
}
void Envelope3d::moveTo(Vertex3d & v){
	moveTo(v.x, v.y, v.z);
}
Vertex3d Envelope3d::center(){
	Vertex3d v;
	v.x = (minx + maxx) / 2;
	v.y = (miny + maxy) / 2;
	v.z = (minz + maxz) / 2;
	return v;
}
const Vertex3d Envelope3d::getMin() const {
	Vertex3d v;
	v.x = minx;
	v.y = miny;
	v.z = minz;
	return v;
}
const Vertex3d Envelope3d::getMax() const {
	Vertex3d v;
	v.x = maxx;
	v.y = maxy;
	v.z = maxz;
	return v;
}
/** 取得八个顶点之一
* @param [in] iIndex
*  - 0	Xmin,Ymin,Zmin
*  - 1	Xmax,Ymin,Zmin
*  - 2	Xmin,Ymax,Zmin
*  - 3  Xmax,Ymax,Zmin
*  - 4	Xmin,Ymin,Zmax
*  - 5	Xmax,Ymin,Zmax
*  - 6	Xmin,Ymax,Zmax
*  - 7  Xmax,Ymax,Zmax
* @return 八个顶点之一
*/
void Envelope3d::getCorner(unsigned int i, Vertex3d & v)const {

	switch (i)
	{
	case 0:
	{
		v.x = minx;
		v.y = miny;
		v.z = minz;
		break;
	}
	case 1:
	{
		//v.x=minx;
		v.y = miny;
		v.z = minz;

		v.x = maxx;
		break;
	}
	case 2:
	{
		//v.x=minx;
		//v.y=miny;
		v.z = minz;

		v.x = maxx;
		v.y = maxy;
		break;
	}
	case 3:
	{
		v.x = minx;
		//v.y=miny;
		v.z = minz;

		v.y = maxy;
		break;
	}
	case 4:
	{
		v.x = minx;
		v.y = miny;
		//v.z=minz;

		v.z = maxz;
		break;
	}
	case 5:
	{
		//v.x=minx;
		v.y = miny;
		//v.z=minz;

		v.z = maxz;
		v.x = maxx;
		break;
	}
	case 6:
	{
		v.z = maxz;
		v.x = maxx;
		v.y = maxy;
		break;
	}
	case 7:
	{
		v.x = minx;
		//v.y=miny;
		//v.z=minz;

		v.z = maxz;
		v.y = maxy;
		break;
	}
	}
}
Vertex3d Envelope3d::getCorner(unsigned int i){
	Vertex3d v;
	getCorner(i, v);
	return v;
}

//获取对角线长度
double Envelope3d::getDiagonal()
{
	return sqrt((maxx - minx)*(maxx - minx) + (maxy - miny)*(maxy - miny) + (maxz - minz)*(maxz - minz));
}
void Envelope3d::setMin(const Vertex3d   v){
	minx = v.x;
	miny = v.y;
	minz = v.z;
}
void Envelope3d::setMax(const Vertex3d   v){
	maxx = v.x;
	maxy = v.y;
	maxz = v.z;
}
void Envelope3d::get(double v[6]){
	v[0] = minx;		v[1] = maxx;
	v[2] = miny;		v[3] = maxy;
	v[4] = minz;		v[5] = maxz;
}
void Envelope3d::set(double v[6]){
	minx = v[0];		maxx = v[1];
	miny = v[2];		maxy = v[3];
	minz = v[4];		maxz = v[5];
}
void Envelope3d::get(double minv[3], double maxv[3]){
	minv[0] = minx; minv[1] = miny; minv[2] = minz;
	maxv[0] = maxx; maxv[1] = maxy; maxv[2] = maxz;
}
void Envelope3d::set(double minv[3], double maxv[3]){
	minx = minv[0]; miny = minv[1]; minz = minv[2];
	maxx = maxv[0]; maxy = maxv[1]; maxz = maxv[2];
}
void Envelope3d::set(const Vertex3d& minv, const Vertex3d & maxv)	{
	minx = minv.x; miny = minv.y; minz = minv.z;
	maxx = maxv.x; maxy = maxv.y; maxz = maxv.z;
}
void Envelope3d::set(double x1, double x2, double y1, double y2, double z1, double z2){
	minx = x1; miny = y1; minz = z1;
	maxx = x2; maxy = y2; maxz = z2;
}
Envelope3d::Envelope3d(){
	minx = DBL_MAX;
	miny = minx;
	minz = minx;
	maxx = DBL_MIN;
	maxy = maxx;
	maxz = maxx;
	recPlaneIndex = -1;
}
Envelope3d::Envelope3d(double x1, double x2, double y1, double y2, double z1, double z2){
	minx = x1;
	miny = y1;
	minz = z1;
	maxx = x2;
	maxy = y2;
	maxz = z2;
	recPlaneIndex = -1;
}
Envelope3d::Envelope3d(const Envelope2d & e2d, double z1, double z2){
	minx = e2d.minx;
	miny = e2d.miny;
	minz = z1;
	maxx = e2d.maxx;
	maxy = e2d.maxy;
	maxz = z2;
	recPlaneIndex = -1;
}
Envelope3d::Envelope3d(const Envelope3d & e){
	minx = e.minx;
	miny = e.miny;
	minz = e.minz;
	maxx = e.maxx;
	maxy = e.maxy;
	maxz = e.maxz;
	recPlaneIndex = e.recPlaneIndex;
}
Envelope3d Envelope3d::operator = (const Envelope3d & e){
	minx = e.minx;
	miny = e.miny;
	minz = e.minz;
	maxx = e.maxx;
	maxy = e.maxy;
	maxz = e.maxz;
	recPlaneIndex = e.recPlaneIndex;
	return *this;
}
void Envelope3d::extend(const Vertex3d & v){
	if (valid()){
		if (minx > v.x) minx = v.x;
		if (miny > v.y) miny = v.y;
		if (minz > v.z) minz = v.z;

		if (maxx < v.x) maxx = v.x;
		if (maxy < v.y) maxy = v.y;
		if (maxz < v.z) maxz = v.z;
	}
	else{
		minx = v.x - TOLERANCE * 2;
		miny = v.y - TOLERANCE * 2;
		minz = v.z - TOLERANCE * 2;

		maxx = v.x + TOLERANCE * 2;
		maxy = v.y + TOLERANCE * 2;
		maxz = v.z + TOLERANCE * 2;
	}
}
void Envelope3d::extend(const Envelope3d & v){
	if (valid() && v.valid()){
		if (minx > v.minx) minx = v.minx;
		if (miny > v.miny) miny = v.miny;
		if (minz > v.minz) minz = v.minz;

		if (maxx < v.maxx) maxx = v.maxx;
		if (maxy < v.maxy) maxy = v.maxy;
		if (maxz < v.maxz) maxz = v.maxz;
		return;
	}
	if (v.valid()){
		minx = v.minx;
		miny = v.miny;
		minz = v.minz;
		maxx = v.maxx;
		maxy = v.maxy;
		maxz = v.maxz;
		return;
	}
}

void Envelope3d::extend(const double v[6]){
	if (valid()){
		if (minx > v[0]) minx = v[0];
		if (miny > v[2]) miny = v[2];
		if (minz > v[4]) minz = v[4];

		if (maxx < v[1]) maxx = v[1];
		if (maxy < v[3]) maxy = v[3];
		if (maxz < v[5]) maxz = v[5];
		return;
	}
	else{
		minx = v[0];
		miny = v[2];
		minz = v[4];
		maxx = v[1];
		maxy = v[3];
		maxz = v[5];
		return;
	}
}

bool Envelope3d::valid() const{

	if (minx >= maxx || miny >= maxy || minz >= maxz)
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
int Envelope3d::test(Envelope3d & a, Envelope3d & b){
	if (a.minx > b.maxx || a.maxx<b.minx
		|| a.miny>b.maxy || a.maxy<b.miny
		|| a.minz>b.maxz || a.maxz<b.minz)
		return 0;
	else if (
		(a.minx>b.minx && a.minx<b.maxx) || ((a.maxx>b.minx && a.maxx<b.maxx))
		&&
		(a.miny>b.miny && a.miny<b.maxy) || ((a.maxy>b.miny && a.maxy<b.maxy))
		&&
		(a.minz>b.minz && a.minz<b.maxz) || ((a.maxz>b.minz && a.maxz < b.maxz))){
		return 1;
	}
	else if ((a.minx >= b.minx && a.minx <= b.maxx) && ((a.maxx >= b.minx && a.maxx <= b.maxx))
		&&
		(a.miny >= b.miny && a.miny <= b.maxy) && ((a.maxy >= b.miny && a.maxy <= b.maxy))
		&&
		(a.minz >= b.minz && a.minz <= b.maxz) && ((a.maxz >= b.minz && a.maxz <= b.maxz))){
		return 2;
	}
	else
		return 3;
}


Envelope3d Envelope3d::merge(const Vertex3d& pnt)
{
	//xx_0128
	//	if(pnt.x < minx) minx = pnt.x;
	if ((pnt.x - minx) < -MathEx::TOL) minx = pnt.x;
	//	if(pnt.x > maxx) maxx = pnt.x;
	if ((pnt.x - maxx) > MathEx::TOL) maxx = pnt.x;
	//	if(pnt.y < miny) miny = pnt.y;
	if ((pnt.y - miny) < -MathEx::TOL) miny = pnt.y;
	//	if(pnt.y > maxy) maxy = pnt.y;
	if ((pnt.y - maxy) > MathEx::TOL) maxy = pnt.y;
	//	if(pnt.z < minz) minz = pnt.z;
	if ((pnt.z - minz) < -MathEx::TOL) minz = pnt.z;
	//	if(pnt.z > maxz) maxz = pnt.z;
	if ((pnt.z - maxz) > MathEx::TOL) maxz = pnt.z;
	return *this;
}

Envelope3d Envelope3d::merge(const Vertex3d* pnts, int nPntNum)
{
	for (int i = 0; i<nPntNum; i++)
		merge(pnts[i]);
	return *this;
}

Envelope3d Envelope3d::merge(const std::vector<Vertex3d>& pnts)
{
	int nCount = pnts.size();
	for (int i = 0; i<nCount; i++)
		merge(pnts[i]);
	return *this;
}

Envelope3d Envelope3d::merge(const Envelope3d& otherEnvlp)
{
	/*if(maxx < otherEnvlp.maxx)	maxx = otherEnvlp.maxx;
	if(maxy < otherEnvlp.maxy)	maxy = otherEnvlp.maxy;
	if(maxz < otherEnvlp.maxz)	maxz = otherEnvlp.maxz;*/
	if ((maxx - otherEnvlp.maxx) < -MathEx::TOL)	maxx = otherEnvlp.maxx;
	if ((maxy - otherEnvlp.maxy) < -MathEx::TOL)	maxy = otherEnvlp.maxy;
	if ((maxz - otherEnvlp.maxz) < -MathEx::TOL)	maxz = otherEnvlp.maxz;

	// xx_0128
	/*if(minx > otherEnvlp.minx)	minx = otherEnvlp.minx;
	if(miny > otherEnvlp.miny)	miny = otherEnvlp.miny;
	if(minz > otherEnvlp.minz)	minz = otherEnvlp.minz;*/
	if ((minx - otherEnvlp.minx) > MathEx::TOL)	minx = otherEnvlp.minx;
	if ((miny - otherEnvlp.miny) > MathEx::TOL)	miny = otherEnvlp.miny;
	if ((minz - otherEnvlp.minz) > MathEx::TOL)	minz = otherEnvlp.minz;
	return *this;
}

int  Envelope3d::intersect(const Envelope3d& otherbox, Envelope3d& retBox) const
{
	//	if(	maxx < otherbox.minx ||  //xx_0128
	if ((maxx - otherbox.minx) < -MathEx::TOL ||
		//		minx > otherbox.maxx || 
		(minx - otherbox.maxx) > MathEx::TOL ||
		//		maxy < otherbox.miny || 
		(maxy - otherbox.miny) < -MathEx::TOL ||
		//		miny > otherbox.maxy ||
		(miny - otherbox.maxy) > MathEx::TOL ||
		//		maxz < otherbox.minz || 
		(maxz - otherbox.minz) < -MathEx::TOL ||
		//		minz > otherbox.maxz
		(minz - otherbox.maxz) > MathEx::TOL
		)//不相交
		return 0;

	//	if(maxx > otherbox.maxx && maxy > otherbox.maxy && maxz > otherbox.maxz  //xx_0128
	if (((maxx - otherbox.maxx) > MathEx::TOL) && ((maxy - otherbox.maxy) > MathEx::TOL) && ((maxz - otherbox.maxz) > MathEx::TOL)
		//&& minx < otherbox.minx && miny < otherbox.miny && minz < otherbox.minz)//包含
		&& (minx - otherbox.minx) < -MathEx::TOL && (miny - otherbox.miny) < -MathEx::TOL && (minz - otherbox.minz) < -MathEx::TOL)//包含
		return 2;

	if (maxx < otherbox.maxx && maxy < otherbox.maxy && maxz < otherbox.maxz
		//	&& minx > otherbox.minx && miny > otherbox.miny && minz > otherbox.minz)//被包含
		&& ((minx - otherbox.minx) > MathEx::TOL) && ((miny - otherbox.miny) > MathEx::TOL) && ((minz - otherbox.minz) > MathEx::TOL))//被包含
		return 3;

	retBox.maxx = min(maxx, otherbox.maxx);
	retBox.maxy = min(maxy, otherbox.maxy);
	retBox.maxz = min(maxz, otherbox.maxz);
	retBox.minx = max(minx, otherbox.minx);
	retBox.miny = max(miny, otherbox.miny);
	retBox.minz = max(minz, otherbox.minz);

	return 1;
}

int Envelope3d::test(const Vertex3d& vPt) const
{
	//Vertex3d vmin = m_VMin;
	/*	int bRes = (vPt.x >= minx) && (vPt.x <= maxx) &&
	(vPt.y >= miny) && (vPt.y <= maxy) &&
	(vPt.z >= minz) && (vPt.z <= maxz);*/

	int bRes = (vPt.x - minx > -MathEx::TOL) &&
		(vPt.x - maxx <  MathEx::TOL) &&
		(vPt.y - miny > -MathEx::TOL) &&
		(vPt.y - maxy < MathEx::TOL) &&
		(vPt.z - minz > -MathEx::TOL) &&
		(vPt.z - maxz < MathEx::TOL);

	return bRes;
}

int Envelope3d::test(const Envelope3d& inBox, char proAxis, Envelope3d * intersectBox) const // 
{
	Vertex3d Vmin = inBox.getMin();
	Vertex3d Vmax = inBox.getMax();

	if (proAxis == 'z')
	{
		if ((Vmin.x - maxx) > MathEx::TOL || (minx - Vmax.x) > MathEx::TOL) return 0;
		if ((Vmin.y - maxy) > MathEx::TOL || (miny - Vmax.y) > MathEx::TOL) return 0;

		if (intersectBox)
		{
			Vertex3d nMin, nMax;
			nMin.x = max(Vmin.x, minx);
			nMin.y = max(Vmin.y, miny);

			nMax.x = min(Vmax.x, maxx);
			nMax.y = min(Vmax.y, maxy);
		}
		return 1;
	}
	else if (proAxis == 'y')
	{
		if ((Vmin.x - maxx) > MathEx::TOL || (minx - Vmax.x) > MathEx::TOL) return 0;
		if ((Vmin.z - maxz) > MathEx::TOL || (minz - Vmax.z) > MathEx::TOL) return 0;

		if (intersectBox)
		{
			Vertex3d nMin, nMax;
			nMin.x = max(Vmin.x, minx);
			nMin.z = max(Vmin.z, minz);

			nMax.x = min(Vmax.x, maxx);
			nMax.z = min(Vmax.z, maxz);
		}
		return 1;
	}
	else if (proAxis == 'x')
	{
		if ((Vmin.y - maxy) > MathEx::TOL || (miny - Vmax.y) > MathEx::TOL) return 0;
		if ((Vmin.z - maxz) > MathEx::TOL || (minz - Vmax.z) > MathEx::TOL) return 0;

		if (intersectBox)
		{
			Vertex3d nMin, nMax;
			nMin.y = max(Vmin.y, miny);
			nMin.z = max(Vmin.z, minz);

			nMax.y = min(Vmax.y, maxy);
			nMax.z = min(Vmax.z, maxz);
		}
		return 1;
	}
	else
		return 0;

}

int Envelope3d::test(const Envelope3d& inBox) const 
{
	Vertex3d Vmin = inBox.getMin();
	Vertex3d Vmax = inBox.getMax();

	// 分别判断两外包盒在X\Y\Z方向的投影
	if ((Vmin.x - maxx) > MathEx::TOL || (minx - Vmax.x) > MathEx::TOL) return 0;
	if ((Vmin.y - maxy) > MathEx::TOL || (miny - Vmax.y) > MathEx::TOL) return 0;
	if ((Vmin.z - maxz) > MathEx::TOL || (minz - Vmax.z) > MathEx::TOL) return 0;
	return 1;
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
