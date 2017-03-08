package test.gtl.stil.impl;

import gtl.stil.Entry;
import gtl.math.geometry.Envelope;
import gtl.stil.IndexSuits;
import gtl.stil.impl.EntryImpl;
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
        Envelope[] envs= IndexSuits.readEnvelopeFile(IndexSuits.DATA_DIR+"test2d100.envelopes");
        Entry[] entries= new Entry[4];
        Entry[] entries2= new Entry[4];
        for(int i=0;i<4;i++){
            entries[i]=new EntryImpl(IndexSuits.createIdentifier(-1L),
                    IndexSuits.createRegion(envs[i].getLowCoordinates(),envs[i].getHighCoordinates()),
                    envs[i].storeToByteArray());

            entries2[i]=new EntryImpl(IndexSuits.createIdentifier(10L),
                    IndexSuits.createRegion(envs[i+50].getLowCoordinates(),envs[i+50].getHighCoordinates()),
                    envs[i+50].storeToByteArray());

            entries2[i].loadFromByteArray(entries[i].storeToByteArray());

            Entry e = entries[i];
            Entry e2=entries2[i];
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