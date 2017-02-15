package gtl.stil.rtree.impl;

import gtl.stil.Entry;
import gtl.stil.Identifier;
import gtl.stil.Node;
import gtl.stil.shape.Region;

import java.util.ArrayList;
import java.util.Stack;

/**
 * Created by ZhenwenHe on 2017/2/13.
 */
public class RTreeExternalNodeImpl extends RTreeNodeImpl{
    public RTreeExternalNodeImpl(RTreeImpl tree, Identifier identifier, int level, int capacity) {
        super(tree, identifier, level, capacity);
    }

    public RTreeExternalNodeImpl(RTreeImpl tree) {
        super(tree);
    }

    @Override
    public Object clone() {
        return null;
    }

    @Override
    protected Node chooseSubtree(Region mbr, int level, Stack<Identifier> pathBuffer) {

        return null;
    }

    @Override
    protected Node findLeaf(Region mbr, Identifier id, Stack<Identifier> pathBuffer) {
        int childCount=getChildrenCount();
        for (int cChild = 0; cChild < childCount; ++cChild){
            // should make sure to relinquish other PoolPointer lists that might be pointing to the same leaf.
            if (getChildIdentifier(cChild).equals(id) && getChildShape(cChild).equals(mbr))
                return this;
        }
        return null;
    }

    @Override
    protected Node[] split(Entry e) {
        tree.m_stats.setSplitTimes(tree.m_stats.getSplitTimes()+1);
        ArrayList<Integer> g1=new ArrayList<Integer>();
        ArrayList<Integer> g2=new ArrayList<Integer>();
        switch (tree.m_treeVariant) {
            case RV_LINEAR:
            case RV_QUADRATIC:
                rtreeSplit( e, g1, g2);
                break;
            case RV_RSTAR:
                rstarSplit(e, g1, g2);
                break;
            default:
                return null;
        }

        Node[] nodes=new RTreeExternalNodeImpl[2];
        RTreeExternalNodeImpl pLeft = (RTreeExternalNodeImpl)(nodes[0]);
        RTreeExternalNodeImpl pRight = (RTreeExternalNodeImpl)(nodes[1]);

        pLeft.getShape().copyFrom(tree.m_infiniteRegion);
        pRight.getShape().copyFrom(tree.m_infiniteRegion);

        int cIndex;
        int tIndex;
        for (cIndex = 0; cIndex < g1.size(); ++cIndex){
            tIndex=g1.get(cIndex);
            pLeft.insertEntry(getChildEntry(tIndex));
            // we don't want to delete the data array from this node's destructor!
            this.setChildData(tIndex,null);
        }

        for (cIndex = 0; cIndex < g2.size(); ++cIndex){
            tIndex=g2.get(cIndex);
            pRight.insertEntry(getChildEntry(tIndex));
            // we don't want to delete the data array from this node's destructor!
            this.setChildData(tIndex,null);
        }

        return nodes;
    }
}
