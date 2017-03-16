package test.gtl.geom;

import junit.framework.TestCase;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;

/**
 * Created by hadoop on 17-3-24.
 */
public class GeometryCollectionTest extends TestCase {
    public void setUp() throws Exception {
        super.setUp();

    }

    public void tearDown() throws Exception {

    }

    public void testSize() throws Exception {

    }

    public void testIsEmpty() throws Exception {

    }

    public void testContains() throws Exception {

    }

    public void testIterator() throws Exception {

    }

    public void testToArray() throws Exception {

    }

    public void testToArray1() throws Exception {

    }

    public void testAdd() throws Exception {

    }

    public void testRemove() throws Exception {

    }

    public void testContainsAll() throws Exception {

    }

    public void testAddAll() throws Exception {

    }

    public void testRemoveAll() throws Exception {

    }

    public void testRetainAll() throws Exception {

    }

    public void testClear() throws Exception {

    }

    public void testCopyFrom() throws Exception {

    }

    public void testLoad() throws Exception {
        ByteArrayOutputStream baos= new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(baos);
        dos.writeInt(1098);
        dos.writeInt(1099);
        byte [] bs = baos.toByteArray();
        assertTrue(bs.length==8);
        DataInputStream dis = new DataInputStream(new ByteArrayInputStream(bs));
        int v1=dis.readInt();
        assertTrue(v1==1098);

        int v2=dis.readInt();
        assertTrue(v2==1099);
    }

    public void testStore() throws Exception {

    }

    public void testGetByteArraySize() throws Exception {

    }

    public void testClone() throws Exception {

    }

}