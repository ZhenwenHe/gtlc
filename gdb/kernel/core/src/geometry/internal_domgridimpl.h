#pragma once
#include "terrain.h"


begin_gtl_namespace
begin_gdb_namespace

class  DOMGridImpl:public DOMGrid
{
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

	virtual bool copy(const DOMGridSharedPtr & src, bool bCopyData)
	{
		return DOMGridImpl::CopyDom(*(DOMGridImpl*)(src.get()), *this, bCopyData) == 0 ? false : true;
	}
	virtual void setParameters(double resolvx, double resolvy, double east, double west, double north, double south,
		int ImageW, int ImageH, int ImageD, unsigned char * pData,
		unsigned long long datalength, bool bAlloced = true);
	virtual void setParameters(double east, double west, double north, double south, const ImageSharedPtr & p);
	virtual void getParameters(double& resolvx, double& resolvy, double& east, double& west, double& north, double& south,
		int& ImageW, int& ImageH, int& ImageD, unsigned char *& pData,
		unsigned long long & datalength, bool bAlloced = true);

	virtual void getResolution(double& resolvx, double& resolvy);

	virtual void getDirection(double& east, double& west, double& north, double& south);

	virtual void getImageSize(int& ImageW, int& ImageH, int& ImageD);

	virtual unsigned char * getImageData();
	virtual unsigned long long  getImageLength();

	virtual unsigned int getBitsPerPixel() const;
	virtual DOMBlockSharedPtr clip(const Envelope2d & rect);
	virtual unsigned char * getPixel(int ix, int iy);
	virtual unsigned char * getPixel(double x, double y);

	virtual void IP2WP(int x, int y, double * wx, double * wy);
	virtual void WP2IP(double   wx, double   wy, int *ix, int *iy);
	//rect.lengthX() 计算出xPixelNumber和yPixelMumber
	virtual unsigned long long calculateXPixelNmuber(double length);
	virtual unsigned long long  calculateYPixelNmuber(double length);

	virtual DOMBlockSharedPtr clip(int begX, int xNumbers, int begY, int yNumbers);

	virtual void clip(DOMLayerInfoSharedPtr lay, std::vector<DOMBlockSharedPtr> & blocks);
	virtual unsigned char* getLayerPixel(DOMLayerInfoSharedPtr lay, int row, int col);

protected:

	/** @name 属性（成员变量）
	* @{
	*/

	/** x方向分辨率 */
	double m_resolvx;

	/** y方向分辨率 */
	double m_resolvy;

	/** 东 */
	double m_east;

	/** 西 */
	double m_west;

	/** 北 */
	double m_north;

	/** 南 */
	double m_south;

	/** 影像宽度 */
	int m_ImageW;

	/** 影像高度 */
	int m_ImageH;

	/** 影像深度 */
	int m_ImageD;

	/** 影像数据 */
	unsigned char* m_pData;

	/** 影像数据长度 */
	unsigned long long m_pDataLength;

	///**影像压缩类型 */
	//int m_compressType;

	/** 内存维护标记 */
	bool m_bDomDataAlloced;

	/** @} */ // 属性结尾

public:

	/** @defgroup DOMGridImpl 操作（成员函数）
	* @{
	*/


	/** @name 构造与析构函数
	*  @{
	*/

	/** 构造函数
	*/
	DOMGridImpl() :m_resolvx(1.0), m_resolvy(1.0), m_east(1.0), m_west(1.0), m_north(1.0), m_south(1.0),
		m_ImageW(1), m_ImageH(1), m_ImageD(24), m_pData(NULL), m_pDataLength(0), m_bDomDataAlloced(true) {}


	/** 析构函数
	*/
	virtual ~DOMGridImpl()
	{
		if (m_pData)
		{
			delete[]m_pData;
			m_pData = NULL;
		}
	}

	/** @} */ // 构造与析构函数结尾

	/** 拷贝dom数据
	* @param [in] src	源dom数据
	* @param [out] tar	dom数据
	* @return 1 -- 拷贝成功；0 -- 拷贝失败
	*/
	static long CopyDom(DOMGridImpl& src, DOMGridImpl& tar, bool bCopyData);


	/** @name 数据操作
	*  @{
	*/

	/** 设置Dom
	* @param [in] resolvx x方向分辨率
	* @param [in] resolvy y方向分辨率
	* @param [in] east 东
	* @param [in] west 西
	* @param [in] north 北
	* @param [in] south 南
	* @param [in] ImageW  影像宽度
	* @param [in] ImageH  影像高度
	* @param [in] pData  影像数据
	* @param [in] bAlloced,是否需要自身维护内存
	* -自身维护内存
	* -自身不维护内存
	*/
	void SetDom(double resolvx, double resolvy, double east, double west, double north, double south, int ImageW, int ImageH, int ImageD, unsigned char* pData, unsigned long long datalength, bool bAlloced = true)
	{
		m_resolvx = resolvx;
		m_resolvy = resolvy;

		m_east = east;
		m_west = west;
		m_north = north;
		m_south = south;

		m_ImageW = ImageW;
		m_ImageH = ImageH;
		m_ImageD = ImageD;

		if (m_pData)
		{
			if (m_bDomDataAlloced)
			{
				delete[]m_pData;
			}
			m_pData = NULL;
		}

		m_pData = pData;
		m_bDomDataAlloced = bAlloced;

		m_pDataLength = datalength;
	}

	/** 获取Dom参数
	* @param [out] resolvx x方向分辨率
	* @param [out] resolvy y方向分辨率
	* @param [out] east 东
	* @param [out] west 西
	* @param [out] north 北
	* @param [out] south 南
	* @param [out] ImageW  影像宽度
	* @param [out] ImageH  影像高度
	* @param [out] pData  影像数据
	* @param [in] bAlloced,是否重新开辟内存
	*/
	void GetDomPara(double& resolvx, double& resolvy, double& east, double& west, double& north, double& south, int& ImageW, int& ImageH, int& ImageD, unsigned char*& pData, unsigned long long& datalength, bool bAlloced = true)
	{
		resolvx = m_resolvx;
		resolvy = m_resolvy;

		east = m_east;
		west = m_west;
		north = m_north;
		south = m_south;

		ImageW = m_ImageW;
		ImageH = m_ImageH;
		ImageD = m_ImageD;

		if (bAlloced)
		{
			if (pData)
				delete[]pData;
			pData = NULL;

			int SizeData = m_ImageW * m_ImageH * m_ImageD / 8;
			pData = new unsigned char[SizeData];
			for (int i = 0; i < SizeData; i++)
				pData[i] = m_pData[i];
		}
		else
			pData = m_pData;

		datalength = m_pDataLength;
	}


	/** 获取Dom影像分辨率
	* @param [out] resolvx DomX方向分辨率
	* @param [out] resolvy DomY方向分辨率
	*/
	void GetDomResolv(double& resolvx, double& resolvy)
	{
		resolvx = m_resolvx;
		resolvy = m_resolvy;
	}

	/** 获取Dom影像方向
	* @param [out] east 东
	* @param [out] west 西
	* @param [out] north 北
	* @param [out] south 南
	*/
	void GetDomDir(double& east, double& west, double& north, double& south)
	{
		east = m_east;
		west = m_west;
		north = m_north;
		south = m_south;
	}

	/** 获取Dom影像大小
	* @param [out] ImageW 影像宽度
	* @param [out] ImageH 影像高度
	* @param [out] ImageD 影像深度
	*/
	void GetDomImageSize(int& ImageW, int& ImageH, int& ImageD)
	{
		ImageW = m_ImageW;
		ImageH = m_ImageH;
		ImageD = m_ImageD;
	}

	/** 获取Dom影像数据
	* @return 指向Dom影像数据的指针
	*/
	unsigned char* GetDomData()
	{
		return m_pData;
	}

	unsigned long long GetImageLength()
	{
		return m_pDataLength;
	}

	/** @} */ // 数据操作组结尾



	/** @name 计算操作
	*  @{
	*/

	/** @} */ // 计算操作组结尾

	/** @} */ // 操作结尾

	/** @} */ // 模块结尾


};
end_gdb_namespace
end_gtl_namespace



