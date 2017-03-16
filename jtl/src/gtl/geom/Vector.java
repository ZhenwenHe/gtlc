package gtl.geom;

import gtl.io.Serializable;

/**
 * Created by ZhenwenHe on 2016/12/8.
 */
public interface Vector extends Serializable {
    double[] getCoordinates();
    double getOrdinate(int i);

    int getDimension();

    double getX();
    double getY();
    double getZ();
    void setX(double x);
    void setY(double y);
    void setZ(double z);

    void makeInfinite(int dimension);
    void makeDimension(int dimension);

    void reset(double [] coordinates);

    double normalize();// return the original vector length
    double dotProduct(Vector v);
    Vector crossProduct(Vector b);// this X  b
    double length();
    double angle(Vector a , Vector b);

    Vector subtract  (Vector b);//this - b;
    Vector add(Vector b);//this + b;
}
