package gtl.stil.shape;

import gtl.stil.Vertex;

/**
 * Created by ZhenwenHe on 2016/12/6.
 */
public interface Point extends Shape,Vertex {
    double getMinimumDistance(Point p) ;
}
