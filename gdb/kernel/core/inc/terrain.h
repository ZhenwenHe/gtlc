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
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Buffer.h"
#include "material.h"
#include "matrix4x4.h"
#include "describablebuffer.h"
#include "connector.h"
#include "image.h"


#ifndef VERINFOLENGTH
#define VERINFOLENGTH				256				//DEM与DOM数据库版本信息长度
#endif

#ifndef MAXDEMLAYERNUMBER
#define MAXDEMLAYERNUMBER			24			//最大DEM金字塔层数
#endif

#ifndef MAXDOMLAYERNUMBER
#define MAXDOMLAYERNUMBER			24			//最大DOM金字塔层数
#endif

#ifndef MAX_NAME
#define MAX_NAME					256				//最长名称
#endif

#ifndef MAX_PATH
#define MAX_PATH					512				//最长路径
#endif

#ifndef MULTIFACTOR
#define MULTIFACTOR					100000			//坐标单位为D时的高程坐标缩放因子
#endif



begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

//释放内存
#define SAFE_DELETE(p)  { if(p) { delete[] (p);     (p)=0; }  }
#define SAFE_FREE(p)  { if(p) { free (p);     (p)=0; }  }


//分配内存
#define SAFE_NEW(p,type,num)												\
{																			\
	SAFE_DELETE( p );														\
	( p ) = new type[num];													\
}																			\

#define SAFE_ALLOC(p,type,num)											        \
{																			    \
	SAFE_FREE( p );														        \
	( p ) = static_cast<type *>(malloc((unsigned long long)sizeof(type) * num));\
}																			    \

//取整
#define GET_INT(result,divisor,dividend)					                \
{																			\
	result = (int)( ( divisor ) / dividend );				                \
	if( result >= 0 )										                \
	if( ( divisor ) / dividend - result - 1 > -0.0001)						\
	result += 1;															\
	else													                \
	if( ( divisor ) / dividend - result  + 1 < 0.0001 )						\
	result -= 1;															\
}															                \

//取除数上限
#define GET_CEIL_INT(result,divisor,dividend)				                \
{																			\
	result = (int)( ( divisor ) / dividend );				                \
	if( ( divisor )  - result * (dividend)  + dividend < 0.001 )		    \
	result --;																\
	else if( ( divisor ) - result * dividend > 0.001 )		                \
	result ++;																\
}															                \

//取除数下限
#define GET_FLOOR_INT(result,divisor,dividend)				                \
{																			\
	result = (int)( ( divisor ) / dividend );				                \
	if( ( divisor ) - result * dividend - dividend > -0.001 )		        \
	result ++;																\
	else if( ( divisor ) - result * dividend < -0.001 )	                    \
	result --;																\
}															                \


//投影参数
struct ProjectionParameter
{
	int		ellipsoid;
	int		projection;
	double	longitude;
	int		zoneWidth;
};


/**************************************************************/
/********************  DEM主要数据结构   **********************/

class DEMBlock;
typedef std::shared_ptr<DEMBlock> DEMBlockSharedPtr;

/** @defgroup terrain DEMBlock-DEM数据块 
*  @{
*/
class GV3DCORE_API DEMBlock								
{
public:
	Identifier::raw_type		blockID;		//DEM数据块ID
	Identifier::raw_type		layerID;    	//金字塔层ID
	int			row;							//数据块的行号
	int			column;						    //数据块的列号
	int			full;							//是否包含填充的无效值 0:无填充，全部为有效值; 1:全部填充无效值; 2:部分填充

	Envelope2d	scope;							//当前块在X、Y方向的范围
	int			pointNumber;					//当前块的格网点数
	float		maxZ;							//当前块在Z方向的最大值
	float		minZ;							//当前块在Z方向的最小值
	int			valid;							//数据块的有效性0：无效；1：有效
	float	   *rawData;						//当前块高程数据

	DEMBlock();
	void clear();
	~DEMBlock();
public:
	/** 从Buffer中读取信息填充本几何对象
     * @param  [in,out] buf Buffer & ，缓冲区对象引用 
     * @return 成功返回true，否则返回false
    */
    virtual bool readBuffer( Buffer & buf) ;
	/** 将本类的信息写入Buffer中 
     * @param  [in,out] buf Buffer & ，缓冲区对象引用
     * @return 成功返回true，否则返回false
    */
    virtual bool writeBuffer(Buffer& buf) ;
};
/** @} */ 

class DEMDataInfo;
typedef std::shared_ptr<DEMDataInfo> DEMDataInfoSharedPtr;
/** @addtogroup terrain DEMDataInfo-DEM数据描述信息 
*  @{
*/
class GV3DCORE_API DEMDataInfo		
{
public:
	std::string			version;	            //DEM数据库标志
	int					rectified;				//数据库原点是否调整标志。当递交DEM的原点与任意给定范围的原点没有相差CELL的整数倍时，给定范围需要调整
	double				xDistance;				//原始数据在X方向上的宽度
	double				yDistance;				//原始数据在Y方向上的宽度
	double				cellSize;				//原始数据格网间距
	char				coordinateUnit[1];		//原始数据坐标单位
	float				elevationOffset;		//原始数据高程偏移量
	double				xOverlayDistance;		//原始数据在X方向上的重叠
	double				yOverlayDistance; 		//原始数据在Y方向上的重叠
	int					scale;					//原始数据比例尺		
	float				zMax;					//数据库最大高程
	float				zMin;					//数据库最小高程

	unsigned char				byteNumber;				//4字节为FLOAT，2字节为SHORT INT
	unsigned char				zoomFactor;				//2字节时高程放大因子
	unsigned char				blockFlagNumber;		//块标志数量 字节1：块是否有效 字节2：高程放大因子 字节3：保留 字节4：保留
	unsigned char				haveMMZ;				//最大最小高程数量，其值为0或者2
	unsigned char				compress;				//0x00 = 未压缩	0x01 = LZW压缩

	double				        threshold;				//原始数据在重叠范围内的精度阈值
	ProjectionParameter		    projectionParameter;	//投影参数

	long						invalidData;			//无效高程值

