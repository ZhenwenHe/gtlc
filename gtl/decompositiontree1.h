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



#include <stack>
#include <list>
#include <map>
#include <hash_map>
#include "vertex.h"
#include "envelope.h"
#include "fastlist.h"
#include "pointerarray.h"
#include "omp.h"

#if GTL_VERSION<2 

begin_gtl_namespace
enum {
	DT_POINTS = 1,// for moving points
	DT_ENVELOPES = 2// for any moving object which can be represented by envelope
};

template <class OBJECTTYPE, int NUMDIMS>
struct default_decomposition_tree_trait
{
	static const int  capacity = 32;//maximum number permitted in each leaf
	static const int  growth = 1;//growth number of object array of leaf
	static const int  shape = DT_POINTS;
	static const int  velocity = 0;// not use velocity
};



template<typename  OBJECTTYPE,
	typename COORDTYPE/*=double*/,
	int   NUMDIMS/*=4*/,
	typename DTTRAITS /*= default_decomposition_tree_trait*/,
	typename ELEMENT = char,
	typename TRAITS = std::char_traits<ELEMENT>,
	typename ALLOC = std::allocator<ELEMENT>
>
class  DecompositionTree : public Object<ELEMENT, TRAITS, ALLOC>
{
public:
	typedef Envelope<COORDTYPE, NUMDIMS, ELEMENT, TRAITS, ALLOC>                   envelope;
	typedef Vertex<COORDTYPE, NUMDIMS, ELEMENT, TRAITS, ALLOC>                     vertex;
	typedef OBJECTTYPE*                                   object_ptr;
	typedef std::vector<OBJECTTYPE*>                      object_ptr_vector;
	typedef std::vector<envelope>                         envelope_vector;
	typedef COORDTYPE*                                    velocity_ptr;
public:
	virtual std::basic_string<ELEMENT, TRAITS, ALLOC> getClassName(){
		return "DecompositionTree";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){

	}
	virtual void readBuffer(const void *  buffer, size_t count = 0){

	}
	virtual Object * clone() {
		return 0;
	}
public:
	class Entry{
	public:
		inline const uint64_t getIdentifier() const { return identifier; }
		inline const uint64_t getOrder() const { return order; }
		inline const vertex & getCenter() const {
			if (DTTRAITS::shape == DT_POINTS)
				return *(static_cast<const vertex*>(shape));
			else
			{
				vertex v;
				(static_cast<envelope*>(shape))->getCenter(v);
				return v;
			}
		}

		inline const envelope & getEnvelope() const {
			if (DTTRAITS::shape == DT_ENVELOPES)
				return *(static_cast< envelope *>(shape));
			else
			{
				envelope v;
				COORDTYPE * d = v.getLowerBounds();
				for (int i = 0; i<NUMDIMS; i++)
				{
					d[i] = (static_cast<const vertex*>(shape))->get(i);
					d[i] -= std::numeric_limits<COORDTYPE>::epsilon();
				}
				d = v.getUpperBounds();
				for (int i = 0; i<NUMDIMS; i++)
				{
					d[i] = (static_cast<const vertex*>(shape))->get(i);
					d[i] += std::numeric_limits<COORDTYPE>::epsilon();
				}
				return v;
			}

		}
		inline void getEnvelope(const double & t, envelope & e){
			assert(t >= getLastTime());
			if (DTTRAITS::shape == DT_POINTS){
				vertex* ver = static_cast<vertex*>(shape);
				velocity_ptr veo = getVelocities();
				e.setLowerBounds(*ver);
				e.setUpperBound(0, t);
				for (int i = 1; i<NUMDIMS; i++){
					e.setUpperBound(i, (t - ver->get(0)) * (veo[i - 1]) + ver->get(i));
				}

				e.makeValid();
			}
			else {
				e = *((static_cast<envelope*>(shape)));
			}
		}
		inline const double getLastTime() const {
			if (DTTRAITS::shape == DT_POINTS)
				return (static_cast<const vertex*>(shape))->get(0);
			else
				return *((static_cast<envelope*>(shape))->getUpperBounds());
		}
		inline const velocity_ptr getVelocities() const { return (velocity_ptr)velocities; }
		inline  velocity_ptr getVelocities()  { return (velocity_ptr)velocities; }
		inline void setVisited(bool v){
			if (v)
				status |= 0x01;
			else
				status &= 0xfe;
		}
		inline bool isVisited(){
			return status & 0x01 == 1;
		}
		inline OBJECTTYPE * getObject() { return object; }
	public:
		Entry(const vertex & v, OBJECTTYPE * obj, unsigned int len = 0){
			assert(DTTRAITS::shape == DT_POINTS);
			assert(DTTRAITS::velocity == 0);
			object = obj;
			length = len;
			uint64_t id = 0;
			shape = (void*) new vertex(v);
			status = 0;
			identifier = 0;
			order = 0;
			velocities = 0;
		}

		Entry(const vertex & v, const velocity_ptr vv, OBJECTTYPE * obj, unsigned int len = 0){
			assert(DTTRAITS::shape == DT_POINTS);
			assert(DTTRAITS::velocity != 0);

			object = obj;
			length = len;
			uint64_t id = 0;
			shape = (void*) new vertex(v);
			status = 0;
			identifier = 0;
			order = 0;

			velocities = new COORDTYPE[NUMDIMS - 1];
			memcpy(velocities, vv, sizeof(COORDTYPE)* (NUMDIMS - 1));
		}

		Entry(const uint64_t & id, const uint64_t  & ull_order, const vertex & v, OBJECTTYPE * obj, unsigned int len = 0){
			assert(DTTRAITS::shape == DT_POINTS);
			assert(DTTRAITS::velocity == 0);
			object = obj;
			length = len;
			shape = (void*) new vertex(v);
			status = 0;
			identifier = id;
			order = ull_order;
			velocities = 0;
		}

		Entry(const uint64_t & id, const uint64_t  & ull_order, const vertex & v, const velocity_ptr vv, OBJECTTYPE * obj, unsigned int len = 0){
			assert(DTTRAITS::shape == DT_POINTS);
			assert(DTTRAITS::velocity != 0);

			object = obj;
			length = len;
			shape = (void*) new vertex(v);
			status = 0;
			identifier = id;
			order = ull_order;
			velocities = new COORDTYPE[NUMDIMS - 1];
			memcpy(velocities, vv, sizeof(COORDTYPE)* (NUMDIMS - 1));
		}

