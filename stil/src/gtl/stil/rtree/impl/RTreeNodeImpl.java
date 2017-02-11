package gtl.stil.rtree.impl;

import gtl.stil.Entry;
import gtl.stil.Identifier;
import gtl.stil.IndexSuits;
import gtl.stil.Node;
import gtl.stil.impl.EntryImpl;
import gtl.stil.impl.NodeImpl;
import gtl.stil.rtree.RTreeVariant;
import gtl.stil.shape.Region;
import gtl.stil.shape.Shape;
import gtl.stil.shape.impl.RegionImpl;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.Stack;
import java.util.function.Supplier;

/**
 * Created by ZhenwenHe on 2017/2/12.
 */
public abstract  class RTreeNodeImpl extends NodeImpl {
    // Parent of all nodes.
    RTreeImpl tree;

    public RTreeNodeImpl(RTreeImpl tree,Identifier identifier, int level, int capacity) {
        super(identifier, level, capacity);
        this.tree = tree;
        Shape s= newShape();
        setShape(s);
    }

    public RTreeNodeImpl(RTreeImpl tree) {
        super();
        this.tree = tree;
        Shape s= newShape();
        setShape(s);
    }

    protected void  insertEntry(byte[] pData, Region mbr, Identifier id) {
        EntryImpl e = new EntryImpl(id,mbr,pData);
        insertEntry(e);
        //计算节点的包围矩形
        Region r = (Region) getShape();
        r.combineRegion(mbr);
    }

    /**
     *
     * @param index index >= 0 && index < m_children
     */
    protected void deleteEntry(int index) {
        Entry e = removeEntry(index);
        if(e==null) return;

        Region er =(Region) e.getShape();
        Region r=(Region) getShape();
        if (getChildrenCount() == 0){
            r.makeInfinite();
        }
        else if (this.tree.m_bTightMBRs && r.touchesRegion(er)){
            recalculateShape();
        }
    }

    @Override
    public abstract Object clone();

    @Override
    public Shape recalculateShape() {
        Region er =null;
        Region r=(Region) getShape();
        int children =getChildrenCount();
        //重新计算节点的矩形区域
        for (int cDim = 0; cDim < r.getDimension(); ++cDim){
            r.setLowCoordinate(cDim,Double.MAX_VALUE);
            r.setHighCoordinate(cDim,-Double.MAX_VALUE);
            for (int u32Child = 0; u32Child < children; ++u32Child){
                er = (Region) getChildShape(u32Child);
                r.setLowCoordinate(cDim,Math.min(r.getLowCoordinate(cDim),er.getLowCoordinate(cDim)));
                r.setHighCoordinate(cDim,Math.max(r.getHighCoordinate(cDim),er.getHighCoordinate(cDim)));
            }
        }
        return r;
    }

    @Override
    public Shape newShape() {
        return IndexSuits.createRegion();
    }


    protected boolean insertData(byte[] pData, Region mbr, Identifier id, Stack<Identifier> pathBuffer, byte[] overflowTable) {
        return true;
    }
    protected void reinsertData(byte[] pData, Region mbr, Identifier id, ArrayList<Integer> reinsert, ArrayList<Integer> keep){

    }
    protected void rtreeSplit(byte[] pData, Region mbr, Identifier id, ArrayList<Integer> group1, ArrayList<Integer> group2){
    }
    protected void rstarSplit(byte[] pData, Region mbr, Identifier id, ArrayList<Integer> group1, ArrayList<Integer> group2){
    }
    protected void pickSeeds(int []indexes){

    }
    protected void condenseTree(Stack<Node> toReinsert, Stack<Identifier> pathBuffer, Node  ptrThis){

    }

    protected abstract Node chooseSubtree( Region mbr, int level, Stack<Identifier> pathBuffer);

    protected abstract Node findLeaf(Region mbr, Identifier id, Stack<Identifier> pathBuffer);

    protected abstract Node[] split(byte[] pData, Region mbr, Identifier id ) ;

    class RstarSplitEntry  {
        Region region;
        int index;
        int sortDim;


        public RstarSplitEntry(Region  v_region, int v_index, int v_dimension){
            region =v_region;
            index =v_index;
            sortDim=v_dimension;
        }
    }; // RstarSplitEntry
    class RstarSplitEntryLowComparator implements Comparator<RstarSplitEntry> {
        @Override
        public int compare(RstarSplitEntry pe1, RstarSplitEntry pe2) {
            assert(pe1.sortDim == pe2.sortDim);

            if (pe1.region.getLowCoordinate(pe1.sortDim) < pe2.region.getLowCoordinate(pe2.sortDim)) return -1;
            if (pe1.region.getLowCoordinate(pe1.sortDim) > pe2.region.getLowCoordinate(pe2.sortDim)) return 1;
            return 0;
        }
    }
    class RstarSplitEntryHighComparator implements Comparator<RstarSplitEntry>{
        @Override
        public int compare(RstarSplitEntry pe1, RstarSplitEntry pe2){

            assert(pe1.sortDim == pe2.sortDim);

            if (pe1.region.getHighCoordinate(pe1.sortDim) < pe2.region.getHighCoordinate(pe2.sortDim)) return -1;
            if (pe1.region.getHighCoordinate(pe1.sortDim) > pe2.region.getHighCoordinate(pe2.sortDim)) return 1;
            return 0;
        }
    }
    class ReinsertEntry implements Comparable{
        int index;
        double dist;

        ReinsertEntry(int v_index, double v_dist) {
            index =v_index;
            dist =v_dist;
        }

        @Override
        public int compareTo(Object o) {
            ReinsertEntry pe2=( ReinsertEntry)o;
            if (this.dist < pe2.dist) return -1;
            if (this.dist > pe2.dist) return 1;
            return 0;
        }
    };
}
