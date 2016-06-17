package gtl.stil;

import java.io.*;

/**
 * Created by ZhenwenHe on 2016/12/8.
 */
public interface Serializable extends java.io.Serializable, Cloneable{
    Object clone();
    boolean read(InputStream in) throws IOException;
    boolean write(OutputStream out) throws IOException;

    long getByteArraySize();

    default  byte[] storeToByteArray()  throws IOException{
        ByteArrayOutputStream baos= new ByteArrayOutputStream();
         write(baos);
        byte [] bs = baos.toByteArray();
        baos.close();
        return bs;
    }
    default boolean loadFromByteArray(byte[] bs) throws IOException{
        ByteArrayInputStream bais = new ByteArrayInputStream(bs);
        boolean b=  read(bais);
        bais.close();
        return b;
    }

}
