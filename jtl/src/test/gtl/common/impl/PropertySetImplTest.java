package test.gtl.common.impl;

import gtl.common.CommonSuits;
import gtl.common.PropertySet;
import gtl.common.Variant;
import junit.framework.TestCase;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;

/**
 * Created by hadoop on 17-2-18.
 */
public class PropertySetImplTest extends TestCase {
    PropertySet ps=null;
    public void setUp() throws Exception {
        super.setUp();
        System.out.println("setUp");
        ps = CommonSuits.createPropertySet();//new PropertySetImpl();
        ps.put("Byte",new Variant((byte)1));
        ps.put("Character",new Variant('1'));
        ps.put("Boolean",new Variant(true));
        ps.put("Short",new Variant((short)1));
        ps.put("Integer",new Variant(1));
        ps.put("Long",new Variant(1L));
        ps.put("Float",new Variant(1.0f));
        ps.put("Double",new Variant(1.0));
        ps.put("String",new Variant("1"));
        byte [] data = new byte[10];
        for(int i=0;i<10;i++) data[i]=(byte)1;
        ps.put("Bytes",new Variant(data));
    }

    public void tearDown() throws Exception {
        ps.clear();
        System.out.println("tearDown");
    }

    public void testCopyFrom() throws Exception {
        System.out.println("testCopyFrom");
        PropertySet ps3=CommonSuits.createPropertySet();// PropertySetImpl();

        ps.copyTo(ps3);
        assertTrue(((Variant)ps3.get("Byte")).byteValue()==(byte)1);
        assertTrue(((Variant)ps3.get("Boolean")).booleanValue()==true);
        assertTrue(((Variant)ps3.get("Character")).charValue()=='1');
        assertTrue(((Variant)ps3.get("Short")).shortValue()==1);
        assertTrue(((Variant)ps3.get("Integer")).intValue()==1);
        assertTrue(((Variant)ps3.get("Long")).longValue()==1);
        assertTrue(((Variant)ps3.get("Float")).floatValue()==1.0f);
        assertTrue(((Variant)ps3.get("Double")).doubleValue()==1);
        assertTrue(((String)ps3.get("String").getValue()).equals("1"));
        byte[] data=(byte[]) ps3.get("Bytes").getValue();
        assertTrue(data.length==10);
        for(byte a:data)
            assertTrue(a==1);
    }

    public void testRead() throws Exception {
        System.out.println("testRead");
        byte [] data=ps.storeToByteArray();
        PropertySet ps2=CommonSuits.createPropertySet();//new PropertySetImpl();
        ps2.loadFromByteArray(data);
        assertTrue(((Variant)ps2.get("Byte")).byteValue()==(byte)1);
        assertTrue(((Variant)ps2.get("Boolean")).booleanValue()==true);
        assertTrue(((Variant)ps2.get("Character")).charValue()=='1');
        assertTrue(((Variant)ps2.get("Short")).shortValue()==1);
        assertTrue(((Variant)ps2.get("Integer")).intValue()==1);
        assertTrue(((Variant)ps2.get("Long")).longValue()==1);
        assertTrue(((Variant)ps2.get("Float")).floatValue()==1.0f);
        assertTrue(((Variant)ps2.get("Double")).doubleValue()==1);
        assertTrue(((String)ps2.get("String").getValue()).equals("1"));
        data=(byte[]) ps2.get("Bytes").getValue();
        assertTrue(data.length==10);
        for(byte a:data)
            assertTrue(a==1);

    }

    public void testWrite() throws Exception {
        System.out.println("testWrite");
        ByteArrayOutputStream bos=new ByteArrayOutputStream(10240);
        ps.write(bos);
        int len = bos.size();
        assertTrue(len==ps.getByteArraySize());
        byte[]data=bos.toByteArray();
        ByteArrayInputStream bis=new ByteArrayInputStream(data);
        PropertySet ps2=CommonSuits.createPropertySet();//new PropertySetImpl();
        ps2.read(bis);
        assertTrue(((Variant)ps2.get("Byte")).byteValue()==(byte)1);
        assertTrue(((Variant)ps2.get("Boolean")).booleanValue()==true);
        assertTrue(((Variant)ps2.get("Character")).charValue()=='1');
        assertTrue(((Variant)ps2.get("Short")).shortValue()==1);
        assertTrue(((Variant)ps2.get("Integer")).intValue()==1);
        assertTrue(((Variant)ps2.get("Long")).longValue()==1);
        assertTrue(((Variant)ps2.get("Float")).floatValue()==1.0f);
        assertTrue(((Variant)ps2.get("Double")).doubleValue()==1);
        assertTrue(((String)ps2.get("String").getValue()).equals("1"));
        data=(byte[]) ps2.get("Bytes").getValue();
        assertTrue(data.length==10);
        for(byte a:data)
            assertTrue(a==1);
    }

    public void testGetByteArraySize() throws Exception {
        System.out.println("testGetByteArraySize");
    }

}