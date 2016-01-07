#include "config.h"
#include "terrain.h" 
#include "terrainbuilder.h"


begin_gtl_namespace
begin_gdb_namespace

DEMLayerInfo::DEMLayerInfo()
{
	layerID = 0;
	demdbID = 0;
	//strcpy_s(layerName,MAX_NAME,"");
	rowsPreblock = 0;
	colsPreblock = 0;
	xBlocksNumber = 0;
	yBlocksNumber = 0;
	blockSize = 0.0;
	scale = 2;

}

DEMLayerInfo::DEMLayerInfo( Identifier::raw_type lyrID,Identifier::raw_type dbID, char* name, Envelope2d lyrScope, Envelope2d dataScope, int xBlocks, int yBlocks, int rows, int cols, double res,
	int nScale,int boriDataLayer, int btopLayer )
{
	layerID = lyrID;
	demdbID = dbID;
	//strcpy_s(layerName,MAX_NAME,name);
	layerName = (const char*) name;

	layerScope = lyrScope;
	dataScope = dataScope;
	xBlocksNumber = xBlocks;
	yBlocksNumber = yBlocks;
	rowsPreblock = rows;
	colsPreblock = cols;
	blockSize = res;
	scale = nScale;

}

DEMLayerInfo DEMLayerInfo::operator=(const DEMLayerInfo &other )
{
	layerID = other.layerID;
	demdbID = other.demdbID;
	layerName=other.layerName;
	layerScope = other.layerScope;
	dataScope = other.dataScope;
	xBlocksNumber = other.xBlocksNumber;
	yBlocksNumber = other.yBlocksNumber;
	rowsPreblock = other.rowsPreblock;
	colsPreblock = other.colsPreblock;
	blockSize = other.blockSize;
	scale = other.scale;
	return *this;
}

bool DEMLayerInfo::operator == ( DEMLayerInfo &other )
{
	if( xBlocksNumber == other.xBlocksNumber && yBlocksNumber == other.yBlocksNumber
		&& rowsPreblock == other.rowsPreblock && colsPreblock == other.colsPreblock 
		&& fabs( blockSize - other.blockSize ) < 0.001 && scale == other.scale )
		return true;
	else
		return false;
}

bool DEMLayerInfo::operator != ( DEMLayerInfo &other )
{
	if( xBlocksNumber != other.xBlocksNumber || yBlocksNumber != other.yBlocksNumber
		|| rowsPreblock != other.rowsPreblock || colsPreblock != other.colsPreblock 
		|| fabs( blockSize - other.blockSize ) > 0.001 && scale == other.scale )
		return true;
	else
		return false;
}

void DEMLayerInfo::clear()
{
	layerID = 0;
	demdbID = 0;
	layerName.clear();
	rowsPreblock = 0;
	colsPreblock = 0;
	xBlocksNumber = 0;
	yBlocksNumber = 0;
	blockSize = 0.0;
	scale = 2;
	layerScope = Envelope2d();
	dataScope = Envelope2d();

}

void DEMLayerInfo::calculateBlockNumbers(long & rowBlockNumber, long &colBlockNumber){
	//计算当前层可以划分成多少个数据块
	double dxn = (layerScope.maxx-layerScope.minx)/blockSize ;
	double dyn = (layerScope.maxy-layerScope.miny)/blockSize ;
	colBlockNumber = long(dxn);
	colBlockNumber=colBlockNumber+(dxn-colBlockNumber>0?1:0);
	rowBlockNumber = long(dyn);
	rowBlockNumber = rowBlockNumber+(dyn-rowBlockNumber>0?1:0);
}
	
Envelope2d DEMLayerInfo::calcualteBlockEnvelope(unsigned int block_row_in_layer,unsigned int block_col_in_layer){
	return Envelope2d (layerScope.minx+block_col_in_layer*blockSize,
		layerScope.minx+(block_col_in_layer+1)*blockSize,
		layerScope.miny+block_row_in_layer*blockSize,
		layerScope.miny+(block_row_in_layer+1)*blockSize);
}
/** 计算图层中单元网格的间距
    * @param  [in] 无
    * @return  图层中单元网格的间距
*/
double DEMLayerInfo::calcualteCellSize() {return blockSize/(colsPreblock-1);}
/** 根据图层范围和块的大小，计算块的所有节点坐标
    * @param  [in] block_row_in_layer 数据块在图层行索引 
	* @param  [in] block_col_in_layer 数据块在图层列索引 
    * @return  
*/
void DEMLayerInfo::calculateBlockVertices(unsigned int block_row_in_layer,unsigned int block_col_in_layer,std::vector<Vertex3d> & vv){
	Envelope2d e2d = calcualteBlockEnvelope(block_row_in_layer,block_col_in_layer);
	if(vv.size()!=colsPreblock*rowsPreblock) vv.resize(colsPreblock*rowsPreblock);
	std::vector<Vertex3d>::iterator it = vv.begin();
	double cellsize = calcualteCellSize();
	//计算块中所有的节点坐标
	for(int y=0;y<rowsPreblock;y++){
		for(int x=0;x<colsPreblock;x++){
			it->x = e2d.minx+x*cellsize;
			it->y = e2d.miny+y*cellsize;
			it->z = DEMGrid::getDefaultInvalidValue();
			it++;
		}
	}
}
/** 创建一个新的数据块
* @param  [in] block_row_in_layer 数据块在图层行索引 
* @param  [in] block_col_in_layer 数据块在图层列索引 
* @param  [in] vv 数据块对应的节点坐标数组 
* @return  
*/
DEMBlockSharedPtr DEMLayerInfo::createDEMBlock(unsigned int block_row_in_layer,
	unsigned int block_col_in_layer,
	std::vector<Vertex3d> & vv){
	unsigned int s = vv.size();
	float * data = new float[s];
	float * p=data;
	for(std::vector<Vertex3d>::iterator it = vv.begin();it!=vv.end();it++){
		*p = it->z;
		p++;
	}
	DEMBlockSharedPtr dsp = createDEMBlock(block_row_in_layer,block_col_in_layer,data,s);
	delete [] data;
	return dsp;
}