	char				        pyramid; 				//是否创建了金字塔层，已创建设为1,否则设为-1，默认值-1
	char						preprocess; 			//是否进行了预处理，已经预处理设为1，否则为-1，默认值-1
	char						domResolution; 			//对应DOM数据库原始分辨率，默认值-1
	char				        reserved[8]; 			//保留

	DEMDataInfo();
	bool operator == ( DEMDataInfo &other );
	bool operator != ( DEMDataInfo &other );
public:
	/** 从Buffer中读取信息填充本几何对象
     * @param  [in,out] buf Buffer & ，缓冲区对象引用 
     * @return 成功返回true，否则返回false
    */
    virtual bool readBuffer( Buffer & buf) ;
	/** 将本类的信息写入Buffer中 
     * @param  [in,out] buf Buffer & ，缓冲区对象引用
     * @return 成功返回true，否则返回false
    */
    virtual bool writeBuffer(Buffer& buf) ;
};
/** @} */ 

class DEMLayerInfo ;
typedef std::shared_ptr<DEMLayerInfo> DEMLayerInfoSharedPtr;
/** @addtogroup terrain DEMLayerInfo-DEM金字塔层描述信息 
*  @{
*/
class GV3DCORE_API DEMLayerInfo	
{
public:
	Identifier::raw_type				layerID;			//该DEM层的ID
	Identifier::raw_type				demdbID;			//该层所属的DEM数据库ID

	std::string			layerName;	                        //该层名称
	double				blockSize;			                //该DEM层数据块格网间距,也即每个数据块的大小
	Envelope2d			layerScope;				            //DEM层的范围
	Envelope2d			dataScope;				            //该DEM层拥有数据的范围
	Envelope2d          domSubmitScope;                     //递交dom数据的边界，避免DEMBlock有重合的情况
	int					rowsPreblock;				        //每个数据块的行数
	int					colsPreblock;				        //每个数据块的列数
	int					xBlocksNumber;				        //该层X方向的数据块数
	int					yBlocksNumber;				        //该层Y方向的数据块数
	int					scale;						        //金字塔层之间的分辨率的比率；默认为2

	 
	DEMLayerInfo();
	DEMLayerInfo( Identifier::raw_type lyrID,Identifier::raw_type dbID, char* name, Envelope2d lyrScope, Envelope2d dataScope, int xBlocks, int yBlocks, int rows, int cols, double res,
		int nScale,int boriDataLayer, int btopLayer );
	DEMLayerInfo operator=(const DEMLayerInfo &other );
	bool operator == ( DEMLayerInfo &other );
	bool operator != ( DEMLayerInfo &other );
	void clear();
public:
	/** 根据图层范围和块的大小，计算图层可以在X和Y方向上分别划分成多少个块
     * @param  [in,out] colBlockNumber 图层可以在X方向上分别划分成多少个块 
	 * @param  [in,out] rowBlockNumber 图层可以在Y方向上分别划分成多少个块 
     * @return  
    */
	virtual void calculateBlockNumbers(long & rowBlockNumber, long &colBlockNumber);
	/** 计算块的范围
     * @param  [in] block_row_in_layer 数据块在图层行索引 
	 * @param  [in] block_col_in_layer 数据块在图层列索引 
     * @return  
    */
	virtual Envelope2d calcualteBlockEnvelope(unsigned int block_row_in_layer,unsigned int block_col_in_layer);
	/** 计算图层中单元网格的间距
     * @param  [in] 无
     * @return  图层中单元网格的间距
    */
	virtual double calcualteCellSize();
	/** 根据图层范围和块的大小，计算块的所有节点坐标
     * @param  [in] block_row_in_layer 数据块在图层行索引 
	 * @param  [in] block_col_in_layer 数据块在图层列索引 
     * @return  
    */
	virtual void calculateBlockVertices(unsigned int block_row_in_layer,unsigned int block_col_in_layer,std::vector<Vertex3d> & vv);
	/** 创建一个新的数据块
     * @param  [in] block_row_in_layer 数据块在图层行索引 
	 * @param  [in] block_col_in_layer 数据块在图层列索引 
	 * @param  [in] vv 数据块对应的节点坐标数组 
     * @return  
    */
	virtual DEMBlockSharedPtr createDEMBlock(unsigned int block_row_in_layer,unsigned int block_col_in_layer,std::vector<Vertex3d> & vv);

	/** 创建一个新的数据块
     * @param  [in] block_row_in_layer 数据块在图层行索引 
	 * @param  [in] block_col_in_layer 数据块在图层列索引 
	 * @param  [in] rawData 数据块对应的节点坐标数组 
     * @return  
    */
	virtual DEMBlockSharedPtr createDEMBlock(unsigned int block_row_in_layer,unsigned int block_col_in_layer,const float * rawData,const unsigned int pointNumber);

public:
	/** 从Buffer中读取信息填充本几何对象
     * @param  [in,out] buf Buffer & ，缓冲区对象引用 
     * @return 成功返回true，否则返回false
    */
    virtual bool readBuffer( Buffer & buf) ;
	/** 将本类的信息写入Buffer中 
     * @param  [in,out] buf Buffer & ，缓冲区对象引用
     * @return 成功返回true，否则返回false
    */
    virtual bool writeBuffer(Buffer& buf) ;
};
/** @} */ 

/** @addtogroup terrain BLOCK_FILL_FLAG-地形数据块填充无效值的标识 
*  @{
*/
enum BLOCK_FILL_FLAG{
	NO_FILL_INVALIDDATA,
	ALL_FILL_INVALIDDATA,
	PART_FILL_INVALIDDATA
};
/** @} */ 

struct DEMInfo;
typedef std::shared_ptr<DEMInfo> DEMInfoSharedPtr;
/** @addtogroup terrain DEMInfo-DEM描述信息 
*  @{
*/
struct DEMInfo	
{
	double	x0;				//最小值x
	double	y0;				//最小值y
	double	cellSize;		//格网间距
	double	minZ;			//最小z值
	double	maxZ;			//最大z值
	int		columnNumber;	//列数
	int		rowNumber;		//行数

