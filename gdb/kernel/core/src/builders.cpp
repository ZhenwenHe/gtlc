#include "connector.h"
#include "builder.h"
#include "factory.h"


begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

Builders::Builders(){}
Builders::~Builders(){}


void Builders::copy(const Builders& b){
	_modelBuilder = b._modelBuilder;
	_meshBuilder = b._meshBuilder;
	_terrainBuilder = b._terrainBuilder;
}

Builders::Builders(const Builders& b){
	copy(b);
}

void Builders::close(){
	if(_modelBuilder.get())
		_modelBuilder->close();
	if(_meshBuilder.get())
		_meshBuilder->close();
	if(_terrainBuilder.get())
		_terrainBuilder->close(); 
}

bool Builders::open(const Connector & conn){
	if(_modelBuilder.get())
		_modelBuilder->open(conn);
	if(_meshBuilder.get())
		_meshBuilder->open(conn);
	if(_terrainBuilder.get())
		_terrainBuilder->open(conn);

	return true;
}

Builders Builders::operator= (const Builders & b){
	copy(b);
	return *this;
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace