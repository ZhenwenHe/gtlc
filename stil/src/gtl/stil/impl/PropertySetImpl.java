package gtl.stil.impl;

import gtl.stil.PropertySet;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.HashMap;

/**
 * Created by ZhenwenHe on 2016/12/27.
 */
public class PropertySetImpl extends HashMap<String,Object> implements PropertySet{
    @Override
    public void copyFrom(Object i) {

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
