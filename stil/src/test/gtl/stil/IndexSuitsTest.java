package test.gtl.stil;

import gtl.stil.Envelope;
import gtl.stil.IndexSuits;
import gtl.stil.Variant;
import gtl.stil.shape.EvolvingShape;
import junit.framework.TestCase;

import java.io.*;

/**
 * Created by hadoop on 17-2-18.
 */
public class IndexSuitsTest extends TestCase {
    public void setUp() throws Exception {
        super.setUp();

    }

    public void tearDown() throws Exception {

    }

    public void testReadBytes() throws Exception {

    }

    public void testReadChars() throws Exception {

    }

    public void testReadString() throws Exception {


    }

    public void testWriteBytes() throws Exception {
        String s = "Hello world 测试";
        ByteArrayOutputStream bos = new ByteArrayOutputStream(1024);
        DataOutputStream dos = new DataOutputStream(bos);
        Variant.writeString(dos,s);
        byte[] dat = bos.toByteArray();
        int len = bos.size();
        dos.close();
        bos.close();

        ByteArrayInputStream bis=new ByteArrayInputStream(dat);
        DataInputStream dis=new DataInputStream(bis);
        String ss=Variant.readString(dis);
        dis.close();
        bis.close();

        assertTrue(s.equals(ss));
        System.out.println(ss);
        System.out.println(len);
    }

    public void testWriteChars() throws Exception {

    }

    public void testWriteString() throws Exception {

    }

    public void testWriteEnvelopeFile()throws Exception{
        Envelope [] envs=IndexSuits.generateRandomEnvelopes(100,4,0.0001,0.001);
        IndexSuits.writeEnvelopeFile(envs,IndexSuits.DATA_DIR +"test.envelopes");
        Envelope[] tenvs=IndexSuits.readEnvelopeFile(IndexSuits.DATA_DIR +"test.envelopes");
        for(int i=0;i<100;++i) {
            assertTrue(tenvs[i].equals(envs[i]));
        }
    }

}