package gtl.stil.rtree.impl;

import gtl.stil.*;
import gtl.stil.rtree.RTreeVariant;
import gtl.stil.shape.Region;
import gtl.stil.shape.Shape;
import gtl.stil.shape.impl.RegionImpl;

import java.io.*;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.Stack;

/**
 * Created by ZhenwenHe on 2016/12/19.
 */
public abstract  class RTreeNodeImplBackup implements Node {


    RTreeImpl m_pTree;
    // Parent of all nodes.

    int m_level;
    // The level of the node in the tree.
    // Leaves are always at level 0.

    Identifier m_identifier;
    // The unique ID of this node.

    int m_children;
    // The number of children pointed by this node.

    int m_capacity;
    // Specifies the node capacity.

    Region m_nodeMBR;
    // The minimum bounding region enclosing all data contained in the node.

    byte[][] m_pData;
    // The data stored in the node.

    Region [] m_ptrMBR;
    // The corresponding data MBRs.

    Identifier [] m_pIdentifier;
    // The corresponding data identifiers.

    int m_totalDataLength;


    public RTreeNodeImplBackup(RTreeImpl pTree, Identifier id, int level, int capacity) {
        this.m_pTree = pTree;
        this.m_identifier= IndexSuits.createIdentifier(id.longValue());
        this.m_level=level;
        this.m_capacity=capacity;

        this.m_children=0;
        this.m_pData =null;
        this.m_ptrMBR=null;
        this.m_pIdentifier=null;
        this.m_totalDataLength=0;

        this.m_nodeMBR=new RegionImpl();
        this.m_nodeMBR.makeInfinite(this.m_pTree.getDimension());
        //数组中的每个元素依然是null
        this.m_pData = new byte[this.m_capacity + 1][];
        //数组中的每个元素依然是null
        this.m_ptrMBR = new Region[(this.m_capacity + 1)];
        //数组中的每个元素依然是null
        this.m_pIdentifier = new Identifier[(this.m_capacity + 1)];
    }

    public RTreeNodeImplBackup() {
        this.m_pTree = null;
        this.m_identifier= IndexSuits.createIdentifier(-1);
        this.m_level=0;
        this.m_capacity=32;
        this.m_totalDataLength=0;

        this.m_children=0;
        this.m_nodeMBR=new RegionImpl();
        this.m_nodeMBR.makeInfinite(this.m_pTree.getDimension());
        //数组中的每个元素依然是null
        this.m_pData = new byte[this.m_capacity + 1][];
        //数组中的每个元素依然是null
        this.m_ptrMBR = new Region[(this.m_capacity + 1)];
        //数组中的每个元素依然是null
        this.m_pIdentifier = new Identifier[(this.m_capacity + 1)];
    }
    @Override
    public int getChildrenCount() {
        return this.m_children;
    }

    @Override
    public Identifier getIdentifier() {
        return this.m_identifier;
    }

    @Override
    public void setIdentifier(long id) {
        this.m_identifier.reset(id);
    }

    @Override
    public void setIdentifier(Identifier id) {
        if(id==null) return ;
        this.m_identifier.reset(id.longValue());
    }

    @Override
    public Shape newShape(){
        return IndexSuits.createRegion();
    }
    @Override
    public void setShape(Shape s) {
        if(s==null) return ;
        if(s instanceof Region  || s instanceof Envelope){
            this.m_nodeMBR.copyFrom(s);
        }
    }

    @Override
    public abstract Object clone();

    @Override
    public Identifier getChildIdentifier(int index) {
        if (index <= this.m_children)
            return this.m_pIdentifier[index];
        else
            return null;
    }
    @Override
    public void setChildIdentifier(int index,Identifier id){
        if(id==null) return;
        if (index <= this.m_children) {
            if(this.m_pIdentifier[index]==null){
                this.m_pIdentifier[index]=IndexSuits.createIdentifier(id.longValue());
            }
            else {
                this.m_pIdentifier[index].reset(id.longValue());
            }
        }
    }
    @Override
    public Shape getShape() {
        return this.m_nodeMBR;
    }

