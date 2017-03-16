package test.gtl.index;

import gtl.math.geometry.Envelope;
import gtl.math.geometry.GeometrySuits;
import gtl.index.IndexSuits;
import gtl.common.Variant;
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
        {
            Envelope [] envs= GeometrySuits.generateRandomEnvelopes(100,2,0.0001,0.001);
            GeometrySuits.writeEnvelopeFile(envs,IndexSuits.DATA_DIR +"test2d100.envelopes");
            Envelope[] tenvs=GeometrySuits.readEnvelopeFile(IndexSuits.DATA_DIR +"test2d100.envelopes");
            for(int i=0;i<100;++i) {
                assertTrue(tenvs[i].equals(envs[i]));
            }
        }

        {
            Envelope [] envs= GeometrySuits.generateRandomEnvelopes(100,3,0.0001,0.001);
            GeometrySuits.writeEnvelopeFile(envs,IndexSuits.DATA_DIR +"test3d100.envelopes");
            Envelope[] tenvs=GeometrySuits.readEnvelopeFile(IndexSuits.DATA_DIR +"test3d100.envelopes");
            for(int i=0;i<100;++i) {
                assertTrue(tenvs[i].equals(envs[i]));
            }
        }

        {
            Envelope [] envs= GeometrySuits.generateRandomEnvelopes(100,4,0.0001,0.001);
            GeometrySuits.writeEnvelopeFile(envs,IndexSuits.DATA_DIR +"test4d100.envelopes");
            Envelope[] tenvs=GeometrySuits.readEnvelopeFile(IndexSuits.DATA_DIR +"test4d100.envelopes");
            for(int i=0;i<100;++i) {
                assertTrue(tenvs[i].equals(envs[i]));
            }
        }
    }

}