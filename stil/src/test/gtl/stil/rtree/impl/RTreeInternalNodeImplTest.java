package test.gtl.stil.rtree.impl;

import gtl.stil.Entry;
import gtl.math.geometry.Envelope;
import gtl.common.Identifier;
import gtl.stil.IndexSuits;
import gtl.stil.impl.EntryImpl;
import gtl.stil.rtree.RTree;
import gtl.stil.rtree.RTreeVariant;
import gtl.stil.rtree.impl.RTreeImpl;
import gtl.stil.rtree.impl.RTreeInternalNodeImpl;
import gtl.storage.StorageManager;
import junit.framework.TestCase;

import java.util.Arrays;

/**
 * Created by ZhenwenHe on 2017/3/3.
 */
public class RTreeInternalNodeImplTest extends TestCase {
    public void setUp() throws Exception {
        super.setUp();

    }

    public void tearDown() throws Exception {

    }

    public void testClone() throws Exception {
        Envelope[] envelopes= IndexSuits.readEnvelopeFile(IndexSuits.DATA_DIR+"test2d100.envelopes");
        StorageManager sm = IndexSuits.createDiskStorageManager(IndexSuits.DATA_DIR+"rtree",32,true);
        RTree rtree= IndexSuits.createRTree(sm,2,4,4, RTreeVariant.RV_RSTAR);
        RTreeInternalNodeImpl leafNode = new RTreeInternalNodeImpl((RTreeImpl) rtree,IndexSuits.createIdentifier(-1L),1);

        for(int i=0;i<4;i++) {
            Entry e = new EntryImpl(IndexSuits.createIdentifier(-1L),
                    IndexSuits.createRegion(envelopes[i].getLowCoordinates(), envelopes[i].getHighCoordinates()),
                    envelopes[i].storeToByteArray());
            leafNode.insertEntry(e);
        }

        byte[] bs = leafNode.storeToByteArray();

        RTreeInternalNodeImpl leafNode2 = new RTreeInternalNodeImpl((RTreeImpl) rtree,IndexSuits.createIdentifier(2L),1);
        leafNode2.loadFromByteArray(bs);

        assertTrue(leafNode.equals(leafNode2));
        Identifier page = IndexSuits.createIdentifier(StorageManager.NEW_PAGE);
        sm.storeByteArray(page,leafNode2.storeToByteArray());
        leafNode2.setIdentifier(page);
        byte [] bs2=sm.loadByteArray(page);
        RTreeInternalNodeImpl leafNode3 = new RTreeInternalNodeImpl((RTreeImpl) rtree,IndexSuits.createIdentifier(3L),1);
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

    public void testAdjustTree() throws Exception {

    }

    public void testAdjustTree1() throws Exception {

    }

    public void testFindLeastEnlargement() throws Exception {

    }

    public void testFindLeastOverlap() throws Exception {

    }

}