    @Override
    public void copyFrom(Object i) {
        if(i==null) return ;
        if(i instanceof RTreeNodeImplBackup){
            RTreeNodeImplBackup ni = ((RTreeNodeImplBackup)i);
            this.m_pTree=ni.m_pTree;
            this.m_level=ni.m_level;
            this.m_identifier=(Identifier) ni.m_identifier.clone();
            this.m_children =ni.m_children;
            this.m_capacity=ni.m_capacity;
            this.m_nodeMBR.copyFrom(ni.m_nodeMBR);
            //copy byte [][] array
            this.m_pData=IndexSuits.createByteArray(ni.m_pData);
            //copy Region []
            this.m_ptrMBR=IndexSuits.createRegionArray(ni.m_ptrMBR);
            //copy Identifier []
            this.m_pIdentifier= IndexSuits.createIdentifierArray(ni.m_pIdentifier);
            this.m_totalDataLength=ni.m_totalDataLength;
        }
    }

    @Override
    public byte[] getChildData(int index) {
        if (index <= this.m_children) {
            return  this.m_pData[index];
        }
        else
            return null;
    }
    public void setChildData(int index,byte[] data){
        if(data==null) return;
        if (index <= this.m_children) {
            this.m_pData[index]=IndexSuits.createByteArray(data);
        }
    }
    @Override
    public Shape getChildShape(int index) {
        if (index <= this.m_children)
            return this.m_ptrMBR[index];
        else
            return null;
    }
    @Override
    public int getLevel() {
        return this.m_level;
    }

    @Override
    public boolean isIndex() {
        return isInternalNode();
    }

    @Override
    public boolean isLeaf() {
        return isExternalNode();
    }

    @Override
    public boolean isInternalNode() {
        return (this.m_level != 0);
    }

    @Override
    public boolean isExternalNode() {
        return (this.m_level == 0);
    }

    @Override
    public int getCapacity() {
        return this.m_capacity;
    }
    @Override
    public boolean read(InputStream in) throws IOException {
        this.m_nodeMBR.makeInfinite(this.m_pTree.getDimension());
        this.m_totalDataLength =0;
        DataInputStream dis=new DataInputStream(in);
        // skip the node type information, it is not needed.
        /*int nodeType = */ dis.readInt();
        this.m_level=dis.readInt();
        this.m_children=dis.readInt();
        for (int u32Child = 0; u32Child < this.m_children; ++u32Child) {
            if(this.m_ptrMBR[u32Child]==null){
                this.m_ptrMBR[u32Child]=IndexSuits.createRegion();
            }
            this.m_ptrMBR[u32Child].read(in);
            if(this.m_pIdentifier[u32Child]==null){
                this.m_pIdentifier[u32Child]=IndexSuits.createIdentifier(-1);
            }
            this.m_pIdentifier[u32Child].read(in);
            int len =dis.readInt();
            if(len>0) {
                this.m_pData[u32Child] = new byte[len];
                dis.read(this.m_pData[u32Child]);
                this.m_totalDataLength+=len;
            }
            else
                this.m_pData[u32Child]=null;
        }
        this.m_nodeMBR.read(in);
        dis.close();
        return true;
    }
    @Override
    public boolean write(OutputStream out) throws IOException {
        DataOutputStream dos =new DataOutputStream(out);
        int nodeType;
        if (m_level == 0)
            nodeType = 1;//leaf node
        else
            nodeType = 2;//index node
        dos.writeInt(nodeType);

        dos.writeInt(this.m_level);
        dos.writeInt(this.m_children);
        dos.flush();
        for (int u32Child = 0; u32Child < this.m_children; ++u32Child){
            this.m_ptrMBR[u32Child].write(out);
            this.m_pIdentifier[u32Child].write(out);
            if(this.m_pData[u32Child]==null)
                dos.writeInt(0);
            else
                dos.writeInt(this.m_pData[u32Child].length);
            dos.write(this.m_pData[u32Child]);
        }
        // store the node MBR for efficiency. This increases the node size a little bit.
        this.m_nodeMBR.write(out);
        dos.close();
        return true;
    }
    @Override
    public long getByteArraySize() {
        long sum = 3*4;//nodeType,level,children
        for (int u32Child = 0; u32Child < this.m_children; ++u32Child){
            sum+=this.m_ptrMBR[u32Child].getByteArraySize();
            sum+=this.m_pIdentifier[u32Child].getByteArraySize();
            sum+=4;// data length
        }
        sum+=this.m_totalDataLength;
        sum+=this.m_nodeMBR.getByteArraySize();
        return sum;
    }

