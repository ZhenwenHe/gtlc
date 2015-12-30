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
#include "buffer.h"
#include "material.h"
#include "matrix4x4.h"
#include "describablebuffer.h"
#include "connector.h"
#include "image.h"


#ifndef VERINFOLENGTH
#define VERINFOLENGTH				256				//DEM��DOM���ݿ�汾��Ϣ����
#endif

#ifndef MAXDEMLAYERNUMBER
#define MAXDEMLAYERNUMBER			24			//���DEM����������
#endif

#ifndef MAXDOMLAYERNUMBER
#define MAXDOMLAYERNUMBER			24			//���DOM����������
#endif

#ifndef MAX_NAME
#define MAX_NAME					256				//�����
#endif

#ifndef MAX_PATH
#define MAX_PATH					512				//�·��
#endif

#ifndef MULTIFACTOR
#define MULTIFACTOR					100000			//���굥λΪDʱ�ĸ߳�������������
#endif



begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

//�ͷ��ڴ�
#define SAFE_DELETE(p)  { if(p) { delete[] (p);     (p)=0; }  }
#define SAFE_FREE(p)  { if(p) { free (p);     (p)=0; }  }


//�����ڴ�
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

//ȡ��
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

//ȡ��������
#define GET_CEIL_INT(result,divisor,dividend)				                \
{																			\
	result = (int)( ( divisor ) / dividend );				                \
	if( ( divisor )  - result * (dividend)  + dividend < 0.001 )		    \
	result --;																\
	else if( ( divisor ) - result * dividend > 0.001 )		                \
	result ++;																\
}															                \

//ȡ��������
#define GET_FLOOR_INT(result,divisor,dividend)				                \
{																			\
	result = (int)( ( divisor ) / dividend );				                \
	if( ( divisor ) - result * dividend - dividend > -0.001 )		        \
	result ++;																\
	else if( ( divisor ) - result * dividend < -0.001 )	                    \
	result --;																\
}															                \


//ͶӰ����
struct ProjectionParameter
{
	int		ellipsoid;
	int		projection;
	double	longitude;
	int		zoneWidth;
};


/**************************************************************/
/********************  DEM��Ҫ���ݽṹ   **********************/

class DEMBlock;
typedef std::shared_ptr<DEMBlock> DEMBlockSharedPtr;

/** @defgroup terrain DEMBlock-DEM���ݿ� 
*  @{
*/
class CORE_API DEMBlock								
{
public:
	Identifier::raw_type		blockID;		//DEM���ݿ�ID
	Identifier::raw_type		layerID;    	//��������ID
	int			row;							//���ݿ���к�
	int			column;						    //���ݿ���к�
	int			full;							//�Ƿ����������Чֵ 0:����䣬ȫ��Ϊ��Чֵ; 1:ȫ�������Чֵ; 2:�������

	Envelope2d	scope;							//��ǰ����X��Y����ķ�Χ
	int			pointNumber;					//��ǰ��ĸ�������
	float		maxZ;							//��ǰ����Z��������ֵ
	float		minZ;							//��ǰ����Z�������Сֵ
	int			valid;							//���ݿ����Ч��0����Ч��1����Ч
	float	   *rawData;						//��ǰ��߳�����

	DEMBlock();
	void clear();
	~DEMBlock();
public:
	/** ��Buffer�ж�ȡ��Ϣ��䱾���ζ���
     * @param  [in,out] buf Buffer & ���������������� 
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool readBuffer( Buffer & buf) ;
	/** ���������Ϣд��Buffer�� 
     * @param  [in,out] buf Buffer & ����������������
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool writeBuffer(Buffer& buf) ;
};
/** @} */ 

class DEMDataInfo;
typedef std::shared_ptr<DEMDataInfo> DEMDataInfoSharedPtr;
/** @addtogroup terrain DEMDataInfo-DEM����������Ϣ 
*  @{
*/
class CORE_API DEMDataInfo		
{
public:
	std::string			version;	            //DEM���ݿ��־
	int					rectified;				//���ݿ�ԭ���Ƿ������־�����ݽ�DEM��ԭ�������������Χ��ԭ��û�����CELL��������ʱ��������Χ��Ҫ����
	double				xDistance;				//ԭʼ������X�����ϵĿ��
	double				yDistance;				//ԭʼ������Y�����ϵĿ��
	double				cellSize;				//ԭʼ���ݸ������
	char				coordinateUnit[1];		//ԭʼ�������굥λ
	float				elevationOffset;		//ԭʼ���ݸ߳�ƫ����
	double				xOverlayDistance;		//ԭʼ������X�����ϵ��ص�
	double				yOverlayDistance; 		//ԭʼ������Y�����ϵ��ص�
	int					scale;					//ԭʼ���ݱ�����		
	float				zMax;					//���ݿ����߳�
	float				zMin;					//���ݿ���С�߳�

	unsigned char				byteNumber;				//4�ֽ�ΪFLOAT��2�ֽ�ΪSHORT INT
	unsigned char				zoomFactor;				//2�ֽ�ʱ�̷߳Ŵ�����
	unsigned char				blockFlagNumber;		//���־���� �ֽ�1�����Ƿ���Ч �ֽ�2���̷߳Ŵ����� �ֽ�3������ �ֽ�4������
	unsigned char				haveMMZ;				//�����С�߳���������ֵΪ0����2
	unsigned char				compress;				//0x00 = δѹ��	0x01 = LZWѹ��

	double				        threshold;				//ԭʼ�������ص���Χ�ڵľ�����ֵ
	ProjectionParameter		    projectionParameter;	//ͶӰ����

	long						invalidData;			//��Ч�߳�ֵ

	char				        pyramid; 				//�Ƿ񴴽��˽������㣬�Ѵ�����Ϊ1,������Ϊ-1��Ĭ��ֵ-1
	char						preprocess; 			//�Ƿ������Ԥ�����Ѿ�Ԥ������Ϊ1������Ϊ-1��Ĭ��ֵ-1
	char						domResolution; 			//��ӦDOM���ݿ�ԭʼ�ֱ��ʣ�Ĭ��ֵ-1
	char				        reserved[8]; 			//����

	DEMDataInfo();
	bool operator == ( DEMDataInfo &other );
	bool operator != ( DEMDataInfo &other );
public:
	/** ��Buffer�ж�ȡ��Ϣ��䱾���ζ���
     * @param  [in,out] buf Buffer & ���������������� 
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool readBuffer( Buffer & buf) ;
	/** ���������Ϣд��Buffer�� 
     * @param  [in,out] buf Buffer & ����������������
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool writeBuffer(Buffer& buf) ;
};
/** @} */ 

class DEMLayerInfo ;
typedef std::shared_ptr<DEMLayerInfo> DEMLayerInfoSharedPtr;
/** @addtogroup terrain DEMLayerInfo-DEM��������������Ϣ 
*  @{
*/
class CORE_API DEMLayerInfo	
{
public:
	Identifier::raw_type				layerID;			//��DEM���ID
	Identifier::raw_type				demdbID;			//�ò�������DEM���ݿ�ID

	std::string			layerName;	                        //�ò�����
	double				blockSize;			                //��DEM�����ݿ�������,Ҳ��ÿ�����ݿ�Ĵ�С
	Envelope2d			layerScope;				            //DEM��ķ�Χ
	Envelope2d			dataScope;				            //��DEM��ӵ�����ݵķ�Χ
	Envelope2d          domSubmitScope;                     //�ݽ�dom���ݵı߽磬����DEMBlock���غϵ����
	int					rowsPreblock;				        //ÿ�����ݿ������
	int					colsPreblock;				        //ÿ�����ݿ������
	int					xBlocksNumber;				        //�ò�X��������ݿ���
	int					yBlocksNumber;				        //�ò�Y��������ݿ���
	int					scale;						        //��������֮��ķֱ��ʵı��ʣ�Ĭ��Ϊ2

	 
	DEMLayerInfo();
	DEMLayerInfo( Identifier::raw_type lyrID,Identifier::raw_type dbID, char* name, Envelope2d lyrScope, Envelope2d dataScope, int xBlocks, int yBlocks, int rows, int cols, double res,
		int nScale,int boriDataLayer, int btopLayer );
	DEMLayerInfo operator=(const DEMLayerInfo &other );
	bool operator == ( DEMLayerInfo &other );
	bool operator != ( DEMLayerInfo &other );
	void clear();
public:
	/** ����ͼ�㷶Χ�Ϳ�Ĵ�С������ͼ�������X��Y�����Ϸֱ𻮷ֳɶ��ٸ���
     * @param  [in,out] colBlockNumber ͼ�������X�����Ϸֱ𻮷ֳɶ��ٸ��� 
	 * @param  [in,out] rowBlockNumber ͼ�������Y�����Ϸֱ𻮷ֳɶ��ٸ��� 
     * @return  
    */
	virtual void calculateBlockNumbers(long & rowBlockNumber, long &colBlockNumber);
	/** �����ķ�Χ
     * @param  [in] block_row_in_layer ���ݿ���ͼ�������� 
	 * @param  [in] block_col_in_layer ���ݿ���ͼ�������� 
     * @return  
    */
	virtual Envelope2d calcualteBlockEnvelope(unsigned int block_row_in_layer,unsigned int block_col_in_layer);
	/** ����ͼ���е�Ԫ����ļ��
     * @param  [in] ��
     * @return  ͼ���е�Ԫ����ļ��
    */
	virtual double calcualteCellSize();
	/** ����ͼ�㷶Χ�Ϳ�Ĵ�С�����������нڵ�����
     * @param  [in] block_row_in_layer ���ݿ���ͼ�������� 
	 * @param  [in] block_col_in_layer ���ݿ���ͼ�������� 
     * @return  
    */
	virtual void calculateBlockVertices(unsigned int block_row_in_layer,unsigned int block_col_in_layer,std::vector<Vertex3d> & vv);
	/** ����һ���µ����ݿ�
     * @param  [in] block_row_in_layer ���ݿ���ͼ�������� 
	 * @param  [in] block_col_in_layer ���ݿ���ͼ�������� 
	 * @param  [in] vv ���ݿ��Ӧ�Ľڵ��������� 
     * @return  
    */
	virtual DEMBlockSharedPtr createDEMBlock(unsigned int block_row_in_layer,unsigned int block_col_in_layer,std::vector<Vertex3d> & vv);

	/** ����һ���µ����ݿ�
     * @param  [in] block_row_in_layer ���ݿ���ͼ�������� 
	 * @param  [in] block_col_in_layer ���ݿ���ͼ�������� 
	 * @param  [in] rawData ���ݿ��Ӧ�Ľڵ��������� 
     * @return  
    */
	virtual DEMBlockSharedPtr createDEMBlock(unsigned int block_row_in_layer,unsigned int block_col_in_layer,const float * rawData,const unsigned int pointNumber);

public:
	/** ��Buffer�ж�ȡ��Ϣ��䱾���ζ���
     * @param  [in,out] buf Buffer & ���������������� 
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool readBuffer( Buffer & buf) ;
	/** ���������Ϣд��Buffer�� 
     * @param  [in,out] buf Buffer & ����������������
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool writeBuffer(Buffer& buf) ;
};
/** @} */ 

/** @addtogroup terrain BLOCK_FILL_FLAG-�������ݿ������Чֵ�ı�ʶ 
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
/** @addtogroup terrain DEMInfo-DEM������Ϣ 
*  @{
*/
struct DEMInfo	
{
	double	x0;				//��Сֵx
	double	y0;				//��Сֵy
	double	cellSize;		//�������
	double	minZ;			//��Сzֵ
	double	maxZ;			//���zֵ
	int		columnNumber;	//����
	int		rowNumber;		//����

	DEMInfo();
	DEMInfo(double x0,double y0, double	cellSize, double minZ,double maxZ, int column,int row);
public:
	/** ��Buffer�ж�ȡ��Ϣ��䱾���ζ���
     * @param  [in,out] buf Buffer & ���������������� 
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool readBuffer( Buffer & buf) ;
	/** ���������Ϣд��Buffer�� 
     * @param  [in,out] buf Buffer & ����������������
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool writeBuffer(Buffer& buf) ;
}; 
/** @} */ 

/**************************************************************************/
/********************  DOM���ݿ���Ҫ���ݽṹ ******************************/
struct DOMInfo;
typedef std::shared_ptr<DOMInfo> DOMInfoSharedPtr;
/** @addtogroup terrain DOMInfo-����ԭʼӰ��Ŀռ���Ϣ�ṹ
*  @{
*/
struct DOMInfo	
{
	double	x0;					//ԭʼӰ�����ݵ����½�X����
	double	y0;					//ԭʼӰ�����ݵ����½�Y����
	double	xResolution;		//ԭʼӰ�����ݵ�X�����ϵķֱ���
	double	yResolution;		//ԭʼӰ�����ݵ�Y�����ϵķֱ���
	int		xPixelsNumber;		//ԭʼӰ�����ݵ�X�����ϵ�������
	int		yPixelsNumber; 		//ԭʼӰ�����ݵ�Y�����ϵ�������
	int		bitsCount; 		    //ԭʼӰ����������λ��

	DOMInfo();
	DOMInfo( double x0, double y0, double resX, double resY, int nXPixelsNum, int nYPixelsNum, int nBitsNum );
public:
	/** ��Buffer�ж�ȡ��Ϣ��䱾���ζ���
     * @param  [in,out] buf Buffer & ���������������� 
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool readBuffer( Buffer & buf) ;
	/** ���������Ϣд��Buffer�� 
     * @param  [in,out] buf Buffer & ����������������
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool writeBuffer(Buffer& buf) ;
};
/** @} */ 

class DOMBlock;
typedef std::shared_ptr<DOMBlock> DOMBlockSharedPtr;
//DOM���ݿ���Ϣ�ṹ��
/** @addtogroup terrain DOMBlock-DOM��ṹ
*  @{
*/
class CORE_API DOMBlock
{
public:
	Identifier::raw_type		    blockID;				//DOM���ݿ�ID
	Identifier::raw_type			layerID;				//��������ID
	int			row;					                    //���ݿ���к�
	int			column;					                    //���ݿ���к�
	int			full;					                    //�Ƿ����������Чֵ,1��ʾû�У�0��ʾ����Чֵ
	int			xPixelsNumber;				                //���ݿ�X����������
	int			yPixelsNumber;				                //���ݿ�Y����������

	Envelope2d	  scope;					               //��ǰ����X��Y����ķ�Χ
	unsigned char*imageData;				               //Ӱ�����ݿ�
	int			  valid;					               //Ӱ�����ݿ����Ч��
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
	/** ��Buffer�ж�ȡ��Ϣ��䱾���ζ���
     * @param  [in,out] buf Buffer & ���������������� 
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool readBuffer( Buffer & buf) ;
	/** ���������Ϣд��Buffer�� 
     * @param  [in,out] buf Buffer & ����������������
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool writeBuffer(Buffer& buf) ;
	/** ��̬��������DOM�Ŀ�ת����Ӱ������
     * @param  [in,out] p DOMBlockSharedPtr & ��DOMBlock��������
     * @return �ɹ�����ImageSharedPtr�����򷵻�ImageSharedPtr�Ŀ�ָ��
    */
	static  ImageSharedPtr DOMBlock2Image(DOMBlockSharedPtr & p);
	//ֻ������һ��û��blockID��layid����Ϣ��block
	/** ��̬��������Ӱ��ת����DOMBlock
     * @param  [in,out] image ImageSharedPtr & ��Ӱ���������
     * @return �ɹ�����ת�����DOMBlockSharedPtr�����򷵻ؿյ�DOMBlockSharedPtr
    */
	static DOMBlockSharedPtr Image2DOMBlock(ImageSharedPtr &image);
	/** ��̬������������DOMBlock�ϲ���һ��DOMBlock
     * @param  [in] p1 DOMBlockSharedPtr &DOM���������
	 * @param  [in] p2 DOMBlockSharedPtr &���������
     * @return �ɹ����غϲ����DOMBlockSharedPtr�����򷵻ؿյ�DOMBlockSharedPtr
    */
	static DOMBlockSharedPtr merge(const DOMBlockSharedPtr & p1, const DOMBlockSharedPtr & p2);
	/** ��̬��������DOMBlock�к��еĵ�Ӱ�����ݽ����ز���
     * @param  [in] p1 DOMBlockSharedPtr &DOM���������
	 * @param  [in] newx ���� �ز�����Ŀ��
	 * @param  [in] newy ���� �ز�����ĸ߶�
     * @return �ɹ������ز������DOMBlockSharedPtr�����򷵻ؿյ�DOMBlockSharedPtr
    */
	static DOMBlockSharedPtr resample(const DOMBlockSharedPtr & p1,int newx, int newy);
}; 
/** @} */ 

/** �õ�DOMBlock�е�ĳ������ֵ�ĵ�ַ
* @param  [in] nCol ���� �������ڵ���
* @param  [in] nRow ���� �������ڵ���
* @return �ɹ����ظ����صĵ�ַ�����򷵻�
*/
inline unsigned char* DOMBlock::getPixel(int nCol,int nRow){
	if((nCol<xPixelsNumber)&&(nRow<yPixelsNumber)&&(imageData)) 
		return imageData + imageLength/(xPixelsNumber*yPixelsNumber)*(nRow*xPixelsNumber+nCol) ;
	else 
		return 0;
}
/** �õ�DOMBlock�е�ĳ�����صĵ�ַ
* @param  [in] nRow ���� �������ڵ���
* @return �ɹ����ظ������ص���ʼ��ַ�����򷵻�0
*/
inline unsigned char * DOMBlock::getRowPixels(int nRow){
	//return imageData+(nRow*xPixelsNumber)*(imageLength/(xPixelsNumber*yPixelsNumber);
	return imageData+(nRow*imageLength/yPixelsNumber);
}
/** ����DOMBlock��ÿ�����ص��ֽ���
* @return �ɹ����ظ������ص��ֽ��������򷵻�
*/
inline int DOMBlock::computeBitsPrepixel(){return imageLength/(xPixelsNumber*yPixelsNumber)*8;}

class DOMLayerInfo;
typedef std::shared_ptr<DOMLayerInfo> DOMLayerInfoSharedPtr;
//DOM���ݿ���Ϣ�ṹ��
/** @addtogroup terrain DOMLayerInfo-DOM���ݿ��ṹ��Ϣ
*  @{
*/
class CORE_API DOMLayerInfo			//DOM���ݿ��ṹ��Ϣ
{
public:
	Identifier::raw_type  layerID;			    //DOM���ID
	Identifier::raw_type  demLayerID;			//DOM���Ӧ��DEM��ID
	Identifier::raw_type  domdbID;			    //DOM��������DOM���ݿ�ID
	Envelope2d			  layerScope;			//DOM��ķ�Χ
	Envelope2d			  dataScope;			//DOM��ķ�Χ

