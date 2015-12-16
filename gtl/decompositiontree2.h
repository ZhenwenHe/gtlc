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
#include "buffer.h"
#include "fastlist.h"
#include "pointerarray.h"
#include "omp.h"
#include "StorageManager.h"

#if GTL_VERSION>=2

begin_gtl_namespace

 
enum {
	DT_POINTS = 1,// for moving points
	DT_ENVELOPES = 2// for any moving object which can be represented by envelope
};


struct default_decomposition_tree_trait
{
	static const int  capacity = 32;//maximum number permitted in each leaf
	static const int  growth = 1;//growth number of object array of leaf
	static const int  shape = DT_POINTS;
	static const int  velocity = 0;// not use velocity
};


 
template<
	/*
	OBJECTTYPE can be any class which implements writeBuffer and readBuffer methods
	*/
	typename  OBJECTTYPE,
	typename COORDTYPE/*=double*/,
	int   NUMDIMS/*=4*/,
	typename DTTRAITS = default_decomposition_tree_trait,
	typename CHARTYPE = char,
	typename TRAITS = std::char_traits<CHARTYPE>,
	typename ALLOC = std::allocator<CHARTYPE>
>
class  DecompositionTree : public Object<CHARTYPE, TRAITS, ALLOC>
{
public:
	typedef Envelope<COORDTYPE, NUMDIMS, CHARTYPE, TRAITS, ALLOC>    envelope;
	typedef Vertex<COORDTYPE, NUMDIMS, CHARTYPE, TRAITS, ALLOC>      vertex;
	typedef std::vector<envelope>                                    envelope_vector;
	typedef COORDTYPE                                                coordinate;
	typedef COORDTYPE*                                               velocity_ptr;
	typedef StorageManager<CHARTYPE, TRAITS, ALLOC>                  storage_manager; 
	typedef Buffer<CHARTYPE, TRAITS, ALLOC>                          buffer;
	typedef Object<CHARTYPE, TRAITS, ALLOC>                          object;
public:// the Entry class 
	class Entry :public Object<CHARTYPE, TRAITS, ALLOC>{
	public:
		virtual std::basic_string<CHARTYPE, TRAITS, ALLOC> getClassName()
		{
			return std::basic_string<CHARTYPE, TRAITS, ALLOC>("Entry");
		}
		virtual void writeBuffer(void* & buffer, size_t & count)
		{
			Buffer<CHARTYPE, TRAITS, ALLOC> buf;
			buf.write((void*)&identifier, sizeof(uint64_t));
			buf.write((void*)&status, sizeof(unsigned char));
			buf.write((void*)&order, sizeof(uint64_t));
			buf.write((void*)&length, sizeof(unsigned int));
			if (DTTRAITS::shape == DT_POINTS)
			{
				buf.write((void*)(vshp->get()), sizeof(coordinate)*NUMDIMS);
				if (DTTRAITS::velocity != 0)
				{
					buf.write((void*)velocities, sizeof(coordinate)*(NUMDIMS - 1));
				}
			}
			else
			{
				buf.write((void*)(eshp->getLowerBounds()), sizeof(coordinate)*NUMDIMS);
				buf.write((void*)(eshp->getUpperBounds()), sizeof(coordinate)*NUMDIMS);
				if (DTTRAITS::velocity != 0)
				{
					buf.write((void*)velocities, sizeof(coordinate)*(NUMDIMS - 1) * 2);
				}
			}

			if (length != 0 && data != 0)
				buf.write((void*)data, length);

			buffer = buf.detach(count);
		}
		virtual void readBuffer(const void *  buffer, size_t count)
		{

			Buffer<CHARTYPE, TRAITS, ALLOC> buf;
			buf.attach((void*)buffer, count);
			buf.read((void*)&identifier, sizeof(uint64_t));
			buf.read((void*)&status, sizeof(unsigned char));
			buf.read((void*)&order, sizeof(uint64_t));
			buf.read((void*)&length, sizeof(unsigned int));
			if (DTTRAITS::shape == DT_POINTS)
			{
				buf.read((void*)(vshp->get()), sizeof(coordinate)*NUMDIMS);
				if (DTTRAITS::velocity != 0)
				{
					buf.read((void*)velocities, sizeof(coordinate)*(NUMDIMS - 1));
				}
			}
			else
			{
				buf.read((void*)(eshp->getLowerBounds()), sizeof(coordinate)*NUMDIMS);
				buf.read((void*)(eshp->getUpperBounds()), sizeof(coordinate)*NUMDIMS);
				if (DTTRAITS::velocity != 0)
				{
					buf.read((void*)velocities, sizeof(coordinate)*(NUMDIMS - 1) * 2);
				}
			}

			if (length != 0)
			{
				if (data)
					delete[] data;

				data = new unsigned char[length];
				buf.read((void*)data, length);
			}
			else
			{
				if (data)
					delete[] data;
				data = 0;
			}

			buf.detach(count);
		}
		virtual Object * clone()
		{
			return (Object *)new Entry(*this);
		}
	protected:
		uint64_t identifier;// the id of saptio-temporal object

		unsigned char status;//the status of the entity
		// it may be a point or an envelope used by the dtree index.
		union{
			void *     shape;
			vertex *   vshp;
			envelope * eshp;
		};

		uint64_t order;// the order of the vertex on a moving line
		/*
		the velocity of a sptio-temporal object
		the dimension of this variable is NUMDIMS-1
		if shape is an envelope, there ere lower and upper velocities
		*/
		velocity_ptr  velocities;

		unsigned char * data;//the data of the spatio-temporal object
		unsigned int length;// the byte size of the object;
	public:
		inline const uint64_t getIdentifier() const { return identifier; }
		inline const uint64_t getOrder() const { return order; }
		inline const vertex getCenter() const {
			if (DTTRAITS::shape == DT_POINTS)
				return *(static_cast<const vertex*>(shape));
			else
			{
				vertex v;
				(static_cast<envelope*>(shape))->getCenter(v);
				return v;
			}
		}

		inline const envelope  getEnvelope() const {
			if (DTTRAITS::shape == DT_ENVELOPES)
				return *(static_cast<envelope *>(shape));
			else
			{
				envelope v;
				coordinate * d = v.getLowerBounds();
				for (int i = 0; i < NUMDIMS; i++)
				{
					d[i] = (static_cast<const vertex*>(shape))->get(i);
					d[i] -= std::numeric_limits<coordinate>::epsilon();
				}
				d = v.getUpperBounds();
				for (int i = 0; i < NUMDIMS; i++)
				{
					d[i] = (static_cast<const vertex*>(shape))->get(i);
					d[i] += std::numeric_limits<coordinate>::epsilon();
				}
				return v;
			}

		}
		/*
		according to the current velocity of the spatio-temporal object,
		calculate an envelope in the future time t
		*/
		inline void getEnvelope(const double & t, envelope & e){
			assert(t >= getLastTime());
			if (DTTRAITS::shape == DT_POINTS){
				vertex* ver = static_cast<vertex*>(shape);
				velocity_ptr veo = getVelocities();
				e.setLowerBounds(*ver);
				e.setUpperBound(0, t);
				for (int i = 1; i < NUMDIMS; i++){
					e.setUpperBound(i, (t - ver->get(0)) * (veo[i - 1]) + ver->get(i));
				}

				e.makeValid();
			}
			else {
				e = *((static_cast<envelope*>(shape)));
			}
		}
		/*
		get the last time value, if it is a point ,the first dimensional value will be returned,
		otherwise, it is the first dimensional upper bound value of a envelope
		*/
		inline const double getLastTime() const {
			if (DTTRAITS::shape == DT_POINTS)
				return (static_cast<const vertex*>(shape))->get(0);
			else
				return *((static_cast<envelope*>(shape))->getUpperBounds(0));
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

		inline const unsigned char *  getData(unsigned int & len) {
			len = this->length;
			return data;
		}

	public:
		Entry(){
			status = 0;
			identifier = 0;
			order = 0;
			velocities = 0;
			length = 0;
			data = 0;
			if (DTTRAITS::shape == DT_POINTS)
			{
				vshp = new vertex();
				if (DTTRAITS::velocity != 0)
				{
					velocities = new coordinate[NUMDIMS - 1];
				}
			}
			else
			{
				eshp = new envelope();
				if (DTTRAITS::velocity != 0)
				{
					velocities = new coordinate[2 * (NUMDIMS - 1)];
				}
			}
		}

		Entry(const vertex & v, const void * obj, unsigned int len){
			assert(DTTRAITS::shape == DT_POINTS);
			assert(DTTRAITS::velocity == 0);

			uint64_t id = 0;
			shape = (void*) new vertex(v);
			status = 0;
			identifier = 0;
			order = 0;
			velocities = 0;

			length = len;
			data = new unsigned char[len];
			std::memcpy(data, obj, len);
		}

		Entry(const vertex & v, const velocity_ptr vv, const void * obj, unsigned int len){
			assert(DTTRAITS::shape == DT_POINTS);
			assert(DTTRAITS::velocity != 0);

			length = len;
			data = new unsigned char[len];
			std::memcpy(data, obj, len);

			uint64_t id = 0;
			shape = (void*) new vertex(v);
			status = 0;
			identifier = 0;
			order = 0;

			velocities = new coordinate[NUMDIMS - 1];
			std::memcpy(velocities, vv, sizeof(coordinate)* (NUMDIMS - 1));
		}

		Entry(const uint64_t & id, const uint64_t  & ull_order, const vertex & v, const char * obj, unsigned int len){
			assert(DTTRAITS::shape == DT_POINTS);
			assert(DTTRAITS::velocity == 0);

			shape = (void*) new vertex(v);
			status = 0;
			identifier = id;
			order = ull_order;
			velocities = 0;

			length = len;
			data = new unsigned char[len];
			std::memcpy(data, obj, len);
		}

		Entry(const uint64_t & id, const uint64_t  & ull_order, const vertex & v, const velocity_ptr vv, const void * obj, unsigned int len){
			assert(DTTRAITS::shape == DT_POINTS);
			assert(DTTRAITS::velocity != 0);

			length = len;
			data = new unsigned char[len];
			std::memcpy(data, obj, len);

			shape = (void*) new vertex(v);
			status = 0;
			identifier = id;
			order = ull_order;
			velocities = new coordinate[NUMDIMS - 1];
			memcpy(velocities, vv, sizeof(coordinate)* (NUMDIMS - 1));
		}

		Entry(const envelope & v, const void * obj, unsigned int len){
			assert(DTTRAITS::shape == DT_ENVELOPES);
			assert(DTTRAITS::velocity == 0);


			length = len;
			data = new unsigned char[len];
			std::memcpy(data, obj, len);

			uint64_t id = 0;
			shape = (void*) new envelope(v);
			status = 0;
			identifier = 0;
			order = 0;
			velocities = 0;
		}

		Entry(const envelope & v, const velocity_ptr vv, const void * obj, unsigned int len){
			assert(DTTRAITS::shape == DT_ENVELOPES);
			assert(DTTRAITS::velocity != 0);


			length = len;
			data = new unsigned char[len];
			std::memcpy(data, obj, len);

			uint64_t id = 0;
			shape = (void*) new envelope(v);
			status = 0;
			identifier = 0;
			order = 0;
			velocities = new coordinate[(NUMDIMS - 1) * 2];
			memcpy(velocities, vv, sizeof(coordinate)* (NUMDIMS - 1) * 2);
		}

		Entry(const uint64_t & id, const uint64_t  & ullorder, const envelope & v, const void * obj, unsigned int len){
			assert(DTTRAITS::shape == DT_ENVELOPES);
			assert(DTTRAITS::velocity == 0);

			length = len;
			data = new unsigned char[len];
			std::memcpy(data, obj, len);

			uint64_t id = 0;
			shape = (void*) new envelope(v);
			status = 0;
			identifier = id;
			order = ullorder;
			velocities = 0;
		}
		Entry(const uint64_t & id, const uint64_t  & ullorder, const envelope & v, const velocity_ptr vv, const void * obj, unsigned int len){
			assert(DTTRAITS::shape == DT_ENVELOPES);
			assert(DTTRAITS::velocity != 0);


			length = len;
			data = new unsigned char[len];
			std::memcpy(data, obj, len);

			uint64_t id = 0;
			shape = (void*) new envelope(v);
			status = 0;
			identifier = id;
			order = ullorder;
			velocities = new coordinate[(NUMDIMS - 1) * 2];
			memcpy(velocities, vv, sizeof(coordinate)* (NUMDIMS - 1) * 2);
		}

		virtual ~Entry(){
			clear();
		}

		inline void clear(){

			if (DTTRAITS::shape == DT_POINTS)
				delete (vertex*)shape;
			else
				delete (envelope*)shape;
			shape = 0;
			status = 0;

			if (velocities)
				delete[](velocity_ptr) velocities;

			if (data)
				delete[] data;

			data = 0;
			length = 0;

		}
		Entry(const Entry& e){


			length = e.length;
			data = new unsigned char[length];
			std::memcpy(data, e.data, length);

			if (DTTRAITS::shape == DT_POINTS){
				vshp = new vertex(*e.vshp);
				if (e.velocities){
					velocities = new coordinate[NUMDIMS - 1];
					memcpy(velocities, e.velocities, sizeof(coordinate)*(NUMDIMS - 1));
				}
				else{
					velocities = 0;
				}
			}
			else{
				eshp = new envelope(*e.eshp);
				if (e.velocities){
					velocities = new coordinate[(NUMDIMS - 1) * 2];
					memcpy(velocities, e.velocities, sizeof(coordinate)*(NUMDIMS - 1) * 2);
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

			if (DTTRAITS::shape == DT_POINTS){
				*vshp = *e.vshp;
				if (DTTRAITS::velocity != 0){
					memcpy(velocities, e.velocities, sizeof(coordinate)*(NUMDIMS - 1));
				}
			}
			else{
				*((envelope*)shape) = *((envelope*)e.shape);
				if (DTTRAITS::velocity != 0){
					memcpy(velocities, e.velocities, sizeof(coordinate)*(NUMDIMS - 1) * 2);
				}
			}
			status = e.status;
			identifier = e.identifier;
			order = e.order;
			if (length != e.length)
			{
				if (data)
					delete[] data;
				length = e.length;
				data = new unsigned char[length];
			}
			std::memcpy(data, e.data, length);

			return *this;
		}

	}; 
public:// the Entries class
	class Entries :public Object<CHARTYPE, TRAITS, ALLOC>{ 
		std::vector<Entry> _entries;
	public:
		virtual std::basic_string<CHARTYPE, TRAITS, ALLOC> getClassName()
		{
			return std::basic_string<CHARTYPE, TRAITS, ALLOC>("Entries");
		}
		virtual void writeBuffer(void* & buffer, size_t & count)
		{
			void * bf = 0;
			size_t s = _entries.size();
			int is = s;
			Buffer<CHARTYPE, TRAITS, ALLOC> buf;
			buf.write((void*)&is, sizeof(int));
			for (auto it = _entries.begin(); it != _entries.end(); it++)
			{
				it->writeBuffer(bf, s);
				is = s;
				buf.write(&is, sizeof(int));
				buf.write(bf, s);
				if (bf) delete[] bf;
				bf = 0;
			}
			buffer = buf.detach(count);
		}
		virtual void readBuffer(const void *  buffer, size_t count)
		{
			void * bf = 0;
			size_t s = 0;
			int is = 0;
			Buffer<CHARTYPE, TRAITS, ALLOC> buf;
			buf.attach((void*)buffer, count);
			buf.read((void*)&is, sizeof(int));
			s = is;
			_entries.resize(s);
			for (auto it = _entries.begin(); it != _entries.end(); it++)
			{
				buf.read(&is, sizeof(int));
				s = is;

				/* this block can be replaced with it->readBuffer(buf.tell(),s);avoid the memory copy*/
				/*bf = new unsigned char[s];
				buf.read(bf, s);
				it->readBuffer(bf, s);
				delete[] bf;
				bf = 0;*/
				it->readBuffer(buf.tell(), s);
			}
			buf.detach(count);
		}
		virtual Object * clone()
		{
			return new Entries(*this);
		}
	public:
		Entries(){
		}
		Entries(const Entries & es){
			_entries.resize(es.size());
			std::copy(es._entries.begin(), es._entries.end(), _entries.begin());
		}
		Entries operator = (const Entries & es){
			_entries.resize(es.size());
			std::copy(es._entries.begin(), es._entries.end(), _entries.begin());
			return *this;
		}
	public:
		inline int  capacity() { return DTTRAITS::capacity; }
		inline const size_t size()const { return _entries.size(); }
		inline Entry & get(int i){ return _entries[i]; }
		inline const Entry & get(int i)const { return _entries[i]; }
		inline bool add(const Entry & e) {
			if (size() == DTTRAITS::capacity)
				return false;
			_entries.push_back(e);
			return true;
		}
	}; 
public:// the leaf class
	class Node;
	class  Leaf : public Object<CHARTYPE, TRAITS, ALLOC>
	{
	protected:
		envelope _envelope;
		int64_t _page;//the first page to store the entries; 
		Entries  _entries;// the number is DTTRAITS::capacity	
	public:
		Leaf(const envelope &e) :_entries()
		{
			_envelope = e;
			_page = storage_manager::StorageManagerConstants::NewPage;
		}
		Leaf(const envelope & e ,const int64_t & page) :_entries()
		{
			_envelope = e;
			_page = page;
		}
		Leaf(const Leaf& ln) :_envelope(ln._envelope), _entries(ln._entries), _page(ln._page)
		{

		}
		Leaf operator =(const Leaf& ln)
		{
			_envelope = ln._envelope;
			_entries = ln._entries;
			_page = ln._page;
			return *this;
		}
		virtual ~Leaf()
		{
		}
	public:
		inline const envelope & getEnvelope() const { return _envelope; }
		inline const int64_t  getPage() const { return _page; }
		inline Entries & getEntries(){ return _entries; }
	public:
		virtual std::basic_string<CHARTYPE, TRAITS, ALLOC> getClassName()
		{
			return std::basic_string<CHARTYPE, TRAITS, ALLOC>("Leaf");
		}
		virtual void writeBuffer(void* & buffer, size_t & count)
		{
			void * bf = 0;
			size_t s = 0;
			int is = 0;
			Buffer<CHARTYPE, TRAITS, ALLOC> buf;
			buf.write((void*)&_page, sizeof(int64_t));

			_envelope.writeBuffer(bf, s);
			is = s;
			buf.write(&is, sizeof(int));
			buf.write(bf, s);
			if (bf) delete[] bf;
			bf = 0;

			_entries.writeBuffer(bf, s);
			is = s;
			buf.write(&is, sizeof(int));
			buf.write(bf, s);
			if (bf) delete[] bf;
			bf = 0;

			buffer = buf.detach(count);
		}
		virtual void readBuffer(const void *  buffer, size_t count)
		{
			void * bf = 0;
			size_t s = 0;
			int is = 0;
			Buffer<CHARTYPE, TRAITS, ALLOC> buf;
			buf.attach((void*)buffer, count);
			buf.read((void*)&_page, sizeof(int64_t));

			buf.read(&is, sizeof(int));
			s = is;
			bf = new unsigned char[s];
			buf.read(bf, s);
			_envelope.readBuffer(bf, s);
			delete[](char*)bf;
			bf = 0;


			buf.read(&is, sizeof(int));
			s = is;
			bf = new unsigned char[s];
			buf.read(bf, s);
			_entries.readBuffer(bf, s);
			delete[](char*)bf;
			bf = 0;
			buf.detach(s);
		}
		virtual Object * clone()
		{
			return new Leaf(*this);
		}
	public:
		inline void  load(storage_manager & sm,int64_t page)
		{
			_page = page;
			unsigned char * buf = 0;
			uint32_t len = 0;
			sm.load(_page, len, &buf);
			_entries.readBuffer(buf, len);
			delete[](unsigned char*)buf;
		}
		inline int64_t  store (storage_manager & sm)
		{
			void* buf = 0;
			size_t len = 0;
			_entries.writeBuffer(buf, len);
			sm.store(_page, len,(const unsigned char*) buf);
			delete[](char*)buf;
			return _page;
		}
 
		inline bool insert(storage_manager & sm, const Entry & e)
		{
			if (_entries.size() == DTTRAITS::capacity)
			{
				Node node(_envelope, _page);
				node.decompose();
				node.store(sm);
				for (int i = 0; i < NUMDIMS*NUMDIMS; i++)
				{
					node.insert(sm, _entries.get(i));
				}
				node.insert(sm, e);
				return true;
			}
			else
			{
				_entries.add(e);
				store(sm);
				return true;
			}
		}

		size_t search(storage_manager & sm, const envelope & e, std::vector<Entry> & results)
		{
			size_t c = 0;
			if (e.overlaps(_envelope))
			{
				for (int i = 0; i < _entries.size(); i++)
				{
					if (e.test(_entries.get(i).getCenter()) != 0)
					{
						results.push_back(_entries.get(i));
						c++;
					}
				}
			}
			return c;
		}
	}; 
public:
	class  Node : public Object<CHARTYPE, TRAITS, ALLOC>
	{
	public:
		struct BRANCH{
			envelope e;//envelope
			int64_t  p;//page
			unsigned char f;//0-null, 1-node, 2-leaf
		};
	protected:
		envelope       _envelope;
		int64_t        _page;
		BRANCH         _branches[NUMDIMS*NUMDIMS];
	public:
		inline BRANCH & getBranch(int i){ return _branches[i]; }
		inline BRANCH* getBranches( ){ return _branches; }
	public:
		Node(){
			_page = storage_manager::StorageManagerConstants::NewPage;
			for (int i = 0; i < NUMDIMS*NUMDIMS; i++)
			{
				_branches[i].f = 0;
			}
		}
		Node(const envelope & e){
			_envelope = e;
			_page = storage_manager::StorageManagerConstants::NewPage; 
			decompose();
		}
		Node(const envelope & e,int64_t p){
			_envelope = e;
			_page = p;
			decompose();
		}
		Node(const Node & n){
			_envelope = n._envelope;
			_page = n._page; 
			size_t s = NUMDIMS*NUMDIMS;
			for (size_t i = 0; i < s; i++){
				_branches[i] = n._branches[i];
			}
		}
		virtual ~Node(){
		}
	public:
		virtual std::basic_string<CHARTYPE, TRAITS, ALLOC> getClassName()
		{
			return std::basic_string<CHARTYPE, TRAITS, ALLOC>("Node");
		}
		virtual void writeBuffer(void* & buffer, size_t & count)
		{
			void * bf = 0;
			size_t s = 0;
			int is = 0;
			Buffer<CHARTYPE, TRAITS, ALLOC> buf;
			//write the page id
			buf.write((void*)&_page, sizeof(int64_t)); 
			//write the total envelope of this node
			_envelope.writeBuffer(bf, s);
			is = s;
			buf.write(&is, sizeof(int));
			buf.write(bf, s);
			if (bf) delete[] bf;
			bf = 0;
 
			s = NUMDIMS*NUMDIMS;
			is = s;
			buf.write(&is, sizeof(int));
			for (size_t i = 0; i < s; i++){
				//write the page id
				buf.write((void*)&(_branches[i].p), sizeof(int64_t));
				//write the total envelope of this node
				_branches[i].e.writeBuffer(bf, s);
				is = s;
				buf.write(&is, sizeof(int));
				buf.write(bf, s);
				if (bf) delete[] bf;
				bf = 0;
				buf.write((void*)&(_branches[i].f), sizeof(unsigned char));
			}
			

			buffer = buf.detach(count);
		}
		virtual void readBuffer(const void *  buffer, size_t count)
		{
			void * bf = 0;
			size_t s = 0;
			int is = 0;
			Buffer<CHARTYPE, TRAITS, ALLOC> buf;
			buf.attach((void *)buffer, count);
			buf.read((void*)&_page, sizeof(int64_t)); 

			buf.read(&is, sizeof(int));
			s = is;
			bf = new unsigned char[s];
			buf.read(bf, s);
			_envelope.readBuffer(bf, s);
			delete[](char*)bf;
			bf = 0;

 
			s = NUMDIMS*NUMDIMS;
			is = s;
			buf.read(&is, sizeof(int));
			assert(is == s);
			for (size_t i = 0; i < s; i++){
				//read the page id
				buf.read((void*)&(_branches[i].p), sizeof(int64_t));
				//read  envelope  					
				buf.read(&is, sizeof(int));
				is = s;
				bf = new char[s];
				buf.read(bf, s);
				_branches[i].e.readBuffer(bf, s);
				if (bf) delete[] bf;
				bf = 0;
				buf.read((void*)&(_branches[i].f), sizeof(unsigned char));
			}		 

			buf.detach(s);
		}
		virtual Object * clone()
		{
			return new Node(*this);
		}
	public:
		inline void  load(storage_manager & sm,int64_t page)
		{
			_page = page;
			unsigned char * buf = 0;
			uint32_t len = 0;
			sm.load(_page, len, &buf);
			readBuffer(buf, len);
			delete[](unsigned char*)buf;
		}
		inline int64_t  store(storage_manager & sm)
		{
			void* buf = 0;
			size_t len = 0;
			writeBuffer(buf, len);
			sm.store(_page, len, (const unsigned char*)buf);
			delete[](char*)buf;
			return _page;
		}
		inline int containsTest(const Entry & e){
			for (int i = 0; i < NUMDIMS*NUMDIMS; i++)
			{
				if (_branches[i].e.test(e.getCenter()) != 0)
					return i;
			}
		}
		inline void decompose(){
			const envelope & p = _envelope;
			int index = 0;
			uint64_t mark ;
			for (int j = 0; j < NUMDIMS*NUMDIMS; j++)
			{
				index = j;
				envelope & e = _branches[j].e; 				
				mark = 1;
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
				_branches[j].p = storage_manager::StorageManagerConstants::NewPage;
				_branches[j].f = 0;
			}
		}
		bool insert(storage_manager & sm , const Entry & e)
		{
			int i = containsTest(e);
			switch (_branches[i].f)
			{
			case 0://null, insert a leaf
				{
					Leaf leaf(_branches[i].e, _branches[i].p);
					leaf.insert(sm,e);
					_branches[i].p = leaf.getPage();
					store(sm);
					return true;
				}				
			case 1://subnode
				{
					Node subnode(_branches[i].e);
					subnode.load(sm, _branches[i].p);
					return subnode.insert(sm, e);
				}				
			case 2://leaf
				{
					Leaf leaf(_branches[i].e, _branches[i].p);
					leaf.load(sm, _branches[i].p);
					return leaf.insert(sm, e); 
				}				
			}
			return true;
		}

		size_t search(storage_manager & sm, const envelope & e, std::vector<Entry> & results)
		{
			size_t c = 0;
			if (_envelope.overlaps(e))
			{
				for (int i = 0; i < NUMDIMS*NUMDIMS; i++)
				{
					switch (_branches[i].f)
					{
					case 0:
						break;
					case 1:
						{
							if (_branches[i].e.overlaps(e))
							{
								Node n;
								n.load(sm,_branches[i].p);
								c += n.search(sm,e, results);
							}
							
							break;
						}
					case 2:
						{
							if (_branches[i].e.overlaps(e))
							{
								Leaf leaf(_branches[i].e,_branches[i].p);
								leaf.load(sm,_branches[i].p);
								c += leaf.search(sm,e, results);
							}							
							break;
						}
							
					}
				}
			}
			return c;
		}
	};
public:// object class methods
	virtual std::basic_string<CHARTYPE, TRAITS, ALLOC> getClassName(){
		return "DecompositionTree";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		assert(false);
	}
	virtual void readBuffer(const void *  buffer, size_t count = 0){
		assert(false);
	}
	virtual Object * clone() {
		assert(false);
		return 0;
	}
protected: 
	storage_manager * _storageManager;
	envelope          _total;
	uint64_t          _rootPage;
protected:	
	/*
	divide the envelope p into _pow_of_dimensions envelopes, and get the envelope located in index
	*/
	inline void decompose(const envelope& p, int index, envelope & e)
	{
		uint64_t mark = 1;
		for (int i = 0; i<NUMDIMS; i++)t
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
	/*
	divide the envelope p into _pow_of_dimensions envelopes
	*/
	inline void  decompose(const envelope& p, envelope es[])
	{
		int s = NUMDIMS*NUMDIMS;
		for (int i = 0; i<s; i++){
			decompose(p, i, es[i]);
		}
	}
	
public://constructor and deconstructor
	DecompositionTree(const envelope & e, storage_manager * sm) 
	{
		_storageManager = sm;
		_total = e;
		_rootPage = storage_manager::StorageManagerConstants::NewPage;
		Node n(_total);
		_rootPage=n.store(*_storageManager); 
	}
	virtual ~DecompositionTree()
	{
		 
	}
public:
	 
	bool insert(const Entry & e)
	{
		// if the Entry is not in the totoal envelope, then return directly
		if (_total.test(e.getCenter())==0)
		{
			return false;
		}
		//1 load the root node
		Node root;
		root.load(*_storageManager, _rootPage);
		return root.insert(*_storageManager, e);
	}
	size_t search(const envelope & e, std::vector<Entry> & results)
	{
		//1 load the root node
		Node root;
		root.load(*_storageManager, _rootPage);
		return root.search(*_storageManager, e, results);
	}
};

end_gtl_namespace

#endif