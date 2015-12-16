/*
 *  Copyright (c) 2008 Dustin Spicuzza <dustin@virtualroadside.com>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of version 2 of the GNU General Public License
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/*
 *	This is intended to be a templated implementation of an R* Tree, designed
 *	to create an efficient and (relatively) small indexing container in N
 *	dimensions. At the moment, it is a memory-based container instead of disk
 *  based.
 *
 *	Based on "The R*-Tree: An Efficient and Robust Access Method for Points
 *	and Rectangles" by N. Beckmann, H.P. Kriegel, R. Schneider, and B. Seeger
 */


#pragma once

#include <list>
#include <vector>
#include <limits>
#include <algorithm>
#include <cassert>
#include <functional>

#include <iostream>
#include <sstream>
#include <fstream>
#include <utility>
#include <cstddef>
#include <string>
#include <sstream>
#include "object.h"

begin_gtl_namespace
////////////////////////////////////////////////////////////////////////////////
template <std::size_t dimensions>
struct RStarBoundingBox {

	// edges[x].first is low value, edges[x].second is high value
	std::pair<double, double> edges[dimensions];

	void set(const double minv[dimensions],const double maxv[dimensions])
	{
		for (std::size_t axis = 0; axis < dimensions; axis++)
		{
			edges[axis].first = minv[axis];
			edges[axis].second = maxv[axis];
		}
	}
	// forces all edges to their extremes so we can stretch() it
	void reset()
	{
		for (std::size_t axis = 0; axis < dimensions; axis++)
		{
            edges[axis].first = DBL_MAX;//std::numeric_limits<double>::max();
            edges[axis].second = -DBL_MAX;//- edges[axis].first;
		}
	}

	// returns a new bounding box that has the maximum boundaries
	static RStarBoundingBox MaximumBounds()
	{
		RStarBoundingBox<dimensions> bound;
		bound.reset();
		return bound;
	}


	// fits another box inside of this box, returns true if a stretch occured
	bool stretch(const RStarBoundingBox<dimensions> &bb)
	{
		bool ret = false;

		for (std::size_t axis = 0; axis < dimensions; axis++)
		{

			if (edges[axis].first > bb.edges[axis].first)
			{
				edges[axis].first = bb.edges[axis].first;
				ret = true;
			}

			if (edges[axis].second < bb.edges[axis].second)
			{
				edges[axis].second = bb.edges[axis].second;
				ret = true;
			}
		}

		return ret;
	}

	// the sum of all deltas between edges
	inline double edgeDeltas() const
	{
		double distance = 0;
		for (std::size_t axis = 0; axis < dimensions; axis++)
			distance += edges[axis].second - edges[axis].first;

		return distance;
	}

	// calculates the area of a bounding box
	inline double area() const
	{
		double area = 1;
		for (std::size_t axis = 0; axis < dimensions; axis++)
			area *= (double)(edges[axis].second - edges[axis].first);

		return area;
	}

	// this determines if a bounding box is fully contained within this bounding box
	inline bool encloses(const RStarBoundingBox<dimensions>& bb) const
	{
		// if (y1 < x1 || x2 < y2)
		for (std::size_t axis = 0; axis < dimensions; axis++)
			if (bb.edges[axis].first < edges[axis].first || edges[axis].second < bb.edges[axis].second)
				return false;

		return true;
	}

	// a quicker way to determine if two bounding boxes overlap
	inline bool overlaps(const RStarBoundingBox<dimensions>& bb) const
	{
		// do it this way so theres no equal signs (in case of doubles)
		// if (!(x1 < y2) && !(x2 > y1))
		for (std::size_t axis = 0; axis < dimensions; axis++)
		{
			if (!(edges[axis].first < bb.edges[axis].second) || !(bb.edges[axis].first < edges[axis].second))
				return false;
		}

		return true;
	}

	// calculates the total overlapping area of two boxes
	double overlap(const RStarBoundingBox<dimensions>& bb) const
	{
		double area = 1.0;
		for (std::size_t axis = 0; area && axis < dimensions; axis++)
		{
			// this makes it easier to understand
			const double x1 = edges[axis].first;
			const double x2 = edges[axis].second;
			const double y1 = bb.edges[axis].first;
			const double y2 = bb.edges[axis].second;

			// left edge outside left edge
			if (x1 < y1)
			{
				// and right edge inside left edge
				if (y1 < x2)
				{
					// right edge outside right edge
					if (y2 < x2)
						area *= (double)( y2 - y1 );
					else
						area *= (double)( x2 - y1 );

					continue;
				}
			}
			// right edge inside left edge
			else if (x1 < y2)
			{
				// right edge outside right edge
				if (x2 < y2)
					area *= (double)( x2 - x1 );
				else
					area *= (double)( y2 - x1 );

				continue;
			}

			// if we get here, there is no overlap
			return 0.0;
		}

		return area;
	}

	// sums the total distances from the center of another bounding box
	double distanceFromCenter(const RStarBoundingBox<dimensions>& bb) const
	{
		double distance = 0, t;
		for (std::size_t axis = 0; axis < dimensions; axis++)
		{
			t = ((double)edges[axis].first + (double)edges[axis].second +
			     (double)bb.edges[axis].first + (double)bb.edges[axis].second)
				 /2.0;
			distance += t*t;
		}

		return distance;
	}

