#include "vertex2f.h" 



begin_gtl_namespace
begin_gdb_namespace


Vertex2f::Vertex2f() :x(0), y(0){	}

Vertex2f::Vertex2f(float xx, float yy) : x(xx), y(yy){	}

Vertex2f::Vertex2f(const Vertex2f & vt)	{
	x = vt.x; y = vt.y;
}

void Vertex2f::operator*=(float rhd){
	x *= rhd;
	y *= rhd;
}

Vertex2f Vertex2f::operator = (const Vertex2f & c){
	x = c.x; y = c.y;
	return *this;
}

end_gdb_namespace
end_gtl_namespace


