#include "feature.h"



begin_gtl_namespace
begin_gdb_namespace 

/** @defgroup  工作区管理类
*  @{
*/
class WorkspaceImpl : public std::enable_shared_from_this<WorkspaceImpl>, public Workspace{
	/** @name 保护成员变量说明
	*  @{
	*/
protected:
	/**工作区/分区ID，在工程范围内是唯一的*/
	WKSPID   _regid;
	/**工作区名称*/
	std::string _name;
	/**工作区编码*/
	std::string _code;
	/**包围盒边界矩形,排列顺序为，X的最小最大，Y的最小最大，Z的最小最大*/
	double _bound[6];
	/**多边形区域*/
	Vertex3dVector  _polybound;
	/**空间参考坐标系统ID*/
	SREFID  _srefid;
	/**分区的级别*/
	int     _level;
	/**图层的集合*/
	std::vector< LayerSharedPtr > _set;
	/**当前图层索引，默认操作在当前层*/
	int  _cur_index;//
	/**工作区所在的工程*/
	ProjectSharedPtr  _localproject;
	/** @} */
public:
	/** @name 属性设置获得操作集
	*  @{
	*/
	/**获取工作区ID
	* @param  无
	* @return   工作区ID
	*/
	inline WKSPID getID(){ return _regid; }
	/**设置工作区ID
	* @param [in] id 工作区ID
	* @return 无
	*/
	inline void  setID(WKSPID id){ _regid = id; }

	/**获取工作区名称
	* @param 无
	* @return  工作区名称
	*/
	inline std::string getName(){ return _name; }
	/**设置工作区名称
	* @param [in] v 工作区名称
	* @return
	*/
	inline void  setName(std::string v){ _name = v; }
	/**获取工作区编码
	* @param 无
	* @return  工作区编码
	*/
	inline std::string getCode(){ return _code; }
	/**设置工作区编码
	* @param [in] v 工作区编码
	* @return
	*/
	inline void  setCode(std::string v){ _code = v; }

	/**获取包围盒边界矩形
	* @param [out] vl 包围盒边界矩形
	* @return  无
	*/
	inline void getBound(Envelope3d & vl) {
		vl.minx = _bound[0]; vl.maxx = _bound[1];
		vl.miny = _bound[2]; vl.maxy = _bound[3];
		vl.minz = _bound[4]; vl.maxz = _bound[5];
	}
	/**设置包围盒边界矩形
	* @param [in] vl 包围盒边界矩形
	* @return  无
	*/
	inline void setBound(Envelope3d & vl) {
		_bound[0] = vl.minx; _bound[1] = vl.maxx;
		_bound[2] = vl.miny; _bound[3] = vl.maxy;
		_bound[4] = vl.minz; _bound[5] = vl.maxz;
	}
	/**获取多边形区域
	* @param 无
	* @return  多边形区域
	*/
	inline Vertex3dVector & getPolyBound(){ return _polybound; }
	/**设置多边形区域
	* @param [in] plybound 多边形区域
	* @return  无
	*/
	inline void setPolyBound(Vertex3dVector & plybound) {
		_polybound.resize(plybound.size());
		std::copy(plybound.begin(), plybound.end(), _polybound.begin());
	}
	/**获取空间参考坐标系统ID
	* @param 无
	* @return  空间参考坐标系统ID
	*/
	inline SREFID getSRefID(){ return _srefid; }
	/**设置空间参考坐标系统ID
	* @param [in] s 空间参考坐标系统ID
	* @return  无	*/
	inline void setSRefID(SREFID s){ _srefid = s; }
	/**获取分区的级别
	* @param 无
	* @return  分区的级别
	*/
	inline int getLevel(){ return _level; }
	/**设置分区的级别
	* @param [in] l 分区的级别
	* @return  无	*/
	inline void setLevel(int l){ _level = l; }
	/**获取当前图层索引
	* @param  无
	* @return 当前图层索引
	*/
	inline LayerSharedPtr  getCurLayer(){
		LayerSharedPtr p;
		if (_cur_index<0)
			return p;
		return  _set[_cur_index];
	}
	/**设置当前图层索引，未实现
	* @param [in] layername 当前图层索引
	* @return  无
	*/
	void setCurLayer(std::string layername);
	/**设置当前图层索引
	* @param [in] s 当前图层索引
	* @return  无
	*/
	void setCurLayer(size_t s);
	/**设置工作区所在的工程
	* @param [in] pProj 工作区所在的工程指针
	* @return  无
	*/
	inline void setLocalProject(ProjectSharedPtr  pProj){ _localproject = pProj; }//设置
	/**获取工作区所在的工程
	* @param 无
	* @return  工作区所在的工程指针
	*/
	inline ProjectSharedPtr getLocalProject(){ return _localproject; }//获取工作区所在的工程
	/**获取包围盒边界矩形
	* @param [out] pv 包围盒边界矩形
	* @return  无
	*/
	inline void getBound(double * pv) {
		memcpy((void*)pv, _bound, sizeof(double) * 6);
	}
	/**设置包围盒边界矩形
	* @param [in] d[6] 包围盒边界矩形
	* @return  无
	*/
	inline void setBound(double d[6]){
		memcpy((void*)_bound, d, sizeof(double) * 6);
	}


	/** @} */

public:

	/** @name 构造与析构函数
	*  @{
	*/
	WorkspaceImpl();
	~WorkspaceImpl();
	void clear();
	/** @} */
	/** @name 拷贝构造与赋值构造函数
	*  @{
	*/
	void copy(const WorkspaceImpl &);
	WorkspaceImpl(const WorkspaceImpl &);
	WorkspaceImpl operator=(const WorkspaceImpl &);
	WorkspaceImpl(WKSPID id, std::string name, std::string code, double * bound, int level = 0, SREFID sid = 0);

	/** @} */


	/** 运算符重载
	* @param
	*/
	LayerSharedPtr operator() (size_t s);
	/** 运算符重载
	* @param
	*/
	const LayerRef operator() (size_t s) const;
	/** 运算符重载
	* @param
	*/
	bool operator==(const WorkspaceImpl &);


	/** @name 其它操作函数集
	*  @{
	*/

	/** 添加一个图层
	* @param [in] p 图层指针
	* @return 无
	*/
	void addLayer(std::shared_ptr<Layer> p);

	/** 根据要素类型ID，获取指定的要素集合;
	* @param [in] fclsid 要素类ID
	* @return 要素集指针
	*/
	FeatureSetSharedPtr findFeatureSet(FCLSID fclsid);

	/** 获取图层，未实现
	* @param [in] name 图层名称
	* @return 图层指针
	*/
	inline std::shared_ptr<Layer> getLayer(std::string name){
		std::shared_ptr<Layer> p;
		return p;
	}
	/** 获取图层的集合
	* @param 无
	* @return 图层的集合
	*/
	inline std::vector< std::shared_ptr<Layer> > & getLayers(){ return _set; }

	/** 索引图层，未实现
	* @param [in] name 图层名称
	* @return 图层ID
	*/
	size_t indexLayer(std::string layername);
	/** 获取图层的集合大小
	* @param 无
	* @return 图层的集合大小
	*/
	inline size_t  size() const { return _set.size(); }


	/** 将本工作区内所有对象的ID统一设置为当前工作区的ID
	* @param 无
	* @return 无
	*/
	void unifyID();


	/** @} */

	/** @name 文件IO操作函数集
	*  @{
	*/
public:
	/** 写文件
	* @param [in] f输出流
	* @return 无
	*/
	virtual void write(std::ostream & f);
	/** 读文件
	* @param [in] f输入流
	* @return 无
	*/
	virtual void read(std::istream & f);
	/** 加载文件
	* @param [in] sz文件路径
	* @return 无
	*/
	virtual void load(std::string & sz);
	/** 保存文件
	* @param [in] sz文件路径
	* @return 无
	*/
	virtual void save(std::string & sz);
	/** @} */
};
/** @} */ // End of the group block

 
end_gdb_namespace
end_gtl_namespace


