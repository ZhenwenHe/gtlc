package gtl.shape;

/**
 * Created by ZhenwenHe on 2016/12/8.
 */
public interface LineSegment extends Shape {
    boolean intersectsLineSegment( LineSegment l) ;
    boolean intersectsRegion( Region p) ;
    double getMinimumDistance( Point p) ;
    double getRelativeMinimumDistance( Point p) ;
    double getRelativeMaximumDistance( Region  r) ;
    double getAngleOfPerpendicularRay();

    void makeInfinite(int dimension);
    void makeDimension(int dimension);

    double[] getStartCoordinates();
    double[] getEndCoordinates();

    double getStartCoordinate(int i);
    double getEndCoordinate(int i);

    void reset(double [] s,double []e);
}