	DEMInfo();
	DEMInfo(double x0,double y0, double	cellSize, double minZ,double maxZ, int column,int row);
public:
	/** 从Buffer中读取信息填充本几何对象
     * @param  [in,out] buf Buffer & ，缓冲区对象引用 
     * @return 成功返回true，否则返回false
    */
    virtual bool readBuffer( Buffer & buf) ;
	/** 将本类的信息写入Buffer中 
     * @param  [in,out] buf Buffer & ，缓冲区对象引用
     * @return 成功返回true，否则返回false
    */
    virtual bool writeBuffer(Buffer& buf) ;
}; 
/** @} */ 

/**************************************************************************/
/********************  DOM数据库主要数据结构 ******************************/
struct DOMInfo;
typedef std::shared_ptr<DOMInfo> DOMInfoSharedPtr;
/** @addtogroup terrain DOMInfo-定义原始影像的空间信息结构
*  @{
*/
struct DOMInfo	
{
	double	x0;					//原始影像数据的左下角X坐标
	double	y0;					//原始影像数据的左下角Y坐标
	double	xResolution;		//原始影像数据的X方向上的分辨率
	double	yResolution;		//原始影像数据的Y方向上的分辨率
	int		xPixelsNumber;		//原始影像数据的X方向上的像素数
	int		yPixelsNumber; 		//原始影像数据的Y方向上的像素数
	int		bitsCount; 		    //原始影像数据像素位数

	DOMInfo();
	DOMInfo( double x0, double y0, double resX, double resY, int nXPixelsNum, int nYPixelsNum, int nBitsNum );
public:
	/** 从Buffer中读取信息填充本几何对象
     * @param  [in,out] buf Buffer & ，缓冲区对象引用 
     * @return 成功返回true，否则返回false
    */
    virtual bool readBuffer( Buffer & buf) ;
	/** 将本类的信息写入Buffer中 
     * @param  [in,out] buf Buffer & ，缓冲区对象引用
     * @return 成功返回true，否则返回false
    */
    virtual bool writeBuffer(Buffer& buf) ;
};
/** @} */ 

class DOMBlock;
typedef std::shared_ptr<DOMBlock> DOMBlockSharedPtr;
//DOM数据块信息结构体
/** @addtogroup terrain DOMBlock-DOM块结构
*  @{
*/
class GV3DCORE_API DOMBlock
{
public:
	Identifier::raw_type		    blockID;				//DOM数据块ID
	Identifier::raw_type			layerID;				//金字塔层ID
	int			row;					                    //数据块的行号
	int			column;					                    //数据块的列号
	int			full;					                    //是否包含填充的无效值,1表示没有，0表示有无效值
	int			xPixelsNumber;				                //数据块X方向像素数
	int			yPixelsNumber;				                //数据块Y方向像素数

	Envelope2d	  scope;					               //当前块在X、Y方向的范围
	unsigned char*imageData;				               //影像数据块
	int			  valid;					               //影像数据块的有效性
	int			  imageLength;

	DOMBlock();
	void clear();
	DOMBlock operator=(const DOMBlock &other);
	virtual ~DOMBlock();

public:
	inline int computeBitsPrepixel();		
	inline unsigned char* getPixel(int nCol,int nRow);
	inline unsigned char * getRowPixels(int nRow);
public:
	/** 从Buffer中读取信息填充本几何对象
     * @param  [in,out] buf Buffer & ，缓冲区对象引用 
     * @return 成功返回true，否则返回false
    */
    virtual bool readBuffer( Buffer & buf) ;
	/** 将本类的信息写入Buffer中 
     * @param  [in,out] buf Buffer & ，缓冲区对象引用
     * @return 成功返回true，否则返回false
    */
    virtual bool writeBuffer(Buffer& buf) ;
	/** 静态函数，将DOM的块转换成影像数据
     * @param  [in,out] p DOMBlockSharedPtr & ，DOMBlock对象引用
     * @return 成功返回ImageSharedPtr，否则返回ImageSharedPtr的空指针
    */
	static  ImageSharedPtr DOMBlock2Image(DOMBlockSharedPtr & p);
	//只能生成一个没有blockID号layid等信息的block
	/** 静态函数，将影像转换成DOMBlock
     * @param  [in,out] image ImageSharedPtr & ，影像对象引用
     * @return 成功返回转换后的DOMBlockSharedPtr，否则返回空的DOMBlockSharedPtr
    */
	static DOMBlockSharedPtr Image2DOMBlock(ImageSharedPtr &image);
	/** 静态函数，将两个DOMBlock合并成一个DOMBlock
     * @param  [in] p1 DOMBlockSharedPtr &DOM块对象引用
	 * @param  [in] p2 DOMBlockSharedPtr &块对象引用
     * @return 成功返回合并后的DOMBlockSharedPtr，否则返回空的DOMBlockSharedPtr
    */
	static DOMBlockSharedPtr merge(const DOMBlockSharedPtr & p1, const DOMBlockSharedPtr & p2);
	/** 静态函数，将DOMBlock中含有的的影像数据进行重采样
     * @param  [in] p1 DOMBlockSharedPtr &DOM块对象引用
	 * @param  [in] newx 整数 重采样后的宽度
	 * @param  [in] newy 整数 重采样后的高度
     * @return 成功返回重采样后的DOMBlockSharedPtr，否则返回空的DOMBlockSharedPtr
    */
	static DOMBlockSharedPtr resample(const DOMBlockSharedPtr & p1,int newx, int newy);
}; 
/** @} */ 

/** 得到DOMBlock中的某个像素值的地址
* @param  [in] nCol 整数 像素所在的行
* @param  [in] nRow 整数 像素所在的列
* @return 成功返回该像素的地址，否则返回
*/
inline unsigned char* DOMBlock::getPixel(int nCol,int nRow){
	if((nCol<xPixelsNumber)&&(nRow<yPixelsNumber)&&(imageData)) 
		return imageData + imageLength/(xPixelsNumber*yPixelsNumber)*(nRow*xPixelsNumber+nCol) ;
	else 
		return 0;
}
/** 得到DOMBlock中的某行像素的地址
* @param  [in] nRow 整数 像素所在的列
* @return 成功返回该行像素的起始地址，否则返回0
*/
inline unsigned char * DOMBlock::getRowPixels(int nRow){
	//return imageData+(nRow*xPixelsNumber)*(imageLength/(xPixelsNumber*yPixelsNumber);
	return imageData+(nRow*imageLength/yPixelsNumber);
}
/** 计算DOMBlock的每个像素的字节数
* @return 成功返回该行像素的字节数，否则返回
*/
inline int DOMBlock::computeBitsPrepixel(){return imageLength/(xPixelsNumber*yPixelsNumber)*8;}

