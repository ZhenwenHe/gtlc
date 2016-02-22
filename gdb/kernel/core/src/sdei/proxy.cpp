#include "proxy.h"



begin_gtl_namespace
begin_gdb_namespace

Proxy::Proxy(){

}

Proxy::~Proxy(){

}

double  Proxy::getDiagonal(double d[])
{
	return sqrt((d[1] - d[0])*(d[1] - d[0])+ (d[3] - d[2])*(d[3] - d[2]) + (d[5] - d[4])*(d[5] - d[4]))/2;
}
end_gdb_namespace
end_gtl_namespace


