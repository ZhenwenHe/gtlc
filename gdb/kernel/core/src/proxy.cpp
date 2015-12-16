#include "Proxy.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
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
end_cug_namespace
end_edu_namespace
end_cn_namespace
