package gtl.stil.impl;

import gtl.stil.*;

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
        return false;
    }

    @Override
    public boolean containsShape(Shape in) {
        return false;
    }

    @Override
    public boolean touchesShape(Shape in) {
        return false;
    }

    @Override
    public Point getCenter() {
        return null;
    }

    @Override
    public int getDimension() {
        return 0;
    }

    @Override
    public Envelope getMBR() {
        return null;
    }

    @Override
    public double getArea() {
        return 0;
    }

    @Override
    public double getMinimumDistance(Shape in) {
        return 0;
    }

    @Override
    public Object clone() {
        return new RegionImpl(this.low,this.high);
    }

    @Override
    public boolean intersectsRegion(Region in) {
        return false;
    }

    @Override
    public boolean containsRegion(Region in) {
        return false;
    }

    @Override
    public boolean touchesRegion(Region in) {
        return false;
    }

    @Override
    public double getMinimumDistance(Region in) {
        return 0;
    }

    @Override
    public boolean intersectsLineSegment(LineSegment in) {
        return false;
    }

    @Override
    public boolean containsPoint(Point in) {
        return false;
    }

    @Override
    public boolean touchesPoint(Point in) {
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
