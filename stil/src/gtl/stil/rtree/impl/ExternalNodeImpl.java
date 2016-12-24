package gtl.stil.rtree.impl;

import gtl.stil.ExternalNode;
import gtl.stil.Identifier;
import gtl.stil.Node;
import gtl.stil.rtree.RTree;
import gtl.stil.shape.Region;

import java.util.Stack;

/**
 * Created by ZhenwenHe on 2016/12/22.
 */
public class ExternalNodeImpl extends NodeImpl implements ExternalNode{
    ExternalNodeImpl(){
        super();
    }
    ExternalNodeImpl(RTreeImpl pTree,Identifier id){
        super(pTree,id,0,pTree.m_leafCapacity);
    }

    @Override
    Node chooseSubtree(Region mbr, int level, Stack<Identifier> pathBuffer) {
        return this;
    }

    @Override
    Node findLeaf(Region mbr, Identifier id, Stack<Identifier> pathBuffer) {
        for (int cChild = 0; cChild < m_children; ++cChild){
            // should make sure to relinquish other PoolPointer lists that might be pointing to the
            // same leaf.
            if (m_pIdentifier[cChild].equals(id) && m_ptrMBR[cChild].equals(mbr))
                return this;
        }
        return null;
    }

    @Override
    public Node[] split(byte[] pData, Region mbr, Identifier id ) {
        return null;
    }

    void deleteData(Identifier id, Stack<Identifier> pathBuffer){

    }

    @Override
    public Object clone() {
        ExternalNodeImpl e=new ExternalNodeImpl();
        e.copyFrom(this);
        return e;
    }
}
