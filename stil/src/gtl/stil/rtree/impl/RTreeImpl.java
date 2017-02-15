package gtl.stil.rtree.impl;

import gtl.stil.*;
import gtl.stil.rtree.RTree;
import gtl.stil.rtree.RTreeVariant;
import gtl.stil.rtree.RangeQueryType;
import gtl.stil.shape.Point;
import gtl.stil.shape.Region;
import gtl.stil.shape.Shape;
import gtl.stil.storage.StorageManager;

import java.util.Map;

/**
 * Created by ZhenwenHe on 2016/12/19.
 */
public class RTreeImpl implements RTree{

    public RTreeImpl() {

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
        this.m_pStorageManager=storageManager;
        this.m_rootID=IndexSuits.createIdentifier(StorageManager.NEW_PAGE);
        this.m_headerID=IndexSuits.createIdentifier(StorageManager.NEW_PAGE);
        this.m_treeVariant=RTreeVariant.RV_RSTAR;
        this.m_fillFactor=0.7;
        this.m_indexCapacity=100;
        this.m_leafCapacity=100;
        this.m_nearMinimumOverlapFactor=32;
        this.m_splitDistributionFactor=(0.4);
        this.m_reinsertFactor=(0.3);
        this.m_dimension=(2);
        this.m_bTightMBRs=(true);
        this.m_infiniteRegion=IndexSuits.createRegion();
        this.m_stats=new StatisticsImpl();


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

        this.m_pStorageManager=storageManager;
        this.m_rootID=IndexSuits.createIdentifier(StorageManager.NEW_PAGE);
        this.m_headerID=IndexSuits.createIdentifier(StorageManager.NEW_PAGE);
        this.m_treeVariant=RTreeVariant.RV_RSTAR;
        this.m_fillFactor=0.7;
        this.m_indexCapacity=100;
        this.m_leafCapacity=100;
        this.m_nearMinimumOverlapFactor=32;
        this.m_splitDistributionFactor=(0.4);
        this.m_reinsertFactor=(0.3);
        this.m_dimension=(2);
        this.m_bTightMBRs=(true);
        this.m_infiniteRegion=IndexSuits.createRegion();
        this.m_stats=new StatisticsImpl();

        if(indexIdentifier==null){//new

        }
        else {//old

        }
    }

    StorageManager m_pStorageManager;

    Identifier m_rootID, m_headerID;

    RTreeVariant m_treeVariant;

    double m_fillFactor;

    int m_indexCapacity;

    int m_leafCapacity;

    int m_nearMinimumOverlapFactor;
    // The R*-Tree 'p' constant, for calculating nearly minimum overlap cost.
    // [Beckmann, Kriegel, Schneider, Seeger 'The R*-tree: An efficient and Robust Access Method
    // for Points and Rectangles', Section 4.1]

    double m_splitDistributionFactor;
    // The R*-Tree 'm' constant, for calculating spliting distributions.
    // [Beckmann, Kriegel, Schneider, Seeger 'The R*-tree: An efficient and Robust Access Method
    // for Points and Rectangles', Section 4.2]

    double m_reinsertFactor;
    // The R*-Tree 'p' constant, for removing entries at reinserts.
    // [Beckmann, Kriegel, Schneider, Seeger 'The R*-tree: An efficient and Robust Access Method
    //  for Points and Rectangles', Section 4.3]

    int m_dimension;

    Region m_infiniteRegion;

    StatisticsImpl m_stats;

    boolean  m_bTightMBRs;

    @Override
    public void insert(byte[] pData, Shape shape, Identifier shapeIdentifier) {

    }

    @Override
    public boolean delete(Shape shape, Identifier shapeIdentifier) {
        return false;
    }

    @Override
    public void contains(Shape query, Visitor v) {

    }

    @Override
    public void intersects(Shape query, Visitor v) {

    }

    @Override
    public void pointLocation(Point query, Visitor v) {

    }

    @Override
    public void nearestNeighbor(int k, Shape query, Visitor v, NearestNeighborComparator nnc) {

    }

    @Override
    public void nearestNeighbor(int k, Shape query, Visitor v) {

    }

    @Override
    public void selfJoin(Shape s, Visitor v) {

    }

    @Override
    public void queryStrategy(QueryStrategy qs) {

    }

    @Override
    public PropertySet getProperties() {
        return null;
    }

    @Override
    public void addCommand(Command in, CommandType ct) {

    }

    @Override
    public boolean isValid() {
        return false;
    }

    @Override
    public Statistics getStatistics() {
        return null;
    }

    public int getDimension(){
        return 0;
    }

    void initNew(PropertySet ps){

    }
    void initOld(PropertySet  ps){

    }
    void storeHeader(){

    }
    void loadHeader(){

    }

    void insertData_impl( byte[] pData, Region mbr, Identifier id){

    }
    void insertData_impl(byte[] pData, Region mbr, Identifier id, int level, byte[] overflowTable){

    }
    void insertData_impl(Entry e, int level, byte[] overflowTable){

    }
    boolean deleteData_impl(Region mbr, Identifier id){
        return false;
    }

    Identifier writeNode(Node n){
        return null;
    }
    Node readNode(Identifier page){
        return null;
    }
    void deleteNode(Node n){

    }

    void rangeQuery(RangeQueryType type, Shape query, Visitor v){

    }
    void selfJoinQuery(Identifier id1, Identifier id2,  Region r, Visitor vis){

    }
    void visitSubTree(Node  subTree, Visitor  v){

    }
}
