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
    void reset(double[] low, double [] high, int dimension);
    void reset(double[] low, double [] high);

    boolean intersects(Envelope e);
    boolean contains(Envelope e);
    boolean touches(Envelope e);
    boolean contains(Vertex p);
    boolean touches(Vertex p);


    Envelope getIntersectingEnvelope(Envelope e);
    double getIntersectingArea(Envelope e);
    double getMargin();

    void combine(Envelope e);
    void combine(Vertex v);
    Envelope getCombinedEnvelope(Envelope e);

}