	std::string			  layerName;	        //�ò�����
	double				  layerResolution; 		//DOM��ֱ���
	int					  xPixelsNumber;		//DOM��Ӱ�����X�����ϵ�������
	int					  yPixelsNumber;		//DOM��Ӱ�����Y�����ϵ�������
	int					  xBlocksNumber;		//DOM���X����Ŀ�����
	int					  yBlocksNumber;		//DOM���Y����Ŀ�����
	int					  scale;				//�����֮��ֱ��ʵı���

	DOMLayerInfo();
	DOMLayerInfo( Identifier::raw_type lyrID,Identifier::raw_type demLyrID,Identifier::raw_type dbID,char* name, Envelope2d lyrScope, Envelope2d dataScope, int xBlocks, int yBlocks, int xPixels, int yPixels, double res,
		int nScale, int boriDataLayer, int btopLayer );
	DOMLayerInfo operator=(const DOMLayerInfo &other);
	bool operator == ( DOMLayerInfo &other );
	bool operator != ( DOMLayerInfo &other );
	void clear();
public:
	/** �����ķ�Χ
     * @param  [in] block_row_in_layer ���ݿ���ͼ�������� 
	 * @param  [in] block_col_in_layer ���ݿ���ͼ�������� 
     * @return  
    */
	virtual Envelope2d calcualteBlockEnvelope(unsigned int block_row_in_layer,unsigned int block_col_in_layer);
	/** ��Buffer�ж�ȡ��Ϣ��䱾���ζ���
     * @param  [in,out] buf Buffer & ���������������� 
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool readBuffer( Buffer & buf) ;
	/** ���������Ϣд��Buffer�� 
     * @param  [in,out] buf Buffer & ����������������
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool writeBuffer(Buffer& buf) ;
	/**
	 * @param  [in] row��col���ݿ��е�����
	 * @param  [in��out] blkID�����룬����������ڵĿ��
	 * @param  [in,out] blkID�����룬����������ڿ�����к�
	*/
	//ע�⣺�˺���������ÿ��block��xPixelNumbers��yPixelNumbersȷ����ȷʱ������
	virtual void calculate(unsigned long long  row,unsigned long long col, Identifier::raw_type &blkID,int &blockRow,int &blockCol);
	/**
	 * @param  [in] blkID�����룬����������ڵĿ��
	 * @param  [in] blkID�����룬����������ڿ�����к�
	 * @param  [in,out] row��col���ݿ��е�����
	*/
	//ע�⣺�˺���������ÿ��block��xPixelNumbers��yPixelNumbersȷ����ȷʱ������
	virtual void calculate(Identifier::raw_type blkID,int blockRow,int blockCol,unsigned long long  &row,unsigned long long &col);
	/**
	 * @param  [in]col,row��������ڵ����к�
	 * @param  [in��out]*wx,*wy,���롢���������������ֵ
	*/
	virtual void IP2WP(unsigned long long  col, unsigned long long row, double * wx, double * wy);
	/**
	 * @param  [in]*wx,*wy,����������������ֵ
	 * @param  [in��out]col,row���롢��������ڵ����к�
	*/
	virtual void WP2IP(double   wx, double   wy,unsigned long long *col, unsigned long long *row);
}; 
/** @} */ 
class DOMDataInfo;
typedef std::shared_ptr<DOMDataInfo> DOMDataInfoSharedPtr;
//DOM���ݿ���Ϣ�ṹ��
/** @addtogroup terrain DOMDataInfo-ԭʼDOM������Ϣ
*  @{
*/

class CORE_API DOMDataInfo				
{
public:
	std::string			version;		             //DOM���ݿ��־
	double				xDistance;					//ԭʼ������X�����ϵĿ��
	double				yDistance;					//ԭʼ������Y�����ϵĿ��
	double				resolution;					//ԭʼ���ݷֱ���
	int					bitsCount;					//ԭʼ��������λ��	
	char				coordinateUnit[1];			//ԭʼ�������굥λ
	double				xOverlayDistance;			//ԭʼ������X�����ϵ��ص�
	double				yOverlayDistance;			//ԭʼ������Y�����ϵ��ص�
	int					scale;						//ԭʼ���ݱ�����		

	int					xOverlayNumber;				//���X���������ص�����
	int					yOverlayNumber;				//���Y���������ص�����
	unsigned char		compressFormat;				//0x00: δѹ����0x01��С��ѹ����0X02��JPEGѹ��
	int					compressQuilty;				//С��ѹ��ָѹ���ȣ�JPEGѹ��ָѹ������1-100

	int					demLayersNumber;			//���DEM���ݿ������
	double				originDEMCellSize;			//ԭʼDEM�������
	unsigned char		paletteReady;				//�Ƿ����ɵ�ɫ��
	int					indexedLayersNumber;		//����Ӱ�������
	int					grayLayersNumber;			//�Ҷ�Ӱ�������

	unsigned char				userDefBits;		//�û��ݽ���һ��ԭʼӰ��λ��
	unsigned char				cvs8To24;			//�Ƿ�8λ��24λת������ 0 = ��ת�� 1 = ת��
	PALETTEENTRY 		        palette[256];		//ԭʼӰ�����ݵĵ�ɫ��

	unsigned char				blockFlagNumber;	//���־����
	unsigned char				power2Blk;			//�ڵݽ�����ʱ���Ƿ���ԭʼӰ��ķֱ��ʱ�������ݡ�0 = ԭʼӰ��ֱ��� 1 = ��2�������������������

	unsigned char				xSOPixelNumber;		//ԭʼ���ݿ��X������ص������������鲻Ϊ2����ʱ����ֵ��Ҫ���㣬����Ϊ2����ʱ����ֵ�����û��趨���ص�������
	unsigned char				ySOPixelNumber;			

