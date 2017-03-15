package gtl.shape;

import gtl.math.geometry.Envelope;
import gtl.math.geometry.Vertex;

/**
 * Created by ZhenwenHe on 2016/12/6.
 */
public interface Point extends Shape {
    double getMinimumDistance(Point p) ;
    public double[] getCoordinates();
    public double getCoordinate(int i);
    public double getX();
    public double getY();
    public double getZ() ;
    public void setX(double x) ;
    public void setY(double y) ;
    public void setZ(double z) ;
    public void makeInfinite(int dimension) ;
    public void makeDimension(int dimension);
    public void reset(double[] coordinates);
    public boolean intersectsShape(Shape in) ;
    public boolean containsShape(Shape in);
    public boolean touchesShape(Shape in) ;
    public Vertex getCenter();
    public int getDimension();
    public Envelope getMBR();
    public double getMinimumDistance(Shape in);
}
