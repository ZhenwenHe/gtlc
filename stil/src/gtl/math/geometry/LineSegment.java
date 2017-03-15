package gtl.math.geometry;

import gtl.storage.Serializable;

/**
 * Created by ZhenwenHe on 2017/3/13.
 */
public interface LineSegment extends Serializable{
    void reset(Vertex s, Vertex e);
    void reset(double[] s, double[] e);
    Vertex getStartVertex();
    Vertex getEndVertex();
}