	char				reserved1;					//����
	char				reserved2;					//����
	char				reserved3;					//����
	char				reserved4;					//����

	DOMDataInfo();
	bool operator == ( DOMDataInfo &other );
	bool operator != ( DOMDataInfo &other );
public:
	/** ��Buffer�ж�ȡ��Ϣ��䱾���ζ���
     * @param  [in,out] buf Buffer & ���������������� 
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool readBuffer( Buffer & buf) ;
	/** ���������Ϣд��Buffer�� 
     * @param  [in,out] buf Buffer & ����������������
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool writeBuffer(Buffer& buf) ;
};
/** @} */ 

class  DEMDBInfo;
typedef std::shared_ptr<DEMDBInfo> DEMDBInfoSharedPtr;
/** @addtogroup terrain DEMDBInfo-DEM���ݿ���Ϣ
*  @{
*/

class CORE_API  DEMDBInfo		
{
public:
	Identifier::raw_type		demdbID;                //DEM���ݿ�ID
	std::string		            demdbName;				//DEM���ݿ�����
	std::string		            demdbPath;				//DEM���ݿ�·��

	Envelope2d	    demdbScope;							//DEM���ݿⷶΧ

	double		lowestResolution;							//DEM���ݿ����ֲڷֱ���
	double		highestResolution;							//DEM���ݿ���ϸ�ֱ���

	DEMDataInfoSharedPtr		demDataInfo;						//DEM����������Ϣ	
	std::vector<DEMLayerInfoSharedPtr>	demLayerInfos;	//DEM���ݿ�Ĳ���Ϣ

	Identifier::raw_type		srefID;								//DEM���ݿ�Ŀռ�ο�ID
	Identifier::raw_type		createTime;							//DEM���ݿⴴ��ʱ��
	Identifier::raw_type		updateTime;							//DEM���ݿⴴ��ʱ��

	DEMDBInfo();
	bool operator == ( DEMDBInfo &other );
	bool operator != ( DEMDBInfo &other );
	DEMDBInfo operator=(const DEMDBInfo &other);
	void clear();
public:
	/** ��Buffer�ж�ȡ��Ϣ��䱾���ζ���
     * @param  [in,out] buf Buffer & ���������������� 
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool readBuffer( Buffer & buf) ;
	/** ���������Ϣд��Buffer�� 
     * @param  [in,out] buf Buffer & ����������������
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool writeBuffer(Buffer& buf) ;
}; 
/** @} */ 

class DOMDBInfo;
typedef std::shared_ptr<DOMDBInfo> DOMDBInfoSharedPtr;
/** @addtogroup terrain DOMDBInfo-DOM���ݿ���Ϣ��
*  @{
*/

class CORE_API DOMDBInfo		
{
public:
	Identifier::raw_type			domdbID;						//DOM���ݿ�ID
	Identifier::raw_type			demdbID;						//DOM���ݿ��Ӧ��DEM���ݿ�ID
	std::string			domdbName;			//DOM���ݿ�����
	std::string			domdbPath;			//DOM���ݿ�·��

	Envelope2d		domdbScope;						//DOM���ݿⷶΧ
	//int				layerNum;						//DOM���ݿ�Ľ�����������
	double			lowestResolution;						//DOM���ݿ����ֲڷֱ���
	double			highestResolution;						//DOM���ݿ���ϸ�ֱ���

	DOMDataInfoSharedPtr	    domDataInfo;					//DOM����������Ϣ	
	std::vector<DOMLayerInfoSharedPtr>	domLayerInfos;	//DEM���ݿ�Ĳ���Ϣ


	Identifier::raw_type			srefID;							//DOM���ݿ�Ŀռ�ο�ID
	Identifier::raw_type			createTime;						//DOM���ݿⴴ��ʱ��
	Identifier::raw_type			updateTime;						//DOM���ݿⴴ��ʱ��

	DOMDBInfo();
	DOMDBInfo operator=(const DOMDBInfo &other);
	void clear();
public:
	/** ��Buffer�ж�ȡ��Ϣ��䱾���ζ���
     * @param  [in,out] buf Buffer & ���������������� 
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool readBuffer( Buffer & buf) ;
	/** ���������Ϣд��Buffer�� 
     * @param  [in,out] buf Buffer & ����������������
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool writeBuffer(Buffer& buf) ;
}; 
/** @} */ 
class SourceDataInfo;
typedef std::shared_ptr<SourceDataInfo> SourceDataInfoSharedPtr;
/** @addtogroup terrain SourceDataInfo-ԭʼ�ļ�����������Ϣ��
*  @{
*/

class CORE_API SourceDataInfo
{
public:
	Identifier::raw_type			dbID;					//��ԭʼ�����������ݿ�ID
	int			dataType;				//��ԭʼ���ݵ����ͣ�0:dem; 1:dom
	std::string		srcName;		//��ԭʼ���ݵ�����
	std::string		srcPath;		//��ԭʼ���ݵ�·��
	double		srcCellsize;			//��ԭʼ���ݵķֱ���
	Envelope2d	srcScope;				//��ԭʼ���ݷ�Χ
	Identifier::raw_type		srcCreatetime;			//��ԭʼ���ݵ����ʱ��

