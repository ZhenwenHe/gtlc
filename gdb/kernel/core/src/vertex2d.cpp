#include "vertex2d.h" 

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

 
Vertex2d::Vertex2d() :x(0), y(0){	}

Vertex2d::Vertex2d(double xx, double yy) : x(xx), y(yy){	}

Vertex2d::Vertex2d(Vertex2d & vt)	{
	x = vt.x; y = vt.y;
}

void Vertex2d::operator*=(double rhd){
	x *= rhd;
	y *= rhd;
}

Vertex2d Vertex2d::operator = (const Vertex2d & c){
	x = c.x; y = c.y;
	return *this;
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
