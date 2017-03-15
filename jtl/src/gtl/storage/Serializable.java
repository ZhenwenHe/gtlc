package gtl.storage;

import gtl.common.Variant;

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

    default boolean read(InputStream in) throws IOException{
        byte [] size=new byte[4];
        in.read(size);
        int len= Variant.byteArrayToInteger(size);
        if(len>0) {
            byte[] data = new byte[len];
            in.read(data);
            return loadFromByteArray(data);
        }
        else
            return false;
    }
    default boolean write(OutputStream out) throws IOException{
        byte [] data = storeToByteArray();
        byte[] size = Variant.integerToByteArray(data.length);
        out.write(size);
        out.write(data);
        return true;
    }

    boolean load(DataInput in) throws IOException;
    boolean store(DataOutput out) throws IOException;

    long getByteArraySize();

    default  byte[] storeToByteArray()  throws IOException{
        ByteArrayOutputStream baos= new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(baos);
         store(dos);
        dos.flush();
        byte [] bs = baos.toByteArray();
        dos.close();
        return bs;
    }
    default boolean loadFromByteArray(byte[] bs) throws IOException{
        DataInputStream dis = new DataInputStream(new ByteArrayInputStream(bs));
        boolean b=  load(dis);
        dis.close();
        return b;
    }
}