class DOMLayerInfo;
typedef std::shared_ptr<DOMLayerInfo> DOMLayerInfoSharedPtr;
//DOM数据块信息结构体
/** @addtogroup terrain DOMLayerInfo-DOM数据库层结构信息
*  @{
*/
class GV3DCORE_API DOMLayerInfo			//DOM数据库层结构信息
{
public:
	Identifier::raw_type  layerID;			    //DOM层的ID
	Identifier::raw_type  demLayerID;			//DOM层对应的DEM层ID
	Identifier::raw_type  domdbID;			    //DOM层所属的DOM数据库ID
	Envelope2d			  layerScope;			//DOM层的范围
	Envelope2d			  dataScope;			//DOM层的范围

	std::string			  layerName;	        //该层名称
	double				  layerResolution; 		//DOM层分辨率
	int					  xPixelsNumber;		//DOM层影像块在X方向上的像素数
	int					  yPixelsNumber;		//DOM层影像块在Y方向上的像素数
	int					  xBlocksNumber;		//DOM层的X方向的块数量
	int					  yBlocksNumber;		//DOM层的Y方向的块数量
	int					  scale;				//层与层之间分辨率的比率

	DOMLayerInfo();
	DOMLayerInfo( Identifier::raw_type lyrID,Identifier::raw_type demLyrID,Identifier::raw_type dbID,char* name, Envelope2d lyrScope, Envelope2d dataScope, int xBlocks, int yBlocks, int xPixels, int yPixels, double res,
		int nScale, int boriDataLayer, int btopLayer );
	DOMLayerInfo operator=(const DOMLayerInfo &other);
	bool operator == ( DOMLayerInfo &other );
	bool operator != ( DOMLayerInfo &other );
	void clear();
public:
	/** 计算块的范围
     * @param  [in] block_row_in_layer 数据块在图层行索引 
	 * @param  [in] block_col_in_layer 数据块在图层列索引 
     * @return  
    */
	virtual Envelope2d calcualteBlockEnvelope(unsigned int block_row_in_layer,unsigned int block_col_in_layer);
	/** 从Buffer中读取信息填充本几何对象
     * @param  [in,out] buf Buffer & ，缓冲区对象引用 
     * @return 成功返回true，否则返回false
    */
    virtual bool readBuffer( Buffer & buf) ;
	/** 将本类的信息写入Buffer中 
     * @param  [in,out] buf Buffer & ，缓冲区对象引用
     * @return 成功返回true，否则返回false
    */
    virtual bool writeBuffer(Buffer& buf) ;
	/**
	 * @param  [in] row，col数据库中的行列
	 * @param  [in，out] blkID，输入，输出数据所在的块号
	 * @param  [in,out] blkID，输入，输出数据所在块的行列号
	*/
	//注意：此函数必须在每块block的xPixelNumbers和yPixelNumbers确定正确时才能用
	virtual void calculate(unsigned long long  row,unsigned long long col, Identifier::raw_type &blkID,int &blockRow,int &blockCol);
	/**
	 * @param  [in] blkID，输入，输出数据所在的块号
	 * @param  [in] blkID，输入，输出数据所在块的行列号
	 * @param  [in,out] row，col数据库中的行列
	*/
	//注意：此函数必须在每块block的xPixelNumbers和yPixelNumbers确定正确时才能用
	virtual void calculate(Identifier::raw_type blkID,int blockRow,int blockCol,unsigned long long  &row,unsigned long long &col);
	/**
	 * @param  [in]col,row输入层所在的行列号
	 * @param  [in，out]*wx,*wy,输入、输出世界坐标坐标值
	*/
	virtual void IP2WP(unsigned long long  col, unsigned long long row, double * wx, double * wy);
	/**
	 * @param  [in]*wx,*wy,输入世界坐标坐标值
	 * @param  [in，out]col,row输入、输出层所在的行列号
	*/
	virtual void WP2IP(double   wx, double   wy,unsigned long long *col, unsigned long long *row);
}; 
/** @} */ 
class DOMDataInfo;
typedef std::shared_ptr<DOMDataInfo> DOMDataInfoSharedPtr;
//DOM数据块信息结构体
/** @addtogroup terrain DOMDataInfo-原始DOM数据信息
*  @{
*/

class GV3DCORE_API DOMDataInfo				
{
public:
	std::string			version;		             //DOM数据库标志
	double				xDistance;					//原始数据在X方向上的宽度
	double				yDistance;					//原始数据在Y方向上的宽度
	double				resolution;					//原始数据分辨率
	int					bitsCount;					//原始数据像素位数	
	char				coordinateUnit[1];			//原始数据坐标单位
	double				xOverlayDistance;			//原始数据在X方向上的重叠
	double				yOverlayDistance;			//原始数据在Y方向上的重叠
	int					scale;						//原始数据比例尺		

	int					xOverlayNumber;				//块间X方向象素重叠数量
	int					yOverlayNumber;				//块间Y方向象素重叠数量
	unsigned char		compressFormat;				//0x00: 未压缩；0x01：小波压缩；0X02：JPEG压缩
	int					compressQuilty;				//小波压缩指压缩比；JPEG压缩指压缩质量1-100

	int					demLayersNumber;			//相关DEM数据库层数量
	double				originDEMCellSize;			//原始DEM格网间距
	unsigned char		paletteReady;				//是否生成调色板
	int					indexedLayersNumber;		//索引影象层数量
	int					grayLayersNumber;			//灰度影象层数量

	unsigned char				userDefBits;		//用户递交第一幅原始影像位数
	unsigned char				cvs8To24;			//是否将8位以24位转换保存 0 = 无转换 1 = 转换
	PALETTEENTRY 		        palette[256];		//原始影像数据的调色板

