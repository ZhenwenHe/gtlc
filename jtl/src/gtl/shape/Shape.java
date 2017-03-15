package gtl.shape;


import gtl.math.geometry.Envelope;
import gtl.storage.Serializable;
import gtl.math.geometry.Vertex;

/**
 * Created by ZhenwenHe on 2016/12/6.
 */
public interface Shape extends Serializable {
    boolean intersectsShape(Shape in) ;
    boolean containsShape(Shape in)  ;
    boolean touchesShape(Shape in)  ;
    Vertex getCenter() ;
    int getDimension()  ;
    Envelope getMBR( ) ;
    double getArea()  ;
    double getMinimumDistance(Shape in)  ;
    Object clone();
}
