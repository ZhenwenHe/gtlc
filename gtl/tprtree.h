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

#pragma  once
#include "movingenvelope.h"
#include "../third/SpatiotemporalIndex/include/SpatiotemporalIndex.h"
#include "../third/SpatiotemporalIndex/include/TPRTree.h"


begin_gtl_namespace
template <
	typename OBJECTTYPE,
	typename DataType,
	int DIMS=4,
	typename ELEMENT=char,
	typename TRAITS=std::char_traits<ELEMENT>,
	typename ALLOC=std::allocator<ELEMENT> 
>
 class TPRTree:  public Object<ELEMENT,TRAITS,ALLOC> {
	 SpatiotemporalIndex::ISpatiotemporalIndex * _tree;
	 SpatiotemporalIndex::StorageManager::IBuffer *_buffer;
	 SpatiotemporalIndex::IStorageManager * _storageManager;
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "TPRTree";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
 public:
	 typedef MovingEnvelope<DataType,DIMS,ELEMENT,TRAITS,ALLOC> movingenvelope_type;
	 typedef SpatiotemporalIndex::MovingRegion movingregion_type;
	  typedef OBJECTTYPE* object_ptr;

	  class MyVisitor : public SpatiotemporalIndex::IVisitor
	 {
	 public:
		 size_t m_indexIO;
		 size_t m_leafIO;
		 std::vector<object_ptr>   *m_results;
	 public:
		 MyVisitor() : m_indexIO(0), m_leafIO(0),m_results(0) {}

		 void visitNode(const SpatiotemporalIndex::INode& n)
		 {
			 if (n.isLeaf()) m_leafIO++;
			 else m_indexIO++;
		 }

		 void visitData(const SpatiotemporalIndex::IData& d)
		 {
			 //IShape* pS;
			 //d.getShape(&pS);
			 //// do something.
			 //delete pS;

			 // the ID of this data entry is an answer to the query. I will just print it to stdout.
			 //std::cout << d.getIdentifier() << std::endl;
			 m_results->push_back((object_ptr)(d.getIdentifier()));
			 // data should be an array of characters representing a Region as a string.
			 /*OBJECTTYPE* pData = 0;
			 uint32_t cLen = 0;
			 d.getData(cLen, (byte**)&pData);*/
			 /*if(m_results) m_results->push_back(pData);
			 delete[] pData;*/	 
			 
		 }

		 void visitData(std::vector<const SpatiotemporalIndex::IData*>& v) {}
	 } _visitor;

 public:
	 TPRTree(const std::string & baseName,
		 unsigned int pageSize=4096,
		 unsigned int cacheSize=10,
		 double fillFactor=0.7,
		 unsigned int indexCapacity=32,
		 unsigned int leafCapacity=32,
		 double horizon=200.0
		 ){

		std::string  basename=baseName;
		 _storageManager = SpatiotemporalIndex::StorageManager::createNewDiskStorageManager(basename, pageSize);
		 _buffer  = SpatiotemporalIndex::StorageManager::createNewRandomEvictionsBuffer(*_storageManager, cacheSize, false);
		 int64_t  indexIdentifier;
		 _tree = SpatiotemporalIndex::TPRTree::createNewTPRTree(*_buffer, fillFactor, indexCapacity, leafCapacity, DIMS-1, SpatiotemporalIndex::TPRTree::TPRV_RSTAR, horizon, indexIdentifier);
	 }

	 TPRTree(
		 unsigned int cacheSize=10,
		 double fillFactor=0.7,
		 unsigned int indexCapacity=32,
		 unsigned int leafCapacity=32,
		 double horizon=200.0
		 ){

			 _storageManager = SpatiotemporalIndex::StorageManager::createNewMemoryStorageManager();
			 _buffer  = SpatiotemporalIndex::StorageManager::createNewRandomEvictionsBuffer(*_storageManager, cacheSize, false);
			 int64_t  indexIdentifier;
			 _tree = SpatiotemporalIndex::TPRTree::createNewTPRTree(*_buffer, fillFactor, indexCapacity, leafCapacity, DIMS-1, SpatiotemporalIndex::TPRTree::TPRV_RSTAR, horizon, indexIdentifier);
	 }

	 virtual ~TPRTree( ){
		 delete _tree;
		 delete _buffer;
		 delete _storageManager; 
	 }
 public:
	 void insert(const movingenvelope_type & me, object_ptr obj){
		
		 movingregion_type mr= convert_envelope_to_region(me);
		 _tree->insertData(0,0,mr,(SpatiotemporalIndex::id_type)(obj));
	 }
	 void search(const movingenvelope_type & me,std::vector<object_ptr> & results){
		 movingregion_type mr= convert_envelope_to_region(me);
		 _visitor.m_results=&results;
		 _tree->intersectsWithQuery(mr,_visitor);
		 _visitor.m_results=0;
	 }
	 void getObjectNumber(unsigned long long & objects,unsigned long long & nodes){

		 SpatiotemporalIndex::IStatistics * _statistics=0;

		_tree->getStatistics(&_statistics);
		objects = _statistics->getNumberOfData();
		nodes = _statistics->getNumberOfNodes();
		delete _statistics;
	 }
 protected:
	 static movingregion_type convert_envelope_to_region(const movingenvelope_type & me){
		 return movingregion_type(me.getLowPosition(),me.getHighPosition(),me.getLowVelocites(),me.getHighVelocites(),me.start(),me.end(),DIMS-1);
	 }
 };

end_gtl_namespace