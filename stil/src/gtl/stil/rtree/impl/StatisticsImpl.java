package gtl.stil.rtree.impl;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * Created by ZhenwenHe on 2016/12/16.
 */
public class StatisticsImpl implements gtl.stil.Statistics{
    @Override
    public long getReads() {
        return 0;
    }

    @Override
    public long getWrites() {
        return 0;
    }

    @Override
    public Object clone() {
        return null;
    }

    @Override
    public int getNumberOfNodes() {
        return 0;
    }

    @Override
    public void copyFrom(Object i) {

    }

    @Override
    public long getNumberOfData() {
        return 0;
    }

    @Override
    public void copyTo(Object i) {

    }

    @Override
    public boolean read(InputStream in) throws IOException {
        return false;
    }

    @Override
    public boolean write(OutputStream out) throws IOException {
        return false;
    }

    @Override
    public long getByteArraySize() {
        return 0;
    }
}
