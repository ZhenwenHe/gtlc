#pragma  once
#include "bplustree.h"
#include "trajectories.h"

begin_gtl_namespace
template <
	typename KeyType, 
	typename ObjectType,
	int DIMS,
	typename TraitsType,
	// template paramenters for Object class 
	typename T=char,
	typename TRAITS=std::char_traits<T>,
	typename ALLOC=std::allocator<T>

>
class CompressedBPlusTrees  £ºpublic Object <T,TRAITS,ALLOC> {
{
public:
	virtual std::basic_string<T,TRAITS,ALLOC> getClassName(){
		return "CompressedBPlusTrees";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
	
public:
	typedef BPlusTree< KeyType,  ObjectType,std::pair<KeyType, ObjectType>,std::less<KeyType>,TraitsType,true> bplus_tree;
	typedef typename bplus_tree::pair_type pair_type;
protected:
	bplus_tree  _trees[DIMS];
private:
	struct CompareKey 
	{
		bool operator ()(pair_type  i, pair_type j){
			if(i.first<j.first)
				return true;
			else
				return false;
		}
	} compare_object;
public:
	void insert(const int axis,std::vector<pair_type>& objs){
		if(axis==0)
			std::sort(objs.begin(),objs.end(),compare_object);

		for(std::vector<pair_type>::iterator it = objs.begin();it!=objs.end();it++) 
			_trees[axis].insert(it->first,it->second);
	}

	void insert(const int axis,const KeyType & k, const ObjectType & obj){
		_trees[axis].insert(k,obj);
	}
	void search(const Envelope<KeyType,DIMS> & qe, std::vector<ObjectType> & results){
		bplus_tree::iterator bit,eit;
		std::vector<ObjectType> rs[DIMS];
		int i=0;
		for(i=0;i<DIMS;i++){
			bit = _trees[i].lower_bound(*(qe.getLowerBounds()+i));
			eit = _trees[i].upper_bound(*(qe.getUpperBounds()+i));

			for(;bit!=eit;bit++)
			{
				//if(std::find(rs[i].begin(),rs[i].end(),bit->second)==rs[i].end()){
					rs[i].push_back(bit->second);
				//}
			}
		}
		bool add=true;
		for(std::vector<ObjectType>::iterator it = rs[0].begin();it!=rs[0].end();it++){
			add=true;
			for(i=1;i<DIMS;i++){
				if(std::find(rs[i].begin(),rs[i].end(),*it)==rs[i].end()){
					add=false;
					break;
				}
			}
			if(add)
				results.push_back(*it);
		}
	}
	void getObjectNumber(unsigned long long &objectNumber, unsigned long long & nodeNumber){
		objectNumber=0;
		nodeNumber=0;
		for(int i=0;i<DIMS;i++)
		{
			objectNumber += _trees[i].get_stats().itemcount;
			nodeNumber+=_trees[i].get_stats().nodes();
		}
	}
};

template <
	typename _Key, typename _Data,
    typename _Value = std::pair<_Key, _Data>,
    typename _Compare = std::less<_Key>,
	typename _Traits = btree_default_map_traits<_Key, _Data>,
	bool _Duplicates = false,
	typename _Alloc = std::allocator<_Value> ,
	// template paramenters for Object class 
	typename T=char,
	typename TRAITS=std::char_traits<T>,
	typename ALLOC=std::allocator<T>
>
class CBPlusTree:public BPlusTree< _Key,  _Data, _Value , _Compare , _Traits, _Duplicates ,_Alloc >
{
public:
	virtual std::basic_string<T,TRAITS,ALLOC> getClassName(){
		return "CBPlusTree";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
public:
	inline size_t find(const key_type& key_low,const key_type& key_high,std::list<_Data> & values){
		node *n = root;
        if (!n) return 0;

        while(!n->isleafnode())
        {
            const inner_node *inner = static_cast<const inner_node*>(n);
            int slot = find_lower(inner, key_low);

            n = inner->childid[slot];
        }

        leaf_node *leaf = static_cast<leaf_node*>(n);

        int slot = find_lower(leaf, key_low);

		if(leaf==0)
			return 0;

		for(int i=slot;i<leaf->slotuse;i++){
			if(leaf->slotkey[i]>=key_low && leaf->slotkey[i]<=key_high)
				values.push_back(leaf->slotdata[i]);
		}
		do{
			leaf = leaf->nextleaf;
			if(leaf)
			{
				for(int i=0;i<leaf->slotuse;i++){
					if(leaf->slotkey[i]>=key_low && leaf->slotkey[i]<=key_high)
						values.push_back(leaf->slotdata[i]);
					else
						return values.size();
				}
			}
			else
				return values.size();

		}while(leaf->slotkey[0]<=key_high);
		return values.size();
	}
	
};

template <
	typename KeyType,
	int DIMS,
	typename TraitsType,
	// template paramenters for Object class 
	typename T=char,
	typename TRAITS=std::char_traits<T>,
	typename ALLOC=std::allocator<T>
>
class STCBPlusTrees£ºpublic Object <T,TRAITS,ALLOC> {
public:
	virtual std::basic_string<T,TRAITS,ALLOC> getClassName(){
		return "STCBPlusTrees";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
public:
	typedef MovingSegment<KeyType,DIMS> moving_segment;
	typedef Envelope<KeyType,DIMS>      envelope;	
	typedef std::vector<moving_segment*> moving_sement_ptr_vector;
protected:
	struct Entry{
		unsigned long long identifier;
		KeyType            key;
		KeyType            another;
		moving_segment * object;
		bool isStartEntry(){
			if(key<another)
				return true;
			else
				return false;
		}
		bool isEndEntry(){
			if(key<another)
				return false;
			else
				return true;
		}
		bool operator == (const Entry & i){
			if(i.identifier==identifier)
				return true;
			else
				return false;
		}
	};

	typedef CBPlusTree< KeyType,  Entry ,std::pair<KeyType, Entry>,std::less<KeyType>,TraitsType,true> bplus_tree;
	typedef typename bplus_tree::pair_type pair_type;

	bplus_tree  _trees[DIMS];
public:
	void insert(moving_segment * ps){
		Entry   e ;
		envelope env = ps->getEnvelope();
		for(int i=0;i<DIMS;i++){ 
			e.identifier=ps->getIdentifier();
			e.key = env.getRange(i).first;
			e.another = env.getRange(i).second;
			_trees[i].insert(e.key,e);

			/*e.identifier=ps->getIdentifier();
			e.key = env.getRange(i).second;
			e.another = env.getRange(i).first;
			_trees[i].insert(e.key,e);*/
		}		
	}
	void search(const envelope & qe, std::vector<moving_segment*> & results){// this method may not find the too long segment which length is larger than the envelope's widths
		bplus_tree::iterator bit,eit;
		std::list<Entry> rs[DIMS];
		int i=0;
		for(i=0;i<DIMS;i++){
			_trees[i].find(*(qe.getLowerBounds()+i),*(qe.getUpperBounds()+i),rs[i]);
		}
		bool add=true;
		for(std::list<Entry>::iterator it = rs[0].begin();it!=rs[0].end();it++){
			add=true;
			for(i=1;i<DIMS;i++){
				if(std::find(rs[i].begin(),rs[i].end(),*it)==rs[i].end()){
					add=false;
					break;
				}
			}
			if(add)
				results.push_back(it->object);
		}
	}
	void getObjectNumber(unsigned long long &objectNumber, unsigned long long & nodeNumber){
		objectNumber=0;
		nodeNumber=0;
		for(int i=0;i<DIMS;i++)
		{
			objectNumber += _trees[i].get_stats().itemcount;
			nodeNumber+=_trees[i].get_stats().nodes();
		}
	}
};
end_gtl_namespace