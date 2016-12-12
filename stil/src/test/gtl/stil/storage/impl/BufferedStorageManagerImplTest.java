package test.gtl.stil.storage.impl;

import gtl.stil.Identifier;
import gtl.stil.IndexSuits;
import gtl.stil.storage.BufferedStorageManager;
import gtl.stil.storage.StorageManager;
import junit.framework.Assert;
import junit.framework.TestCase;

import java.io.File;
import java.util.ArrayList;
import java.util.Iterator;

/**
 * Created by ZhenwenHe on 2016/12/10.
 */
public class BufferedStorageManagerImplTest extends TestCase {
    public void setUp() throws Exception {
        super.setUp();
        StorageManager sm = IndexSuits.createMemoryStorageManager();
        BufferedStorageManager bsm = IndexSuits.createBufferedStorageManager(sm,5,true);
        ArrayList<Identifier> ids=new ArrayList<Identifier>();
        int pageSize =1024*8;//8k
        int dataSize=(int)(pageSize*1.6);
        byte [] data =new byte[dataSize];
        for(int i=0;i<10;i++){
            for(int j=0;j<dataSize;j++){
                data[j]=(byte)i;
            }
            Identifier pi = IndexSuits.createIdentifier(StorageManager.NEW_PAGE);
            bsm.storeByteArray(pi,data);
            ids.add((Identifier) pi.clone());
        }


        Iterator<Identifier> it = ids.iterator();
        int k=0;
        while(it.hasNext()){
            data=null;
            Identifier id=it.next();
            System.out.println(id);
            data=bsm.loadByteArray(id);
            Assert.assertEquals(data.length,dataSize);
            System.out.println(data.length);
            System.out.println(data[0]);
            Assert.assertEquals(k,data[0]);
            System.out.println(data[data.length-1]);
            Assert.assertEquals(k,data[data.length-1]);
            k++;
        }

        System.out.println("hits="+bsm.getHits());
        bsm.close();
    }

    public void tearDown() throws Exception {

    }

    public void testGetHits() throws Exception {

    }

    public void testLoadByteArray() throws Exception {

    }

    public void testStoreByteArray() throws Exception {
        int pageSize =1024*8;//8k
        ArrayList<Identifier> ids=new ArrayList<Identifier>();
        StorageManager sm = IndexSuits.createDiskStorageManager("h:"+ File.separator+"test",pageSize,true);
        BufferedStorageManager bsm = IndexSuits.createBufferedStorageManager(sm,5,true);

        int dataSize=(int)(pageSize*2.6);
        byte [] data =new byte[dataSize];
        for(int i=0;i<10;i++){
            for(int j=0;j<dataSize;j++){
                data[j]=(byte)i;
            }
            Identifier pi = IndexSuits.createIdentifier(StorageManager.NEW_PAGE);
            bsm.storeByteArray(pi,data);
            ids.add((Identifier) pi.clone());
        }

        Iterator<Identifier> it = ids.iterator();
        int k=0;
        while(it.hasNext()){
            data=null;
            Identifier id=it.next();
            System.out.println(id);
            data=bsm.loadByteArray(id);
            Assert.assertEquals(data.length,dataSize);
            System.out.println(data.length);
            System.out.println(data[0]);
            Assert.assertEquals(k,data[0]);
            System.out.println(data[data.length-1]);
            Assert.assertEquals(k,data[data.length-1]);
            k++;
        }

        System.out.println("hits="+bsm.getHits());

        bsm.close();
    }

    public void testDeleteByteArray() throws Exception {

    }

    public void testFlush() throws Exception {

    }

    public void testClear() throws Exception {

    }

    public void testAddEntry() throws Exception {

    }

    public void testRemoveEntry() throws Exception {

    }

    public void testClose() throws Exception {

    }

    public void testEvictionStrategy() throws Exception {

    }

}