		Entry(const envelope & v, OBJECTTYPE * obj, unsigned int len = 0){
			assert(DTTRAITS::shape == DT_ENVELOPES);
			assert(DTTRAITS::velocity == 0);
			object = obj;
			length = len;
			uint64_t id = 0;
			shape = (void*) new envelope(v);
			status = 0;
			identifier = 0;
			order = 0;
			velocities = 0;
		}

		Entry(const envelope & v, const velocity_ptr vv, OBJECTTYPE * obj, unsigned int len = 0){
			assert(DTTRAITS::shape == DT_ENVELOPES);
			assert(DTTRAITS::velocity != 0);
			object = obj;
			length = len;
			uint64_t id = 0;
			shape = (void*) new envelope(v);
			status = 0;
			identifier = 0;
			order = 0;
			velocities = new COORDTYPE[(NUMDIMS - 1) * 2];
			memcpy(velocities, vv, sizeof(COORDTYPE)* (NUMDIMS - 1) * 2);
		}

		Entry(const uint64_t & id, const uint64_t  & ullorder, const envelope & v, OBJECTTYPE * obj, unsigned int len = 0){
			assert(DTTRAITS::shape == DT_ENVELOPES);
			assert(DTTRAITS::velocity == 0);
			object = obj;
			length = len;
			uint64_t id = 0;
			shape = (void*) new envelope(v);
			status = 0;
			identifier = id;
			order = ullorder;
			velocities = 0;
		}
		Entry(const uint64_t & id, const uint64_t  & ullorder, const envelope & v, const velocity_ptr vv, OBJECTTYPE * obj, unsigned int len = 0){
			assert(DTTRAITS::shape == DT_ENVELOPES);
			assert(DTTRAITS::velocity != 0);
			object = obj;
			length = len;
			uint64_t id = 0;
			shape = (void*) new envelope(v);
			status = 0;
			identifier = id;
			order = ullorder;
			velocities = new COORDTYPE[(NUMDIMS - 1) * 2];
			memcpy(velocities, vv, sizeof(COORDTYPE)* (NUMDIMS - 1) * 2);
		}

		virtual ~Entry(){
			object = 0;
			length = 0;
			if (DTTRAITS::shape == DT_POINTS)
				delete (vertex*)shape;
			else
				delete (envelope*)shape;
			shape = 0;
			status = 0;

			if (velocities)
				delete[](velocity_ptr) velocities;
		}
		Entry(const Entry& e){
			object = obj;
			length = len;

			if (DTTRAITS::shape == DT_POINTS){
				shape = (void*) new vertex(*((vertex*)e.shape));
				if (e.velocities){
					velocities = new COORDTYPE[NUMDIMS - 1];
					memcpy(velocities, e.velocities, sizeof(COORDTYPE)*(NUMDIMS - 1));
				}
				else{
					velocities = 0;
				}
			}
			else{
				shape = (void*) new envelope(*((envelope*)e.shape));
				if (e.velocities){
					velocities = new COORDTYPE[(NUMDIMS - 1) * 2];
					memcpy(velocities, e.velocities, sizeof(COORDTYPE)*(NUMDIMS - 1) * 2);
				}
				else{
					velocities = 0;
				}
			}
			status = e.status;
			identifier = e.identifier;
			order = e.order;
		}
		Entry operator = (const Entry& e){
			object = obj;
			length = len;
			if (DTTRAITS::shape == DT_POINTS){
				*((vertex*)shape) = *((vertex*)e.shape);
				if (e.velocities){
					memcpy(velocities, e.velocities, sizeof(COORDTYPE)*(NUMDIMS - 1));
				}
			}
			else{
				*((envelope*)shape) = *((envelope*)e.shape);
				if (e.velocities){
					memcpy(velocities, e.velocities, sizeof(COORDTYPE)*(NUMDIMS - 1) * 2);
				}
			}
			status = e.status;
			identifier = e.identifier;
			order = e.order;
			return *this;
		}
	protected:
		void *  shape;
		OBJECTTYPE* object;
		unsigned int length;// the byte size of the object;
		unsigned char status;
		uint64_t identifier;// the id of moving object
		uint64_t order;// the order of the vertex in line
		void * velocities;
	};

	typedef PointerArray<Entry, true, ELEMENT, TRAITS, ALLOC>  entry_ptr_array;
	typedef typename entry_ptr_array::object_ptr           entry_ptr;
	typedef std::vector<entry_ptr>                         entry_ptr_vector;

	struct CompareEntry{
		bool operator()(entry_ptr i, entry_ptr j){
			if (i->getIdentifier()<j->getIdentifier())
				return true;
			else if (i->getIdentifier() == j->getIdentifier())
			{
				if (i->getOrder()<j->getOrder())
					return true;
				else
					return false;
			}
			else
				return false;
		}
	}compare_entry_object;



	struct LeafNode{
		entry_ptr_array *objects;
		uint64_t  identifier;
		LeafNode(){
			objects = 0;
			identifier = 0;
		}
		LeafNode(const LeafNode & p){
			objects = p.objects;
			identifier = p.identifier;
		}
		~LeafNode(){
			if (objects)
				delete objects;
		}
		inline void push_back(entry_ptr p){
			if (objects)
				objects->push_back(p);
			else{
				objects = new entry_ptr_array(DTTRAITS::capacity);
				objects->push_back(p);
			}
		}
		inline size_t size(){
			if (objects == 0)
				return 0;
			else
				return objects->size();
		}
	};


	struct remove_leaf_in_list_predicate
	{
		uint64_t a;
		remove_leaf_in_list_predicate(uint64_t p){
			a = p;
		}

		bool operator() (LeafNode * ln) {
			uint64_t c = ln->identifier;
			if (c <= a) return false;
			while (c != 0)
			{
				c = c >> NUMDIMS;
				if (c<a)
					return false;
				if (c == a)
					return true;
			}
			return false;
		}
	};

	struct Statistics {
		uint64_t splitTimes;
		uint64_t nodeNumber;
		uint64_t objectNumber;
		Statistics(){
			splitTimes = 0;
			nodeNumber = 0;
			objectNumber = 0;
		}
	};

	typedef LeafNode                                                              leaf_node_type;
	typedef typename std::map<uint64_t, LeafNode>                                  map_type;
	typedef typename map_type::iterator                                           map_iterator;
private:
	//the maximum number of entries permitted in a leaf
	unsigned int                            _blockingFactor;
	//the depth of the tree
	int                                     _depth;
	// the total range of the interest region
	envelope                           _total;
	// store the leaf nodes
	map_type                                _directory;
	// store the statistic information 
	Statistics                              _statistics;
	// for the future query, store the last entries at current time
	double                                  _currentTime;
	//the current time entry map
	std::map<uint64_t, entry_ptr >  _currentEntries;

private://temporary variants for improving performance	
	//equal 2^NUMDIM
	int                      _pow_of_dimensions;
	// 2^NUMDIM envelopes
	envelope *           _temp_envelopes;
	//for query 
	std::vector<LeafNode*>   _vector;
public://constructor and deconstructor
	DecompositionTree(const envelope & e)
	{
		_blockingFactor = DTTRAITS::capacity;
		_depth = 1;
		_total = e;
		_pow_of_dimensions = (int)std::pow(2.0, NUMDIMS);
		_directory[1].identifier = 1;
		_temp_envelopes = new envelope[_pow_of_dimensions];
		omp_set_num_threads(4);
	}
	virtual ~DecompositionTree()
	{
		delete[] _temp_envelopes;
	}

public: // inline functions 
	inline map_type & getMap()
	{
		return _directory;
	}

	inline int getDepth()const
	{
		return _depth;
	}
	inline const double getCurrentTime()const { return _currentTime; }
	inline void getObjectNumber(uint64_t & objectnumber, uint64_t & nodenumber)
	{
		nodenumber = _directory.size();
		objectnumber = 0;
		for (map_iterator it = _directory.begin(); it != _directory.end(); it++)
			objectnumber += it->second.size();
	}

	inline void prepare()//prepare the temporary vector
	{
		_vector.resize(_directory.size());
		std::vector<LeafNode*>::iterator itv = _vector.begin();
		for (map_iterator it = _directory.begin(); it != _directory.end(); it++)
		{
			*itv = &(it->second);
			(*itv)->identifier = it->first;
			itv++;
		}
	}
protected:
	/*
	calculate the number identifer of the envelope where the interval belongs in the depath
	*/
	inline uint64_t calculateEnvelopeIdentifer(const vertex& r, int depth)
	{
		envelope total = _total;
		envelope e2d;
		int i = 1;
		uint64_t s = 1;
		while (i<depth){
			for (int j = 0; j<_pow_of_dimensions; j++){
				decompose(total, j, e2d);
				if (e2d.test(r) != 0)	{
					total = e2d;
					s = s << NUMDIMS;
					s += j;
					break;
				}
			}
			i++;
		}
		return s;
	}

	inline uint64_t calculateEnvelopeIdentifer(const envelope & r, const int depth)
	{
		envelope total = _total;
		envelope e2d;
		int i = 1;
		uint64_t s = 1;
		while (i<depth){
			for (int j = 0; j<_pow_of_dimensions; j++){
				decompose(total, j, e2d);
				if (e2d.encloses(r)){
					total = e2d;
					s = s << NUMDIMS;
					s += j;
					break;
				}
			}
			i++;
		}
		return s;
	}

	inline uint64_t calculateEnvelopeIdentifer(const envelope & r, const int depth, int & length)
	{
		envelope total = _total;
		envelope e2d;
		int i = 1;
		uint64_t s = 1;
		length = 1;
		while (i<depth){
			for (int j = 0; j<_pow_of_dimensions; j++){
				decompose(total, j, e2d);
				if (e2d.encloses(r)){
					total = e2d;
					s = s << NUMDIMS;
					length++;
					s += j;
					break;
				}
			}
			i++;
		}
		return s;
	}

	/*
	calculate the depth of the number identifer
	*/
	inline int calculateIdentiferDepth(uint64_t code)
	{
		int i = 0;
		while (code != 0)
		{
			code = code >> NUMDIMS;
			i++;
		}
		return i;
	}

	/*
	calculate the maximum envelope at maximum depth
	*/
	inline void calculateEnvelope(uint64_t code, envelope & e2d)
	{
		if (code == 1)
		{
			e2d = _total;//envelope(_minmumValue,_maximumValue,_minmumValue,_maximumValue);
			return;
		}
		std::stack<unsigned char> sk;
		envelope total = _total;
		while (code != 1)
		{
			sk.push((unsigned char)(code & 0xf));//Ê®Áù½øÖÆµÄ×îºóÒ»Î»
			code = code >> NUMDIMS;
		}
		while (!sk.empty())
		{
			decompose(total, sk.top(), e2d);
			total = e2d;
			sk.pop();
		}
	}

	inline uint64_t calculateEnvelopeIdentifer(const entry_ptr & obj, const int depth){
		if (DTTRAITS::shape == DT_POINTS){
			return  calculateEnvelopeIdentifer(obj->getCenter(), depth);
		}
		else{
			return  calculateEnvelopeIdentifer(obj->getEnvelope(), depth);
		}
	}

	//a is a prefix of b ? ÅÐ¶ÏaÊÇ·ñÊÇcµÄÇ°×º
	inline bool isPrefix(const uint64_t   a, uint64_t   c){
		if (c <= a) return false;
		while (c != 0)
		{
			c = c >> NUMDIMS;
			if (c<a)
				return false;
			if (c == a)
				return true;
		}
		return false;
	}

	inline bool isPrefix(const uint64_t  & a, const uint64_t  &b) const{
		if (b <= a) return false;
		uint64_t c = b;
		while (c != 0)
		{
			c = c >> NUMDIMS;
			if (c<a)
				return false;
			if (c == a)
				return true;
		}
		return false;
	}

	/*
	if a is a prefix of b, return 1;
	if a==b , return 0;
	if b is a prefix of a, return -1;

	return -2;
	*/
	inline int test(const uint64_t  & a, const uint64_t  &b) const{
		if (a == b) return 0;
		uint64_t c;
		if (a>b)
		{
			c = a;

			while (c != 0)
			{
				c = c >> NUMDIMS;
				if (c == b)
					return -1;
				if (c<b)
					return -2;
			}

		}
		else
		{
			c = b;
			while (c != 0)
			{
				c = c >> NUMDIMS;
				if (c == a)
					return 1;

				if (c<a)
					return -2;
			}
		}
		return -2;
	}

	//ÅÐ¶ÏmapÖÐÊÇ·ñ´æÔÚÇ°×ºÎªprefixµÄ¶ÔÏó´æÔÚ
	inline bool hasPrefix(const uint64_t & prefix){
		for (map_iterator it = _directory.begin(); it != _directory.end(); it++)
		{
			if (isPrefix(prefix, it->first))
				return true;
		}
		return false;
	}

	inline uint64_t parentIdentifer(const uint64_t &  code)
	{
		return code >> NUMDIMS;
	}

	inline uint64_t getPrefix(const uint64_t &  code, int n)
	{
		return code >> (NUMDIMS*(calculateIdentiferDepth(code) - n));
	}

	/*
	divide the envelope p into _pow_of_dimensions envelopes
	*/
	inline void  decompose(const envelope& p, envelope es[])
	{
		for (int i = 0; i<_pow_of_dimensions; i++){
			decompose(p, i, es[i]);
		}
	}

	/*
	divide the envelope p into _pow_of_dimensions envelopes, and get the envelope located in index
	*/
	inline void decompose(const envelope& p, int index, envelope & e)
	{
		uint64_t mark = 1;
		for (int i = 0; i<NUMDIMS; i++)
		{
			if (index & mark)
			{
				e.minData[i] = (p.minData[i] + p.maxData[i]) / 2;
				e.maxData[i] = p.maxData[i];
			}
			else{
				e.minData[i] = p.minData[i];
				e.maxData[i] = (p.minData[i] + p.maxData[i]) / 2;
			}
			mark = mark << 1;
		}
	}
	inline int test(const envelope & e, const entry_ptr & obj){

		if (DTTRAITS::shape == DT_POINTS){
			return e.test(obj->getCenter());
		}
		else{
			return e.test(obj->getEnvelope());
		}
	}

protected://slipt functions
	inline void split_for_envelope(leaf_node_type& it, entry_ptr pi){

		uint64_t code = it.identifier;
		envelope total;
		calculateEnvelope(code, total);
#ifdef  _GTL_DEBUG_
		assert(total.isValid());
#endif
		bool repeated;
		int k;
		entry_ptr_array * pa = it.objects;
		map_type subnodes;
		leaf_node_type * lnt = 0;
		envelope temp_envelope;

		bool enclosed = false;
		double temp_value1, temp_value2;
		int temp_k;

#ifdef  _GTL_DEBUG_
		int repeated_count = 0;
#endif
		do {

#ifdef  _GTL_DEBUG_
			gtl::trace("repeated number = %d\n", repeated_count);
			assert(repeated_count<15);
			assert(total.isValid());
#endif

			decompose(total, _temp_envelopes);

			for (size_t i = 0; i<pa->size(); i++)
			{
				temp_envelope = pa->get(i)->getEnvelope();

#ifdef  _GTL_DEBUG_
				//assert(temp_envelope.isValid());
#endif
				enclosed = false;
				temp_k = -1;
				temp_value2 = 0.0;
				for (k = 0; k<_pow_of_dimensions; k++)
				{

					//如果完全包含
					if (_temp_envelopes[k].encloses(temp_envelope))
					{
						lnt = &(subnodes[(code << NUMDIMS) + k]);
						lnt->identifier = (code << NUMDIMS) + k;
						lnt->push_back(pa->get(i));
						lnt = 0;
						enclosed = true;
						break;
					}
					else if (temp_envelope.encloses(_temp_envelopes[k]))
					{
						lnt = &(subnodes[(code << NUMDIMS) + k]);
						lnt->identifier = (code << NUMDIMS) + k;
						lnt->push_back(pa->get(i));
						lnt = 0;
						enclosed = true;
						break;
					}
					else {//如果部分相交，计算相交矩形最大的节点，并加入
						if (_temp_envelopes[k].overlaps(temp_envelope, temp_value1)){
							if (temp_value2<temp_value1){
								temp_k = k;
								temp_value2 = temp_value1;
							}
						}
					}
				}

				//加入相交矩形最大的节点
				if (enclosed == false && temp_k>0){
					lnt = &(subnodes[(code << NUMDIMS) + temp_k]);
					lnt->identifier = (code << NUMDIMS) + temp_k;
					lnt->push_back(pa->get(i));
					lnt = 0;
				}
			}
			//如果全部加入到了一个节点中，则重复分裂
			if (subnodes.size() == 1){
				code = subnodes.begin()->first;
				pa = subnodes.begin()->second.objects;
				subnodes.begin()->second.objects = 0;
				calculateEnvelope(code, total);
				subnodes.clear();
				repeated = true;
			}
			else
				repeated = false;

#ifdef  _GTL_DEBUG_
			repeated_count++;
#endif

		} while (repeated);

		//添加pi对象
		temp_envelope = pi->getEnvelope();
		temp_value2 = 0.0;
		enclosed = false;
		for (k = 0; k<_pow_of_dimensions; k++)
		{
			//如果完全包含
			if (_temp_envelopes[k].encloses(temp_envelope) || temp_envelope.encloses(_temp_envelopes[k]))
			{
				subnodes[(code << NUMDIMS) + k].push_back(pi);
				enclosed = true;
				break;
			}
			else {//如果部分相交，计算相交矩形最大的节点，并加入
				if (_temp_envelopes[k].overlaps(temp_envelope, temp_value1)){
					if (temp_value2<temp_value1){
						temp_k = k;
						temp_value2 = temp_value1;
					}
				}
			}
		}
		if (enclosed == false)
		{
			subnodes[(code << NUMDIMS) + temp_k].push_back(pi);
		}

		_depth = max(calculateIdentiferDepth(code << NUMDIMS), _depth);

		for (map_type::iterator its = subnodes.begin(); its != subnodes.end(); its++)
		{
			lnt = &(_directory[its->first]);
			lnt->identifier = its->first;
			lnt->objects = its->second.objects;
			its->second.objects = 0;
			lnt = 0;
		}
	}

	inline void split_for_point(leaf_node_type& it, entry_ptr pe){
		_statistics.splitTimes++;

		uint64_t code = it.identifier;
		envelope total;

		bool repeated;
		int k, i, j;
		entry_ptr_array * pa = it.objects;
		it.objects = 0;

		map_type subnodes;
		leaf_node_type * lnt = 0;
		calculateEnvelope(code, total);

		do {
			repeated = false;
			decompose(total, _temp_envelopes);
			for (k = 0; k<_pow_of_dimensions; k++)
			{
				for (i = 0; i<pa->size(); i++)
				{
					if (pa->get(i)->isVisited()) continue;

					if (test(_temp_envelopes[k], pa->get(i)) != 0)
					{
						if (lnt == 0)
							lnt = &(_directory[(code << NUMDIMS) + k]);

						lnt->identifier = (code << NUMDIMS) + k;
						lnt->push_back(pa->get(i));
						pa->get(i)->setVisited(true);
					}
				}

				if (lnt) //there are some objects which have been inserted into _directory[(code<<NUMDIMS)+k]
				{
					if (pa->size() == lnt->size()){//if all of the objects have been inserted into a same node _directory[(code<<NUMDIMS)+k];

						if (pe->isVisited() == false)// if pe have not yet been inserted into _directory
						{
							// and if the pe should be inserted into the same node too, we should repeat 
							if (test(_temp_envelopes[k], pe) != 0)
							{
								repeated = true;
								pa = lnt->objects;
								lnt->objects = 0;
								code = (code << NUMDIMS) + k;
								_directory.erase(_directory.find(code));
								total = _temp_envelopes[k];

								for (j = 0; j<pa->size(); j++)
								{
									pa->get(j)->setVisited(false);
								}

								lnt = 0;
								break;
							}
							else // the pe does not belong to the node _directory[(code<<NUMDIMS)+k]
							{
								repeated = false;

								for (j = k + 1; j<_pow_of_dimensions; j++)
								{
									if (test(_temp_envelopes[j], pe) != 0)
									{
										lnt = &(_directory[(code << NUMDIMS) + j]);
										lnt->push_back(pe);
										_statistics.objectNumber++;
										return;
									}
								}
								return;
							}
						}
						else{// pe has been inserted into one of the nodes[0,k-1]
							return;
						}

					}
					else{//test whether pe should be inserted into the node _directory[(code<<NUMDIMS)+k]
						if (pe->isVisited() == false)
						{
							if (test(_temp_envelopes[k], pe) != 0)
							{
								lnt->push_back(pe);
								_statistics.objectNumber++;
								pe->setVisited(true);
							}
						}
						repeated = false;
					}
				}
				else//there are no objects which have been inserted into _directory[(code<<NUMDIMS)+k]
				{
					//judge whether pe should be inserted into  _directory[(code<<NUMDIMS)+k]
					if (pe->isVisited() == false)
					{
						if (test(_temp_envelopes[k], pe) != 0)
						{
							lnt = &(_directory[(code << NUMDIMS) + k]);
							lnt->push_back(pe);
							_statistics.objectNumber++;
							pe->setVisited(true);
						}
					}
				}

				lnt = 0;
			}
			/*decompose(total,_temp_envelopes);
			for(size_t i =0;i<pa->size();i++)
			{
			for( k=0;k<_pow_of_dimensions;k++)
			{
			if(test(_temp_envelopes[k],pa->get(i))!=0)
			{
			lnt =&(subnodes[(code<<NUMDIMS)+k]);
			lnt->identifier=(code<<NUMDIMS)+k;
			lnt->push_back(pa->get(i));
			lnt=0;
			break;
			}
			}
			}
			if(subnodes.size()==1){
			code = subnodes.begin()->first;
			pa = subnodes.begin()->second.objects;
			subnodes.begin()->second.objects=0;
			calculateEnvelope(code,total);
			subnodes.clear();
			repeated =true;
			}
			else
			repeated = false;*/
		} while (repeated);

		/*for( k=0;k<_pow_of_dimensions;k++)
		{
		if(test(_temp_envelopes[k],pe )!=0)
		{
		subnodes[(code<<NUMDIMS)+k].push_back(pe);
		break;
		}
		}		*/
		_depth = max(calculateIdentiferDepth(code << NUMDIMS), _depth);

		/*for(map_type::iterator its = subnodes.begin();its!=subnodes.end();its++)
		{
		lnt = & (_directory[its->first]);
		lnt->identifier = its->first;
		lnt->objects = its->second.objects;
		its->second.objects=0;
		lnt=0;
		}*/

		//restore the visited flags
		for (i = 0; i<pa->size(); i++)
		{
			pa->get(i)->setVisited(false);
		}
		pe->setVisited(false);
		pa->removeAll();//avoiding repeated deletions
	}
public: //insertion functions
	/*inline bool insert(OBJECTTYPE * pi) {
	if(DTTRAITS::shape==DT_POINTS){
	return insertPoint(new Entry(pi->getCenter(),pi));
	}
	else{
	return insertEnvelope(new Entry(pi->getEnvelope(),pi));
	}
	}
	inline bool insert(const vertex & v, OBJECTTYPE * pi, const unsigned int len=0){
	assert(DTTRAITS::shape == DT_POINTS);
	return insertPoint(new Entry(v,pi,len));
	}
	*/


	/*inline bool insert(const envelope & e, OBJECTTYPE * pi, const unsigned int len=0){
	assert(DTTRAITS::shape == DT_ENVELOPES);
	return insertEnvelope(new Entry(pi->getEnvelope(),pi,len));
	}*/
	inline bool insert(const uint64_t & id, const uint64_t & order,
		const vertex & v, OBJECTTYPE * pi, const unsigned int len = 0){
		assert(DTTRAITS::shape == DT_POINTS);
		return insertPoint(new Entry(id, order, v, pi, len));
	}

	inline bool insert(const uint64_t & id, const uint64_t & order, const envelope & e, OBJECTTYPE * pi, const unsigned int len = 0){
		assert(DTTRAITS::shape == DT_ENVELOPES);
		return insertEnvelope(new Entry(id, order, pi->getEnvelope(), pi, len));
	}

	inline bool insert(const uint64_t & id, const uint64_t & order,
		const vertex & v, const velocity_ptr vv, OBJECTTYPE * pi, const unsigned int len = 0){

		assert(DTTRAITS::shape == DT_POINTS && DTTRAITS::velocity);
		return insertPoint(new Entry(id, order, v, vv, pi, len));
	}

	inline bool insert(const uint64_t & id, const uint64_t & order,
		const envelope & e, const velocity_ptr vv,
		OBJECTTYPE * pi, const unsigned int len = 0){
		assert((DTTRAITS::shape == DT_ENVELOPES) && (DTTRAITS::velocity != 0));
		return insertEnvelope(new Entry(id, order, pi->getEnvelope(), vv, pi, len));
	}
protected:
	bool insertPoint(entry_ptr pi)
	{

		if (_currentTime<pi->getLastTime())
		{
			_currentTime = pi->getLastTime();
			_currentEntries[pi->getIdentifier()] = pi;
		}

		entry_ptr_array * pat = 0;
		uint64_t code = calculateEnvelopeIdentifer(pi, _depth);
		uint64_t old_code = code;
		map_iterator it;
		leaf_node_type split_node;
		while (code != 0){
			it = _directory.find(code);
			if (it != _directory.end())
			{
				if (it->second.size()<_blockingFactor)
				{
					it->second.push_back(pi);
					_statistics.objectNumber++;
					return true;
				}
				else
				{
					split_node.identifier = it->first;
					split_node.objects = it->second.objects;
					it->second.objects = 0;
					_directory.erase(it);
					split_for_point(split_node, pi);
					return true;
				}
			}
			else
				code = code >> NUMDIMS;
		}
		//Èç¹ûÖ´ÐÐÏÂÃæµÄ´úÂë£¬ÔòËµÃ÷codeµÄËùÓÐÇ°×º¼°Æä±¾ÉíÔÚ_directoryÖÐ¶¼²»´æÔÚ£¬ÐèÒªÌí¼ÓÐÂµÄÒ¶×Ó½Úµã
		int i = 1;
		code = old_code;
		int dep = calculateIdentiferDepth(code);
		uint64_t  prefix;
		do{
			prefix = getPrefix(code, i);
			if (hasPrefix(prefix))
			{
				i++;
			}
			else
				break;
		} while (i <= dep);
		_directory[prefix].push_back(pi);
		_statistics.objectNumber++;
		return false;
	}

	bool insertEnvelope(entry_ptr pi)
	{

		if (_currentTime<pi->getLastTime())
		{
			_currentTime = pi->getLastTime();
			_currentEntries[pi->getIdentifier()] = pi;
		}


#ifdef _GTL_DEBUG_
		envelope e = pi->getEnvelope();
		//assert(e.isValid());
#endif //_GTL_DEBUG_
		entry_ptr_array * pat = 0;
		uint64_t code = calculateEnvelopeIdentifer(pi, _depth);
		uint64_t old_code = code;
		map_iterator it;
		leaf_node_type split_node;
		//1.¼ì²écodeµÄËùÓÐÇ°×º¼°Æä±¾ÉíÔÚ_directoryÖÐÊÇ·ñ´æÔÚ£¬Èç¹û´æÔÚÔò²åÈë
		while (code != 0){
			it = _directory.find(code);
			if (it != _directory.end())
			{
				if (it->second.size()<_blockingFactor)
				{
					it->second.push_back(pi);
					return true;
				}
				else
				{
					split_node.identifier = it->first;
					split_node.objects = it->second.objects;
					it->second.objects = 0;
					_directory.erase(it);
					split_for_envelope(split_node, pi);
					return true;
				}
			}
			else
				code = code >> NUMDIMS;
		}
		//2.ÅÐ¶ÏÊÇ·ñ´æÔÚÒÔcodeÎªÇ°×ºµÄ½Úµã´æÔÚ£¬Èç¹ûÓÐ£¬ÕÒµ½ËùÓÐÒÔcodeÎªÇ°×ºµÄ½Úµã£¬
		//Ñ¡È¡ÓÚ²åÈë¾ØÐÎÖØµþ·¶Î§×î´óµÄ½Úµã²åÈë£¬Èç¹ûÐèÒª·Ö½âÔò·Ö½â
		code = old_code;
		envelope env_it;
		for (map_iterator it = _directory.begin(); it != _directory.end(); it++)
		{
			if (isPrefix(code, it->first))
			{
				calculateEnvelope(it->first, env_it);
				if (test(env_it, pi) != 0)
				{
					if (it->second.size()<_blockingFactor)
					{
						it->second.push_back(pi);
						return true;
					}
					else
					{
						split_node.identifier = it->first;
						split_node.objects = it->second.objects;
						it->second.objects = 0;
						_directory.erase(it);
						split_for_envelope(split_node, pi);
						return true;
					}
				}
			}
		}

		//3.ÐèÒªÌí¼ÓÐÂµÄÒ¶×Ó½Úµã
		int i = 1;
		code = old_code;
		int dep = calculateIdentiferDepth(code);
		uint64_t  prefix;
		do{
			prefix = getPrefix(code, i);
			if (hasPrefix(prefix))
			{
				i++;
			}
			else
				break;
		} while (i <= dep);
		_directory[prefix].push_back(pi);
		return true;
	}
public: // functions for inserting the external object,when object is not in the rectangle _total.
	void insertExternalObject(entry_ptr p)
	{
		if (_currentTime<pi->getLastTime())
		{
			_currentTime = pi->getLastTime();
			_currentEntries[pi->getIdentifier()] = pi;
		}

		int index = 0;
		std::stack<int> s;
		envelope new_root_rect;
		envelope old_root_rect = _total;
		vertex root_center;
		vertex object_center = p->getCenter();
		COORDTYPE new_distance = 0;
		COORDTYPE old_distance = 0;
		//1. calculate the root node code in a new tree 
		bool run = true;
		do {
			for (int i = 0; i<_pow_of_dimensions; i++)
			{
				calculateRootRectangle(old_root_rect, i, new_root_rect);
				if (new_root_rect.test(object_center) != 0)
				{
					s.push(i);
					run = false;
					break;
				}
				else
				{
					new_root_rect.getCenter(root_center);
					new_distance = vertex::calculateDistance(root_center, object_center);
					if (old_distance< new_distance)
					{
						old_distance = new_distance;
						index = i;
					}
				}
			}
			s.push(index);
		} while (run);

		//the old_root code in the new tree is 
		uint64_t code = 1;
		_depth += s.size();
		while (!s.empty())
		{
			code = code << NUMDIMS;
			code += s.top();
			s.pop();
		}
		changeRootCode(code);
		insert(p);
	}

	void changeRootCode(uint64_t new_code)
	{
		//1.change the basic rectangle
		envelope new_root_rectangle;
		calculateRootRectangle(_total, new_code, new_root_rectangle);
		_total = new_root_rectangle;
		//2.calculate the number of the digits of new_code;
		int number = calculateIdentiferDepth(new_code);
		//3.cut off the first left digit, it must be 1
		uint64_t code = 1;
		int i = 1;
		while (i<number){
			code = code << NUMDIMS;
			i++;
		}
		new_code = new_code - code;
		//4.insert the digits of new_code into every code of leaves at the beginning of the second position
		//for exmaple, if the new_code is 0xab, the leaf code is 0x1c3f, the result will be 0x1abc3f
		int len = 0;
		for (map_iterator it = _directory.begin(); it != _directory.end(); it++)
		{
			code = it->first;
			len = calculateIdentiferDepth(code);
			code = 1;
			for (i = 1; i<len; i++){
				code = code << NUMDIMS;
			}
			code = it->first - code;
			for (i = 1; i<len; i++)
			{
				new_code = new_code << NUMDIMS;
			}
			new_code = new_code + code;
			len = len + number - 1;
			code = 1;
			for (i = 1; i<len; i++)
				code = code << NUMDIMS;
			it->first = new_code + code;
		}

	}
	void calculateRootRectangle(const envelope & e, uint64_t code, envelope & root)
	{
		envelope   subrect = e;
		while (code != 1){
			uint64_t mark = 1;
			int index = code - (code >> NUMDIMS) << NUMDIMS;
			for (int i = 0; i<NUMDIMS; i++)
			{
				if (index & mark)
				{
					root.maxData[i] = subrect.maxData[i];
					root.minData[i] = subrect.minData[i] * 2 - root.maxData[i];
				}
				else{
					root.minData[i] = subrect.minData[i];
					root.maxData[i] = 2 * e.maxData[i] - root.minData[i];
				}
				mark = mark << 1;
			}
			code = code >> NUMDIMS;
			subrect = root;
		}
	}

	void calculateRootRectangle(const envelope & subrect, int index, envelope & root)
	{
		uint64_t mark = 1;
		for (int i = 0; i<NUMDIMS; i++)
		{
			if (index & mark)
			{
				root.maxData[i] = subrect.maxData[i];
				root.minData[i] = subrect.minData[i] * 2 - root.maxData[i];
			}
			else{
				root.minData[i] = subrect.minData[i];
				root.maxData[i] = 2 * e.maxData[i] - root.minData[i];
			}
			mark = mark << 1;
		}
	}

public:// query functions
	inline void search(const envelope & e2d, object_ptr_vector & res, bool parallel = false){
		std::vector<entry_ptr>   results;
		if (e2d.getUpperBound(0) <= _currentTime){ //query for past data		
			if (parallel)
				search_parallel_impl_vector(e2d, results);
			else
				search_single_thread_list(e2d, results);
		}
		else if (e2d.getLowerBound(0) >= _currentTime){//query for future
			search_current_future(e2d, results);
		}
		else{// mixed query
			assert(false);
		}

		res.resize(results.size());
		for (size_t it = 0; it<results.size(); it++)
		{
			res[it] = results[it]->getObject();
		}
	}
	inline void search(const envelope & e2d, entry_ptr_vector & results, bool parallel = false){

		if (e2d.getUpperBound(0) <= _currentTime){ //query for past data		
			if (parallel)
				search_parallel_impl_vector(e2d, results);
			else
				search_single_thread_list(e2d, results);
		}
		else if (e2d.getLowerBound(0) >= _currentTime){//query for future
			search_current_future(e2d, results);
		}
		else{// mixed query
			assert(false);
		}

	}

	inline void search(const envelope & e2d,
		std::map<uint64_t, object_ptr_vector >& res,
		bool parallel = false)
	{
		std::vector<entry_ptr>   results;
		if (e2d.getUpperBound(0) <= _currentTime){ //query for past data		
			if (parallel)
				search_parallel_impl_vector(e2d, results);
			else
				search_single_thread_list(e2d, results);
		}
		else if (e2d.getLowerBound(0) >= _currentTime){//query for future
			search_current_future(e2d, results);
		}
		else{// mixed query
			assert(false);
		}

		std::sort(results.begin(), results.end(), compare_entry_object);

		for (size_t it = 0; it<results.size(); it++)
		{
			res[results[it]->getIdentifier()].push_back(results[it]->getObject());
		}
	}
protected: // search funtion for single thread
	inline void search_single_thread_vector(const envelope & e2d, std::vector<entry_ptr> & results)
	{
		//¼ÆËã°üº¬e2dµÄ×î³¤µÄÇ°×º
		uint64_t query_object_parent_identifier = calculateEnvelopeIdentifer(e2d, _depth);
		entry_ptr_array::iterator object_it;
		int s = _vector.size();
		int t = 0;
		leaf_node_type * pln = 0;
		entry_ptr_array * pa = 0;
		uint64_t t_identifier = 0;
		int code_relationship;
		envelope current_rect;
		for (t = 0; t<s; t++)
		{
			pln = _vector[t];
			code_relationship = test(pln->identifier, query_object_parent_identifier);
			pa = pln->objects;
			if (code_relationship == 0 || code_relationship == 1)
			{
				for (object_it = pa->begin(); object_it != pa->end(); object_it++)
				{
					if (test(e2d, *object_it) != 0)
					{
						results.push_back((*object_it));
					}
				}
				return;
			}
			if (code_relationship == -1)
			{
				calculateEnvelope(pln->identifier, current_rect);

				if (e2d.overlaps(current_rect))
				{
					if (current_rect.encloses(e2d))
					{
						for (object_it = pa->begin(); object_it != pa->end(); object_it++)
						{
							if (test(e2d, *object_it) != 0)
								results.push_back((*object_it));
							return;
						}
					}
					else if (e2d.encloses(current_rect))
					{
						for (object_it = pa->begin(); object_it != pa->end(); object_it++)
						{
							results.push_back((*object_it));
						}
					}
					else
					{
						for (object_it = pa->begin(); object_it != pa->end(); object_it++)
						{
							if (test(e2d, *object_it) != 0)
								results.push_back((*object_it));
						}
					}
				}
			}
		}
	}
protected://// search funtion for multithread
	//²¢ÐÐ²éÕÒÊµÏÖ
	void search_parallel_impl_vector(const envelope & e2d, std::vector<entry_ptr> & results)
	{
		//¼ÆËã°üº¬e2dµÄ×î³¤µÄÇ°×º
		uint64_t query_object_parent_identifier = calculateEnvelopeIdentifer(e2d, _depth);
		entry_ptr_array::iterator object_it;
		int s = _vector.size();
		int t = 0;
		entry_ptr_array * pa = 0;
		leaf_node_type * pln = 0;
		int code_relationship;
		envelope current_rect;
#pragma  omp parallel for private(pa,t,code_relationship,current_rect)
		for (t = 0; t<s; t++)
		{
			pln = _vector[t];
			pa = pln->objects;
			code_relationship = test(pln->identifier, query_object_parent_identifier);
			if (code_relationship == 0 || code_relationship == 1)
			{
				for (object_it = pa->begin(); object_it != pa->end(); object_it++)
				{
					if (test(e2d, *object_it) != 0)
					{
						results.push_back((*object_it));
					}
				}
				//return; //"return" can not be in the structure of openMP for
			}
			if (code_relationship == -1)
			{
				calculateEnvelope(pln->identifier, current_rect);
				if (e2d.overlaps(current_rect))
				{
					if (current_rect.encloses(e2d))
					{
						for (object_it = pa->begin(); object_it != pa->end(); object_it++)
						{
							if (test(e2d, *object_it) != 0)
								results.push_back((*object_it));
							//return ;//"return" can not be in the structure of openMP for
						}
					}
					else if (e2d.encloses(current_rect))
					{
						for (object_it = pa->begin(); object_it != pa->end(); object_it++)
						{
							results.push_back((*object_it));
						}
					}
					else
					{
						for (object_it = pa->begin(); object_it != pa->end(); object_it++)
						{
							if (test(e2d, *object_it) != 0)
								results.push_back((*object_it));
						}
					}
				}
			}
		}
	}
protected:
	inline void search_current_future(const envelope & qe, std::vector<entry_ptr> & results){
		assert(qe.getLowerBound(0) >= _currentTime);
		assert(DTTRAITS::velocity != 0);


		if (DTTRAITS::shape == DT_POINTS){
			envelope  e;
			for (std::map<uint64_t, entry_ptr>::iterator it = _currentEntries.begin(); it != _currentEntries.end(); it++){
				it->second->getEnvelope(qe.getUpperBound(0), e);
				if (e.overlaps(qe))
					results.push_back(it->second);
			}
		}
		else{
			assert(false);
		}
	}
	inline void search_single_thread_list(const envelope & qe, std::vector<entry_ptr> & results)
	{
		std::list<LeafNode*> leaflist;
		uint64_t query_object_parent_identifier = calculateEnvelopeIdentifer(qe, _depth);
		int t = 0;
		entry_ptr_array * pa = 0;
		entry_ptr_array::iterator entry_iter;
		LeafNode * ln = 0;
		uint64_t code, parentcode;

		for (map_iterator it = _directory.begin(); it != _directory.end(); it++)
		{
			t = test(query_object_parent_identifier, it->first);
			if (t == 0 || t == 1 || t == -1){
				ln = &(it->second);
				ln->identifier = it->first;
				leaflist.push_back(ln);
			}
		}

		/*for(std::vector<LeafNode*>::iterator it = _vector.begin();it!=_vector.end();it++)
		{
		t = test(query_object_parent_identifier,(*it)->identifier);
		if(t==0 || t==1 || t==-1){
		ln = (*it);
		leaflist.push_back(ln);
		}
		}*/
		envelope leafenvelope;
		while (!leaflist.empty()){
			ln = leaflist.front();
			leaflist.pop_front();
			code = ln->identifier;

			calculateEnvelope(code, leafenvelope);
			t = leafenvelope.test(qe);
			pa = ln->objects;
			switch (t){
			case 3:// leafenvelope encloses qe
			{
				for (entry_iter = pa->begin(); entry_iter != pa->end(); entry_iter++)
				{
					if (test(qe, *entry_iter) != 0)
					{
						results.push_back((*entry_iter));
					}
				}
				return;
			}
			case 2:// qe encloses leafenvelope;
			{
				for (entry_iter = pa->begin(); entry_iter != pa->end(); entry_iter++)
				{
					results.push_back((*entry_iter));
				}
				break;
			}
			case 1://qe overlaps leafenvelope;
			{
				for (entry_iter = pa->begin(); entry_iter != pa->end(); entry_iter++)
				{
					if (test(qe, *entry_iter) != 0)
					{
						results.push_back((*entry_iter));
					}
				}
				break;
			}
			default://disjiont
			{
				// calculate the shortest code which does not overlap qe
				do{
					parentcode = code;
					code = code >> NUMDIMS;
					calculateEnvelope(code, leafenvelope);
				} while (qe.overlaps(leafenvelope) == false);
				// remove all the leaves which include the prefix parentcode
				leaflist.remove_if(remove_leaf_in_list_predicate(parentcode));
				break;
			}
			}
		}
	}

	inline void predicative_query(const envelope & qe, entry_ptr_vector & entries){
		assert(qe.getLowerBound(0) >= _currentTime);
	}
protected:// deletion functions 
	inline void remove(entry_ptr p)
	{
		uint64_t code = calculateEnvelopeIdentifer(pi, _depth);
		while (code != 0)
		{
			map_iterator it = _directory.find(code);
			if (it != _directory.end())//find 
			{
				for (entry_ptr_array::iterator i = it->second.begin(); i != it->second.end(); i++)
				{
					if ((*i) == p)
					{
						it->second.erase(i);
						if (it->second.size() == 0)
							_directory.erase(it);
						return true;
					}
				}
			}
			else
				code = code >> NUMDIMS;
		}
		return false;
	}
};



end_gtl_namespace


#endif