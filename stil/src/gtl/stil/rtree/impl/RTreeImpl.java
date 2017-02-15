package gtl.stil.rtree.impl;

import gtl.stil.*;
import gtl.stil.rtree.RTree;
import gtl.stil.rtree.RTreeVariant;
import gtl.stil.rtree.RangeQueryType;
import gtl.stil.shape.Point;
import gtl.stil.shape.Region;
import gtl.stil.shape.Shape;
import gtl.stil.storage.StorageManager;

import java.util.ArrayList;

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
        this.stats =new StatisticsImpl();


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
        this.stats =new StatisticsImpl();

        if(indexIdentifier==null){//new

        }
        else {//old

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

    void range(RangeQueryType type, Shape query, Visitor v){

    }
    void selfJoin(Identifier id1, Identifier id2,  Region r, Visitor vis){

    }
    void visitSubTree(Node  subTree, Visitor  v){

    }
}
