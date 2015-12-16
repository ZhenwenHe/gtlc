#pragma once

// NOTE This file compiles under MSVC 6 SP5 and MSVC .Net 2003 it may not work on other compilers without modification.

// NOTE These next few lines may be win32 specific, you may need to modify them to compile on other platform
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <vector>
#include <set>
#include "gtl.h"
#include "Identifier.h"
#include "buffer.h"
 
begin_gtl_namespace

//屏蔽警告
#pragma warning(push)
#pragma warning(disable:4005)
#define ASSERT assert // RTree uses ASSERT( condition )
#pragma warning(pop)

//重新定义最大最小函数
#ifndef Min
  #define Min __min 
#endif //Min
#ifndef Max
  #define Max __max 
#endif //Max

//
// RTree.h
//

#define RTREEBASE_TEMPLATE template<typename DATATYPE, typename ELEMTYPE, int NUMDIMS, typename ELEMTYPEREAL, int TMAXNODES, int TMINNODES,typename T, typename TRAITS,typename ALLOC>
#define RTREEBASE_QUAL RTreeBase<DATATYPE, ELEMTYPE, NUMDIMS, ELEMTYPEREAL, TMAXNODES, TMINNODES,T,TRAITS,ALLOC>

#define RTREE_DONT_USE_MEMPOOLS // This version does not contain a fixed memory allocator, fill in lines with EXAMPLE to implement one.
#define RTREE_USE_SPHERICAL_VOLUME // Better split classification, may be slower on some systems

// Fwd decl


/// \class RTree
/// Implementation of RTree, a multidimensional bounding rectangle tree.
/// Example usage: For a 3-dimensional tree use RTree<Object*, float, 3> myTree;
///
/// This modified, templated C++ version by Greg Douglas at Auran (http://www.auran.com)
///
/// DATATYPE Referenced data, should be int, void*, obj* etc. no larger than sizeof<void*> and simple type
/// ELEMTYPE Type of element such as int or float
/// NUMDIMS Number of dimensions such as 2 or 3
/// ELEMTYPEREAL Type of element that allows fractional and large values such as float or double, for use in volume calcs
///
/// NOTES: Inserting and removing data requires the knowledge of its constant Minimal Bounding Rectangle.
///        This version uses new/delete for nodes, I recommend using a fixed size allocator for efficiency.
///        Instead of using a callback function for returned results, I recommend and efficient pre-sized, grow-only memory
///        array similar to MFC CArray or STL Vector for returning search query result.
///
template<
	typename DATATYPE, 
	typename ELEMTYPE, 
	int NUMDIMS, 
	typename ELEMTYPEREAL = ELEMTYPE,
	int TMAXNODES = 8,
	int TMINNODES = TMAXNODES / 2,

	// template paramenters for Object class 
	typename T=char,
	typename TRAITS=std::char_traits<T>,
	typename ALLOC=std::allocator<T>
>
class RTreeBase : public Object <T,TRAITS,ALLOC> {
public:
	virtual std::basic_string<T,TRAITS,ALLOC> getClassName(){
		return "RTreeBase";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object <T,TRAITS,ALLOC> * clone() {
		return 0;
	}
public:
	unsigned long long accessedLeafNodeNumber;
	unsigned long long accessedInnerNodeNumber;
	unsigned long long splitNumber;
	unsigned long long comparisonNumber;
public:
	struct Node;  // Fwd decl.  Used by other internal structs and iterator
	struct Branch;
	struct Point{
		ELEMTYPE m_pos[NUMDIMS];
	};

  // These constant must be declared after Branch and before Node struct
  // Stuck up here for MSVC 6 compiler.  NSVC .NET 2003 is much happier.
  enum
  {
    MAXNODES = TMAXNODES,                         ///< Max elements in node
    MINNODES = TMINNODES,                         ///< Min elements in node
  };


public:

  RTreeBase();
  virtual ~RTreeBase();
  
  /// Insert entry
  /// \param a_min Min of bounding rect
  /// \param a_max Max of bounding rect
  /// \param a_dataId Positive Id of data.  Maybe zero, but negative numbers not allowed.
  void Insert(const ELEMTYPE a_min[NUMDIMS], const ELEMTYPE a_max[NUMDIMS], const DATATYPE& a_dataId);
  
  /// Remove entry
  /// \param a_min Min of bounding rect
  /// \param a_max Max of bounding rect
  /// \param a_dataId Positive Id of data.  Maybe zero, but negative numbers not allowed.
  void Remove(const ELEMTYPE a_min[NUMDIMS], const ELEMTYPE a_max[NUMDIMS], const DATATYPE& a_dataId);
  
  /// Find all within search rectangle
  /// \param a_min Min of search bounding rect
  /// \param a_max Max of search bounding rect
  /// \param a_searchResult Search result array.  Caller should set grow size. Function will reset, not append to array.
  /// \param a_resultCallback Callback function to return result.  Callback should return 'true' to continue searching
  /// \param a_context User context to pass as parameter to a_resultCallback
  /// \return Returns the number of entries found
  int Search(const ELEMTYPE a_min[NUMDIMS], const ELEMTYPE a_max[NUMDIMS], bool __cdecl a_resultCallback(DATATYPE a_data, void* a_context), void* a_context);
  int Search(const ELEMTYPE a_min[NUMDIMS], const ELEMTYPE a_max[NUMDIMS],std::vector<DATATYPE>&a_data);//用于包围盒查询
  int Search(const ELEMTYPE a_min[NUMDIMS], const ELEMTYPE a_max[NUMDIMS],std::set<DATATYPE>&a_arrid);//用于包围盒查询
  int Search(std::vector<Point> a_line,std::set<DATATYPE>&a_arrid);//主要用于线查询和面查询
  
  /// Remove all entries from tree
  void RemoveAll();

  /// Count the data elements in this container.  This is slow as no internal counter is maintained.
  int Count();
  //write in file
  void write(std::ostream & f,std::vector<DATATYPE> &a_vecdata);
  //read from file
  void read(std::istream & f,std::vector<DATATYPE> &a_vecdata);

  // 计算size
  size_t sizeBuffer();
  //写buffer
  virtual bool writeBuffer(Buffer<char>& buf);
  //读buffer
  virtual  bool readBuffer(const Buffer<char> & buf);

  Node* AllocNode();

protected:

  /// Minimal bounding rectangle (n-dimensional)
  struct Rect
  {
    ELEMTYPE m_min[NUMDIMS];                      ///< Min dimensions of bounding box 
    ELEMTYPE m_max[NUMDIMS];                      ///< Max dimensions of bounding box 
  };

  /// May be data or may be another subtree
  /// The parents level determines this.
  /// If the parents level is 0, then this is data
  struct Branch
  {
    Rect m_rect;                                  ///< Bounds
    Node* m_child;                              ///< Child node
    DATATYPE m_data;                            ///< Data Id or Ptr
  };

  /// Node for each branch level
  struct Node
  {
    bool IsInternalNode()                         { return (m_level > 0); } // Not a leaf, but a internal node
    bool IsLeaf()                                 { return (m_level == 0); } // A leaf, contains data
    
    int m_count;                                  ///< Count
    int m_level;                                  ///< Leaf is zero, others positive
    Branch m_branch[MAXNODES];                    ///< Branch
  };
  
  /// A link list of nodes for reinsertion after a delete operation
  struct ListNode
  {
    ListNode* m_next;                             ///< Next in list
    Node* m_node;                                 ///< Node
  };

  /// Variables for finding a split partition
  struct PartitionVars
  {
    int m_partition[MAXNODES+1];
    int m_total;
    int m_minFill;
    int m_taken[MAXNODES+1];
    int m_count[2];
    Rect m_cover[2];
    ELEMTYPEREAL m_area[2];

    Branch m_branchBuf[MAXNODES+1];
    int m_branchCount;
    Rect m_coverSplit;
    ELEMTYPEREAL m_coverSplitArea;
  }; 
 
  void FreeNode(Node* a_node);
  void InitNode(Node* a_node);
  void InitRect(Rect* a_rect);
  bool InsertRectRec(Rect* a_rect, const DATATYPE& a_id, Node* a_node, Node** a_newNode, int a_level);
  bool InsertRect(Rect* a_rect, const DATATYPE& a_id, Node** a_root, int a_level);
  Rect NodeCover(Node* a_node);
  bool AddBranch(Branch* a_branch, Node* a_node, Node** a_newNode);
  void DisconnectBranch(Node* a_node, int a_index);
  int PickBranch(Rect* a_rect, Node* a_node);
  Rect CombineRect(Rect* a_rectA, Rect* a_rectB);
  void SplitNode(Node* a_node, Branch* a_branch, Node** a_newNode);
  ELEMTYPEREAL RectSphericalVolume(Rect* a_rect);
  ELEMTYPEREAL RectVolume(Rect* a_rect);
  ELEMTYPEREAL CalcRectVolume(Rect* a_rect);
  void GetBranches(Node* a_node, Branch* a_branch, PartitionVars* a_parVars);
  void ChoosePartition(PartitionVars* a_parVars, int a_minFill);
  void LoadNodes(Node* a_nodeA, Node* a_nodeB, PartitionVars* a_parVars);
  void InitParVars(PartitionVars* a_parVars, int a_maxRects, int a_minFill);
  void PickSeeds(PartitionVars* a_parVars);
  void Classify(int a_index, int a_group, PartitionVars* a_parVars);
  bool RemoveRect(Rect* a_rect, const DATATYPE& a_id, Node** a_root);
  bool RemoveRectRec(Rect* a_rect, const DATATYPE& a_id, Node* a_node, ListNode** a_listNode);
  ListNode* AllocListNode();
  void FreeListNode(ListNode* a_listNode);
  bool Overlap(Rect* a_rectA, Rect* a_rectB);
  void ReInsert(Node* a_node, ListNode** a_listNode);
  bool Search(Node* a_node, Rect* a_rect, int& a_foundCount, bool __cdecl a_resultCallback(DATATYPE a_data, void* a_context), void* a_context);
  bool Search(Node* a_node, Rect* a_rect, int& a_foundCount, std::vector<DATATYPE>&a_data);
  bool Search(Node* a_node, Rect* a_rect, int& a_foundCount,std::set<DATATYPE> &a_arrid);
  void RemoveAllRec(Node* a_node);
  void Reset();
  void CountRec(Node* a_node, int& a_count);


//有没有意义现在没有办法知道，需要确认
  void writeNode(Node * pnode,std::ostream &f,std::vector<DATATYPE> &a_vecdata);
  void readNode(Node * pnode,std::istream & f,std::vector<DATATYPE> &a_vecdata);
  unsigned int  findPos(DATATYPE a_data,std::vector<DATATYPE> &a_vecdata);
  size_t sizeNodeBuffer(Node* a_node);
  bool writeNodeBuffer(Node * pnode,Buffer<char> &buf,size_t s);
  bool readNodeBuffer(Node * pnode,const Buffer<char>& buf);
  bool insertNode(Node * a_node,Node** a_root);



  Node* m_root;                                    ///< Root of tree
  ELEMTYPEREAL m_unitSphereVolume;                 ///< Unit sphere constant for required number of dimensions
};




RTREEBASE_TEMPLATE
RTREEBASE_QUAL::RTreeBase()
{
  ASSERT(MAXNODES > MINNODES);
  ASSERT(MINNODES > 0);

  accessedLeafNodeNumber=0;
  accessedInnerNodeNumber=0;
  comparisonNumber=0;
  splitNumber=0;

  // We only support machine word size simple data type eg. integer index or object pointer.
  // Since we are storing as union with non data branch
 // ASSERT(sizeof(DATATYPE) == sizeof(void*) || sizeof(DATATYPE) == sizeof(int));

  // Precomputed volumes of the unit spheres for the first few dimensions
  const float UNIT_SPHERE_VOLUMES[] = {
    0.000000f, 2.000000f, 3.141593f, // Dimension  0,1,2
    4.188790f, 4.934802f, 5.263789f, // Dimension  3,4,5
    5.167713f, 4.724766f, 4.058712f, // Dimension  6,7,8
    3.298509f, 2.550164f, 1.884104f, // Dimension  9,10,11
    1.335263f, 0.910629f, 0.599265f, // Dimension  12,13,14
    0.381443f, 0.235331f, 0.140981f, // Dimension  15,16,17
    0.082146f, 0.046622f, 0.025807f, // Dimension  18,19,20 
  };

  m_root = AllocNode();
  m_root->m_level = 0;
  m_unitSphereVolume = (ELEMTYPEREAL)UNIT_SPHERE_VOLUMES[NUMDIMS];
}


RTREEBASE_TEMPLATE
RTREEBASE_QUAL::~RTreeBase()
{
  Reset(); // Free, or reset node memory
}


RTREEBASE_TEMPLATE
void RTREEBASE_QUAL::Insert(const ELEMTYPE a_min[NUMDIMS], const ELEMTYPE a_max[NUMDIMS], const DATATYPE& a_dataId)
{
#ifdef _DEBUG
  for(int index=0; index<NUMDIMS; ++index)
  {
    ASSERT(a_min[index] <= a_max[index]);
  }
#endif //_DEBUG

  Rect rect;
  
  for(int axis=0; axis<NUMDIMS; ++axis)
  {
    rect.m_min[axis] = a_min[axis];
    rect.m_max[axis] = a_max[axis];
  }
  
  InsertRect(&rect, a_dataId, &m_root, 0);
}


RTREEBASE_TEMPLATE
void RTREEBASE_QUAL::Remove(const ELEMTYPE a_min[NUMDIMS], const ELEMTYPE a_max[NUMDIMS], const DATATYPE& a_dataId)
{
#ifdef _DEBUG
  for(int index=0; index<NUMDIMS; ++index)
  {
    ASSERT(a_min[index] <= a_max[index]);
  }
#endif //_DEBUG

  Rect rect;
  
  for(int axis=0; axis<NUMDIMS; ++axis)
  {
    rect.m_min[axis] = a_min[axis];
    rect.m_max[axis] = a_max[axis];
  }

  RemoveRect(&rect, a_dataId, &m_root);
}


RTREEBASE_TEMPLATE
int RTREEBASE_QUAL::Search(const ELEMTYPE a_min[NUMDIMS], const ELEMTYPE a_max[NUMDIMS], bool __cdecl a_resultCallback(DATATYPE a_data, void* a_context), void* a_context)
{
#ifdef _DEBUG
  for(int index=0; index<NUMDIMS; ++index)
  {
    ASSERT(a_min[index] <= a_max[index]);
  }
#endif //_DEBUG
  Rect rect;
  for(int axis=0; axis<NUMDIMS; ++axis)
  {
    rect.m_min[axis] = a_min[axis];
    rect.m_max[axis] = a_max[axis];
  }
  // NOTE: May want to return search result another way, perhaps returning the number of found elements here.
  int foundCount = 0;
  Search(m_root, &rect, foundCount, a_resultCallback, a_context);
  return foundCount;
}
RTREEBASE_TEMPLATE
int RTREEBASE_QUAL::Search(const ELEMTYPE a_min[NUMDIMS], const ELEMTYPE a_max[NUMDIMS], std::vector<DATATYPE>&a_data)
{
#ifdef _DEBUG
	for(int index=0; index<NUMDIMS; ++index)
	{
		ASSERT(a_min[index] <= a_max[index]);
	}
#endif //_DEBUG

	Rect rect;
	for(int axis=0; axis<NUMDIMS; ++axis)
	{
		rect.m_min[axis] = a_min[axis];
		rect.m_max[axis] = a_max[axis];
	}
	// NOTE: May want to return search result another way, perhaps returning the number of found elements here.
	int foundCount = 0;
	Search(m_root, &rect, foundCount, a_data);
	return foundCount;
}
RTREEBASE_TEMPLATE
int RTREEBASE_QUAL::Search(const ELEMTYPE a_min[NUMDIMS], const ELEMTYPE a_max[NUMDIMS], std::set<DATATYPE>&a_data)
{
#ifdef _DEBUG
	for(int index=0; index<NUMDIMS; ++index)
	{
		ASSERT(a_min[index] <= a_max[index]);
	}
#endif //_DEBUG

	Rect rect;
	for(int axis=0; axis<NUMDIMS; ++axis)
	{
		rect.m_min[axis] = a_min[axis];
		rect.m_max[axis] = a_max[axis];
	}
	// NOTE: May want to return search result another way, perhaps returning the number of found elements here.
	int foundCount = 0;
	Search(m_root, &rect, foundCount, a_data);
	return foundCount;
}


RTREEBASE_TEMPLATE
int RTREEBASE_QUAL::Count()
{
  int count = 0;
  CountRec(m_root, count);
  return count;
}



RTREEBASE_TEMPLATE
void RTREEBASE_QUAL::CountRec(Node* a_node, int& a_count)
{
  if(a_node->IsInternalNode())  // not a leaf node
  {
    for(int index = 0; index < a_node->m_count; ++index)
    {
      CountRec(a_node->m_branch[index].m_child, a_count);
    }
  }
  else // A leaf node
  {
    a_count += a_node->m_count;
  }
}

RTREEBASE_TEMPLATE
void RTREEBASE_QUAL::RemoveAll()
{
  // Delete all existing nodes
  Reset();
  m_root = AllocNode();
  m_root->m_level = 0;
}


RTREEBASE_TEMPLATE
void RTREEBASE_QUAL::Reset()
{
#ifdef RTREE_DONT_USE_MEMPOOLS
  // Delete all existing nodes
  RemoveAllRec(m_root);
#else // RTREE_DONT_USE_MEMPOOLS
  // Just reset memory pools.  We are not using complex types
  // EXAMPLE
#endif // RTREE_DONT_USE_MEMPOOLS
}


RTREEBASE_TEMPLATE
void RTREEBASE_QUAL::RemoveAllRec(Node* a_node)
{
  ASSERT(a_node);
  ASSERT(a_node->m_level >= 0);

  if(a_node->IsInternalNode()) // This is an internal node in the tree
  {
    for(int index=0; index < a_node->m_count; ++index)
    {
      RemoveAllRec(a_node->m_branch[index].m_child);
    }
  }
  FreeNode(a_node); 
}


RTREEBASE_TEMPLATE
typename RTREEBASE_QUAL::Node* RTREEBASE_QUAL::AllocNode()
{
  Node* newNode;
#ifdef RTREE_DONT_USE_MEMPOOLS
  newNode = new Node;
#else // RTREE_DONT_USE_MEMPOOLS
  // EXAMPLE
#endif // RTREE_DONT_USE_MEMPOOLS
  InitNode(newNode);
  return newNode;
}


RTREEBASE_TEMPLATE
void RTREEBASE_QUAL::FreeNode(Node* a_node)
{
  ASSERT(a_node);
#ifdef RTREE_DONT_USE_MEMPOOLS
  delete a_node;
#else // RTREE_DONT_USE_MEMPOOLS
  // EXAMPLE
#endif // RTREE_DONT_USE_MEMPOOLS
}


// Allocate space for a node in the list used in DeletRect to
// store Nodes that are too empty.
RTREEBASE_TEMPLATE
typename RTREEBASE_QUAL::ListNode* RTREEBASE_QUAL::AllocListNode()
{
#ifdef RTREE_DONT_USE_MEMPOOLS
  return new ListNode;
#else // RTREE_DONT_USE_MEMPOOLS
  // EXAMPLE
#endif // RTREE_DONT_USE_MEMPOOLS
}


RTREEBASE_TEMPLATE
void RTREEBASE_QUAL::FreeListNode(ListNode* a_listNode)
{
#ifdef RTREE_DONT_USE_MEMPOOLS
  delete a_listNode;
#else // RTREE_DONT_USE_MEMPOOLS
  // EXAMPLE
#endif // RTREE_DONT_USE_MEMPOOLS
}


RTREEBASE_TEMPLATE
void RTREEBASE_QUAL::InitNode(Node* a_node)
{
  a_node->m_count = 0;
  a_node->m_level = -1;
}


RTREEBASE_TEMPLATE
void RTREEBASE_QUAL::InitRect(Rect* a_rect)
{
  for(int index = 0; index < NUMDIMS; ++index)
  {
    a_rect->m_min[index] = (ELEMTYPE)0;
    a_rect->m_max[index] = (ELEMTYPE)0;
  }
}


// Inserts a new data rectangle into the index structure.
// Recursively descends tree, propagates splits back up.
// Returns 0 if node was not split.  Old node updated.
// If node was split, returns 1 and sets the pointer pointed to by
// new_node to point to the new node.  Old node updated to become one of two.
// The level argument specifies the number of steps up from the leaf
// level to insert; e.g. a data rectangle goes in at level = 0.
RTREEBASE_TEMPLATE
bool RTREEBASE_QUAL::InsertRectRec(Rect* a_rect, const DATATYPE& a_id, Node* a_node, Node** a_newNode, int a_level)
{
  ASSERT(a_rect && a_node && a_newNode);
  ASSERT(a_level >= 0 && a_level <= a_node->m_level);

  int index;
  Branch branch;
  Node* otherNode;

  // Still above level for insertion, go down tree recursively
  if(a_node->m_level > a_level)
  {
    index = PickBranch(a_rect, a_node);
    if (!InsertRectRec(a_rect, a_id, a_node->m_branch[index].m_child, &otherNode, a_level))
    {
      // Child was not split
      a_node->m_branch[index].m_rect = CombineRect(a_rect, &(a_node->m_branch[index].m_rect));
      return false;
    }
    else // Child was split
    {
      a_node->m_branch[index].m_rect = NodeCover(a_node->m_branch[index].m_child);
      branch.m_child = otherNode;
      branch.m_rect = NodeCover(otherNode);
      return AddBranch(&branch, a_node, a_newNode);
    }
  }
  else if(a_node->m_level == a_level) // Have reached level for insertion. Add rect, split if necessary
  {
    branch.m_rect = *a_rect;
    branch.m_data =  a_id;
    // Child field of leaves contains id of data record
    return AddBranch(&branch, a_node, a_newNode);
  }
  else
  {
    // Should never occur
    ASSERT(0);
    return false;
  }
}


// Insert a data rectangle into an index structure.
// InsertRect provides for splitting the root;
// returns 1 if root was split, 0 if it was not.
// The level argument specifies the number of steps up from the leaf
// level to insert; e.g. a data rectangle goes in at level = 0.
// InsertRect2 does the recursion.
//
RTREEBASE_TEMPLATE
bool RTREEBASE_QUAL::InsertRect(Rect* a_rect, const DATATYPE& a_id, Node** a_root, int a_level)
{
  ASSERT(a_rect && a_root);
  ASSERT(a_level >= 0 && a_level <= (*a_root)->m_level);
#ifdef _DEBUG
  for(int index=0; index < NUMDIMS; ++index)
  {
    ASSERT(a_rect->m_min[index] <= a_rect->m_max[index]);
  }
#endif //_DEBUG  

  Node* newRoot;
  Node* newNode;
  Branch branch;

  if(InsertRectRec(a_rect, a_id, *a_root, &newNode, a_level))  // Root split
  {
    newRoot = AllocNode();  // Grow tree taller and new root
    newRoot->m_level = (*a_root)->m_level + 1;
    branch.m_rect = NodeCover(*a_root);
    branch.m_child = *a_root;
    AddBranch(&branch, newRoot, NULL);
    branch.m_rect = NodeCover(newNode);
    branch.m_child = newNode;
    AddBranch(&branch, newRoot, NULL);
    *a_root = newRoot;
    return true;
  }

  return false;
}


// Find the smallest rectangle that includes all rectangles in branches of a node.
RTREEBASE_TEMPLATE
typename RTREEBASE_QUAL::Rect RTREEBASE_QUAL::NodeCover(Node* a_node)
{
  ASSERT(a_node);
  
  int firstTime = true;
  Rect rect;
  InitRect(&rect);
  
  for(int index = 0; index < a_node->m_count; ++index)
  {
    if(firstTime)
    {
      rect = a_node->m_branch[index].m_rect;
      firstTime = false;
    }
    else
    {
      rect = CombineRect(&rect, &(a_node->m_branch[index].m_rect));
    }
  }
  
  return rect;
}


// Add a branch to a node.  Split the node if necessary.
// Returns 0 if node not split.  Old node updated.
// Returns 1 if node split, sets *new_node to address of new node.
// Old node updated, becomes one of two.
RTREEBASE_TEMPLATE
bool RTREEBASE_QUAL::AddBranch(Branch* a_branch, Node* a_node, Node** a_newNode)
{
  ASSERT(a_branch);
  ASSERT(a_node);

  if(a_node->m_count < MAXNODES)  // Split won't be necessary
  {
    a_node->m_branch[a_node->m_count] = *a_branch;
    ++a_node->m_count;

    return false;
  }
  else
  {
    ASSERT(a_newNode);
    
    SplitNode(a_node, a_branch, a_newNode);
    return true;
  }
}


// Disconnect a dependent node.
// Caller must return (or stop using iteration index) after this as count has changed
RTREEBASE_TEMPLATE
void RTREEBASE_QUAL::DisconnectBranch(Node* a_node, int a_index)
{
  ASSERT(a_node && (a_index >= 0) && (a_index < MAXNODES));
  ASSERT(a_node->m_count > 0);

  // Remove element by swapping with the last element to prevent gaps in array
  a_node->m_branch[a_index] = a_node->m_branch[a_node->m_count - 1];
  
  --a_node->m_count;
}


// Pick a branch.  Pick the one that will need the smallest increase
// in area to accomodate the new rectangle.  This will result in the
// least total area for the covering rectangles in the current node.
// In case of a tie, pick the one which was smaller before, to get
// the best resolution when searching.
RTREEBASE_TEMPLATE
int RTREEBASE_QUAL::PickBranch(Rect* a_rect, Node* a_node)
{
  ASSERT(a_rect && a_node);
  
  bool firstTime = true;
  ELEMTYPEREAL increase;
  ELEMTYPEREAL bestIncr = (ELEMTYPEREAL)-1;
  ELEMTYPEREAL area;
  ELEMTYPEREAL bestArea;
  int best;
  Rect tempRect;

  for(int index=0; index < a_node->m_count; ++index)
  {
    Rect* curRect = &a_node->m_branch[index].m_rect;
    area = CalcRectVolume(curRect);
    tempRect = CombineRect(a_rect, curRect);
    increase = CalcRectVolume(&tempRect) - area;
    if((increase < bestIncr) || firstTime)
    {
      best = index;
      bestArea = area;
      bestIncr = increase;
      firstTime = false;
    }
    else if((increase == bestIncr) && (area < bestArea))
    {
      best = index;
      bestArea = area;
      bestIncr = increase;
    }
  }
  return best;
}


// Combine two rectangles into larger one containing both
RTREEBASE_TEMPLATE
typename RTREEBASE_QUAL::Rect RTREEBASE_QUAL::CombineRect(Rect* a_rectA, Rect* a_rectB)
{
  ASSERT(a_rectA && a_rectB);

  Rect newRect;

  for(int index = 0; index < NUMDIMS; ++index)
  {
    newRect.m_min[index] = Min(a_rectA->m_min[index], a_rectB->m_min[index]);
    newRect.m_max[index] = Max(a_rectA->m_max[index], a_rectB->m_max[index]);
  }

  return newRect;
}



// Split a node.
// Divides the nodes branches and the extra one between two nodes.
// Old node is one of the new ones, and one really new one is created.
// Tries more than one method for choosing a partition, uses best result.
RTREEBASE_TEMPLATE
void RTREEBASE_QUAL::SplitNode(Node* a_node, Branch* a_branch, Node** a_newNode)
{
  ASSERT(a_node);
  ASSERT(a_branch);
  
  splitNumber++;
  // Could just use local here, but member or external is faster since it is reused
  PartitionVars localVars;
  PartitionVars* parVars = &localVars;
  int level;

  // Load all the branches into a buffer, initialize old node
  level = a_node->m_level;
  GetBranches(a_node, a_branch, parVars);

  // Find partition
  ChoosePartition(parVars, MINNODES);

  // Put branches from buffer into 2 nodes according to chosen partition
  *a_newNode = AllocNode();
  (*a_newNode)->m_level = a_node->m_level = level;
  LoadNodes(a_node, *a_newNode, parVars);
  
  ASSERT((a_node->m_count + (*a_newNode)->m_count) == parVars->m_total);
}


// Calculate the n-dimensional volume of a rectangle
RTREEBASE_TEMPLATE
ELEMTYPEREAL RTREEBASE_QUAL::RectVolume(Rect* a_rect)
{
  ASSERT(a_rect);
  
  ELEMTYPEREAL volume = (ELEMTYPEREAL)1;

  for(int index=0; index<NUMDIMS; ++index)
  {
    volume *= a_rect->m_max[index] - a_rect->m_min[index];
  }
  
  ASSERT(volume >= (ELEMTYPEREAL)0);
  
  return volume;
}


// The exact volume of the bounding sphere for the given Rect
RTREEBASE_TEMPLATE
ELEMTYPEREAL RTREEBASE_QUAL::RectSphericalVolume(Rect* a_rect)
{
  ASSERT(a_rect);
   
  ELEMTYPEREAL sumOfSquares = (ELEMTYPEREAL)0;
  ELEMTYPEREAL radius;

  for(int index=0; index < NUMDIMS; ++index) 
  {
    ELEMTYPEREAL halfExtent = ((ELEMTYPEREAL)a_rect->m_max[index] - (ELEMTYPEREAL)a_rect->m_min[index]) * 0.5f;
    sumOfSquares += halfExtent * halfExtent;
  }

  radius = (ELEMTYPEREAL)sqrt(sumOfSquares);
  
  // Pow maybe slow, so test for common dims like 2,3 and just use x*x, x*x*x.
  if(NUMDIMS == 3)
  {
    return (radius * radius * radius * m_unitSphereVolume);
  }
  else if(NUMDIMS == 2)
  {
    return (radius * radius * m_unitSphereVolume);
  }
  else
  {
    return (ELEMTYPEREAL)(pow(radius, NUMDIMS) * m_unitSphereVolume);
  }
}


// Use one of the methods to calculate retangle volume
RTREEBASE_TEMPLATE
ELEMTYPEREAL RTREEBASE_QUAL::CalcRectVolume(Rect* a_rect)
{
#ifdef RTREE_USE_SPHERICAL_VOLUME
  return RectSphericalVolume(a_rect); // Slower but helps certain merge cases
#else // RTREE_USE_SPHERICAL_VOLUME
  return RectVolume(a_rect); // Faster but can cause poor merges
#endif // RTREE_USE_SPHERICAL_VOLUME  
}


// Load branch buffer with branches from full node plus the extra branch.
RTREEBASE_TEMPLATE
void RTREEBASE_QUAL::GetBranches(Node* a_node, Branch* a_branch, PartitionVars* a_parVars)
{
  ASSERT(a_node);
  ASSERT(a_branch);

  ASSERT(a_node->m_count == MAXNODES);
    
  // Load the branch buffer
  for(int index=0; index < MAXNODES; ++index)
  {
    a_parVars->m_branchBuf[index] = a_node->m_branch[index];
  }
  a_parVars->m_branchBuf[MAXNODES] = *a_branch;
  a_parVars->m_branchCount = MAXNODES + 1;

  // Calculate rect containing all in the set
  a_parVars->m_coverSplit = a_parVars->m_branchBuf[0].m_rect;
  for(int index=1; index < MAXNODES+1; ++index)
  {
    a_parVars->m_coverSplit = CombineRect(&a_parVars->m_coverSplit, &a_parVars->m_branchBuf[index].m_rect);
  }
  a_parVars->m_coverSplitArea = CalcRectVolume(&a_parVars->m_coverSplit);

  InitNode(a_node);
}


// Method #0 for choosing a partition:
// As the seeds for the two groups, pick the two rects that would waste the
// most area if covered by a single rectangle, i.e. evidently the worst pair
// to have in the same group.
// Of the remaining, one at a time is chosen to be put in one of the two groups.
// The one chosen is the one with the greatest difference in area expansion
// depending on which group - the rect most strongly attracted to one group
// and repelled from the other.
// If one group gets too full (more would force other group to violate min
// fill requirement) then other group gets the rest.
// These last are the ones that can go in either group most easily.
RTREEBASE_TEMPLATE
void RTREEBASE_QUAL::ChoosePartition(PartitionVars* a_parVars, int a_minFill)
{
  ASSERT(a_parVars);
  
  ELEMTYPEREAL biggestDiff;
  int group, chosen, betterGroup;
  
  InitParVars(a_parVars, a_parVars->m_branchCount, a_minFill);
  PickSeeds(a_parVars);

  while (((a_parVars->m_count[0] + a_parVars->m_count[1]) < a_parVars->m_total)
       && (a_parVars->m_count[0] < (a_parVars->m_total - a_parVars->m_minFill))
       && (a_parVars->m_count[1] < (a_parVars->m_total - a_parVars->m_minFill)))
  {
    biggestDiff = (ELEMTYPEREAL) -1;
    for(int index=0; index<a_parVars->m_total; ++index)
    {
      if(!a_parVars->m_taken[index])
      {
        Rect* curRect = &a_parVars->m_branchBuf[index].m_rect;
        Rect rect0 = CombineRect(curRect, &a_parVars->m_cover[0]);
        Rect rect1 = CombineRect(curRect, &a_parVars->m_cover[1]);
        ELEMTYPEREAL growth0 = CalcRectVolume(&rect0) - a_parVars->m_area[0];
        ELEMTYPEREAL growth1 = CalcRectVolume(&rect1) - a_parVars->m_area[1];
        ELEMTYPEREAL diff = growth1 - growth0;
        if(diff >= 0)
        {
          group = 0;
        }
        else
        {
          group = 1;
          diff = -diff;
        }

        if(diff > biggestDiff)
        {
          biggestDiff = diff;
          chosen = index;
          betterGroup = group;
        }
        else if((diff == biggestDiff) && (a_parVars->m_count[group] < a_parVars->m_count[betterGroup]))
        {
          chosen = index;
          betterGroup = group;
        }
      }
    }
    Classify(chosen, betterGroup, a_parVars);
  }

  // If one group too full, put remaining rects in the other
  if((a_parVars->m_count[0] + a_parVars->m_count[1]) < a_parVars->m_total)
  {
    if(a_parVars->m_count[0] >= a_parVars->m_total - a_parVars->m_minFill)
    {
      group = 1;
    }
    else
    {
      group = 0;
    }
    for(int index=0; index<a_parVars->m_total; ++index)
    {
      if(!a_parVars->m_taken[index])
      {
        Classify(index, group, a_parVars);
      }
    }
  }

  ASSERT((a_parVars->m_count[0] + a_parVars->m_count[1]) == a_parVars->m_total);
  ASSERT((a_parVars->m_count[0] >= a_parVars->m_minFill) && 
        (a_parVars->m_count[1] >= a_parVars->m_minFill));
}


// Copy branches from the buffer into two nodes according to the partition.
RTREEBASE_TEMPLATE
void RTREEBASE_QUAL::LoadNodes(Node* a_nodeA, Node* a_nodeB, PartitionVars* a_parVars)
{
  ASSERT(a_nodeA);
  ASSERT(a_nodeB);
  ASSERT(a_parVars);

  for(int index=0; index < a_parVars->m_total; ++index)
  {
    ASSERT(a_parVars->m_partition[index] == 0 || a_parVars->m_partition[index] == 1);
    
    if(a_parVars->m_partition[index] == 0)
    {
      AddBranch(&a_parVars->m_branchBuf[index], a_nodeA, NULL);
    }
    else if(a_parVars->m_partition[index] == 1)
    {
      AddBranch(&a_parVars->m_branchBuf[index], a_nodeB, NULL);
    }
  }
}


// Initialize a PartitionVars structure.
RTREEBASE_TEMPLATE
void RTREEBASE_QUAL::InitParVars(PartitionVars* a_parVars, int a_maxRects, int a_minFill)
{
  ASSERT(a_parVars);

  a_parVars->m_count[0] = a_parVars->m_count[1] = 0;
  a_parVars->m_area[0] = a_parVars->m_area[1] = (ELEMTYPEREAL)0;
  a_parVars->m_total = a_maxRects;
  a_parVars->m_minFill = a_minFill;
  for(int index=0; index < a_maxRects; ++index)
  {
    a_parVars->m_taken[index] = false;
    a_parVars->m_partition[index] = -1;
  }
}


RTREEBASE_TEMPLATE
void RTREEBASE_QUAL::PickSeeds(PartitionVars* a_parVars)
{
  int seed0, seed1;
  ELEMTYPEREAL worst, waste;
  ELEMTYPEREAL area[MAXNODES+1];

  for(int index=0; index<a_parVars->m_total; ++index)
  {
    area[index] = CalcRectVolume(&a_parVars->m_branchBuf[index].m_rect);
  }

  worst = -a_parVars->m_coverSplitArea - 1;
  for(int indexA=0; indexA < a_parVars->m_total-1; ++indexA)
  {
    for(int indexB = indexA+1; indexB < a_parVars->m_total; ++indexB)
    {
      Rect oneRect = CombineRect(&a_parVars->m_branchBuf[indexA].m_rect, &a_parVars->m_branchBuf[indexB].m_rect);
      waste = CalcRectVolume(&oneRect) - area[indexA] - area[indexB];
      if(waste > worst)
      {
        worst = waste;
        seed0 = indexA;
        seed1 = indexB;
      }
    }
  }
  Classify(seed0, 0, a_parVars);
  Classify(seed1, 1, a_parVars);
}


// Put a branch in one of the groups.
RTREEBASE_TEMPLATE
void RTREEBASE_QUAL::Classify(int a_index, int a_group, PartitionVars* a_parVars)
{
  ASSERT(a_parVars);
  ASSERT(!a_parVars->m_taken[a_index]);

  a_parVars->m_partition[a_index] = a_group;
  a_parVars->m_taken[a_index] = true;

  if (a_parVars->m_count[a_group] == 0)
  {
    a_parVars->m_cover[a_group] = a_parVars->m_branchBuf[a_index].m_rect;
  }
  else
  {
    a_parVars->m_cover[a_group] = CombineRect(&a_parVars->m_branchBuf[a_index].m_rect, &a_parVars->m_cover[a_group]);
  }
  a_parVars->m_area[a_group] = CalcRectVolume(&a_parVars->m_cover[a_group]);
  ++a_parVars->m_count[a_group];
}


// Delete a data rectangle from an index structure.
// Pass in a pointer to a Rect, the tid of the record, ptr to ptr to root node.
// Returns 1 if record not found, 0 if success.
// RemoveRect provides for eliminating the root.
RTREEBASE_TEMPLATE
bool RTREEBASE_QUAL::RemoveRect(Rect* a_rect, const DATATYPE& a_id, Node** a_root)
{
  ASSERT(a_rect && a_root);
  ASSERT(*a_root);

  Node* tempNode;
  ListNode* reInsertList = NULL;

  if(!RemoveRectRec(a_rect, a_id, *a_root, &reInsertList))
  {
    // Found and deleted a data item
    // Reinsert any branches from eliminated nodes
    while(reInsertList)
    {
      tempNode = reInsertList->m_node;
	  //遍历获取叶子节点【liuyuanyuan 】【9.11】
	  insertNode(tempNode,a_root);

      /*for(int index = 0; index < tempNode->m_count; ++index)
      {
        InsertRect(&(tempNode->m_branch[index].m_rect),
                   tempNode->m_branch[index].m_data,
                   a_root,
                   tempNode->m_level);
      }*/
      
      ListNode* remLNode = reInsertList;
      reInsertList = reInsertList->m_next;
      
      FreeNode(remLNode->m_node);
      FreeListNode(remLNode);
    }
    
    // Check for redundant root (not leaf, 1 child) and eliminate
    if((*a_root)->m_count == 1 && (*a_root)->IsInternalNode())
    {
      tempNode = (*a_root)->m_branch[0].m_child;
      
      ASSERT(tempNode);
      FreeNode(*a_root);
      *a_root = tempNode;
    }
    return false;
  }
  else
  {
    return true;
  }
}


// Delete a rectangle from non-root part of an index structure.
// Called by RemoveRect.  Descends tree recursively,
// merges branches on the way back up.
// Returns 1 if record not found, 0 if success.
RTREEBASE_TEMPLATE
bool RTREEBASE_QUAL::RemoveRectRec(Rect* a_rect, const DATATYPE& a_id, Node* a_node, ListNode** a_listNode)
{
  ASSERT(a_rect && a_node && a_listNode);
  ASSERT(a_node->m_level >= 0);

  if(a_node->IsInternalNode())  // not a leaf node
  {
    for(int index = 0; index < a_node->m_count; ++index)
    {
      if(Overlap(a_rect, &(a_node->m_branch[index].m_rect)))
      {
        if(!RemoveRectRec(a_rect, a_id, a_node->m_branch[index].m_child, a_listNode))
        {
          if(a_node->m_branch[index].m_child->m_count >= MINNODES)
          {
            // child removed, just resize parent rect
            a_node->m_branch[index].m_rect = NodeCover(a_node->m_branch[index].m_child);
          }
          else
          {
            // child removed, not enough entries in node, eliminate node
            ReInsert(a_node->m_branch[index].m_child, a_listNode);
            DisconnectBranch(a_node, index); // Must return after this call as count has changed
          }
          return false;
        }
      }
    }
    return true;
  }
  else // A leaf node
  {
    for(int index = 0; index < a_node->m_count; ++index)
    {
      if(a_node->m_branch[index].m_data == a_id)
      {
        DisconnectBranch(a_node, index); // Must return after this call as count has changed
        return false;
      }
    }
    return true;
  }
}


// Decide whether two rectangles overlap.
RTREEBASE_TEMPLATE
bool RTREEBASE_QUAL::Overlap(Rect* a_rectA, Rect* a_rectB)
{
  ASSERT(a_rectA && a_rectB);
  comparisonNumber++;
  for(int index=0; index < NUMDIMS; ++index)
  {
    if (a_rectA->m_min[index] > a_rectB->m_max[index] ||
        a_rectB->m_min[index] > a_rectA->m_max[index])
    {
      return false;
    }
  }
  return true;
}


// Add a node to the reinsertion list.  All its branches will later
// be reinserted into the index structure.
RTREEBASE_TEMPLATE
void RTREEBASE_QUAL::ReInsert(Node* a_node, ListNode** a_listNode)
{
  ListNode* newListNode;

  newListNode = AllocListNode();
  newListNode->m_node = a_node;
  newListNode->m_next = *a_listNode;
  *a_listNode = newListNode;
}


// Search in an index tree or subtree for all data retangles that overlap the argument rectangle.
RTREEBASE_TEMPLATE
bool RTREEBASE_QUAL::Search(Node* a_node, Rect* a_rect, int& a_foundCount, bool __cdecl a_resultCallback(DATATYPE a_data, void* a_context), void* a_context)
{
  ASSERT(a_node);
  ASSERT(a_node->m_level >= 0);
  ASSERT(a_rect);

  if(a_node->IsInternalNode()) // This is an internal node in the tree
  {
	  this->accessedInnerNodeNumber++;
    for(int index=0; index < a_node->m_count; ++index)
    {
      if(Overlap(a_rect, &a_node->m_branch[index].m_rect))
      {
        if(!Search(a_node->m_branch[index].m_child, a_rect, a_foundCount, a_resultCallback, a_context))
        {
          return false; // Don't continue searching
        }
      }
    }
  }
  else // This is a leaf node
  {
	  this->accessedLeafNodeNumber++;
    for(int index=0; index < a_node->m_count; ++index)
    {
      if(Overlap(a_rect, &a_node->m_branch[index].m_rect))
      {
        DATATYPE& id = a_node->m_branch[index].m_data;
        
        // NOTE: There are different ways to return results.  Here's where to modify
        if(&a_resultCallback)
        {
          ++a_foundCount;
          if(!a_resultCallback(id, a_context))
          {
            return false; // Don't continue searching
          }
        }
      }
    }
  }

  return true; // Continue searching
}
RTREEBASE_TEMPLATE
bool RTREEBASE_QUAL::Search(Node* a_node, Rect* a_rect, int& a_foundCount, std::vector<DATATYPE>&a_data)
{
	ASSERT(a_node);
	ASSERT(a_node->m_level >= 0);
	ASSERT(a_rect);

	if(a_node->IsInternalNode()) // This is an internal node in the tree
	{
		for(int index=0; index < a_node->m_count; ++index)
		{
			if(Overlap(a_rect, &a_node->m_branch[index].m_rect))
			{
				if(!Search(a_node->m_branch[index].m_child, a_rect, a_foundCount, a_data))
				{
					return false; // Don't continue searching
				}
			}
		}
	}
	else // This is a leaf node
	{
		for(int index=0; index < a_node->m_count; ++index)
		{
			if(Overlap(a_rect, &a_node->m_branch[index].m_rect))
			{
				DATATYPE& id = a_node->m_branch[index].m_data;

				// NOTE: There are different ways to return results.  Here's where to modify
				
				++a_foundCount;
				a_data.push_back(id);
					
			}
		}
	}

	return true; // Continue searching
}
RTREEBASE_TEMPLATE
bool RTREEBASE_QUAL::Search(Node* a_node, Rect* a_rect, int& a_foundCount, std::set<DATATYPE>& a_arrid)
{
	ASSERT(a_node);
	ASSERT(a_node->m_level >= 0);
	ASSERT(a_rect);

	if(a_node->IsInternalNode()) // This is an internal node in the tree
	{
		accessedInnerNodeNumber++;
		
		for(int index=0; index < a_node->m_count; ++index)
		{
			if(Overlap(a_rect, &a_node->m_branch[index].m_rect))
			{
				if(!Search(a_node->m_branch[index].m_child, a_rect, a_foundCount, a_arrid))
				{
					return false; // Don't continue searching
				}
			}
		}
	}
	else // This is a leaf node
	{
		accessedLeafNodeNumber++;
		for(int index=0; index < a_node->m_count; ++index)
		{
			if(Overlap(a_rect, &a_node->m_branch[index].m_rect))
			{
				// NOTE: There are different ways to return results.  Here's where to modify
				DATATYPE m_data= a_node->m_branch[index].m_data;
				++a_foundCount;
				a_arrid.insert(m_data);		
			}
		}
	}
	return true; // Continue searching
}
RTREEBASE_TEMPLATE
int RTREEBASE_QUAL::Search(std::vector<Point> a_line,std::set<DATATYPE>&a_arrid){
	int f=0;
	if (a_line.size()<2){
		return 0;
	}
	for (std::vector<Point>::size_type i= 0;i!=a_line.size()-1;i++){
		Point point1 = a_line[i];Point point2 = a_line[i+1];
		Rect rectTemp;
		for (int j=0;j<NUMDIMS;j++){
			rectTemp.m_min[j]=Min(point1.m_pos[j],point2.m_pos[j]);
			rectTemp.m_max[j]=Max(point1.m_pos[j],point2.m_pos[j]);
		}
		int f1=0;
		Search(m_root,&rectTemp,f1,a_arrid);
		f+=f1;
	}
	return f;
}
RTREEBASE_TEMPLATE
unsigned int RTREEBASE_QUAL::findPos(DATATYPE a_data,std::vector<DATATYPE> &a_vecdata){
	unsigned int i=0;
	std::vector<DATATYPE>::iterator itr = a_vecdata.begin();
	while(itr!=a_vecdata.end())
	{
		if ((*itr)==a_data)
		{
			return i;
		}
		itr++;
		i++;
	}
	return i;
}
RTREEBASE_TEMPLATE
void RTREEBASE_QUAL::write(std::ostream & f,std::vector<DATATYPE> &a_vecdata){
	//写入基本信息
	int dataFileId = ('R'<<0)|('T'<<8)|('R'<<16)|('E'<<24);
	int dataSize = sizeof(DATATYPE);
	int dataNumDims = NUMDIMS;
	int dataElemSize = sizeof(ELEMTYPE);
	int dataElemRealSize = sizeof(ELEMTYPEREAL);
	int dataMaxNodes = TMAXNODES;
	int dataMinNodes = TMINNODES;
	f.write((char*)(&dataFileId),sizeof(int));
	f.write((char*)(&dataSize),sizeof(int));
	f.write((char*)(&dataNumDims),sizeof(int));
	f.write((char*)(&dataElemSize),sizeof(int));
	f.write((char*)(&dataElemRealSize),sizeof(int));
	f.write((char*)(&dataMaxNodes),sizeof(int));
	f.write((char*)(&dataMinNodes),sizeof(int));
	//写入根结点信息
	writeNode(m_root,f,a_vecdata);
}
RTREEBASE_TEMPLATE
void RTREEBASE_QUAL::writeNode(Node * pnode,std::ostream &f,std::vector<DATATYPE> &a_vecdata){
	f.write((char*)(&(pnode->m_level)),sizeof(int));
	f.write((char*)(&(pnode->m_count)),sizeof(int));
	if (pnode->IsInternalNode()){
		for(int index = 0; index < pnode->m_count; ++index){
			Branch* curBranch = &pnode->m_branch[index];
			for (int i=0;i<NUMDIMS;i++){
				f.write((char*)(&(curBranch->m_rect.m_min[i])),sizeof(ELEMTYPE));
				f.write((char*)(&(curBranch->m_rect.m_max[i])),sizeof(ELEMTYPE));
			}
			writeNode(curBranch->m_child,f,a_vecdata);
		}
	}
	else{
		for(int index = 0; index < pnode->m_count; ++index){
			Branch* curBranch = &pnode->m_branch[index];
			for (int i=0;i<NUMDIMS;i++){
				f.write((char*)(&(curBranch->m_rect.m_min[i])),sizeof(ELEMTYPE));
				f.write((char*)(&(curBranch->m_rect.m_max[i])),sizeof(ELEMTYPE));
			}
			unsigned int pos = findPos(curBranch->m_data,a_vecdata);//查找行号
			f.write((char*)(&pos),sizeof(unsigned int));
		}
	}
}
RTREEBASE_TEMPLATE
void RTREEBASE_QUAL::read(std::istream & f,std::vector<DATATYPE> &a_vecdata){
	//读出基本信息
	int _dataFileId = ('R'<<0)|('T'<<8)|('R'<<16)|('E'<<24);
	int _dataSize = sizeof(DATATYPE);
	int _dataNumDims = NUMDIMS;
	int _dataElemSize = sizeof(ELEMTYPE);
	int _dataElemRealSize = sizeof(ELEMTYPEREAL);
	int _dataMaxNodes = TMAXNODES;
	int _dataMinNodes = TMINNODES;

	int dataFileId = 0;
	int dataSize = 0;
	int dataNumDims = 0;
	int dataElemSize = 0;
	int dataElemRealSize = 0;
	int dataMaxNodes = 0;
	int dataMinNodes = 0;

	f.read((char*)(&dataFileId),sizeof(int));
	f.read((char*)(&dataSize),sizeof(int));
	f.read((char*)(&dataNumDims),sizeof(int));
	f.read((char*)(&dataElemSize),sizeof(int));
	f.read((char*)(&dataElemRealSize),sizeof(int));
	f.read((char*)(&dataMaxNodes),sizeof(int));
	f.read((char*)(&dataMinNodes),sizeof(int));
	//进行判断
	if((dataFileId == _dataFileId) 
		&& (dataSize == _dataSize) 
		&& (dataNumDims == _dataNumDims) 
		&& (dataElemSize == _dataElemSize) 
		&& (dataElemRealSize == _dataElemRealSize) 
		&& (dataMaxNodes == _dataMaxNodes) 
		&& (dataMinNodes == _dataMinNodes) )
	{
		//读取节点信息
		readNode(m_root, f,a_vecdata);
	}
}

RTREEBASE_TEMPLATE
void RTREEBASE_QUAL::readNode(Node * pnode,std::istream & f,std::vector<DATATYPE> &a_vecdata){
	f.read((char*)(&(pnode->m_level)),sizeof(int));
	f.read((char*)(&(pnode->m_count)),sizeof(int));
	if(pnode->IsInternalNode()){
		for(int index = 0; index < pnode->m_count; ++index){
			Branch* curBranch = &pnode->m_branch[index];
			for (int i=0;i<NUMDIMS;i++){
				f.read((char*)(&(curBranch->m_rect.m_min[i])),sizeof(ELEMTYPE));
				f.read((char*)(&(curBranch->m_rect.m_max[i])),sizeof(ELEMTYPE));
			}
			curBranch->m_child = AllocNode();
			readNode(curBranch->m_child, f,a_vecdata);
		}
	}
	else {
		for(int index = 0; index < pnode->m_count; ++index){
			Branch* curBranch = &pnode->m_branch[index];
			for (int i=0;i<NUMDIMS;i++){
				f.read((char*)(&(curBranch->m_rect.m_min[i])),sizeof(ELEMTYPE));
				f.read((char*)(&(curBranch->m_rect.m_max[i])),sizeof(ELEMTYPE));
			}
			unsigned int pos;
			f.read((char*)(&pos),sizeof(unsigned int));
			curBranch->m_data = a_vecdata[pos];
		}
	}
}



RTREEBASE_TEMPLATE
size_t RTREEBASE_QUAL::sizeBuffer(){
	return sizeNodeBuffer(m_root);
}

RTREEBASE_TEMPLATE
size_t RTREEBASE_QUAL::sizeNodeBuffer(Node* a_node){
	size_t s=0;
	s =s + sizeof(int)+sizeof(int);
	if(a_node->IsInternalNode()){
		for (int i=0;i<a_node->m_count;i++){
			s=s+sizeof(ELEMTYPE)*NUMDIMS*2
				+sizeof(int);
			Branch* curBranch = &a_node->m_branch[i];
			s+=sizeNodeBuffer(curBranch->m_child);
		}
	}
	if (a_node->IsLeaf()){
		for (int i=0;i<a_node->m_count;i++){
			s=s+sizeof(ELEMTYPE)*NUMDIMS*2;
			s=s+sizeof(DATATYPE);
		}
	}
	return s;
}

RTREEBASE_TEMPLATE
bool RTREEBASE_QUAL::writeBuffer(Buffer<char>& buf){
	size_t s = sizeBuffer();
	return writeNodeBuffer(m_root,buf,s);
}
RTREEBASE_TEMPLATE
bool RTREEBASE_QUAL::writeNodeBuffer(Node * pnode,Buffer<char> &buf,size_t s){
	int * pi = (int *) new unsigned char[s];
	buf.attach(pi,s);
	*pi = pnode->m_level;
	pi++;
	*pi = pnode->m_count;
	pi++;
	if (pnode->IsInternalNode()){
		for (int i=0;i<pnode->m_count;i++){
			ELEMTYPE *Ele = (ELEMTYPE *)pi;
			Branch * curBranch = &pnode->m_branch[i];
			for (int j=0;j<NUMDIMS;j++){
				*Ele = curBranch->m_rect.m_min[j];
				Ele++;
				*Ele = curBranch->m_rect.m_max[j];
				Ele++;
			}

			pi = (int*)Ele;
			size_t s1=sizeNodeBuffer(curBranch->m_child);
			*pi = s1;
			pi++;
			unsigned char* pc = (unsigned char*)pi;
			Buffer<char> buf1;
			writeNodeBuffer(curBranch->m_child,buf1,(size_t)s1);
			memcpy(pc,buf1.data(),buf1.size());
			pc=pc+s1;
			pi=(int*)pc;
		}
	}
	if (pnode->IsLeaf()){
		for (int i =0;i<pnode->m_count;i++){
			ELEMTYPE *Ele = (ELEMTYPE *)pi;
			Branch * curBranch = &pnode->m_branch[i];
			for (int j=0;j<NUMDIMS;j++){
				*Ele = curBranch->m_rect.m_min[j];
				Ele++;
				*Ele = curBranch->m_rect.m_max[j];
				Ele++;
			}
			pi = (int*)Ele;
			DATATYPE *dat = (DATATYPE *)pi;
			*dat = curBranch->m_data;
			dat++;
			pi =(int *)dat;
		}
	}
	return true;
}

RTREEBASE_TEMPLATE
bool RTREEBASE_QUAL::readBuffer(const Buffer<char> & buf){
	return readNodeBuffer(m_root,buf);
}

RTREEBASE_TEMPLATE
bool RTREEBASE_QUAL::readNodeBuffer(Node * pnode,const Buffer<char> & buf){
	int * pi = (int *)(buf.data());
	pnode->m_level = *pi ;
	pi++;
	pnode->m_count = *pi;
	pi++;
	if (pnode->IsInternalNode()){
		for (int i=0;i<pnode->m_count;i++){
			ELEMTYPE *Ele = (ELEMTYPE *)pi;
			Branch * curBranch = &pnode->m_branch[i];
			for (int j=0;j<NUMDIMS;j++){
				curBranch->m_rect.m_min[j] = *Ele;
				Ele++;
				curBranch->m_rect.m_max[j] = *Ele;
				Ele++;
			}
			curBranch->m_child =AllocNode();
			pi =(int*)Ele;
			int nLen = *pi;
			pi++;
			unsigned char * pc = (unsigned char*)pi;
			Buffer<char> buf1;
			buf1.set(pc,(size_t)nLen,true);
			readNodeBuffer(curBranch->m_child,buf1);
			pc=pc+nLen;
			pi=(int *)pc;
		}
	}
	if (pnode->IsLeaf()){
		for (int i =0;i<pnode->m_count;i++){
			ELEMTYPE *Ele = (ELEMTYPE *)pi;
			Branch * curBranch = &pnode->m_branch[i];
			for (int j=0;j<NUMDIMS;j++){
				curBranch->m_rect.m_min[j] = *Ele;
				Ele++;
				curBranch->m_rect.m_max[j] = *Ele;
				Ele++;	
			}
			pi = (int*)Ele;
			DATATYPE *dat = (DATATYPE *)pi;
			curBranch->m_data = *dat;
			dat++;
			pi = (int *)dat;
		}
	}
	return true;
}
//插入结点列表是调用
RTREEBASE_TEMPLATE
bool RTREEBASE_QUAL::insertNode(Node * a_node,Node** a_root)
{
	ASSERT(a_node);
	if (a_node->IsInternalNode())
	{
		for (int index = 0; index < a_node->m_count; ++index)
		{
			Node* tempNode= a_node->m_branch[index].m_child;
			insertNode(tempNode,a_root);
		}
	}
	else 
	{
		for(int index = 0; index < a_node->m_count; ++index)
		{
			InsertRect(&(a_node->m_branch[index].m_rect),
				a_node->m_branch[index].m_data,
				a_root,
				a_node->m_level);
		}  
	}
	return true;
}


#undef RTREE_TEMPLATE
#undef RTREE_QUAL
end_gtl_namespace 
