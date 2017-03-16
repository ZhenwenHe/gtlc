package test.gtl.common;

import gtl.common.Variant;
import junit.framework.TestCase;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.util.Arrays;

/**
 * Created by hadoop on 17-2-18.
 */
public class VariantTest extends TestCase {

    Variant [] vs =null;
    byte [] buffer=null;
    public void setUp() throws Exception {
        super.setUp();
        vs = new Variant[18];
        vs[0]=new Variant((boolean) true);
        vs[1]=new Variant((char)('1'));
        vs[2]=new Variant((byte)1);
        vs[3]=new Variant((short) 1);
        vs[4]=new Variant((int) 1);
        vs[5]=new Variant((long) 1);
        vs[6]=new Variant((float) 1);
        vs[7]=new Variant((double) 1);
        vs[8]=new Variant((String) "1");
        boolean [] bb = new boolean[10];
        Arrays.fill(bb,true);
        vs[9]=new Variant(bb);
        char []cc =new char[10];
        Arrays.fill(cc,'1');
        vs[10]=new Variant( cc);
        short ss[]=new short[10];
        Arrays.fill(ss,(short)1);
        vs[11]=new Variant(ss);
        int ii[]=new int[10];
        Arrays.fill(ii,(int)1);
        vs[12]=new Variant(ii);
        float ff[]=new float[10];
        Arrays.fill(ff,(float) 1);
        vs[13]=new Variant(ff);
        double dd[]=new double[10];
        Arrays.fill(dd,(double) 1);
        vs[14]=new Variant(dd);
        byte[] yy=new byte[10];
        Arrays.fill(yy,(byte)1);
        vs[15]=new Variant(yy);
        long [] ll = new long[10];
        Arrays.fill(ll,(long)1);
        vs[16]=new Variant(ll);
        String [] tt=new String[10];
        Arrays.fill(tt,"1");
        vs[17]=new Variant(tt);


    }

    public void tearDown() throws Exception {

    }

    public void testClone() throws Exception {
        Variant []tv = new Variant[18];
        int i=0;
        for(Variant v:vs){
            tv[i]=(Variant) v.clone();
            assertTrue(tv[i].equals(v));
            i++;
        }
    }

    public void testReset() throws Exception {

    }

    public void testReset1() throws Exception {

    }

    public void testReset2() throws Exception {

    }

    public void testReset3() throws Exception {

    }

    public void testReset4() throws Exception {

    }

    public void testReset5() throws Exception {

    }

    public void testReset6() throws Exception {

    }

    public void testReset7() throws Exception {

    }

    public void testReset8() throws Exception {

    }

    public void testReset9() throws Exception {

    }

    public void testReset10() throws Exception {

    }

    public void testReset11() throws Exception {

    }

    public void testReset12() throws Exception {

    }

    public void testReset13() throws Exception {

    }

    public void testReset14() throws Exception {

    }

    public void testReset15() throws Exception {

    }

    public void testReset16() throws Exception {

    }

    public void testReset17() throws Exception {

    }

    public void testReset18() throws Exception {

    }

    public void testCopyFrom() throws Exception {

        int num=18;
        Variant []vts=new Variant[num];
        for(int i=0;i<num;i++){
            vts[i]=new Variant();
            vs[i].copyTo(vts[i]);

            assertTrue(vts[i].equals(vs[i]));

            vts[i].loadFromByteArray(vs[i].storeToByteArray());
            assertTrue(vs[i].equals(vts[i]));
        }


    }

    public void testRead() throws Exception {

    }

    public void testWrite() throws Exception {
        int num=18;
        ByteArrayOutputStream bos = new ByteArrayOutputStream(10240);
        int len =0;
        for(int i=0;i<num;i++){
            vs[i].write(bos);
            len+=vs[i].getByteArraySize();

        }
        byte [] buf = bos.toByteArray();
        ByteArrayInputStream bis=new ByteArrayInputStream(buf);
        Variant []vts=new Variant[num];
        for(int i=0;i<num;i++){
            vts[i]=new Variant();
            vts[i].read(bis);
            assertTrue(vts[i].equals(vs[i]));
        }

    }

    public void testGetByteArraySize() throws Exception {

    }

    public void testReadBytes() throws Exception {

    }

    public void testWriteBytes() throws Exception {

    }

    public void testReadBooleans() throws Exception {

    }

    public void testWriteBooleans() throws Exception {

    }

    public void testReadChars() throws Exception {

    }

    public void testWriteChars() throws Exception {

    }

    public void testReadString() throws Exception {

    }

    public void testWriteString() throws Exception {

    }

    public void testReadShorts() throws Exception {

    }

    public void testWriteShorts() throws Exception {

    }

    public void testReadIntegers() throws Exception {

    }

    public void testWriteIntegers() throws Exception {
        byte [] s = Variant.integerToByteArray(250);
        int v=Variant.byteArrayToInteger(s);
        assertTrue(v==250);
    }

    public void testReadLongs() throws Exception {

    }

    public void testWriteLongs() throws Exception {
        byte [] s = Variant.longToByteArray(2500000345L);
        long v=Variant.byteArrayToLong(s);
        assertTrue(v==2500000345L);
    }

    public void testReadFloats() throws Exception {

    }

    public void testWriteFloats() throws Exception {
        byte [] s = Variant.floatToByteArray(3456.789f);
        float v=Variant.byteArrayToFloat(s);
        assertTrue(v==3456.789f);
    }

    public void testReadDoubles() throws Exception {

    }

    public void testWriteDoubles() throws Exception {
        byte [] s = Variant.doubleToByteArray(3456.789909090);
        double v=Variant.byteArrayToDouble(s);
        assertTrue(v==3456.789909090);
    }

    public void testReadStrings() throws Exception {

    }

    public void testWriteStrings() throws Exception {

    }

}