	SourceDataInfo();
	SourceDataInfo operator=(const SourceDataInfo &other);
public:
	/** ��Buffer�ж�ȡ��Ϣ��䱾���ζ���
     * @param  [in,out] buf Buffer & ���������������� 
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool readBuffer( Buffer & buf) ;
	/** ���������Ϣд��Buffer�� 
     * @param  [in,out] buf Buffer & ����������������
     * @return �ɹ�����true�����򷵻�false
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
/** @addtogroup terrain DEMGrid-DEM�����ṹ��
*  @{
*/

class CORE_API DEMGrid{
public:
	static DEMGridSharedPtr create();
	static double getDefaultInvalidValue() ;
public:
	struct DEM{//GTL-DEM��ʽ������CNSDTF-DEM
		std::string _DataMark;//--------GTL-DEM��ʽ���������֣�����ȱʡ��
		std::string _Version;//--------�ÿռ����ݽ�����ʽ�İ汾��,��1.0���������֣�����ȱʡ��		
		double      _Alpha;//--------����ǡ��������֣�����ȱʡ��
		std::string _Unit;//--------���굥λ,K��ʾ����,M��ʾ��,D��ʾ�Զ�Ϊ��λ�ľ�γ��,S��ʾ�Զȷ����ʾ�ľ�γ��(��ʱ�����ʽΪDDDMMSS.SSSS, DDDΪ��, MMΪ��, SS.SSSSΪ��)���������֣�����ȱʡ��
		double      _Compress;//--------ѹ��������0��ʾ��ѹ��,1��ʾ�γ̱��롣�������֣�����ȱʡ��
		double      _Xo;//--------���½�ԭ��X���ꡣ�������֣�����ȱʡ��
		double      _Yo;//--------���½�ԭ��Y���ꡣ�������֣�����ȱʡ��
		double      _DX;//--------X����ļ�ࡣ�������֣�����ȱʡ��
		double      _DY;//--------Y����ļ�ࡣ�������֣�����ȱʡ��
		long        _Row;//--------�������������֣�����ȱʡ��
		long        _Col;//--------�������������֣�����ȱʡ��
		std::string _ValueType;//--------����ֵ�����ͣ���Integer����ѡ��Ĭ��ΪDouble
		double      _HZoom;//--------�̷߳Ŵ��ʡ��������֣�����ȱʡ�����ø̵߳ķŴ���,ʹ�߳����ݿ�����������,��߳̾��Ⱦ�ȷ������,�̵߳ķŴ���Ϊ100�����Ϊ0��ʾ�������á�
		double      _MinV;//--------��ѡ���֣���Сֵ��
		double      _MaxV;//--------��ѡ���֣����ֵ��

		double      _Nodata;//------��ѡ���֣���ֵ��ʶ

		double*     _Data;//�߳�ֵ���飬�����½ǿ�ʼ�洢
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
	NSDTF-DEM��ʽ˵����
	DataMark--------�й�����ռ����ݽ�����ʽ-�������ݽ�����ʽ(CNSDTF-RAS��CNSDTF-DEM)�ı�־���������֣�����ȱʡ��
	Version--------�ÿռ����ݽ�����ʽ�İ汾��,��1.0���������֣�����ȱʡ��
	Alpha--------����ǡ��������֣�����ȱʡ��
	Unit--------���굥λ,K��ʾ����,M��ʾ��,D��ʾ�Զ�Ϊ��λ�ľ�γ��,S��ʾ�Զȷ����ʾ�ľ�γ��(��ʱ�����ʽΪDDDMMSS.SSSS, DDDΪ��, MMΪ��, SS.SSSSΪ��)���������֣�����ȱʡ��
	Compress--------ѹ��������0��ʾ��ѹ��,1��ʾ�γ̱��롣�������֣�����ȱʡ��
	Xo--------���Ͻ�ԭ��X���ꡣ�������֣�����ȱʡ��
	Yo--------���Ͻ�ԭ��Y���ꡣ�������֣�����ȱʡ��
	DX--------X����ļ�ࡣ�������֣�����ȱʡ��
	DY--------Y����ļ�ࡣ�������֣�����ȱʡ��
	Row--------�������������֣�����ȱʡ��
	Col--------�������������֣�����ȱʡ��
	HZoom--------�̷߳Ŵ��ʡ��������֣�����ȱʡ�����ø̵߳ķŴ���,ʹ�߳����ݿ�����������,��߳̾��Ⱦ�ȷ������,�̵߳ķŴ���Ϊ100���������DEM��HZoomΪ1��
	MinV--------��ѡ���֣���Сֵ��
	MaxV--------��ѡ���֣����ֵ��
	*/
	static bool readCNSDTF(std::string filename, DEM & _dem);
	static bool writeCNSDTF(const DEM & _dem, std::string filename);
	/*ArcGIS��GRIDת���ı��ļ���ʽ˵���� �ִ�5

	ncols         319----------------��������
	nrows         198----------------��������
	xllcorner     131985----------------�������½ǵ�Xֵ
	yllcorner     19020----------------�������½ǵ�Yֵ
	cellsize      5----------------���ݷֱ��ʣ�դ��Ԫ�Ŀ�ߣ�
	NODATA_value -99999----------------��ֵ���ݱ�־*/
	static bool readASC(std::string filename, DEM & _dem);
	static bool writeASC(const DEM & _dem, std::string filename);
public:
	/** ��Buffer�ж�ȡ��Ϣ��䱾���ζ���
     * @param  [in,out] buf Buffer & ���������������� 
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool readBuffer( Buffer & buf)=0 ;
	/** ���������Ϣд��Buffer�� 
     * @param  [in,out] buf Buffer & ����������������
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool writeBuffer(Buffer& buf) =0;

	/** ����dem����
	* @param [in] src	Դdem����
	* @param [in] bCopyData	�Ƿ񿽱�����
	* @return 1 -- �����ɹ���0 -- ����ʧ��
    */
	virtual bool copy(const DEMGridSharedPtr & src, bool bCopyData)=0;

	
	/** ��ȡdem����ά�����
	* @param 
	* @return dem����ά�����
    */
	virtual Envelope3d getEnvelope()=0;

	/** ����alfa
	* @param [in] dAlfa	����
    */
	virtual void setAlpha(double dAlfa)=0;

	/** ����Belta
	* @param [in] dBelta	����
    */
	virtual void setBelta(double dBelta)=0;

    /** ��ȡalfa
	* @return Alfa
    */
	virtual double getAlpha() const=0;

    /** ��ȡbelta
	* @return Belta
    */
	virtual double getBelta() const=0;

	/** ����DEM���½�ԭ��x,y����
	* @param [in] dx x����
	* @param [in] dy y����
    */
	virtual void setOrigin(double dx,double dy)=0;

	/** ��ȡDEM���½�ԭ��x,y����
	* @param [out] dx DEM���½�ԭ��x����
	* @param [out] dy DEM���½�ԭ��y����
    */
	virtual void getOrigin(double& dx,double& dy)=0;

    /** ����DEM�̷߳�Χ
	* @param [in] fminz ��С�߳�ֵ
	* @param [in] fmaxz ���߳�ֵ
    */
	virtual void setHeightRange(float fminz,float fmaxz)=0;

    /** ��ȡDEM�̷߳�Χ
	* @param [out] fminz DEM��С�߳�ֵ
	* @param [out] fmaxz DEM���߳�ֵ
    */
	virtual void getHeightRange(float& fminz,float& fmaxz)=0;

    /** ����DEM����
	* @param [in] nRow dem����
    */
	virtual void setRows(int nRow)=0;
	 /** ����DEM����
	* @param [in] nCol dem����
    */
	virtual void setCols(int nCol)=0;

     /** ��ȡDEM����
	* @param 
	* @return  dem����
    */
	virtual int getRows()const =0;
	 /** ��ȡDEM����
	* @param 
	* @return  dem����
    */
	virtual int getCols()const =0;
    /** ����DEM����ϵ����
	* @param [in] bflag,����ϵ����,��Ϊ���¼���:
	* -1Ϊ�������
	* -0Ϊ��γ������ϵ
    */
	virtual void setCoordinateSystem(int bFlag = 1)=0;
    /** ��ȡDEM����ϵ����
	* @return ����ϵ����,��Ϊ���¼���:
	* -1Ϊ�������
	* -0Ϊ��γ������ϵ
    */
	virtual int getCoordinateSystem()=0;

    /** ����DEM��λ����
	* @param [in] bflag,���굥λ����,��Ϊ���¼���:
	* -1Ϊ��
	* -0Ϊ����
    */
	virtual void setUnit(int bFlag = 1)=0;
    /** ��ȡDEM���굥λ����
	* @return ����ϵ����,��Ϊ���¼���:
	* -1Ϊ��
	* -0Ϊ����
    */
	virtual int getUnit()=0;
    /** ����DEM�������
	* @param [in] dSize,�������
    */
	virtual void setCellSize(double dSize)=0;
    /** ��ȡDEM�������
	* @return double���͸������
    */
	virtual double getCellSize()=0;

    /** ����DEM�߳�ֵ���ű���
	* @param [in] lScale,�߳�ֵ���ű���
    */
	virtual void setScale(long lScale)=0;

    /** ��ȡDEM�߳�ֵ���ű���
	* @return �߳�ֵ���ű���
    */
	virtual long getScale()=0;

    /** ����DEMÿ��������ĸ߳�����
	* @param [in] pData,ָ��float���͵�һ�����ݵ�ָ��
    * @param [in] bAlloced,�Ƿ���Ҫ����ά���ڴ�
    * -����ά���ڴ�
	* -����ά���ڴ�
    */
	virtual void setHeightData(float *pData, bool bAlloced = true)=0;
    /** ��ȡDEMÿ��������ĸ߳�����
	* @return ָ��demÿ��������߳����ݵ�ָ��
    */
	virtual float* getHeightData()=0;

    /** ����DEMÿ�����������ɫֵ
	* @param [in] pData,ָ��unsigned long���͵�һ�����ݵ�ָ��
    * @param [in] bAlloced,�Ƿ���Ҫ����ά���ڴ�
    * -����ά���ڴ�
	* -����ά���ڴ�
    */
	virtual void setVextexColors(std::vector<Color4f> & colors)=0;
    /** ��ȡDEMÿ�����������ɫֵ
	* @return ָ��demÿ����������ɫ���ݵ�ָ��
    */
	virtual void getVextexColors(std::vector<Color4f> & colors)=0;

    /** ����DEMÿ�������������������
	* @param [in] pData,ָ��Vertex2f���͵�һ�����ݵ�ָ��
    * @param [in] bAlloced,�Ƿ���Ҫ����ά���ڴ�
    * -����ά���ڴ�
	* -����ά���ڴ�
    */
	virtual void setVertexTextureCoordinates(std::vector<Vertex2d> & VexTexCoor)=0;

	/** ��ȡDEMÿ�������������������
	* @return ָ��demÿ���������������������ݵ�ָ��
    */
	virtual void getVertexTextureCoordinates(std::vector<Vertex2d> & VexTexCoor)=0;

	/** ����DEM��Ч�߳�ֵ
	* @param [in] pData,ָ��long���͵�����,����Ϊ��Ч�߳�ֵ�Ĵ�С
    */
	virtual void setInvalidValue(long lInvalidata)=0;
	/** ��ȡDEM��Ч�߳�ֵ
	* @return DEM��Ч�߳�ֵ
    */
	virtual long getInvalidValue()=0;


	/** �ڵ�ǰDEM���ڲ�������ĸ߳�
	* @param [in] vVer	���������
    */
	virtual double interpolate(const double x, const double y)=0;
	virtual int interpolate(Vertex3d & v)=0;

	/** �ڵ�ǰDEM���ڲ������㼯�����и߳�
	* ���ܣ��ڲ������ĸ߳�ֵ.
	* ������PointInΪ�����ĵ㣬vVerticesΪ�����ĵ㼯������x,yӦ���ɵ����߸�ֵ����z�����ڲ�ĸ߳�ֵ.
	* ע�⣺������������һ���Ǵ�����꣬���Եģ�������Եģ���DEMԭ��ģ�
	*      ��������ĵ�ƽ��λ�ò���DEM��Χ�ڣ����ڲ�ĸ߳�ֵ������ЧֵNODATA��
	* @param [in] vVertices	����㼯
    */
	virtual int interpolate(std::vector<Vertex3d>& vVertices)=0;

	/** �õ���ǰDEM�߳����ĵ�
	* @return ��ǰDEM�߳����ĵ�
    */
	virtual Vertex3d getMaxPoint() const=0;
 
	/** �õ���ǰDEM�߳���С�ĵ�
	* @return ��ǰDEM�߳���С�ĵ�
    */
	virtual Vertex3d getMinPoint() const=0;

	/** �Ե�ǰDEM�ϵ����е�ĸ߳̽�������
	* @param [in] dScale ���ű���
    */
	virtual void scale(double dScale)=0;

