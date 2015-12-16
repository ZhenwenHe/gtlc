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
#include "envelope.h"
#include "interval.h"
#include "intervals.h"
#include "IsoscelesRightTriangle.h"
begin_gtl_namespace 

#ifndef MAX_INTERVAL_NUMBER_PRENODE
#define	MAX_INTERVAL_NUMBER_PRENODE 32
#endif

class TriTree
{
	typedef IsoscelesRightTriangle<char> triangle_type;
public:
	struct TriTreeNode
	{
		triangle_type *  triangle;
		TriTreeNode * parent;
		TriTreeNode * left;
		TriTreeNode * right;
		Interval1dPtr                 intervals[MAX_INTERVAL_NUMBER_PRENODE];
		unsigned char                 intervalsNumber;

		TriTreeNode()
		{
			parent=0;
			left=0;
			right=0;
			triangle=0;  
			intervalsNumber=0;
		}

		~TriTreeNode()
		{

			if(triangle!=0)
				delete triangle;

			triangle=0;


			parent=0;
			left=0;
			right=0;
			intervalsNumber=0;
		}
	};
protected:
	//表示树的叶子节点包含Interval的最大个数为_blocking_factor，大于这个数则该节点会分裂
	unsigned int  _blockingFactor;
	/*
		all the intervals should be in the triangle of [_minmumValue,_minmumValue],
		[_minmumValue,_maximumValue],[_maximumValue,_maximumValue];
	*/
	double   _minmumValue;//最小值
	double   _maximumValue;//最大值
	TriTreeNode _root;// root of the tree

	short    _dimensionalOrder;//指向的维度序号；
public:
	inline short getDimensionalOrder() const {return _dimensionalOrder;}
	inline void setDimensionalOrder(short d)  { _dimensionalOrder=d;}
public:
	unsigned long long accessedLeafNodeNumber;
	unsigned long long accessedInnerNodeNumber;
	unsigned long long splitNumber;
public:

	TriTree(double minValue, double maxValue, unsigned int maxNumberInBlock)
	{
		_maximumValue=maxValue;
		_minmumValue=minValue;
		_blockingFactor=MAX_INTERVAL_NUMBER_PRENODE;

		_root.parent=0;
		_root.left=0;
		_root.right=0;
		_root.triangle = new triangle_type(Vertex3d(minValue,maxValue,0),Vertex3d(maxValue,maxValue,0),Vertex3d(minValue,minValue,0));

		accessedLeafNodeNumber=0;
	    accessedInnerNodeNumber=0;
	    splitNumber=0;

	}
	TriTreeNode * findParentNode(Interval1dPtr pi)
	{
		Vertex3d v(pi->start(),pi->end(),0);

		if(_root.triangle->test(v)==0)//在三角形外面
		{
				return (TriTreeNode *)0;
		}
		else{
			TriTreeNode * left = _root.left;
			TriTreeNode * right = _root.right;
			TriTreeNode * parent = & _root;

			if(left==0 && right ==0)
				return parent;

			while(left && right)
			{
				if(left->triangle->test(v)!=0){
					parent = left;
					left = parent->left;
					right = parent->right;
				}
				else{
					parent = right;
					left = parent->left;
					right = parent->right;
				}
			}

			return parent;
		}
	}
	bool insert(Interval1dPtr pi)
	{
		if(pi==0)
			return false;

		TriTreeNode  * parent = findParentNode(pi);

		if(parent==0) return false;

		if(parent->intervalsNumber==0)
		{
			parent->intervals[parent->intervalsNumber]=pi;
			parent->intervalsNumber++;
			return true;
		}
		else
		{
			if(parent->intervalsNumber>=_blockingFactor)
			{
				split(parent,pi);
			}
			else
			{
				parent->intervals[parent->intervalsNumber]=pi;
				parent->intervalsNumber++;
			}
		
			return true;
		}
	}

	void split(TriTreeNode * parent,Interval1dPtr pi)
	{
	
		Interval1dPtr * intervals = parent->intervals;

		//generate left subtree
		TriTreeNode* left = new TriTreeNode;
		parent->left = left;
		left->parent=parent;
		left->triangle = new triangle_type;
		left->left=0;
		left->right=0;

		//generate right subtree
		TriTreeNode* right = new TriTreeNode;	
		parent->right=right;
		right->parent=parent;	
		right->triangle= new triangle_type;
		right->left=0;
		right->right=0;

		//calculate the triangles of left node and right node;
		parent->triangle->decompose(*(left->triangle),*(right->triangle));
		splitNumber++;

		//alloc the intervals for left node and right node 
		/*left->intervals= new Interval1dPtr [_blockingFactor];
		right->intervals= new Interval1dPtr [_blockingFactor];*/

		//insert the intervals into left node or right node
		for(int i=0;i<_blockingFactor;i++)
		{
			if(left->triangle->test(Vertex3d(intervals[i]->start(),intervals[i]->end(),0))!=0){		
				left->intervals[left->intervalsNumber]=intervals[i];
				left->intervalsNumber++;
			}
			else{
				right->intervals[right->intervalsNumber]=intervals[i];
				right->intervalsNumber++;
			}
			intervals[i]=0;
		}	
		parent->intervalsNumber=0;
		intervals=0;

		while(left->intervalsNumber==_blockingFactor || right->intervalsNumber==_blockingFactor)
		{
			if(left->intervalsNumber==_blockingFactor)
				parent = left;
			if(right->intervalsNumber==_blockingFactor)
				parent = right;

			intervals = parent->intervals;

			left = new TriTreeNode;
			parent->left = left;
			left->parent=parent;
			left->triangle = new triangle_type;
			left->left=0;
			left->right=0;

			//generate right subtree
			right = new TriTreeNode;	
			parent->right=right;
			right->parent=parent;	
			right->triangle= new triangle_type;
			right->left=0;
			right->right=0;

			//calculate the triangles of left node and right node;
			parent->triangle->decompose(*(left->triangle),*(right->triangle));
			splitNumber++;
			//alloc the intervals for left node and right node 
			/*left->intervals= new Interval1dPtr [_blockingFactor];
			right->intervals= new Interval1dPtr [_blockingFactor];*/

			for(int i=0;i<_blockingFactor;i++)
			{
				if(left->triangle->test(Vertex3d(intervals[i]->start(),intervals[i]->end(),0))!=0){		
					left->intervals[left->intervalsNumber]=intervals[i];
					left->intervalsNumber++;
				}
				else{
					right->intervals[right->intervalsNumber]=intervals[i];
					right->intervalsNumber++;
				}
				intervals[i]=0;
			}
			intervals=0;
			parent->intervalsNumber=0;
		}
	
		//insert pi
		if(left->triangle->test(Vertex3d(pi->start(),pi->end(),0))!=0){		
			left->intervals[left->intervalsNumber]=pi;
			left->intervalsNumber++;
		}
		else{
			right->intervals[right->intervalsNumber]=pi;
			right->intervalsNumber++;
		}

	}
	void deleteTree(TriTreeNode * parent)
	{
		if(parent==0) return;

		if(parent->left==0 && parent->right==0)
		{
			delete parent;
			parent=0;
		}
		else{

			if(parent->left!=0)
				deleteTree(parent->left);

			if(parent->right!=0)
				deleteTree(parent->right);
		}
	}

	~TriTree()
	{
		deleteTree(_root.left);
		deleteTree(_root.right);
	}
	bool intersect(const Envelope2d & e2d, const triangle_type & irt)
	{
		if(e2d.test(irt.get(0))!=0)
			return true;
		if(e2d.test(irt.get(1))!=0)
			return true;
		if(e2d.test(irt.get(2))!=0)
			return true;

		Vertex3d v(e2d.minData[0],e2d.minData[1],0);
		if(irt.test(v)!=0)
			return true;

 
		v.setX(e2d.minData[0]);
		v.setY(e2d.maxData[1]);
		if(irt.test(v)!=0)
			return true;
 

		v.setX(e2d.maxData[0]);
		v.setY(e2d.maxData[1]);

		if(irt.test(v)!=0)
			return true;

 
		v.setX(e2d.maxData[0]);
		v.setY(e2d.minData[1]);
		if(irt.test(v)!=0)
			return true;

		return false;
	}
	void search(TriTreeNode* root, const Envelope2d & e2d)
	{
		if(root==0) return ;

		if(!intersect(e2d,*root->triangle))
			return ;

		TriTreeNode*left = root->left;
		TriTreeNode*right = root->right;
		Interval1dPtr * intervals = root->intervals;

		

		if(root->intervalsNumber!=0)
		{
			for(int i=0;i<root->intervalsNumber;i++)
				if(e2d.test(intervals[i]->start(),intervals[i]->end())!=0) 
					if(intervals[i]->parent())
						((Interval4d*)(intervals[i]->parent()))->flags(_dimensionalOrder,1); 
			accessedLeafNodeNumber++;
		}
		else
		{
			accessedInnerNodeNumber ++;

			if(left){
				if(intersect(e2d,*left->triangle))
				{
					search(left,e2d);
				}
			}
			if(right){
				if(intersect(e2d,*right->triangle))
				{
					search(right,e2d);
				}
			}
		}
	}


	void search(TriTreeNode* proot, const Envelope2d & e2d,std::vector<Interval1dPtr> & results)
	{
		TriTreeNode* root =proot;

		if(root==0) return ;

		if(!intersect(e2d,*root->triangle))
			return ;

		TriTreeNode*left = root->left;
		TriTreeNode*right = root->right;
		Interval1dPtr * intervals = root->intervals;
		
		if(root->intervalsNumber!=0)
		{
			accessedLeafNodeNumber++;

			for(int i=0;i<root->intervalsNumber;i++)
				if(e2d.test(intervals[i]->start(),intervals[i]->end())!=0) {
					if(intervals[i]->parent())
						((Interval4d*)(intervals[i]->parent()))->flags(_dimensionalOrder,1); 
					results.push_back(intervals[i]);
				}
				
		}
		else
		{
			accessedInnerNodeNumber++;
			if(intersect(e2d,*left->triangle))
			{
				search(left,e2d,results);
			}
			if(intersect(e2d,*right->triangle))
			{
				search(right,e2d,results);
			}
		}
	}

	inline std::pair<unsigned long long , unsigned long long> search(const Envelope2d & e2d,std::vector<Interval1dPtr> & results)
	{
		accessedLeafNodeNumber=0;
		accessedInnerNodeNumber=0;

		search(&_root,e2d,results);

		std::pair<unsigned long long , unsigned long long> accessedInfo;
		accessedInfo.first=accessedInnerNodeNumber;
		accessedInfo.second=accessedLeafNodeNumber;
		return accessedInfo;
	}
	inline std::pair<unsigned long long , unsigned long long> search(const Envelope2d & e2d )
	{
		accessedLeafNodeNumber=0;
		accessedInnerNodeNumber=0;

		search(&_root,e2d); 

		std::pair<unsigned long long , unsigned long long> accessedInfo;
		accessedInfo.first=accessedInnerNodeNumber;
		accessedInfo.second=accessedLeafNodeNumber;
		return accessedInfo;
	}
};
end_gtl_namespace