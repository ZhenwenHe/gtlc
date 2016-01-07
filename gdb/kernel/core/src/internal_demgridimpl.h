#pragma once
#include "terrain.h"


begin_gtl_namespace
begin_gdb_namespace

 

class DEMGridImpl :	public DEMGrid
{
public:

public:
	/** 从Buffer中读取信息填充本几何对象
	* @param  [in,out] buf Buffer & ，缓冲区对象引用
	* @return 成功返回true，否则返回false
	*/
	virtual bool readBuffer(Buffer & buf){
		return false;
	}
	/** 将本类的信息写入Buffer中
	* @param  [in,out] buf Buffer & ，缓冲区对象引用
	* @return 成功返回true，否则返回false
	*/
	virtual bool writeBuffer(Buffer& buf){
		return false;
	}

	virtual bool copy(const DEMGridSharedPtr & src, bool bCopyData)
	{
		return DEMGridImpl::CopyDem(*(DEMGridImpl*)(src.get()), *this, bCopyData) == 0 ? false : true;
	}
	virtual Envelope3d getEnvelope()
	{
		return  GetDEMScope(); 
	}
	virtual void setAlpha(double dAlfa){
		SetDEMAlpha(dAlfa);
	}

	virtual void setBelta(double dBelta){
		SetDEMBelta(dBelta);
	}
	virtual double getAlpha() const
	{
		return GetDEMAlpha();
	}

	virtual double getBelta() const
	{
		return GetDEMBelta();
	}

	virtual	void setOrigin(double dx, double dy)
	{
		SetDEMOriginXY(dx, dy);
	}

	virtual	void getOrigin(double& dx, double& dy)
	{
		GetDEMOriginXY(dx, dy);
	}
	virtual	void setHeightRange(float fminz, float fmaxz)
	{
		SetDEMHeightRange(fminz, fmaxz);
	}

	virtual  void getHeightRange(float& fminz, float& fmaxz)
	{
		GetDEMHeightRange(fminz, fmaxz);
	}
	virtual void setRows(int nRow){
		m_nRow = nRow;
	}

	virtual void setCols(int nCol){
		m_nCol = nCol;
	}

	virtual int getRows()const{
		return m_nRow;
	}

	virtual int getCols()const{
		return m_nCol;
	}
	virtual void setCoordinateSystem(int bFlag = 1){
		SetDEMCoorFlag(bFlag);
	}

	virtual int getCoordinateSystem(){
		return m_bCoorFlag;
	}

	virtual void setUnit(int bFlag = 1){
		m_bUnitFlag = bFlag;
	}

	virtual int getUnit(){
		return m_bUnitFlag;
	}

	virtual void setCellSize(double dSize){
		m_dCellSize = dSize;
	}
	virtual double getCellSize(){
		return m_dCellSize;
	}

	virtual void setScale(long lScale){
		m_lScale = lScale;
	}
	virtual long getScale(){
		return m_lScale;
	}

	virtual void setHeightData(float *pData, bool bAlloced = true){
		SetDEMHeightData(pData, bAlloced);
	}

	virtual float* getHeightData(){
		return m_pfDemHeightData;
	}

	virtual void setVextexColors(std::vector<Color4f> & colors){
		size_t s = colors.size();
		unsigned long * cs = new unsigned long[s];
		for (size_t i = 0; i<s; i++)
		{
			long c = 0;
			Color::convert(colors[i], c);
			cs[i] = c;
		}
		SetDEMVexColor(cs, true);
	}

	virtual void getVextexColors(std::vector<Color4f> & colors){
		//unsigned long* _colors =GetDEMVexColor();
		size_t s = getRows()*getCols();
		for (size_t i = 0; i<s; i++)
		{
			Color4f _col;
			Color::convert(m_pcVexColor[i], _col);
			colors.push_back(_col);
		}

	}

	virtual void setVertexTextureCoordinates(std::vector<Vertex2d> & VexTexCoor){
		size_t s = VexTexCoor.size();
		Vertex2f* vexTexCoor = new Vertex2f[s];
		for (size_t i = 0; i<s; i++)
		{
			vexTexCoor[i].x = VexTexCoor[i].x;
			vexTexCoor[i].y = VexTexCoor[i].y;
		}

		SetDEMVexTexCoor(vexTexCoor, true);
	}
	virtual void getVertexTextureCoordinates(std::vector<Vertex2d> & VexTexCoor){
		//Vertex2f* vexTexCoor = GetDEMVexTexCoor();
		size_t s = getRows()*getCols();
		for (size_t i = 0; i<s; i++)
		{
			VexTexCoor[i].x = m_pvVexTexCoor[i].x;
			VexTexCoor[i].y = m_pvVexTexCoor[i].y;
		}
	}
	virtual void setInvalidValue(long lInvalidata){
		m_lInvalidData = lInvalidata;
	}

