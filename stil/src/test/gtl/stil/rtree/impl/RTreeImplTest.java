package test.gtl.stil.rtree.impl;

import gtl.stil.shape.Region;
import junit.framework.TestCase;

import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.util.Map;
import java.util.Random;

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
        FileOutputStream fw = new FileOutputStream("H:"+ File.separator+"rtree.dat");
        DataOutputStream dos = new DataOutputStream(fw);

        dos.close();
        fw.close();
    }

    public void testReset1() throws Exception {

    }

    public void testInsert() throws Exception {

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