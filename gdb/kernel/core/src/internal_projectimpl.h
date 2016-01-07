#include "feature.h"



begin_gtl_namespace
begin_gdb_namespace 

/** @defgroup ProjectImpl 工程管理类
*  @{
*/
class ProjectImpl : public std::enable_shared_from_this<ProjectImpl>, public Project{
protected:

	/**工程ID*/
	PRJID                     _prjid;
	/**工程名称*/
	std::string               _name;
	/**工程边界*/
	double                    _bound[6];
	/**分区集合*/
	std::vector<WorkspaceSharedPtr> _regs;
	//RTree<FID,double,3,double>  _rtree;//本工作区范围内的RTree索引，默认为空树	
	//SpatialIndex * _indextree;
	/**当前分区索引，默认操作在当前区*/
	size_t  _cur_index;
	/**工程的多边形边界 */
	Vertex3dVector              _polybound;

public:
	/** @name 工程属性操作函数集
	*  @{
	*/
	/** 获取当前工作区
	* @param  无
	* @return 当前工作区
	*/
	WorkspaceSharedPtr  getCurWorkspace();
	/** 获取当前工程ID
	* @param  无
	* @return 当前工程ID
	*/
	inline PRJID getID()const;
	/** 设置当前工程ID
	* @param  [in] id 当前工程ID
	* @return 无
	*/
	inline void  setID(PRJID id);
	/** 获取当前工程名称
	* @param  无
	* @return 当前工程名称
	*/
	inline std::string getName();
	/** 设置当前工程名称
	* @param  [in] v 当前工程名称
	* @return 无
	*/
	inline void  setName(std::string v);
	/** 获取当前工程包围盒
	* @param  [out] vl 当前工程包围盒
	* @return 无
	*/
	inline void	getBound(Envelope3d & vl);
	virtual void getEnvelope(Envelope3dRef vl){
		getBound(vl);
	}
	/** 设置当前工程包围盒
	* @param  [in] vl 当前工程包围盒
	* @return 无
	*/
	inline void setBound(Envelope3d & vl);
	virtual void setEnvelope(Envelope3dRef vl){
		setBound(vl);
	}
	/** 设置当前工程包围盒
	* @param  [in] d[6] 当前工程包围盒
	* @return 无
	*/
	inline void setBound(double d[6]);
	virtual void setEnvelope(double * p){
		setBound(p);
	}
	/** 获取当前工程包围盒
	* @param  [out] pv 当前工程包围盒指针
	* @return 无
	*/
	inline void getBound(double * pv);
	/** 获取当前工程所有工作区
	* @param  无
	* @return 当前工程所有工作区集合
	*/
	inline std::vector<WorkspaceSharedPtr> & getWorkspaces();
	/** 当前工程添加工作区
	* @param [in] p 工作区指针
	* @return 无
	*/
	void addWorkspace(WorkspaceSharedPtr p);
	/** 清空工程，使工作区集合为空
	* @param 无
	* @return 无
	*/
	void clear();
	/** 获取工程的多边形边界
	* @param 无
	* @return 工程的多边形边界
	*/
	inline Vertex3dVector & getPolyBound();

	/** 设置工程的多边形边界
	* @param [in] plybound 工程多边形边界
	* @return 无
	*/
	inline void setPolyBound(Vertex3dVector & plybound);
	/** @} */


public:
	/** @name 构造与析构函数
	*  @{
	*/
	ProjectImpl();
	ProjectImpl(PRJID id, std::string name, double * bound);
	void copy(const ProjectImpl &);
	ProjectImpl(const ProjectImpl &);
	ProjectImpl operator=(const ProjectImpl &impl);
	~ProjectImpl();
	/** @} */

	/** @name 运算符重载
	*  @{
	*/
	/** 返回指定工作区
	* @param [in] s 工作区号
	* @return 工作区
	*/
	Workspace & operator() (size_t s);
	/** 返回指定工作区
	* @param [in] s 工作区号
	* @return 工作区（const引用）
	*/
	const Workspace & operator() (size_t s) const;
	/** 获取工作区个数
	* @param 无
	* @return 工作区外个数
	*/
	inline size_t  size() const { return _regs.size(); }
	/** @} */

	/** @name 文件读写函数集
	*  @{
	*/
public:
	/** 写文件
	* @param [in] f 输出流
	* @return 无
	*/
	virtual void write(std::ostream & f);
	/** 读文件
	* @param [in] f 输入流
	* @return 无
	*/
	virtual int read(std::istream & f);//xushicong 2009-10-14 返回值：0：正常；2：文件读取错误
	/** 从文件加载工程
	* @param [in] sz工程路径
	* @return 无
	*/
	virtual void load(std::string & sz);
	/** 将工程保存到文件
	* @param [in] sz工程路径
	* @return 无
	*/
	virtual void save(std::string & sz);

	virtual void setProjectAdapter(ProjectAdapterSharedPtr prjadapt){};

	/** @} */
};
/** @} */


//分布式数据库的工程对象实现，对应Project::create(signal),signal =1;
class DDBProjectImpl :public ProjectImpl
{
protected:
	ProjectAdapterSharedPtr m_projadapt;
public:
	DDBProjectImpl(ProjectAdapterSharedPtr _projadapt);
	DDBProjectImpl(PRJID id, std::string name, double * bound);
	void copy(const DDBProjectImpl &);
	DDBProjectImpl(const DDBProjectImpl &);
	DDBProjectImpl operator=(const DDBProjectImpl &impl);

	~DDBProjectImpl();
public:
	//重写工程的读写
	virtual void load(std::string & sz);
	virtual void save(std::string & sz);
	virtual void setProjectAdapter(ProjectAdapterSharedPtr prjadapt);

};


end_gdb_namespace
end_gtl_namespace