    /**
     * In order to reduce memory copy, the three parameters could not be changed
     * after this function called.The safe way is to set them null after calling.
     * 为了提高效率，减少内存复制，这里传入的三个参数在这个函数执行后，
     * 不能在进行修改，否则将导致传入的数据在这个函数后也会被连带修改;因此，
     * 这个函数执行后，最好将这三个参数都设置为null,避免错误的修改。
     *eg：
     *      insertEntry(pData,  mbr, id)；
     *      pData=null;
     *      mbr=null;
     *      id=null;
     *
     */
    protected void  insertEntry(byte[] pData, Region mbr, Identifier id) {
        assert(this.m_children < this.m_capacity);
        this.m_pData[this.m_children] = pData;
        this.m_ptrMBR[this.m_children] = (Region) mbr;
        this.m_pIdentifier[this.m_children] = (Identifier) id;
        if(pData!=null)
            this.m_totalDataLength += pData.length;
        ++this.m_children;
        this.m_nodeMBR.combineRegion(mbr);
    }

    /**
     *
     * @param index index >= 0 && index < m_children
     */
    protected void deleteEntry(int index) {
        assert(index >= 0 && index < this.m_children);
        // cache it, since I might need it for "touches" later.
        Region ptrR = this.m_ptrMBR[index];
        if(this.m_pData[index]!=null)
            this.m_totalDataLength -= this.m_pData[index].length;
        if (this.m_children > 1 && index != this.m_children - 1){
            this.m_pData[index] = this.m_pData[this.m_children - 1];
            this.m_ptrMBR[index] = this.m_ptrMBR[this.m_children - 1];
            this.m_pIdentifier[index] = this.m_pIdentifier[this.m_children - 1];
        }
        --this.m_children;
        // WARNING: index has now changed. Do not use it below here.
        if (this.m_children == 0){
            this.m_nodeMBR.makeInfinite();
        }
        else if (this.m_pTree.m_bTightMBRs && this.m_nodeMBR.touchesRegion(ptrR)){
            //重新计算节点的矩形区域
            for (int cDim = 0; cDim < this.m_nodeMBR.getDimension(); ++cDim){
                this.m_nodeMBR.setLowCoordinate(cDim,Double.MAX_VALUE);
                this.m_nodeMBR.setHighCoordinate(cDim,-Double.MAX_VALUE);
                for (int u32Child = 0; u32Child < m_children; ++u32Child){
                    this.m_nodeMBR.setLowCoordinate(cDim,Math.min(this.m_nodeMBR.getLowCoordinate(cDim),m_ptrMBR[u32Child].getLowCoordinate(cDim)));
                    this.m_nodeMBR.setHighCoordinate(cDim,Math.max(this.m_nodeMBR.getHighCoordinate(cDim),m_ptrMBR[u32Child].getHighCoordinate(cDim)));
                }
            }
        }
    }
    /**
     * 注意：为了提高效率，减少内存复制，这里传入的三个参数pData,  mbr, id,
     * 在这个函数执行后，不能在进行修改，否则将导致传入的数据在这个函数后也会被连带修改，
     * 因此，这个函数执行后，最好将这三个参数都设置为null,避免错误的修改。
     *例如：
     *      insertData(pData,  mbr, id)；
     *      pData=null;
     *      mbr=null;
     *      id=null;
     */
    protected boolean insertData(byte[] pData, Region mbr, Identifier id, Stack<Identifier> pathBuffer, byte[] overflowTable) {
        // 如果子节点个数小于容量
        if (m_children < m_capacity){
            boolean adjusted = false;

            // this has to happen before insertEntry modifies m_nodeMBR.
            boolean b = this.m_nodeMBR.containsRegion(mbr);

            this.insertEntry( pData, mbr, id);
            pData=null;
            mbr=null;
            id=null;

            this.m_pTree.writeNode(this);

            if ((! b) && (! pathBuffer.empty())){
                Identifier cParent = pathBuffer.pop();
                Node ptrN = this.m_pTree.readNode(cParent);
                InternalRTreeNodeImplBack p = (InternalRTreeNodeImplBack)(ptrN);
                p.adjustTree(this, pathBuffer);
                adjusted = true;
            }
            return adjusted;
        }
        else if (this.m_pTree.m_treeVariant == RTreeVariant.RV_RSTAR && (! pathBuffer.empty()) && overflowTable[m_level] == 0){
            //如果是RStarTree则需要重新插入
            overflowTable[m_level] = 1;
            //记录需要重插的孩子节点下标
            ArrayList<Integer> vReinsert=new ArrayList<Integer>();
            //记录需要重插的孩子节点下标
            ArrayList<Integer>  vKeep=new ArrayList<Integer>();
            //执行RStarTree的重插数据，得到哪些子节点是需要保留的，哪些是需要重插的
            this.reinsertData(pData, mbr, id, vReinsert, vKeep);
            pData=null;
            mbr=null;
            id=null;
            //需要重插的项数
            int lReinsert = vReinsert.size();
            //需要保留的项数
            int lKeep = vKeep.size();

            byte[][]  reinsertdata = new byte[lReinsert][];
            Region[] reinsertmbr = new Region[lReinsert];
            Identifier[] reinsertid = new Identifier[lReinsert];

            byte[][] keepdata = new byte[m_capacity + 1][];
            Region[] keepmbr = new Region[m_capacity + 1];
            Identifier[] keepid = new Identifier[m_capacity + 1];

            int cIndex;
            //需要重新插入的数据项
            for (cIndex = 0; cIndex < lReinsert; ++cIndex){
                reinsertdata[cIndex] = m_pData[vReinsert.get(cIndex)];
                reinsertmbr[cIndex] = m_ptrMBR[vReinsert.get(cIndex)];
                reinsertid[cIndex] = m_pIdentifier[vReinsert.get(cIndex)];
            }
            //需要保留的数据项
            for (cIndex = 0; cIndex < lKeep; ++cIndex){
                keepdata[cIndex] = m_pData[vKeep.get(cIndex)];
                keepmbr[cIndex] = m_ptrMBR[vKeep.get(cIndex)];
                keepid[cIndex] = m_pIdentifier[vKeep.get(cIndex)];
            }
            //将本节点设置为保留数据项
            this.m_pData = keepdata;
            this.m_ptrMBR = keepmbr;
            this.m_pIdentifier = keepid;
            this.m_children = lKeep;
            this.m_totalDataLength = 0;
            for (int u32Child = 0; u32Child < this.m_children; ++u32Child)
                this.m_totalDataLength += keepdata[u32Child].length;
            //重新计算节点的边界矩形
            for (int cDim = 0; cDim < this.m_nodeMBR.getDimension(); ++cDim){
                this.m_nodeMBR.setLowCoordinate(cDim,Double.MAX_VALUE);
                this.m_nodeMBR.setHighCoordinate(cDim, -Double.MAX_VALUE);

                for (int u32Child = 0; u32Child < this.m_children; ++u32Child){
                    this.m_nodeMBR.setLowCoordinate(cDim,Math.min(this.m_nodeMBR.getLowCoordinate(cDim),this.m_ptrMBR[u32Child].getLowCoordinate(cDim)));
                    this.m_nodeMBR.setHighCoordinate(cDim,Math.max(this.m_nodeMBR.getHighCoordinate(cDim),this.m_ptrMBR[u32Child].getHighCoordinate(cDim)));
                }
            }
            keepdata=null;
            keepmbr=null;
            keepid=null;

            this.m_pTree.writeNode(this);

            // Divertion from R*-Tree algorithm here. First adjust
            // the path to the root, then start reinserts, to avoid complicated handling
            // of changes to the same node from multiple insertions.
            Identifier cParent = pathBuffer.pop();
            Node ptrN = this.m_pTree.readNode(cParent);
            InternalRTreeNodeImplBack p = (InternalRTreeNodeImplBack)ptrN;
            p.adjustTree(this, pathBuffer);
            //将需要重新插入的数据进行重插
            for (cIndex = 0; cIndex < lReinsert; ++cIndex){
                this.m_pTree.insertData_impl(
                        reinsertdata[cIndex],
                        reinsertmbr[cIndex], reinsertid[cIndex],
                        m_level, overflowTable);
                reinsertdata[cIndex]=null;
                reinsertmbr[cIndex]=null;
                reinsertid[cIndex]=null;
            }
            return true;
        }
        else  {
            Node [] nodes = split(pData,mbr,id);
            pData=null;
            mbr=null;
            id=null;

            RTreeNodeImplBackup n=(RTreeNodeImplBackup)nodes[0];
            RTreeNodeImplBackup nn=(RTreeNodeImplBackup)nodes[1];

            if (pathBuffer.empty()){
                n.m_level = this.m_level;
                nn.m_level = this.m_level;
                n.m_identifier.reset(-1);
                nn.m_identifier.reset(-1);
                m_pTree.writeNode(n);
                m_pTree.writeNode(nn);

                RTreeNodeImplBackup ptrR = new InternalRTreeNodeImplBack(this.m_pTree,m_pTree.m_rootID,this.m_level+1);

                ptrR.insertEntry(null, n.m_nodeMBR, n.m_identifier);
                ptrR.insertEntry(null, nn.m_nodeMBR, nn.m_identifier);

                m_pTree.writeNode(ptrR);
                n=null;
                nn=null;
                nodes=null;

                StatisticsImpl s =(StatisticsImpl) (m_pTree.m_stats);
                s.setNodeNumberInLevel(m_level,2L);
                m_pTree.m_stats.getNodeNumberInLevelArray().add(1L);
                m_pTree.m_stats.setTreeHeight(m_level + 2);
            }
            else {
                n.m_level = this.m_level;
                nn.m_level = this.m_level;
                n.m_identifier.reset(this.m_identifier.longValue());
                nn.m_identifier.reset(-1L);

                m_pTree.writeNode(n);
                m_pTree.writeNode(nn);

                Identifier cParent =  pathBuffer.pop();
                InternalRTreeNodeImplBack ptrN =(InternalRTreeNodeImplBack) m_pTree.readNode(cParent);
                //ptrN.adjustTree(n, nn, pathBuffer, overflowTable);
                n=null;
                nn=null;
                nodes=null;
            }
            return true;
        }
    }

    /**
     * 计算插入节点（ pData,  mbr,  id）时候，
     * 本节点中哪些项需要重新插入，其下标纪录在reinsert中，
     * 需要保留的项，其下标纪录在keep中
     * @param pData
     * @param mbr
     * @param id
     * @param reinsert
     * @param keep
     */
    protected void reinsertData(byte[] pData, Region mbr, Identifier id, ArrayList<Integer> reinsert, ArrayList<Integer> keep){
        ReinsertEntry[]v = new ReinsertEntry[m_capacity + 1];

        m_pData[m_children] = pData;
        m_ptrMBR[m_children] = mbr;
        m_pIdentifier[m_children]=id;
        pData=null;
        mbr=null;
        id=null;

        Vertex nc =m_nodeMBR.getCenter();
        Vertex c = null;

        for (int u32Child = 0; u32Child < m_capacity + 1; ++u32Child){
            v[u32Child] = new ReinsertEntry(u32Child, 0.0);
            c=m_ptrMBR[u32Child].getCenter();

            // calculate relative distance of every entry from the node MBR (ignore square root.)
            for (int cDim = 0; cDim < m_nodeMBR.getDimension(); ++cDim){
                double d = nc.getCoordinate(cDim) - c.getCoordinate(cDim);
                v[u32Child].dist += d * d;
            }
        }

        // sort by increasing order of distances.
        java.util.Arrays.sort(v);

        int cReinsert = (int)(Math.floor((m_capacity + 1) * m_pTree.m_reinsertFactor));

        int cCount;

        for (cCount = 0; cCount < cReinsert; ++cCount) {
            reinsert.add(v[cCount].index);
        }

        for (cCount = cReinsert; cCount < m_capacity + 1; ++cCount){
            keep.add(v[cCount].index);
        }
    }

