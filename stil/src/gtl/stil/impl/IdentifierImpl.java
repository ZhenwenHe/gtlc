package gtl.stil.impl;

import gtl.stil.Identifier;

import java.io.*;

/**
 * Created by ZhenwenHe on 2016/12/9.
 */
public class IdentifierImpl implements Identifier {
    long data;

    public IdentifierImpl() {
        this.data = 0;
    }

    private IdentifierImpl(long data) {
        this.data = data;
    }

    public void setData(long d){
        this.data=d;
    }

    @Override
    public long longValue() {
        return data;
    }

    @Override
    public int intValue() {
        return (int)data;
    }

    @Override
    public Object clone() {
        return new IdentifierImpl(this.data);
    }

    @Override
    public boolean read(InputStream in) throws IOException {
        DataInputStream dis = new DataInputStream(in);
        this.data=dis.readLong();
        dis.close();
        return true;
    }

    @Override
    public boolean write(OutputStream out) throws IOException {
        DataOutputStream dos = new DataOutputStream(out);
        dos.writeLong(this.data);
        dos.close();
        return true;
    }

    @Override
    public long getByteArraySize() {
        return 8;
    }

    @Override
    public String toString() {
        return "IdentifierImpl{" +
                "data=" + data +
                '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof IdentifierImpl)) return false;

        IdentifierImpl that = (IdentifierImpl) o;

        return data == that.data;

    }

    @Override
    public int hashCode() {
        return (int) (data ^ (data >>> 32));
    }

    @Override
    public void reset(long v) {
        this.data=v;
    }

    @Override
    public void copyFrom(Identifier i) {
        this.data = i.longValue();
    }

    @Override
    public void copyTo(Identifier i) {
        i.reset(this.data);
    }

    @Override
    public void increase() {
        ++this.data;
    }
}
