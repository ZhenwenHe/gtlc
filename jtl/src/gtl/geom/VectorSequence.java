package gtl.geom;

import java.util.Collection;

/**
 * Created by hadoop on 17-3-25.
 */
public interface VectorSequence extends Collection<Vector> ,gtl.io.Serializable{
    int getDimension();
    double getOrdinate(int index, int dim);
    double getX(int index);
    double getY(int index);
    double getZ(int index);
    void setX(int index,double d);
    void setY(int index,double d);
    void setZ(int index,double d);
    Vector3D getVector3D(int index);
    Vector2D getVector2D(int index);
    Vector4D getVector4D(int index);
    Vector getVector(int index);
    Vector getVector(int index, Vector v);
    int find(Vector v);
    Vector remove(int index);
    void insert(int index, Vector v);
    void add(double x, double y);
    void add(double x, double y, double z);
    void add(double x, double y, double z,double w);
    void reset(double[] vectors,int dimension);
}