	unsigned char				blockFlagNumber;	//块标志数量
	unsigned char				power2Blk;			//在递交数据时，是否以原始影像的分辨率保存块数据。0 = 原始影像分辨率 1 = 以2的幂像素数保存块数据

	unsigned char				xSOPixelNumber;		//原始数据块的X方向的重叠像素数。当块不为2的幂时，此值需要计算，当块为2的幂时，此值等于用户设定的重叠像素数
	unsigned char				ySOPixelNumber;			

	char				reserved1;					//保留
	char				reserved2;					//保留
	char				reserved3;					//保留
	char				reserved4;					//保留

	DOMDataInfo();
	bool operator == ( DOMDataInfo &other );
	bool operator != ( DOMDataInfo &other );
public:
	/** 从Buffer中读取信息填充本几何对象
     * @param  [in,out] buf Buffer & ，缓冲区对象引用 
     * @return 成功返回true，否则返回false
    */
    virtual bool readBuffer( Buffer & buf) ;
	/** 将本类的信息写入Buffer中 
     * @param  [in,out] buf Buffer & ，缓冲区对象引用
     * @return 成功返回true，否则返回false
    */
    virtual bool writeBuffer(Buffer& buf) ;
};
/** @} */ 

class  DEMDBInfo;
typedef std::shared_ptr<DEMDBInfo> DEMDBInfoSharedPtr;
/** @addtogroup terrain DEMDBInfo-DEM数据库信息
*  @{
*/

class GV3DCORE_API  DEMDBInfo		
{
public:
	Identifier::raw_type		demdbID;                //DEM数据库ID
	std::string		            demdbName;				//DEM数据库名称
	std::string		            demdbPath;				//DEM数据库路径

	Envelope2d	    demdbScope;							//DEM数据库范围

	double		lowestResolution;							//DEM数据库的最粗糙分辨率
	double		highestResolution;							//DEM数据库的最精细分辨率

	DEMDataInfoSharedPtr		demDataInfo;						//DEM数据描述信息	
	std::vector<DEMLayerInfoSharedPtr>	demLayerInfos;	//DEM数据库的层信息

	Identifier::raw_type		srefID;								//DEM数据库的空间参考ID
	Identifier::raw_type		createTime;							//DEM数据库创建时间
	Identifier::raw_type		updateTime;							//DEM数据库创建时间

	DEMDBInfo();
	bool operator == ( DEMDBInfo &other );
	bool operator != ( DEMDBInfo &other );
	DEMDBInfo operator=(const DEMDBInfo &other);
	void clear();
public:
	/** 从Buffer中读取信息填充本几何对象
     * @param  [in,out] buf Buffer & ，缓冲区对象引用 
     * @return 成功返回true，否则返回false
    */
    virtual bool readBuffer( Buffer & buf) ;
	/** 将本类的信息写入Buffer中 
     * @param  [in,out] buf Buffer & ，缓冲区对象引用
     * @return 成功返回true，否则返回false
    */
    virtual bool writeBuffer(Buffer& buf) ;
}; 
/** @} */ 

class DOMDBInfo;
typedef std::shared_ptr<DOMDBInfo> DOMDBInfoSharedPtr;
/** @addtogroup terrain DOMDBInfo-DOM数据库信息　
*  @{
*/

class GV3DCORE_API DOMDBInfo		
{
public:
	Identifier::raw_type			domdbID;						//DOM数据库ID
	Identifier::raw_type			demdbID;						//DOM数据库对应的DEM数据库ID
	std::string			domdbName;			//DOM数据库名称
	std::string			domdbPath;			//DOM数据库路径

	Envelope2d		domdbScope;						//DOM数据库范围
	//int				layerNum;						//DOM数据库的金字塔层数量
	double			lowestResolution;						//DOM数据库的最粗糙分辨率
	double			highestResolution;						//DOM数据库的最精细分辨率

	DOMDataInfoSharedPtr	    domDataInfo;					//DOM数据描述信息	
	std::vector<DOMLayerInfoSharedPtr>	domLayerInfos;	//DEM数据库的层信息


	Identifier::raw_type			srefID;							//DOM数据库的空间参考ID
	Identifier::raw_type			createTime;						//DOM数据库创建时间
	Identifier::raw_type			updateTime;						//DOM数据库创建时间

	DOMDBInfo();
	DOMDBInfo operator=(const DOMDBInfo &other);
	void clear();
public:
	/** 从Buffer中读取信息填充本几何对象
     * @param  [in,out] buf Buffer & ，缓冲区对象引用 
     * @return 成功返回true，否则返回false
    */
    virtual bool readBuffer( Buffer & buf) ;
	/** 将本类的信息写入Buffer中 
     * @param  [in,out] buf Buffer & ，缓冲区对象引用
     * @return 成功返回true，否则返回false
    */
    virtual bool writeBuffer(Buffer& buf) ;
}; 
/** @} */ 
class SourceDataInfo;
typedef std::shared_ptr<SourceDataInfo> SourceDataInfoSharedPtr;
/** @addtogroup terrain SourceDataInfo-原始文件入库的描述信息　
*  @{
*/

class GV3DCORE_API SourceDataInfo
{
public:
	Identifier::raw_type			dbID;					//该原始数据入库的数据库ID
	int			dataType;				//该原始数据的类型，0:dem; 1:dom
	std::string		srcName;		//该原始数据的名称
	std::string		srcPath;		//该原始数据的路径
	double		srcCellsize;			//该原始数据的分辨率
	Envelope2d	srcScope;				//该原始数据范围
	Identifier::raw_type		srcCreatetime;			//该原始数据的入库时间

	SourceDataInfo();
	SourceDataInfo operator=(const SourceDataInfo &other);
public:
	/** 从Buffer中读取信息填充本几何对象
     * @param  [in,out] buf Buffer & ，缓冲区对象引用 
     * @return 成功返回true，否则返回false
    */
    virtual bool readBuffer( Buffer & buf) ;
	/** 将本类的信息写入Buffer中 
     * @param  [in,out] buf Buffer & ，缓冲区对象引用
     * @return 成功返回true，否则返回false
    */
    virtual bool writeBuffer(Buffer& buf) ;
};
/** @} */ 
typedef DEMBlock* DEMBlockPtr;
typedef DOMBlock* DOMBlockPtr;

