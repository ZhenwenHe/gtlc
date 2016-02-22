#include "vertex2d.h" 



begin_gtl_namespace
begin_gdb_namespace

 
Vertex2d::Vertex2d() :x(0), y(0){	}

Vertex2d::Vertex2d(double xx, double yy) : x(xx), y(yy){	}

Vertex2d::Vertex2d(const Vertex2d & vt)	{
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
end_gtl_namespace


