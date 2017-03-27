package gtl.index.shape.impl;


import gtl.geom.Envelope;
import gtl.geom.Geom3DSuits;
import gtl.geom.Vector;
import gtl.index.shape.*;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

/**
 * Created by ZhenwenHe on 2016/12/8.
 */
class RegionShapeImpl implements RegionShape {

    Envelope data;

    public RegionShapeImpl() {
        this.data= Geom3DSuits.createEnvelope();
    }

    public RegionShapeImpl(double[] low, double[] high) {
        this.data= Geom3DSuits.createEnvelope(low, high);
    }

    public RegionShapeImpl(Envelope e) {
        this.data= Geom3DSuits.createEnvelope(e.getLowCoordinates(), e.getHighCoordinates());
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof RegionShapeImpl)) return false;

        RegionShapeImpl region = (RegionShapeImpl) o;

        return data.equals(region.data);
    }

    @Override
    public int hashCode() {
        return data.hashCode();
    }

    @Override
    public double getLowCoordinate(int i) {
        return this.data.getLowCoordinate(i);
    }

    @Override
    public double getHighCoordinate(int i) {
        return this.data.getHighCoordinate(i);
    }

    @Override
    public double[] getLowCoordinates() {
        return this.data.getLowCoordinates();
    }

    @Override
    public double[] getHighCoordinates() {
        return this.data.getHighCoordinates();
    }

    @Override
    public void setLowCoordinate(int i, double d) {
        this.data.setLowCoordinate(i,d);
    }

    @Override
    public void setHighCoordinate(int i, double d) {
        this.data.setHighCoordinate(i,d);
    }

    @Override
    public void makeInfinite(int dimension) {
        this.data.makeInfinite(dimension);
    }

    @Override
    public void makeInfinite() {
        this.data.makeInfinite();
    }

    @Override
    public void makeDimension(int dimension) {
        this.data.makeDimension(dimension);
    }

    @Override
    public void reset(double[] low, double[] high, int dimension) {
        this.data.reset(low,high,dimension);
    }

    @Override
    public void reset(double[] low, double[] high) {
        this.data.reset(low,high);
    }

    @Override
    public void copyFrom(Object i) {
        if(i instanceof RegionShape){
            this.data.copyFrom(((RegionShapeImpl)i).data);
        }
        else if(i instanceof Envelope){
            this.data.copyFrom(i);
        }
        else {
            assert false;
        }
    }

    @Override
    public boolean load(DataInput in) throws IOException {
        return this.data.load(in);
    }

    @Override
    public boolean store(DataOutput out) throws IOException {
        return this.data.store(out);
    }

    @Override
    public long getByteArraySize() {
        return this.data.getByteArraySize();
    }

    @Override
    public boolean intersectsShape(Shape in) {
        if(in ==null) return false;
        if(in instanceof RegionShape){
            return this.intersectsRegion((RegionShape)in);
        }

        if(in instanceof LineSegmentShape){
            return this.intersectsLineSegment((LineSegmentShape)in);
        }

        if(in instanceof PointShape){
            return this.containsPoint((PointShape)in);
        }

        return false;
    }

    @Override
    public boolean containsShape(Shape in) {
        if(in ==null) return false;

        if(in instanceof RegionShape){
            return this.containsRegion((RegionShape)in);
        }

        if(in instanceof PointShape){
            return this.containsPoint((PointShape)in);
        }

        return false;
    }

    @Override
    public boolean touchesShape(Shape in) {
        if(in ==null) return false;

        if(in instanceof RegionShape){
            return this.touchesRegion((RegionShape)in);
        }

        if(in instanceof PointShape){
            return this.touchesPoint((PointShape)in);
        }

        return false;
    }

    @Override
    public Vector getCenter() {
        PointShape p = ShapeSuits.createPoint();

        int dims=this.getDimension();
        p.makeDimension(dims);
        double [] cc = p.getCoordinates();
        for(int i=0;i<dims;i++) {
            cc[i] = (this.getLowCoordinate(i) + this.getHighCoordinate(i)) / 2.0;
        }
        return p.getCenter();
    }

    @Override
    public int getDimension() {
        return this.data.getDimension();
    }

    @Override
    public Envelope getMBR() {
        return (Envelope) this.data.clone();
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

        if(in instanceof RegionShape){
            return this.getMinimumDistance((RegionShape)in);
        }

        if(in instanceof PointShape){
            return this.getMinimumDistance((PointShape)in);
        }

        return 0;
    }

    @Override
    public Object clone() {
        return new RegionShapeImpl(this.getLowCoordinates(),this.getHighCoordinates());
    }

    @Override
    public boolean intersectsRegion(RegionShape in) {
        return this.data.intersects(in.getMBR());
    }

    @Override
    public boolean containsRegion(RegionShape in) {
        return this.data.contains(in.getMBR());
    }

    @Override
    public boolean touchesRegion(RegionShape in) {
        return this.data.touches(in.getMBR());
    }

    @Override
    public double getMinimumDistance(RegionShape e) {
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
    public boolean containsPoint(PointShape in) {
        return this.data.contains(in.getCenter());
    }

    @Override
    public boolean touchesPoint(PointShape in) {
        return this.data.touches(in.getCenter());
    }

    @Override
    public boolean intersectsLineSegment(LineSegmentShape e) {
        if(e==null) return false;
        int dims = this.getDimension();
        if(dims!=e.getDimension()) return false;

        assert dims==2;

        // there may be a more efficient method, but this suffices for now
        PointShape ll = ShapeSuits.createPoint(this.getLowCoordinates());
        PointShape ur = ShapeSuits.createPoint(this.getHighCoordinates());
        // fabricate ul and lr coordinates and points
        PointShape ul = ShapeSuits.createPoint(this.getLowCoordinate(0),this.getHighCoordinate(1));
        PointShape lr = ShapeSuits.createPoint(this.getHighCoordinate(0),this.getLowCoordinate(1));

        // Points/LineSegmentShape for the segment
        PointShape p1 = ShapeSuits.createPoint(e.getStartCoordinates());
        PointShape p2 = ShapeSuits.createPoint(e.getEndCoordinates());


        //Check whether either or both the endpoints are within the region OR
        //whether any of the bounding segments of the RegionShape intersect the segment
        return (this.containsPoint(p1) || this.containsPoint(p2) ||
                e.intersectsShape(ShapeSuits.createLineSegment(ll, ul)) || e.intersectsShape(ShapeSuits.createLineSegment(ul, ur)) ||
                e.intersectsShape(ShapeSuits.createLineSegment(ur, lr)) || e.intersectsShape(ShapeSuits.createLineSegment(lr, ll)));
    }
    @Override
    public double getMinimumDistance(PointShape p) {

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
    public RegionShape getIntersectingRegion(RegionShape r) {
        return new RegionShapeImpl(this.data.getIntersectingEnvelope(r.getMBR()));
    }

    @Override
    public double getIntersectingArea(RegionShape in) {
        return this.data.getIntersectingArea(in.getMBR());
    }

    @Override
    public double getMargin() {
        return this.data.getMargin();
    }

    @Override
    public void combineRegion(RegionShape in) {
        this.data.combine(in.getMBR());
    }

    @Override
    public void combinePoint(PointShape in) {
        this.data.combine(in.getCenter());
    }

    @Override
    public RegionShape getCombinedRegion(RegionShape in) {

        RegionShapeImpl r =(RegionShapeImpl) this.clone();
        r.combineRegion(in);
        return r;
    }
}