    /**
     * 普通Rtree的分解策略
     * @param pData
     * @param mbr
     * @param id
     * @param group1
     * @param group2
     */
    protected void rtreeSplit(byte[] pData, Region mbr, Identifier id, ArrayList<Integer> group1, ArrayList<Integer> group2){
        int u32Child;
        int minimumLoad = (int)(Math.floor(m_capacity * m_pTree.m_fillFactor));

        // use this mask array for marking visited entries.
	    byte[] mask = new byte[m_capacity + 1];
        java.util.Arrays.fill(mask,(byte)0);
        // insert new data in the node for easier manipulation. Data arrays are always
        // by one larger than node capacity.
        m_pData[m_capacity] = pData;
        m_ptrMBR[m_capacity] = mbr;
        m_pIdentifier[m_capacity]=id;
        pData=null;
        mbr=null;
        id=null;
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
        Region  mbr1 =(Region) m_ptrMBR[seed1].clone();
        Region  mbr2 =(Region) m_ptrMBR[seed2].clone();

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
                        a=mbr1.getCombinedRegion(m_ptrMBR[u32Child]);
                        d1 = a.getArea() - a1;
                        b=mbr2.getCombinedRegion(m_ptrMBR[u32Child]);
                        d2 = b.getArea() - a2;
                        d = Math.abs(d1 - d2);

                        if (d > m) {
                            m = d;
                            md1 = d1; md2 = d2;
                            sel = u32Child;
                            if (m_pTree.m_treeVariant== RTreeVariant.RV_LINEAR || m_pTree.m_treeVariant == RTreeVariant.RV_RSTAR)
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
                    mbr1.combineRegion(m_ptrMBR[sel]);
                }
                else {
                    mbr2.combineRegion(m_ptrMBR[sel]);
                }
            }
        }
    }

    /**
     * RStarTree的分解策略
     * @param pData
     * @param mbr
     * @param id
     * @param group1
     * @param group2
     */
    protected void rstarSplit(byte[] pData, Region mbr, Identifier id, ArrayList<Integer> group1, ArrayList<Integer> group2){
        RstarSplitEntry[] dataLow = new RstarSplitEntry[m_capacity + 1];
        RstarSplitEntry[] dataHigh = new RstarSplitEntry[m_capacity + 1];

        m_pData[m_capacity] = pData;
        m_ptrMBR[m_capacity] = mbr;
        m_pIdentifier[m_capacity]=id;
        pData=null;
        mbr=null;
        id=null;
        // m_totalDataLength does not need to be increased here.

        int nodeSPF =(int)( Math.floor((m_capacity + 1) * m_pTree.m_splitDistributionFactor));
        int splitDistribution = (m_capacity + 1) - (2 * nodeSPF) + 2;

        int u32Child = 0, cDim, cIndex;

        for (u32Child = 0; u32Child <= m_capacity; ++u32Child) {
            dataLow[u32Child] = new RstarSplitEntry(m_ptrMBR[u32Child], u32Child, 0);
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
        for (cDim = 0; cDim < m_pTree.m_dimension; ++cDim){
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

    //

    /**
     * 选择种子，为了能将参数传递进去，
     * 并将修改值带出来，采用数组实现
     * @param indexes
     */
    protected void pickSeeds(int []indexes){
        int index1=indexes[0];
        int index2=indexes[1];

        double separation = -Double.MAX_VALUE;
        double inefficiency = -Double.MAX_VALUE;
        int cDim, u32Child, cIndex;

        switch (m_pTree.m_treeVariant) {
            case RV_LINEAR:
            case RV_RSTAR: {
                for (cDim = 0; cDim < m_pTree.m_dimension; ++cDim) {
                    double leastLower = this.m_ptrMBR[0].getLowCoordinate(cDim);
                    double greatestUpper = this.m_ptrMBR[0].getHighCoordinate(cDim);
                    int greatestLower = 0;
                    int leastUpper = 0;
                    double width;

                    for (u32Child = 1; u32Child <= m_capacity; ++u32Child) {
                        if (m_ptrMBR[u32Child].getLowCoordinate(cDim) > m_ptrMBR[greatestLower].getLowCoordinate(cDim))
                            greatestLower = u32Child;
                        if (m_ptrMBR[u32Child].getHighCoordinate(cDim) < m_ptrMBR[leastUpper].getHighCoordinate(cDim))
                            leastUpper = u32Child;

                        leastLower = Math.min(m_ptrMBR[u32Child].getLowCoordinate(cDim), leastLower);
                        greatestUpper = Math.max(m_ptrMBR[u32Child].getHighCoordinate(cDim), greatestUpper);
                    }

                    width = greatestUpper - leastLower;
                    if (width <= 0) width = 1;

                    double f = (m_ptrMBR[greatestLower].getLowCoordinate(cDim) - m_ptrMBR[leastUpper].getHighCoordinate(cDim)) / width;

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
                    double a = m_ptrMBR[u32Child].getArea();

                    for (cIndex = u32Child + 1; cIndex <= m_capacity; ++cIndex) {
                        // get the combined MBR of those two entries.
                        Region r= m_ptrMBR[u32Child].getCombinedRegion( m_ptrMBR[cIndex]);

                        // find the inefficiency of grouping these entries together.
                        double d = r.getArea() - a - m_ptrMBR[cIndex].getArea();

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

    /**
     *
     * @param toReinsert
     * @param pathBuffer
     * @param ptrThis
     */
    void condenseTree(Stack<Node> toReinsert, Stack<Identifier> pathBuffer, Node  ptrThis){
        int minimumLoad = (int)(Math.floor(m_capacity * m_pTree.m_fillFactor));
        double d1,d2;//临时变量
        int dims=0;
        if (pathBuffer.empty()){
            // eliminate root if it has only one child.
            if (m_level != 0 && m_children == 1){
                RTreeNodeImplBackup ptrN = (RTreeNodeImplBackup)m_pTree.readNode(m_pIdentifier[0]);
                m_pTree.deleteNode(ptrN);
                ptrN.m_identifier.reset(m_pTree.m_rootID.longValue());
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
                if (p.getChildIdentifier(child).equals(m_identifier))
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
                assert this.m_nodeMBR.copyTo(p.getChildShape(child))!=null;

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

    abstract Node chooseSubtree( Region mbr, int level, Stack<Identifier> pathBuffer);

    abstract Node findLeaf(Region mbr, Identifier id, Stack<Identifier> pathBuffer);

    //
    public abstract Node[] split(byte[] pData, Region mbr, Identifier id ) ;

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
