package gtl.stil;

import gtl.common.Identifier;
import gtl.shape.Shape;
import gtl.storage.Serializable;

/**
 * Created by ZhenwenHe on 2016/12/7.
 */
public interface Entry extends Serializable {
    Identifier getIdentifier() ;
    Shape getShape();
    void setIdentifier(long id) ;
    void setIdentifier(Identifier id) ;
    void setShape(Shape s) ;
    byte[] getData();
    void setData(byte[] data);
    long getDataLength();
}
