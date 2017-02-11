package gtl.stil.rtree.impl;

import gtl.stil.*;
import gtl.stil.shape.Region;
import gtl.stil.shape.Shape;

import java.util.ArrayList;
import java.util.Stack;

/**
 * Created by ZhenwenHe on 2016/12/22.
 */
public class ExternalRTreeNodeImplBackup extends RTreeNodeImplBackup implements ExternalNode{
    @Override
    public long getDataLength() {
        return 0;
    }

    @Override
    public byte[] getData() {
        return new byte[0];
    }

    @Override
    public void setData(byte[] data) {

    }

    ExternalRTreeNodeImplBackup(){
        super();
    }
    ExternalRTreeNodeImplBackup(RTreeImpl pTree, Identifier id){
        super(pTree,id,0,pTree.m_leafCapacity);
    }

    @Override
    Node chooseSubtree(Region mbr, int level, Stack<Identifier> pathBuffer) {
        return this;
    }

    @Override
    Node findLeaf(Region mbr, Identifier id, Stack<Identifier> pathBuffer) {
        int childCount=this.getChildrenCount();
        for (int cChild = 0; cChild < childCount; ++cChild){
            // should make sure to relinquish other PoolPointer lists that might be pointing to the
            // same leaf.
            if (this.getChildIdentifier(cChild).equals(id) && this.getChildShape(cChild).equals(mbr))
                return this;
        }
        return null;
    }

    @Override
    public Node[] split(byte[] pData, Region mbr, Identifier id ) {
        m_pTree.m_stats.setSplitTimes(m_pTree.m_stats.getSplitTimes()+1);
        ArrayList<Integer> g1=new ArrayList<Integer>();
        ArrayList<Integer> g2=new ArrayList<Integer>();
        switch (m_pTree.m_treeVariant) {
            case RV_LINEAR:
            case RV_QUADRATIC:
                rtreeSplit( pData, mbr, id, g1, g2);
                break;
            case RV_RSTAR:
                rstarSplit(pData, mbr, id, g1, g2);
                break;
            default:
               return null;
        }

        Node[] nodes=new ExternalRTreeNodeImplBackup[2];
        ExternalRTreeNodeImplBackup pLeft = (ExternalRTreeNodeImplBackup)(nodes[0]);
        ExternalRTreeNodeImplBackup pRight = (ExternalRTreeNodeImplBackup)(nodes[1]);

        pLeft.getShape().copyFrom(m_pTree.m_infiniteRegion);
        pRight.getShape().copyFrom(m_pTree.m_infiniteRegion);

        int cIndex;
        int tIndex;
        for (cIndex = 0; cIndex < g1.size(); ++cIndex){
            tIndex=g1.get(cIndex);
            pLeft.insertEntry(getChildData(tIndex),(Region) getChildShape(tIndex), getChildIdentifier(tIndex));
            // we don't want to delete the data array from this node's destructor!
            this.setChildData(tIndex,null);
        }

        for (cIndex = 0; cIndex < g2.size(); ++cIndex){
            tIndex=g2.get(cIndex);
            pRight.insertEntry(getChildData(tIndex),(Region) getChildShape(tIndex), getChildIdentifier(tIndex));
            // we don't want to delete the data array from this node's destructor!
            this.setChildData(tIndex,null);
        }

        return nodes;
    }

    void deleteData(Identifier id, Stack<Identifier> pathBuffer){
        int child;
        int childCount = this.getChildrenCount();
        for (child = 0; child < childCount; ++child) {
            if (this.getChildIdentifier(child).equals(id)) break;
        }

        deleteEntry(child);
        m_pTree.writeNode(this);

        Stack<Node> toReinsert=new Stack<Node>();
        condenseTree(toReinsert, pathBuffer, this);
        // re-insert eliminated nodes.
        while (! toReinsert.empty()){
            Node n = toReinsert.pop();
            m_pTree.deleteNode(n);

            for (int cChild = 0; cChild < n.getChildrenCount(); ++cChild){
                // keep this in the for loop. The tree height might change after insertions.
			    byte[] overflowTable = IndexSuits.createByteArray((int)m_pTree.m_stats.getTreeHeight(),(byte)0);
                m_pTree.insertData_impl(n.getChildData(cChild),(Region) n.getChildShape(cChild) ,n.getChildIdentifier(cChild), n.getLevel(), overflowTable);
                n.setChildData(cChild,null);
            }
        }
    }
    @Override
    public Object clone() {
        ExternalRTreeNodeImplBackup e=new ExternalRTreeNodeImplBackup();
        e.copyFrom(this);
        return e;
    }

    @Override
    public Shape recalculateShape() {
        return null;
    }

    @Override
    public Entry getChildEntry(int index) {
        return null;
    }

    @Override
    public void insertEntry(Entry e) {

    }

    @Override
    public Entry removeEntry(int index) {
        return null;
    }
}
