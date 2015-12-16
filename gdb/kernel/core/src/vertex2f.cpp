#include "vertex2f.h" 

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace


Vertex2f::Vertex2f() :x(0), y(0){	}

Vertex2f::Vertex2f(float xx, float yy) : x(xx), y(yy){	}

Vertex2f::Vertex2f(Vertex2f & vt)	{
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
end_cug_namespace
end_edu_namespace
end_cn_namespace