	virtual long getInvalidValue(){
		return m_lInvalidData;
	}
	virtual double interpolate(const double x, const double y){
		Vertex3d _v;
		_v.x = x;
		_v.y = y;
		Interpolate(_v);
		return _v.z;
	}

	virtual int interpolate(Vertex3d & v){
		Vertex3d _v;
		converFroGv(_v, v);
		int r = Interpolate(_v);
		v.x = _v.x;
		v.y = _v.y;
		v.z = _v.z;
		return r;
	}
	virtual int interpolate(std::vector<Vertex3d>& vVertices){
		if (vVertices.empty()) return 0;
		std::vector<Vertex3d>::iterator it = vVertices.begin();

		for (; it != vVertices.end(); it++){
			interpolate(*it);
			/*if(interpolate(*it)==0){
			it->z=m_lInvalidData;
			}*/
		}
		return 1;
	}
	virtual Vertex3d getMaxPoint() const {
		//没实现GetMaxPoint(),所以整个还没实现
		//Vertex3d _v = GetMaxPoint();
		//Vertex3d v;
		//v.x=_v.x;
		//v.y=_v.y;
		//v.z=_v.z;
		//return v;
		return Vertex3d();
	}

	virtual Vertex3d getMinPoint() const{
		//没实现GetMaxPoint(),所以整个还没实现
		//Vertex3d _v  = GetMinPoint();
		//Vertex3d v;
		//v.x=_v.x;
		//v.y=_v.y;
		//v.z=_v.z;
		//return v;
		return Vertex3d();
	}
	virtual void scale(double dScale)
	{
		//没实现ScaleDEM(),所以整个还没实现
		//ScaleDEM(dScale);
	}
	virtual Vertex3d getMaxPointInPolygon(std::vector<Vertex3d>& pPolygon) const{

		//没实现ScaleDEM(),所以整个还没实现
		return Vertex3d();
	}

	virtual Vertex3d getMinPointInPolygon(std::vector<Vertex3d>&pPolygon) const{
		//没实现ScaleDEM(),所以整个还没实现
		return Vertex3d();
	}
	virtual int clip(std::vector<Vertex3d>&pPolygon){
		//没实现ScaleDEM(),所以整个还没实现
		return 0;
	}

	virtual void extract(int beg_nRow, int beg_nCol, int end_nRow, int end_nCol, DEMGridSharedPtr& extractGrid){
		if (!extractGrid.get())
			extractGrid = DEMGrid::create();
		extractGrid->setOrigin(m_dXOrigin + beg_nCol*m_dCellSize, m_dYOrigin + beg_nRow*m_dCellSize);
		extractGrid->setAlpha(m_dAlfa);
		extractGrid->setBelta(m_dBelta);
		extractGrid->setCellSize(m_dCellSize);
		extractGrid->setCols(end_nCol - beg_nCol + 1);
		extractGrid->setRows(end_nRow - beg_nRow + 1);
		extractGrid->setCoordinateSystem(m_bCoorFlag);
		extractGrid->setScale(m_lScale);
		extractGrid->setUnit(m_bUnitFlag);
		extractGrid->setInvalidValue(m_lInvalidData);
		extractGrid->setScale(m_lScale);
		//设置每个顶点的颜色值
		if (m_pcVexColor != NULL)
		{
			//extractGrid->setVextexColors
		}
		//设置每个顶点的纹理坐标
		if (m_pvVexTexCoor != NULL)
		{
		}
		//设置高程值
		if (m_pfDemHeightData != NULL)
		{
			if (extractGrid->getHeightData() == NULL)
			{
				float* pd = new float[(end_nRow - beg_nRow + 1)*(end_nCol - beg_nCol + 1)];
				extractGrid->setHeightData(pd, true);
			}
			for (int i = beg_nRow; i <= end_nRow; i++)
			{
				for (int j = beg_nCol; j <= end_nCol; j++)
				{
					//float* pd = new float[(end_nRow-beg_nRow+1)*(end_nCol-beg_nCol+1)]
					double _x, _y;
					getXY(i, j, _x, _y);
					extractGrid->interpolate(_x, _y);
				}
			}
		}
	}
	virtual void getDEMBound(std::vector<Vertex3d>&pPolygon){
		////没实现ScaleDEM(),所以整个还没实现
	}
	virtual void transform(Matrix4x4& m4x4){
		//没实现
	}

