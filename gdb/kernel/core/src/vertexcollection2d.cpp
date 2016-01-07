#include "vertexcollection2d.h"
#include "buffer.h"




begin_gtl_namespace
begin_gdb_namespace

 
/** 构造函数
* @param [in] 无
* @return
*/
VertexCollection2d::VertexCollection2d(void)
{
}

/** 拷贝构造
* @param [in] obj
* @return 无
*/
VertexCollection2d::VertexCollection2d(VertexCollection2d& obj)
{
	long lCount = obj.getCount();
	for (long i = 0; i < lCount; i++)
	{
		m_pointList.push_back(new Vertex2d(*(obj.getPointPtr(i))));
	}
}

/**析构函数
* @param [in]
* @return
*/
VertexCollection2d::~VertexCollection2d(void)
{
	clear();
}

/**将本类的信息写入输出流中，便于文件读写或缓冲区写操作
* @param  [in,out] f  std::ostream &，标准输出流对象引用
* @return 成功返回true，否则返回false
*/
void VertexCollection2d::write(std::ostream & f)
{
	size_t bufSize = sizeBuffer();
	Buffer buf;
	void * p = malloc(bufSize);
	buf.attach(p, bufSize);
	bool success = writeBuffer(buf);

	f.write((const char *)(&bufSize), sizeof(size_t));
	f.write((const char *)(&buf), bufSize);
	buf.clear();
}

/**从标准输入流中提取本类的信息
* @param  [in,out] f  std::istream &，标准输入流对象引用
* @return 成功返回true，否则返回false
*/
void VertexCollection2d::read(std::istream & f)
{
	size_t bufSize = 0;
	f.read((char *)(&bufSize), sizeof(size_t));/*读取对象写入到Buffer中所占用的字节数*/

	Buffer buf;
	void * p = malloc(bufSize);  /*申请内存*/
	buf.attach(p, bufSize);      /*Attach来指定一个有效的句柄付给这个对象*/
	f.read((char *)(&buf), bufSize);/*将对象信息读取到buffer中*/
	bool success = readBuffer(buf);
	buf.clear();
}

/**从Buffer中读取信息填充本几何对象
* @param  [in,out] buf Buffer & ，缓冲区对象引用
* @return 成功返回true，否则返回false
*##ModelId=4D0B58C70264
*/
bool VertexCollection2d::writeBuffer(Buffer &buf)
{
	int nelementNum = m_pointList.size();
	buf.write(&nelementNum, sizeof(int));

	for (std::vector<Vertex2d *>::iterator it = m_pointList.begin(); it != m_pointList.end(); it++)
	{
		buf.write(&((*it)->x), sizeof(double));
		buf.write(&((*it)->y), sizeof(double));
		//double x = (*it)->x;
		//double y = (*it)->y;
		//buf.write(&x,sizeof(double));
		//buf.write(&y,sizeof(double));
	}

	// 	for (int i=0;i<nelementNum;i++)
	// 	{
	// 		buf.write(&(m_pointList[i]->x),sizeof(double));
	// 		buf.write(&(m_pointList[i]->y),sizeof(double));
	// 	}
	return true;
}

/**从Buffer中读取信息填充本几何对象
* @param  [in,out] buf Buffer & ，缓冲区对象引用
* @return 成功返回true，否则返回false
*##ModelId=4D0B58C70264
*/
bool VertexCollection2d::readBuffer(Buffer &buf)
{
	clear();
	int listSize = 0;
	buf.read(&listSize, sizeof(int));

	double x, y;
	for (long i = 0; i < listSize; i++)
	{
		buf.read(&x, sizeof(double));
		buf.read(&y, sizeof(double));
		m_pointList.push_back(new Vertex2d(x, y));
	}
	return true;
}


/** 获取对象写入到Buffer中所占用的字节数
* @param 无
* @return 获取到的buffer字节数
*/
size_t VertexCollection2d::sizeBuffer()
{
	return sizeof(int) + m_pointList.size() * 2 * sizeof(double);
}


