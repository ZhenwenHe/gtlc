package test.gtl.index.impl;

import gtl.common.CommonSuits;
import gtl.math.geometry.GeometrySuits;
import gtl.index.Entry;
import gtl.math.geometry.Envelope;
import gtl.index.IndexSuits;
import gtl.index.impl.EntryImpl;
import gtl.shape.ShapeSuits;
import junit.framework.TestCase;

/**
 * Created by ZhenwenHe on 2017/3/3.
 */
public class EntryImplTest extends TestCase {
    public void setUp() throws Exception {
        super.setUp();

    }

    public void tearDown() throws Exception {

    }

    public void testGetIdentifier() throws Exception {

    }

    public void testClone() throws Exception {

    }

    public void testGetShape() throws Exception {

    }

    public void testCopyFrom() throws Exception {

    }

    public void testSetIdentifier() throws Exception {

    }

    public void testSetIdentifier1() throws Exception {

    }

    public void testSetShape() throws Exception {

    }

    public void testGetData() throws Exception {

    }

    public void testSetData() throws Exception {

    }

    public void testLoad() throws Exception {

    }

    public void testStore() throws Exception {

    }

    public void testGetByteArraySize() throws Exception {
        Envelope[] envs= GeometrySuits.readEnvelopeFile(IndexSuits.DATA_DIR+"test2d100.envelopes");
        Entry[] entries= new Entry[4];
        Entry[] entries2= new Entry[4];
        for(int i=0;i<4;i++){
            entries[i]=new EntryImpl(CommonSuits.createIdentifier(-1L),
                    ShapeSuits.createRegion(envs[i].getLowCoordinates(),envs[i].getHighCoordinates()),
                    envs[i].storeToByteArray());

            entries2[i]=new EntryImpl(CommonSuits.createIdentifier(10L),
                    ShapeSuits.createRegion(envs[i+50].getLowCoordinates(),envs[i+50].getHighCoordinates()),
                    envs[i+50].storeToByteArray());

            entries2[i].loadFromByteArray(entries[i].storeToByteArray());

            Entry e = entries[i];
            Entry e2=entries2[i];
            System.out.println(e);
            System.out.println(e2);
            assertTrue(e.equals(e2));
        }
    }

    public void testGetDataLength() throws Exception {

    }

    public void testEquals() throws Exception {

    }

    public void testHashCode() throws Exception {

    }

    public void testToString() throws Exception {

    }

}