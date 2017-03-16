package gtl.geom;

import gtl.io.Serializable;

/**
 * Created by ZhenwenHe on 2016/12/7.
 */
public interface Envelope extends Serializable {
    int getDimension();
    double[] getLowCoordinates();
    double[] getHighCoordinates();
    double getLowCoordinate(int i);
    double getHighCoordinate(int i);
    void setLowCoordinate(int i,double d);
    void setHighCoordinate(int i,double d);
    void makeInfinite(int dimension);
    void makeInfinite() ;
    void makeDimension(int dimension);
    void reset(double[] low, double [] high, int dimension);
    void reset(double[] low, double [] high);
    Envelope2D flap();

    boolean intersects(Envelope e);
    boolean contains(Envelope e);
    boolean touches(Envelope e);
    boolean contains(Vector p);
    boolean touches(Vector p);


    Envelope getIntersectingEnvelope(Envelope e);
    double getIntersectingArea(Envelope e);
    double getMargin();

    void combine(Envelope e);
    void combine(Vector v);
    Envelope getCombinedEnvelope(Envelope e);

}
