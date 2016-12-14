package gtl.stil.shape;

import gtl.stil.Envelope;

/**
 * Created by ZhenwenHe on 2016/12/7.
 */
public interface Region extends Shape,Envelope {
    boolean intersectsRegion( Region in) ;
    boolean containsRegion( Region in) ;
    boolean touchesRegion( Region in) ;
    double getMinimumDistance( Region in) ;

    boolean intersectsLineSegment( LineSegment in) ;

    boolean containsPoint( Point in) ;
    boolean touchesPoint( Point in) ;
    double getMinimumDistance( Point in) ;

    Region getIntersectingRegion( Region r) ;
    double getIntersectingArea( Region in) ;
    double getMargin() ;

    void combineRegion( Region in);
    void combinePoint( Point in);
    Region  getCombinedRegion(  Region in) ;
}
