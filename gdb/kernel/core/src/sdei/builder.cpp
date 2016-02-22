#include "connector.h"
#include "builder.h"
#include "factory.h"




begin_gtl_namespace
begin_gdb_namespace

BuilderSharedPtr Builder::newBuilder(int signal){
	switch(signal){
	case Factory::FACTORYTYPE_FDB_BUILDER://文件数据库
		{
			return ((FDBBuilderFactory*)Factory::getFactory(signal))->newBuilder();
		}
	case Factory::FACTORYTYPE_RDB_BUILDER://关系数据库
		{
			return ((RDBBuilderFactory*)Factory::getFactory(signal))->newBuilder();
		}
	case Factory::FACTORYTYPE_CDB_BUILDER://集群数据库
		{
			return ((CDBBuilderFactory*)Factory::getFactory(signal))->newBuilder();
		}
	case Factory::FACTORYTYPE_DDB_BUILDER://分布式并行数据库
		{
			return ((DDBBuilderFactory*)Factory::getFactory(signal))->newBuilder();
		}
	}
	return BuilderSharedPtr();
}

Builder::Builder():_connector(""){
	_builderType=0;
	_quicktype = false; //默认为false，需要查询ID，判断是否存在，然后数据
}
Builder::~Builder(){}

/** @name 其它操作函数集  
*  @{
*/	
/**  获取builder的类型
* @param [in] 无
* @return 返回1为文件系统,否则返回2为数据库系统
*/
int Builder::getType(){
	return _builderType;
}

/**  获取临时操作的中间结果记录
* @param [in] 无
* @return 返回临时操作中间结果记录TemporarySets
*/
TemporarySets & Builder::getTemporarySets(){ return _tempsets;}

/**  获取选择集合
* @param [in] 无
* @return 返回临时操作中间结果记录TemporarySets
*/
Selector & Builder::getSelector(){ return _selector;}

/**  删除选择集中的要素对象
* @return 返回删除的个数 
*/
long Builder::eraseSelectedFeatures() {
	size_t r = _selector.size();
	for(size_t i =0; i<r;i++){
		FeatureSharedPtr p = _selector.removeAt(i);
		if(p)
			removeFeature(p->getID(),p->getFeatureClassID());
	}
	return r;
}

long Builder::mergeFeatureClass( const FCLSID & fclsid,vector<FCLSID> &fclsids)
{
	long iCountMerge = 0;
	FeatureClassSharedPtr mainFcsp;
	getProxy()->getFeatureClass(fclsid,mainFcsp);
	for(int i = 0 ;i < fclsids.size(); ++i)
	{
		FeatureClassSharedPtr fcsp;
		getProxy()->getFeatureClass(fclsids[i],fcsp);
		//如果要素类结构一致，则可以合并
		if (mainFcsp->isEqual(*fcsp))
		{
			vector<FeatureSharedPtr> objs;
			FeatureSharedPtr obj;
			getProxy()->queryByFeatureClassID(fclsids[i],objs,true);
			for(size_t j = 0; j < objs.size(); ++j)
			{
				objs[j]->setFeatureClass(mainFcsp);
				addFeature(objs[j]);
				addLOD(objs[j]);
			}
			removeFeatureClass(fclsids[i]);
			iCountMerge++;
		}
	}
	return iCountMerge;
}

/**  对数据库进行裁剪，得到的结果保存到resultBuilder中
* @param [in,out] resultBuilder 
* @return 执行成功返回true，否则返回false
*/
bool Builder::clip( const Envelope3dRef bound, Builder * resultBuilder) {

	vector<FeatureClassSharedPtr> fclass_set;
	vector<FCLSID> fclassid_set;
	_proxySptr->getAllFeatureClass(fclass_set);
	vector<FeatureClassSharedPtr>::iterator it1=fclass_set.begin();
	while(it1!=fclass_set.end())	{
		fclassid_set.push_back((*it1)->getID());
		it1++;
	}

	std::vector<FeatureSharedPtr> fobj_set;
	_proxySptr->queryByBox(bound,fclassid_set,fobj_set);//按范围查出要素对象
	_proxySptr->loadLOD(fobj_set);//获取要素对象的LOD信息

	//保存
	if (fobj_set.size()>0)	{
		//获取符合查询条件的要素对象的要素类id
		FCLSID temfclsid;
		vector<FCLSID> vecFclsIdinCombox;
		vector<FeatureSharedPtr>::iterator it = fobj_set.begin();
		FeatureClass *pfcls = 0;
		this->setActive();
		for (unsigned int i = 0;i<fobj_set.size();i++)	{
			pfcls = (*it)->getFeatureClass().get();
			temfclsid = pfcls->getID();
			int flg = 0;
			for (unsigned int j =0;j< vecFclsIdinCombox.size();j++)	{
				if (temfclsid == vecFclsIdinCombox[j])	{
					flg = 1;
					break;
				}
			}

			if (!flg)	{
				vecFclsIdinCombox.push_back(temfclsid);
				vector<FeatureClassSharedPtr>::iterator itfcls=fclass_set.begin();
				while(itfcls!=fclass_set.end())
				{
					if ((*itfcls)->getID() == temfclsid)
					{
						resultBuilder->setActive();
						resultBuilder->addFeatureClass(*itfcls);
					}
					itfcls++;
				}
			}

			resultBuilder->setActive();
			resultBuilder->addFeature(*it);
			resultBuilder->addLOD(*it);
			this->setActive();
			it++;
		}

		//保存
		std::vector<MATID> sMatIDSet; //用于存放包含的材质ID
		std::vector<TEXID> sTexIDSet; //用于存放包含的纹理ID
		std::vector<MDLID> sMdlIDSet; //用于存放包含的共享模型ID

		vector<MaterialSharedPtr> aMaterials;//材质索引信息
		vector<TextureSharedPtr> aTextures;//材质索引信息
		vector<SharedModelSharedPtr> aModels;//材质索引信息

		//获取Feature对象中包含材质、纹理和共享模型ID
		for(vector<FeatureSharedPtr>::iterator fit= fobj_set.begin();fit!=fobj_set.end();fit++)
		{
			GeometrySharedPtr pGeom = (*fit)->getGeometry();
			/*SharedMdlFileUtils::getFileUtils().getSharedModelIDFromGeometry(pGeom,sMdlIDSet);
			MaterialFileUtils::getFileUtils().getMaterilIDFromGeometry(pGeom,sMatIDSet);*/
			_proxySptr->getSharedModelIDs(pGeom,sMdlIDSet);
			_proxySptr->getMaterilIDs(pGeom,sMatIDSet);
		}


		//从共享模型库中查找模型
		for(vector<MDLID>::iterator MdlidIT = sMdlIDSet.begin(); MdlidIT != sMdlIDSet.end();MdlidIT++)
		{
			this->setActive();
			SharedModelSharedPtr pMdl = _proxySptr->getSharedModel(*MdlidIT);
			if (pMdl)
			{
				aModels.push_back(pMdl);
				resultBuilder->setActive();
				resultBuilder->addSharedModel(pMdl);
			}
		}

		//获取共享模型中关联的材质ID
		for(vector<SharedModelSharedPtr>::iterator mdlit = aModels.begin();mdlit!=aModels.end();mdlit++)		
		{
			vector<MATID> linkmatid = (*mdlit)->getMaterialIDs();

			long num=linkmatid.size();
			for (long i=0;i<num;i++)
			{
				MATID nMatID = linkmatid[i];
				sMatIDSet.push_back(nMatID);
			}
		}
		//从材质库中查找材质
		for(vector<MATID>::iterator MatidIT = sMatIDSet.begin(); MatidIT != sMatIDSet.end();MatidIT++)	{
			this->setActive();
			MaterialSharedPtr pMat = _proxySptr->getMaterial(*MatidIT);
			if (pMat)	{
				aMaterials.push_back(pMat);
				resultBuilder->setActive();
				resultBuilder->addMaterial(pMat);
			}
		}


		//分材质类型获取纹理ID
		for(vector<MaterialSharedPtr>::iterator matit = aMaterials.begin();matit!=aMaterials.end();matit++)	{
			int MatType = (*matit)->getType();
			if(MatType == 0)
			{
				SimpleMaterial * psimple = dynamic_cast<SimpleMaterial *>((*matit).get());
				TEXID TextureID = psimple->getTextureID();
				sTexIDSet.push_back(TextureID);
			}
			else if(MatType == 1)
			{
				ComplexMaterial * pcomplex = dynamic_cast<ComplexMaterial *>((*matit).get());
				std::vector<COMMAT_ITEM> &MaterialItemList = pcomplex->getMaterialItems();
				long Num = MaterialItemList.size();
				for(long i=0; i<Num; i++)
				{
					TEXID texid = MaterialItemList[i].textureID;
					sTexIDSet.push_back(texid);
				}
			}
		}

		//从纹理库中查找纹理
		for(vector<TEXID>::iterator TexidIT = sTexIDSet.begin(); TexidIT != sTexIDSet.end();TexidIT++)
		{
			this->setActive();
			TextureSharedPtr pTex = _proxySptr->getTexture(*TexidIT);
			if (pTex)
			{
				aTextures.push_back(pTex);
				resultBuilder->setActive();
				resultBuilder->addTexture(pTex);
			}
		}

		//拓扑节点
		this->setActive();
		vector<TopoItemSharedPtr> topo_node;
		_proxySptr->getAllTopos(topo_node);
		resultBuilder->setActive();
		resultBuilder->addTopos(topo_node);
		topo_node.clear();

		//拓扑语义
		this->setActive();
		vector<TopoInfoSharedPtr> topo_info;
		_proxySptr->getAllTopos(topo_info);
		resultBuilder->setActive();
		resultBuilder->addTopos(topo_info);
		topo_info.clear();

		//释放
		resultBuilder->save();
		this->setActive();
		return true;
	}
	else
		return false;
}

bool Builder::exclude( const Envelope3dRef  bound)
{
	return false;
}		
/**  将fromBuilder中的数据导入进本数据库中,不包含DEM和DOM数据
* @param [in] fromBuilder 
* @return 执行成功返回true，否则返回false
*/
bool Builder::import( Builder * fromBuilder) {
	vector<FeatureClassSharedPtr> fcls_orgset;
	_proxySptr->getAllFeatureClass(fcls_orgset); //获得当前工程的要素类

	//读取导入数据库中的数据
	fromBuilder->setActive();
	ProxySharedPtr proxy_from = fromBuilder->getProxy();

	//导入要素类及要素对象信息
	vector<FeatureClassSharedPtr> fcls_newset;
	proxy_from->getAllFeatureClass(fcls_newset);//获得要导入工程的要素类

	vector<FeatureClassSharedPtr>::iterator it1=fcls_newset.begin();
	while(it1!=fcls_newset.end())	{
		bool bfind=false;
		//判断新要素类ID与当前的是否一致
		vector<FeatureClassSharedPtr>::iterator it2=fcls_orgset.begin();
		while(it2!=fcls_orgset.end()){
			if ((*it1)->getID()==(*it2)->getID()){
				bfind=true;
				break;
			}
			it2++;
		}
		if (bfind==false){//如ID不一致则添加新要素类			
			//添加要素类
			this->setActive();
			this->addFeatureClass(*it1);
		}
		//通过此新要素类获取所有要素对象并添加到要素类中
		fromBuilder->setActive();
		//[2010-07-26 add by liuyanting 修改获取要素对象方式，原来的数据量大时容易死机]
		vector<FID> fidset;
		//获取要素对象ID集合
		FCLSID fclsid = (*it1)->getID();
		proxy_from->queryFeatureIDsByFeatureClassID(fclsid,fidset);
		vector<FeatureSharedPtr> fobjset;
		vector<FID>::iterator it3 = fidset.begin();
		while (it3!=fidset.end()){
			fromBuilder->setActive();
			FeatureSharedPtr fobj;
			//获取要素对象
			proxy_from->queryByFeatureID((*it3),fclsid,fobj);
			//获取LOD信息

			fobjset.push_back(fobj);
			proxy_from->loadLOD(fobjset);//获取LOD信息

			this->setActive();
			this->addFeature(fobj);//添加要素对象
			this->addLOD(fobj);//添加LOD信息

			fobjset.clear();
			it3++;
		}
		it1++;
	}

	//添加纹理数据、材质数据、共享模型数据
	//材质数据
	fromBuilder->setActive();
	vector<MATID> mater_id; 
	proxy_from->getAllMaterialID(mater_id);
	vector<MATID>::iterator matit=mater_id.begin();
	while (matit!=mater_id.end()){
		fromBuilder->setActive();
		MaterialSharedPtr pMat = proxy_from->getMaterial(*matit);
		this->setActive();
		this->addMaterial(pMat);
		matit++;			
	}
	mater_id.clear();

	//纹理数据
	fromBuilder->setActive();
	vector<TEXID> tex_id;
	proxy_from->getAllTextureID(tex_id);
	vector<TEXID>::iterator texit=tex_id.begin();
	while (texit!=tex_id.end())	{
		fromBuilder->setActive();
		TextureSharedPtr pTex = proxy_from->getTexture(*texit);
		this->setActive();
		this->addTexture(pTex);
		texit++;
	}
	tex_id.clear();

	//共享模型数据
	fromBuilder->setActive();
	vector<MDLID> mdl_id;
	proxy_from->getAllSharedModelID(mdl_id);
	vector<MDLID>::iterator mdlit=mdl_id.begin();
	while (mdlit!=mdl_id.end()) {
		fromBuilder->setActive();
		SharedModelSharedPtr pMdl = proxy_from->getSharedModel(*mdlit);
		this->setActive();
		this->addSharedModel(pMdl);
		mdlit++;		
	}
	mdl_id.clear();

	//拓扑节点
	fromBuilder->setActive();
	vector<TopoItemSharedPtr> topo_node;
	proxy_from->getAllTopos(topo_node);
	this->setActive();
	this->addTopos(topo_node);
	topo_node.clear();

	//拓扑语义
	fromBuilder->setActive();
	vector<TopoInfoSharedPtr> topo_info;
	proxy_from->getAllTopos(topo_info);
	this->setActive();
	this->addTopos(topo_info);
	topo_info.clear();

	//导入完成
	this->save();	
	return true;
}


