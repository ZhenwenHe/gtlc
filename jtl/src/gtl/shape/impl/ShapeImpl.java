package gtl.shape.impl;

import gtl.geom.Envelope;
import gtl.geom.Vector;
import gtl.shape.LineSegment;
import gtl.shape.Point;
import gtl.shape.Region;

/**
 * Created by ZhenwenHe on 2017/3/15.
 */
public class ShapeImpl {
    public static LineSegment createLineSegment(Vector s, Vector e){
        return new LineSegmentImpl(s,e);
    }
    public static LineSegment createLineSegment(Point s, Point e){
        return new LineSegmentImpl(s.getCenter(),e.getCenter());
    }
    public static Point createPoint( ){
        return new PointImpl( );
    }
    public static Point createPoint(double[] c ){
        return new PointImpl(c);
    }
    public static Point createPoint(double x,double y){
        return new PointImpl(x,y);
    }
    public static Point createPoint(double x,double y,double z){
        return new PointImpl(x,y,z);
    }
    public static Point[] createPointArray(int size ){
        return new PointImpl[size];
    }
    public static Region createRegion(){
        return new RegionImpl();
    }
    public static Region createRegion(double [] low, double [] high){
        return new RegionImpl(low,high);
    }
    public static Region createRegion(Envelope e){
        return new RegionImpl(e);
    }
    public static Region[] createRegionArray(Region[] c ){
        Region[] r= new  Region[c.length];
        for(int i=0;i<r.length;i++){
            r[i]=(Region) c[i].clone();
        }
        return r;
    }
}
