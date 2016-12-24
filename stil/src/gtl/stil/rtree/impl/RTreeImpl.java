package gtl.stil.rtree.impl;

import gtl.stil.*;
import gtl.stil.rtree.RTree;
import gtl.stil.rtree.RTreeVariant;
import gtl.stil.rtree.RangeQueryType;
import gtl.stil.shape.Point;
import gtl.stil.shape.Region;
import gtl.stil.shape.Shape;
import gtl.stil.storage.StorageManager;

/**
 * Created by ZhenwenHe on 2016/12/19.
 */
public class RTreeImpl implements RTree{
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
