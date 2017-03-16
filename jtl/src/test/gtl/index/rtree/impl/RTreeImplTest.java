package test.gtl.index.rtree.impl;

import gtl.common.CommonSuits;
import gtl.geom.Envelope;
import gtl.common.Identifier;
import gtl.geom.Geom3DSuits;
import gtl.index.IndexSuits;
import gtl.index.rtree.RTree;
import gtl.index.rtree.RTreeVariant;
import gtl.shape.Region;
import gtl.shape.ShapeSuits;
import gtl.io.storage.StorageManager;
import gtl.io.storage.StorageSuits;
import junit.framework.TestCase;

/**
 * Created by ZhenwenHe on 2017/2/24.
 */
public class RTreeImplTest extends TestCase {
    public void setUp() throws Exception {
        super.setUp();


    }

    public void tearDown() throws Exception {

    }

    public void testReset() throws Exception {

    }

    public void testReset1() throws Exception {

    }

    /**
     * // i==5出错，EnvelopeImpl.load(DataInput dis) 函数的dims数据读取出错，原因需要查找
     * //可能的原因：
     * 1.Identifier的读写出错
     * 2.EntryImpl的读写出错
     * 3.Envelope2D 的读写出错
     * 4.RegionImpl的读写出错
     * 5.NodeImpl
     * 6.RTreeNodeImpl
     * 7.RTreeImpl
     * 8.StorageManager
     * @throws Exception
     */
    public void testInsert() throws Exception {
        Envelope[] envelopes= Geom3DSuits.readEnvelopeFile(IndexSuits.DATA_DIR+"test2d100.envelopes");
        StorageManager sm = StorageSuits.createDiskStorageManager(IndexSuits.DATA_DIR+"rtree",32,true);
        RTree rtree= IndexSuits.createRTree(sm,2,4,4, RTreeVariant.RV_RSTAR);

        for(int i=0;i<envelopes.length;++i){
            Region r = ShapeSuits.createRegion(envelopes[i].getLowCoordinates(),envelopes[i].getHighCoordinates());
            byte [] data=r.storeToByteArray();
            Identifier id = CommonSuits.createIdentifier(i);
            rtree.insert(data,r,id);
        }
    }

    public void testDelete() throws Exception {

    }

    public void testContains() throws Exception {

    }

    public void testIntersects() throws Exception {

    }

    public void testPointLocation() throws Exception {

    }

    public void testNearestNeighbor() throws Exception {

    }

    public void testNearestNeighbor1() throws Exception {

    }

    public void testSelfJoin() throws Exception {

    }

    public void testQueryStrategy() throws Exception {

    }

    public void testGetProperties() throws Exception {

    }

    public void testAddCommand() throws Exception {

    }

    public void testIsValid() throws Exception {

    }

    public void testGetStatistics() throws Exception {

    }

    public void testGetDimension() throws Exception {

    }

    public void testInitNew() throws Exception {

    }

    public void testInitOld() throws Exception {

    }

    public void testStoreHeader() throws Exception {

    }

    public void testLoadHeader() throws Exception {

    }

    public void testInsertData_impl() throws Exception {

    }

    public void testInsertData_impl1() throws Exception {

    }

    public void testInsertData_impl2() throws Exception {

    }

    public void testDeleteData_impl() throws Exception {

    }

    public void testWriteNode() throws Exception {

    }

    public void testReadNode() throws Exception {

    }

    public void testDeleteNode() throws Exception {

    }

    public void testRange() throws Exception {

    }

    public void testSelfJoin1() throws Exception {

    }

    public void testVisitSubTree() throws Exception {

    }

}