	// determines if two bounding boxes are identical
	bool operator==(const RStarBoundingBox<dimensions>& bb)
	{
		for (std::size_t axis = 0; axis < dimensions; axis++)
			if (edges[axis].first != bb.edges[axis].first || edges[axis].second != bb.edges[axis].second)
				return false;

		return true;
	}


	// very slow, use for debugging only
	std::string ToString() const
	{
		std::stringstream name("");
		name << "[";
		for (std::size_t axis = 0; axis < dimensions; axis++)
		{
			name << "(" << edges[axis].first << "," << edges[axis].second << ")";
			if (axis != dimensions -1)
				name << ",";
		}
		name << "]";

		return name.str();
	}
};



template <std::size_t dimensions>
struct RStarBoundedItem {
	typedef RStarBoundingBox<dimensions> BoundingBox;

	BoundingBox bound;
};


/**********************************************************
 * Functor used to iterate over a set and stretch a
 * bounding box
 **********************************************************/

// for_each(items.begin(), items.end(), StretchBoundedItem::BoundingBox(bound));
template <typename BoundedItem>
struct StretchBoundingBox :
	public std::unary_function< const BoundedItem * const, void >
{
	typename BoundedItem::BoundingBox * m_bound;
	explicit StretchBoundingBox(typename BoundedItem::BoundingBox * bound) : m_bound(bound) {}

	void operator() (const BoundedItem * const item)
	{
		m_bound->stretch(item->bound);
	}
};


/**********************************************************
 * R* Tree related functors used for sorting BoundedItems
 *
 * TODO: Take advantage of type traits
 **********************************************************/

template <typename BoundedItem>
struct SortBoundedItemsByFirstEdge :
	public std::binary_function< const BoundedItem * const, const BoundedItem * const, bool >
{
	const std::size_t m_axis;
	explicit SortBoundedItemsByFirstEdge (const std::size_t axis) : m_axis(axis) {}

	bool operator() (const BoundedItem * const bi1, const BoundedItem * const bi2) const
	{
		return bi1->bound.edges[m_axis].first < bi2->bound.edges[m_axis].first;
	}
};

template <typename BoundedItem>
struct SortBoundedItemsBySecondEdge :
	public std::binary_function< const BoundedItem * const, const BoundedItem * const, bool >
{
	const std::size_t m_axis;
	explicit SortBoundedItemsBySecondEdge (const std::size_t axis) : m_axis(axis) {}

	bool operator() (const BoundedItem * const bi1, const BoundedItem * const bi2) const
	{
		return bi1->bound.edges[m_axis].second < bi2->bound.edges[m_axis].second;
	}
};


template <typename BoundedItem>
struct SortBoundedItemsByDistanceFromCenter :
	public std::binary_function< const BoundedItem * const, const BoundedItem * const, bool >
{
	const typename BoundedItem::BoundingBox * const m_center;
	explicit SortBoundedItemsByDistanceFromCenter(const typename BoundedItem::BoundingBox * const center) : m_center(center) {}

	bool operator() (const BoundedItem * const bi1, const BoundedItem * const bi2) const
	{
		return bi1->bound.distanceFromCenter(*m_center) < bi2->bound.distanceFromCenter(*m_center);
	}
};

template <typename BoundedItem>
struct SortBoundedItemsByAreaEnlargement :
	public std::binary_function< const BoundedItem * const, const BoundedItem * const, bool >
{
	const double area;
	explicit SortBoundedItemsByAreaEnlargement(const typename BoundedItem::BoundingBox * center) : area(center->area()) {}

	bool operator() (const BoundedItem * const bi1, const BoundedItem * const bi2) const
	{
		return area - bi1->bound.area() < area - bi2->bound.area();
	}
};

template <typename BoundedItem>
struct SortBoundedItemsByOverlapEnlargement :
	public std::binary_function< const BoundedItem * const, const BoundedItem * const, bool >
{
	const typename BoundedItem::BoundingBox * const m_center;
	explicit SortBoundedItemsByOverlapEnlargement(const typename BoundedItem::BoundingBox * const center) : m_center(center) {}

	bool operator() (const BoundedItem * const bi1, const BoundedItem * const bi2) const
	{
		return bi1->bound.overlap(*m_center) < bi2->bound.overlap(*m_center);
	}
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
	\file

	I'm not convinced that these are really the best way to implement
	this, but it works so I'll stick with it for the moment

	It should be noted that all of these items are typedef'ed inside
	of the RStarTree class, so you shouldn't generally need to
	directly use them.
 */


/********************************************************************
 * These are all 'acceptor' functors used for queries and removals,
 * which will have the following characteristics:
 *
 * template<typename Node, typename Leaf>
 *
 *	bool operator()(const Node * node)
 *		-- returns true if this branch should be visited
 *
 *	bool operator()(const Leaf * leaf)
 *		-- returns true if this leaf should be visited
 *
 * This class of functions should be easy to copy, and are expected
 * to be const. They are only used to determine whether something
 * should be visited, and not do the actual visiting.
 *
 ********************************************************************/

// returns true if the node overlaps the specified bound
template <typename Node, typename Leaf>
struct RStarAcceptOverlapping
{
	const typename Node::BoundingBox &m_bound;
	explicit RStarAcceptOverlapping(const typename Node::BoundingBox &bound) : m_bound(bound) {}

	bool operator()(const Node * const node) const
	{
		return m_bound.overlaps(node->bound);
	}

	bool operator()(const Leaf * const leaf) const
	{
		return m_bound.overlaps(leaf->bound);
	}

	private: RStarAcceptOverlapping(){}
};


// returns true if the compared boundary is within the specified bound
template <typename Node, typename Leaf>
struct RStarAcceptEnclosing
{
	const typename Node::BoundingBox &m_bound;
	explicit RStarAcceptEnclosing(const typename Node::BoundingBox &bound) : m_bound(bound) {}

	bool operator()(const Node * const node) const
	{
		return m_bound.overlaps(node->bound);
	}

	bool operator()(const Leaf * const leaf) const
	{
		return m_bound.encloses(leaf->bound);
	}

	private: RStarAcceptEnclosing(){}
};


// will always return true, no matter what
template <typename Node, typename Leaf>
struct RStarAcceptAny
{
	bool operator()(const Node * const node) const { return true; }
	bool operator()(const Leaf * const leaf) const { return true; }
};


/********************************************************************
 * These are all 'visitor' styled functions -- even though these are
 * specifically targeted for removal tasks, visitor classes are
 * specified exactly the same way.
 *
 * bool operator()(RStarLeaf<LeafType, dimensions> * leaf)
 * 		-- Removal: if returns true, then remove the node
 *		-- Visitor: return can actually be void, not used
 *
 * bool ContinueVisiting; (not a function)
 *		-- if false, then the query will end as soon as possible. It
 *		is not guaranteed that the operator() will not be called, so
 *		items may be removed/visited after this is set to false
 *
 * You may modify the items that the leaf points to, but under no
 * circumstance should the bounds of the item be modified (since
 * that would screw up the tree).
 *
 ********************************************************************/


/*
	Default functor used to delete nodes from the R* tree. You can specify
	a different functor to use, as long as it has the same signature as this.
*/
template <typename Leaf>
struct RStarRemoveLeaf{

	const bool ContinueVisiting;
	RStarRemoveLeaf() : ContinueVisiting(true) {}

	bool operator()(const Leaf * const leaf) const
	{
		return true;
	}
};


// returns true if the specific leaf is matched. If remove duplicates is true,
// then it searches for all possible instances of the item
template <typename Leaf>
struct RStarRemoveSpecificLeaf
{
	mutable bool ContinueVisiting;
	bool m_remove_duplicates;
	const typename Leaf::leaf_type &m_leaf;

	explicit RStarRemoveSpecificLeaf(const typename Leaf::leaf_type &leaf, bool remove_duplicates = false) :
		ContinueVisiting(true), m_remove_duplicates(remove_duplicates), m_leaf(leaf) {}

	bool operator()(const Leaf * const leaf) const
	{
		if (ContinueVisiting && m_leaf == leaf->leaf)
		{
			if (!m_remove_duplicates)
				ContinueVisiting = false;
			return true;
		}
		return false;
	}

	private: RStarRemoveSpecificLeaf(){}
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// R* tree parameters
#define RTREE_REINSERT_P 0.30
#define RTREE_CHOOSE_SUBTREE_P 32

// template definition:
#define RSTAR_TEMPLATE


// definition of an leaf
template <typename BoundedItem, typename LeafType>
struct RStarLeaf : BoundedItem {

	typedef LeafType leaf_type;
	LeafType leaf;
};

// definition of a node
template <typename BoundedItem>
struct RStarNode : BoundedItem {
	std::vector< BoundedItem* > items;
	bool hasLeaves;
};



/**
	\class RStarTree
	\brief Implementation of an RTree with an R* index

	@tparam LeafType		type of leaves stored in the tree
	@tparam dimensions  	number of dimensions the bounding boxes are described in
	@tparam	min_child_items m, in the range 2 <= m < M
	@tparam max_child_items M, in the range 2 <= m < M
	@tparam	RemoveLeaf 		A functor used to remove leaves from the tree
*/
template <
	typename LeafType,
	std::size_t dimensions, 
	std::size_t min_child_items, 
	std::size_t max_child_items,	
	typename T=char,
	typename TRAITS=std::char_traits<T>,
	typename ALLOC=std::allocator<T>
>
class RStarTree: public Object<T,TRAITS,ALLOC>{
public:
	virtual std::basic_string<T,TRAITS,ALLOC> getClassName(){
		return "RStarTree";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
    virtual Object<T,TRAITS,ALLOC> * clone() {
		return 0;
	}
public:

	// shortcuts
	typedef RStarBoundedItem<dimensions>		BoundedItem;
	typedef typename BoundedItem::BoundingBox	BoundingBox;

	typedef RStarNode<BoundedItem> 				Node;
	typedef RStarLeaf<BoundedItem, LeafType> 	Leaf;

	// acceptors
	typedef RStarAcceptOverlapping<Node, Leaf>	AcceptOverlapping;
	typedef RStarAcceptEnclosing<Node, Leaf>	AcceptEnclosing;
	typedef RStarAcceptAny<Node, Leaf>			AcceptAny;

	// predefined visitors
	typedef RStarRemoveLeaf<Leaf>				RemoveLeaf;
	typedef RStarRemoveSpecificLeaf<Leaf>		RemoveSpecificLeaf;


	// default constructor
	RStarTree() : m_root(NULL), m_size(0)
	{
		assert(1 <= min_child_items && min_child_items <= max_child_items/2);
	}

	// destructor
	~RStarTree() {
		Remove(
			AcceptAny(),
			RemoveLeaf()
		);
	}
protected:
	struct DefaultVisitor {
		bool ContinueVisiting;
		std::vector<LeafType> * results;
		unsigned long long accessedInnerNodeNumber;
		unsigned long long  accessedLeafNodeNumber;
		unsigned long long  keyComparisionTimes ;

		DefaultVisitor(std::vector<LeafType> * pr) : ContinueVisiting(true),results(pr) {};

		void operator()(const Leaf * const leaf)
		{
			if(results)
				results->push_back(leaf->leaf);
		}
	};

public:
	inline void insert(const double minv[dimensions],const double maxv[dimensions],LeafType leaf)
	{
		BoundingBox bb;
		bb.set(minv,maxv);
		Insert(leaf,bb);
	} 
	inline  std::pair<unsigned long long , unsigned long long> search(const double minv[dimensions],const double maxv[dimensions],std::vector<LeafType> & results)
	{
		BoundingBox bb;
		bb.set(minv,maxv);
		std::pair<unsigned long long , unsigned long long> accessNumbers;
		DefaultVisitor dv= Query(AcceptOverlapping(bb),DefaultVisitor(&results));
		accessNumbers.first=dv.accessedInnerNodeNumber;
		accessNumbers.second=dv.accessedLeafNodeNumber;
		return accessNumbers;
	}
	inline std::pair<unsigned long long , unsigned long long> search(const double minv[dimensions],const double maxv[dimensions],int runtimes=1)
	{
		BoundingBox bb;
		bb.set(minv,maxv);
		std::pair<unsigned long long , unsigned long long> accessNumbers;
		DefaultVisitor dv(0);
		for(int i=0;i<runtimes;i++){
			DefaultVisitor dv = Query(AcceptOverlapping(bb),DefaultVisitor(0));
			accessNumbers.first=dv.accessedInnerNodeNumber;
			accessNumbers.second=dv.accessedLeafNodeNumber;
		}
		return accessNumbers;
	}

	inline void search(const double minv[dimensions],const double maxv[dimensions],std::vector<LeafType> & results,std::vector<unsigned long long >& stats)
	{
		BoundingBox bb;
		bb.set(minv,maxv);
		DefaultVisitor dv= Query(AcceptOverlapping(bb),DefaultVisitor(&results));
		stats.push_back(dv.accessedInnerNodeNumber);
		stats.push_back(dv.accessedLeafNodeNumber);
		stats.push_back(dv.keyComparisionTimes);
	}
protected:
	// Single insert function, adds a new item to the tree
	void Insert(LeafType leaf, const BoundingBox &bound)
	{
		// ID1: Invoke Insert starting with the leaf level as a
		// parameter, to Insert a new data rectangle
		Leaf * newLeaf = new Leaf();
		newLeaf->bound = bound;
		newLeaf->leaf  = leaf;

		// create a new root node if necessary
		if (!m_root)
		{
			m_root = new Node();
			m_root->hasLeaves = true;

			// reserve memory
			m_root->items.reserve(min_child_items);
			m_root->items.push_back(newLeaf);
			m_root->bound = bound;
		}
		else
			// start the insertion process
			InsertInternal(newLeaf, m_root);

		m_size += 1;
	}


	/*
		This is an interpretation of the bulk insert algorithm described
		in "Improving Performance with Bulk-Inserts in Oracle R-Trees"
		by N. An, R. Kanth, V. Kothuri, and S. Ravada

		I think this is essentially right, since if you think about it for too
		long then it makes sense ;) The idea is to work your way down to the
		bottom of the tree, make some child nodes, perform a split,	and work
		your way back up continually. The bounding boxes have to be adjusted
		on the way up the tree, and not on the way down.

	Entries * BulkInsert(Node * node, Node * buddy, vector<Leaf*> &entries)
	{
		if (entries.empty() && !buddy)
			return node;

		if (node->hasLeaves)
			child_entries = node.items + buddy.items + entries;
		else
		{
			combine items in node and buddy;

			for each item in entries?
			for each possible partition in entries
			{
				pick ci and bi using choose subtree, where
				ci is not null, bi can be null

				each entry can only be in one partition

				child_entries += BulkInsert(ci, bi, entries);
			}
		}

		// this part builds up the tree from the ground up, and then
		// passes it back to the parent to be split more until we reach
		// the root node

		// create new nodes: the split algorithm generalized to N

		return rtreeCluster(child_entries);

	}
	*/

	/**
		\brief Touches each node using the visitor pattern

		You must specify an	acceptor functor that takes a BoundingBox and a
		visitor that takes a BoundingBox and a const LeafType&.

		See RStarVisitor.h for more information about the various visitor
		types available.

		@param acceptor 		An acceptor functor that returns true if this
		branch or leaf of the tree should be considered for visitation.

		@param visitor			A visitor functor that does the visiting

		@return This will return the Visitor object, so you can retrieve whatever
		data it has in it if needed (for example, to get the count of items
		visited). It returns by value, so ensure that the copy is cheap
		for decent performance.
	*/
	template <typename Acceptor, typename Visitor>
	Visitor Query(const Acceptor &accept, Visitor visitor)
	{
		if (m_root)
		{
			QueryFunctor<Acceptor, Visitor> query(accept, visitor);
			query(m_root);
			visitor.accessedInnerNodeNumber= query.accessedInnerNodeNumber;
			visitor.accessedLeafNodeNumber= query.accessedLeafNodeNumber;
			visitor.keyComparisionTimes = query.keyComparisionTimes;
		}

		return visitor;
	}


	/**
		\brief Removes item(s) from the tree.

		See RStarVisitor.h for more information about the various visitor
		types available.

		@param acceptor 	A node acceptor functor that returns true if this
		branch or leaf of the tree should be considered for deletion
		(it does not delete it, however. That is what the LeafRemover does).

		@param leafRemover		A visitor functor that decides whether that
		individual item should be removed from the tree. If it returns true,
		then the node holding that item will be deleted.

		See also RemoveBoundedArea, RemoveItem for examples of how this
		function can be called.
	*/
	template <typename Acceptor, typename LeafRemover>
	void Remove( const Acceptor &accept, LeafRemover leafRemover)
	{
		std::list<Leaf*> itemsToReinsert;

		if (!m_root)
			return;

		RemoveFunctor<Acceptor, LeafRemover> remove(accept, leafRemover, &itemsToReinsert, &m_size);
		remove(m_root, true);

		if (!itemsToReinsert.empty())
		{
			// reinsert anything that needs to be reinserted
			typename std::list< Leaf* >::iterator it = itemsToReinsert.begin();
			typename std::list< Leaf* >::iterator end = itemsToReinsert.end();

			// TODO: do this whenever that actually works..
			// BulkInsert(itemsToReinsert, m_root);

			for(;it != end; it++)
				InsertInternal(*it, m_root);
		}
	}

	// stub that removes any items contained in an specified area
	void RemoveBoundedArea( const BoundingBox &bound )
	{
		Remove(AcceptEnclosing(bound), RemoveLeaf());
	}

	// removes a specific item. If removeDuplicates is true, only the first
	// item found will be removed
	void RemoveItem( const LeafType &item, bool removeDuplicates = true )
	{
		Remove( AcceptAny(), RemoveSpecificLeaf(item, removeDuplicates));
	}


	std::size_t GetSize() const { return m_size; }
	std::size_t GetDimensions() const { return dimensions; }


protected:

	// choose subtree: only pass this items that do not have leaves
	// I took out the loop portion of this algorithm, so it only
	// picks a subtree at that particular level
	Node * ChooseSubtree(Node * node, const BoundingBox * bound)
	{
		// If the child pointers in N point to leaves
		if (static_cast<Node*>(node->items[0])->hasLeaves)
		{
			// determine the minimum overlap cost
			if (max_child_items > (RTREE_CHOOSE_SUBTREE_P*2)/3  && node->items.size() > RTREE_CHOOSE_SUBTREE_P)
			{
				// ** alternative algorithm:
				// Sort the rectangles in N in increasing order of
				// then area enlargement needed to include the new
				// data rectangle

				// Let A be the group of the first p entrles
				std::partial_sort( node->items.begin(), node->items.begin() + RTREE_CHOOSE_SUBTREE_P, node->items.end(),
					SortBoundedItemsByAreaEnlargement<BoundedItem>(bound));

				// From the items in A, considering all items in
				// N, choose the leaf whose rectangle needs least
				// overlap enlargement

				return static_cast<Node*>(* std::min_element(node->items.begin(), node->items.begin() + RTREE_CHOOSE_SUBTREE_P,
					SortBoundedItemsByOverlapEnlargement<BoundedItem>(bound)));
			}

			// choose the leaf in N whose rectangle needs least
			// overlap enlargement to include the new data
			// rectangle Resolve ties by choosmg the leaf
			// whose rectangle needs least area enlargement, then
			// the leaf with the rectangle of smallest area

			return static_cast<Node*>(* std::min_element(node->items.begin(), node->items.end(),
				SortBoundedItemsByOverlapEnlargement<BoundedItem>(bound)));
		}

		// if the chlld pointers in N do not point to leaves

		// [determine the minimum area cost],
		// choose the leaf in N whose rectangle needs least
		// area enlargement to include the new data
		// rectangle. Resolve ties by choosing the leaf
		// with the rectangle of smallest area

		return static_cast<Node*>(*	std::min_element( node->items.begin(), node->items.end(),
				SortBoundedItemsByAreaEnlargement<BoundedItem>(bound)));
	}


	// inserts nodes recursively. As an optimization, the algorithm steps are
	// way out of order. :) If this returns something, then that item should
	// be added to the caller's level of the tree
	Node * InsertInternal(Leaf * leaf, Node * node, bool firstInsert = true)
	{
		// I4: Adjust all covering rectangles in the insertion path
		// such that they are minimum bounding boxes
		// enclosing the children rectangles
		node->bound.stretch(leaf->bound);


		// CS2: If we're at a leaf, then use that level
		if (node->hasLeaves)
		{
			// I2: If N has less than M items, accommodate E in N
			node->items.push_back(leaf);
		}
		else
		{
			// I1: Invoke ChooseSubtree. with the level as a parameter,
			// to find an appropriate node N, m which to place the
			// new leaf E

			// of course, this already does all of that recursively. we just need to
			// determine whether we need to split the overflow or not
			Node * tmp_node = InsertInternal( leaf, ChooseSubtree(node, &leaf->bound), firstInsert );

			if (!tmp_node)
				return NULL;

			// this gets joined to the list of items at this level
			node->items.push_back(tmp_node);
		}


		// If N has M+1 items. invoke OverflowTreatment with the
		// level of N as a parameter [for reinsertion or split]
		if (node->items.size() > max_child_items )
		{

			// I3: If OverflowTreatment was called and a split was
			// performed, propagate OverflowTreatment upwards
			// if necessary

			// This is implicit, the rest of the algorithm takes place in there
			return OverflowTreatment(node, firstInsert);
		}

		return NULL;
	}


	// TODO: probably could just merge this in with InsertInternal()
	Node * OverflowTreatment(Node * level, bool firstInsert)
	{
		// OT1: If the level is not the root level AND this is the first
		// call of OverflowTreatment in the given level during the
		// insertion of one data rectangle, then invoke Reinsert
		if (level != m_root && firstInsert)
		{
			Reinsert(level);
			return NULL;
		}

		Node * splitItem = Split(level);

		// If OverflowTreatment caused a split of the root, create a new root
		if (level == m_root)
		{
			Node * newRoot = new Node();
			newRoot->hasLeaves = false;

			// reserve memory
			newRoot->items.reserve(min_child_items);
			newRoot->items.push_back(m_root);
			newRoot->items.push_back(splitItem);

			// Do I4 here for the new root item
			newRoot->bound.reset();
			for_each(newRoot->items.begin(), newRoot->items.end(), StretchBoundingBox<BoundedItem>(&newRoot->bound));

			// and we're done
			m_root = newRoot;
			return NULL;
		}

		// propagate it upwards
		return splitItem;
	}

	// this combines Split, ChooseSplitAxis, and ChooseSplitIndex into
	// one function as an optimization (they all share data structures,
	// so it would be pointless to do all of that copying)
	//
	// This returns a node, which should be added to the items of the
	// passed node's parent
	Node * Split(Node * node)
	{
		Node * newNode = new Node();
		newNode->hasLeaves = node->hasLeaves;

		const std::size_t n_items = node->items.size();
		const std::size_t distribution_count = n_items - 2*min_child_items + 1;

		std::size_t split_axis = dimensions+1, split_edge = 0, split_index = 0;
		int split_margin = 0;

		BoundingBox R1, R2;

		// these should always hold true
		assert(n_items == max_child_items + 1);
		assert(distribution_count > 0);
		assert(min_child_items + distribution_count-1 <= n_items);

		// S1: Invoke ChooseSplitAxis to determine the axis,
		// perpendicular to which the split 1s performed
		// S2: Invoke ChooseSplitIndex to determine the best
		// distribution into two groups along that axis

		// NOTE: We don't compare against node->bound, so it gets overwritten
		// at the end of the loop

		// CSA1: For each axis
		for (std::size_t axis = 0; axis < dimensions; axis++)
		{
			// initialize per-loop items
			int margin = 0;
			double overlap = 0, dist_area, dist_overlap;
			std::size_t dist_edge = 0, dist_index = 0;

            dist_overlap = DBL_MAX;//std::numeric_limits<double>::max();

			dist_area = dist_overlap;
			// Sort the items by the lower then by the upper
			// edge of their bounding box on this particular axis and
			// determine all distributions as described . Compute S. the
			// sum of all margin-values of the different
			// distributions

			// lower edge == 0, upper edge = 1
			for (std::size_t edge = 0; edge < 2; edge++)
			{
				// sort the items by the correct key (upper edge, lower edge)
				if (edge == 0)
					std::sort(node->items.begin(), node->items.end(), SortBoundedItemsByFirstEdge<BoundedItem>(axis));
				else
					std::sort(node->items.begin(), node->items.end(), SortBoundedItemsBySecondEdge<BoundedItem>(axis));

				// Distributions: pick a point m in the middle of the thing, call the left
				// R1 and the right R2. Calculate the bounding box of R1 and R2, then
				// calculate the margins. Then do it again for some more points
				for (std::size_t k = 0; k < distribution_count; k++)
		        {
					double area = 0;

					// calculate bounding box of R1
					R1.reset();
					for_each(node->items.begin(), node->items.begin()+(min_child_items+k), StretchBoundingBox<BoundedItem>(&R1));

					// then do the same for R2
					R2.reset();
					for_each(node->items.begin()+(min_child_items+k+1), node->items.end(), StretchBoundingBox<BoundedItem>(&R2));


					// calculate the three values
					margin 	+= R1.edgeDeltas() + R2.edgeDeltas();
					area 	+= R1.area() + R2.area();		// TODO: need to subtract.. overlap?
					overlap =  R1.overlap(R2);


					// CSI1: Along the split axis, choose the distribution with the
					// minimum overlap-value. Resolve ties by choosing the distribution
					// with minimum area-value.
					if (overlap < dist_overlap || (overlap == dist_overlap && area < dist_area))
					{
						// if so, store the parameters that allow us to recreate it at the end
						dist_edge = 	edge;
						dist_index = 	min_child_items+k;
						dist_overlap = 	overlap;
						dist_area = 	area;
					}
				}
			}

			// CSA2: Choose the axis with the minimum S as split axis
			if (split_axis == dimensions+1 || split_margin > margin )
			{
				split_axis 		= axis;
				split_margin 	= margin;
				split_edge 		= dist_edge;
				split_index 	= dist_index;
			}
		}

		// S3: Distribute the items into two groups

		// ok, we're done, and the best distribution on the selected split
		// axis has been recorded, so we just have to recreate it and
		// return the correct index

		if (split_edge == 0)
			std::sort(node->items.begin(), node->items.end(), SortBoundedItemsByFirstEdge<BoundedItem>(split_axis));

		// only reinsert the sort key if we have to
		else if (split_axis != dimensions-1)
			std::sort(node->items.begin(), node->items.end(), SortBoundedItemsBySecondEdge<BoundedItem>(split_axis));

		// distribute the end of the array to the new node, then erase them from the original node
		newNode->items.assign(node->items.begin() + split_index, node->items.end());
		node->items.erase(node->items.begin() + split_index, node->items.end());

		// adjust the bounding box for each 'new' node
		node->bound.reset();
		std::for_each(node->items.begin(), node->items.end(), StretchBoundingBox<BoundedItem>(&node->bound));

		newNode->bound.reset();
		std::for_each(newNode->items.begin(), newNode->items.end(), StretchBoundingBox<BoundedItem>(&newNode->bound));

		return newNode;
	}

	// This routine is used to do the opportunistic reinsertion that the
	// R* algorithm calls for
	void Reinsert(Node * node)
	{
		std::vector< BoundedItem* > removed_items;

		const std::size_t n_items = node->items.size();
		const std::size_t p = (std::size_t)((double)n_items * RTREE_REINSERT_P) > 0 ? (std::size_t)((double)n_items * RTREE_REINSERT_P) : 1;

		// RI1 For all M+l items of a node N, compute the distance
		// between the centers of their rectangles and the center
		// of the bounding rectangle of N
		assert(n_items == max_child_items + 1);

		// RI2: Sort the items in increasing order of their distances
		// computed in RI1
		std::partial_sort(node->items.begin(), node->items.end() - p, node->items.end(),
			SortBoundedItemsByDistanceFromCenter<BoundedItem>(&node->bound));

		// RI3.A: Remove the last p items from N
		removed_items.assign(node->items.end() - p, node->items.end());
		node->items.erase(node->items.end() - p, node->items.end());

		// RI3.B: adjust the bounding rectangle of N
		node->bound.reset();
		for_each(node->items.begin(), node->items.end(), StretchBoundingBox<BoundedItem>(&node->bound));

		// RI4: In the sort, defined in RI2, starting with the
		// minimum distance (= close reinsert), invoke Insert
		// to reinsert the items
		for (typename std::vector< BoundedItem* >::iterator it = removed_items.begin(); it != removed_items.end(); it++)
			InsertInternal( static_cast<Leaf*>(*it), m_root, false);
	}

	/****************************************************************
	 * These are used to implement walking the entire R* tree in a
	 * conditional way
	 ****************************************************************/

	// visits a node if necessary
	template <typename Acceptor, typename Visitor>
	struct VisitFunctor : std::unary_function< const BoundingBox *, void > {

		const Acceptor &accept;
		Visitor &visit;

		explicit VisitFunctor(const Acceptor &a, Visitor &v) : accept(a), visit(v) {}

		void operator()( BoundedItem * item )
		{
			Leaf * leaf = static_cast<Leaf*>(item);

			if (accept(leaf))
				visit(leaf);
		}
	};


	// this functor recursively walks the tree
	template <typename Acceptor, typename Visitor>
	struct QueryFunctor : std::unary_function< const BoundedItem, void > {
		const Acceptor &accept;
		Visitor &visitor;
		unsigned long long accessedLeafNodeNumber;
		unsigned long long accessedInnerNodeNumber;
		unsigned long long keyComparisionTimes;
		explicit QueryFunctor(const Acceptor &a, Visitor &v) : accept(a), visitor(v),keyComparisionTimes(0),accessedLeafNodeNumber(0),accessedInnerNodeNumber(0) {}

		void operator()(BoundedItem * item)
		{
			Node * node = static_cast<Node*>(item);
			accessedInnerNodeNumber+=1;
			keyComparisionTimes++;
			if (visitor.ContinueVisiting && accept(node))
			{
				if (node->hasLeaves){
					for_each(node->items.begin(), node->items.end(), VisitFunctor<Acceptor, Visitor>(accept, visitor));
					keyComparisionTimes += node->items.size();
					accessedLeafNodeNumber+=node->items.size();
				}
				else{
					
					for_each(node->items.begin(), node->items.end(), *this);
					keyComparisionTimes += node->items.size();
					accessedInnerNodeNumber+=node->items.size();
				}
			}
		}
	};


	/****************************************************************
	 * Used to remove items from the tree
	 *
	 * At some point, the complexity just gets ridiculous. I'm pretty
	 * sure that the remove functions are close to that by now...
	 ****************************************************************/



	// determines whether a leaf should be deleted or not
	template <typename Acceptor, typename LeafRemover>
	struct RemoveLeafFunctor :
		std::unary_function< const BoundingBox *, bool >
	{
		const Acceptor &accept;
		LeafRemover &remove;
		std::size_t * size;

		explicit RemoveLeafFunctor(const Acceptor &a, LeafRemover &r, std::size_t * s) :
			accept(a), remove(r), size(s) {}

		bool operator()(BoundedItem * item ) const {
			Leaf * leaf = static_cast<Leaf *>(item);

			if (accept(leaf) && remove(leaf))
			{
				--(*size);
				delete leaf;
				return true;
			}

			return false;
		}
	};


	template <typename Acceptor, typename LeafRemover>
	struct RemoveFunctor :
		std::unary_function< const BoundedItem *, bool >
	{
		const Acceptor &accept;
		LeafRemover &remove;

		// parameters that are passed in
		std::list<Leaf*> * itemsToReinsert;
		std::size_t * m_size;

		// the third parameter is a list that the items that need to be reinserted
		// are put into
		explicit RemoveFunctor(const Acceptor &na, LeafRemover &lr, std::list<Leaf*>* ir, std::size_t * size)
			: accept(na), remove(lr), itemsToReinsert(ir), m_size(size) {}

		bool operator()(BoundedItem * item, bool isRoot = false)
		{
			Node * node = static_cast<Node*>(item);

			if (accept(node))
			{
				// this is the easy part: remove nodes if they need to be removed
				if (node->hasLeaves)
					node->items.erase(std::remove_if(node->items.begin(), node->items.end(), RemoveLeafFunctor<Acceptor, LeafRemover>(accept, remove, m_size)), node->items.end());
				else
					node->items.erase(std::remove_if(node->items.begin(), node->items.end(), *this), node->items.end() );

				if (!isRoot)
				{
					if (node->items.empty())
					{
						// tell parent to remove us if theres nothing left
						delete node;
						return true;
					}
					else if (node->items.size() < min_child_items)
					{
						// queue up the items that need to be reinserted
						QueueItemsToReinsert(node);
						return true;
					}
				}
				else if (node->items.empty())
				{
					// if the root node is empty, setting these won't hurt
					// anything, since the algorithms don't actually require
					// the nodes to have anything in them.
					node->hasLeaves = true;
					node->bound.reset();
				}
			}

			// anything else, don't remove it
			return false;

		}

		// theres probably a better way to do this, but this
		// traverses and finds any leaves, and adds them to a
		// list of items that will later be reinserted
		void QueueItemsToReinsert(Node * node)
		{
			typename std::vector< BoundedItem* >::iterator it = node->items.begin();
			typename std::vector< BoundedItem* >::iterator end = node->items.end();

			if (node->hasLeaves)
			{
				for(; it != end; it++)
					itemsToReinsert->push_back(static_cast<Leaf*>(*it));
			}
			else
				for (; it != end; it++)
					QueueItemsToReinsert(static_cast<Node*>(*it));

			delete node;
		}
	};


private:
	Node * m_root;

	std::size_t m_size;

	void numb_nodes(Node * root,unsigned long long & inner_nodes, unsigned long long & leaves){
		if(root){
			if(root->hasLeaves){
				inner_nodes++;
				leaves+=root->items.size();
				return ;
			}
			else {
				inner_nodes++;
				for(int i=0;i<root->items.size();i++){
					numb_nodes((Node*)(root->items[i]),inner_nodes,leaves);
				}
			}
		}
	}
public:
	void getObjectNumber(unsigned long long &objectNumber, unsigned long long & nodeNumber){
		objectNumber=m_size;
		nodeNumber=0;
		unsigned long long leaves=0;
		numb_nodes(m_root,nodeNumber,leaves);
		assert(objectNumber==leaves);
	}
	unsigned long long numberNodes( ){
		 
		unsigned long long  nodeNumber=0;
		unsigned long long leaves=0;
		numb_nodes(m_root,nodeNumber,leaves);
		assert(m_size==leaves);
		return nodeNumber;
	}
};

#undef RSTAR_TEMPLATE

#undef RTREE_SPLIT_M
#undef RTREE_REINSERT_P
#undef RTREE_CHOOSE_SUBTREE_P



end_gtl_namespace
