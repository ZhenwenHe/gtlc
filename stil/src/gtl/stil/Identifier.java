package gtl.stil;


/**
 * Created by ZhenwenHe on 2016/12/6.
 */
public interface Identifier extends Serializable{
    long longValue();
    int intValue();
    void reset(long v);
    void copyFrom(Identifier i );
    void copyTo(Identifier i);
    void increase();
}