typedef DEMDBInfo* DEMDBInfoPtr;
typedef DOMDBInfo* DOMDBInfoPtr;

//////////////////////////////////////////////////////////
class DEMGrid;
typedef std::shared_ptr<DEMGrid> DEMGridSharedPtr;
/** @addtogroup terrain DEMGrid-DEM格网结构　
*  @{
*/

class GV3DCORE_API DEMGrid{
public:
	static DEMGridSharedPtr create();
	static double getDefaultInvalidValue() ;
public:
	struct DEM{//GTL-DEM格式，兼容CNSDTF-DEM
		std::string _DataMark;//--------GTL-DEM格式；基本部分，不可缺省。
		std::string _Version;//--------该空间数据交换格式的版本号,如1.0。基本部分，不可缺省。		
		double      _Alpha;//--------方向角。基本部分，不可缺省。
		std::string _Unit;//--------坐标单位,K表示公里,M表示米,D表示以度为单位的经纬度,S表示以度分秒表示的经纬度(此时坐标格式为DDDMMSS.SSSS, DDD为度, MM为分, SS.SSSS为秒)。基本部分，不可缺省。
		double      _Compress;//--------压缩方法。0表示不压缩,1表示游程编码。基本部分，不可缺省。
		double      _Xo;//--------左下角原点X坐标。基本部分，不可缺省。
		double      _Yo;//--------左下角原点Y坐标。基本部分，不可缺省。
		double      _DX;//--------X方向的间距。基本部分，不可缺省。
		double      _DY;//--------Y方向的间距。基本部分，不可缺省。
		long        _Row;//--------行数。基本部分，不可缺省。
		long        _Col;//--------列数。基本部分，不可缺省。
		std::string _ValueType;//--------数据值的类型，如Integer，可选，默认为Double
		double      _HZoom;//--------高程放大倍率。基本部分，不可缺省。设置高程的放大倍率,使高程数据可以整数存贮,如高程精度精确到厘米,高程的放大倍率为100。如果为0表示不起作用。
		double      _MinV;//--------可选部分，最小值。
		double      _MaxV;//--------可选部分，最大值。

		double      _Nodata;//------可选部分，无值标识

		double*     _Data;//高程值数组，从左下角开始存储
	};
	union HValue{
		double _dblVal;
		float  _fltVal;
		int    _intVal;
	};
public:
	static void fillDEMGrid(DEM & _dem, DEMGridSharedPtr & grid);
	static void initial(DEM & _dem);
	static void initial(DEM & _dem, long rows, long cols);
	static void copy(DEM & _dem, const DEM & _d);
	static void getXYZ(const DEM & _dem, long r, long c, double & x, double & y, double & z);
	static void getXYZ(const DEM & _dem, long r, long c, double *xyz);
	static void getXYZ(const DEM & _dem, double *xyz);
	static void uninitial(DEM & _dem);
	/*
	NSDTF-DEM格式说明：
	DataMark--------中国地球空间数据交换格式-格网数据交换格式(CNSDTF-RAS或CNSDTF-DEM)的标志。基本部分，不可缺省。
	Version--------该空间数据交换格式的版本号,如1.0。基本部分，不可缺省。
	Alpha--------方向角。基本部分，不可缺省。
	Unit--------坐标单位,K表示公里,M表示米,D表示以度为单位的经纬度,S表示以度分秒表示的经纬度(此时坐标格式为DDDMMSS.SSSS, DDD为度, MM为分, SS.SSSS为秒)。基本部分，不可缺省。
	Compress--------压缩方法。0表示不压缩,1表示游程编码。基本部分，不可缺省。
	Xo--------左上角原点X坐标。基本部分，不可缺省。
	Yo--------左上角原点Y坐标。基本部分，不可缺省。
	DX--------X方向的间距。基本部分，不可缺省。
	DY--------Y方向的间距。基本部分，不可缺省。
	Row--------行数。基本部分，不可缺省。
	Col--------列数。基本部分，不可缺省。
	HZoom--------高程放大倍率。基本部分，不可缺省。设置高程的放大倍率,使高程数据可以整数存贮,如高程精度精确到厘米,高程的放大倍率为100。如果不是DEM则HZoom为1。
	MinV--------可选部分，最小值。
	MaxV--------可选部分，最大值。
	*/
	static bool readCNSDTF(std::string filename, DEM & _dem);
	static bool writeCNSDTF(const DEM & _dem, std::string filename);
	/*ArcGIS中GRID转换文本文件格式说明： 字串5

	ncols         319----------------数据列数
	nrows         198----------------数据行数
	xllcorner     131985----------------数据左下角的X值
	yllcorner     19020----------------数据左下角的Y值
	cellsize      5----------------数据分辨率（栅格单元的宽高）
	NODATA_value -99999----------------无值数据标志*/
	static bool readASC(std::string filename, DEM & _dem);
	static bool writeASC(const DEM & _dem, std::string filename);
public:
	/** 从Buffer中读取信息填充本几何对象
     * @param  [in,out] buf Buffer & ，缓冲区对象引用 
     * @return 成功返回true，否则返回false
    */
    virtual bool readBuffer( Buffer & buf)=0 ;
	/** 将本类的信息写入Buffer中 
     * @param  [in,out] buf Buffer & ，缓冲区对象引用
     * @return 成功返回true，否则返回false
    */
    virtual bool writeBuffer(Buffer& buf) =0;

	/** 拷贝dem数据
	* @param [in] src	源dem数据
	* @param [in] bCopyData	是否拷贝数据
	* @return 1 -- 拷贝成功；0 -- 拷贝失败
    */
	virtual bool copy(const DEMGridSharedPtr & src, bool bCopyData)=0;

	
	/** 获取dem的三维外包盒
	* @param 
	* @return dem的三维外包盒
    */
	virtual Envelope3d getEnvelope()=0;

	/** 设置alfa
	* @param [in] dAlfa	数据
    */
	virtual void setAlpha(double dAlfa)=0;

