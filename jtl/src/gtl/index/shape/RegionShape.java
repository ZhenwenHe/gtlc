package gtl.index.shape;

/**
 * Created by ZhenwenHe on 2016/12/7.
 */
public interface RegionShape extends Shape {
    boolean intersectsRegion( RegionShape in) ;
    boolean containsRegion( RegionShape in) ;
    boolean touchesRegion( RegionShape in) ;
    double getMinimumDistance( RegionShape in) ;

    boolean intersectsLineSegment( LineSegmentShape in) ;

    boolean containsPoint( PointShape in) ;
    boolean touchesPoint( PointShape in) ;
    double getMinimumDistance( PointShape in) ;

    RegionShape getIntersectingRegion(RegionShape r) ;
    double getIntersectingArea( RegionShape in) ;
    double getMargin() ;

    void combineRegion( RegionShape in);
    void combinePoint( PointShape in);
    RegionShape getCombinedRegion(RegionShape in) ;

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
