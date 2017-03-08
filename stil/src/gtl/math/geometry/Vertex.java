package gtl.math.geometry;

import gtl.storage.Serializable;

/**
 * Created by ZhenwenHe on 2016/12/8.
 */
public interface Vertex extends Serializable {
    double[] getCoordinates();
    double getCoordinate(int i);

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
    double dotProduct(Vertex v);
    Vertex crossProduct(Vertex b);// this X  b
    double length();
    double angle(Vertex a , Vertex b);

    Vertex subtract  (Vertex b);//this - b;
    Vertex add(Vertex b);//this + b;
}
