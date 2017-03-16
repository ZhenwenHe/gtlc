package gtl.shape;

/**
 * Created by ZhenwenHe on 2016/12/7.
 */
public interface Region extends Shape {
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

    double getLowCoordinate(int i);
    double getHighCoordinate(int i);

    double [] getLowCoordinates();
    double [] getHighCoordinates();

    void setLowCoordinate(int i,double d);
    void setHighCoordinate(int i,double d);
    void makeInfinite(int dimension);
    void makeInfinite() ;
    void makeDimension(int dimension);
    void reset(double[] low, double [] high, int dimension);
    void reset(double[] low, double [] high);
}
