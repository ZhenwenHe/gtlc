#include "internal_EclipseMeshReader.h"
#include "internal_EclipseMeshImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
EclipseMeshReader::EclipseMeshReader(void)
: fp(NULL)
{
}

EclipseMeshReader::~EclipseMeshReader(void)
{
}

//功能描述：比较字符串是否与给定关键字相等，相等返回1
int EclipseMeshReader::CompareKeyword(char* keyword, int flag) //
{
	int length = strlen(keyword);
	int res;
	if (flag == 0)
		res = strncmp(m_Line, keyword, length);

	return(res == 0);
}
int EclipseMeshReader::ReadSpecGrid()
{
	std::string p = getLine();
	if (p.empty())
		return 0;
	int res = sscanf(m_Line, "%d %d %d", &m_ni, &m_nj, &m_nk);
	if (res != 3)
	{
		return 0;
	}
	return 1;
}
char* EclipseMeshReader::getLine()    //读取文本文件的一行，如果该行是注释行或者空行则跳过
{
	int goOn = 1;
	char* p = fgets(m_Line, 1024, fp);
	while (goOn && p)
	{
		m_LineNumber++;
		goOn = 0;
		if (CompareKeyword("--") && m_LineNumber<20)//注释行以“--”开头
		{
			p = fgets(m_Line, 1024, fp);
			goOn = 1;
		}
		if (p[0] == 10)					//判断是否为空行
		{
			p = fgets(m_Line, 1024, fp);
			goOn = 1;
		}
	}
	return(p);
}
int EclipseMeshReader::ReadCoord()
{
	int ndata = (m_ni + 1)*(m_nj + 1) * 6;
	m_coord = new double[ndata];
	memset(m_coord, 0, sizeof(float)*ndata);
	int res = ReadDoubles(ndata, m_coord);
	return res;
}
int EclipseMeshReader::ReadZcorn()
{
	int ndata = m_ni * m_nj *m_nk * 8;
	m_zcorn = new double[ndata];
	memset(m_zcorn, 0, sizeof(double)*ndata);
	int res = ReadDoubles(ndata, m_zcorn);
	return res;
}
int EclipseMeshReader::ReadActnum()
{
	int ndata = m_ni * m_nj *m_nk;
	m_actnum = new char[ndata];
	int res = ReadBytes(ndata, m_actnum);
	return res;
}
int EclipseMeshReader::ReadBytes(int nbytes, char* barray)
{
	char seps[] = " \t\n";
	char* token;
	int res;
	int d1;
	int index = 0;

	char *p = getLine();
	if (!p)
	{
		return 0;
	}

	int goOn = 1;
	while (goOn)
	{
		token = strtok(m_Line, seps);
		if (!token) goOn = 0;

		while ((token != NULL) && goOn)
		{
			res = sscanf(token, "%d", &d1);
			if (res == 1)
			{
				barray[index] = (d1 == 1);
				index++;
			}
			else
			{
				if (token[0] == '/')
				{
					goOn = 0;
				}
			}
			token = strtok(NULL, seps);
		}
		if (goOn)
		{
			p = getLine();
		}
	}
	printf("%f\n", barray[index - 1]);
	return 1;
}
int EclipseMeshReader::ReadDoubles(int nfloats, double* farray)
{
	char seps[] = " \t\n";
	char* token;
	int res;
	float f1;
	int index = 0;

	char *p = getLine();
	if (!p)
	{
		return 0;
	}

	int goOn = 1;
	while (goOn)
	{
		token = strtok(m_Line, seps);
		if (!token) goOn = 0;

		while ((token != NULL) && goOn)
		{
			res = sscanf(token, "%f", &f1);
			if (res == 1)
			{
				farray[index] = f1;
				index++;
			}
			else
			{
				if (token[0] == '/')
				{
					goOn = 0;
				}
			}
			token = strtok(NULL, seps);
		}
		if (goOn)
		{
			p = getLine();
		}
	}
	printf("%f\n", farray[index - 1]);
	return 1;
}

