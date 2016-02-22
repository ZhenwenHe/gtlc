#pragma once
/*
* Geosciences Template Library
*
* Copyright (c) 2008
* Zhenwen He (zwhe@hotmail.com)
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Zhenwen He  makes no  representations
* about the suitability of this software for any purpose.
* It is provided "as is" without express or implied warranty.
*/
#pragma once
#include "mesh.h" 


begin_gtl_namespace
begin_gdb_namespace
 

/** @addtogroup Mesh EclipseMesh-Eclipse网格
*  @{
*/
class EclipseMeshImpl : public EclipseMesh
{
public:
	static int EclipseMeshImpl::_reindex[2][2][2];
protected:
	/**（_xCellNumber+1）X (_yCellNumber+1) */
	std::vector<gdb::Vertex3d>        _topCtrlVertices;
	/**（_xCellNumber+1）X (_yCellNumber+1) */
	std::vector<gdb::Vertex3d>        _bottomCtrlVertices;
	/** 存放Z值 */
	double *                           _zValues;
protected:
	int generate_pillar_vertex(double top_bottom[6], double zcorn, double xyz[3]);
	int generate_cell_vertices(unsigned long long ix, unsigned long long iy, unsigned long long iz, double xyz[24]);
	//将当前网格划分成xScale*yScale*zScale个网格，计算每个子网格单元，并返回
	void calculate_subcell_vertices(unsigned long long ix, unsigned long long iy, unsigned long long iz, int xScale, int yScale, int zScale, Cell & outCell);
	BlockSharedPtr  generate_block(unsigned long long xCellNumberPreBlock, unsigned long long yCellNumberPreBlock, unsigned long long zCellNumberPreBlock,
		unsigned int x_blkidx, unsigned int z_blkidx, unsigned int y_blkidx);
public:
	virtual double * reallocZValues() { if (_zValues) delete[] _zValues; _zValues = new double[getZValuesNumber()]; return _zValues; }

	virtual unsigned long long getZValuesNumber() { return 8 * _xCellNumber*_yCellNumber *_zCellNumber; }

	virtual double * getZValues() { return _zValues; }

	virtual std::vector<gdb::Vertex3d> & getTopCtrlVertices() { return _topCtrlVertices; }

	virtual std::vector<gdb::Vertex3d> & getBottomCtrlVertices() { return _bottomCtrlVertices; }

	virtual void clear();

	virtual void reset(unsigned long long cx, unsigned long long cy, unsigned long long cz, unsigned long long propNumberPreCell = 1);

	virtual Cell& getCurrentCell(unsigned long long ix, unsigned long long iy, unsigned long long iz);

	virtual CellSharedPtr getCell(unsigned long long ix, unsigned long long iy, unsigned long long iz);

	virtual MeshSharedPtr submesh(
		unsigned long long bx, unsigned long long ex,
		unsigned long long by, unsigned long long ey,
		unsigned long long bz, unsigned long long ez,
		unsigned long long cx, unsigned long long cy, unsigned long long cz);

	virtual void queryInterface(int type, void ** ppInterface);

	virtual void copy(const EclipseMeshImpl & p);

	virtual bool readAscii(const std::string & fileName);
	virtual void getBlocks(unsigned long long xCellNumberPreBlock,
		unsigned long long yCellNumberPreBlock,
		unsigned long long zCellNumberPreBlock,
		std::vector<BlockSharedPtr> & blocks);

	virtual Envelope3d getEnvelope();
	virtual void setOrigin(const Vertex3d & v);
public:
	EclipseMeshImpl();
	EclipseMeshImpl(unsigned long long cx, unsigned long long cy, unsigned long long cz, unsigned long long propNumberPreCell = 1);
	EclipseMeshImpl(const EclipseMeshImpl & p);
};
/** @} */

end_gdb_namespace
end_gtl_namespace

