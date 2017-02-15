package gtl.stil.rtree.impl;

import com.sun.org.apache.regexp.internal.RE;
import gtl.stil.*;
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

import static gtl.stil.rtree.RTreeVariant.RV_LINEAR;
import static gtl.stil.rtree.RTreeVariant.RV_QUADRATIC;
import static gtl.stil.rtree.RTreeVariant.RV_RSTAR;

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

    @Override
    public void  insertEntry(Entry e) {
        super.insertEntry(e);
        //计算节点的包围矩形
        Region r = (Region) getShape();
        Region er = (Region) e.getShape();
        r.combineRegion(er);
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


    protected boolean insertData(Entry e, Stack<Identifier> pathBuffer, byte[] overflowTable) {
        assert (e instanceof EntryImpl);
        int cIndex=0;
        int children=getChildrenCount();
        int capacity=getCapacity();
        Region nodeMBR = (Region) getShape();
        Region mbr = (Region) e.getShape();
        int level = getLevel();
        // 如果子节点个数小于容量
        if (children < capacity){
            boolean adjusted = false;

            // this has to happen before insertEntry modifies m_nodeMBR.
            boolean b = nodeMBR.containsRegion(mbr);
            this.insertEntry(e);
            this.tree.writeNode(this);

            if ((! b) && (! pathBuffer.empty())){
                Identifier cParent = pathBuffer.pop();
                Node ptrN = this.tree.readNode(cParent);
                RTreeInternalNodeImpl p = (RTreeInternalNodeImpl)(ptrN);
                p.adjustTree(this, pathBuffer);
                adjusted = true;
            }
            return adjusted;
        }
        else if (this.tree.m_treeVariant == RV_RSTAR && (! pathBuffer.empty()) && overflowTable[level] == 0){
            //如果是RStarTree则需要重新插入
            overflowTable[level] = 1;
            //记录需要重插的孩子节点下标
            ArrayList<Integer> vReinsert=new ArrayList<Integer>();
            //记录需要重插的孩子节点下标
            ArrayList<Integer>  vKeep=new ArrayList<Integer>();
            //执行RStarTree的重插数据，得到哪些子节点是需要保留的，哪些是需要重插的
            this.reinsertData(e, vReinsert, vKeep);
            //需要重插的项数
            int lReinsert = vReinsert.size();
            //需要保留的项数
            int lKeep = vKeep.size();

            Entry [] reinsertEntries=new Entry[lReinsert];
            Entry [] keepEntries=new Entry[capacity + 1];
            //需要重新插入的数据项
            for(cIndex=0;cIndex<lReinsert;++cIndex){
                reinsertEntries[cIndex] = getChildEntry(vReinsert.get(cIndex));
            }
            //需要保留的数据项
            for(cIndex=0;cIndex<lKeep;++cIndex){
                keepEntries[cIndex] = getChildEntry(vKeep.get(cIndex));
            }
            //将本节点设置为保留数据项
            setChildEntries(keepEntries);
            //重新计算节点的边界矩形
            recalculateShape();
            //重新计算节点的数据长度
            this.tree.writeNode(this);

            // Divertion from R*-Tree algorithm here. First adjust
            // the path to the root, then start reinserts, to avoid complicated handling
            // of changes to the same node from multiple insertions.
            Identifier cParent = pathBuffer.pop();
            Node ptrN = this.tree.readNode(cParent);
            RTreeInternalNodeImpl p = (RTreeInternalNodeImpl)ptrN;
            p.adjustTree(this, pathBuffer);
            //将需要重新插入的数据进行重插

            for ( cIndex=0; cIndex < lReinsert; ++cIndex){
                this.tree.insertData_impl(
                        reinsertEntries[cIndex],
                        level, overflowTable);
            }
            return true;
        }
        else  {
            Node [] nodes = split(e);
            RTreeNodeImpl  n=(RTreeNodeImpl )nodes[0];
            RTreeNodeImpl  nn=(RTreeNodeImpl )nodes[1];

            if (pathBuffer.empty()){
                n.setLevel(this.getLevel());
                nn.setLevel(this.getLevel());
                n.setIdentifier(-1L);
                nn.setIdentifier(-1L);
                this.tree.writeNode(n);
                this.tree.writeNode(nn);

                RTreeNodeImpl ptrR = new RTreeInternalNodeImpl(this.tree,this.tree.m_rootID,this.getLevel()+1);

                ptrR.insertEntry((Entry)n);
                ptrR.insertEntry((Entry)nn);

                this.tree.writeNode(ptrR);

                StatisticsImpl s =(StatisticsImpl) (this.tree.m_stats);
                s.setNodeNumberInLevel(level,2L);
                this.tree.m_stats.getNodeNumberInLevelArray().add(1L);
                this.tree.m_stats.setTreeHeight(level + 2);
            }
            else {
                n.setLevel(this.getLevel());
                nn.setLevel(this.getLevel());
                nn.setIdentifier(-1L);
                n.setIdentifier(this.getIdentifier().longValue());

                this.tree.writeNode(n);
                this.tree.writeNode(nn);

                Identifier cParent =  pathBuffer.pop();
                RTreeInternalNodeImpl ptrN =(RTreeInternalNodeImpl) this.tree.readNode(cParent);
                ptrN.adjustTree(n, nn, pathBuffer, overflowTable);
            }
            return true;
        }
    }
    protected void reinsertData(Entry e, ArrayList<Integer> reinsert, ArrayList<Integer> keep){
        int m_capacity=getCapacity();
        int m_children=getChildrenCount();
        ReinsertEntry[]v = new ReinsertEntry[m_capacity + 1];
        setChildEntry(m_children,e);
        Region m_nodeMBR = (Region)getShape();
        Vertex nc =m_nodeMBR.getCenter();
        Vertex c = null;

        for (int u32Child = 0; u32Child < m_capacity + 1; ++u32Child){
            v[u32Child] = new ReinsertEntry(u32Child, 0.0);
            c=getChildShape(u32Child).getCenter();
            // calculate relative distance of every entry from the node MBR (ignore square root.)
            for (int cDim = 0; cDim < m_nodeMBR.getDimension(); ++cDim){
                double d = nc.getCoordinate(cDim) - c.getCoordinate(cDim);
                v[u32Child].dist += d * d;
            }
        }

        // sort by increasing order of distances.
        java.util.Arrays.sort(v);

        int cReinsert = (int)(Math.floor((m_capacity + 1) * this.tree.m_reinsertFactor));

        int cCount;

        for (cCount = 0; cCount < cReinsert; ++cCount) {
            reinsert.add(v[cCount].index);
        }

        for (cCount = cReinsert; cCount < m_capacity + 1; ++cCount){
            keep.add(v[cCount].index);
        }
    }
    protected void rtreeSplit(Entry e, ArrayList<Integer> group1, ArrayList<Integer> group2){
        int u32Child;
        int m_capacity=getCapacity();
        int minimumLoad = (int)(Math.floor(m_capacity * this.tree.m_fillFactor));

        // use this mask array for marking visited entries.
        byte[] mask = new byte[m_capacity + 1];
        java.util.Arrays.fill(mask,(byte)0);
        // insert new data in the node for easier manipulation. Data arrays are always
        // by one larger than node capacity.
        setChildEntry(m_capacity,e);
        // m_totalDataLength does not need to be increased here.

        // initialize each group with the seed entries.
        int [] seeds=new int[2];
        pickSeeds(seeds);
        int seed1=seeds[0];
        int seed2=seeds[2];
        seeds=null;

        group1.add(seed1);
        group2.add(seed2);

        mask[seed1] = 1;
        mask[seed2] = 1;

        // find MBR of each group.
        Region  mbr1 =(Region) getChildShape(seed1).clone();
        Region  mbr2 =(Region) getChildShape(seed2).clone();

        // count how many entries are left unchecked (exclude the seeds here.)
        int cRemaining = m_capacity + 1 - 2;

        while (cRemaining > 0) {
            if (minimumLoad - group1.size() == cRemaining){
                // all remaining entries must be assigned to group1 to comply with minimun load requirement.
                for (u32Child = 0; u32Child < m_capacity + 1; ++u32Child) {
                    if (mask[u32Child] == 0){
                        group1.add(u32Child);
                        mask[u32Child] = 1;
                        --cRemaining;
                    }
                }
            }
            else if (minimumLoad - group2.size() == cRemaining){
                // all remaining entries must be assigned to group2 to comply with minimun load requirement.
                for (u32Child = 0; u32Child < m_capacity + 1; ++u32Child){
                    if (mask[u32Child] == 0){
                        group2.add(u32Child);
                        mask[u32Child] = 1;
                        --cRemaining;
                    }
                }
            }
            else{
                // For all remaining entries compute the difference of the cost of grouping an
                // entry in either group. When done, choose the entry that yielded the maximum
                // difference. In case of linear split, select any entry (e.g. the first one.)
                int sel=0;
                double md1 = 0.0, md2 = 0.0;
                double m = -Double.MAX_VALUE;
                double d1, d2, d;
                double a1 = mbr1.getArea();
                double a2 = mbr2.getArea();

                Region a = null;
                Region b = null;

                for (u32Child = 0; u32Child < m_capacity + 1; ++u32Child){
                    if (mask[u32Child] == 0){
                        a=mbr1.getCombinedRegion((Region) getChildShape(u32Child));
                        d1 = a.getArea() - a1;
                        b=mbr2.getCombinedRegion((Region) getChildShape(u32Child));
                        d2 = b.getArea() - a2;
                        d = Math.abs(d1 - d2);

                        if (d > m) {
                            m = d;
                            md1 = d1; md2 = d2;
                            sel = u32Child;
                            if (this.tree.m_treeVariant== RV_LINEAR || this.tree.m_treeVariant == RV_RSTAR)
                                break;
                        }
                    }
                }

                // determine the group where we should add the new entry.
                int group = -1;

                if (md1 < md2) {
                    group1.add(sel);
                    group = 1;
                }
                else if (md2 < md1){
                    group2.add(sel);
                    group = 2;
                }
                else if (a1 < a2){
                    group1.add(sel);
                    group = 1;
                }
                else if (a2 < a1){
                    group2.add(sel);
                    group = 2;
                }
                else if (group1.size() < group2.size()){
                    group1.add(sel);
                    group = 1;
                }
                else if (group2.size() < group1.size()){
                    group2.add(sel);
                    group = 2;
                }
                else{
                    group1.add(sel);
                    group = 1;
                }
                mask[sel] = 1;
                --cRemaining;
                if (group == 1){
                    mbr1.combineRegion((Region) getChildShape(sel));
                }
                else {
                    mbr2.combineRegion((Region) getChildShape(sel));
                }
            }
        }
    }
    protected void rstarSplit(Entry e, ArrayList<Integer> group1, ArrayList<Integer> group2){
        int m_capacity=getCapacity();
        RstarSplitEntry[] dataLow = new RstarSplitEntry[m_capacity + 1];
        RstarSplitEntry[] dataHigh = new RstarSplitEntry[m_capacity + 1];
        setChildEntry(m_capacity,e);
        // m_totalDataLength does not need to be increased here.

        int nodeSPF =(int)( Math.floor((m_capacity + 1) * this.tree.m_splitDistributionFactor));
        int splitDistribution = (m_capacity + 1) - (2 * nodeSPF) + 2;

        int u32Child = 0, cDim, cIndex;

        for (u32Child = 0; u32Child <= m_capacity; ++u32Child) {
            dataLow[u32Child] = new RstarSplitEntry((Region)getChildShape(u32Child), u32Child, 0);
            dataHigh[u32Child] = dataLow[u32Child];
        }

        double minimumMargin = Double.MAX_VALUE;
        int splitAxis = Integer.MAX_VALUE;
        int sortOrder =  Integer.MAX_VALUE;

        Region bbl1=IndexSuits.createRegion();
        Region bbl2=IndexSuits.createRegion();
        Region bbh1=IndexSuits.createRegion();
        Region bbh2=IndexSuits.createRegion();

        // chooseSplitAxis.
        for (cDim = 0; cDim < this.tree.m_dimension; ++cDim){
            java.util.Arrays.sort(dataLow,new RstarSplitEntryLowComparator());
            java.util.Arrays.sort(dataHigh,new RstarSplitEntryHighComparator());
            // calculate sum of margins and overlap for all distributions.
            double marginl = 0.0;
            double marginh = 0.0;



            for (u32Child = 1; u32Child <= splitDistribution; ++u32Child){
                int ls = nodeSPF - 1 + u32Child;

                bbl1.copyFrom(dataLow[0].region);
                bbh1.copyFrom(dataHigh[0].region);

                for (cIndex = 1; cIndex < ls; ++cIndex){
                    bbl1.combineRegion(dataLow[cIndex].region);
                    bbh1.combineRegion(dataHigh[cIndex].region);
                }

                bbl2.copyFrom(dataLow[ls].region);
                bbh2.copyFrom(dataHigh[ls].region);

                for (cIndex = ls + 1; cIndex <= m_capacity; ++cIndex){
                    bbl2.combineRegion(dataLow[cIndex].region);
                    bbh2.combineRegion(dataHigh[cIndex].region);
                }

                marginl += bbl1.getMargin() + bbl2.getMargin();
                marginh += bbh1.getMargin() + bbh2.getMargin();
            } // for (u32Child)

            double margin = Math.min(marginl, marginh);

            // keep minimum margin as split axis.
            if (margin < minimumMargin){
                minimumMargin = margin;
                splitAxis = cDim;
                sortOrder = (marginl < marginh) ? 0 : 1;
            }

            // increase the dimension according to which the data entries should be sorted.
            for (u32Child = 0; u32Child <= m_capacity; ++u32Child){
                dataLow[u32Child].sortDim = cDim + 1;
            }
        } // for (cDim)

        for (u32Child = 0; u32Child <= m_capacity; ++u32Child){
            dataLow[u32Child].sortDim= splitAxis;
        }

        if(sortOrder == 0)
            java.util.Arrays.sort(dataLow,new RstarSplitEntryLowComparator());
        else
            java.util.Arrays.sort(dataLow,new RstarSplitEntryHighComparator());

        double ma = Double.MAX_VALUE;
        double mo = Double.MAX_VALUE;
        int splitPoint = Integer.MAX_VALUE;
        Region bb1=IndexSuits.createRegion();
        Region bb2=IndexSuits.createRegion();
        for (u32Child = 1; u32Child <= splitDistribution; ++u32Child){
            int ls = nodeSPF - 1 + u32Child;
            bb1.copyFrom(dataLow[0].region);
            for (cIndex = 1; cIndex < ls; ++cIndex){
                bb1.combineRegion(dataLow[cIndex].region);
            }
            bb2.copyFrom(dataLow[ls].region);
            for (cIndex = ls + 1; cIndex <= m_capacity; ++cIndex){
                bb2.combineRegion(dataLow[cIndex].region);
            }
            double o = bb1.getIntersectingArea(bb2);

            if (o < mo){
                splitPoint = u32Child;
                mo = o;
                ma = bb1.getArea() + bb2.getArea();
            }
            else if (o == mo){
                double a = bb1.getArea() + bb2.getArea();

                if (a < ma)
                {
                    splitPoint = u32Child;
                    ma = a;
                }
            }
        } // for (u32Child)

        int l1s = nodeSPF - 1 + splitPoint;

        for (cIndex = 0; cIndex < l1s; ++cIndex){
            group1.add(dataLow[cIndex].index);
        }

        for (cIndex = l1s; cIndex <= m_capacity; ++cIndex){
            group2.add(dataLow[cIndex].index);
        }
    }
    protected void pickSeeds(int []indexes){
        int m_capacity=getCapacity();
        int index1=indexes[0];
        int index2=indexes[1];

        double separation = -Double.MAX_VALUE;
        double inefficiency = -Double.MAX_VALUE;
        int cDim, u32Child, cIndex;

        switch (this.tree.m_treeVariant) {
            case RV_LINEAR:
            case RV_RSTAR: {
                for (cDim = 0; cDim < this.tree.m_dimension; ++cDim) {
                    double leastLower = ((Region)getChildShape(0)).getLowCoordinate(cDim);
                    double greatestUpper =((Region)getChildShape(0)).getHighCoordinate(cDim);
                    int greatestLower = 0;
                    int leastUpper = 0;
                    double width;

                    for (u32Child = 1; u32Child <= m_capacity; ++u32Child) {
                        if (((Region)getChildShape(u32Child)).getLowCoordinate(cDim) > ((Region)getChildShape(greatestLower)).getLowCoordinate(cDim))
                            greatestLower = u32Child;
                        if (((Region)getChildShape(u32Child)).getHighCoordinate(cDim) <  ((Region)getChildShape(leastUpper)).getHighCoordinate(cDim))
                            leastUpper = u32Child;

                        leastLower = Math.min(((Region)getChildShape(u32Child)).getLowCoordinate(cDim), leastLower);
                        greatestUpper = Math.max(((Region)getChildShape(u32Child)).getHighCoordinate(cDim), greatestUpper);
                    }

                    width = greatestUpper - leastLower;
                    if (width <= 0) width = 1;

                    double f = (((Region)getChildShape(greatestLower)).getLowCoordinate(cDim)
                            - ((Region)getChildShape(leastUpper)).getHighCoordinate(cDim)) / width;

                    if (f > separation) {
                        index1 = leastUpper;
                        index2 = greatestLower;
                        separation = f;
                    }
                }  // for (cDim)

                if (index1 == index2) {
                    if (index2 == 0) ++index2;
                    else --index2;
                }

                break;
            }

            case RV_QUADRATIC: {
                // for each pair of Regions (account for overflow Region too!)
                for (u32Child = 0; u32Child < m_capacity; ++u32Child) {
                    double a = ((Region)getChildShape(u32Child)).getArea();

                    for (cIndex = u32Child + 1; cIndex <= m_capacity; ++cIndex) {
                        // get the combined MBR of those two entries.
                        Region r= ((Region)getChildShape(u32Child)).getCombinedRegion( ((Region)getChildShape(cIndex)));

                        // find the inefficiency of grouping these entries together.
                        double d = r.getArea() - a - ((Region)getChildShape(u32Child)).getArea();

                        if (d > inefficiency) {
                            inefficiency = d;
                            index1 = u32Child;
                            index2 = cIndex;
                        }
                    }  // for (cIndex)
                } // for (u32Child)

                break;
            }
        }

        //计算完后将得到的值传入数组带回
        indexes[0]=index1;
        indexes[1]=index2;
    }
    protected void condenseTree(Stack<Node> toReinsert, Stack<Identifier> pathBuffer, Node  ptrThis){
        int m_capacity=getCapacity();
        int m_children=getChildrenCount();
        int m_level= getLevel();
        RTreeImpl m_pTree=this.tree;
        Region m_nodeMBR=(Region)getShape();
        int minimumLoad = (int)(Math.floor(m_capacity * m_pTree.m_fillFactor));
        double d1,d2;//临时变量
        int dims=0;
        if (pathBuffer.empty()){
            // eliminate root if it has only one child.
            if (m_level != 0 && m_children == 1){
                RTreeNodeImpl ptrN = (RTreeNodeImpl)m_pTree.readNode(getChildIdentifier(0));
                m_pTree.deleteNode(ptrN);
                ptrN.setIdentifier(m_pTree.m_rootID.longValue());
                m_pTree.writeNode(ptrN);

                ArrayList<Long> alNodesInLevel = m_pTree.m_stats.getNodeNumberInLevelArray();
                //删除最后一个元素
                alNodesInLevel.remove(alNodesInLevel.size()-1);
                m_pTree.m_stats.setTreeHeight(m_pTree.m_stats.getTreeHeight()-1);
                // HACK: pending deleteNode for deleted child will decrease nodesInLevel, later on.
                alNodesInLevel.set((int)(m_pTree.m_stats.getTreeHeight()-1),2L);
            }
        }
        else{
            Identifier cParent = pathBuffer.pop();
            RTreeNodeImplBackup ptrParent = (RTreeNodeImplBackup)m_pTree.readNode(cParent);
            InternalRTreeNodeImplBack p = (InternalRTreeNodeImplBack)(ptrParent);

            // find the entry in the parent, that points to this node.
            int child;
            for (child = 0; child != p.getChildrenCount(); ++child){
                if (p.getChildIdentifier(child).equals(this.getIdentifier()))
                    break;
            }

            if (m_children < minimumLoad){
                // used space less than the minimum
                // 1. eliminate node entry from the parent. deleteEntry will fix the parent's MBR.
                p.deleteEntry(child);
                // 2. add this node to the stack in order to reinsert its entries.
                toReinsert.push(ptrThis);
            }
            else{
                // adjust the entry in 'p' to contain the new bounding region of this node.
                //*(p->m_ptrMBR[child]) = m_nodeMBR;
                assert m_nodeMBR.copyTo(p.getChildShape(child))!=null;

                // global recalculation necessary since the MBR can only shrink in size,
                // due to data removal.
                if (m_pTree.m_bTightMBRs){
                    dims=p.getShape().getDimension();
                    Region r =(Region) p.getShape();
                    Region r2=null;
                    int childC=p.getChildrenCount();
                    for (int cDim = 0; cDim < dims; ++cDim){
                        r.setLowCoordinate(cDim,Double.MAX_VALUE);
                        r.setHighCoordinate(cDim,-Double.MAX_VALUE);

                        for (int u32Child = 0; u32Child < childC; ++u32Child){
                            r2=(Region) p.getChildShape(u32Child);
                            d1=Math.min(r.getLowCoordinate(cDim),r2.getLowCoordinate(cDim));
                            r.setLowCoordinate(cDim,d1);
                            d2=Math.max(r.getHighCoordinate(cDim),r2.getHighCoordinate(cDim));
                            r.setHighCoordinate(cDim,d2);
                        }
                    }
                }
            }

            // write parent node back to storage.
            m_pTree.writeNode(p);

            p.condenseTree(toReinsert, pathBuffer, ptrParent);
        }
    }

    protected abstract Node chooseSubtree( Region mbr, int level, Stack<Identifier> pathBuffer);

    protected abstract Node findLeaf(Region mbr, Identifier id, Stack<Identifier> pathBuffer);

    protected abstract Node[] split(Entry e );

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