	//另外添加用于转换的两个辅助函数
	void converToGv(Vertex3d & _v, Vertex3d & v){
		v.x = _v.x;
		v.y = _v.y;
		v.z = _v.z;
	}
	void converFroGv(Vertex3d & _v, Vertex3d & v){
		_v.x = v.x;
		_v.y = v.y;
		_v.z = v.z;
	}

protected:

	/** @name 属性（成员变量）
	* @{
	*/

	/** 左下角X原点坐标 */
	double m_dXOrigin;

	/** 左下角Y原点坐标 */
	double m_dYOrigin;

	/** 最大，最小高程值 */
	float m_fMinZ, m_fMaxZ;

	/** alfa */
	double m_dAlfa;

	/** belta */
	double m_dBelta;

	/** DEM列数 */
	int m_nCol;

	/** DEM行数 */
	int m_nRow;

	/** 坐标系表示,1为大地坐标,0为经纬度坐标系 */
	int m_bCoorFlag;

	/** 坐标单位,1为米,0为毫米 */
	int m_bUnitFlag;

	/** 格网间距 */
	double m_dCellSize;

	/** 无效高程值 */
	long m_lInvalidData;

	/** 比例尺 */
	long m_lScale;

	/** 每个格网点高程数据 */
	float *m_pfDemHeightData;

	/** DEM 每个格网点颜色值 */
	unsigned long *m_pcVexColor;

	/** 顶点纹理坐标 */
	Vertex2f *m_pvVexTexCoor;

	/** DEM 高程、颜色、纹理坐标等数据是需要自身维护的内存 */
	bool m_bDemDataAlloced;
	bool m_bVexColorAlloced;
	bool m_bTexCoorAlloced;

	/** @} */ // 属性结尾

public:

	/** @defgroup DEMGridImpl 操作（成员函数）
	* @{
	*/


	/** @name 构造与析构函数
	*  @{
	*/

	/** 构造函数
	*/
	DEMGridImpl() :m_dXOrigin(0), m_dYOrigin(0), m_fMinZ(0), m_fMaxZ(0), m_dAlfa(0), m_dBelta(0), m_nCol(0), m_nRow(0), m_dCellSize(0),
		m_bCoorFlag(1), m_bUnitFlag(1), m_lInvalidData(-99999), m_lScale(1), m_pfDemHeightData(NULL), m_pcVexColor(NULL), m_pvVexTexCoor(NULL),
		m_bDemDataAlloced(true), m_bVexColorAlloced(true), m_bTexCoorAlloced(true) {}


	virtual ~DEMGridImpl()
	{
		if (m_pfDemHeightData)
		{
			if (m_bDemDataAlloced)
				delete[]m_pfDemHeightData;
			m_pfDemHeightData = NULL;
		}
		if (m_pcVexColor)
		{
			if (m_bVexColorAlloced)
				delete[]m_pcVexColor;
			m_pcVexColor = NULL;
		}
		if (m_pvVexTexCoor)
		{
			if (m_bTexCoorAlloced)
				delete[]m_pvVexTexCoor;
			m_pvVexTexCoor = NULL;
		}
	}
	/** @} */ // 构造与析构函数结尾

	/** 拷贝dem数据
	* @param [in] src	源dem数据
	* @param [out] tar	dem数据
	* @return 1 -- 拷贝成功；0 -- 拷贝失败
	*/
	static long CopyDem(DEMGridImpl& src, DEMGridImpl& tar, bool bCopyData);


	/** @name 数据操作
	*  @{
	*/
	/** 获取dem的三维外包盒
	* @param
	* @return dem的三维外包盒
	*/
	Envelope3d GetDEMScope()
	{
		Vertex3d vMin, vMax;
		GetDEMOriginXY(vMin.x, vMin.y);
		vMax.x = vMin.x + (m_nCol - 1) * m_dCellSize;
		vMax.y = vMin.y + (m_nRow - 1) * m_dCellSize;
		float minz, maxz;
		GetDEMHeightRange(minz, maxz);
		vMin.z = minz;
		vMax.z = maxz;

		Envelope3d env;
		env.setMin(vMin);
		env.setMax(vMax);
		return env;
	}

	/** 设置alfa
	* @param [in] dAlfa	数据
	*/
	void SetDEMAlpha(double dAlfa)
	{
		m_dAlfa = dAlfa;
	}

