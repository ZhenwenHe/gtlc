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

    default boolean read(InputStream in) throws IOException{
        byte [] size=new byte[4];
        in.read(size);
        int ch1 = size[0];
        int ch2 = size[1];
        int ch3 = size[2];
        int ch4 = size[3];
        if ((ch1 | ch2 | ch3 | ch4) < 0)
            throw new EOFException();
        int len= ((ch1 << 24) + (ch2 << 16) + (ch3 << 8) + (ch4 << 0));
        byte [] data=new byte[len];
        in.read(data);
        return loadFromByteArray(data);
    }
    default boolean write(OutputStream out) throws IOException{
        byte [] data = storeToByteArray();
        int v= data.length;
        assert v==getByteArraySize();
        if (v==0)return false;
        byte[] size = new byte[4];
        size[0]=(byte) ((v >>> 24) & 0xFF);
        size[1]=(byte)((v >>> 16) & 0xFF);
        size[2]=(byte)((v >>>  8) & 0xFF);
        size[3]=(byte)((v >>>  0) & 0xFF);
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
        byte [] bs = baos.toByteArray();
        baos.close();
        return bs;
    }
    default boolean loadFromByteArray(byte[] bs) throws IOException{
        DataInputStream dis = new DataInputStream(new ByteArrayInputStream(bs));
        boolean b=  load(dis);
        dis.close();
        return b;
    }
}