/** 添加元素到元素集中
* 如果before, after都为空，则添加到元素集的末尾
* @param [in] pt，要插入的元素
* @param [in] before，在此索引之前插入
* @param [in] after，在此索引之后插入
* @return
*/
void VertexCollection2d::addPointPtr(Vertex2d* obj, int* before, int* after)
{
	if (after == NULL && before == NULL)
	{
		m_pointList.push_back(obj);
	}
	else
	{
		if (before == NULL)
		{
			m_pointList.insert(m_pointList.begin() + *after, obj);
		}
		else
		{
			m_pointList.insert(m_pointList.begin() + *before - 1, obj);
		}
	}
}


/** 添加元素到元素集中
* 如果before, after都为空，则添加到元素集的末尾
* @param [in] pt，要插入的元素
* @param [in] before，在此索引之前插入
* @param [in] after，在此索引之后插入
* @return
*/
void VertexCollection2d::addPoint(double x, double y, int* before, int* after)
{
	Vertex2d* obj = new Vertex2d(x, y);
	if (after == NULL && before == NULL)
	{
		m_pointList.push_back(obj);
	}
	else
	{
		if (before == NULL)
		{
			m_pointList.insert(m_pointList.begin() + *after, obj);
		}
		else
		{
			m_pointList.insert(m_pointList.begin() + *before - 1, obj);
		}
	}
}

/** 将目标元素集中的元素添加到当前元素集的末尾
* @param [in] newElements，目标元素集
* @return 添加是否成功
*/
bool VertexCollection2d::addCollection(VertexCollection2dSharedPtr& newElements)
{
	if (newElements == NULL) return false;
	long lCount = newElements->getCount();
	double x, y;
	for (long i = 0; i < lCount; i++)
	{
		newElements->getPoint(i, x, y);
		m_pointList.push_back(new Vertex2d(x, y));
	}
	return true;
}

/** 将目标元素集中的元素插入到当前元素集中
* @param [in] index，插入位置
* @param [in] newElements，目标元素集
* @return 添加是否成功
*/
bool VertexCollection2d::insertCollection(long index, VertexCollection2dSharedPtr& newElements)
{
	if (newElements == NULL) return false;
	index = max(0, index);
	index = min(index, (long)m_pointList.size() - 1);

	long lCount = newElements->getCount();
	double x, y;
	for (long i = 0; i < lCount; i++)
	{
		newElements->getPoint(i, x, y);
		m_pointList.insert((m_pointList.begin() + index + i), new Vertex2d(x, y));
	}
	return true;
}

/** 获取指定索引的元素
* @param [in] index: 要获取的元素的索引
* @return: 获取到的元素的指针，获取失败则返回NULL
*/
Vertex2d* VertexCollection2d::getPointPtr(long index)
{
	return m_pointList.at(index);
}

const Vertex2d & VertexCollection2d::getPoint(long index)
{
	return *(m_pointList.at(index));
}

void VertexCollection2d::getPoint(long index, Vertex2d & outPoint)
{
	outPoint.x = m_pointList[index]->x;
	outPoint.y = m_pointList[index]->y;
}

void VertexCollection2d::getPoint(long index, double &x, double &y)
{
	x = m_pointList[index]->x;
	y = m_pointList[index]->y;
}


/** 设置指定索引的元素坐标
* @param [in] index: 要指定的元素的索引
* @return:
*/
void VertexCollection2d::setPoint(long index, const Vertex2d & outPoint)
{
	m_pointList[index]->x = outPoint.x;
	m_pointList[index]->y = outPoint.y;
}

void VertexCollection2d::setPoint(long index, double x, double y)
{
	m_pointList[index]->x = x;
	m_pointList[index]->y = y;
}


/** 获取元素的个数
* @return: 元素的个数
*/
long VertexCollection2d::getCount()
{
	return (long)m_pointList.size();
}

/** 交换元素集中的两个元素
* @param [in] index1,index2: 要交换的元素在数组中的索引
* @return: 交换成功返回true，否则返回false
*/
bool VertexCollection2d::swap(long index1, long index2)
{
	if (index1<0 || index1>(long)m_pointList.size() - 1)
	{
		return false;
	}
	if (index2<0 || index2>(long)m_pointList.size() - 1)
	{
		return false;
	}
	Vertex2d * ptemp = m_pointList[index1];
	m_pointList[index1] = m_pointList[index2];
	m_pointList[index2] = ptemp;
	return true;
}

