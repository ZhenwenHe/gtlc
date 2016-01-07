#include "config.h"
#include "envelope2d.h" 


begin_gtl_namespace
begin_gdb_namespace
//////////////////////////////////////////////////////////////
void Envelope2d::moveTo(double x, double y){
	double w = maxx - minx;
	minx = x - w / 2;
	maxx = x + w / 2;
	w = maxy - miny;
	miny = y - w / 2;
	maxy = y + w / 2;
}
void Envelope2d::moveTo(Vertex2d & v){
	moveTo(v.x, v.y);
}
Vertex2d Envelope2d::center(){
	Vertex2d v;
	v.x = (minx + maxx) / 2;
	v.y = (miny + maxy) / 2;
	return v;
}
Vertex2d Envelope2d::getMin() {
	Vertex2d v;
	v.x = minx;
	v.y = miny;
	return v;
}
Vertex2d Envelope2d::getMax() {
	Vertex2d v;
	v.x = maxx;
	v.y = maxy;
	return v;
}
void Envelope2d::get(double v[4]){
	v[0] = minx;		v[1] = maxx;
	v[2] = miny;		v[3] = maxy;
}
void Envelope2d::set(double v[4]){
	minx = v[0];		maxx = v[1];
	miny = v[2];		maxy = v[3];
}
void Envelope2d::get(double minv[2], double maxv[2]){
	minv[0] = minx; minv[1] = miny;
	maxv[0] = maxx; maxv[1] = maxy;
}
void Envelope2d::set(double minv[2], double maxv[2]){
	minx = minv[0]; miny = minv[1];
	maxx = maxv[0]; maxy = maxv[1];
}

/////////////////////////////////////////////////////////////////////////////////
Envelope2d::Envelope2d(){
	miny = minx = DBL_MAX;
	maxy = maxx = DBL_MIN;
}
Envelope2d::Envelope2d(double x1, double x2, double y1, double y2){
	minx = x1;
	miny = y1;
	maxy = y2;
	maxx = x2;
}
Envelope2d::Envelope2d(const Envelope2d & e){
	minx = e.minx;
	miny = e.miny;
	maxy = e.maxy;
	maxx = e.maxx;
}
Envelope2d Envelope2d::operator = (const Envelope2d & e){
	minx = e.minx;
	miny = e.miny;
	maxy = e.maxy;
	maxx = e.maxx;
	return *this;
}
void Envelope2d::extend(const Vertex2d & v){
	if (valid()){
		if (minx > v.x) minx = v.x;
		if (miny > v.y) miny = v.y;

		if (maxx < v.x) maxx = v.x;
		if (maxy < v.y) maxy = v.y;
	}
	else{
		minx = v.x - TOLERANCE * 2;
		miny = v.y - TOLERANCE * 2;

		maxx = v.x + TOLERANCE * 2;
		maxy = v.y + TOLERANCE * 2;
	}
}
void Envelope2d::extend(const Envelope2d & v){
	if (valid() && v.valid()){
		if (minx > v.minx) minx = v.minx;
		if (miny > v.miny) miny = v.miny;

		if (maxx < v.maxx) maxx = v.maxx;
		if (maxy < v.maxy) maxy = v.maxy;
		return;
	}
	if (v.valid()){
		minx = v.minx;
		miny = v.miny;

		maxx = v.maxx;
		maxy = v.maxy;

		return;
	}
}

void Envelope2d::extend(const double v[4]){
	if (valid()){
		if (minx > v[0]) minx = v[0];
		if (miny > v[2]) miny = v[2];

		if (maxx < v[1]) maxx = v[1];
		if (maxy < v[3]) maxy = v[3];

		return;
	}
	else{
		minx = v[0];
		miny = v[2];

		maxx = v[1];
		maxy = v[3];

		return;
	}
}

bool Envelope2d::valid() const{

	if (minx >= maxx || miny >= maxy)
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
//int Envelope2d::test(Envelope2d & a, Envelope2d & b){
//	if(a.minx>b.maxx || a.maxx<b.minx
//		|| a.miny>b.maxy || a.maxy<b.miny)
//		return 0;
//	if(
//		(a.minx>b.minx && a.minx<b.maxx)||((a.maxx>b.minx && a.maxx<b.maxx))
//		&&
//		(a.miny>b.miny && a.miny<b.maxy)||((a.maxy>b.miny && a.maxy<b.maxy))){
//			return 1;
//	}
//	if((a.minx>=b.minx && a.minx<=b.maxx)&&((a.maxx>=b.minx && a.maxx<=b.maxx))
//		&&
//		(a.miny>=b.miny && a.miny<=b.maxy)&&((a.maxy>=b.miny && a.maxy<=b.maxy))){
//			return 2;
//	}
//	else
//		return 3;
//}
int Envelope2d::test(Envelope2d & a, Envelope2d & b){
	if (a.minx > b.maxx || a.maxx<b.minx || a.miny>b.maxy || a.maxy < b.miny)
		return 0;
	else if ((a.minx >= b.minx && a.minx <= b.maxx) && ((a.maxx >= b.minx && a.maxx <= b.maxx))
		&& (a.miny >= b.miny && a.miny <= b.maxy) && ((a.maxy >= b.miny && a.maxy <= b.maxy))){
		return 2;
	}
	else if ((b.minx >= a.minx && b.minx <= a.maxx) && ((b.maxx >= a.minx && b.maxx <= a.maxx))
		&& (b.miny >= a.miny && b.miny <= a.maxy) && ((b.maxy >= a.miny && b.maxy <= a.maxy))){
		return 3;
	}
	else
		return 1;
}
end_gdb_namespace
end_gtl_namespace


