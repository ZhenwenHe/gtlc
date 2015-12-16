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
#include "assert.h"
#include <cmath>
#include "vertex2d.h" 


begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

class Buffer;
class VertexCollection2d;
typedef std::shared_ptr< VertexCollection2d >  VertexCollection2dSharedPtr;

/** @addtogroup vertex  VertexCollection2d-点集类，可以用来管理点元素集
*  @{
*/
class CORE_API VertexCollection2d
{
public:
	/** 构造函数
	* @param [in] 无
	* @return
	*/
	VertexCollection2d(void);

	/** 拷贝构造
	* @param [in] obj
	* @return 无
	*/
	VertexCollection2d(VertexCollection2d& obj);


	/**析构函数
	* @param [in]
	* @return
	*/
	~VertexCollection2d(void);

	/**将本类的信息写入输出流中，便于文件读写或缓冲区写操作
	* @param  [in,out] f  std::ostream &，标准输出流对象引用
	* @return 成功返回true，否则返回false
	*/
	void write(std::ostream & f);

	/**从标准输入流中提取本类的信息
	* @param  [in,out] f  std::istream &，标准输入流对象引用
	* @return 成功返回true，否则返回false
	*/
	void read(std::istream & f);

	/*从Buffer中读取信息填充本几何对象
	* @param  [in,out] buf Buffer & ，缓冲区对象引用
	* @return 成功返回true，否则返回false
	*/
	bool writeBuffer(cn::edu::cug::gdb::Buffer &buf);

	/*从Buffer中读取信息填充本几何对象
	* @param  [in,out] buf Buffer & ，缓冲区对象引用
	* @return 成功返回true，否则返回false
	*/
	bool readBuffer(cn::edu::cug::gdb::Buffer &buf);

	/** 获取对象写入到Buffer中所占用的字节数
	* @param 无
	* @return 获取到的buffer字节数
	*/
	size_t sizeBuffer();

	/** 添加元素到元素集中
	* 如果before, after都为空，则添加到元素集的末尾
	* @param [in] pt，要插入的元素
	* @param [in] before，在此索引之前插入
	* @param [in] after，在此索引之后插入
	* @return
	*/
	void addPoint(double x, double y, int* before = NULL, int* after = NULL);
	void addPointPtr(Vertex2d* obj, int* before = NULL, int* after = NULL);

	/** 将目标元素集中的元素添加到当前元素集的末尾
	* @param [in] newElements，目标元素集
	* @return 添加是否成功
	*/
	bool addCollection(VertexCollection2dSharedPtr& newElements);

	/** 将目标元素集中的元素插入到当前元素集中
	* @param [in] index，插入位置
	* @param [in] newElements，目标元素集
	* @return 添加是否成功
	*/
	bool insertCollection(long index, VertexCollection2dSharedPtr& newElements);

	/** 获取指定索引的元素
	* @param [in] index: 要获取的元素的索引
	* @return: 获取到的元素的指针，获取失败则返回NULL
	*/
	Vertex2d* getPointPtr(long index);
	virtual const Vertex2d & getPoint(long index);
	virtual void getPoint(long index, Vertex2d & outPoint);
	virtual void getPoint(long index, double &x, double &y);

	/** 设置指定索引的元素坐标
	* @param [in] index: 要指定的元素的索引
	* @return:
	*/
	virtual void setPoint(long index, const Vertex2d &pPoint);
	virtual void setPoint(long index, double x, double y);

	/** 获取元素的个数
	* @return: 元素的个数
	*/
	long getCount();

	/** 交换元素集中的两个元素
	* @param [in] index1,index2: 要交换的元素在数组中的索引
	* @return: 交换成功返回true，否则返回false
	*/
	bool swap(long index1, long index2);

	/** 从元素集中移除指定索引的元素，并返回移除的元素
	* @param [in] index: 要移除的元素的索引
	* @return: 移除的元素的指针，失败则返回NULL
	*/
	Vertex2d * removePointPtr(long index);

	/** 从元素集中删除指定索引的元素
	* @param [in] index: 要删除的元素的索引
	* @return: 删除失败则返回false
	*/
	bool deletePoint(long index);

	/** 从元素集中删除无效的点（如果相邻点坐标相同，则删除）
	* @param [in]
	* @return: 清除无效点后，点阵的点的个数
	*/
	int deleteInvalidPoint();

	/** 清空当前元素集
	* @return
	*/
	void clear();

	/** @ 将目标数组 g 中的拷贝到 当前数组中
	* @param  [in] g , ElementCollection2d *, 指向待拷贝的数组的指针
	* @return 是否拷贝成功
	*/
	bool copy(const VertexCollection2dSharedPtr& g);

	/** 获取线长度
	* @param 无
	* @return 线长度
	*/
	double getLength();

	/** 获取点阵包围盒
	* @param [out] envp
	* @return
	*/
	virtual void getEnvelope(cn::edu::cug::gdb::Envelope3d * envp);
protected:
	std::vector< Vertex2d  * > m_pointList;

};
/** @} */

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace

