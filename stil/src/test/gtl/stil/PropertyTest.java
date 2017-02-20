package test.gtl.stil;

import gtl.stil.Property;
import gtl.stil.Variant;
import junit.framework.TestCase;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.util.Arrays;

/**
 * Created by hadoop on 17-2-20.
 */
public class PropertyTest extends TestCase {

    Property [] ps = null;

    public void setUp() throws Exception {
        super.setUp();
        ps = new Property[18];
        ps[0]=new Property("1",(Object) new Byte((byte)1));
        ps[1]=new Property("2",(Object) new Character('1'));
        ps[2]=new Property("3",(Object) new Boolean(true));
        ps[3]=new Property("4",(Object) new Short((short)1));
        ps[4]=new Property("5",(Object) new Integer((int)1));
        ps[5]=new Property("6",(Object) new Long((long)1L));
        ps[6]=new Property("7",(Object) new Float((float) 1.0f));
        ps[7]=new Property("8",(Object) new Double(1.0));
        ps[8]=new Property("9",(Object) new String("1"));
        byte[] bs = new byte[10];
        Arrays.fill(bs,(byte)1);
        ps[9]=new Property("10",(Object) bs);
        char[] cs = new char[10];
        Arrays.fill(cs,(char)'1');
        ps[10]=new Property("11",(Object) cs);
        short[] ss = new short[10];
        Arrays.fill(ss,(short)1);
        ps[11]=new Property("12",(Object) ss);
        int[] iss = new int[10];
        Arrays.fill(iss,(int)1);
        ps[12]=new Property("13",(Object) iss);
        long[] ls=new long[10];
        Arrays.fill(ls,1L);
        ps[13]=new Property("14",(Object) ls);
        float [] fs = new float[10];
        Arrays.fill(fs,1.0f);
        ps[14]=new Property("15",(Object) fs);
        double[] ds = new double[10];
        Arrays.fill(ds,1.0);
        ps[15]=new Property("16",(Object) ds);
        boolean[] bbs = new boolean[10];
        Arrays.fill(bbs,true);
        ps[16]=new Property("17",(Object) bbs);
        String[] ssv = new String [10];
        Arrays.fill(ssv,"1");
        ps[17]=new Property("18",(Object) ssv);

    }

    public void tearDown() throws Exception {

    }

    public void testGetName() throws Exception {

    }

    public void testSetName() throws Exception {

    }

    public void testEquals() throws Exception {

    }

    public void testToString() throws Exception {

    }

    public void testCopyFrom() throws Exception {

    }

    public void testRead() throws Exception {

    }

    public void testWrite() throws Exception {
        ByteArrayOutputStream bos = new ByteArrayOutputStream(10240);
        int len =0;
        for(int i=0;i<18;i++){
            ps[i].write(bos);
            len += ps[i].getByteArraySize();
        }
        byte[] bs = bos.toByteArray();
        assertTrue(bs.length==len);
        ByteArrayInputStream bis =new ByteArrayInputStream(bs);
        Property [] ps2=new Property[18];
        for(int i=0;i<18;i++){
            ps2[i]=new Property();
            ps2[i].read(bis);
        }


        assertTrue(((Variant)ps2[0]).byteValue()==(byte)1);
        assertTrue(((Variant)ps2[1]).charValue()==(char)1);
        assertTrue(((Variant)ps2[2]).booleanValue()==true);
        assertTrue(((Variant)ps2[3]).shortValue()==(short)1);
        assertTrue(((Variant)ps2[4]).intValue()==(int)1);
        assertTrue(((Variant)ps2[5]).longValue()==(long)1);
        assertTrue(((Variant)ps2[6]).floatValue()==(float) 1);
        assertTrue(((Variant)ps2[7]).doubleValue()==(double) 1);
        assertTrue(((Variant)ps2[8]).strValue()=="1");


    }

    public void testGetByteArraySize() throws Exception {

    }

}