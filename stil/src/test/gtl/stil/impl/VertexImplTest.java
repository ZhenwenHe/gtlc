package test.gtl.stil.impl;

import gtl.stil.IndexSuits;
import gtl.stil.Vertex;
import junit.framework.Assert;
import junit.framework.TestCase;

import java.awt.geom.Arc2D;

/**
 * Created by ZhenwenHe on 2016/12/15.
 */
public class VertexImplTest extends TestCase {
    public void setUp() throws Exception {
        super.setUp();


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
        Vertex v = IndexSuits.createVertex();
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

    }

    public void testWrite() throws Exception {

    }

    public void testGetByteArraySize() throws Exception {

    }

    public void testSetX() throws Exception {

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
        Vertex v = IndexSuits.createVertex();
        v.setX(10);
        v.setY(10);
        v.setZ(10);
        v.getCoordinates()[0]=20.0;
        Assert.assertEquals(v.getX(),20.0);
    }

}