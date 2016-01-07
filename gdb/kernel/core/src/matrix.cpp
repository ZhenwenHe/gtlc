#include "feature.h"






begin_gtl_namespace
begin_gdb_namespace

Matrix::Matrix(int iDim,double* const pData)
{
	assert(iDim > 0);

	m_dData = new double[iDim * iDim];

	if(pData)
	{
		memcpy(m_dData,pData,iDim * iDim * sizeof(double));
	}
	else
	{
		memset(m_dData,0,iDim * iDim * sizeof(double));
	}
	m_iDimM = iDim;

}

Matrix::Matrix(const Matrix& rv)
{
	int iDim = rv.m_iDimM;
	assert(iDim > 0);

	m_iDimM =iDim;
	m_dData = new double[iDim * iDim];
	memcpy(m_dData,rv.m_dData,iDim * iDim * sizeof(double));
}

void Matrix::operator*=(const Matrix& rhm)
{
	assert(m_iDimM == rhm.m_iDimM);//矩阵维数不等，不能相乘
	int i,j,k;
	double temp,a,b;
	Matrix tempMatrix(m_iDimM); 

	for( i = 0;i<m_iDimM;i++)
		for( j =0;j<m_iDimM;j++)
		{
			double* pData = tempMatrix.getData();
			pData[i*m_iDimM+j] = 0.0;
			temp = 0.0; 
			for( k = 0;k<m_iDimM;k++)
			{
				a = m_dData[i*m_iDimM+k];
				b = rhm.m_dData[ k*m_iDimM+j];
				temp = temp+a*b;
			} 
			pData[i*m_iDimM+j] = temp;
		}

		(*this) = tempMatrix;
}


int  Matrix::inverse()
{
	double d ,t;
	int i,j,k;
	int *is,*js;
	int x,y; 
	int n = m_iDimM;

	if( n == 1) 
	{
		if( fabs(m_dData[0]) < TOLERANCE)  return 0; 
		m_dData[0] = 1/m_dData[0];
		return 1;
	}

	is = new int[n];
	js = new int[n];

	for( k=0;k<n;k++)
	{
		d = 0.f;

		for( i=k;i<n;i++)
			for( j=k;j<n;j++)
			{
				t = m_dData[ i*n+j];
				if( fabs(t) > d)
				{
					d = fabs(t);
					is[k] = i;
					js[k] = j;
				}

			}

			if( d < 0.000001f) 
			{
				if(is) delete [] is;
				if(js) delete [] js;
				return -1;
			}	 
			x = is[k]; 
			y = js[k];

			for( j = 0;j<n;j++)
			{
				t = m_dData[k*n+j];
				m_dData[k*n+j] = m_dData[x*n+j];
				m_dData[x*n+j] = t;
			}

			for( i = 0;i<n;i++)
			{
				t = m_dData[i*n+k];
				m_dData[i*n+k] = m_dData[i*n+y];
				m_dData[i*n+y] = t;
			}

			m_dData[k*n+k] = 1.f/m_dData[k*n+k];

			for( i=0;i<n;i++)
			{
				if( i != k)   m_dData[i*n+k] = -m_dData[i*n+k]*m_dData[k*n+k];
			}

			for( i = 0;i<n;i++)
			{
				if( i != k)
				{
					for( j = 0;j<n;j++)
					{
						if( j != k)
							m_dData[i*n+j] = m_dData[i*n+j]+m_dData[k*n+j]*m_dData[i*n+k] ;
					} 
				}

			}

			for( j=0;j<n;j++)
			{
				if( j != k ) m_dData[k*n+j] = m_dData[k*n+j]*m_dData[k*n+k];
			}
	}
	for( k =n-1;k>=0 ;k--)
	{
		x = is[k];
		y = js[k];

		for( j=0;j<n;j++)
		{
			t = m_dData[k*n+j];
			m_dData[k*n+j] = m_dData[y*n+j];
			m_dData[y*n+j] = t;
		}

		for( i=0;i<n;i++)
		{
			t = m_dData[i*n+k];
			m_dData[i*n+k] = m_dData[i*n+x];
			m_dData[i*n+x] = t;
		}
	}
	if(is) delete [] is;
	if(js) delete [] js; 
	return 1;
}


void  Matrix::flip()
{
	int i,j;
	double *temp = new double[m_iDimM * m_iDimM];
	for( i=0; i<m_iDimM; i++)
		for( j=0; j<m_iDimM; j++)
			temp[i*m_iDimM+j] = m_dData[j*m_iDimM+i];

	for(i=0; i<m_iDimM*m_iDimM; i++)
		m_dData[i] = temp[i];

	delete []temp;
}


void  Matrix::loadIdentity()
{
	int i,j;
	for( i = 0 ;i < m_iDimM; i++)
		for( j = 0;j < m_iDimM; j++)
		{
			if( i == j ) m_dData[i*m_iDimM+j] = 1.f;
			else m_dData[i*m_iDimM+j] = 0.f;
		} 
}

const Matrix operator+(const Matrix& lhm, const Matrix& rhm)
{
	Matrix matRes(lhm);
	matRes += rhm;
	return matRes;
}

const Matrix operator-(const Matrix& lhm, const Matrix& rhm)
{
	Matrix matRes(lhm);
	matRes -= rhm;
	return matRes;
}

const Matrix operator+(const Matrix& lhm, double rhd)
{
	Matrix matRes(lhm);
	matRes += rhd;
	return matRes;
}

const Matrix operator-(const Matrix& lhm, double rhd)
{
	Matrix matRes(lhm);
	matRes -= rhd;
	return matRes;
}

const Matrix operator*(const Matrix& lhm, const Matrix& rhm)
{
	Matrix matRes(lhm);
	matRes *= rhm;
	return matRes;
}

const Matrix operator*(const Matrix& lhm, double rhd)
{
	Matrix matRes(lhm);
	matRes *= rhd;
	return matRes;
}

const Matrix operator/(const Matrix& lhm, double rhd)
{
	Matrix matRes(lhm);
	matRes /= rhd;
	return matRes;
}
//////////////////////////////////////////////////////////////
// 构造函数2 ：带参构造函数（初始化列表）
Matrix3x3::Matrix3x3(double* const pData):Matrix(3,pData){}

/** 构造函数1：带参构造函数（初始化列表）
*/
Matrix3x3::Matrix3x3():Matrix(3,NULL)
{
	operator=(IDENTITY3X3);
}

// 构造函数3 ：拷贝构造函数（初始化列表）
Matrix3x3::Matrix3x3(const Matrix3x3& rv):Matrix(rv){}

// 构造函数4
Matrix3x3::Matrix3x3 (double fEntry00, double fEntry01, double fEntry02,
	double fEntry10, double fEntry11, double fEntry12,
	double fEntry20, double fEntry21, double fEntry22):Matrix(3)
{
	m_dData[0] = fEntry00;  m_dData[1] = fEntry01;  m_dData[2] = fEntry02;
	m_dData[3] = fEntry10;  m_dData[4] = fEntry11;  m_dData[5] = fEntry12;
	m_dData[6] = fEntry20;  m_dData[7] = fEntry21;  m_dData[8] = fEntry22;
}

// 构造函数5
Matrix3x3::Matrix3x3(const Vertex3d& rowV1,const Vertex3d& rowV2,const Vertex3d& rowV3):Matrix(3)
{
	m_dData[0] = rowV1.x;  m_dData[1] = rowV1.y;  m_dData[2] = rowV1.z;
	m_dData[3] = rowV2.x;  m_dData[4] = rowV2.y;  m_dData[5] = rowV2.z;
	m_dData[6] = rowV3.x;  m_dData[7] = rowV3.y;  m_dData[8] = rowV3.z;
}
end_gdb_namespace
end_gtl_namespace


