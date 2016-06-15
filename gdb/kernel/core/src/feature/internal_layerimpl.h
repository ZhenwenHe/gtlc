#include "feature.h"



begin_gtl_namespace
begin_gdb_namespace
class LayerImpl : public std::enable_shared_from_this<LayerImpl>, public gdb::Layer//规定：一种要素类型只能属于一个图层
{

	/** @name 保护成员变量说明
	*  @{
	*/

protected:
	/** @name 属性（成员变量）
	* @{
	*/

	/** 图层id */
	LYRID _layid;

	/** 图层名称 */
	std::string _name;

	/** 图层包括的要素类型ID集合 */
	std::vector<FCLSID>	m_fclsids;

	/** 图层对应的要素集合 */
	std::vector<FeatureSetSharedPtr> _set;

	/** 图层的当前要素集合索引 */
	int    _cur_index;

	/** 图层所在的工作 */
	WorkspaceSharedPtr _localworkspace;//图层所在的工作

	/** 图层所在的工程 */
	PRJID  _prjID;
	/** @} */
public:
	/** @name 构造与析构函数
	*  @{
	*/
	LayerImpl();
	~LayerImpl();
	LayerImpl(/*std::string name,*/LYRID layid);
	/** @} */
	/** @name 拷贝构造与赋值构造函数
	*  @{
	*/
	void copy(const LayerRef);
	LayerImpl(const LayerImpl &);
	LayerImpl operator=(const LayerImpl &);
	/** @} */

	/** @name 属性设置获得操作集
	*  @{
	*/

	/**获取图层ID
	* @param  无
	* @return   图层ID
	*/
	inline LYRID getID(){ return _layid; }
	/**设置图层ID
	* @param [in] layid 图层ID
	* @return 无
	*/
	inline void  setID(LYRID layid){ _layid = layid; }

	/**获取图层名称
	* @param 无
	* @return  图层名称
	*/
	inline std::string getName(){ return _name; }
	/**设置图层名称
	* @param [in] s 图层名称
	* @return
	*/
	inline void setName(std::string s){ _name = s; }


	/**获取图层类型所在工程
	* @param 无
	* @return  prjID 工程ID
	*/
	inline PRJID getProjectID(){ return _prjID; }
	/**设置图层类型所在的工程
	* @param [in] prjID 工程ID
	* @return
	*/
	inline void  setProjectID(PRJID prjID){ _prjID = prjID; }

	/**获取图层对应的要素类型ID集合
	* @param 无
	* @return  图层对应的要素类型ID集合
	*/
	inline vector<FCLSID>& getFeatureClassIDs() { return m_fclsids; }
	/**设置图层对应的要素类型ID集合
	* @param [in] fclsids  要素类型ID集合
	* @return  无
	*/
	inline void setFeatureClassIDs(vector<FCLSID>& fclsids) { m_fclsids = fclsids; }

	/** 获取图层对应的要素集合
	* @param 无
	* @return  图层对应的要素集合
	*/
	inline std::vector<FeatureSetSharedPtr> & getFeatureSetVector(){ return _set; }
	/** 获取图层对应的当前要素
	* @param 无
	* @return  图层对应的当前要素
	*/
	inline FeatureSetSharedPtr getCurrentFeatureSet(){
		FeatureSetSharedPtr p;
		if (_cur_index<0) return p;
		return _set[_cur_index];
	}
	/** 设置图层对应的当前要素
	* @param [in] s 当前要素集合索引
	* @return  无
	*/
	inline void setCurrentFeatureSet(size_t s){ _cur_index = (int)s; }
	/** @} */

	/** @name 其它操作函数集
	*  @{
	*/
	/** 在图层的要素类型ID集合中添加指定要素类型ID
	* @param [in] fclsid 要素类型ID
	* @return  无
	*/
	inline void addFeatureClassID(FCLSID fclsid) { m_fclsids.push_back(fclsid); }

	/** 在图层的要素类型ID集合中删除指定要素类型ID
	* @param [in] fclsid 要素类型ID
	*/
	void eraseFeatureClassID(FCLSID fclsid);


	/** 获取指定要素ID的要素所在要素集合索引
	* @param [in] id 要素ID
	* @return  失败 -1,成功所在要素集合索引
	*/
	size_t indexFeatureSet(FSID id);

	/** 创建一个指定要素类的要素集
	* @param [in] fc 指定要素数据
	* @return 要素集指针
	*/
	FeatureSetSharedPtr createFeatureSet(const FeatureClassRef fc);
	/** 向图层中增加一个要素集合数据
	* @param [in] p 要素数据指针
	* return 要素数据指针
	*/
	FeatureSetSharedPtr addFeatureSet(FeatureSetSharedPtr fs);
	/** 在图层的要素集合中删除指定要素集合ID的要素集合数据
	* @param [in] fid 要素ID
	* return 无
	*/
	void eraseFeatureSet(FSID fsid);
	/** 运算符重载
	* @param
	*/
	FeatureSetSharedPtr  operator() (size_t s);
	/** 运算符重载
	* @param
	*/
	bool operator ==(const LayerImpl& lay);
	/** 运算符重载
	* @param
	*/
	const FeatureSet&   operator() (size_t s) const;
	/** 查询图层要素集合大小
	* @param 无
	* return 图层要素集合大小
	*/
	inline size_t  size() const { return _set.size(); }

	/** 设置图层所在的工作区ID的要素集合数据
	* @param [in] pws 当前工作区指针
	*/
	inline void setLocalWorkspace(WorkspaceSharedPtr pws){ _localworkspace = pws; }
	/** 获取图层所在的工作区
	* @param [in]
	* @return 获取图层所在的工作区
	*/
	inline WorkspaceSharedPtr getLocalWorkspace(){ return _localworkspace; }

	/** 清空图层对应的要素集合
	* @param 无
	* @return 无
	*/
	void clear();

	virtual FeatureSetSharedPtr getFeatureSet(int i)
	{
		return _set[i];
	}
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


	virtual bool isEqual(const LayerRef  lr) {
		return (*this) == (LayerImpl&)lr;
	}
	virtual bool isEqual(const LayerSharedPtr l) {
		return (*this) == (LayerImpl&)(*l.get());
	}
};
/** @} */ // End of the group block

end_gdb_namespace
end_gtl_namespace


