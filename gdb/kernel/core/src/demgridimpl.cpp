#include "internal_demgridimpl.h"
#include "mathex.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
 
 Vertex3d DEMGridImpl::GetMaxPoint() const
 {
 	// TODO: Add your specialized code here.
 	// NOTE: Requires a correct return value to compile.
	 return Vertex3d();
 }
 
 
 Vertex3d DEMGridImpl::GetMinPoint() const
 {
 	// TODO: Add your specialized code here.
 	// NOTE: Requires a correct return value to compile.

	 return Vertex3d();
 }
  
 
 void DEMGridImpl::Extract(int nRow, int nCol)
 {
 	// TODO: Add your specialized code here.
 }

//注意：传进来的坐标一定是大地坐标，绝对的，不是相对的（减DEM原点的）
//得到的是PointIn.z
int DEMGridImpl::Interpolate(Vertex3d& PointIn)
{
	double m_THRESHOLD = -99998;

	if (m_pfDemHeightData == NULL)
		return 0;

	PointIn.z = m_lInvalidData;
	int row, col;
	double fDist[4], dx, dy, fdx, fdy;
	long np, np1, np2, np3;

	//减DEM原点的
	double xx = PointIn.x - m_dXOrigin;
	double yy = PointIn.y - m_dYOrigin;

	//PointIn的X,Y 值决定了内插点所在的行列号
	row = (int)(yy / m_dCellSize);
	col = (int)(xx / m_dCellSize);

	//所求行列号 超出目前DEM 范围则高程赋予无效值
	if (row<0 || col<0 || row >= m_nRow || col >= m_nCol)
	{
		PointIn.z = m_lInvalidData;
		return 0;
	}
	np = row * m_nCol + col;
	dx = xx - col * m_dCellSize;
	dy = yy - row * m_dCellSize;
	//if(fabs(dx)<0.001 && fabs(dy)<0.001) 
	if ((fabs(dx) - 0.001) < -MathEx::TOL && (fabs(dy) - 0.001) < -MathEx::TOL)
	{
		//	if( m_pfDemHeightData[np] < m_THRESHOLD ) 
		if ((m_pfDemHeightData[np] - m_THRESHOLD) < -MathEx::TOL)
		{
			PointIn.z = m_lInvalidData;
			return 0;
		}
		else
		{
			PointIn.z = m_pfDemHeightData[np];
			return 1;
		}
	}

	if (col != m_nCol - 1)
		np1 = np + 1;
	else
		np1 = np;

	if (row != m_nRow - 1)
		np3 = np + m_nCol;
	else
		np3 = np;

	if (col != m_nCol - 1)
		np2 = np3 + 1;
	else
		np2 = np;

	/*if(m_pfDemHeightData[np]<m_THRESHOLD || m_pfDemHeightData[np1]<m_THRESHOLD ||
	m_pfDemHeightData[np2]<m_THRESHOLD || m_pfDemHeightData[np3]<m_THRESHOLD)*/
	if ((m_pfDemHeightData[np] - m_THRESHOLD) < -MathEx::TOL || (m_pfDemHeightData[np1] - m_THRESHOLD) < -MathEx::TOL ||
		(m_pfDemHeightData[np2] - m_THRESHOLD) < -MathEx::TOL || (m_pfDemHeightData[np3] - m_THRESHOLD) < -MathEx::TOL)
	{
		PointIn.z = m_lInvalidData;
		return 0;
	}
	//	if(fabs(dx)<0.001)
	if ((fabs(dx) - 0.001) < -MathEx::TOL)
	{
		fDist[0] = dy / m_dCellSize;
		PointIn.z = (m_pfDemHeightData[np] + (m_pfDemHeightData[np3] - m_pfDemHeightData[np])*fDist[0]);
		return 1;
	}
	//else if(fabs(dy)<0.001)
	else if ((fabs(dy) - 0.001) < -MathEx::TOL)
	{
		fDist[0] = dx / m_dCellSize;
		PointIn.z = (m_pfDemHeightData[np] + (m_pfDemHeightData[np1] - m_pfDemHeightData[np])*fDist[0]);
		return 1;
	}
	else
	{
		//先判断四个点是否都是有效值（只有四个点都有效了才能进行插值运算）
		if ((m_lInvalidData != m_pfDemHeightData[np]) && (m_lInvalidData != m_pfDemHeightData[np1]) && (m_lInvalidData != m_pfDemHeightData[np2]) && (m_lInvalidData != m_pfDemHeightData[np3]))
		{
			fdx = m_dCellSize - dx;
			fdy = m_dCellSize - dy;
			fDist[0] = 1.0f / (dx*dx + dy*dy);
			fDist[1] = 1.0f / (fdx*fdx + dy*dy);
			fDist[2] = 1.0f / (fdx*fdx + fdy*fdy);
			fDist[3] = 1.0f / (fdy*fdy + dx*dx);
			PointIn.z = (m_pfDemHeightData[np] * fDist[0] + m_pfDemHeightData[np1] * fDist[1]
				+ m_pfDemHeightData[np2] * fDist[2] + m_pfDemHeightData[np3] * fDist[3]) / (fDist[0] + fDist[1] + fDist[2] + fDist[3]);
			return 1;
		}
		else
		{
			PointIn.z = m_lInvalidData;
			return 0;
		}
	}
}

//只有每个点都内插了有效地高程，函数才返回TRUE;
int DEMGridImpl::Interpolate(std::vector<Vertex3d>& vVertices)
{
	long nCount = vVertices.size();

	int flag = 0;
	for (long i = 0; i<nCount; i++)
	{
		flag = Interpolate(vVertices[i]);//疑问 ： 内插后的点 需要做为源数据点 参与后续点的高程计算吗？？
		if (!flag) return 0;
	}
	return 1;
}


long DEMGridImpl::CopyDem(DEMGridImpl& src, DEMGridImpl& tar, bool bCopyData)
{
	double dXOrigin = 0.0;				//左下角X原点坐标
	double dYOrigin = 0.0;				//左下角Y原点坐标
	float fMinZ = 999999;
	float fMaxZ = -99999;				//最大，最小高程值
	double dAlfa = 1.0;					//alfa
	double dBelta = 0.0;				//belta
	int nCol = 1;						//DEM列数
	int nRow = 1;						//DEM行数
	int bCoorFlag = 1;				//坐标系表示,TRUE为大地坐标,FALSE为经纬度坐标系(m_bZblx)
	int bUnitFlag = 1;				//坐标单位,TRUE为米,FALSE为毫米(m_bMeter)
	double dCellSize = 1.0;				//格网间距(m_fCellSize)
	long lInvalidData = -99999;			//无效高程值(m_lNoData)
	long lScale = 1;					//比例尺(m_Scale)
	float *pfDemHeightData = NULL;		//DEM实际高程数据(m_pDataByte)
	unsigned long *pcVexColor = NULL;		//顶点颜色
	Vertex2f *pvVexTexCoor = NULL;		// 顶点纹理坐标

	src.GetDEMOriginXY(dXOrigin, dYOrigin);
	src.GetDEMHeightRange(fMinZ, fMaxZ);
	dAlfa = src.GetDEMAlpha();
	dBelta = src.GetDEMBelta();
	src.GetDEMRowCol(nCol, nRow);
	bCoorFlag = src.GetDEMCoorFlag();
	bUnitFlag = src.GetDEMUnitFlag();
	dCellSize = src.GetDEMCellSize();
	lInvalidData = src.GetDEMInvalidata();
	lScale = src.GetDEMScale();

	tar.SetDEMOriginXY(dXOrigin, dYOrigin);
	tar.SetDEMHeightRange(fMinZ, fMaxZ);
	tar.SetDEMAlpha(dAlfa);
	tar.SetDEMBelta(dBelta);
	tar.SetDEMRowCol(nCol, nRow);
	tar.SetDEMCoorFlag(bCoorFlag);
	tar.SetDEMUnitFlag(bUnitFlag);
	tar.SetDEMCellSize(dCellSize);
	tar.SetDEMInvalidata(lInvalidData);
	tar.SetDEMScale(lScale);

	if (bCopyData)
	{
		int np = nCol * nRow;
		pfDemHeightData = new float[np];
		float* pData1 = src.GetDEMHeightData();
		memcpy(pfDemHeightData, pData1, sizeof(float)*np);


		unsigned long* pData2 = src.GetDEMVexColor();
		if (pData2)
		{
			pcVexColor = new unsigned long[np];
			memcpy(pcVexColor, pData2, sizeof(unsigned long)*np);
		}

		Vertex2f* pData3 = src.GetDEMVexTexCoor();
		if (pData3)
		{
			pvVexTexCoor = new Vertex2f[np];
			memcpy(pvVexTexCoor, pData3, sizeof(Vertex2f)*np);
		}

		tar.SetDEMHeightData(pfDemHeightData);
		tar.SetDEMVexColor(pcVexColor);
		tar.SetDEMVexTexCoor(pvVexTexCoor);

	}
	else
	{
		float* pfDemHeightData = src.GetDEMHeightData();
		tar.SetDEMHeightData(pfDemHeightData, false);
		unsigned long* pData2 = src.GetDEMVexColor();
		tar.SetDEMVexColor(pData2, false);
		Vertex2f* pvVexTexCoor = src.GetDEMVexTexCoor();
		tar.SetDEMVexTexCoor(pvVexTexCoor, false);
	}

	return 1;
}

/** 根据传入的旋转矩阵，旋转当前DEM
* @param [in] m4x4 已知的旋转矩阵
* @return
*/
void DEMGridImpl::TranslateForm(Matrix4x4& m4x4)
{

}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace

