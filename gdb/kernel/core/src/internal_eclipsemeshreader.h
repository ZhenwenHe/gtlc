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
class EclipseMeshImpl;
//属性信息结构体
struct PropertyInfo
{
	std::string name;
	double* proData;
};
//加密体信息结构体
struct CarfinInfo
{
	//加密的位置
	int mini;
	int maxi;
	int minj;
	int maxj;
	int mink;
	int maxk;
	//加密体格网数
	int ni;
	int nj;
	int nk;
	//加密体属性
	std::vector<PropertyInfo> VecPropertyInfo;
};
class EclipseMeshReader
{
public:
	EclipseMeshReader(void);
	~EclipseMeshReader(void);
	void readGredclFile(const std::string filename);
	void constructMesh(EclipseMeshImpl * mesh);//对mesh对象赋值
private:
	char* getLine();
	int CompareKeyword(char* keyword, int flag = 0);
	int ReadSpecGrid();
	int ReadCoord();
	int ReadZcorn();
	int ReadActnum();
	int ReadDoubles(int nfloats, double* farray);
	int ReadBytes(int nbytes, char* barray);
	static int getCarinfo(const char* line, int& i0, int& i1, int& j0, int&j1, int &k0, int& k1,
		int& nx, int& ny, int& nz);

private:
	FILE* fp;
	char m_Line[1024];
	int m_LineNumber;
	int m_ni, m_nj, m_nk;
	//存放角点网格模型属性信息
	std::vector<PropertyInfo> m_VecPropertyInfo;
	std::vector<CarfinInfo> m_VecCarfin;
public:
	double* m_coord; //存储地层顶底面坐标，共6*(m_iDim+1)*(m_jDim+1)个值。
	double* m_zcorn; //所有角点网格顶点的独立Z值，(2*m_iDim)*(2*m_jDim)*(2*m_kDim)个值
	char* m_actnum; //网格有效性，nx*ny*nz个值
};
end_gdb_namespace
end_gtl_namespace

