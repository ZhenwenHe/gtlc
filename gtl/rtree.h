#pragma once
#include <string>
#include <sstream>
#include <vector>
#include "RTreebase.h"
 
begin_gtl_namespace

#define RTREE_TEMPLATE template<class DATATYPE, class ELEMTYPE, int NUMDIMS, class ELEMTYPEREAL, int TMAXNODES, int TMINNODES>
#define RTREE_QUAL RTree<DATATYPE, ELEMTYPE, NUMDIMS, ELEMTYPEREAL, TMAXNODES, TMINNODES>

template<
	typename DATATYPE, 
	typename ELEMTYPE, 
	int NUMDIMS=3, 
	typename ELEMTYPEREAL = ELEMTYPE,
	int TMAXNODES = 32, 
	int TMINNODES = TMAXNODES / 2,
	// template paramenters for Object class 
	typename T=char,
	typename TRAITS=std::char_traits<T>,
	typename ALLOC=std::allocator<T>
>
class RTree:public RTreeBase<DATATYPE,ELEMTYPE,NUMDIMS,ELEMTYPEREAL,TMAXNODES,TMINNODES,T,TRAITS,ALLOC> {
public:
	virtual std::basic_string<T,TRAITS,ALLOC> getClassName(){
		return "RTree";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
protected:
	typedef RTreeBase<DATATYPE,ELEMTYPE,NUMDIMS,ELEMTYPEREAL,TMAXNODES,TMINNODES,T,TRAITS,ALLOC> _base;
public:
	RTree():RTreeBase(){
	};
	~RTree(){
	};
	//获取根节点
	Node * getRoot(){ return m_root;}

	//插入节点
	void insert(const ELEMTYPE a_min[NUMDIMS],const ELEMTYPE a_max[NUMDIMS] ,const DATATYPE & a_dataId){
		_base::Insert(a_min,a_max, a_dataId);
	}
	void insert(const ELEMTYPE bound[NUMDIMS*2], const DATATYPE & a_dataId){
		ELEMTYPE a_min[NUMDIMS];
		ELEMTYPE a_max[NUMDIMS];
		for (int i=0;i<NUMDIMS;i++){
			a_min[i] = bound[2*i];
			a_max[i] = bound[2*i+1];
		}
		_base::Insert(a_min,a_max, a_dataId);
	}
	void getRootRect(ELEMTYPE a_min[NUMDIMS], ELEMTYPE a_max[NUMDIMS]){
		int index;
		Rect m_rect=m_root->m_branch[0].m_rect;
		for(int i=0;i<NUMDIMS;++i){
			a_min[i]=m_rect.m_min[i];
			a_max[i]=m_rect.m_max[i];
		}

		for(index=1; index < m_root->m_count; ++index){
			Rect rect=m_root->m_branch[index].m_rect;
			for(int i=0;i<NUMDIMS;++i){
				if(a_min[i]>rect.m_min[i])	a_min[i]=rect.m_min[i];
				if(a_max[i]<rect.m_max[i])	a_max[i]=rect.m_max[i];
			}
		}
	}

	DATATYPE insertWeight(){
		//insertWeight(m_root);		
		DATATYPE hit=insertWeight(m_root);
		return hit;
	}

	DATATYPE  insertWeight(Node *a_node){
		DATATYPE hit=0;
		int index;
		if(a_node->IsInternalNode()) {
			for(index=0; index < a_node->m_count; ++index){
				DATATYPE m_weight = insertWeight(a_node->m_branch[index].m_child);
				hit+=m_weight;
				a_node->m_branch[index].m_data=m_weight/RectVolume(&(a_node->m_branch[index].m_rect));
			}			
		}
		else{
			for(index=0; index < a_node->m_count; ++index){
				hit+=a_node->m_branch[index].m_data*RectVolume(&(a_node->m_branch[index].m_rect));
			}
		}
		//DATATYPE weight=hit/(a_node->m_count);
		return hit;
	}



	void remove(const ELEMTYPE a_min[NUMDIMS], const ELEMTYPE a_max[NUMDIMS], const DATATYPE& a_dataId){
		_base::Remove(a_min,a_max,a_dataId);
	}
	void remove(const ELEMTYPE bound[NUMDIMS*2], const DATATYPE& a_dataId){
		ELEMTYPE a_min[NUMDIMS];
		ELEMTYPE a_max[NUMDIMS];
		for (int i=0;i<NUMDIMS;i++){
			a_min[i] = bound[2*i];
			a_max[i] = bound[2*i+1];
		}
		_base::Remove(a_min,a_max,a_dataId);
	}


	int search(const ELEMTYPE a_min[NUMDIMS], const ELEMTYPE a_max[NUMDIMS], bool __cdecl a_resultCallback(DATATYPE a_data, void* a_context), void* a_context){
		return _base::Search(a_min,a_max,a_resultCallback,a_context);
	}
	int search(const ELEMTYPE a_min[NUMDIMS], const ELEMTYPE a_max[NUMDIMS],std::vector<DATATYPE> &a_arrid){
		return _base::Search(a_min,a_max,a_arrid);
	}
	std::pair<unsigned long long , unsigned long long> search(const ELEMTYPE a_min[NUMDIMS], const ELEMTYPE a_max[NUMDIMS],int runtimes){
		for(int i=0;i<runtimes;i++){
			accessedLeafNodeNumber=0;
			accessedInnerNodeNumber=0;
			_base::Search(a_min,a_max,0,0);
		}
		return std::pair<unsigned long long , unsigned long long> (accessedInnerNodeNumber,accessedLeafNodeNumber);
	}
	// bound Search
	int search(const ELEMTYPE bound[NUMDIMS*2],std::set<DATATYPE> &a_arrid){
		ELEMTYPE a_min[NUMDIMS];
		ELEMTYPE a_max[NUMDIMS];
		for (int i=0;i<NUMDIMS;i++){
			a_min[i] = bound[2*i];
			a_max[i] = bound[2*i+1];
		}
		return _base::Search(a_min,a_max,a_arrid);
	}
	int search(const ELEMTYPE bound[NUMDIMS*2],std::vector<DATATYPE> &a_arrid){
		ELEMTYPE a_min[NUMDIMS];
		ELEMTYPE a_max[NUMDIMS];
		for (int i=0;i<NUMDIMS;i++){
			a_min[i] = bound[2*i];
			a_max[i] = bound[2*i+1];
		}
		return _base::Search(a_min,a_max,a_arrid);
	}
	//Line Search
	int search(std::vector<Point>a_line,std::set<DATATYPE> &a_arrid){
		return _base::Search(a_line,a_arrid);
	}
	//Region Search
	int search(std::vector<Point>a_reg,std::vector<DATATYPE> &a_arrid){
		ELEMTYPE a_min[NUMDIMS];
		ELEMTYPE a_max[NUMDIMS];
		std::vector<Point>::iterator itr =a_reg.begin();
		for (int i=0;i<NUMDIMS;i++) {
			a_min[i] =a_max[i]=(*itr).m_pos[i];
		}
		itr++;
		while(itr!=a_reg.end()){
			Point point1 =(*itr);
			for (int i=0;i<NUMDIMS;i++){
				a_min[i] =Min(a_min[i],point1.m_pos[i]);
				a_max[i] =Max(a_max[i],point1.m_pos[i]);
			}
			itr++;
		}
		return _base::Search(a_min,a_max,a_arrid);
	}


	void removeAll(){
		_base::RemoveAll();
	}

	int count(){
		return _base::Count();
	}
	int numbNodes() {
		int s = 0;
		Node * root =m_root;
		numb_nodes(root,s);
		return s;
	}

	void write(std::ostream & f,std::vector<DATATYPE> &a_vecdata){
		_base::write(f,a_vecdata);
	}
	void read(std::istream & f,std::vector<DATATYPE> &a_vecdata){
		_base::read(f,a_vecdata);
	}
	virtual bool writeBuffer(Buffer<char>& buf){
		return _base::writeBuffer(buf);
	}
	virtual bool readBuffer(const Buffer<char> & buf){
		return _base::readBuffer(buf);
	}
protected:
	void numb_nodes(Node * root,int & s){
		if(root){
			if(root->m_level==0){
				s+=(root->m_count+1);
				return ;
			}
			else {
				s+=(root->m_count+1);
				for(int i=0;i<root->m_count;i++){
					numb_nodes((Node*)(root->m_branch[i].m_child),s);
				}
			}
		}
	}
};
#undef RTREE_TEMPLATE
#undef RTREE_QUAL


end_gtl_namespace
