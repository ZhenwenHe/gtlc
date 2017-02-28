package gtl.stil.rtree.impl;

import gtl.stil.*;
import gtl.stil.exception.IllegalArgumentException;
import gtl.stil.impl.EntryImpl;
import gtl.stil.impl.IdentifierImpl;
import gtl.stil.impl.PropertySetImpl;
import gtl.stil.rtree.RTree;
import gtl.stil.rtree.RTreeVariant;
import gtl.stil.rtree.RangeQueryType;
import gtl.stil.shape.Point;
import gtl.stil.shape.Region;
import gtl.stil.shape.Shape;
import gtl.stil.shape.impl.RegionImpl;
import gtl.stil.storage.StorageManager;

import java.io.*;
import java.util.*;

/**
 * Created by ZhenwenHe on 2016/12/19.
 */
public class RTreeImpl implements RTree{

    /**
     * String                   Value     Description
     * ----------------------------------------------
     * IndexIdentifier         Identifier   If specified an existing index will be openened from the supplied
     *                          storage manager with the given index id. Behaviour is unspecified
     *                          if the index id or the storage manager are incorrect.
     * Dimension                Integer  Dimensionality of the data that will be inserted.
     * IndexCapacity            Integer  The index node capacity. Default is 100.
     * LeafCapacity             Integer  The leaf node capacity. Default is 100.
     * FillFactor               Double The fill factor. Default is 70%
     * TreeVariant              Integer   Can be one of Linear, Quadratic or Rstar. Default is Rstar
     * NearMinimumOverlapFactor Integer  Default is 32.
     * SplitDistributionFactor  Double Default is 0.4
     * ReinsertFactor           Double Default is 0.3
     * EnsureTightMBRs          Boolean   Default is true
     * @param storageManager
     * @param propSet
     */
    public RTreeImpl(StorageManager storageManager,PropertySet propSet) {
        reset(storageManager,propSet);
    }

    /**
     * String                   Value     Description
     * ----------------------------------------------
     * IndexIdentifier         Identifier   If specified an existing index will be openened from the supplied
     *                          storage manager with the given index id. Behaviour is unspecified
     *                          if the index id or the storage manager are incorrect.
     * Dimension                Integer  Dimensionality of the data that will be inserted.
     * IndexCapacity            Integer  The index node capacity. Default is 100.
     * LeafCapacity             Integer  The leaf node capacity. Default is 100.
     * FillFactor               Double The fill factor. Default is 70%
     * TreeVariant              Integer   Can be one of Linear, Quadratic or Rstar. Default is Rstar
     * NearMinimumOverlapFactor Integer  Default is 32.
     * SplitDistributionFactor  Double Default is 0.4
     * ReinsertFactor           Double Default is 0.3
     * EnsureTightMBRs          Boolean   Default is true
     * @param storageManager
     * @param propSet
     */
    @Override
    public void reset(StorageManager storageManager,PropertySet propSet) {
        this.storageManager =storageManager;
        this.rootIdentifier =IndexSuits.createIdentifier(StorageManager.NEW_PAGE);
        this.headerIdentifier =IndexSuits.createIdentifier(StorageManager.NEW_PAGE);
        this.treeVariant =RTreeVariant.RV_RSTAR;
        this.fillFactor =0.7;
        this.indexCapacity =100;
        this.leafCapacity =100;
        this.nearMinimumOverlapFactor =32;
        this.splitDistributionFactor =(0.4);
        this.reinsertFactor =(0.3);
        this.dimension =(2);
        this.tightMBRs =(true);
        this.infiniteRegion =IndexSuits.createRegion();
        this.infiniteRegion.makeInfinite(this.dimension);
        this.stats =new StatisticsImpl();

        try {
            Variant v = propSet.getProperty("IndexIdentifier");
            if (v != null && !v.isEmpty()) {
                if (v.isNumber())
                    this.headerIdentifier.reset(v.longValue());
                else
                    throw new IllegalArgumentException("RTree: Property IndexIdentifier must be number");

                initOld(propSet);
            } else {
                initNew(propSet);
                v=new Variant(this.headerIdentifier.longValue());
                propSet.put("IndexIdentifier", v);
            }
        }
        catch (Exception e){
            e.printStackTrace();
        }
    }

