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
#include <cstdlib>			// standard C++ includes
#include <math.h>			// math routines
#include "rtree.h"
#include "matrix.h"
#include "sort.h"
#include "envelope.h"
#include "../kmeans/KMlocal.h"

begin_gtl_namespace

template<
	typename OBJTYPE, 
	typename ELEMTYPE, 
	int NUMDIMS,
	typename ELEMTYPEREAL = ELEMTYPE,
	typename  NodeVisitorType=RTreeNodeVisitor<OBJTYPE>,
	int TMAXNODES = 8, 
	int TMINNODES = TMAXNODES / 2,
	// template paramenters for Object class 
	typename T=char,
	typename TRAITS=std::char_traits<T>,
	typename ALLOC=std::allocator<T> >
class CSRTree:public RTree<OBJTYPE,ELEMTYPE,NUMDIMS,ELEMTYPEREAL,NodeVisitorType,TMAXNODES, TMINNODES,T,TRAITS,ALLOC>
{
public:
	virtual std::basic_string<T,TRAITS,ALLOC> getClassName(){
		return "CSRTree";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
public:
	typedef Envelope<ELEMTYPE,NUMDIMS> BoundRect;

	typedef RTree<OBJTYPE,ELEMTYPE,NUMDIMS,ELEMTYPEREAL,NodeVisitorType,TMAXNODES, TMINNODES> BaseClass;
	typedef std::pair<OBJTYPE,BoundRect> Item;//空间对象和该对象的边界矩形,称为空间实体项
	typedef std::vector<Item*> Items;//空间实体项集合,表示聚类后产生的同类空间项集合
	typedef std::vector<std::vector<int>>     ItemsIndexVector;//聚类后产生的结果,vector的元素个数就是聚类分类种数
protected:
	Items       items;//聚类分析前的数据集合
	ItemsIndexVector itemsvector;//聚类分析的结果,其中包含Items的个数即为分类种数
	ELEMTYPE    * centers;//每类的中心点,大小为[/*count*NUMDIMS*/]
protected:
	//本函数供xyzSort调用,主要实现对节点的XYZ扫描排序,排序结果放在index_orders中
	void xyz_sort(const int count, const ELEMTYPE cts[/*count*NUMDIMS*/],std::vector<int>& index_orders)
	{
		int ** iosv = new int* [NUMDIMS];
		int s = count;
		ELEMTYPE * temp = new ELEMTYPE [count];
		ELEMTYPE * sumv  = new ELEMTYPE [NUMDIMS];
		for(int i=0;i<NUMDIMS;i++){
			iosv[i]= new int [count];
			for(int j=0;j<count;j++){
				temp[j]=cts[j*NUMDIMS+i];
				iosv[i][j]=j;
			}
			//对temp中的数据进行排序,排序结果放在isov[i]中
			IndexSorter<ELEMTYPE,int,void*> sorter(temp,iosv[i],count);
			sorter.sortAsce();
			sorter.getResult(iosv[i],&s);
			sumv[i] =0;
			for(int j=0;j<s-1;j++){
				sumv[i]+=(temp[iosv[i][j+1]]-temp[iosv[i][j]]);
			}
		}
		//比较sumv的各个分量大小,去最小的
		ELEMTYPE t = sumv[0];
		s=0;
		for(int i=1;i<NUMDIMS;i++){
			if(t>sumv[i]){
				s=i;
				t=sumv[i];
			}
		}
		//s 为累加和最小的维数分量,则排序后的索引结果应该是isov[s]
		index_orders.resize(count);
		std::copy(iosv[s],iosv[s]+count,index_orders.begin());

		delete [] temp;
		delete [] sumv;
		for(int i=0;i<NUMDIMS;i++){
			delete [] iosv[i];
		}
		delete [] iosv;
	}

	//根据分类后的结果ItemsIndexVector,构建树的最底层,其生成的父节点存放在parents中
	void construct_tree_leaves(const ItemsIndexVector & data, std::vector<Node*> & children){
		for(int i=0;i<data.size();i++){
			int s = data[i].size();
			int node_count  = (s+MAXNODES)/MAXNODES;
			std::vector<Node*> nodes(node_count);
			//处理第0-node_count-2的节点生成
			for(int j=0;j<node_count-1;j++){
				nodes[j] = new Node;
				nodes[j]->m_count=MAXNODES;
				nodes[j]->m_level=0;
				//设置Branch数据项
				for(int m = 0; m<nodes[j]->m_count;m++){
					nodes[j]->m_branch[m].m_data=data[i][j*MAXNODES+m];
					BoundRect & br = items[data[i][j*MAXNODES+m]]->second;
					std::copy(br.getMinVector(),br.getMinVector()+NUMDIMS,nodes[j]->m_branch[m].m_rect.m_min);
					std::copy(br.getMaxVector(),br.getMaxVector()+NUMDIMS,nodes[j]->m_branch[m].m_rect.m_max);	
				}
			}
			//处理第node_count-1的节点生成
			nodes[node_count-1] = new Node;
			nodes[node_count-1]->m_count=s-(node_count-1)*MAXNODES;
			nodes[node_count-1]->m_level=0;
			//设置第node_count-1的节点的Branch数据项
			for(int m = 0; m<nodes[node_count-1]->m_count;m++){
				nodes[node_count-1]->m_branch[m].m_data=data[i][(node_count-1)*MAXNODES+m];
				BoundRect & br = items[data[i][(node_count-1)*MAXNODES+m]]->second;
				std::copy(br.getMinVector(),br.getMinVector()+NUMDIMS,nodes[(node_count-1)]->m_branch[m].m_rect.m_min);
				std::copy(br.getMaxVector(),br.getMaxVector()+NUMDIMS,nodes[(node_count-1)]->m_branch[m].m_rect.m_max);	
			}
			//将节点添加到总列表中
			for(int n =0;n<node_count;n++)
				children.push_back(nodes[n]);
		}
	}
	//通过construct_tree_leaves处理得到的子节点列表构建树,返回根节点
	void construct_tree(std::vector<Node*> & children, std::vector<Node*> & newNodes){
		int oldnodes_count=children.size();
		int newnodes_count = (oldnodes_count+MAXNODES)/MAXNODES;
		std::vector<Node*> nodes(newnodes_count);
		//处理第0-newnodes_count-2的节点生成
		for(int j=0;j<newnodes_count-1;j++){
			nodes[j] = new Node;
			nodes[j]->m_count=MAXNODES;
			nodes[j]->m_level=1;
			//设置Branch数据项
			for(int m = 0; m<nodes[j]->m_count;m++){
				nodes[j]->m_branch[m].m_child=children[j*MAXNODES+m];
				nodes[j]->m_branch[m].m_rect=NodeCover(children[j*MAXNODES+m]);
			}
		}
		//处理第node_count-1的节点生成
		nodes[newnodes_count-1] = new Node;
		nodes[newnodes_count-1]->m_count=oldnodes_count-(newnodes_count-1)*MAXNODES;
		nodes[newnodes_count-1]->m_level=1;
		//设置第newnodes_count-1的节点的Branch数据项
		for(int m = 0; m<nodes[newnodes_count-1]->m_count;m++){
			nodes[newnodes_count-1]->m_branch[m].m_child=children[(newnodes_count-1)*MAXNODES+m];
			nodes[newnodes_count-1]->m_branch[m].m_rect=NodeCover(children[(newnodes_count-1)*MAXNODES+m]);
		}
		//将节点添加到总列表中
		for(int n =0;n<newnodes_count;n++)
			newNodes.push_back(nodes[n]);

		if(newnodes_count>1)
		{
			children.clear();
			children.resize(newNodes.size());
			std::copy(newNodes.begin(),newNodes.end(),children.begin());
			newNodes.clear();
			construct_tree(children,newNodes);
		}
		else{
			m_root= newNodes[0];
			return;
		}
	}

public:
	
	//用于测试是生成原始任意数据
	void randomBounds(int ncount){
		clearItems(items);
		ELEMTYPE d[NUMDIMS];
		std::vector<ELEMTYPE> rs(NUMDIMS);
		for(int i=0;i<ncount;i++){
			for(int k=0;k<NUMDIMS;k++){
				d[k]=rand()%10000;
				rs[k]=rand()%10;
			}
			Item * t= new Item;
			t->first=i;
			t->second=BoundRect(d,rs);
			items.push_back(t);
		}
	}
	void addRandomBounds(const int ncount,BoundRect& br ){
		ELEMTYPE d[NUMDIMS];
		ELEMTYPE d2[NUMDIMS];
		for(int i=0;i<ncount;i++){
			for(int k=0;k<NUMDIMS;k++){
				do{
					d[k]=rand();//%((long)((br.getMaxVector())[k]));
				}while(d[k]<(br.getMinVector())[k] || d[k]>(br.getMaxVector())[k]);
				do{
					d2[k]=rand();//%((long)((br.getMaxVector())[k]));
				}while(d2[k]<(br.getMinVector())[k] || d2[k]>(br.getMaxVector())[k]);

				d[k]=min(d[k],d2[k]);
				d2[k]=max(d[k],d2[k]);
			}
			Item * t= new Item;
			t->first=i;
			t->second=BoundRect(d,d2);
			items.push_back(t);
		}
	}
	BoundRect randomBound(){
		ELEMTYPE d[NUMDIMS];
		std::vector<ELEMTYPE> rs(NUMDIMS);
		
		for(int k=0;k<NUMDIMS;k++){
			d[k]=rand()%10000;
			rs[k]=rand()%500;
		}
		return BoundRect(d,rs);
	}

	Items & getOriginalData(){return items;}

	void clearItems(Items & v){
		for(Items::iterator it = v.begin();it!=v.end();it++){
			delete (*it);//销毁底层的Item对象
		}
		v.clear();
	}
	
	void resizeCenters(int n ){
		if(centers){
			delete [] centers;
			centers=0;
		}
		centers = new ELEMTYPE[n*NUMDIMS];
	}
	
	int getCenters(std::vector<int> & indexs,ELEMTYPE ** cts){
		int s = indexs.size();
		* cts = new ELEMTYPE[s*NUMDIMS];
		for(int i =0;i<s;i++){
			items[indexs[i]]->second.getCenter(*cts+i*NUMDIMS);
		}
		return indexs.size();
	}
	

public:
	CSRTree():RTree(){
		centers=0;
	}
	~CSRTree(){
		if(centers) delete [] centers;
		clearItems(items);
	}
public:	
	/* 通过K-Means聚类分析将数据分为n类,返回聚类的类数 */
	int kmeansClustering(const int k, const Items & dataIn, ItemsIndexVector &  dataOut,ELEMTYPE cts[/*k*NUMDIMS*/] ){


		int dim =NUMDIMS;
		int nptns=dataIn.size();

		KMterm term(500,0,0,0,
			0.1,0.1,3,
			0.5,10,0.95);
		
		KMdata data(dim,nptns);
		//kmUniformPts(data.getPts(),nptns,dim);
		for(int i=0;i<nptns;i++){
			((BoundRect&)(dataIn[i]->second)).getCenter(data[i]);
		}
		data.buildKcTree();
		KMfilterCenters fc(k,data);
		KMlocalLloyds kl(fc,term);
		fc=kl.execute();
		//fc.print();
		//获取每类的中心点
		KMcenterArray ca = fc.getCtrPts();
		for(int i=0;i<k;i++){
			for(int j=0;j<NUMDIMS;j++){
				cts[i*NUMDIMS+j]=ca[i][j];
			}
		}
		//获取每个点属于哪一类的信息
		KMctrIdxArray closeCtr = new KMctrIdx[data.getNPts()];
		double* sqDist = new double[data.getNPts()];
		fc.getAssignments(closeCtr, sqDist);

		
		dataOut.resize(k);
		for(int i=0;i<k;i++){
			dataOut[i].clear();
		}
		for(int i=0;i<data.getNPts();i++){
			dataOut[closeCtr[i]].push_back(i);
		}

		delete [] closeCtr;
		delete [] sqDist;
		
		return 0;
	}
	/* 对ItemsVector 中的每类数据进行XYZ排序*/
	void xyzSort(ItemsIndexVector &  dataInOut ){
		//按照每类的中心点排序
		std::vector<int> iv;
		int k= dataInOut.size();
		xyz_sort(k,centers,iv);
		ItemsIndexVector iiv(k);
		for(int i=0;i<k;i++){
			iiv[i].resize(dataInOut[iv[i]].size());
			std::copy(dataInOut[iv[i]].begin(),dataInOut[iv[i]].end(),iiv[i].begin());
		}
		//对每类中的节点进行排序,也即对iiv[i]中的节点进行排序
		iv.clear();
		for(int i=0;i<k;i++){
			int s = iiv[i].size();//获取第i类的节点个数
			// 构造中心点为坐标
			ELEMTYPE * cts = 0;
			getCenters(iiv[i],&cts);
			xyz_sort(s,cts,iv);
			delete [] cts;
			std::vector<int> t(s);
			for(int j=0;j<s;j++){
				t[j]=iiv[i][iv[j]];
			}
			std::copy(t.begin(),t.end(),iiv[i].begin());
		}
		//将iiv拷贝到dataInOut中
		for(int i=0;i<iiv.size();i++){
			int s = iiv[i].size();
			dataInOut[i].resize(s);
			std::copy(iiv[i].begin(),iiv[i].end(),dataInOut[i].begin());
		}
	}
	/*	通过聚类后的结果反向生成R树	,K为分类种数*/
	void constructTree(int k){	

		resizeCenters(k);
		kmeansClustering(k,items,itemsvector,centers);
		//printClusteringResults();
		xyzSort(itemsvector);
		//printSortResults();
		std::vector<Node*> children,newNodes;
		construct_tree_leaves(itemsvector,children);
		construct_tree(children,newNodes);
	}

	void printClusteringResults(){
		std::cout<<"\n"<<"begin print clustering results:\n";
		int k = itemsvector.size();
		for (int i = 0; i < k; i++) {
			for(int j=0;j<itemsvector[i].size();j++)
			{
				ELEMTYPE * mi=items[itemsvector[i][j]]->second.getMinVector();
				ELEMTYPE * ma=items[itemsvector[i][j]]->second.getMaxVector();
				std::cout	<<i<<" (";
				for(int m=0;m<NUMDIMS;m++){
					std::cout<<centers[i*NUMDIMS+m]<<" ";
				}
				std::cout<< ") "<< items[itemsvector[i][j]]->first<< "" ;

				std::cout<<" ( ";
				for(int m=0;m<NUMDIMS;m++){
					std::cout<<mi[m]<<" ";
					std::cout<<ma[m]<<" ";
				}
				std::cout<< ") "<<endl;

			}
			std::cout<<"\n";
		}
		std::cout<<"\n"<<"end print clustering results!\n";
		getchar();
	}
	void printSortResults(){
		std::cout<<"\n"<<"begin print sort results:\n";
		int k = itemsvector.size();
		ELEMTYPE * cts =new ELEMTYPE[NUMDIMS];
		for (int i = 0; i < k; i++) {
			for(int j=0;j<itemsvector[i].size();j++)
			{
				ELEMTYPE * mi=items[itemsvector[i][j]]->second.getMinVector();
				ELEMTYPE * ma=items[itemsvector[i][j]]->second.getMaxVector();
				items[itemsvector[i][j]]->second.getCenter(cts);
				std::cout	<<i<<" (";
				for(int m=0;m<NUMDIMS;m++){
					std::cout<<cts[m]<<" ";
				}
				std::cout<< ") "<< items[itemsvector[i][j]]->first<< "" ;

				std::cout<<" ( ";
				for(int m=0;m<NUMDIMS;m++){
					std::cout<<mi[m]<<" ";
					std::cout<<ma[m]<<" ";
				}
				std::cout<< ") "<<endl;

			}
			std::cout<<"\n";
		}
		delete [] cts;
		std::cout<<"\n"<<"end print sort results!\n";

	}
};


///*	TreeGrid的单元网格模板类；
//OBJTYPE是空间对象的对象类型,ELEMTYPE是边界矩形的元素类型,
//NUMDIMS是边界矩形的维数或空间对象的维数,ELEMTYPEREAL用于数值计算采用的数据类型,
//NodeVisitorType是树节点访问对象类型,TMAXNODES表示一个节点上最大的分支数,
//TMINNODES表示一个节点上最少必须有TMINNODES个分支。*/
//template<class OBJTYPE, class ELEMTYPE, int NUMDIMS,class ELEMTYPEREAL = ELEMTYPE,
//class  NodeVisitorType=RTreeNodeVisitor<OBJTYPE>,int TMAXNODES = 8, int TMINNODES = TMAXNODES/2>
//class GridCell:public Object{
//public:
//	typedef Envelope<ELEMTYPE,NUMDIMS> BoundRect;
//protected:
//	BoundRect bound;//网格块的边界矩形
//	//网格块对应的CR-Tree二级索引
//	CSRTree<OBJTYPE,ELEMTYPE,NUMDIMS,ELEMTYPEREAL,NodeVisitorType,TMAXNODES, TMINNODES> crtree;
//	std::string  name;//网格块对应的区域模型文件名称或者数据库中的相应的网格块名称
//public:
//
//};
///*	TreeGrid从矩阵类继承,举证元素为GridCell；
//OBJTYPE是空间对象的对象类型,ELEMTYPE是边界矩形的元素类型,
//NUMDIMS是边界矩形的维数或空间对象的维数,ELEMTYPEREAL用于数值计算采用的数据类型,
//NodeVisitorType是树节点访问对象类型,TMAXNODES表示一个节点上最大的分支数,
//TMINNODES表示一个节点上最少必须有TMINNODES个分支。*/
//template<class OBJTYPE, class ELEMTYPE, int NUMDIMS,class ELEMTYPEREAL = ELEMTYPE,
//class  NodeVisitorType=RTreeNodeVisitor<OBJTYPE>,int TMAXNODES = 8, int TMINNODES = TMAXNODES/2>
//class TreeGrid:public Matrix<GridCell<OBJTYPE,ELEMTYPE,NUMDIMS,ELEMTYPEREAL,NodeVisitorType,TMAXNODES, TMINNODES>>
//{
//};
//

end_gtl_namespace