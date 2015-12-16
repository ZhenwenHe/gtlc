#include "internal_EclipseMeshImpl.h" 
#include "internal_EclipseMeshReader.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

int EclipseMeshImpl::_reindex[2][2][2] =
{ {
	{ 0, 4 },
	{ 3, 7 }
	},
	{
		{ 1, 5 },
		{ 2, 6 }
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
EclipseMeshImpl::EclipseMeshImpl(){
	_zValues = 0;
}

EclipseMeshImpl::EclipseMeshImpl(unsigned long long cx, unsigned long long cy, unsigned long long cz, unsigned long long propNumberPreCell)
{
	Mesh::reset(cx, cy, cz, propNumberPreCell);
	_zValues = 0;

	_topCtrlVertices.resize((getXCellNumber() + 1)*(getYCellNumber() + 1));

	_bottomCtrlVertices.resize((getXCellNumber() + 1)*(getYCellNumber() + 1));

	_zValues = reallocZValues();
}

void EclipseMeshImpl::clear() {
	Mesh::clear();
	_topCtrlVertices.clear();
	_bottomCtrlVertices.clear();
	if (_zValues)
		delete[] _zValues;
	_zValues = 0;
}
Envelope3d EclipseMeshImpl::getEnvelope(){
	Envelope3d e3d;
	for (std::vector<Vertex3d>::iterator it = _topCtrlVertices.begin(); it != _topCtrlVertices.end(); it++)
	{
		e3d.extend(*it);
	}
	for (std::vector<Vertex3d>::iterator it = _bottomCtrlVertices.begin(); it != _bottomCtrlVertices.end(); it++)
	{
		e3d.extend(*it);
	}
	return e3d;
}
void EclipseMeshImpl::setOrigin(const Vertex3d & v){
	_origin = v;

	for (std::vector<Vertex3d>::iterator it = _topCtrlVertices.begin(); it != _topCtrlVertices.end(); it++)
	{
		*it = *it - _origin;
	}
	for (std::vector<Vertex3d>::iterator it = _bottomCtrlVertices.begin(); it != _bottomCtrlVertices.end(); it++)
	{
		*it = *it - _origin;
	}

	unsigned long long s = getZValuesNumber();
	for (unsigned long long i = 0; i<s; i++)
		_zValues[i] -= _origin.z;
}
void EclipseMeshImpl::copy(const EclipseMeshImpl & p){
	Mesh::copy((const Mesh&)p);

	_topCtrlVertices.resize(p._topCtrlVertices.size());
	std::copy(p._topCtrlVertices.begin(), p._topCtrlVertices.end(), _topCtrlVertices.begin());

	_bottomCtrlVertices.resize(p._bottomCtrlVertices.size());
	std::copy(p._bottomCtrlVertices.begin(), p._bottomCtrlVertices.end(), _bottomCtrlVertices.begin());

	_zValues = reallocZValues();
	memcpy(_zValues, p._zValues, getZValuesNumber()*sizeof(double));
}

EclipseMeshImpl::EclipseMeshImpl(const EclipseMeshImpl & p){
	copy(p);
}

void EclipseMeshImpl::reset(unsigned long long cx, unsigned long long cy, unsigned long long cz, unsigned long long propNumberPreCell){
	clear();

	Mesh::reset(cx, cy, cz, propNumberPreCell);

	_topCtrlVertices.resize((getXCellNumber() + 1)*(getYCellNumber() + 1));

	_bottomCtrlVertices.resize((getXCellNumber() + 1)*(getYCellNumber() + 1));

	_zValues = reallocZValues();
}

Cell& EclipseMeshImpl::getCurrentCell(unsigned long long ix, unsigned long long iy, unsigned long long iz){

	if (!_validBitset.get(iz*_yCellNumber*_xCellNumber + iy*_xCellNumber + ix)){
		_currentCell.clear();
		return _currentCell;
	}

	double vv[24];
	generate_cell_vertices(ix, iy, iz, vv);
	_currentCell.setX(ix);
	_currentCell.setY(iy);
	_currentCell.setZ(iz);
	_currentCell.getVertices().clear();
	for (int i = 0; i<8; i++)
		_currentCell.getVertices().push_back(Vertex3d(vv[i * 3], vv[i * 3 + 1], vv[i * 3 + 2]));

	/*if(same<Vertex3d>(_currentCell.getVertices()))
	_currentCell.setValid(false);
	else
	_currentCell.setValid(true);*/
	_currentCell.setValid(true);

	int s = getPropNumberPreCell();
	double * val = getCellValues(ix, iy, iz);
	_currentCell.getPropValues().clear();
	for (int i = 0; i<s; i++)
		_currentCell.getPropValues().push_back(val[i]);

	return _currentCell;
}

CellSharedPtr EclipseMeshImpl::getCell(unsigned long long ix, unsigned long long iy, unsigned long long iz){
	if (getCurrentCell(ix, iy, iz).isValid())
		return CellSharedPtr(new Cell(_currentCell));
	else
		return CellSharedPtr();
}

MeshSharedPtr EclipseMeshImpl::submesh(
	unsigned long long bx, unsigned long long ex,
	unsigned long long by, unsigned long long ey,
	unsigned long long bz, unsigned long long ez,
	unsigned long long cx, unsigned long long cy, unsigned long long cz)
{
	EclipseMeshImpl  * pr = new EclipseMeshImpl(cx, cy, cz, this->getPropNumberPreCell());
	//网格开始结束范围
	pr->_beginX = bx;
	pr->_endX = ex;
	pr->_beginY = by;
	pr->_endY = ey;
	pr->_beginZ = bz;
	pr->_endZ = ez;
	//网格属性名称
	std::copy(_propNames.begin(), _propNames.end(), pr->_propNames.begin());
	//属性值一般为外部设置

	int xScale = cx / (ex - bx);
	int yScale = cy / (ey - by);
	int zScale = cz / (ez - bz);
	////设置有效性,Z值和控制面上的点
	unsigned long long ix = 0, iy = 0, iz = 0;
	unsigned long long iix = bx + ix / xScale, iiy = by + iy / yScale, iiz = bz + iz / zScale;
	Cell outCell;
	getCurrentCell(iix, iiy, iiz);
	for (iz = 0; iz<cz; iz++){
		for (iy = 0; iy<cy; iy++){
			for (ix = 0; ix<cx; ix++){
				if (iix != bx + ix / xScale || iiy != by + iy / yScale || iiz != bz + iz / zScale){
					iix = bx + ix / xScale, iiy = by + iy / yScale, iiz = bz + iz / zScale;
					getCurrentCell(iix, iiy, iiz);
				}
				pr->setCellValid(ix, iy, iz, _currentCell.isValid());
				this->calculate_subcell_vertices(ix, iy, iz, xScale, yScale, zScale, outCell);
				//设置Z值
				for (unsigned long long jPos = 0; jPos <= 1; jPos++)
				{
					for (unsigned long long iPos = 0; iPos <= 1; iPos++)
					{
						for (unsigned long long kPos = 0; kPos <= 1; kPos++)
						{
							unsigned long long zcorn_index = (2 * iz + kPos)*(4 * _xCellNumber*_yCellNumber) + (2 * iy + jPos) * (2 * _xCellNumber) + (2 * ix + iPos);
							assert(zcorn_index< (8 * _xCellNumber * _yCellNumber * _zCellNumber));
							_zValues[zcorn_index] = outCell.getVertices().at(_reindex[iPos][jPos][kPos]).z;
						}
					}
				}
			}
		}
	}
	//设置顶底面坐标
	for (iy = 0; iy<cy; iy++){
		for (ix = 0; ix<cx; ix++){
			if (ix == 0){
				getCurrentCell(ix, iy, 0);
				_topCtrlVertices.push_back(_currentCell.getVertices().at(0));
				getCurrentCell(ix, iy, _zCellNumber - 1);
				_bottomCtrlVertices.push_back(_currentCell.getVertices().at(4));
			}
			getCurrentCell(ix, iy, 0);
			_topCtrlVertices.push_back(_currentCell.getVertices().at(1));
			getCurrentCell(ix, iy, _zCellNumber - 1);
			_bottomCtrlVertices.push_back(_currentCell.getVertices().at(5));
			if (iy == cy - 1){
				getCurrentCell(ix, iy, 0);
				_topCtrlVertices.push_back(_currentCell.getVertices().at(3));
				getCurrentCell(ix, iy, _zCellNumber - 1);
				_bottomCtrlVertices.push_back(_currentCell.getVertices().at(7));
			}
		}
	}

	return MeshSharedPtr((Mesh*)pr);
}

void EclipseMeshImpl::queryInterface(int type, void ** ppInterface)
{
	switch (type){
	case MESH_TYPE_ECLIPSE:
	{
		*ppInterface = (void*)dynamic_cast<EclipseMesh*>(this);
		break;
	}
	case MESH_TYPE_UNKONWN:
	{
		*ppInterface = (void*)dynamic_cast<Mesh*>(this);
		break;
	}
	default:
	{
		*ppInterface = 0;
		break;
	}
	}
}



/**
*函数名:		int EclipseMesh::generate_pillar_vertex(double P12[6],double zcorn, double xyz[3])
*功  能:	求给定地层顶底面位于一条直线上给定Z值的空间坐标。
*参  数:	P12，位于给定顶底面同一节点序号两个点的空间坐标；
zcorn,位于给定2点上某一个角点网格顶点的Z值
xyz,求出给定Z值点的空间坐标
(入口):	P12[6],zcorn
(出口):	xyz[3]
*返   回:	求值成功，则返回1，否则返回0
*/
int EclipseMeshImpl::generate_pillar_vertex(double P12[6], double zcorn, double xyz[3])
{
	double x1, y1, z1, x2, y2, z2, x, y, z;
	x1 = P12[0]; y1 = P12[1]; z1 = P12[2];
	x2 = P12[3]; y2 = P12[4]; z2 = P12[5];
	z = zcorn;

	if ((x1 == x2) && (y1 == y2)) {
		x = x1; y = y1;
	}
	else {
		if (z1 == z2) {
			return 0;
		}
		double a = (z - z1) / (z2 - z1);
		double b = (1 - a);
		x = b*x1 + a*x2;
		y = b*y1 + a*y2;
	}

	xyz[0] = x;
	xyz[1] = -y;
	xyz[2] = -z;
	return 1;
}



/**
*函数名: int EclipseMesh::generate_cell_vertices(int i,int j,int k,double xyz[24])
*功  能:	计算一个角点网格8个顶点的空间坐标
*参  数:	某个角点网格在I,J,K方向的序号，xyz[24]存储8个顶点坐标
(入口):	i,j,k
(出口):	xyz[24]
*返   回:	计算成功，则返回1，否则返回0
*/
int EclipseMeshImpl::generate_cell_vertices(unsigned long long i, unsigned long long  j, unsigned long long  k, double xyz[24])
{
	double P12[6];
	unsigned long long pillar_index;
	unsigned long long zcorn_index;
	double zcorn;
	double v[3];
	unsigned long long res = 0;
	for (unsigned long long jPos = 0; jPos <= 1; jPos++)
	{
		for (unsigned long long iPos = 0; iPos <= 1; iPos++)
		{
			for (unsigned long long kPos = 0; kPos <= 1; kPos++)
			{
				pillar_index = (j + jPos) * (_xCellNumber + 1) + (i + iPos);
				assert(pillar_index < ((_xCellNumber + 1) * (_yCellNumber + 1)));

				//memcpy(P12,m_coord+(pillar_index*6),sizeof(double)*6);
				P12[0] = _topCtrlVertices[pillar_index].x;
				P12[1] = _topCtrlVertices[pillar_index].y;
				P12[2] = _topCtrlVertices[pillar_index].z;
				P12[3] = _bottomCtrlVertices[pillar_index].x;
				P12[4] = _bottomCtrlVertices[pillar_index].y;
				P12[5] = _bottomCtrlVertices[pillar_index].z;


				zcorn_index = (2 * k + kPos)*(4 * _xCellNumber*_yCellNumber) + (2 * j + jPos) * (2 * _xCellNumber) + (2 * i + iPos);
				assert(zcorn_index< (8 * _xCellNumber * _yCellNumber * _zCellNumber));

				zcorn = _zValues[zcorn_index];

				res = generate_pillar_vertex(P12, zcorn, v);
				memcpy(xyz + (3 * _reindex[iPos][jPos][kPos]), v, sizeof(double) * 3);

				if (res != 1)
					return(res);
			}
		}
	}

	return(1);
}
//将当前网格划分成xScale*yScale*zScale个网格，计算每个子网格单元，并返回
void EclipseMeshImpl::calculate_subcell_vertices(unsigned long long ix, unsigned long long iy, unsigned long long iz, int xScale, int yScale, int zScale, Cell & outCell){
	ix = ix%xScale;
	iy = iy%yScale;
	iz = iz%zScale;

	////计算0点
	//Vertex3d v0 ;
	//v0.x = (_currentCell.getVertices()[1].x-_currentCell.getVertices()[0].x)/xScale * ix + _currentCell.getVertices()[0].x;
	//v0.y = (_currentCell.getVertices()[1].y-_currentCell.getVertices()[0].y)/yScale * iy + _currentCell.getVertices()[0].y;
	//v0.z = (_currentCell.getVertices()[1].z-_currentCell.getVertices()[0].z)/zScale * iz + _currentCell.getVertices()[0].z;
	////计算1点
	//Vertex3d v1 ;
	//v1.x = (_currentCell.getVertices()[1].x-_currentCell.getVertices()[0].x)/xScale * (ix+1) + _currentCell.getVertices()[0].x;
	//v1.y = (_currentCell.getVertices()[1].y-_currentCell.getVertices()[0].y)/yScale * (iy+1) + _currentCell.getVertices()[0].y;
	//v1.z = (_currentCell.getVertices()[1].z-_currentCell.getVertices()[0].z)/zScale * (iz+1) + _currentCell.getVertices()[0].z;
	////计算2点
	//Vertex3d v2 ;
	//v2.x = (_currentCell.getVertices()[2].x-_currentCell.getVertices()[3].x)/xScale * (ix+1) + _currentCell.getVertices()[3].x;
	//v2.y = (_currentCell.getVertices()[2].y-_currentCell.getVertices()[3].y)/yScale * (iy+1) + _currentCell.getVertices()[3].y;
	//v2.z = (_currentCell.getVertices()[2].z-_currentCell.getVertices()[3].z)/zScale * (iz+1) + _currentCell.getVertices()[3].z;
	//x方向计算
	Vertex3d v0, v1, v2, v3, v4, v5, v6, v7;
	v0 = (double)ix / (double)xScale * (_currentCell.getVertices()[1] - _currentCell.getVertices()[0]) + _currentCell.getVertices()[0];
	v1 = (double)(ix + 1) / (double)xScale * (_currentCell.getVertices()[1] - _currentCell.getVertices()[0]) + _currentCell.getVertices()[0];
	v2 = (double)(ix + 1) / (double)xScale * (_currentCell.getVertices()[2] - _currentCell.getVertices()[3]) + _currentCell.getVertices()[3];
	v3 = (double)ix / (double)xScale * (_currentCell.getVertices()[2] - _currentCell.getVertices()[3]) + _currentCell.getVertices()[3];
	v4 = (double)ix / (double)xScale * (_currentCell.getVertices()[5] - _currentCell.getVertices()[4]) + _currentCell.getVertices()[4];
	v5 = (double)(ix + 1) / (double)xScale * (_currentCell.getVertices()[5] - _currentCell.getVertices()[4]) + _currentCell.getVertices()[4];
	v6 = (double)(ix + 1) / (double)xScale * (_currentCell.getVertices()[6] - _currentCell.getVertices()[7]) + _currentCell.getVertices()[7];
	v7 = (double)ix / (double)xScale * (_currentCell.getVertices()[6] - _currentCell.getVertices()[7]) + _currentCell.getVertices()[7];
	//y方向计算
	Vertex3d v00, v11, v22, v33, v44, v55, v66, v77;
	v00 = (double)iy / (double)yScale * (v3 - v0) + v0;
	v11 = (double)iy / (double)yScale * (v2 - v1) + v1;
	v22 = (double)(iy + 1) / (double)yScale * (v2 - v1) + v1;
	v33 = (double)(iy + 1) / (double)yScale * (v3 - v0) + v0;
	v44 = (double)iy / (double)yScale * (v7 - v4) + v4;
	v55 = (double)iy / (double)yScale * (v6 - v5) + v5;
	v66 = (double)(iy + 1) / (double)yScale * (v6 - v5) + v5;
	v77 = v44 = (double)(iy + 1) / (double)yScale * (v7 - v4) + v4;
	//z方向计算
	Vertex3d v000, v111, v222, v333, v444, v555, v666, v777;
	v000 = (double)iz / (double)zScale * (v44 - v00) + v00;
	v111 = (double)iz / (double)zScale * (v55 - v11) + v11;
	v222 = (double)iz / (double)zScale * (v66 - v22) + v22;
	v333 = (double)iz / (double)zScale * (v77 - v33) + v33;
	v444 = (double)(iz + 1) / (double)zScale * (v44 - v00) + v00;
	v555 = (double)(iz + 1) / (double)zScale * (v55 - v11) + v11;
	v666 = (double)(iz + 1) / (double)zScale * (v66 - v22) + v22;
	v777 = (double)(iz + 1) / (double)zScale * (v77 - v33) + v33;

	outCell.getVertices().push_back(v000);
	outCell.getVertices().push_back(v111);
	outCell.getVertices().push_back(v222);
	outCell.getVertices().push_back(v333);
	outCell.getVertices().push_back(v444);
	outCell.getVertices().push_back(v555);
	outCell.getVertices().push_back(v666);
	outCell.getVertices().push_back(v777);

}

bool EclipseMeshImpl::readAscii(const std::string & fileName){
	EclipseMeshReader reader;
	reader.readGredclFile(fileName);
	reader.constructMesh(this);
	/*Envelope3d e3d = getEnvelope();
	_origin = e3d.getMin();
	setOrigin(_origin);*/
	return true;
}
BlockSharedPtr  EclipseMeshImpl::generate_block(unsigned long long xCellNumberPreBlock, unsigned long long yCellNumberPreBlock, unsigned long long zCellNumberPreBlock,
	unsigned int x_blkidx, unsigned int z_blkidx, unsigned int y_blkidx)
{
	BlockSharedPtr block(new Block(xCellNumberPreBlock, yCellNumberPreBlock, zCellNumberPreBlock));

	block->setXBegin(x_blkidx*xCellNumberPreBlock);
	block->setYBegin(z_blkidx*yCellNumberPreBlock);
	block->setZBegin(y_blkidx*zCellNumberPreBlock);
	unsigned long long x, y, z;
	CellSharedPtr csp;
	for (unsigned long long k = 0; k<zCellNumberPreBlock; k++){
		z = k + block->getZBegin();
		for (unsigned long long j = 0; j<yCellNumberPreBlock; j++){
			y = j + block->getYBegin();
			for (unsigned long long i = 0; i<xCellNumberPreBlock; i++){
				x = i + block->getXBegin();
				if (x<_xCellNumber && y<_yCellNumber && z<_zCellNumber)
				{
					csp = getCell(x, y, z);
					if (csp.get())
						block->addCell(csp);
				}
			}
		}
	}
	if (block->getCells().empty())
		return BlockSharedPtr();
	else
		return block;
}
void EclipseMeshImpl::getBlocks(unsigned long long xCellNumberPreBlock, unsigned long long yCellNumberPreBlock, unsigned long long zCellNumberPreBlock, std::vector<BlockSharedPtr> & blocks){
	////计算X方向上分块个数
	//unsigned long long bcx = _xCellNumber/xCellNumberPreBlock;
	//if(_xCellNumber%xCellNumberPreBlock!=0)
	//	bcx++;
	////计算Y方向上分块个数
	//unsigned long long bcy = _yCellNumber/yCellNumberPreBlock;
	//if(_yCellNumber%yCellNumberPreBlock!=0)
	//	bcy++;
	////计算Z方向上分块个数
	//unsigned long long bcz = _zCellNumber/zCellNumberPreBlock;
	//if(_zCellNumber%zCellNumberPreBlock!=0)
	//	bcz++;
	//unsigned long long x,y,z;
	//CellSharedPtr csp;
	//for(unsigned long long iz = 0;iz<bcz;iz++){
	//	for(unsigned long long iy = 0;iy<bcy;iy++){
	//		for(unsigned long long ix = 0;ix<bcx;ix++){

	//			BlockSharedPtr block(new Block(xCellNumberPreBlock,yCellNumberPreBlock,zCellNumberPreBlock));
	//			block->setXBegin(ix*xCellNumberPreBlock);
	//			block->setYBegin(iy*yCellNumberPreBlock);
	//			block->setZBegin(iz*zCellNumberPreBlock);
	//			for(unsigned long long k=0;k<zCellNumberPreBlock;k++){
	//				for(unsigned long long j=0;j<yCellNumberPreBlock;j++){
	//					for(unsigned long long i=0;i<xCellNumberPreBlock;i++){
	//						x= i+ix*xCellNumberPreBlock;
	//						y= j+iy*yCellNumberPreBlock;
	//						z= k+iz*zCellNumberPreBlock;
	//						if(x>=_xCellNumber || y>=_yCellNumber || z>=_zCellNumber)
	//							continue;
	//						else{
	//							csp=getCell(x,y,z);
	//							if(csp.get())
	//								block->addCell(csp);
	//						}
	//					}
	//				}
	//			}

	//			if(!block->getCells().empty()){
	//				blocks.push_back(block);
	//			}

	//		}
	//	}
	//}

	//计算X方向上分块个数
	unsigned long long bcx = _xCellNumber / xCellNumberPreBlock;
	if (_xCellNumber%xCellNumberPreBlock != 0)
		bcx++;

	//计算Y方向上分块个数
	unsigned long long bcy = _yCellNumber / yCellNumberPreBlock;
	if (_yCellNumber%yCellNumberPreBlock != 0)
		bcy++;

	//计算Z方向上分块个数
	unsigned long long bcz = _zCellNumber / zCellNumberPreBlock;
	if (_zCellNumber%zCellNumberPreBlock != 0)
		bcz++;

	unsigned long long x, y, z;
	BlockSharedPtr bsp;
	for (unsigned long long iz = 0; iz<bcz; iz++){
		for (unsigned long long iy = 0; iy<bcy; iy++){
			for (unsigned long long ix = 0; ix<bcx; ix++){
				bsp = this->generate_block(xCellNumberPreBlock, yCellNumberPreBlock, zCellNumberPreBlock, ix, iy, iz);
				if (bsp.get())
					blocks.push_back(bsp);
			}
		}
	}
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace