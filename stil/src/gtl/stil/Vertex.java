package gtl.stil;

/**
 * Created by ZhenwenHe on 2016/12/8.
 */
public interface Vertex extends Serializable{
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
}
