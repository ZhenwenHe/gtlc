package gtl.index.shape.impl;

import gtl.geom.Envelope;
import gtl.geom.Vector;
import gtl.index.shape.LineSegmentShape;
import gtl.index.shape.PointShape;
import gtl.index.shape.RegionShape;

/**
 * Created by ZhenwenHe on 2017/3/15.
 */
public class ShapeImpl {
    public static LineSegmentShape createLineSegment(Vector s, Vector e){
        return new LineSegmentShape(s,e);
    }
    public static LineSegmentShape createLineSegment(PointShape s, PointShape e){
        return new LineSegmentShape(s.getCenter(),e.getCenter());
    }
    public static PointShape createPoint( ){
        return new PointShapeImpl( );
    }
    public static PointShape createPoint(double[] c ){
        return new PointShapeImpl(c);
    }
    public static PointShape createPoint(double x, double y){
        return new PointShapeImpl(x,y);
    }
    public static PointShape createPoint(double x, double y, double z){
        return new PointShapeImpl(x,y,z);
    }
    public static PointShape[] createPointArray(int size ){
        return new PointShapeImpl[size];
    }
    public static RegionShape createRegion(){
        return new RegionShapeImpl();
    }
    public static RegionShape createRegion(double [] low, double [] high){
        return new RegionShapeImpl(low,high);
    }
    public static RegionShape createRegion(Envelope e){
        return new RegionShapeImpl(e);
    }
    public static RegionShape[] createRegionArray(RegionShape[] c ){
        RegionShape[] r= new RegionShape[c.length];
        for(int i=0;i<r.length;i++){
            r[i]=(RegionShape) c[i].clone();
        }
        return r;
    }
}
