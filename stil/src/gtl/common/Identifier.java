package gtl.common;


import gtl.storage.Serializable;

/**
 * Created by ZhenwenHe on 2016/12/6.
 */
public interface Identifier extends Serializable {
    byte byteValue();
    short shortValue();
    int intValue();
    long longValue();
    void reset(long v);
    void increase();
    int compare(Identifier i);
}
