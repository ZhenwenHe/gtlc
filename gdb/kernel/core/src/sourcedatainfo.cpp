#include "config.h"
#include "terrain.h" 
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace


SourceDataInfo::SourceDataInfo()
{
	dbID = 0;
	dataType = 0;
	/*strcpy_s(srcName,MAX_NAME,"");
	strcpy_s(srcPath,MAX_PATH,"");*/
	srcCellsize = 5.0;

	srcCreatetime = 0;

}

SourceDataInfo SourceDataInfo::operator=(const SourceDataInfo &other)
{
	dbID = other.dbID;
	dataType = other.dataType;
	srcName = other.srcName;
	srcPath = other.srcPath;
	srcCellsize = other.srcCellsize;
	srcScope.minx = other.srcScope.minx;
	srcScope.maxx = other.srcScope.maxx;
	srcScope.miny = other.srcScope.miny;
	srcScope.maxy = other.srcScope.maxy;

	srcCreatetime = other.srcCreatetime;

	return *this;

}


bool SourceDataInfo::writeBuffer(Buffer& buf){
	buf.write((void*)(&dbID), sizeof(Identifier::raw_type));
	buf.write((void*)(&dataType), sizeof(int));

	size_t s = srcName.size();
	buf.write((void*)(&s), sizeof(size_t));
	buf.write((void*)(&srcName), s);

	s = srcPath.size();
	buf.write((void*)(&s), sizeof(size_t));
	buf.write((void*)(&srcPath), s);

	buf.write((void*)(&srcCellsize), sizeof(double));

	double v[4];
	srcScope.get(v);
	buf.write((void*)v, sizeof(double) * 4);

	buf.write((void*)(&srcCreatetime), sizeof(Identifier::raw_type));

	return true;
}

bool SourceDataInfo::readBuffer(Buffer& buf){
	buf.read((void*)(&dbID), sizeof(Identifier::raw_type));
	buf.read((void*)(&dataType), sizeof(int));

	size_t s = 0;
	buf.read((void*)(&s), sizeof(size_t));
	buf.read((void*)(&srcName), s);
	s = 0;
	buf.read((void*)(&s), sizeof(size_t));
	buf.read((void*)(&srcPath), s);

	buf.read((void*)(&srcCellsize), sizeof(double));

	double v[4];
	buf.read((void*)v, sizeof(double) * 4);
	srcScope.set(v);

	buf.read((void*)(&srcCreatetime), sizeof(Identifier::raw_type));
	return true;
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace