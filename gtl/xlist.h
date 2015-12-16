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
#include <stack> 
#include "envelope.h"
begin_gtl_namespace

/** @defgroup   Xlist  区域广义列表                     
    *  @{
*/

template<
typename  OBJECTTYPE ,/**空间对象类型*/  
typename  VALUETYPE, /*最小边界矩形的值类型*/  
int NUMDIMS ,/*空间对象维数*/
int MAXELEMS ,/*每个子表最大元素个数*/ 
typename T=char,
typename TRAITS=std::char_traits<T>,
typename ALLOC=std::allocator<T>
>
class  Xlist : public Object<T,TRAITS,ALLOC>{
public:
	virtual std::basic_string<T,TRAITS,ALLOC> getClassName(){
		return "Xlist";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
    virtual Object<T,TRAITS,ALLOC> * clone() {
		return 0;
	}
public:
	struct INDEXSTATS{
		unsigned long long objects;// spatiotemopral objects
		unsigned long long nodes;// total nodes in the index
		unsigned long long innernodes;// inner nodes in the index
		unsigned long long leafnodes;// inner nodes in the index
		unsigned long long splits;// the times of splitting operations
		unsigned long long keycomparisons;// the key comparison times;
		unsigned long long reads;// the reading io times;
		unsigned long long writes;//the writing io times;
		unsigned long long depth;// the depth of index tree or list
	} ;

	INDEXSTATS m_stats;
public:
	typedef Envelope<VALUETYPE,NUMDIMS> BoundingBox; /** @brief 最小边界矩形 */
	
	struct Element{
		/** @brief 指向前一个同级并列元素 */
		Element * prev; 
		/** @brief 指向后一个同级并列元素 */
		Element * next; 
		/** @brief 指向上级列表元素 */
		//Element * parent; 
		/** @brief 列表元素类型 0表示为原子元素，1表示为子列表 */
		unsigned char type; 
		/** @brief 当type==0是，objptr有效，否则sublist有效 */
		union {
			OBJECTTYPE   obj;
			Element * sublist;
		};
		/** @brief 元素的最小边界矩形 */
		BoundingBox   bound;
	};

	Xlist(){
		_head=0;
		memset(&m_stats,0,sizeof(INDEXSTATS));
	}
	~Xlist(){
		clear();
		_head=0;
	}
	
protected:
	/** @brief 广义列表的头 */
	Element  * _head;
	/** @brief 整个列表的BoundingBox */
	BoundingBox   _bound;
protected:	
	
	/** 统计 列表中元素个数
		* @param  [in] h 列表头指针
		* @return 返回头节点同级的所有元素个数
		*/
	static int numberElements(Element  * h){
		Element * p = h;
		if(!h) return 0;
		int i=1;
		while(p->prev) p=p->prev;

		while (p->next) {
			i++;
			p=p->next;
		}
		return i;
	}
	/** 在指定的元素之前插入一个元素 
		* @param  [in,out] pos 参照元素
		* @param  [in,out] e 待插入的元素
		* @return 如果执行成功则返回true,否则返回false
		*/
	static bool insertBefore(Element* pos, Element  * e){
		if(!pos || !e) return false;
		if(!pos->prev) { /*如果pos是头 */
			e->next = pos;
			pos->prev=e;
		}
		else{
			Element * p = pos->prev;
			p->next=e;
			e->prev=p;
			pos->prev=e;
			e->next=pos;
		}
		return true;
	}
	/** 在指定的元素之后插入一个元素 
	* @param  [in,out] pos 参照元素
	* @param  [in,out] e 待插入的元素
	* @return 如果执行成功则返回true,否则返回false
	*/
	static bool insertAfter(Element* pos, Element  * e){
		if(!pos || !e) return false;
		if(!pos->next) { /*如果pos是尾*/
			e->prev = pos;
			pos->next=e;
		}
		else{
			Element * p = pos->next;
			pos->next=e;
			e->prev=pos;
			p->prev=e;
			e->next=p;
		}
		return true;
	}
	/** 获取列表中的最后一个元素
	* @param  [in,out] h 列表中的任一个元素
	* @return 返回尾部元素
	*/
	static Element * getTail(Element* h){
		Element * e = h;
		while (e->next)
			e=e->next;
		return e;
	}
	/** 获取列表中的最前一个元素
	* @param  [in,out] h 列表中的任一个元素
	* @return 返回头部元素
	*/
	static Element * getHead(Element* h){
		Element * e = h;
		while (e->prev)
			e=e->prev;
		return e;
	}
	/** 在尾部插入一个元素 
	* @param  [in,out] h 列表中的任一个元素
	* @param  [in,out] e 待插入的元素
	* @return 如果执行成功则返回true,否则返回false
	*/
	static bool addTail(Element* h,Element  * e){
		if(!h || !e) return false;
		Element * pos = getTail(h);
		pos->next=e;
		e->prev=pos;
		return true;
	}
	/** 在头部插入一个元素 
	* @param  [in,out] h 列表中的任一个元素
	* @param  [in,out] e 待插入的元素
	* @return 如果执行成功则返回true,否则返回false
	*/
	static bool addHead(Element* h,Element  * e){
		if(!h || !e) return false;
		Element * pos = getHead(h);
		pos->prev=e;
		e->next=pos;
		return true;
	}
	/** 新建一个原子类型的列表元素 
		* @param  [in] obj 空间对象
		* @param  [in] b 对象BoundingBox
		* @return 返回新元素
		*/
	static Element * newElement(OBJECTTYPE obj, BoundingBox & b){
		Element * e = new Element;
		e->bound=b;
		e->obj=obj;
		e->type=0;
		e->prev=0;
		e->next=0;
		return e;
	}
	/** 新建一个子表类型的列表元素 
	* @param  [in] obj 空间对象
	* @param  [in] b 对象BoundingBox
	* @return 返回新元素
	*/
	static Element * newElement(Element * c,bool isElement){
		Element * e = new Element;
		if(isElement){//如果c表示的是单元素		
			e->bound=c->bound;
			e->sublist=c;
			e->type=1;		
			e->prev=0;
			e->next=0;

			//建立连接关系
			if(c->prev){
				c->prev->next=e;
				e->prev=c->prev;
			}
			if(c->next){
				c->next->prev=e;
				e->next=c->next;
			}
			c->next=0;
			c->prev=0;
		}
		else{//如果c代表的是一个链表
			Element * h = getHead(c);

			e->sublist=h;
			e->type=1;		
			e->prev=0;
			e->next=0;

			while(h){
				e->bound +=h->bound;
				h=h->next;
			}
		}
		
		return e;
	}
	/** 在以h为头的列表中，查询与b相交叠的元素
		* @param  [in] h 列表头元素
		* @param  [in] b 最小边界矩形
		* @return 与b相交叠的元素指针
		*/
	static Element * findOverlap(Element * h,BoundingBox & b){
		Element * e = h;
		while(e){
			if(e->bound.overlaps(b))
				return e;
			else
				e=e->next;
		}
		return e;
	}
	/** 用obj替换以head为头节点的链表的某个元素，使得head链表中的所有元素的包围盒最小 
		* @param  [in,out] head 链表头节点
		* @param  [in,out] obj 要替换进去的元素
		* @return 如果执行成功则返回被替换下来的元素,否则返回0
		*/
	static Element * replace(Element * & head,Element * obj){
		BoundingBox b ,bt;
		Element * h = head;
		Element * p = head;
		Element * p_min=0;

		while (p){
			b+=p->bound;
			p=p->next;
		}

		while(h){
			p=head;
			bt.reset();
			while(p){
				if(p==h){
					p=p->next;
					continue;
				}
				else{
					bt+=p->bound;
					p=p->next;
				}				
			}

			bt+=obj->bound;
			if(bt.volume()<b.volume()){
				b=bt;
				p_min=h;
			}
			h=h->next;
		}
		if(p_min){//如果符合替换条件，将obj和p_min对换
			obj->next=p_min->next;
			obj->prev=p_min->prev;
			if (p_min->prev)
				p_min->prev->next=obj;
			if(p_min->next)
				p_min->next->prev=obj;

			p_min->prev=0;
			p_min->next=0;

			
			head = getHead(obj);
		}
		return p_min;
	}
protected:
	/** 将一个空间对象插入列表中
		* @param  [in,out] head 列表头元素
		* @param  [in,out] bound 列表对应的BoundingBox
		* @param  [in] head_parent head元素的父节点
		* @param  [in] obj 空间对象
		* @param  [in] b 空间对象对应的边界矩形
		* @return 如果执行成功则返回true,否则返回false
		*/
	static bool insert(Element* & head, BoundingBox & bound,Element*  head_parent, OBJECTTYPE obj, BoundingBox & b) {
		//如果是第一个元素，直接添加
		if (!head)	{
			head = newElement(obj,b);
			bound=b;
			head->bound=b;
			return true;
		}
		/*********************************************************************** 
		任取Ri，首先计算Ri与C是否相交：
		1）	如果不相交，并且k<m-1，则将Ri作为C的表尾元素添加；
		2）	如果不相交，并且k=m-1，则将C作为表头，Ri作为表尾，构建新的表，重新记为C；
		3）	如果相交，并且存在Cj与Ri相交，则将Ri与Cj合并成新的Cj；
		4）	如果相交，并且不存在Cj与Ri相交，若k<m-1，则将Ri作为C的表尾元素添加；若k=m-1，则将Ri与Ck(C的表尾元素)合并成新的Ck；
		***********************************************************************/
		if( bound.overlaps(b)){//如果和总表相交
			Element * e = findOverlap(head,b);
			if(e){//和总表中的元素相交
				if(e->type==0) {//如果是原子元素
					if(head_parent){
						int k = numberElements(e);
						if(k<MAXELEMS) {
							addTail(e,newElement(obj,b));
							head_parent->bound.extend(b);
							bound.extend(b);
						}
						else{
							Element * sub = newElement(e,true);
							addTail(e,newElement(obj,b));
							sub->bound.extend(b);
							bound.extend(b);
						}						
					}
					else{
						Element * sub = newElement(e,true);
						
						addTail(e,newElement(obj,b));
						sub->bound.extend(b);
						bound.extend(b);
					}
										
				}
				else{//如果不是原子元素
					bound.extend(b);
					insert(e->sublist,e->bound,e,obj,b);
					
				}				
			}
			else{//不和总表中的任何元素相交
				int k = numberElements(head);
				
				if(k<MAXELEMS){
					e=newElement(obj,b);
					addTail(head,e);
					bound.extend(b);
				}
				else{//需要调整
					Element * e= newElement(obj,b);
					Element* t = replace(head,e);
					if(t){//替换出来的元素不为空
						Element * h = newElement(head,false);
						while(head){
							h->bound += head->bound;
							head=head->next;
						}						
						addTail(h,t);
						bound.extend(b);
						head=h;
					}
					else{
						Element * h = newElement(head,false);
						h->bound=bound;
						addTail(h,e);
						bound.extend(b);
						head=h;
					}									
				}
			}
		}
		else{//如果和总表不相交
			int k = numberElements(head);
			if(k<MAXELEMS){
				Element * e = newElement(obj,b);
				addTail(head,e);
				bound.extend(b);
			}
			else{
				Element * h = newElement(head,false);
				h->bound=bound;
				Element * e= newElement(obj,b);
				addTail(h,e);
				bound.extend(b);
				
				head=h;
			}
		}
		return true;
	}
	/** 区域查找函数 
		* @param  [in] head 查询列表的头指针
		* @param  [in] lists_bound  查询列表的边界矩形
		* @param  [in] search_bound  查询边界矩形
		* @param  [in,out] results  返回查询到的结果
		* @return 如果执行成功则返回查询到的对象个数,否则返回0
		*/
	static int search(Element * head,BoundingBox & lists_bound, BoundingBox & search_bound,std::vector<OBJECTTYPE> & results, INDEXSTATS & stats){
		if(!lists_bound.overlaps(search_bound))
			return 0;

		Element * p = head;
		while (p){
#ifdef  _GTL_DEBUG_
			stats.keycomparisons+=1;
#endif //_GTL_DEBUG_
			if(p->bound.overlaps(search_bound)){
				if(p->type==0){
					results.push_back(p->obj);
					p=p->next;

				}
				else{
					search(p->sublist,p->bound,search_bound,results,stats);
#ifdef  _GTL_DEBUG_
					++ (stats.reads);
#endif //_GTL_DEBUG_
					p=p->next;

				}
			}
			else
			{
				p=p->next;
			}
		}
		return results.size();
	}
	/** 在head表示的列表中查找obj对象的元素节点 
		* @param  [in] obj 空间对象
		* @param  [in] head 列表头节点
		* @param  [in] head_parent 列表头节点的父节点指针
		* @param  [in,out] pe 如果找到，通过pe返回
		* @param  [in,out] pe_parent 返回pe的父节点
		* @return 如果执行成功则返回true,否则返回false
		*/
	static bool find(OBJECTTYPE obj,Element * head, Element * head_parent,Element *  &pe , Element  * & pe_parent){
		
		Element * h = head;
		Element * p = head_parent;
		while (h){
			if(h->type==0){
				if(h->obj==obj){
					pe = h;
					pe_parent=p;
					return true;
				}
				else{
					h=h->next;
				}
			}
			else{
				if(find(obj,h->sublist,h,pe,pe_parent))
					return true;
				else
					h=h->next;
			}
			
		}
		return false;		
	}
	/** 判断以h开头的列表时候含有子列表，如果有返回true，如果没有，返回false 
		* @param  [in] h 头指针
		* @return 如果执行成功则返回true,否则返回false
		*/
	static bool has_sublist(Element * h){
		if(!h) return false;
		Element * p = h;
		while (p){
			if(p->type!=0)
				return true;
			else
				p=p->next;
		}
		return false;
	}
	
	/** 在head表示的列表中查找obj对象的父表路径 
	* @param  [in] obj 空间对象
	* @param  [in] head 列表头节点
	* @param  [in] head_parent 列表头节点的父节点指针
	* @param  [in,out] parents 父表节点栈
	* @return 如果执行成功则返回true,否则返回false
	*/
	static bool find(OBJECTTYPE obj,Element * head, Element * head_parent,std::stack<Element *> &parents){
		/*[find_parents]
		(1)	如果head_parent不为空，将head_parent进栈；
		(2)	如果head是原子，且h->obj等于obj，则返回true；
		(3)	如果head是原子，但h->obj不等于obj，则令h=h->next，直到h为空或h不是原子节点；
		(4)	如果head不为空，则循环执行下面步骤；
		(5)	如果head子表，令head_parent=head，head=head->sublist；
		(6)	递归调用find_parents，如果返回结果为true，则直接返回；
		(7)	如果递归调用返回结果为false，则令head=head_parent，head_parent=栈顶元素，并出栈；
		令head=head->next，递归调用find_parents；如果返回为true，则直接返回；如果返回为false，重复执行步骤(4)到步骤(7)。*/

		
		Element * h = head;
		Element * p = head_parent;
		if(p) 
			parents.push(p);

		while(h){
			if(h->type==0){
				if(h->obj==obj)
					return true;
				h=h->next;
			}
			else
				break;			
		}

		while(h){
			p=h;		
			h=h->sublist;
			if(find(obj,h,p,parents)){				
				return true;
			}
			else{
				h=p;
				p=parents.top();
				parents.pop();
				h=h->next;
				if(find(obj,h,p,parents)){					
					return true;
				}
			}
		}

		parents.pop();
		return false;
	}
	/** 统计列表中总结有多少个节点 
	* @return 如果执行成功则返回节点个数,否则返回-1
	*/
	static long numberNodes(Element * h){
		int i=0;
		while (h)
		{
			i++;
			if(h->type!=0)
				i+=numberNodes(h->sublist);
			h=h->next;
		}
		return i;
	}
	/** 清空列表 
		* @param  [in] head 列表头节点指针
		* @return 无
		*/
	static void clear(Element * head){
		while(head){
			if(head->type==0){//如果是原子，则直接删除该节点，并重新连接链表
				Element *  n = head->next;				
				if(n){
					n->prev=0;
					delete head;
					head = n;
				}
				else
					break;
			}
			else{
				clear(head->sublist);
			}
			head = head->next;
		}
	}
public:
	/** 清空列表，并将_bound重置
		* @return 无
		*/
	void clear(){
		clear(_head);
		_bound.reset();
		_head=0;
	}

	/** 计算整个列表占用的内存空间大小
		* @return 返回整个列表占用的内存空间大小
		*/
	long allocSize(){
		return numberNodes()*sizeof(Element);
	}

	/** 统计列表中总结有多少个节点 
	* @return 如果执行成功则返回节点个数,否则返回-1
	*/
	long numberNodes(){
		return numberNodes(_head);
	}
	/** 插入一个元素 
	* @param  [in] obj 空间对象
	* @param  [in] b 空间对象对应的BoundingBox
	* @return 如果执行成功则返回true,否则返回false
	*/
	bool insert(OBJECTTYPE obj, BoundingBox b){
#ifdef  _GTL_DEBUG_
					++ m_stats.objects;
#endif //_GTL_DEBUG_
		return insert(_head,_bound,0,obj,b);
	}
	/** 区域查找函数 
		* @param  [in] search_bound 查询的区域
		* @param  [in,out] results 查询的结果
		* @return 如果执行成功则返回查找到的个数,否则返回0
		*/
	int search(BoundingBox search_bound,std::vector<OBJECTTYPE> & results){
#ifdef  _GTL_DEBUG_
					++ m_stats.reads;
#endif //_GTL_DEBUG_
		return search(_head,_bound,search_bound,results,m_stats);
	}
	/** 在head表示的列表中查找obj对象的元素节点 
	* @param  [in] obj 空间对象
	* @param  [in,out] pe 如果找到，通过pe返回
	* @param  [in,out] pe_parent 返回pe的父节点
	* @return 如果执行成功则返回true,否则返回false
	*/
	bool find(OBJECTTYPE obj,Element *  &pe , Element  * & pe_parent){
		return find(obj,_head,0,pe,pe_parent);
	}
	/** 在head表示的列表中查找从head到obj对象的路径 
	* @param  [in] obj 空间对象
	* @param  [in] head 列表头节点
	* @param  [in] head_parent 列表头节点的父节点指针
	* @param  [in,out] parents 父表节点栈
	* @return 如果执行成功则返回true,否则返回false
	*/
	bool find(OBJECTTYPE obj,std::stack<Element *> &parents){
		if(find(obj,_head,0,parents)){
				Element * p = parents.top()->sublist;
				while (p){
					if(p->obj!=obj)
						p=p->next;
					else{
						parents.push(p);
						return true;
					}
				}
		}
		return false;
	}

	/** 删除算法 
	* @param  [in] obj 空间对象
	* @return 如果执行成功则返回删除元素指针,否则返回0
	*/
	Element * remove(OBJECTTYPE obj){
		std::stack<Element *> path;
		if(!find(obj,path)) 
			return 0;

		//如果发现节点，返回路径
		Element * p_obj = path.top();
		path.pop();
		Element * p =p_obj;
		Element * h = p;
		if(path.empty()){//为顶层表元素
			if(!p->prev) {//头元素
				h = p->next;
				h->prev=0;
				p->next=0;
			}
			else if (!p->next){//尾元素
				h = p->prev;
				p->prev=0;
				h->next=0;
				h = getHead(h);
			}
			else{//中间元素
				h = p->prev;
				h->next=p->next;
				p->next->prev=h;
				p->prev=0;
				p->next=0;
				h = getHead(h);
			}		
			//重新计算_bound
			_bound.reset();
			while (h){
				_bound+=h->bound;
				h=h->next;
			}
		}
		else{//如果不是顶层元素
			Element * parent   = path.top();
			path.pop();
			if(!p->prev) {//头元素
				h = p->next;
				h->prev=0;
				p->next=0;

			}
			else if (!p->next){//尾元素
				h = p->prev;
				p->prev=0;
				h->next=0;
				h = getHead(h);
			}
			else{//中间元素
				h = p->prev;
				h->next=p->next;
				p->next->prev=h;
				p->prev=0;
				p->next=0;
				h = getHead(h);
			}		
			parent->sublist=h;

			if(!h->next){//如果h所在的同级元素只有1个，则将h合并到上级列表中
				
				h->prev=parent->prev;
				h->next=parent->next;

				if(parent->next)
					parent->next->prev=h;
				if(parent->prev)
					parent->prev->next=h;

				parent->prev=0;
				parent->next=0;
				parent->sublist=0;
				delete parent;

				while(!path.empty()){
					h = getHead(h);
					parent   = path.top();	
					path.pop();
					parent->bound.reset();
					while (h){
						parent->bound +=h->bound;
						h=h->next;
					}
				}

				//计算整体的边界
				_bound.reset();
				parent = getHead(parent);
				while (parent){
					_bound +=parent->bound;
					parent=parent->next;
				}
			}
			else{//重新计算_bound					
				do {
					h = getHead(h);
					parent->bound.reset();
					while (h){
						parent->bound +=h->bound;
						h=h->next;
					}
					h = parent;
					parent   = path.top();	
					path.pop();
				}while(!path.empty());

				//计算整体的边界
				_bound.reset();
				parent = getHead(parent);
				while (parent){
					_bound +=parent->bound;
					parent=parent->next;
				}
			}				
		}
		return p_obj;
		
	}
};


/** @}*/ 


end_gtl_namespace
