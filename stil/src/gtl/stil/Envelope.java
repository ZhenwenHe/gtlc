package gtl.stil;

/**
 * Created by ZhenwenHe on 2016/12/7.
 */
public interface Envelope extends Serializable{
    int getDimension();
    double[] getLowCoordinates();
    double[] getHighCoordinates();
    double getLowCoordinate(int i);
    double getHighCoordinate(int i);
    void makeInfinite(int dimension);
    void makeDimension(int dimension);
    void initialize(double[] low, double [] high, int dimension);
}
