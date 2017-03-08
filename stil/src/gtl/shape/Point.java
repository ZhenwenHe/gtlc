package gtl.shape;

import gtl.math.geometry.Vertex;

/**
 * Created by ZhenwenHe on 2016/12/6.
 */
public interface Point extends Shape,Vertex {
    double getMinimumDistance(Point p) ;
}