/** 从元素集中移除指定索引的元素，并返回移除的元素
* @param [in] index: 要移除的元素的索引
* @return: 移除的元素的指针，失败则返回NULL
*/
Vertex2d * VertexCollection2d::removePointPtr(long index)
{
	if (index<0 || index>(long)m_pointList.size() - 1)
	{
		return 0;
	}
	Vertex2d* p = m_pointList[index];
	m_pointList.erase(m_pointList.begin() + index);
	return p;
}

/** 从元素集中删除指定索引的元素
* @param [in] index: 要删除的元素的索引
* @return: 删除失败则返回false
*/
bool VertexCollection2d::deletePoint(long index)
{
	if (index<0 || (index>(long)m_pointList.size() - 1))
	{
		return false;
	}
	Vertex2d* p = m_pointList[index];
	m_pointList.erase(m_pointList.begin() + index);
	delete p;
	return true;
}

/** 从元素集中删除无效的点（如果相邻点坐标相同，则删除）
* @param [in]
* @return: 清除无效点后，点阵的点的个数
*/
int VertexCollection2d::deleteInvalidPoint()
{
	Vertex2d * pt1 = 0;
	Vertex2d * pt2 = 0;
	for (int i = 1; i < (int)m_pointList.size(); i++)
	{
		pt1 = m_pointList[i];
		pt2 = m_pointList[i - 1];
		if (fabs(pt1->x - pt2->x) < SMALL_NUMBER &&
			fabs(pt1->y - pt2->y) < SMALL_NUMBER)
		{
			deletePoint(i);
			i--;
		}
	}
	return m_pointList.size();
}

/** 清空当前元素集
* @return
*/
void VertexCollection2d::clear()
{
	long lCount = (long)m_pointList.size();
	for (long i = 0; i < lCount; i++)
	{
		delete m_pointList[i];
	}
	m_pointList.clear();
}

/** @ 将目标数组 g 中的拷贝到 当前数组中
* @param  [in] g , ElementCollection2d *, 指向待拷贝的数组的指针
* @return 是否拷贝成功
*/
bool VertexCollection2d::copy(const VertexCollection2dSharedPtr& g)
{
	double x, y;
	for (long i = 0; i < g->getCount(); i++)
	{
		g->getPoint(i, x, y);
		m_pointList.push_back(new Vertex2d(x, y));
	}
	return true;
}

/** 获取线长度
* @param 无
* @return 线长度
*/
double VertexCollection2d::getLength()
{
	int nSize = (int)m_pointList.size();
	if (nSize < 2)return 0.0;

	double dsumLength = 0.0;
	Vertex2d * pCurPoint = NULL;
	Vertex2d * pNextPoint = NULL;
	for (int i = 0; i < nSize - 1; i++)
	{
		pCurPoint = m_pointList[i];
		pNextPoint = m_pointList[i + 1];
		dsumLength += sqrt((pCurPoint->x - pNextPoint->x)*(pCurPoint->x - pNextPoint->x)
			+ (pCurPoint->y - pNextPoint->y)*(pCurPoint->y - pNextPoint->y));
	}
	return dsumLength;
}


/** 获取点阵包围盒
* @param [out] envp
* @return
*/
void VertexCollection2d::getEnvelope(Envelope3d * envp)
{
	envp->minx = 1E20;
	envp->maxx = -1E20;
	envp->miny = 1E20;
	envp->maxy = -1E20;
	envp->minz = -100;
	envp->maxz = 100;
	int ncount = m_pointList.size();

	for (int i = 0; i < ncount; i++)
	{
		envp->minx = min(envp->minx, m_pointList[i]->x);
		envp->maxx = max(envp->maxx, m_pointList[i]->x);
		envp->miny = min(envp->miny, m_pointList[i]->y);
		envp->maxy = max(envp->maxy, m_pointList[i]->y);
	}
}

end_gdb_namespace
end_gtl_namespace


