package gtl.stil.shape.impl;

import gtl.stil.*;
import gtl.stil.impl.EnvelopeImpl;
import gtl.stil.shape.LineSegment;
import gtl.stil.shape.Point;
import gtl.stil.shape.Region;
import gtl.stil.shape.Shape;

/**
 * Created by ZhenwenHe on 2016/12/8.
 */
public class RegionImpl extends EnvelopeImpl implements Region {

    public RegionImpl() {
        super();
    }

    public RegionImpl(double[] low, double[] high) {
        super(low, high);
    }

    @Override
    public boolean intersectsShape(Shape in) {
        if(in ==null) return false;
        if(in instanceof Region){
            return this.intersectsRegion((Region)in);
        }

        if(in instanceof LineSegment){
            return this.intersectsLineSegment((LineSegment)in);
        }

        if(in instanceof Point){
            return this.containsPoint((Point)in);
        }

        return false;
    }

    @Override
    public boolean containsShape(Shape in) {
        if(in ==null) return false;

        if(in instanceof Region){
            return this.containsRegion((Region)in);
        }

        if(in instanceof Point){
            return this.containsPoint((Point)in);
        }

        return false;
    }

    @Override
    public boolean touchesShape(Shape in) {
        if(in ==null) return false;

        if(in instanceof Region){
            return this.touchesRegion((Region)in);
        }

        if(in instanceof Point){
            return this.touchesPoint((Point)in);
        }

        return false;
    }

    @Override
    public Point getCenter() {
        Point p =IndexSuits.createPoint();

        int dims=this.getDimension();
        p.makeDimension(dims);
        double [] cc = p.getCoordinates();
        for(int i=0;i<dims;i++) {
            cc[i] = (this.getLowCoordinate(i) + this.getHighCoordinate(i)) / 2.0;
        }
        return p;
    }

    @Override
    public int getDimension() {

        return super.getDimension();
    }

    @Override
    public Envelope getMBR() {
        return IndexSuits.createEnvelope(super.getLowCoordinates(),super.getHighCoordinates());
    }

    @Override
    public double getArea() {

        double area = 1.0;
        int dims = this.getDimension();
        for (int i = 0; i < dims; ++i)        {
            area *= (this.getHighCoordinate(i) - this.getLowCoordinate(i));
        }

        return area;
    }

    @Override
    public double getMinimumDistance(Shape in) {

        if(in instanceof Region){
            return this.getMinimumDistance((Region)in);
        }

        if(in instanceof Point){
            return this.getMinimumDistance((Point)in);
        }

        return 0;
    }

    @Override
    public Object clone() {
        return new RegionImpl(this.getLowCoordinates(),this.getHighCoordinates());
    }

    @Override
    public boolean intersectsRegion(Region in) {
        return super.intersects((Envelope)in);
    }

    @Override
    public boolean containsRegion(Region in) {

        return super.contains((Envelope)in);
    }

    @Override
    public boolean touchesRegion(Region in) {

        return super.touches((Envelope)in);
    }

    @Override
    public double getMinimumDistance(Region e) {
        if(e==null) return Double.MAX_VALUE;
        int dims = this.getDimension();
        if(dims!=e.getDimension()) return Double.MAX_VALUE;

        double ret = 0.0;

        for (int i = 0; i < dims; ++i){
            double x = 0.0;

            if (e.getHighCoordinate(i) < this.getLowCoordinate(i)){
                x = Math.abs(e.getHighCoordinate(i) - this.getLowCoordinate(i));
            }
            else if (this.getHighCoordinate(i) < e.getLowCoordinate(i)){
                x = Math.abs(e.getLowCoordinate(i)-this.getHighCoordinate(i));
            }

            ret += x * x;
        }

        return Math.sqrt(ret);
    }



    @Override
    public boolean containsPoint(Point in) {

        return super.contains((Vertex)in);
    }

    @Override
    public boolean touchesPoint(Point in) {

        return super.touches((Vertex)in);
    }

    @Override
    public boolean intersectsLineSegment(LineSegment e) {
        if(e==null) return false;
        int dims = this.getDimension();
        if(dims!=e.getDimension()) return false;

        assert dims==2;

//        // there may be a more efficient method, but this suffices for now
//        Point ll = Point(m_pLow, 2);
//        Point ur = Point(m_pHigh, 2);
//        // fabricate ul and lr coordinates and points
//        double c_ul[2] = {m_pLow[0], m_pHigh[1]};
//        double c_lr[2] = {m_pHigh[0], m_pLow[1]};
//        Point ul = Point(&c_ul[0], 2);
//        Point lr = Point(&c_lr[0], 2);
//
//        // Points/LineSegment for the segment
//        Point p1 = Point(in.m_pStartPoint, 2);
//        Point p2 = Point(in.m_pEndPoint, 2);
//
//
//        //Check whether either or both the endpoints are within the region OR
//        //whether any of the bounding segments of the Region intersect the segment
//        return (containsPoint(p1) || containsPoint(p2) ||
//                in.intersectsShape(LineSegment(ll, ul)) || in.intersectsShape(LineSegment(ul, ur)) ||
//                in.intersectsShape(LineSegment(ur, lr)) || in.intersectsShape(LineSegment(lr, ll)));

        return false;
    }
    @Override
    public double getMinimumDistance(Point in) {
        return 0;
    }

    @Override
    public Region getIntersectingRegion(Region r) {
        return null;
    }

    @Override
    public double getIntersectingArea(Region in) {
        return 0;
    }

    @Override
    public double getMargin() {
        return 0;
    }

    @Override
    public void combineRegion(Region in) {

    }

    @Override
    public void combinePoint(Point in) {

    }

    @Override
    public Region getCombinedRegion(Region in) {
        return null;
    }
}
