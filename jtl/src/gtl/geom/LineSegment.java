package gtl.geom;

/**
 * Created by ZhenwenHe on 2017/3/13.
 */
public interface LineSegment extends gtl.io.Serializable, Comparable<LineSegment> {
    void reset(Vector s, Vector e);
    void reset(double[] s, double[] e);
    Vector getStartVertex();
    Vector getEndVertex();
}