BuilderSharedPtr Builder::clone(BuilderSharedPtr gFBuildr ){
	////新建一个Builder，并打开
	//GfileBuilder * gFBuildr= new GfileBuilder;
	//gFBuildr->create(szNewPath);
	//gFBuildr->open(szNewPath);

	//复制要素类型和要素信息
	std::map<FCLSID,FeatureClassSharedPtr> & fclsMap = getFeatureClassMap();
	for(std::map<FCLSID,FeatureClassSharedPtr>::iterator mit = fclsMap.begin();mit!=fclsMap.end();mit++)
	{
	
		gFBuildr->addFeatureClass( mit->second->clone() );
		this->setActive();
		FCLSID fclsid = mit->first;
		vector<FID> fids;
		getProxy()->queryFeatureIDsByFeatureClassID(fclsid,fids);

		vector<FeatureSharedPtr> fobjset;
		for (vector<FID>::iterator it_fid = fids.begin();it_fid != fids.end();it_fid++)
		{
			this->setActive();
			FeatureSharedPtr fobj;
			_proxySptr->queryByFeatureID((*it_fid),fclsid,fobj);
			fobjset.push_back(fobj);

			//以下为获取feature的lod信息一种方式  by liyingying
			/*LODInfo lodinfo = (*fobj).get()->getLODInfo();
			vector<Info> infos =  lodinfo.getLodS();
			for (vector<Info>::iterator it_info = infos.begin();it_info != infos.end();it_info++)
			{
			int lodLevel = (*it_info).first.lodLevel;
			proxy->loadLOD((*fobj).get()->getID(),lodLevel,(*it).get()->getID(),*fobj);
			}*/

			//[2010-07-28 modify by liuyanting 先获取lod信息，再将feature插入，省去addlod时更新feature的步骤]
			_proxySptr->loadLOD(fobjset);//获取LOD信息
			gFBuildr->setActive();
			gFBuildr->addFeature(fobj);
			gFBuildr->addLOD(fobj);
			fobjset.clear();
			gFBuildr->save();
		}
	}

	//复制纹理信息
	this->setActive();
	std::vector<TEXID> texid_set;
	_proxySptr->getAllTextureID(texid_set);
	TextureSharedPtr  ptexture;
	for (vector<TEXID>::iterator it = texid_set.begin();it!=texid_set.end();it++)
	{
		this->setActive();
		ptexture =_proxySptr->getTexture(*it);
		gFBuildr->setActive();
		gFBuildr->addTexture(ptexture);
	}
	gFBuildr->save();

	//复制共享模型信息
	this->setActive();
	std::vector<MDLID> mdlid_set;
	_proxySptr->getAllSharedModelID(mdlid_set);
	SharedModelSharedPtr psharemodel;
	for (vector<MDLID>::iterator it = mdlid_set.begin();it!= mdlid_set.end();it++)
	{		
		this->setActive();
		psharemodel = _proxySptr->getSharedModel(*it);
		gFBuildr->setActive();
		gFBuildr->addSharedModel(psharemodel);
	}
	gFBuildr->save();

	//复制材质信息
	this->setActive();
	std::vector<MATID> matid_set;
	_proxySptr->getAllMaterialID(matid_set);
	MaterialSharedPtr pmat;
	for (vector<MATID>::iterator it = matid_set.begin();it != matid_set.end();it++)
	{
		this->setActive();
		pmat = _proxySptr->getMaterial(*it);
		gFBuildr->setActive();
		gFBuildr->addMaterial(pmat);
	}
	gFBuildr->save();

	//复制拓扑信息
	this->setActive();
	std::vector<TopoItemSharedPtr> topo_nodes;
	std::vector<TopoInfoSharedPtr> topo_infos;
	_proxySptr->getAllTopos(topo_nodes);
	gFBuildr->setActive();
	gFBuildr->addTopos(topo_nodes);
	topo_nodes.clear();

	_proxySptr->getAllTopos(topo_infos);
	gFBuildr->setActive();
	gFBuildr->addTopos(topo_infos);
	topo_infos.clear();


	gFBuildr->save();
	return gFBuildr;
}


/**  将工程平移(dx,dy,dz)
* @param dx
* @param dy
* @param dz
* @return 无
*/
void Builder::translate(double dx, double dy , double dz) {
	//通过界面交互完成平移向量的设定
	Matrix4x4 m4x4 = Matrix4x4::IDENTITY4X4;
	Matrix4x4 m_matrixTransForm;//本次平移旋转缩放的变换累计矩阵
	Vertex3d offset;
	vector<FeatureSharedPtr> fobjs;//要素对象
	std::map<FCLSID,FeatureClassSharedPtr> & fclsMap = getFeatureClassMap();
	for(std::map<FCLSID,FeatureClassSharedPtr>::iterator mit = fclsMap.begin();mit!=fclsMap.end();mit++)
	{

		fobjs.clear();
		getProxy()->queryByFeatureClassID(mit->first,fobjs);

		offset.x = dx;
		offset.y = dy;
		offset.z = dz;

		m4x4.buildTranslateMatrix(offset);
		for (vector<FeatureSharedPtr>::iterator it3 = fobjs.begin();it3 != fobjs.end();it3++ ){
			//(*it3)->getGeometry()->TranslateFrom(m4x4);
			// 如果Geometry存在，则执行平移 [孙卡 2010-8-19]
			GeometrySharedPtr pGeom = (*it3)->getGeometry();
			if (pGeom)
			{								
				if (pGeom->getType()==GEOMTYPE_3D_REFERENCE)
				{
					Reference3d *pRef =0;
					if(pGeom->queryInterface(GEOMTYPE_3D_REFERENCE,(void**)&pRef)){
						MDLID mdlID = pRef->getSharedModelID();
						SharedModelSharedPtr pLinkModel = _proxySptr->getSharedModel(mdlID);
						pRef->setSharedModel(pLinkModel);
					}					
				}
				else if (pGeom->getType()==GEOMTYPE_3D_GROUP)
				{
					// 只往里面进一层，不再向里面进了，数据做得很垃圾，个人认为ref是不能作为group成员的 [孙卡 2010-8-20]
					Group3d  *pGroup = 0;

					if(pGeom->queryInterface(GEOMTYPE_3D_GROUP,(void**)&pGroup)){
						vector<Geometry*> geoCollection ;
						pGroup->getElements(geoCollection);
						for (vector<Geometry*>::iterator itr = geoCollection.begin();itr!=geoCollection.end();itr++){
							if ((*itr)->getType()==GEOMTYPE_3D_REFERENCE)
							{
								Reference3d *pRef = 0; 
								if((*itr)->queryInterface(GEOMTYPE_3D_REFERENCE,(void**)&pRef)){
									MDLID mdlID = pRef->getSharedModelID();
									SharedModelSharedPtr pLinkModel = _proxySptr->getSharedModel(mdlID);
									pRef->setSharedModel(pLinkModel);
								}								
							}
						}
					}
				}
				pGeom->translateFrom(m4x4);
			}
			modifyFeature(*it3);

			//平移几何语意信息
			TopoInfoSharedPtr tip;
			getProxy()->queryTopo((*it3)->getID(),TOPOINFOTYPE_SEMATIC_GEOMETRY,tip);
			SemanticGeometryTopoInfo * sgi = dynamic_cast<SemanticGeometryTopoInfo*>(tip.get());
			if(sgi){
				GeometrySharedPtr pGeom = sgi->getGeometry();
				if (pGeom)
				{								
					if (pGeom->getType()==GEOMTYPE_3D_REFERENCE)
					{
						Reference3d *pRef =0;
						if(pGeom->queryInterface(GEOMTYPE_3D_REFERENCE,(void**)&pRef)){
							MDLID mdlID = pRef->getSharedModelID();
							SharedModelSharedPtr pLinkModel = _proxySptr->getSharedModel(mdlID);
							pRef->setSharedModel(pLinkModel);
						}					
					}
					else if (pGeom->getType()==GEOMTYPE_3D_GROUP)
					{
						// 只往里面进一层，不再向里面进了，数据做得很垃圾，个人认为ref是不能作为group成员的 [孙卡 2010-8-20]
						Group3d *pGroup =0;// (C3DGroup*)pGeom;
						if(pGeom->queryInterface(GEOMTYPE_3D_GROUP,(void**)&pGroup)){
							vector<Geometry*> geoCollection ;
							pGroup->getElements(geoCollection);
							for (vector<Geometry*>::iterator itr = geoCollection.begin();itr!=geoCollection.end();itr++){
								if ((*itr)->getType()==GEOMTYPE_3D_REFERENCE)
								{
									Reference3d *pRef = 0;//(C3DReference*)(*itr);
									if((*itr)->queryInterface(GEOMTYPE_3D_REFERENCE,(void**)&pRef)){
										MDLID mdlID = pRef->getSharedModelID();
										SharedModelSharedPtr pLinkModel = _proxySptr->getSharedModel(mdlID);
										pRef->setSharedModel(pLinkModel);
									}
								}
							}
						}
					}
					pGeom->translateFrom(m4x4);
				}
				this->deleteTopo((*it3)->getID(),TOPOINFOTYPE_SEMATIC_GEOMETRY);
				this->addTopo(tip);
			}
		}
		save();
	}
	save();
}

end_gdb_namespace
end_gtl_namespace

