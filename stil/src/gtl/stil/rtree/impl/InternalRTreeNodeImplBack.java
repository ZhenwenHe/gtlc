package gtl.stil.rtree.impl;

import gtl.stil.*;
import gtl.stil.shape.Region;
import gtl.stil.shape.Shape;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Stack;

/**
 * internal node class
 * Created by ZhenwenHe on 2016/12/19.
 */
public class InternalRTreeNodeImplBack extends RTreeNodeImplBackup implements InternalNode{
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

    InternalRTreeNodeImplBack(){
    }
    InternalRTreeNodeImplBack(RTreeImpl pTree, Identifier id, int level){
        super(pTree, id, level, pTree.m_indexCapacity);
    }
    @Override
    public Object clone() {
        return null;
    }

    int findLeastEnlargement(Region r) {

        double area = Double.MAX_VALUE;
        int best = Integer.MAX_VALUE;

        Region t = null;

        for (int cChild = 0; cChild < m_children; ++cChild){
            t=m_ptrMBR[cChild].getCombinedRegion(r);

            double a = m_ptrMBR[cChild].getArea();
            double enl = t.getArea() - a;

            if (enl < area){
                area = enl;
                best = cChild;
            }
            else if (enl == area){
                // this will rarely happen, so compute best area on the fly only
                // when necessary.
                if (a < m_ptrMBR[best].getArea()) best = cChild;
            }
        }

        return best;
    }

    int findLeastOverlap( Region r) {
        OverlapEntry[] entries = new OverlapEntry[m_children];

        double leastOverlap = Double.MAX_VALUE;
        double me = Double.MAX_VALUE;
        OverlapEntry best = null;

        // find combined region and enlargement of every entry and store it.
        for (int cChild = 0; cChild < m_children; ++cChild){
            entries[cChild] = new OverlapEntry();
            entries[cChild].m_index = cChild;
            entries[cChild].m_original = m_ptrMBR[cChild];
            entries[cChild].m_combined = m_ptrMBR[cChild].getCombinedRegion( r);
            entries[cChild].m_oa = entries[cChild].m_original.getArea();
            entries[cChild].m_ca = entries[cChild].m_combined.getArea();
            entries[cChild].m_enlargement = entries[cChild].m_ca - entries[cChild].m_oa;

            if (entries[cChild].m_enlargement < me){
                me = entries[cChild].m_enlargement;
                best = entries[cChild];
            }
		    else if (entries[cChild].m_enlargement == me && entries[cChild].m_oa < best.m_oa){
                best = entries[cChild];
            }
        }

        if (me < - IndexSuits.EPSILON || me > IndexSuits.EPSILON){
            int cIterations;

            if (m_children > m_pTree.m_nearMinimumOverlapFactor){
                // sort entries in increasing order of enlargement.
                Arrays.sort(entries); 
                assert(entries[0].m_enlargement <= entries[m_children - 1].m_enlargement);

                cIterations = m_pTree.m_nearMinimumOverlapFactor;
            }
            else {
                cIterations = m_children;
            }

            // calculate overlap of most important original entries (near minimum overlap cost).
            for (int cIndex = 0; cIndex < cIterations; ++cIndex){
                double dif = 0.0;
                OverlapEntry e = entries[cIndex];

                for (int cChild = 0; cChild < m_children; ++cChild){
                    if (e.m_index != cChild)
                    {
                        double f = e.m_combined.getIntersectingArea(m_ptrMBR[cChild]);
                        if (f != 0.0) 
                            dif += f - e.m_original.getIntersectingArea(m_ptrMBR[cChild]);
                    }
                } // for (cChild)

                if (dif < leastOverlap){
                    leastOverlap = dif;
                    best = entries[cIndex];
                }
                else if (dif == leastOverlap){
                    if (e.m_enlargement == best.m_enlargement){
                        // keep the one with least area.
                        if (e.m_original.getArea() < best.m_original.getArea())
                            best = entries[cIndex];
                    }
                    else{
                        // keep the one with least enlargement.
                        if (e.m_enlargement < best.m_enlargement) best = entries[cIndex];
                    }
                }
            } // for (cIndex)
        }

        int ret = best.m_index;

        return ret;
    }