// 功  能:	由加密体字符串获取一个加密体的具体描述信息。
// 参  数:	i0:X方向单元格起始位置；i1:X方向单元格终止位置；j0,j1,k0,k1分别为J、K方向单元格起始
//			和终止位置,nx,ny,nz分别为加密后X,Y,Z三个方向的单元数
//   (入口):	 line
//   (出口):	 i0, i1,j0,j1,k0, k1, nx, ny, nz
// 返   回:   正常获取信息，则返回1，否则返回0 
int EclipseMeshReader::getCarinfo(const char* line, int& i0, int& i1, int& j0, int&j1, int &k0, int& k1,
	int& nx, int& ny, int& nz)
{

	sscanf(line, "%*s %d %d %d %d %d %d %d %d %d %*s", &i0, &i1, &j0, &j1, &k0, &k1, &nx, &ny, &nz);

	return 1;
}
void EclipseMeshReader::readGredclFile(const std::string filename)
{
	char* p;
	fp = fopen(filename.c_str(), "r");
	int res = 0;
	int carfin_idx = 0;

	m_LineNumber = 0;


	while (!feof(fp))

	{
		p = getLine();
		m_LineNumber++;

		if (CompareKeyword("SPECGRID"))
		{
			res = ReadSpecGrid();
		}

		if (CompareKeyword("COORDSYS"))
		{
			;
		}

		else

		{
			if (CompareKeyword("COORD"))
			{
				int npillars = (m_ni + 1) * (m_nj + 1);
				int ndata = 6 * npillars;
				m_coord = new double[ndata];
				memset(m_coord, 0, sizeof(double)*ndata);
				res = ReadCoord();
			}
		}

		if (CompareKeyword("ZCORN"))
		{
			int ndata = 8 * m_ni * m_nj * m_nk;
			m_zcorn = new double[ndata];
			memset(m_zcorn, 0, sizeof(double)*ndata);
			res = ReadZcorn();
		}

		if (CompareKeyword("ACTNUM"))
		{
			int ndata = m_ni * m_nj * m_nk;
			m_actnum = new char[ndata];
			memset(m_actnum, 1, sizeof(char)*ndata);
			res = ReadActnum();
		}

		if (CompareKeyword("-- Property"))
		{
			//读取属性的名称
			char* p = fgets(m_Line, 1024, fp);
			PropertyInfo proInfo;
			proInfo.name = p;
			//读取属性信息
			int ndata = m_ni * m_nj * m_nk;
			proInfo.proData = new double[ndata];
			memset(proInfo.proData, 0, sizeof(double)*ndata);
			res = ReadDoubles(ndata, proInfo.proData);
			//把属性信息存放到容器中
			m_VecPropertyInfo.push_back(proInfo);
		}
		if (CompareKeyword("CARFIN"))
		{
			CarfinInfo carfin;
			char *car_info = getLine();
			std::string car_str = car_info;

			int i0, i1, j0, j1, k0, k1, nx, ny, nz;
			//获取加密体的几何信息
			int res_car = getCarinfo(car_info, i0, i1, j0, j1, k0, k1, nx, ny, nz);
			carfin.mini = i0;
			carfin.maxi = i1;
			carfin.minj = j0;
			carfin.maxj = j1;
			carfin.mink = k0;
			carfin.maxk = k1;
			carfin.ni = nx;
			carfin.nj = ny;
			carfin.nk = nz;
			char* p;
			int ndata = nx*ny*nz;
			while (!CompareKeyword("ENDFIN"))
			{
				p = getLine();
				if (CompareKeyword("-- Property"))
				{
					//读取属性的名称
					char* p = fgets(m_Line, 1024, fp);
					PropertyInfo proInfo;
					proInfo.name = p;
					//读取属性信息
					proInfo.proData = new double[ndata];
					memset(proInfo.proData, 0, sizeof(double)*ndata);
					int nfloats_read = 0;
					float data = 0;
					int n = 0;
					res = ReadDoubles(ndata, proInfo.proData);
					carfin.VecPropertyInfo.push_back(proInfo);
				}
			}
			//存放加密信息
			m_VecCarfin.push_back(carfin);
		}
	}
}
void EclipseMeshReader::constructMesh(EclipseMeshImpl * m_pMesh)
{
	//XYZ逻辑单元数
	//m_pMesh = new EclipseMeshImpl(m_ni,m_nj,m_nk,m_VecPropertyInfo.size());
	m_pMesh->reset(m_ni, m_nj, m_nk, m_VecPropertyInfo.size());
	/*m_pMesh->_xCellNumber = m_ni;
	m_pMesh->_yCellNumber = m_nj;
	m_pMesh->_zCellNumber = m_nk;*/
	//顶点面坐标
	std::vector<gdb::Vertex3d> & topVertices = m_pMesh->getTopCtrlVertices();
	std::vector<gdb::Vertex3d> & bottomVertices = m_pMesh->getBottomCtrlVertices();
	for (int i = 0; i<(m_ni + 1)*(m_nj + 1); i++)
	{
		topVertices[i] = (gdb::Vertex3d(m_coord[6 * i], m_coord[6 * i + 1], m_coord[6 * i + 2]));
		bottomVertices[i] = (gdb::Vertex3d(m_coord[6 * i + 3], m_coord[6 * i + 4], m_coord[6 * i + 5]));
	}
	//Z值
	//m_pMesh->_zValues = new double[m_ni*m_nj*m_nk*8];
	memcpy(m_pMesh->getZValues(), m_zcorn, sizeof(double)*m_ni*m_nj*m_nk * 8);
	//网格有效性
	m_pMesh->getValidBitset().set((unsigned char*)m_actnum, m_ni*m_nj*m_nk);
	//memcpy(&m_pMesh->_validBitset._data[0],m_actnum,sizeof(char)*m_ni*m_nj*m_nk);
	//属性种类
	//m_pMesh->_propNumberPreCell = m_VecPropertyInfo.size();
	//属性名称
	for (int i = 0; i<m_VecPropertyInfo.size(); i++)
	{
		m_pMesh->getPropNames()[i] = m_VecPropertyInfo.at(i).name;
	}
	//属性值
	double * propValues = new double[m_ni*m_nj*m_nk*m_pMesh->getPropNumberPreCell()];
	for (int j = 0; j<m_pMesh->getPropNumberPreCell(); j++)
	{
		for (int i = 0; i < m_ni*m_nj*m_nk; i++)
		{
			propValues[i*m_pMesh->getPropNumberPreCell() + j] = m_VecPropertyInfo.at(j).proData[i];
		}
	}
	m_pMesh->setPropValues(propValues);
	delete[] propValues;

	//加密体
	for (int carfinNum = 0; carfinNum<m_VecCarfin.size(); carfinNum++)
	{
		//EclipseMeshImpl* subMesh = new EclipseMeshImpl();
		////加密体XYZ逻辑单元数
		//subMesh->_xCellNumber = m_VecCarfin.at(carfinNum).ni;
		//subMesh->_yCellNumber = m_VecCarfin.at(carfinNum).nj;
		//subMesh->_zCellNumber = m_VecCarfin.at(carfinNum).nk;
		////加密起始和终止位置
		//subMesh->_beginX =  m_VecCarfin.at(carfinNum).mini;
		//subMesh->_endX =  m_VecCarfin.at(carfinNum).maxi;
		//subMesh->_beginY =  m_VecCarfin.at(carfinNum).minj;
		//subMesh->_endY =  m_VecCarfin.at(carfinNum).maxj;
		//subMesh->_beginZ =  m_VecCarfin.at(carfinNum).mink;
		//subMesh->_endZ =  m_VecCarfin.at(carfinNum).maxk;
		////加密体属性个数
		//subMesh->_propNumberPreCell = m_VecCarfin.at(carfinNum).VecPropertyInfo.size();
		////加密体属性名称
		//for (int num=0;num<subMesh->_propNumberPreCell;num++)
		//{
		//	subMesh->_propNames.push_back(m_VecCarfin.at(carfinNum).VecPropertyInfo.at(num).name);
		//}
		////加密体属性值
		//subMesh->_propValues = new double[subMesh->_xCellNumber*subMesh->_yCellNumber*subMesh->_zCellNumber*subMesh->_propNumberPreCell];
		//for (int j=0;j<subMesh->_propNumberPreCell;j++)
		//{
		//	for (int i = 0;i < m_ni*m_nj*m_nk;i++)
		//	{ 
		//		subMesh->_propValues[i*subMesh->_propNumberPreCell+j] = m_VecCarfin.at(carfinNum).VecPropertyInfo.at(j).proData[i];
		//	}
		//
		//}	
		//m_pMesh->_children.push_back(subMesh);



		MeshSharedPtr pSubMesh = m_pMesh->submesh(
			m_VecCarfin.at(carfinNum).mini, m_VecCarfin.at(carfinNum).maxi,
			m_VecCarfin.at(carfinNum).minj, m_VecCarfin.at(carfinNum).maxj,
			m_VecCarfin.at(carfinNum).mink, m_VecCarfin.at(carfinNum).maxk,
			m_VecCarfin.at(carfinNum).ni, m_VecCarfin.at(carfinNum).nj, m_VecCarfin.at(carfinNum).nk);

		EclipseMeshImpl* subMesh = dynamic_cast<EclipseMeshImpl*>(pSubMesh.get());

		//加密体属性值
		double * pvalues = new double[subMesh->getXCellNumber()*subMesh->getYCellNumber()*subMesh->getZCellNumber()*subMesh->getPropNumberPreCell()];
		for (int j = 0; j<subMesh->getPropNumberPreCell(); j++)
		{
			for (int i = 0; i < m_ni*m_nj*m_nk; i++)
			{
				pvalues[i*subMesh->getPropNumberPreCell() + j] = m_VecCarfin.at(carfinNum).VecPropertyInfo.at(j).proData[i];
			}

		}
		subMesh->setPropValues(pvalues);
		delete[] pvalues;
		m_pMesh->getChildren().push_back(pSubMesh);
	}

	if (m_coord)
		delete[] m_coord;
	if (m_zcorn)
		delete[] m_zcorn;
	if (m_actnum)
		delete[] m_actnum;
	for (int num = 0; num<m_pMesh->getPropNumberPreCell(); num++)
	{
		if (m_VecPropertyInfo.at(num).proData)
			delete[] m_VecPropertyInfo.at(num).proData;
	}
	for (int carfinnum = 0; carfinnum<m_VecCarfin.size(); carfinnum++)
	{
		for (int propNum = 0; propNum<m_VecCarfin.at(carfinnum).VecPropertyInfo.size(); propNum++)
		{
			if (m_VecCarfin.at(carfinnum).VecPropertyInfo.at(propNum).proData)
			{
				delete[] m_VecCarfin.at(carfinnum).VecPropertyInfo.at(propNum).proData;
			}
		}
	}

}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace