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
#include "vertex3f.h"
#include "vertex4d.h"
#include "matrix3x3.h"
#include "quaternion.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
class Quaternion;
class Matrix4x4;
typedef std::shared_ptr<Matrix4x4> Matrix4x4SharedPtr;


/** @addtogroup math  Matrix4x4
*  @{
*/
class GV3DCORE_API Matrix4x4 : public Matrix
{
public:

	/** @name 属性（成员变量）
	* @{
	*/

	/** 单位矩阵 */
	static const Matrix4x4 IDENTITY4X4;

	/** @} */ // 属性结尾

public:

	/** @name 构造与析构函数
	*  @{
	*/

	/** 构造函数1：带参构造函数（初始化列表）
	*/
	Matrix4x4(double* const pData) :Matrix(4, pData){}

	/** 构造函数1：带参构造函数（初始化列表）
	*/
	Matrix4x4() :Matrix(4, NULL)
	{
		operator=(IDENTITY4X4);
	}

	/** 构造函数2：拷贝构造函数（初始化列表,用另外一个4*4维矩阵来初始化）
	*/
	Matrix4x4(const Matrix4x4& rv) :Matrix(rv){}

	/** 构造函数3：带参构造函数（初始化列表,用16个double数据来初始化）
	*/
	Matrix4x4(double fEntry00, double fEntry01, double fEntry02, double fEntry03,
		double fEntry10, double fEntry11, double fEntry12, double fEntry13,
		double fEntry20, double fEntry21, double fEntry22, double fEntry23,
		double fEntry30, double fEntry31, double fEntry32, double fEntry33) :Matrix(4)
	{
		m_dData[0] = fEntry00;	m_dData[1] = fEntry01;  m_dData[2] = fEntry02;  m_dData[3] = fEntry03;
		m_dData[4] = fEntry10;	m_dData[5] = fEntry11;  m_dData[6] = fEntry12;  m_dData[7] = fEntry13;
		m_dData[8] = fEntry20;	m_dData[9] = fEntry21;  m_dData[10] = fEntry22;  m_dData[11] = fEntry23;
		m_dData[12] = fEntry30;	m_dData[13] = fEntry31;  m_dData[14] = fEntry32;  m_dData[15] = fEntry33;
	}

	/** 构造函数4：带参构造函数（初始化列表,用4个double型数据来初始化）
	*/
	Matrix4x4(double fEntry0[4], double fEntry1[4], double fEntry2[4], double fEntry3[4]) :Matrix(4)
	{
		m_dData[0] = fEntry0[0];	m_dData[1] = fEntry0[1];  m_dData[2] = fEntry0[2];  m_dData[3] = fEntry0[3];
		m_dData[4] = fEntry1[0];	m_dData[5] = fEntry1[1];  m_dData[6] = fEntry1[2];  m_dData[7] = fEntry1[3];
		m_dData[8] = fEntry2[0];	m_dData[9] = fEntry2[1];  m_dData[10] = fEntry2[2];  m_dData[11] = fEntry2[3];
		m_dData[12] = fEntry3[0];	m_dData[13] = fEntry3[1];  m_dData[14] = fEntry3[2];  m_dData[15] = fEntry3[3];
	}

	/** 构造函数5：带参构造函数（初始化列表,用3*3维矩阵来初始化）
	*/
	inline Matrix4x4(const Matrix3x3& m3x3) :Matrix(4)
	{
		operator=(IDENTITY4X4);
		operator=(m3x3);
	}

	/** @} */ // 构造与析构函数结尾

	/** @name 数据操作
	*  @{
	*/

	/** 赋值操作,将3*3维的矩阵赋值给4*4矩阵
	* @param[in] mat3 3*3维的矩阵
	* @return
	*/
	inline void operator = (const Matrix3x3& mat3)
	{
		m_dData[0] = mat3[0][0]; m_dData[1] = mat3[0][1]; m_dData[2] = mat3[0][2];
		m_dData[4] = mat3[1][0]; m_dData[5] = mat3[1][1]; m_dData[6] = mat3[1][2];
		m_dData[8] = mat3[2][0]; m_dData[9] = mat3[2][1]; m_dData[10] = mat3[2][2];
	}

	inline Matrix4x4& operator=(const Matrix4x4& rhm)
	{
		assert(m_iDimM == rhm.m_iDimM);
		for (int i = 0; i<m_iDimM*m_iDimM; i++)
			m_dData[i] = rhm.m_dData[i];
		return (*this);
	}

	/** 提取操作,将4*4维的矩阵赋值给3*3矩阵
	* @param[out] mat3 3*3维的矩阵
	* @return
	*/
	inline void extract3x3Matrix(Matrix3x3& m3x3) const
	{
		m3x3[0][0] = m_dData[0];
		m3x3[0][1] = m_dData[1];
		m3x3[0][2] = m_dData[2];
		m3x3[1][0] = m_dData[4];
		m3x3[1][1] = m_dData[5];
		m3x3[1][2] = m_dData[6];
		m3x3[2][0] = m_dData[8];
		m3x3[2][1] = m_dData[9];
		m3x3[2][2] = m_dData[10];
	}



	/** 构建转换矩阵,首先设置矩阵为单位矩阵
	* @param[out] v 3维向量
	* @return
	*/
	void buildTranslateMatrix(const Vertex3d& v);
	void buildTranslateMatrix(double dx, double dy, double dz);
	void buildTranslateMatrix(Vertex3d & d);

	/** 构建比例尺矩阵,首先设置矩阵为单位矩阵
	* @param[out] scaleVec 3维向量
	* @param[out] center 3维向量
	* @return
	*/
	void buildScaleMatrix(const Vertex3d& scaleVec, const Vertex3d& center);

	/** 构建旋转矩阵,首先设置矩阵为单位矩阵
	* @param[out] m3x3 3维向量
	* @param[out] center 3维向量
	* @return
	*/
	void buildRotateMatrix(const Matrix3x3& m3x3, const Vertex3d& center);
	void buildRotateMatrix(const Quaternion& rot, const Vertex3d& center);
	
	void buildTransformMatrix(const Vertex3d& position, const Vertex3d& scale, const Quaternion& orientation,
		const Vertex3d& scaleCenter, const Vertex3d& rotateCenter);
	/** 构建转换矩阵
	* @param[out] v 3维向量
	* @return
	*/
	inline void setTranslateVector(const Vertex3d& v)
	{
		(*this)[3][0] = v.x;
		(*this)[3][1] = v.y;
		(*this)[3][2] = v.z;
	}

	/** 获取转换矩阵
	* @return 三维向量
	*/
	inline Vertex3d getTranslateVector() const
	{
		Vertex3d v;
		v.x = (*this)[3][0];
		v.y = (*this)[3][1];
		v.z = (*this)[3][2];
		return v;
	}

	/** 构建比例尺矩阵
	* @param[out] scaleVec 3维向量
	* @param[out] center 3维向量
	* @return
	*/
	inline void setScaleVector(const Vertex3d& v)
	{
		(*this)[0][0] = v.x;
		(*this)[1][1] = v.y;
		(*this)[2][2] = v.z;
	}

	/** 获取比例尺矩阵
	* @return 三维向量
	*/
	inline Vertex3d getScaleVector() const
	{
		Vertex3d v;
		v.x = (*this)[0][0];
		v.y = (*this)[1][1];
		v.z = (*this)[2][2];
		return v;
	}

	/** 构建旋转矩阵
	* @param[out] m3x3 3维向量
	* @param[out] center 3维向量
	* @return
	*/
	inline void setRotateMatrix(const Matrix3x3& m3x3)
	{
		operator=(m3x3);
	}
	/** 设置旋转四元数
	* @param[out] rot 四元数
	* @return
	*/
	inline void setRotateQuaternion(const Quaternion& rot)
	{
		Matrix3x3 m3x3;
		rot.toRotateMatrix(m3x3);
		operator=(m3x3);
	}


	/** Check whether or not the matrix is affine matrix.
	@remarks
	An affine matrix is a 4x4 matrix with row 3 equal to (0, 0, 0, 1),
	e.g. no projective coefficients.
	*/
	inline int isAffineValid(void) const //xx_0121
	{
		//	return (*this)[3][0] == 0 && (*this)[3][1] == 0 && (*this)[3][2] == 0 && (*this)[3][3] == 1;
		return (fabs((*this)[0][3] - 0.0) < TOLERANCE) &&
			(fabs((*this)[1][3] - 0.0) < TOLERANCE) &&
			(fabs((*this)[2][3] - 0.0) < TOLERANCE) &&
			(fabs((*this)[3][3] - 1.0) < TOLERANCE);
	}

	/** Returns the inverse of the affine matrix.
	@note
	The matrix must be an affine matrix. @see Matrix4::isAffine.
	*/
	Matrix4x4 inverseAffine(void) const;

	/** 3-D Vector transformation specially for affine matrix.
	@remarks
	Transforms the given 3-D vector by the matrix, projecting the
	result back into <i>w</i> = 1.
	@note
	The matrix must be an affine matrix. @see Matrix4::isAffine.
	*/
	inline void transformAffine(Vertex3d& vv) const
	{


		Vertex3d v = vv;
		vv.x = (*this)[0][0] * v.x + (*this)[1][0] * v.y + (*this)[2][0] * v.z + (*this)[3][0];
		vv.y = (*this)[0][1] * v.x + (*this)[1][1] * v.y + (*this)[2][1] * v.z + (*this)[3][1];
		vv.z = (*this)[0][2] * v.x + (*this)[1][2] * v.y + (*this)[2][2] * v.z + (*this)[3][2];
	}

	inline void transformAffine(Vertex3f& vv) const
	{
		Vertex3f v = vv;
		vv.x =float( (*this)[0][0] * v.x + (*this)[1][0] * v.y + (*this)[2][0] * v.z + (*this)[3][0]);
		vv.y =float( (*this)[0][1] * v.x + (*this)[1][1] * v.y + (*this)[2][1] * v.z + (*this)[3][1]);
		vv.z =float( (*this)[0][2] * v.x + (*this)[1][2] * v.y + (*this)[2][2] * v.z + (*this)[3][2]);
	}
public:
	virtual void set(const double m[16]){
		memcpy(m_dData, m, sizeof(double) * 16);
	}
	virtual void set(const float m[16]){
		for (int i = 0; i<16; i++){
			m_dData[i] = m[i];
		}
	}
	virtual void set(const double m[4][4]){
		memcpy(m_dData, (void*)m, sizeof(double) * 16);
	}
	virtual void set(const float m[4][4]){
		int k = 0;
		for (int i = 0; i<4; i++){
			for (int j = 0; j<4; j++){
				m_dData[k] = m[i][j];
				k++;
			}
		}
	}

	virtual void get(double m[16]){
		memcpy(m, m_dData, sizeof(double) * 16);
	}

	virtual void get(float m[16]){
		for (int i = 0; i<16; i++)
			m_dData[i] = m[i];
	}

	virtual void get(double m[4][4]){
		memcpy((void*)m, m_dData, sizeof(double) * 16);
	}

	virtual void get(float m[4][4]){
		int k = 0;
		for (int i = 0; i<4; i++){
			for (int j = 0; j<4; j++){
				m[i][j] = (float)m_dData[k];
				k++;
			}
		}
	}

	virtual double  get(int r, int c) {
		return m_dData[r * 4 + c];
	}

	virtual void identity(){
		for (int i = 0; i<4; i++){
			for (int j = 0; j<4; j++){
				if (i == j)
					m_dData[i * 4 + j] = 1;
				else
					m_dData[i * 4 + j] = 0;
			}
		}
	}

	virtual void zero(){
		memset(m_dData, 0, sizeof(double) * 16);
	};



	virtual void copy(const Matrix4x4SharedPtr & m){}

	virtual Matrix4x4SharedPtr multiply(const Matrix4x4SharedPtr & m){
		return Matrix4x4SharedPtr();
	}
	virtual Matrix4x4SharedPtr add(const Matrix4x4SharedPtr & m){
		return Matrix4x4SharedPtr();
	}
	virtual Matrix4x4SharedPtr subtract(const Matrix4x4SharedPtr & m){
		return Matrix4x4SharedPtr();
	}
	virtual Matrix4x4SharedPtr transpose(){
		return Matrix4x4SharedPtr();
	}
	virtual Matrix4x4SharedPtr inverse(){
		return Matrix4x4SharedPtr();
	}

	virtual Matrix4x4SharedPtr rotateX(double r){
		return Matrix4x4SharedPtr();
	}
	virtual Matrix4x4SharedPtr rotateY(double r){
		return Matrix4x4SharedPtr();
	}
	virtual Matrix4x4SharedPtr rotateZ(double r){
		return Matrix4x4SharedPtr();
	}
	virtual void transformVertex(const double * pd, float * pf, int dim = 3){}
	virtual void transformVertex(const double * pd, double * pds, int dim = 3){}

	/** @} */
};
/** @} */

GV3DCORE_API Vertex3d operator*(const Vertex3d& lhv, const Matrix4x4& rhm); 

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
