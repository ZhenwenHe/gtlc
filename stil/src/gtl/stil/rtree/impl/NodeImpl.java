package gtl.stil.rtree.impl;

import gtl.stil.Identifier;
import gtl.stil.IndexSuits;
import gtl.stil.Node;
import gtl.stil.shape.Region;
import gtl.stil.shape.Shape;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Stack;

/**
 * Created by ZhenwenHe on 2016/12/19.
 */
public abstract  class NodeImpl implements Node {

    public NodeImpl(RTreeImpl m_pTree,Identifier id, int level, int capacity) {
        this.m_pTree = m_pTree;
        this.m_identifier= IndexSuits.createIdentifier(id.longValue());
        this.m_level=level;
        this.m_capacity=capacity;

        this.m_children=0;
        this.m_pData =null;
        this.m_ptrMBR=null;
        this.m_pIdentifier=null;
        this.m_totalDataLength=0;

        this.m_nodeMBR.makeInfinite(this.m_pTree.getDimension());
        this.m_nodeMBR.makeInfinite(this.m_pTree.getDimension());
        this.m_pData = new byte[this.m_capacity + 1][];
        m_ptrMBR = IndexSuits.createRegionArray(m_capacity + 1);
        m_pIdentifier = IndexSuits.createIdentifierArray(m_capacity + 1);
    }

    public NodeImpl() {
        this.m_pTree = null;
        this.m_identifier= null;
        this.m_level=0;
        this.m_capacity=0;

        this.m_children=0;
        this.m_pData =null;
        this.m_ptrMBR=null;
        this.m_pIdentifier=null;
        this.m_totalDataLength=0;
    }

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


    @Override
    public int getChildrenCount() {
        return m_children;
    }

    @Override
    public Identifier getIdentifier() {
        return m_identifier;
    }

    @Override
    public Object clone() {
        return null;
    }

    @Override
    public Identifier getChildIdentifier(int index) {
        if (index <= m_children)
            return m_pIdentifier[index];
        else
            return null;
    }

    @Override
    public Shape getShape() {
        return m_nodeMBR;
    }

    @Override
    public void copyFrom(Object i) {

    }

    @Override
    public void copyTo(Object i) {

    }

    @Override
    public byte[] getChildData(int index) {
        if (index <= m_children) {
            return  m_pData[index];
        }
        else
            return null;
    }

    @Override
    public boolean read(InputStream in) throws IOException {
        return false;
    }

    @Override
    public Shape getChildShape(int index) {
        if (index <= m_children)
            return m_ptrMBR[index];
        else
            return null;
    }

    @Override
    public int getLevel() {
        return m_level;
    }

    @Override
    public boolean write(OutputStream out) throws IOException {
        return false;
    }

    @Override
    public boolean isIndex() {
        return (m_level != 0);
    }

    @Override
    public boolean isLeaf() {
        return (m_level == 0);
    }

    @Override
    public long getByteArraySize() {
        return 0;
    }

    void  insertEntry(byte[] pData, Region mbr, Identifier id)
    {
        assert(m_children < m_capacity);
        m_pData[m_children] = pData;
        m_ptrMBR[m_children] = mbr;
        m_pIdentifier[m_children] = id;

        m_totalDataLength += pData.length;
        ++m_children;

        m_nodeMBR.combineRegion(mbr);
    }

    void deleteEntry(int index) {
        assert(index >= 0 && index < m_children);

        // cache it, since I might need it for "touches" later.
        Region ptrR = m_ptrMBR[index];

        m_totalDataLength -= m_pData[index].length;

        if (m_children > 1 && index != m_children - 1){
            m_pData[index] = m_pData[m_children - 1];
            m_ptrMBR[index] = m_ptrMBR[m_children - 1];
            m_pIdentifier[index] = m_pIdentifier[m_children - 1];
        }

        --m_children;

        // WARNING: index has now changed. Do not use it below here.

        if (m_children == 0){
            m_nodeMBR.makeInfinite(m_pTree.getDimension());
        }
        else if (m_pTree.m_bTightMBRs && m_nodeMBR.touchesRegion(ptrR)){
            for (int cDim = 0; cDim < m_nodeMBR.getDimension(); ++cDim){
                //m_nodeMBR.m_pLow[cDim] = Double.MAX_VALUE;
                this.m_nodeMBR.setLowCoordinate(cDim,Double.MAX_VALUE);
                //m_nodeMBR.m_pHigh[cDim] = -Double.MAX_VALUE;
                this.m_nodeMBR.setHighCoordinate(cDim,-Double.MAX_VALUE);
                for (int u32Child = 0; u32Child < m_children; ++u32Child){
                    //m_nodeMBR.m_pLow[cDim] = std::min(m_nodeMBR.m_pLow[cDim], m_ptrMBR[u32Child].m_pLow[cDim]);
                    this.m_nodeMBR.setLowCoordinate(cDim,Math.min(this.m_nodeMBR.getLowCoordinate(cDim),m_ptrMBR[u32Child].getLowCoordinate(cDim)));
                    //m_nodeMBR.m_pHigh[cDim] = std::max(m_nodeMBR.m_pHigh[cDim], m_ptrMBR[u32Child].m_pHigh[cDim]);
                    this.m_nodeMBR.setHighCoordinate(cDim,Math.max(this.m_nodeMBR.getHighCoordinate(cDim),m_ptrMBR[u32Child].getHighCoordinate(cDim)));
                }
            }
        }
    }

    boolean insertData(byte[] pData, Region mbr, Identifier id, Stack<Identifier> pathBuffer, byte[] overflowTable) {
        return false;
    }


    void reinsertData(byte[] pData, Region mbr, Identifier id, ArrayList<Integer> reinsert, ArrayList<Integer> keep){

    }

    void rtreeSplit(byte[] pData, Region mbr, Identifier id, ArrayList<Integer> group1, ArrayList<Integer> group2){

    }

    void rstarSplit(byte[] pData, Region mbr, Identifier id, ArrayList<Integer> group1, ArrayList<Integer> group2){

    }

    //为了能将参数传递进去，并将修改值带出来，采用数组实现
    void pickSeeds(int []indexes){
        int index1=indexes[0];
        int index2=indexes[1];


        //计算完后将得到的值传入数组带回
        indexes[0]=index1;
        indexes[1]=index2;
    }

    void condenseTree(Stack<Node> toReinsert, Stack<Identifier> pathBuffer, Node  ptrThis){

    }

    abstract Node chooseSubtree( Region mbr, int level, Stack<Identifier> pathBuffer);

    abstract Node findLeaf(Region mbr, Identifier id, Stack<Identifier> pathBuffer);

    abstract void split( byte[] pData, Region mbr, Identifier id, Node left, Node right) ;

    class RstarSplitEntry {
        Region region;
        int index;
        int sortDim;


        RstarSplitEntry(Region  v_region, int v_index, int v_dimension){
            region =v_region;
            index =v_index;
            sortDim=v_dimension;
        }
    }; // RstarSplitEntry
    class ReinsertEntry {
        int index;
        double dist;

        ReinsertEntry(int v_index, double v_dist) {
            index =v_index;
            dist =v_dist;
        }
    };
    static int compareReinsertEntry(ReinsertEntry pe1, ReinsertEntry pe2){
        if (pe1.dist < pe2.dist) return -1;
        if (pe1.dist > pe2.dist) return 1;
        return 0;
    }

    static int compareLow(RstarSplitEntry pe1, RstarSplitEntry pe2){ 

        assert(pe1.sortDim == pe2.sortDim);

        if (pe1.region.getLowCoordinate(pe1.sortDim) < pe2.region.getLowCoordinate(pe2.sortDim)) return -1;
        if (pe1.region.getLowCoordinate(pe1.sortDim) > pe2.region.getLowCoordinate(pe2.sortDim)) return 1;
        return 0;
    }

    static int compareHigh(RstarSplitEntry pe1, RstarSplitEntry pe2){
       
        assert(pe1.sortDim == pe2.sortDim);

        if (pe1.region.getHighCoordinate(pe1.sortDim) < pe2.region.getHighCoordinate(pe2.sortDim)) return -1;
        if (pe1.region.getHighCoordinate(pe1.sortDim) > pe2.region.getHighCoordinate(pe2.sortDim)) return 1;
        return 0;
    }
}
