package gtl.geom;

import gtl.io.Serializable;

/**
 * Created by ZhenwenHe on 2016/12/8.
 */
public interface Vector extends Serializable {
    /**
     * The value used to indicate a null or missing ordinate value.
     * In particular, used for the value of ordinates for dimensions
     * greater than the defined dimension of a coordinate.
     */
    public static final double NULL_ORDINATE = Double.NaN;

    double[] getCoordinates();
    double getOrdinate(int i);
    void setOrdinate(int i,double d);

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
    Vector multiply(Scalar s);
    Vector multiply(double s);
}
