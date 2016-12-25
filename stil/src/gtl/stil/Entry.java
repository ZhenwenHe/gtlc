package gtl.stil;

import gtl.stil.shape.Shape;

/**
 * Created by ZhenwenHe on 2016/12/7.
 */
public interface Entry extends Serializable{
    Identifier getIdentifier() ;
    Shape getShape( ) ;
    void setIdentifier(long id) ;
    void setIdentifier(Identifier id) ;
    void setShape(Shape s) ;
}
