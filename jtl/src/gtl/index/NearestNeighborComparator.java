package gtl.index;

import gtl.index.shape.Shape;

/**
 * Created by ZhenwenHe on 2016/12/6.
 */
public interface NearestNeighborComparator {
    public double getMinimumDistance(Shape query, Shape entry) ;
    public double getMinimumDistance(Shape query, Entry  data) ;
}