    @Override
    public void reset(StorageManager storageManager,
                      Identifier indexIdentifier,
                      int dimension,
                      int indexCapacity,
                      int leafCapacity,
                      double fillFactor,
                      RTreeVariant treeVariant,
                      int nearMinimumOverlapFactor,
                      double splitDistributionFactor,
                      double reinsertFactor,
                      boolean ensureTightMBRs) {

        this.storageManager =storageManager;
        this.rootIdentifier =IndexSuits.createIdentifier(StorageManager.NEW_PAGE);
        this.headerIdentifier =IndexSuits.createIdentifier(StorageManager.NEW_PAGE);
        this.treeVariant =treeVariant;//RTreeVariant.RV_RSTAR;
        this.fillFactor =fillFactor>0? fillFactor: 0.7;
        this.indexCapacity =indexCapacity>0?indexCapacity:100;
        this.leafCapacity =leafCapacity>0?leafCapacity:100;
        this.nearMinimumOverlapFactor =nearMinimumOverlapFactor>0?nearMinimumOverlapFactor:32;
        this.splitDistributionFactor =splitDistributionFactor>0?splitDistributionFactor:0.4;
        this.reinsertFactor =reinsertFactor>0?reinsertFactor:0.3;
        this.dimension =dimension>1?dimension:2;
        this.tightMBRs =tightMBRs;//(true);
        this.infiniteRegion =IndexSuits.createRegion();
        this.infiniteRegion.makeInfinite(this.dimension);
        this.stats =new StatisticsImpl();

        if(indexIdentifier==null){//new
            this.stats.treeHeight = 1;
            this.stats.getNodeNumberInLevelArray().add(0L);
            RTreeExternalNodeImpl root =new RTreeExternalNodeImpl(this, IndexSuits.createIdentifier(-1L));
            this.rootIdentifier = writeNode(root);
            storeHeader();
        }
        else {//old
            this.headerIdentifier.reset(indexIdentifier.longValue());
            loadHeader();
            this.infiniteRegion.makeInfinite(this.dimension);
        }
     }

    @Override
    public void insert(byte[] pData, Shape shape, Identifier shapeIdentifier) {
        if (shape.getDimension() != this.dimension) return;

        // convert the shape into a Region (R-Trees index regions only; i.e., approximations of the shapes).
        Region mbr = new RegionImpl(shape.getMBR());
        insertData_impl(pData, mbr, shapeIdentifier);
    }

    @Override
    public boolean delete(Shape shape, Identifier shapeIdentifier) {
        if (shape.getDimension() != this.dimension) return false;
        Region  mbr = new RegionImpl(shape.getMBR());
        boolean ret = deleteData_impl(mbr, shapeIdentifier);
        return ret;
    }

