#include "geometry.h"
#include "geometry2d.h"
#include "geometry3d.h"
#include "factory.h"
#include "tritool.h"



begin_gtl_namespace
begin_gdb_namespace

extern void Triangulate2(std::vector<Vertex3d>& dataXYZ, std::vector<int>& waPs, std::vector<int>& waTris);
extern void Triangulate(Vector3d * pdData, unsigned int * ps, std::vector<int> * pPsArray);

bool TriTool::trianglate(const VertexCollection2d  * in_points, std::vector<int> & tri_index)
{	
	if(in_points)
		return false;
	VertexCollection2d  * pin = (VertexCollection2d  *)in_points;
	unsigned int s = pin->getCount();
	Vector3d * pd = new Vector3d[s];
	Vertex2d * p =0;
	unsigned int * ps = new unsigned int[s+1];
	ps[0]=s;
	for(unsigned int i=0;i<s;i++)
	{
		p= pin->getPointPtr(i);
		pd[i][0]=p->x;
		pd[i][1]=p->y;
		pd[i][2]=0;
		ps[i+1]=i;
	}
	Triangulate(pd,ps,&tri_index);
	delete [] ps;
	delete [] pd;
	return true;
}
bool TriTool::trianglate(const std::vector<Vertex3d>& in_points, std::vector<int> & tri_index)
{	
	unsigned int s = in_points.size();

	if(s<=0)
		return false;

	Vector3d * pd = new Vector3d[s];
	unsigned int * ps = new unsigned int[s+1];
	ps[0]=s;
	unsigned int i=0;
	for(std::vector<Vertex3d>::const_iterator it = in_points.begin();it!=in_points.end();it++)
	{
		pd[i][0]=it->x;
		pd[i][1]=it->y;
		pd[i][2]=it->z;
		ps[i+1]=i;
		i++;
	}
	Triangulate(pd,ps,&tri_index);
	delete [] ps;
	delete [] pd;
	return true;
}
bool TriTool::trianglate(const Vector3d* in_points, const int point_number,std::vector<int> & tri_index)
{
	 
	if(point_number<=0 || in_points==0)
		return false;

	unsigned int * ps = new unsigned int[point_number+1];
	ps[0]=point_number;	
	for(int i=0;i<point_number;i++)
	{
		ps[i+1]=i;
	}
	Triangulate((Vector3d*)in_points,ps,&tri_index);
	delete [] ps;
	return true;
}
bool TriTool::trianglate(std::vector<Vertex3d>& dataXYZ, std::vector<int>& waPs, std::vector<int>& waTris)
{
	Triangulate2( dataXYZ,waPs,waTris);
	return true;
}
end_gdb_namespace
end_gtl_namespace


