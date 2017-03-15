package test.gtl.index.rtree.impl;

import gtl.common.CommonSuits;
import gtl.math.geometry.GeometrySuits;
import gtl.index.Entry;
import gtl.math.geometry.Envelope;
import gtl.common.Identifier;
import gtl.index.IndexSuits;
import gtl.index.impl.EntryImpl;
import gtl.index.rtree.RTree;
import gtl.index.rtree.RTreeVariant;
import gtl.index.rtree.impl.RTreeExternalNodeImpl;
import gtl.index.rtree.impl.RTreeImpl;
import gtl.shape.ShapeSuits;
import gtl.storage.StorageManager;
import gtl.storage.StorageSuits;
import junit.framework.TestCase;

import java.util.Arrays;

/**
 * Created by ZhenwenHe on 2017/3/3.
 */
public class RTreeExternalNodeImplTest extends TestCase {
    public void setUp() throws Exception {
        super.setUp();

    }

    public void tearDown() throws Exception {

    }

    public void testClone() throws Exception {
        Envelope[] envelopes= GeometrySuits.readEnvelopeFile(IndexSuits.DATA_DIR+"test2d100.envelopes");
        StorageManager sm = StorageSuits.createDiskStorageManager(IndexSuits.DATA_DIR+"rtree",32,true);
        RTree rtree= IndexSuits.createRTree(sm,2,4,4, RTreeVariant.RV_RSTAR);
        RTreeExternalNodeImpl leafNode = new RTreeExternalNodeImpl((RTreeImpl) rtree,CommonSuits.createIdentifier(-1L));

        for(int i=0;i<4;i++) {
            Entry e = new EntryImpl(CommonSuits.createIdentifier(-1L),
                    ShapeSuits.createRegion(envelopes[i].getLowCoordinates(), envelopes[i].getHighCoordinates()),
                    envelopes[i].storeToByteArray());
            leafNode.insertEntry(e);
        }

        byte[] bs = leafNode.storeToByteArray();

        RTreeExternalNodeImpl leafNode2 = new RTreeExternalNodeImpl((RTreeImpl) rtree,CommonSuits.createIdentifier(2L));
        leafNode2.loadFromByteArray(bs);

        assertTrue(leafNode.equals(leafNode2));
        Identifier page = CommonSuits.createIdentifier(StorageManager.NEW_PAGE);
        sm.storeByteArray(page,leafNode2.storeToByteArray());
        leafNode2.setIdentifier(page);
        byte [] bs2=sm.loadByteArray(page);
        RTreeExternalNodeImpl leafNode3 = new RTreeExternalNodeImpl((RTreeImpl) rtree,CommonSuits.createIdentifier(3L));
        leafNode3.loadFromByteArray(bs2);
        assertTrue(leafNode.equals(leafNode3));
        assertTrue(Arrays.equals(bs,bs2));
    }

    public void testChooseSubtree() throws Exception {

    }

    public void testFindLeaf() throws Exception {

    }

    public void testSplit() throws Exception {

    }

    public void testDeleteData() throws Exception {

    }

}