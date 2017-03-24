package test.gtl.geom;

import gtl.geom.Geom3DSuits;
import gtl.geom.Vector;
import gtl.geom.Vector2D;
import gtl.geom.Vector3D;
import junit.framework.Assert;
import junit.framework.TestCase;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.util.ArrayList;

/**
 * Created by ZhenwenHe on 2016/12/15.
 */
public class VectorImplTest extends TestCase {
    public void setUp() throws Exception {
        super.setUp();

        System.out.println("setUp");
        ArrayList<Long> a= new ArrayList<Long>();
        a.add(Long.valueOf(1));
        a.add(Long.valueOf(2));
        a.add(Long.valueOf(3));
        for( Long i: a)
            System.out.println(i);
        ArrayList<Long> b= new ArrayList<Long>();

        b.addAll(a);

        for( Long i: a)
            System.out.println(i);

        for( Long i: b)
            System.out.println(i);

        System.out.println("setUp");
    }

    public void tearDown() throws Exception {

    }

    public void testGetCoordinates() throws Exception {

    }

    public void testSetCoordinates() throws Exception {

    }

    public void testClone() throws Exception {

    }

    public void testEquals() throws Exception {

    }

    public void testHashCode() throws Exception {

    }

    public void testToString() throws Exception {

    }

    public void testGetDimension() throws Exception {

    }

    public void testGetX() throws Exception {
        Vector v = Geom3DSuits.createVector();
        v.setX(10);
        v.setY(10);
        v.setZ(10);
        Double x= v.getX();
        x=20.0;
        Assert.assertEquals(v.getX(),10.0);
    }

    public void testGetY() throws Exception {

    }

    public void testGetZ() throws Exception {

    }

    public void testRead() throws Exception {
        Vector[] vv=new Vector[2];
        vv[0]= Geom3DSuits.createVector();
        vv[1]= Geom3DSuits.createVector();
        for(Vector v:vv)
            System.out.println(v.toString());
    }

    public void testWrite() throws Exception {
        ByteArrayOutputStream baos = new ByteArrayOutputStream(1024);
        DataOutputStream dos = new DataOutputStream(baos);
        dos.writeInt(987654);
        Vector v = Geom3DSuits.createVector(1.0,1.0);
        v.write(baos);
        v = Geom3DSuits.createVector(2.0,2.0);
        v.write(baos);
        v = Geom3DSuits.createVector(3.0,3.0);
        v.write(baos);
        dos.writeInt(456789);
        dos.close();
        byte[] data = baos.toByteArray();
        ByteArrayInputStream bios = new ByteArrayInputStream(data);
        DataInputStream dis =new DataInputStream(bios);
        System.out.println(dis.readInt());
        Vector3D v1 = Geom3DSuits.createVector(9.0,9.0,9.0);
        v1.read(bios);
        System.out.println(v1);
        v1 = Geom3DSuits.createVector(9.0,9.0,9.0);
        v1.read(bios);
        System.out.println(v1);
        v1 = Geom3DSuits.createVector(9.0,9.0,9.0);
        v1.read(bios);
        System.out.println(v1);
        System.out.println(dis.readInt());
        dis.close();
    }

    public void testGetByteArraySize() throws Exception {
        Vector2D v1= Geom3DSuits.createVector(1.0,2.0);
        Vector3D v2= Geom3DSuits.createVector(0.0,1.0,1.0);

        byte[] bs= v1.storeToByteArray();
        v2.loadFromByteArray(bs);
        assertTrue(v2.equals(v1));
    }

    public void testSetX() throws Exception {
        int [] iv = new int[3];
        iv[0]=11;
        iv[1]=21;
        iv[2]=31;
        for(int v:iv){
            v=6;
        }
        for(int v:iv){
            System.out.println(v);
        }
        Vector3D[] vv= new Vector3D[3];
        vv[0]= Geom3DSuits.createVector(1,2,3);
        vv[1]= Geom3DSuits.createVector(1,2,3);
        vv[2]= Geom3DSuits.createVector(1,2,3);



        for(Vector3D v:vv){//改变本身是不起作用的
            v= Geom3DSuits.createVector(1.0,7.0,9.0);
        }

        for(Vector3D v:vv){
            assertTrue(v.getX()==1.0);
            assertTrue(v.getY()==2.0);
            assertTrue(v.getZ()==3.0);
            System.out.println(v);
        }

        for(Vector3D v:vv){//改变指向的内容是可以的
            v.setY(89);
            v.setX(98);
            v.setZ(198);
        }

        for(Vector3D v:vv){
            System.out.println(v);
            assertTrue(v.getX()==98.0);
            assertTrue(v.getY()==89.0);
            assertTrue(v.getZ()==198.0);
        }

    }

    public void testSetY() throws Exception {

    }

    public void testSetZ() throws Exception {

    }

    public void testMakeInfinite() throws Exception {

    }

    public void testMakeDimension() throws Exception {

    }

    public void testGetCoordinate() throws Exception {
        Vector v = Geom3DSuits.createVector();
        v.setX(10);
        v.setY(10);
        v.setZ(10);
        v.getCoordinates()[0]=20.0;
        Assert.assertEquals(v.getX(),20.0);
    }

}