	/** 设置Belta
	* @param [in] dBelta	数据
    */
	virtual void setBelta(double dBelta)=0;

    /** 获取alfa
	* @return Alfa
    */
	virtual double getAlpha() const=0;

    /** 获取belta
	* @return Belta
    */
	virtual double getBelta() const=0;

	/** 设置DEM左下角原点x,y坐标
	* @param [in] dx x坐标
	* @param [in] dy y坐标
    */
	virtual void setOrigin(double dx,double dy)=0;

	/** 获取DEM左下角原点x,y坐标
	* @param [out] dx DEM左下角原点x坐标
	* @param [out] dy DEM左下角原点y坐标
    */
	virtual void getOrigin(double& dx,double& dy)=0;

    /** 设置DEM高程范围
	* @param [in] fminz 最小高程值
	* @param [in] fmaxz 最大高程值
    */
	virtual void setHeightRange(float fminz,float fmaxz)=0;

    /** 获取DEM高程范围
	* @param [out] fminz DEM最小高程值
	* @param [out] fmaxz DEM最大高程值
    */
	virtual void getHeightRange(float& fminz,float& fmaxz)=0;

    /** 设置DEM行数
	* @param [in] nRow dem行数
    */
	virtual void setRows(int nRow)=0;
	 /** 设置DEM列数
	* @param [in] nCol dem列数
    */
	virtual void setCols(int nCol)=0;

     /** 获取DEM行数
	* @param 
	* @return  dem行数
    */
	virtual int getRows()const =0;
	 /** 获取DEM列数
	* @param 
	* @return  dem列数
    */
	virtual int getCols()const =0;
    /** 设置DEM坐标系类型
	* @param [in] bflag,坐标系类型,分为以下几种:
	* -1为大地坐标
	* -0为经纬度坐标系
    */
	virtual void setCoordinateSystem(int bFlag = 1)=0;
    /** 获取DEM坐标系类型
	* @return 坐标系类型,分为以下几种:
	* -1为大地坐标
	* -0为经纬度坐标系
    */
	virtual int getCoordinateSystem()=0;

    /** 设置DEM单位类型
	* @param [in] bflag,坐标单位类型,分为以下几种:
	* -1为米
	* -0为毫米
    */
	virtual void setUnit(int bFlag = 1)=0;
    /** 获取DEM坐标单位类型
	* @return 坐标系类型,分为以下几种:
	* -1为米
	* -0为毫米
    */
	virtual int getUnit()=0;
    /** 设置DEM格网间距
	* @param [in] dSize,格网间距
    */
	virtual void setCellSize(double dSize)=0;
    /** 获取DEM格网间距
	* @return double类型格网间距
    */
	virtual double getCellSize()=0;

    /** 设置DEM高程值缩放比例
	* @param [in] lScale,高程值缩放比例
    */
	virtual void setScale(long lScale)=0;

    /** 获取DEM高程值缩放比例
	* @return 高程值缩放比例
    */
	virtual long getScale()=0;

    /** 设置DEM每个格网点的高程数据
	* @param [in] pData,指向float类型的一串数据的指针
    * @param [in] bAlloced,是否需要自身维护内存
    * -自身维护内存
	* -自身不维护内存
    */
	virtual void setHeightData(float *pData, bool bAlloced = true)=0;
    /** 获取DEM每个格网点的高程数据
	* @return 指向dem每个格网点高程数据的指针
    */
	virtual float* getHeightData()=0;

    /** 设置DEM每个格网点的颜色值
	* @param [in] pData,指向unsigned long类型的一串数据的指针
    * @param [in] bAlloced,是否需要自身维护内存
    * -自身维护内存
	* -自身不维护内存
    */
	virtual void setVextexColors(std::vector<Color4f> & colors)=0;
    /** 获取DEM每个格网点的颜色值
	* @return 指向dem每个格网点颜色数据的指针
    */
	virtual void getVextexColors(std::vector<Color4f> & colors)=0;

    /** 设置DEM每个格网顶点的纹理坐标
	* @param [in] pData,指向Vertex2f类型的一串数据的指针
    * @param [in] bAlloced,是否需要自身维护内存
    * -自身维护内存
	* -自身不维护内存
    */
	virtual void setVertexTextureCoordinates(std::vector<Vertex2d> & VexTexCoor)=0;

	/** 获取DEM每个格网顶点的纹理坐标
	* @return 指向dem每个格网顶点纹理坐标数据的指针
    */
	virtual void getVertexTextureCoordinates(std::vector<Vertex2d> & VexTexCoor)=0;

	/** 设置DEM无效高程值
	* @param [in] pData,指向long类型的数据,数据为无效高程值的大小
    */
	virtual void setInvalidValue(long lInvalidata)=0;
	/** 获取DEM无效高程值
	* @return DEM无效高程值
    */
	virtual long getInvalidValue()=0;


	/** 在当前DEM上内插出传入点的高程
	* @param [in] vVer	传入点坐标
    */
	virtual double interpolate(const double x, const double y)=0;
	virtual int interpolate(Vertex3d & v)=0;

	/** 在当前DEM上内插出传入点集的所有高程
	* 功能：内插给定点的高程值.
	* 参数：PointIn为给定的点，vVertices为给定的点集，其中x,y应当由调用者赋值，而z则是内插的高程值.
	* 注意：传进来的坐标一定是大地坐标，绝对的，不是相对的（减DEM原点的）
	*      如果给定的点平面位置不在DEM范围内，则内插的高程值将是无效值NODATA。
	* @param [in] vVertices	传入点集
    */
	virtual int interpolate(std::vector<Vertex3d>& vVertices)=0;

	/** 得到当前DEM高程最大的点
	* @return 当前DEM高程最大的点
    */
	virtual Vertex3d getMaxPoint() const=0;
 
	/** 得到当前DEM高程最小的点
	* @return 当前DEM高程最小的点
    */
	virtual Vertex3d getMinPoint() const=0;

	/** 对当前DEM上的所有点的高程进行缩放
	* @param [in] dScale 缩放比例
    */
	virtual void scale(double dScale)=0;