	/** �õ���ǰDEMָ������η�Χ�ڸ߳����ĵ�
	* @param [in] pPolygon	ָ�������
	* @return ��ǰDEMָ������η�Χ�ڸ߳����ĵ�
    */
	virtual Vertex3d getMaxPointInPolygon(std::vector<Vertex3d>& pPolygon) const=0;

 
	/** �õ���ǰDEMָ������η�Χ�ڸ߳���С�ĵ�
	* @param [in] pPolygon	ָ�������
	* @return ��ǰDEMָ������η�Χ�ڸ߳���С�ĵ�
    */
	virtual Vertex3d getMinPointInPolygon(std::vector<Vertex3d>&pPolygon) const=0;


	/** �Ե�ǰDEM����ָ������η�Χ���вü�
	* @param [in] pPolygon	ָ�������
	* @return 1 -- �ü��ɹ��� 0 -- �ü�ʧ��
    */
	virtual int clip(std::vector<Vertex3d>&pPolygon)=0;

	/** ��ȡһ����Χ��DEM
	* @param [in] nRow �к�
	* @param [in] nCol �к�
    */
	virtual void extract(int beg_nRow, int beg_nCol,int end_nRow,int end_nCol,DEMGridSharedPtr& extractGrid)=0;

 
	/** �õ�DEM�߽�
	* @param [in] pPolygon ����ζ���ָ��
	* @return DEM�߽�����
    */
	virtual void getDEMBound(std::vector<Vertex3d>&pPolygon)=0;

	/** ���ݴ������ת������ת��ǰDEM
	* @param [in] m4x4 ��֪����ת����
	* @return 
	*/
	virtual void transform(Matrix4x4& m4x4)=0;
};
/** @} */ 
//////////////////////////////////////////////////////////
class  DOMGrid;
typedef std::shared_ptr<DOMGrid> DOMGridSharedPtr;
/** @addtogroup terrain DOMGrid-DOM�����ṹ��
*  @{
*/
class CORE_API DOMGrid{
public:
	static DOMGridSharedPtr create();
	static DOMGridSharedPtr create(string szfiledom);
public:
	/** ��Buffer�ж�ȡ��Ϣ��䱾���ζ���
     * @param  [in,out] buf Buffer & ���������������� 
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool readBuffer( Buffer & buf)=0 ;
	/** ���������Ϣд��Buffer�� 
     * @param  [in,out] buf Buffer & ����������������
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool writeBuffer(Buffer& buf) =0;

	/** ����dom����
	* @param [in] src	Դdom����
	* @param [out] tar	dom����
	* @return 1 -- �����ɹ���0 -- ����ʧ��
    */
	virtual bool copy(const DOMGridSharedPtr & src, bool bCopyData)=0;

	/** ����Dom
	* @param [in] resolvx x����ֱ���
	* @param [in] resolvy y����ֱ���
	* @param [in] east ��
	* @param [in] west ��
	* @param [in] north ��
	* @param [in] south ��
	* @param [in] ImageW  Ӱ���� 
	* @param [in] ImageH  Ӱ��߶�
	* @param [in] pData  Ӱ������
	* @param [in] bAlloced,�Ƿ���Ҫ����ά���ڴ�
	* -����ά���ڴ�
	* -����ά���ڴ�
    */
	virtual void setParameters( double resolvx, double resolvy, double east, double west, double north, double south, 
		int ImageW, int ImageH, int ImageD, unsigned char * pData, 
		unsigned long long datalength, bool bAlloced = true )=0;

	/** ����Dom
	
	* @param [in] east ��
	* @param [in] west ��
	* @param [in] north ��
	* @param [in] south ��
	* @param [in] image  
    */
	virtual void setParameters(double east, double west, double north, double south, const ImageSharedPtr & p )=0;

	/** ��ȡDom����
	* @param [out] resolvx x����ֱ���
	* @param [out] resolvy y����ֱ���
	* @param [out] east ��
	* @param [out] west ��
	* @param [out] north ��
	* @param [out] south ��
	* @param [out] ImageW  Ӱ���� 
	* @param [out] ImageH  Ӱ��߶�
	* @param [out] pData  Ӱ������
	* @param [in] bAlloced,�Ƿ����¿����ڴ�
	*/
	virtual void getParameters( double& resolvx, double& resolvy, double& east, double& west, double& north, double& south,
		int& ImageW, int& ImageH, int& ImageD, unsigned char *& pData, 
		unsigned long long & datalength, bool bAlloced = true )=0;

    /** ��ȡDomӰ��ֱ���
	* @param [out] resolvx DomX����ֱ���
	* @param [out] resolvy DomY����ֱ���
    */
	virtual void getResolution( double& resolvx, double& resolvy)=0;
   
    /** ��ȡDomӰ����
	* @param [out] east ��
	* @param [out] west ��
	* @param [out] north ��
	* @param [out] south ��
	*/
	virtual void getDirection(double& east, double& west, double& north, double& south)=0;
    /** ��ȡDomӰ���С
	* @param [out] ImageW Ӱ����
	* @param [out] ImageH Ӱ��߶�
	* @param [out] ImageD Ӱ�����
    */
	virtual void getImageSize( int& ImageW, int& ImageH, int& ImageD )=0;

    
    /** ��ȡDomӰ������
	* @return ָ��DomӰ�����ݵ�ָ��
    */
	virtual unsigned char * getImageData()=0;
	 /** ��ȡDomӰ�����ݵĳ���
	* @return DomӰ�����ݵĳ���
    */
	virtual unsigned long long  getImageLength()=0;
	 /** ��ȡDomӰ������ÿ�����ص��ֽ���
	* @return DomӰ������ÿ�����ص��ֽ���
    */
	virtual unsigned int getBitsPerPixel() const=0;
	 /** ��ȡDomӰ�����ݵ�ĳ������ֵ
	 * @param [in] ix  int ����������
	 * @param [in] iy  int ����������
	* @return DomӰ�����ݵ�ĳ������ֵ
    */
	virtual unsigned char * getPixel(int ix,int iy)=0;
	 /** ��ȡDomӰ�����ݵ�ĳ������ֵ
	 * @param [in] ix  double ��������x����
	 * @param [in] iy  double ��������y����
	* @return DomӰ�����ݵ�ĳ�����ص�ַ
    */
	virtual unsigned char * getPixel(double x, double y)=0;
	/** DomӰ����������תΪ��������
	* @param [in] ix  double ��������x����
	* @param [in] iy  double ��������y���� 
	* @return DomӰ�����ݵ�ĳ�����ص�ַ
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
