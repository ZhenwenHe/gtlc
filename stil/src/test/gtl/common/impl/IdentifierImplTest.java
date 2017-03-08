package test.gtl.common.impl;

import gtl.common.Identifier;
import gtl.stil.IndexSuits;
import junit.framework.TestCase;

/**
 * Created by ZhenwenHe on 2016/12/14.
 */
public class IdentifierImplTest extends TestCase {
    public void setUp() throws Exception {
        super.setUp();

    }

    public void tearDown() throws Exception {

    }

    public void testLongValue() throws Exception {

    }

    public void testIntValue() throws Exception {

    }

    public void testClone() throws Exception {

    }

    public void testRead() throws Exception {

    }

    public void testWrite() throws Exception {
        Identifier i1= IndexSuits.createIdentifier(1L);
        Identifier i2=IndexSuits.createIdentifier(2L);
        byte[] dat= i1.storeToByteArray();
        i2.loadFromByteArray(dat);
        assertTrue(i1.equals(i2)  && i2.intValue()==1);
    }

    public void testGetByteArraySize() throws Exception {

    }

    public void testToString() throws Exception {

    }

    public void testEquals() throws Exception {

    }

    public void testHashCode() throws Exception {

    }

    public void testReset() throws Exception {

    }

    public void testCopyFrom() throws Exception {

    }

    public void testCopyTo() throws Exception {

    }

    public void testIncrease() throws Exception {

    }

    public void testByteValue() throws Exception {

    }

    public void testShortValue() throws Exception {

    }

    public void testCompare() throws Exception {

    }

}