    @Override
    public void contains(Shape query, Visitor v) {
        try{
            if (query.getDimension() != this.dimension)
                throw new IllegalArgumentException("containsWhatQuery: Shape has the wrong number of dimensions.");
            Stack<Node> st=new Stack<>();
            Node root = readNode(this.rootIdentifier);
            st.push(root);
            while (! st.empty()){
                Node n = st.pop();
                if(n.getLevel() == 0){
                    v.visitNode(n);
                    for (int cChild = 0; cChild < n.getChildrenCount(); ++cChild){
                        if(query.containsShape(n.getChildShape(cChild))){
                            Entry data = new EntryImpl(n.getChildIdentifier(cChild),n.getChildShape(cChild),n.getChildData(cChild));
                            v.visitData(data);
                            this.stats.increaseQueryResults();
                        }
                    }
                }
                else {  //not a leaf
                    if(query.containsShape(n.getShape())){
                        visitSubTree(n, v);
                    }
                    else if(query.intersectsShape(n.getShape())){
                        v.visitNode(n);
                        for (int cChild = 0; cChild < n.getChildrenCount(); ++cChild){
                            st.push(readNode(n.getChildIdentifier(cChild)));
                        }
                    }
                }
            }
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }

    @Override
    public void intersects(Shape query, Visitor v) {
        try{
            if (query.getDimension() != this.dimension)
                throw new IllegalArgumentException("intersectsWithQuery: Shape has the wrong number of dimensions.");
            range(RangeQueryType.RQT_INTERSECTION_QUERY, query, v);
        }
        catch (Exception e){
            e.printStackTrace();
        }
    }

    @Override
    public void pointLocation(Point query, Visitor v) {
        try{
            if (query.getDimension() != this.dimension)
                throw new IllegalArgumentException("pointLocationQuery: Shape has the wrong number of dimensions.");
            Region r=new RegionImpl(query.getCoordinates(), query.getCoordinates());
            range(RangeQueryType.RQT_INTERSECTION_QUERY, r, v);
        }
        catch (Exception e){
            e.printStackTrace();
        }
    }

    @Override
    public void nearestNeighbor(int k, Shape query, Visitor v, NearestNeighborComparator nnc) {
        try {
            if (query.getDimension() != this.getDimension())
                throw new IllegalArgumentException("nearestNeighborQuery: Shape has the wrong number of dimensions.");

            PriorityQueue<NNEntry> queue=new PriorityQueue<NNEntry>();

            queue.add(new NNEntry(this.rootIdentifier, null, 0.0));

            int count = 0;
            double knearest = 0.0;

            while (! queue.isEmpty()){
                NNEntry pFirst = queue.peek();
                // report all nearest neighbors with equal greatest distances.
                // (neighbors can be more than k, if many happen to have the same greatest distance).
                if (count >= k && pFirst.m_minDist > knearest)	break;

                queue.poll();

                if (pFirst.m_pEntry ==null) {
                    // n is a leaf or an index.
                    Node  n = readNode(pFirst.m_id);
                    v.visitNode(n);

                    for (int cChild = 0; cChild < n.getChildrenCount(); ++cChild) {
                        if (n.getLevel() == 0) {
                            Entry e = new EntryImpl(n.getChildIdentifier(cChild),n.getChildShape(cChild),n.getChildData(cChild));
                            // we need to compare the query with the actual data entry here, so we call the
                            // appropriate getMinimumDistance method of NearestNeighborComparator.
                            queue.add(new NNEntry(n.getChildIdentifier(cChild), e, nnc.getMinimumDistance(query, e)));
                        }
                        else
                        {
                            queue.add(new NNEntry(n.getChildIdentifier(cChild), null, nnc.getMinimumDistance(query, n.getChildShape(cChild))));
                        }
                    }
                }
                else
                {
                    v.visitData(pFirst.m_pEntry);
                    this.stats.increaseQueryResults();
                    ++count;
                    knearest = pFirst.m_minDist;
                }
            }
            queue.clear();
        }
        catch (Exception e) {
            e.printStackTrace();
        }

    }

    @Override
    public void nearestNeighbor(int k, Shape query, Visitor v) {
        try{
            if (query.getDimension() != this.dimension)
                throw new IllegalArgumentException("nearestNeighborQuery: Shape has the wrong number of dimensions.");
            nearestNeighbor(k, query, v, new NNComparator());
        }
        catch (Exception e){
            e.printStackTrace();
        }
    }

    @Override
    public void selfJoin(Shape query, Visitor v) {
        try{
            if (query.getDimension() != this.dimension)
                throw new IllegalArgumentException("selfJoinQuery: Shape has the wrong number of dimensions.");

            Region mbr = new RegionImpl(query.getMBR());
            selfJoin(this.rootIdentifier, this.rootIdentifier, mbr, v);
        }
        catch (Exception e){
            e.printStackTrace();
        }
    }

    @Override
    public void queryStrategy(QueryStrategy qs) {
        Identifier next = (Identifier)this.rootIdentifier.clone();
        Boolean hasNext = true;

        while (hasNext){
            Node n = readNode(next);
            qs.getNextEntry(n, next, hasNext);
        }
    }

    @Override
    public PropertySet getProperties() {
        PropertySet ps = new PropertySetImpl();

        // dimension
        ps.put("Dimension", new Variant(this.dimension));


        // index capacity
        ps.put("IndexCapacity", new Variant(this.indexCapacity));

        // leaf capacity
        ps.put("LeafCapacity", new Variant(this.leafCapacity));

        // R-tree variant
        ps.put("TreeVariant", new Variant(this.treeVariant.ordinal()));

        // fill factor
        ps.put("FillFactor", new Variant(this.fillFactor));

        // near minimum overlap factor
        ps.put("NearMinimumOverlapFactor", new Variant(this.nearMinimumOverlapFactor));

        // split distribution factor
        ps.put("SplitDistributionFactor", new Variant(this.splitDistributionFactor));

        // reinsert factor
        ps.put("ReinsertFactor", new Variant(this.reinsertFactor));

        // tight MBRs
        ps.put("EnsureTightMBRs", new Variant(this.tightMBRs));

        return ps;
    }

    @Override
    public void addCommand(Command in, CommandType ct) {
        switch (ct)
        {
            case CT_NODEREAD:
                this.readNodeCommands.add(in);
                break;
            case CT_NODEWRITE:
                this.writeNodeCommands.add(in);
                break;
            case CT_NODEDELETE:
                this.deleteNodeCommands.add(in);
                break;
        }
    }

    @Override
    public boolean isValid() {
        boolean ret = true;
        Stack<ValidateEntry>  st=new Stack<>();
        Node root = readNode(this.rootIdentifier);
        int level = root.getLevel();
        Region r =null;
        if (level != this.stats.getTreeHeight() - 1)
            return false;

        HashMap<Integer,Integer>  nodesInLevel= new HashMap<>();
        nodesInLevel.put(level,1);
        r = (Region)root.getShape();
        ValidateEntry e=new ValidateEntry(r, root);
        st.push(e);

        while (! st.empty()) {
            e = st.pop();

            Region tmpRegion=(Region) this.infiniteRegion.clone();

            for (int cDim = 0; cDim < tmpRegion.getDimension(); ++cDim) {
                tmpRegion.setLowCoordinate(cDim,Double.MAX_VALUE);
                tmpRegion.setHighCoordinate(cDim,-Double.MAX_VALUE);
                for (int cChild = 0; cChild < e.node.getChildrenCount(); ++cChild){
                    r= (Region) e.node.getChildShape(cChild);
                    tmpRegion.setLowCoordinate(cDim,Math.min(tmpRegion.getLowCoordinate(cDim),r.getLowCoordinate(cDim)));
                    tmpRegion.setHighCoordinate(cDim,Math.max(tmpRegion.getHighCoordinate(cDim),r.getHighCoordinate(cDim)));
                }
            }

            if (! (tmpRegion.equals( e.node.getShape()))) {
                ret = false;
            }
            else if (! (tmpRegion.equals(e.parentMBR))){
                ret = false;
            }

            if (e.node.getLevel() != 0) {
                for (int cChild = 0; cChild < e.node.getChildrenCount(); ++cChild){
                    Node ptrN = readNode(e.node.getChildIdentifier(cChild));
                    ValidateEntry tmpEntry=new ValidateEntry((Region) e.node.getChildShape(cChild), ptrN);

                    Integer itNodes = nodesInLevel.get(tmpEntry.node.getLevel());

                    if (itNodes == null){
                        nodesInLevel.put( tmpEntry.node.getLevel(), 1);
                    }
                    else {
                        Integer key = tmpEntry.node.getLevel();
                        Integer va= nodesInLevel.get(key);
                        va+=1;
                        nodesInLevel.put(key,va);
                    }

                    st.push(tmpEntry);
                }
            }
        }

        int nodes = 0;
        for (int cLevel = 0; cLevel < this.stats.getTreeHeight(); ++cLevel){
            if (nodesInLevel.get(cLevel) != this.stats.getNodeNumberInLevel(cLevel)){
                ret = false;
            }
            nodes += this.stats.getNodeNumberInLevel(cLevel);
        }

        if (nodes != this.stats.getNodeNumber()){
            ret = false;
        }

        return ret;
    }

    @Override
    public Statistics getStatistics() {
        return this.stats;
    }

    public int getDimension(){
        return this.dimension;
    }



    class NNEntry implements Comparable<NNEntry> {
        Identifier m_id;
        Entry m_pEntry;
        double m_minDist;

        NNEntry(Identifier id, Entry e, double f){
            this.m_id=(Identifier) id.clone();
            this.m_pEntry=e;
            this.m_minDist=f;
        }
        NNEntry() {
        }

        @Override
        public int compareTo(NNEntry o) {
            if(this.m_minDist>o.m_minDist)
                return 1;
            else if(this.m_minDist==o.m_minDist)
                return 0;
            else
                return -1;
        }
    }; // NNEntry

    class NNComparator implements NearestNeighborComparator {
        @Override
        public double getMinimumDistance(Shape query, Shape entry) {
            return query.getMinimumDistance(entry);
        }

        @Override
        public double getMinimumDistance(Shape query, Entry data) {
            Shape   pS=data.getShape();
            double ret = query.getMinimumDistance(pS);
            return ret;
        }
    }; // NNComparator

    class ValidateEntry {
        ValidateEntry(Region r, Node pNode){
            this.parentMBR=(Region) r.clone();
            this.node=pNode;
        }

        Region parentMBR;
        Node node;
    }; // ValidateEntry

    void initNew(PropertySet ps){
        Variant v=null;
        try{
            // tree variant
            v = ps.getProperty("TreeVariant");
            if (v!=null && !v.isEmpty()){
                if(v.isNumber()==false)
                    throw new IllegalArgumentException("initNew: Property TreeVariant must be number");

                int rv = v.intValue();
                if(rv != RTreeVariant.RV_LINEAR.ordinal() &&
                        rv !=  RTreeVariant.RV_QUADRATIC.ordinal() &&
                        rv!= RTreeVariant.RV_RSTAR.ordinal())
                    throw new IllegalArgumentException("initNew: Property TreeVariant must be of RTreeVariant type");;
                this.treeVariant = RTreeVariant.values()[rv];
            }

            // fill factor
            // it cannot be larger than 50%, since linear and quadratic split algorithms
            // require assigning to both nodes the same number of entries.
            v = ps.getProperty("FillFactor");
            if (v!=null && !v.isEmpty()) {
                if(!v.isNumber())
                    throw new IllegalArgumentException("initNew: Property FillFactor was not of type number");
                double rv = v.doubleValue();
                if (rv <= 0.0)  throw new IllegalArgumentException("initNew: Property FillFactor was less than 0.0");

                if (((this.treeVariant == RTreeVariant.RV_LINEAR || this.treeVariant == RTreeVariant.RV_QUADRATIC) && rv > 0.5))
                    throw new IllegalArgumentException(  "initNew: Property FillFactor must be in range (0.0, 0.5) for LINEAR or QUADRATIC index types");
                if ( rv >= 1.0)
                    throw new IllegalArgumentException(  "initNew: Property FillFactor must be in range (0.0, 1.0) for RSTAR index type");
                this.fillFactor = rv;
            }

            // index capacity
            v = ps.getProperty("IndexCapacity");
            if (v!=null && !v.isEmpty()){
                if (!v.isNumber() || v.intValue() < 4)
                    throw new IllegalArgumentException("initNew: Property IndexCapacity must be Number and >= 4");

                this.indexCapacity = v.intValue();
            }
            // leaf capacity
            v = ps.getProperty("LeafCapacity");
            if (v!=null && !v.isEmpty()){
                if (!v.isNumber() || v.longValue() < 4)
                    throw new IllegalArgumentException("initNew: Property LeafCapacity must be number and >= 4");

                this.leafCapacity = v.intValue();
            }
            // near minimum overlap factor
            v = ps.getProperty("NearMinimumOverlapFactor");
            if (v!=null && !v.isEmpty()){
                if (
                        !v.isNumber() ||
                                v.intValue() < 1 ||
                                v.intValue() > this.indexCapacity ||
                                v.intValue() > this.leafCapacity)
                    throw new IllegalArgumentException("initNew: Property NearMinimumOverlapFactor must be Number and less than both index and leaf capacities");

                this.nearMinimumOverlapFactor = v.intValue();
            }
            // split distribution factor
            v = ps.getProperty("SplitDistributionFactor");
            if (v!=null && !v.isEmpty()){
                if (!v.isNumber() ||v.doubleValue() <= 0.0 || v.doubleValue() >= 1.0)
                    throw new IllegalArgumentException("initNew: Property SplitDistributionFactor must be number and in (0.0, 1.0)");

                this.splitDistributionFactor =v.doubleValue();
            }
            // reinsert factor
            v = ps.getProperty("ReinsertFactor");
            if (v!=null && !v.isEmpty()){
                if (!v.isNumber() ||v.doubleValue()<= 0.0 ||v.doubleValue() >= 1.0)
                    throw new IllegalArgumentException("initNew: Property ReinsertFactor must be number and in (0.0, 1.0)");

                this.reinsertFactor = v.doubleValue();
            }
            // dimension
            v = ps.getProperty("Dimension");
            if (v!=null && !v.isEmpty()){
                if (!v.isNumber())
                    throw new IllegalArgumentException("initNew: Property Dimension must be number");
                int rv = v.intValue();
                if (rv <= 1)
                    throw new IllegalArgumentException("initNew: Property Dimension must be greater than 1");

                this.dimension = rv;
            }
            // tight MBRs
            v = ps.getProperty("EnsureTightMBRs");
            if (v!=null && !v.isEmpty()){
                if (v.getType()!=Variant.BOOLEAN)
                    throw new IllegalArgumentException("initNew: Property EnsureTightMBRs must be boolean");

                this.tightMBRs = v.booleanValue();
            }
        }
        catch (Exception e){
            e.printStackTrace();
        }

        this.infiniteRegion=IndexSuits.createRegion();
        this.infiniteRegion.makeInfinite(this.dimension);

        this.stats.treeHeight = 1;
        this.stats.getNodeNumberInLevelArray().add(0L);
        RTreeExternalNodeImpl root =new RTreeExternalNodeImpl(this, IndexSuits.createIdentifier(-1L));
        this.rootIdentifier = writeNode(root);

        storeHeader();
    }
    void initOld(PropertySet  ps){
        loadHeader();

        // only some of the properties may be changed.
        // the rest are just ignored.

        Variant v= null;

        try{
            // tree variant
            v = ps.getProperty("TreeVariant");
            if (v!=null && !v.isEmpty()){
                if (! v.isNumber())
                    throw new IllegalArgumentException("initOld: Property TreeVariant must be number");
                int rv = v.intValue();
                if(rv != RTreeVariant.RV_LINEAR.ordinal() &&
                        rv != RTreeVariant.RV_QUADRATIC.ordinal() &&
                        rv!=RTreeVariant.RV_RSTAR.ordinal())
                    throw new IllegalArgumentException("initOld: Property TreeVariant must be and of RTreeVariant type");

                this.treeVariant = RTreeVariant.values()[rv];
            }
            // near minimum overlap factor
            v = ps.getProperty("NearMinimumOverlapFactor");
            if (v!=null && !v.isEmpty()){
                if (
                        !v.isNumber() ||
                                v.intValue() < 1 ||
                                v.intValue() > this.indexCapacity ||
                                v.intValue() > this.leafCapacity)
                    throw new IllegalArgumentException("initOld: Property NearMinimumOverlapFactor must be Number and less than both index and leaf capacities");

                this.nearMinimumOverlapFactor = v.intValue();
            }
            // split distribution factor
            v = ps.getProperty("SplitDistributionFactor");
            if (v!=null && !v.isEmpty()){
                if (!v.isNumber() ||v.doubleValue() <= 0.0 || v.doubleValue() >= 1.0)
                    throw new IllegalArgumentException("initOld: Property SplitDistributionFactor must be number and in (0.0, 1.0)");

                this.splitDistributionFactor =v.doubleValue();
            }

            // reinsert factor
            v = ps.getProperty("ReinsertFactor");
            if (v!=null && !v.isEmpty()){
                if (!v.isNumber() ||v.doubleValue()<= 0.0 ||v.doubleValue() >= 1.0)
                    throw new IllegalArgumentException("initNew: Property ReinsertFactor must be number and in (0.0, 1.0)");

                this.reinsertFactor = v.doubleValue();
            }

            // tight MBRs
            v = ps.getProperty("EnsureTightMBRs");
            if (v!=null && !v.isEmpty()){
                if (v.getType()!=Variant.BOOLEAN)
                    throw new IllegalArgumentException("initOld: Property EnsureTightMBRs must be boolean");

                this.tightMBRs = v.booleanValue();
            }
        }
        catch (Exception e){
            e.printStackTrace();
        }
        if(this.infiniteRegion==null)
            this.infiniteRegion=IndexSuits.createRegion();
        this.infiniteRegion.makeInfinite(this.dimension);
    }
    void storeHeader(){
        try {
            ByteArrayOutputStream bos=new ByteArrayOutputStream(1024);
            DataOutputStream dos = new DataOutputStream(bos);
            this.rootIdentifier.write(bos);
            dos.writeInt(this.treeVariant.ordinal());
            dos.writeDouble(this.fillFactor);
            dos.writeInt(this.indexCapacity);
            dos.writeInt(this.leafCapacity);
            dos.writeInt(this.nearMinimumOverlapFactor);
            dos.writeDouble(this.splitDistributionFactor);
            dos.writeDouble(this.reinsertFactor);
            dos.writeInt(this.dimension);
            dos.writeBoolean(this.tightMBRs);
            dos.writeLong(this.stats.getNodeNumber());
            dos.writeLong(this.stats.getDataNumber());
            dos.writeLong(this.stats.getTreeHeight());
            for( Long v:this.stats.getNodeNumberInLevelArray()){
                dos.writeLong(v);
            }
            dos.close();
            byte [] data =bos.toByteArray();
            bos.close();
            this.storageManager.storeByteArray(this.headerIdentifier,data);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    void loadHeader(){
        try {
            byte []data=this.storageManager.loadByteArray(this.headerIdentifier);
            ByteArrayInputStream bis=new ByteArrayInputStream(data);
            DataInputStream dis = new DataInputStream(bis);
            this.rootIdentifier.read(bis);
            this.treeVariant=RTreeVariant.values()[dis.readInt()];
            this.fillFactor=dis.readDouble();
            this.indexCapacity=dis.readInt();
            this.leafCapacity=dis.readInt();
            this.nearMinimumOverlapFactor=dis.readInt();
            this.splitDistributionFactor=dis.readDouble();
            this.reinsertFactor=dis.readDouble();
            this.dimension=dis.readInt();
            this.tightMBRs=dis.readBoolean();
            long lval= dis.readLong();
            this.stats.setNodeNumber(lval);
            lval=dis.readLong();
            this.stats.setDataNumber(lval);
            lval=dis.readLong();
            this.stats.setTreeHeight(lval);
            for(int i=0;i<lval;++i)
                this.stats.getNodeNumberInLevelArray().add(dis.readLong());
            dis.close();
            bis.close();

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    void insertData_impl( byte[] pData, Region mbr, Identifier id){
        assert(mbr.getDimension() == this.dimension);
        Stack<Identifier> pathBuffer=new Stack();
        RTreeNodeImpl root = (RTreeNodeImpl)readNode(this.rootIdentifier);
        byte[] overflowTable = new byte[root.getLevel()];
        Arrays.fill(overflowTable,(byte)(0));
        RTreeNodeImpl l =(RTreeNodeImpl) root.chooseSubtree(mbr, 0, pathBuffer);
        l.insertData(new EntryImpl(id,mbr,pData), pathBuffer, overflowTable);
        this.stats.increaseDataNumber();
    }
    void insertData_impl(byte[] pData, Region mbr, Identifier id, int level, byte[] overflowTable){
        assert(mbr.getDimension() == this.dimension);
        insertData_impl(new EntryImpl(id,mbr,pData),level,overflowTable);
    }
    void insertData_impl(Entry e, int level, byte[] overflowTable){
        Region mbr = (Region)e.getShape();
        assert(mbr.getDimension() == this.dimension);
        Stack<Identifier> pathBuffer=new Stack<>();
        RTreeNodeImpl root = (RTreeNodeImpl) readNode(rootIdentifier);
        RTreeNodeImpl n = (RTreeNodeImpl)root.chooseSubtree(mbr, level, pathBuffer);
        assert(n.getLevel() == level);
        n.insertData(e, pathBuffer, overflowTable);
    }
    boolean deleteData_impl(Region mbr, Identifier id){
        assert(mbr.getDimension() == this.dimension);
        Stack<Identifier> pathBuffer=new Stack<>();
        RTreeNodeImpl root = (RTreeNodeImpl)readNode(this.rootIdentifier);
        RTreeNodeImpl l =(RTreeNodeImpl) root.findLeaf(mbr, id, pathBuffer);
        if (l!=null) {
            RTreeExternalNodeImpl  pL = (RTreeExternalNodeImpl) (l);
            pL.deleteData(id, pathBuffer);
            this.stats.decreaseDataNumber();
            return true;
        }
        return false;
    }

    Identifier writeNode(Node n){
        try{
            byte[] buffer=n.storeToByteArray();
            Identifier page = IndexSuits.createIdentifier(-1L);
            if(n.getIdentifier().longValue()<0)
                page.reset(StorageManager.NEW_PAGE);
            else
                page = (Identifier)n.getIdentifier().clone();

            this.storageManager.storeByteArray(page,buffer);

            if (n.getIdentifier().longValue()<0){
                n.setIdentifier(page);
                this.stats.increaseNodeNumber();
                long nn = this.stats.getNodeNumberInLevel(n.getLevel());
                nn++;
                this.stats.setNodeNumberInLevel(n.getLevel(),nn);
            }
            this.stats.increaseWriteTimes();

            for (int cIndex = 0; cIndex < this.writeNodeCommands.size(); ++cIndex){
                this.writeNodeCommands.get(cIndex).execute(n);
            }
            return page;
        }
        catch (Exception e){
            e.printStackTrace();
        }
        return null;
    }
    Node readNode(Identifier page){
        try{
            byte[] buffer=this.storageManager.loadByteArray(page);
            DataInputStream dis =new DataInputStream(new ByteArrayInputStream(buffer));
            int nodeType =dis.readInt();
            RTreeNodeImpl n =null;
            if (nodeType == 1)
                n = new RTreeInternalNodeImpl(this, new IdentifierImpl(-1L), 0);
			else
                n = new RTreeExternalNodeImpl(this, new IdentifierImpl(-1L));

            n.setIdentifier(page);
            n.loadFromByteArray(buffer);
            this.stats.increaseReadTimes();
            for (int cIndex = 0; cIndex < this.readNodeCommands.size(); ++cIndex){
                this.readNodeCommands.get(cIndex).execute(n);
            }
            return n;
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }
    void deleteNode(Node n){
        try{
            this.storageManager.deleteByteArray(n.getIdentifier());
        }
        catch (Exception  e){
            e.printStackTrace();
        }
        this.stats.decreaseNodeNumber();
        long nn=this.stats.getNodeNumberInLevel(n.getLevel());
        nn--;
        this.stats.getNodeNumberInLevelArray().set(n.getLevel(),nn);
        for (int cIndex = 0; cIndex < this.deleteNodeCommands.size(); ++cIndex){
            this.deleteNodeCommands.get(cIndex).execute(n);
        }
    }

    void range(RangeQueryType type, Shape query, Visitor v){
        Stack<Node> st=new Stack<>();
        Node root = readNode(this.rootIdentifier);

        if (root.getChildrenCount() > 0 && query.intersectsShape(root.getShape())) st.push(root);

        while (! st.empty()){
            Node n = st.pop();

            if (n.getLevel() == 0){
                v.visitNode(n);

                for (int cChild = 0; cChild < n.getChildrenCount(); ++cChild) {
                    boolean b;
                    if (type == RangeQueryType.RQT_CONTAINMENT_QUERY)
                        b = query.containsShape(n.getChildShape(cChild));
				    else
				        b = query.intersectsShape(n.getChildShape(cChild));

                    if (b){
                        v.visitData(n.getChildEntry(cChild));
                        this.stats.increaseQueryResults();
                    }
                }
            }
            else{
                v.visitNode(n);
                for (int cChild = 0; cChild < n.getChildrenCount(); ++cChild){
                    if (query.intersectsShape(n.getChildShape(cChild)))
                        st.push(readNode(n.getChildIdentifier(cChild)));
                }
            }
        }
    }
    void selfJoin(Identifier id1, Identifier id2,  Region r, Visitor vis){
        Node n1 = readNode(id1);
        Node n2 = readNode(id2);
        vis.visitNode(n1);
        vis.visitNode(n2);
        Region tr1 = null;
        Region tr2 = null;
        Entry [] ev = new Entry[2];
        Identifier ti1=null;
        Identifier ti2=null;
        for (int cChild1 = 0; cChild1 < n1.getChildrenCount(); ++cChild1) {
            tr1=(Region)n1.getChildShape(cChild1);
            ti1=n1.getChildIdentifier(cChild1);
            if (r.intersectsRegion(tr1)) {
                for (int cChild2 = 0; cChild2 < n2.getChildrenCount(); ++cChild2) {
                    tr2=(Region)n2.getChildShape(cChild2);
                    ti2=n2.getChildIdentifier(cChild2);
                    if (r.intersectsRegion(tr2) &&tr1.intersectsRegion(tr2)){
                        if (n1.getLevel() == 0){
                            if (! ti1.equals(ti2)){
                                assert(n2.getLevel() == 0);

                                ev[0]=n1.getChildEntry(cChild1);
                                ev[1]=n2.getChildEntry(cChild2);
                                vis.visitData(ev);
                            }
                        }
                        else{
                            Region rr = r.getIntersectingRegion(tr1.getIntersectingRegion(tr2));
                            selfJoin(ti1, ti2, rr, vis);
                        }
                    }
                }
            }
        }
    }
    void visitSubTree(Node  subTree, Visitor  v){
        Stack<Node> st=new Stack<>();
        st.push(subTree);
        while (! st.empty()){
            Node n =  st.pop();
            v.visitNode(n);
            if(n.getLevel() == 0){
                for (int cChild = 0; cChild < n.getChildrenCount(); ++cChild) {
                    Entry data = n.getChildEntry(cChild);
                    v.visitData(data);
                    this.stats.increaseQueryResults();
                }
            }
            else {
                for (int cChild = 0; cChild < n.getChildrenCount(); ++cChild) {
                    st.push(readNode(n.getChildIdentifier(cChild)));
                }
            }
        }
    }

    StorageManager storageManager;

    Identifier rootIdentifier;
    Identifier headerIdentifier;

    RTreeVariant treeVariant;

    double fillFactor;

    int indexCapacity;

    int leafCapacity;

    int nearMinimumOverlapFactor;
    // The R*-Tree 'p' constant, for calculating nearly minimum overlap cost.
    // [Beckmann, Kriegel, Schneider, Seeger 'The R*-tree: An efficient and Robust Access Method
    // for Points and Rectangles', Section 4.1]

    double splitDistributionFactor;
    // The R*-Tree 'm' constant, for calculating spliting distributions.
    // [Beckmann, Kriegel, Schneider, Seeger 'The R*-tree: An efficient and Robust Access Method
    // for Points and Rectangles', Section 4.2]

    double reinsertFactor;
    // The R*-Tree 'p' constant, for removing entries at reinserts.
    // [Beckmann, Kriegel, Schneider, Seeger 'The R*-tree: An efficient and Robust Access Method
    //  for Points and Rectangles', Section 4.3]

    int dimension;

    Region infiniteRegion;

    StatisticsImpl stats;

    boolean tightMBRs;

    ArrayList<Command> writeNodeCommands;
    ArrayList<Command> readNodeCommands;
    ArrayList<Command> deleteNodeCommands;


}
