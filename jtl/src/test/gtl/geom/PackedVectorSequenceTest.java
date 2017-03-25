package test.gtl.geom;

import gtl.geom.PackedVectorSequence;
import gtl.geom.Vector3D;
import junit.framework.TestCase;

import java.util.ArrayList;

/**
 * Created by hadoop on 17-3-25.
 */
public class PackedVectorSequenceTest extends TestCase {
    PackedVectorSequence pvs = null;
    int dim=3;
    int size=3;
    ArrayList<Vector3D> v3ds=null;
    public PackedVectorSequenceTest() {
        v3ds=new ArrayList<>(size);
        double  [] temp = new double[size*dim];
        for(int i=0;i<temp.length;++i)
            temp[i]=i;
        for (int s=0;s<size;s++){
            Vector3D v3d=new Vector3D(s*dim,s*dim+1,s*dim+2);
            v3ds.add(v3d);
        }
        this.pvs = new PackedVectorSequence(dim,temp);
        temp=null;
    }

    public void setUp() throws Exception {
        super.setUp();

    }

    public void testClone() throws Exception {

    }

    public void testCopyFrom() throws Exception {

    }

    public void testLoad() throws Exception {

    }

    public void testStore() throws Exception {

    }

    public void testGetByteArraySize() throws Exception {
        assertTrue(dim*size*8+4+4==pvs.getByteArraySize());
    }

    public void testSize() throws Exception {
        assertTrue(this.size==this.pvs.size());
    }

    public void testIsEmpty() throws Exception {
        assertTrue(this.pvs.isEmpty()==false);
    }

    public void testContains() throws Exception {
        Vector3D v3d = new Vector3D(0,1,2);
        assertTrue(this.pvs.contains(v3d)==true);
    }

    public void testIterator() throws Exception {

    }

    public void testToArray() throws Exception {

    }

    public void testToArray1() throws Exception {

    }

    public void testAdd() throws Exception {

    }

    public void testFind() throws Exception {
        assertTrue(pvs.find(v3ds.get(0))==0);
        assertTrue(pvs.find(v3ds.get(1))==1);
        assertTrue(pvs.find(v3ds.get(2))==2);
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

    public void testGetDimension() throws Exception {

    }

    public void testGetOrdinate() throws Exception {

    }

    public void testGetX() throws Exception {

    }

    public void testGetY() throws Exception {

    }

    public void testGetZ() throws Exception {

    }

    public void testSetX() throws Exception {

    }

    public void testSetY() throws Exception {

    }

    public void testSetZ() throws Exception {

    }

    public void testGetVector3D() throws Exception {

    }

    public void testGetVector2D() throws Exception {

    }

    public void testGetVector4D() throws Exception {

    }

    public void testGetVector() throws Exception {

    }

    public void testGetVector1() throws Exception {

    }

    public void testEquals() throws Exception {

    }

    public void testHashCode() throws Exception {

    }

    public void testToString() throws Exception {

    }

}