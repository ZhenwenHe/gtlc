package gtl.stil;

import java.io.*;

/**
 * Created by ZhenwenHe on 2016/12/8.
 */
public interface Serializable extends java.io.Serializable, Cloneable{
    Object clone();
    void copyFrom(Object i );

    /**
     * 将本对象的信息复制到相同的对象i中，并返回i
     * 如果i为空，则调用this.clone()，并返回clone的对象
     * 如果对象类型不匹配，则返回null
     * @param i
     */
    default Object copyTo(Object i){
        if(i==null){
            return clone();
        }
        else {
            if(i instanceof Serializable){
                ((Serializable)i).copyFrom(this);
                return i;
            }
            else{
                return null;
            }
        }
    }

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
