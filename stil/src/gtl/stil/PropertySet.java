package gtl.stil;


import java.util.Map;

/**
 * Created by ZhenwenHe on 2016/12/6.
 */
public interface  PropertySet extends Map<String,Variant>,gtl.stil.Serializable {
    void put(Property p);
    Variant getProperty(String key);
}