    void adjustTree(RTreeNodeImplBackup n, Stack<Identifier> pathBuffer){
        ++(m_pTree.m_stats.adjustments);

        // find entry pointing to old node;
        int child;
        for (child = 0; child < m_children; ++child){
            if (m_pIdentifier[child] == n.m_identifier) break;
        }

        // MBR needs recalculation if either:
        //   1. the NEW child MBR is not contained.
        //   2. the OLD child MBR is touching.
        boolean bContained = m_nodeMBR.containsRegion(n.m_nodeMBR);
        boolean bTouches = m_nodeMBR.touchesRegion(m_ptrMBR[child]);
        boolean bRecompute = (! bContained || (bTouches && m_pTree.m_bTightMBRs));

        assert n.m_nodeMBR.copyTo(m_ptrMBR[child])!=null;

        if (bRecompute) {
            m_nodeMBR.makeInfinite();
            for (int cDim = 0; cDim < m_nodeMBR.getDimension(); ++cDim){
                for (int cChild = 0; cChild < m_children; ++cChild){
                    m_nodeMBR.setLowCoordinate(cDim, Math.min(m_nodeMBR.getLowCoordinate(cDim), m_ptrMBR[cChild].getLowCoordinate(cDim)));
                    m_nodeMBR.setHighCoordinate(cDim, Math.max(m_nodeMBR.getHighCoordinate(cDim), m_ptrMBR[cChild].getHighCoordinate(cDim)));
                }
            }
        }

        m_pTree.writeNode(this);

        if (bRecompute && (! pathBuffer.empty())){
            Identifier cParent = pathBuffer.pop();
            InternalRTreeNodeImplBack p = (InternalRTreeNodeImplBack)m_pTree.readNode(cParent);
            p.adjustTree(this, pathBuffer);
        }
    }
    void adjustTree(RTreeNodeImpl n, Stack<Identifier> pathBuffer){

    }

    @Override
    Node chooseSubtree(Region mbr, int insertionLevel, Stack<Identifier> pathBuffer) {
        if (m_level == insertionLevel) return this;
        pathBuffer.push((Identifier) getIdentifier().clone());
        int child = 0;
        switch (m_pTree.m_treeVariant){
            case RV_LINEAR:
            case RV_QUADRATIC:
                child = findLeastEnlargement(mbr);
                break;
            case RV_RSTAR:
                if (m_level == 1){
                    // if this node points to leaves...
                    child = findLeastOverlap(mbr);
                }
                else{
                    child = findLeastEnlargement(mbr);
                }
                break;
            default:
                return null;
        }
        assert(child != Integer.MAX_VALUE);

        RTreeNodeImplBackup n = (RTreeNodeImplBackup)m_pTree.readNode(m_pIdentifier[child]);
        return n.chooseSubtree(mbr, insertionLevel, pathBuffer);
    }

    @Override
    Node findLeaf(Region mbr, Identifier id, Stack<Identifier> pathBuffer) {
        pathBuffer.push(m_identifier);
        for (int cChild = 0; cChild < m_children; ++cChild){
            if (m_ptrMBR[cChild].containsRegion(mbr)){
                RTreeNodeImplBackup n = (RTreeNodeImplBackup)m_pTree.readNode(m_pIdentifier[cChild]);
                Node  ln = n.findLeaf(mbr, id, pathBuffer);
                if (ln!=null) return ln;
            }
        }
        pathBuffer.pop();
        return null;
    }

    @Override
    public Node[] split(byte[] pData, Region mbr, Identifier id ) {

        m_pTree.m_stats.setSplitTimes(m_pTree.m_stats.getSplitTimes()+1);

        ArrayList<Integer> g1=new ArrayList<>();
        ArrayList<Integer> g2=new ArrayList<>();

        switch (m_pTree.m_treeVariant){
            case RV_LINEAR:
            case RV_QUADRATIC:
                rtreeSplit(pData, mbr, id, g1, g2);
                break;
            case RV_RSTAR:
                rstarSplit(pData, mbr, id, g1, g2);
                break;
            default:
                return null;
        }

        InternalRTreeNodeImplBack ptrLeft = new InternalRTreeNodeImplBack(m_pTree, m_identifier, m_level);
        InternalRTreeNodeImplBack ptrRight =new InternalRTreeNodeImplBack(m_pTree, IndexSuits.createIdentifier(-1L), m_level);

        ptrLeft.m_nodeMBR.copyFrom(m_pTree.m_infiniteRegion);
        ptrRight.m_nodeMBR.copyFrom(m_pTree.m_infiniteRegion);

        int cIndex;

        for (cIndex = 0; cIndex < g1.size(); ++cIndex){
            ptrLeft.insertEntry(null,m_ptrMBR[g1.get(cIndex)], m_pIdentifier[g1.get(cIndex)]);
        }

        for (cIndex = 0; cIndex < g2.size(); ++cIndex){
            ptrRight.insertEntry(null, m_ptrMBR[g2.get(cIndex)], m_pIdentifier[g2.get(cIndex)]);
        }
        Node [] nodes = new Node[2];
        nodes[0]=ptrLeft;
        nodes[1]=ptrRight;
        return nodes;
    }

    class OverlapEntry implements Comparable{
        int m_index;
        double m_enlargement;
        Region m_original;
        Region m_combined;
        double m_oa;
        double m_ca;

        @Override
        public int compareTo(Object o) {
            OverlapEntry pe1 =  (OverlapEntry) this;
            OverlapEntry pe2 =  (OverlapEntry) o;

            if (pe1.m_enlargement < pe2.m_enlargement) return -1;
            if (pe1.m_enlargement > pe2.m_enlargement) return 1;
            return 0;
        }
    }; // OverlapEntry


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