	/** 设置Belta
	* @param [in] dBelta	数据
	*/
	void SetDEMBelta(double dBelta)
	{
		m_dBelta = dBelta;
	}
	/** 获取alfa
	* @return Alfa
	*/
	double GetDEMAlpha() const
	{
		return m_dAlfa;
	}
	/** 获取belta
	* @return Belta
	*/
	double GetDEMBelta() const
	{
		return m_dBelta;
	}

	/** 设置DEM左下角原点x,y坐标
	* @param [in] dx x坐标
	* @param [in] dy y坐标
	*/
	void SetDEMOriginXY(double dx, double dy)
	{
		m_dXOrigin = dx;
		m_dYOrigin = dy;
	}

	/** 获取DEM左下角原点x,y坐标
	* @param [out] dx DEM左下角原点x坐标
	* @param [out] dy DEM左下角原点y坐标
	*/
	void GetDEMOriginXY(double& dx, double& dy)
	{
		dx = m_dXOrigin;
		dy = m_dYOrigin;
	}
	/** 设置DEM高程范围
	* @param [in] fminz 最小高程值
	* @param [in] fmaxz 最大高程值
	*/
	void SetDEMHeightRange(float fminz, float fmaxz)
	{
		m_fMinZ = fminz;
		m_fMaxZ = fmaxz;
	}
	/** 获取DEM高程范围
	* @param [out] fminz DEM最小高程值
	* @param [out] fmaxz DEM最大高程值
	*/
	void GetDEMHeightRange(float& fminz, float& fmaxz)
	{
		fminz = m_fMinZ;
		fmaxz = m_fMaxZ;
	}
	/** 设置DEM行数和列数
	* @param [in] nCol dem列数
	* @param [in] nRow dem行数
	*/
	void SetDEMRowCol(int nCol, int nRow)
	{
		m_nRow = nRow;
		m_nCol = nCol;
	}
	/** 获取DEM行数和列数
	* @param [out] nCol dem列数
	* @param [out] nRow dem行数
	*/
	void GetDEMRowCol(int& nCol, int& nRow)
	{
		nCol = m_nCol;
		nRow = m_nRow;
	}
	/** 设置DEM坐标系类型
	* @param [in] bflag,坐标系类型,分为以下几种:
	* -1为大地坐标
	* -0为经纬度坐标系
	*/
	void SetDEMCoorFlag(int bFlag = 1)
	{
		m_bCoorFlag = bFlag;
	}
	/** 获取DEM坐标系类型
	* @return 坐标系类型,分为以下几种:
	* -1为大地坐标
	* -0为经纬度坐标系
	*/
	int GetDEMCoorFlag()
	{
		return m_bCoorFlag;
	}
	/** 设置DEM单位类型
	* @param [in] bflag,坐标单位类型,分为以下几种:
	* -1为米
	* -0为毫米
	*/
	void SetDEMUnitFlag(int bFlag = 1)
	{
		m_bUnitFlag = bFlag;
	}
	/** 获取DEM坐标单位类型
	* @return 坐标系类型,分为以下几种:
	* -1为米
	* -0为毫米
	*/
	int GetDEMUnitFlag()
	{
		return m_bUnitFlag;
	}
	/** 设置DEM格网间距
	* @param [in] dSize,格网间距
	*/
	void SetDEMCellSize(double dSize)
	{
		m_dCellSize = dSize;
	}
	/** 获取DEM格网间距
	* @return double类型格网间距
	*/
	double GetDEMCellSize()
	{
		return m_dCellSize;
	}
	/** 设置DEM高程值缩放比例
	* @param [in] lScale,高程值缩放比例
	*/
	void SetDEMScale(long lScale)
	{
		m_lScale = lScale;
	}
	/** 获取DEM高程值缩放比例
	* @return 高程值缩放比例
	*/
	long GetDEMScale()
	{
		return m_lScale;
	}
	/** 设置DEM每个格网点的高程数据
	* @param [in] pData,指向float类型的一串数据的指针
	* @param [in] bAlloced,是否需要自身维护内存
	* -自身维护内存
	* -自身不维护内存
	*/
	void SetDEMHeightData(float *pData, bool bAlloced = true)
	{
		if (m_pfDemHeightData)
		{
			if (m_bDemDataAlloced)
			{
				delete[]m_pfDemHeightData;
			}
			m_pfDemHeightData = NULL;
		}

		m_pfDemHeightData = pData;
		m_bDemDataAlloced = bAlloced;
	}
	/** 获取DEM每个格网点的高程数据
	* @return 指向dem每个格网点高程数据的指针
	*/
	float* GetDEMHeightData()
	{
		return m_pfDemHeightData;
	}
	/** 设置DEM每个格网点的颜色值
	* @param [in] pData,指向unsigned long类型的一串数据的指针
	* @param [in] bAlloced,是否需要自身维护内存
	* -自身维护内存
	* -自身不维护内存
	*/
	void SetDEMVexColor(unsigned long *pColor, bool bAlloced = true)
	{
		if (m_pcVexColor)
		{
			if (m_bVexColorAlloced)
			{
				delete[]m_pcVexColor;
			}
			m_pcVexColor = NULL;
		}
		m_pcVexColor = pColor;
		m_bVexColorAlloced = bAlloced;
	}
	/** 获取DEM每个格网点的颜色值
	* @return 指向dem每个格网点颜色数据的指针
	*/
	unsigned long* GetDEMVexColor()
	{
		return m_pcVexColor;
	}
	/** 设置DEM每个格网顶点的纹理坐标
	* @param [in] pData,指向Vertex2f类型的一串数据的指针
	* @param [in] bAlloced,是否需要自身维护内存
	* -自身维护内存
	* -自身不维护内存
	*/
	void SetDEMVexTexCoor(Vertex2f* pVexTexCoor, bool bAlloced = true)
	{
		if (m_pvVexTexCoor)
		{
			if (m_bTexCoorAlloced)
			{
				delete[]m_pvVexTexCoor;
			}
			m_pvVexTexCoor = pVexTexCoor;
		}
		m_pvVexTexCoor = pVexTexCoor;
		m_bTexCoorAlloced = bAlloced;
	}
	/** 获取DEM每个格网顶点的纹理坐标
	* @return 指向dem每个格网顶点纹理坐标数据的指针
	*/
	Vertex2f* GetDEMVexTexCoor()
	{
		return m_pvVexTexCoor;
	}
	/** 设置DEM无效高程值
	* @param [in] pData,指向long类型的数据,数据为无效高程值的大小
	*/
	void SetDEMInvalidata(long lInvalidata)
	{
		m_lInvalidData = lInvalidata;
	}
	/** 获取DEM无效高程值
	* @return DEM无效高程值
	*/
	long GetDEMInvalidata()
	{
		return m_lInvalidData;
	}