	/** 得到当前DEM指定多边形范围内高程最大的点
	* @param [in] pPolygon	指定多边形
	* @return 当前DEM指定多边形范围内高程最大的点
    */
	virtual Vertex3d getMaxPointInPolygon(std::vector<Vertex3d>& pPolygon) const=0;

 
	/** 得到当前DEM指定多边形范围内高程最小的点
	* @param [in] pPolygon	指定多边形
	* @return 当前DEM指定多边形范围内高程最小的点
    */
	virtual Vertex3d getMinPointInPolygon(std::vector<Vertex3d>&pPolygon) const=0;


	/** 对当前DEM按照指定多边形范围进行裁剪
	* @param [in] pPolygon	指定多边形
	* @return 1 -- 裁剪成功； 0 -- 裁剪失败
    */
	virtual int clip(std::vector<Vertex3d>&pPolygon)=0;

	/** 抽取一定范围的DEM
	* @param [in] nRow 行号
	* @param [in] nCol 列号
    */
	virtual void extract(int beg_nRow, int beg_nCol,int end_nRow,int end_nCol,DEMGridSharedPtr& extractGrid)=0;

 
	/** 得到DEM边界
	* @param [in] pPolygon 多边形对象指针
	* @return DEM边界多边形
    */
	virtual void getDEMBound(std::vector<Vertex3d>&pPolygon)=0;

	/** 根据传入的旋转矩阵，旋转当前DEM
	* @param [in] m4x4 已知的旋转矩阵
	* @return 
	*/
	virtual void transform(Matrix4x4& m4x4)=0;
};
/** @} */ 
//////////////////////////////////////////////////////////
class  DOMGrid;
typedef std::shared_ptr<DOMGrid> DOMGridSharedPtr;
/** @addtogroup terrain DOMGrid-DOM格网结构　
*  @{
*/
class GV3DCORE_API DOMGrid{
public:
	static DOMGridSharedPtr create();
	static DOMGridSharedPtr create(string szfiledom);
public:
	/** 从Buffer中读取信息填充本几何对象
     * @param  [in,out] buf Buffer & ，缓冲区对象引用 
     * @return 成功返回true，否则返回false
    */
    virtual bool readBuffer( Buffer & buf)=0 ;
	/** 将本类的信息写入Buffer中 
     * @param  [in,out] buf Buffer & ，缓冲区对象引用
     * @return 成功返回true，否则返回false
    */
    virtual bool writeBuffer(Buffer& buf) =0;

	/** 拷贝dom数据
	* @param [in] src	源dom数据
	* @param [out] tar	dom数据
	* @return 1 -- 拷贝成功；0 -- 拷贝失败
    */
	virtual bool copy(const DOMGridSharedPtr & src, bool bCopyData)=0;

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
	virtual void setParameters( double resolvx, double resolvy, double east, double west, double north, double south, 
		int ImageW, int ImageH, int ImageD, unsigned char * pData, 
		unsigned long long datalength, bool bAlloced = true )=0;

	/** 设置Dom
	
	* @param [in] east 东
	* @param [in] west 西
	* @param [in] north 北
	* @param [in] south 南
	* @param [in] image  
    */
	virtual void setParameters(double east, double west, double north, double south, const ImageSharedPtr & p )=0;

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
	virtual void getParameters( double& resolvx, double& resolvy, double& east, double& west, double& north, double& south,
		int& ImageW, int& ImageH, int& ImageD, unsigned char *& pData, 
		unsigned long long & datalength, bool bAlloced = true )=0;

    /** 获取Dom影像分辨率
	* @param [out] resolvx DomX方向分辨率
	* @param [out] resolvy DomY方向分辨率
    */
	virtual void getResolution( double& resolvx, double& resolvy)=0;
   
    /** 获取Dom影像方向
	* @param [out] east 东
	* @param [out] west 西
	* @param [out] north 北
	* @param [out] south 南
	*/
	virtual void getDirection(double& east, double& west, double& north, double& south)=0;
    /** 获取Dom影像大小
	* @param [out] ImageW 影像宽度
	* @param [out] ImageH 影像高度
	* @param [out] ImageD 影像深度
    */
	virtual void getImageSize( int& ImageW, int& ImageH, int& ImageD )=0;

    
    /** 获取Dom影像数据
	* @return 指向Dom影像数据的指针
    */
	virtual unsigned char * getImageData()=0;
	 /** 获取Dom影像数据的长度
	* @return Dom影像数据的长度
    */
	virtual unsigned long long  getImageLength()=0;
	 /** 获取Dom影像数据每个像素的字节数
	* @return Dom影像数据每个像素的字节数
    */
	virtual unsigned int getBitsPerPixel() const=0;
	 /** 获取Dom影像数据的某个像素值
	 * @param [in] ix  int 像素所在行
	 * @param [in] iy  int 像素所在列
	* @return Dom影像数据的某个像素值
    */
	virtual unsigned char * getPixel(int ix,int iy)=0;
	 /** 获取Dom影像数据的某个像素值
	 * @param [in] ix  double 像素所在x坐标
	 * @param [in] iy  double 像素所在y坐标
	* @return Dom影像数据的某个像素地址
    */
	virtual unsigned char * getPixel(double x, double y)=0;
	/** Dom影像像素坐标转为世界坐标
	* @param [in] ix  double 像素所在x坐标
	* @param [in] iy  double 像素所在y坐标 
	* @return Dom影像数据的某个像素地址
    */
	virtual void IP2WP(int x, int y, double * wx, double * wy)=0;

	virtual void WP2IP( double   wx, double   wy,int *ix, int *iy)=0;

	virtual DOMBlockSharedPtr clip(const Envelope2d & rect)=0;
	virtual DOMBlockSharedPtr clip(int begX,int xNumbers,int begY,int yNumbers)=0;

	virtual unsigned long long  calculateXPixelNmuber (double length) =0;
	virtual unsigned long long  calculateYPixelNmuber (double length) =0;

	virtual void clip(DOMLayerInfoSharedPtr lay,std::vector<DOMBlockSharedPtr> & blocks)=0;

	virtual unsigned char* getLayerPixel(DOMLayerInfoSharedPtr lay ,int row, int col)=0;
	
};
/** @} */ 

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
