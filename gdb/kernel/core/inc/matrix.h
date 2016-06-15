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
#include <cmath>
#include "config.h"
#include "vertex3d.h"
#include "mathex.h"


begin_gtl_namespace
begin_gdb_namespace
 

/** @addtogroup math  Matrix-矩阵（基类） 
    *  @{
    */
class CORE_API Matrix 
{

protected:

	
	/** @name 属性（成员变量）
	* @{
	*/
	
	/** 矩阵数据 */
	double* m_dData;
	
	/** 矩阵维数 */
	 int  m_iDimM;

	/** @} */ // 属性结尾

public:
	
	/** @name 构造与析构
	*  @{
	*/
	/** 构造函数1：带参构造函数（初始化列表）
	*/
	Matrix(int iDim,double* const pData = NULL);
	/** 构造函数3 ：拷贝构造函数（初始化列表）
	*/
	Matrix(const Matrix& rv);

	// 析构函数
	virtual ~Matrix()
	{
		if(m_dData)
			delete m_dData;
	}

	/** @}*/  // 构造与析构组结尾

    /** @name 计算操作
    *  @{
    */

	/** 计算当前矩阵的逆矩阵
	* @param 
	* @return 
    */
	int   inverse();

	//##ModelId=49EEE8070138
	/** 计算当前矩阵的转置矩阵
	* @param 
	* @return 
    */
	void   flip();

	//##ModelId=49ECA67F00FA
	/** 设置单位矩阵
	* @param 
	* @return 
    */
	void   loadIdentity();



	/** @}*/  // 计算操作组结尾

	/** @name 数据操作
    *  @{
    */

	/** 内联函数。重载二元操作符"="，用于对当前矩阵的赋值。
	* @param [in] rhm	用于赋值的矩阵
	* @return 赋值后得到的矩阵
    */
	inline Matrix& operator=(const Matrix& rhm)
	{
		assert(m_iDimM == rhm.m_iDimM);
		for(int i=0; i<m_iDimM*m_iDimM; i++)
			m_dData[i] = rhm.m_dData[i];
		return (*this);
	}

	/** 内联函数。重载二元操作符"=="，用于判断当前矩阵和制定矩阵是否相等。
	* @param [in] rhm	用于赋值的矩阵
	* @return 1 -- 相等；0 -- 不相等
    */
	inline int operator==(const Matrix& rhm)
	{
		assert(m_iDimM == rhm.m_iDimM);
		for(int i=0; i<m_iDimM*m_iDimM; i++)
			if(fabs(m_dData[i] - rhm.m_dData[i]) > TOLERANCE)
				return 0;
		return 1;
	}

	/** 内联函数。重载二元操作符"！="，用于判断当前矩阵和制定矩阵是否不相等。
	* @param [in] rhm	用于赋值的矩阵
	* @return 1 -- 不相等；0 -- 相等
    */
	inline int operator!=(const Matrix& rhm)
	{
		return !((*this) == rhm);
	}


	/** 内联函数。重载二元操作符"^="，用于对矩阵的复合赋值（加法）
	* @param [in] rhm	当前用于复合赋值的矩阵
	* @return 
    */
	inline void operator+=(const Matrix& rhm)
	{
		assert(m_iDimM == rhm.m_iDimM);
		for(int i=0; i<m_iDimM*m_iDimM; i++)
			m_dData[i] += rhm.m_dData[i];
	}

	//##ModelId=49EEE40403C8
	/** 内联函数。重载二元操作符"+="，用于对矩阵的复合赋值（数加）
	* @param [in] rhd	当前用于复合赋值的数
	* @return 
    */
	inline void operator+=(double rhd)
	{
		for(int i=0; i<m_iDimM*m_iDimM; i++)
			m_dData[i] += rhd;
	}

	
	/** 内联函数。重载二元操作符"^="，用于对矩阵的复合赋值（减法）
	* @param [in] rhm	当前用于复合赋值的矩阵
	* @return 
    */
	inline void operator-=(const Matrix& rhm)
	{
		assert(m_iDimM == rhm.m_iDimM);
		for(int i=0; i<m_iDimM*m_iDimM; i++)
			m_dData[i] -= rhm.m_dData[i];
	}

	
	/** 内联函数。重载一元操作符"^="，用于取得矩阵的相反值。
	* @param 
	* @return 矩阵的相反值
	*/
	inline Matrix operator-() const
	{
		Matrix res(*this);
		res *= -1;
		return res;
	}
	
	/** 内联函数。重载二元操作符"^="，用于对矩阵的复合赋值（数减）
	* @param [in] rhd	当前用于复合赋值的数
	* @return 
    */
	inline void operator-=(double rhd)
	{
		for(int i=0; i<m_iDimM*m_iDimM; i++)
			m_dData[i] -= rhd;
	}

	/** 内联函数。重载二元操作符"*="，用于对矩阵的复合赋值（乘法）
	* @param [in] rhd	当前用于复合赋值的矩阵
	* @return 
    */
	void operator*=(const Matrix& rhm);
	
	/** 内联函数。重载二元操作符"^="，用于对矩阵的复合赋值（数乘）
	* @param [in] rhd	当前用于复合赋值的数
	* @return 
    */
	inline void operator*=(double rhd)
	{
		for(int i=0; i<m_iDimM*m_iDimM; i++)
			m_dData[i] *= rhd;
	}

	/** 内联函数。重载二元操作符"^="，用于对矩阵的复合赋值（数除）
	* @param [in] rhd	当前用于复合赋值的数
	* @return 
	*/
	inline void operator/=(double rhd)
	{
		if(fabs(rhd) < TOLERANCE)
			return;
		for(int i=0; i<m_iDimM*m_iDimM; i++)
			m_dData[i] /= rhd;
	}

	/** 内联函数。重载二元操作符"[]"，用于获得矩阵指定索引对应的值
	* @param [in] rhd	当前用于复合赋值的数
	* @return 
	*/
	inline double* operator[](int iIndex) const
	{
		assert(iIndex >= 0 && iIndex < m_iDimM);
		return &m_dData[iIndex*m_iDimM];
	}
	/** @}*/  // 数据操作组结尾

	/** @name 辅助操作
    *  @{
    */

	/** 设置当前矩阵为零矩阵，即所有元素值为0
	* @param 
	* @return 
    */
	void  setNull()
	{
		memset(m_dData,0,m_iDimM*m_iDimM*sizeof(double));
	}
	
	/** 判断当前矩阵是否为零矩阵
	* @param 
	* @return	1 -- 零矩阵；0 - 非零矩阵 
    */
	int  isNull()
	{
		for(int i=0; i<m_iDimM*m_iDimM; i++)
			if(fabs(m_dData[i]) > TOLERANCE)
				return 0;
		return 1;
	}

	/** 获得矩阵的维数
	* @param 
	* @return 矩阵的维数
    */
	int getDimension() const
	{
		return m_iDimM;
	}

	/** 获得位于固定行列的元素值
	* @param [in] nRow		固定行	
	* @param [in] nCol		固定列
	* @return 位于固定行列的元素值
    */
	double getData(int nRow, int nCol) const
	{
		return m_dData[nRow*m_iDimM + nCol];
	}
	/** @}*/  // 辅助操作组结尾

	/** 获得元素值
	* @return
    */
	double* getData() const
	{
		return m_dData;
	}
	/** @}*/  // 辅助操作组结尾

};
/** @}*/

/** @addtogroup math  Matrix-全局操作符号重载 
    *  @{
    */
 CORE_API const Matrix operator+(const Matrix& lhm, const Matrix& rhm);
 CORE_API const Matrix operator-(const Matrix& lhm, const Matrix& rhm);
 CORE_API const Matrix operator+(const Matrix& lhm, double rhd);
 CORE_API const Matrix operator-(const Matrix& lhm, double rhd);
 CORE_API const Matrix operator*(const Matrix& lhm, const Matrix& rhm);
 CORE_API const Matrix operator*(const Matrix& lhm, double rhd);
 CORE_API const Matrix operator/(const Matrix& lhm, double rhd);
 /** @}*/

end_gdb_namespace
end_gtl_namespace