	/** @} */ // 数据操作组结尾



	/** @name 计算操作
	*  @{
	*/

	//##ModelId=49EF1205035B
	/** 在当前DEM上内插出传入点的高程
	* @param [in] vVer	传入点坐标
	*/
	int Interpolate(Vertex3d& vVer);

	//##ModelId=49EF141C0232
	/** 在当前DEM上内插出传入点集的所有高程
	* 功能：内插给定点的高程值.
	* 参数：PointIn为给定的点，vVertices为给定的点集，其中x,y应当由调用者赋值，而z则是内插的高程值.
	* 注意：传进来的坐标一定是大地坐标，绝对的，不是相对的（减DEM原点的）
	*      如果给定的点平面位置不在DEM范围内，则内插的高程值将是无效值NODATA。
	* @param [in] vVertices	传入点集
	*/
	int Interpolate(std::vector<Vertex3d>& vVertices);

	//##ModelId=49EF1297030D
	/** 得到当前DEM高程最大的点
	* @return 当前DEM高程最大的点
	*/
	Vertex3d GetMaxPoint() const;

	//##ModelId=49EF12AC01E4
	/** 得到当前DEM高程最小的点
	* @return 当前DEM高程最小的点
	*/
	Vertex3d GetMinPoint() const;

	//##ModelId=49EF12FC03A9
	/** 对当前DEM上的所有点的高程进行缩放
	* @param [in] dScale 缩放比例
	*/
	void ScaleDEM(double dScale);

	

	//##ModelId=49EF13E7001F
	/** 抽取一定范围的DEM
	* @param [in] nRow 行号
	* @param [in] nCol 列号
	*/
	void Extract(int nRow, int nCol);
 
	//##ModelId=49EF144F00EA
	/** 根据传入的旋转矩阵，旋转当前DEM
	* @param [in] m4x4 已知的旋转矩阵
	* @return
	*/
	void TranslateForm(Matrix4x4& m4x4);

	//得到Grid中行列的xy值
	void getXY(int nRow, int nCol, double& x, double &y){
		x = m_dXOrigin + m_dCellSize*nCol;
		y = m_dYOrigin + m_dCellSize*nRow;
	}

	/** @} */ // 计算操作组结尾

	/** @} */ // 操作结尾

	/** @} */ // 模块结尾


};

end_gdb_namespace
end_gtl_namespace



