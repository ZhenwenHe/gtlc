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

    public RegionImpl(Envelope e) {
        super(e.getLowCoordinates(), e.getHighCoordinates());
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
    public Vertex getCenter() {
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

        // there may be a more efficient method, but this suffices for now
        Point ll = IndexSuits.createPoint(this.getLowCoordinates());
        Point ur = IndexSuits.createPoint(this.getHighCoordinates());
        // fabricate ul and lr coordinates and points
        Point ul = IndexSuits.createPoint(this.getLowCoordinate(0),this.getHighCoordinate(1));
        Point lr = IndexSuits.createPoint(this.getHighCoordinate(0),this.getLowCoordinate(1));

        // Points/LineSegment for the segment
        Point p1 = IndexSuits.createPoint(e.getStartCoordinates());
        Point p2 = IndexSuits.createPoint(e.getEndCoordinates());


        //Check whether either or both the endpoints are within the region OR
        //whether any of the bounding segments of the Region intersect the segment
        return (this.containsPoint(p1) || this.containsPoint(p2) ||
                e.intersectsShape(IndexSuits.createLineSegment(ll, ul)) || e.intersectsShape(IndexSuits.createLineSegment(ul, ur)) ||
                e.intersectsShape(IndexSuits.createLineSegment(ur, lr)) || e.intersectsShape(IndexSuits.createLineSegment(lr, ll)));
    }
    @Override
    public double getMinimumDistance(Point p) {

        if(p==null) return Double.MAX_VALUE;
        int dims = this.getDimension();
        if(dims!=p.getDimension()) return Double.MAX_VALUE;


        double ret = 0.0;

        for (int i = 0; i < dims; ++i){
            if (p.getCoordinate(i) < this.getLowCoordinate(i))
            {
                ret += Math.pow(this.getLowCoordinate(i) - p.getCoordinate(i), 2.0);
            }
            else if (p.getCoordinate(i) > this.getHighCoordinate(i))
            {
                ret += Math.pow(p.getCoordinate(i) - this.getHighCoordinate(i), 2.0);
            }
        }

        return Math.sqrt(ret);
    }

    @Override
    public Region getIntersectingRegion(Region r) {
        return new RegionImpl(super.getIntersectingEnvelope(r));
    }

    @Override
    public double getIntersectingArea(Region in) {
        return super.getIntersectingArea(in);
    }

    @Override
    public double getMargin() {
        return super.getMargin();
    }

    @Override
    public void combineRegion(Region in) {
        super.combine(in);
    }

    @Override
    public void combinePoint(Point in) {
        super.combine(in);
    }

    @Override
    public Region getCombinedRegion(Region in) {

        RegionImpl r =(RegionImpl) this.clone();
        r.combine(in);
        return r;
    }
}
