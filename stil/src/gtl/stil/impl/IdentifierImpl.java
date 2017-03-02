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

    public IdentifierImpl(long data) {
        this.data = data;
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
    public boolean load(DataInput dis) throws IOException {
        this.data=dis.readLong();
        return true;
    }

    @Override
    public boolean store(DataOutput dos) throws IOException {
        dos.writeLong(this.data);
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
    public void copyFrom(Object i) {
        if(i instanceof Identifier)
        this.data = ((Identifier)i).longValue();
    }

    @Override
    public void increase() {
        ++this.data;
    }

    @Override
    public byte byteValue() {
        return (byte)this.data;
    }

    @Override
    public short shortValue() {
        return (short)this.data;
    }

    @Override
    public int compare(Identifier i) {
        if(this.data>i.longValue())
            return 1;
        else if(this.data==i.longValue())
            return 0;
        else
            return -1;
    }



}