/** 创建一个新的数据块
    * @param  [in] block_row_in_layer 数据块在图层行索引 
	* @param  [in] block_col_in_layer 数据块在图层列索引 
	* @param  [in] rawData 数据块对应的节点坐标数组 
    * @return  
*/
DEMBlockSharedPtr DEMLayerInfo::createDEMBlock(unsigned int block_row_in_layer,unsigned int block_col_in_layer,const float * rawData,const unsigned int pointNumber)
{
	if(!rawData || pointNumber==0) return DEMBlockSharedPtr();

	DEMBlockSharedPtr demblock(new DEMBlock());
	demblock->blockID = TerrainBuilder::calculate(block_row_in_layer,block_col_in_layer);
	demblock->layerID = this->layerID;
	demblock->column = block_col_in_layer;
	demblock->row = block_row_in_layer;
	demblock->scope = calcualteBlockEnvelope(block_row_in_layer,block_col_in_layer);
	demblock->pointNumber = colsPreblock*rowsPreblock;
	demblock->valid=1;

	demblock->rawData = new float[demblock->pointNumber];

	unsigned int s = min(demblock->pointNumber,pointNumber);
	//memcpy(demblock->rawData,rawData,sizeof(float)*s);

	demblock->minZ=rawData[0];
	demblock->maxZ=rawData[0];
	float * p = demblock->rawData;
	for(unsigned int i=0;i<s;i++){
		*p = rawData[i];
		if(demblock->maxZ<*p) demblock->maxZ = *p;
		if(demblock->minZ>*p) demblock->minZ = *p;
		p++;
	}


	if(pointNumber>=demblock->pointNumber)
		demblock->full=0;
	else {//部分填充
		demblock->full =2;
		for(unsigned int i=pointNumber;i<demblock->pointNumber;i++){
			demblock->rawData[i]=DEMGrid::getDefaultInvalidValue();
		}
	}

	return demblock;
}



bool DEMLayerInfo::writeBuffer(Buffer& buf){
	buf.write((void*)(&layerID), sizeof(Identifier::raw_type));
	buf.write((void*)(&demdbID), sizeof(Identifier::raw_type));
	/*size_t s =layerName.size();
	buf.write((void*)(&s),sizeof(size_t));
	if(s>0)
	buf.write((void*)(&layerName),s);*/
	buf.write(layerName);

	buf.write((void*)(&blockSize), sizeof(double));

	double v[4];
	layerScope.get(v);
	buf.write((void*)v, sizeof(double) * 4);

	double t[4];
	dataScope.get(t);
	buf.write((void*)t, sizeof(double) * 4);

	double r[4];
	domSubmitScope.get(r);
	buf.write((void*)r, sizeof(double) * 4);

	buf.write((void*)(&rowsPreblock), sizeof(int));
	buf.write((void*)(&colsPreblock), sizeof(int));
	buf.write((void*)(&xBlocksNumber), sizeof(int));
	buf.write((void*)(&yBlocksNumber), sizeof(int));
	buf.write((void*)(&scale), sizeof(int));

	return true;
}

bool DEMLayerInfo::readBuffer(Buffer& buf){
	buf.read((void*)(&layerID), sizeof(Identifier::raw_type));
	buf.read((void*)(&demdbID), sizeof(Identifier::raw_type));
	/*size_t s =0;
	buf.read((void*)(&s),sizeof(size_t));
	if(s>0)
	buf.read((void*)(&layerName),s);*/
	layerName = buf.readString();

	buf.read((void*)(&blockSize), sizeof(double));

	double v[4];
	buf.read((void*)v, sizeof(double) * 4);
	layerScope.set(v);

	double t[4];
	buf.read((void*)t, sizeof(double) * 4);
	dataScope.set(t);

	double r[4];
	buf.read((void*)r, sizeof(double) * 4);
	domSubmitScope.set(r);

	buf.read((void*)(&rowsPreblock), sizeof(int));
	buf.read((void*)(&colsPreblock), sizeof(int));
	buf.read((void*)(&xBlocksNumber), sizeof(int));
	buf.read((void*)(&yBlocksNumber), sizeof(int));
	buf.read((void*)(&scale), sizeof(int));

	return true;
}


end_gdb_namespace
end_gtl_namespace

