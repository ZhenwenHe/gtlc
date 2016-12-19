package gtl.stil.shape.impl;

import gtl.stil.Interval;
import gtl.stil.IntervalType;
import gtl.stil.shape.TimePoint;
import gtl.stil.shape.TimeShape;

/**
 * Created by ZhenwenHe on 2016/12/22.
 */
public class TimePointImpl extends PointImpl implements TimePoint{
    @Override
    public double getLowerBound() {
        return 0;
    }

    @Override
    public boolean intersectsShapeInTime(TimeShape in) {
        return false;
    }

    @Override
    public double getUpperBound() {
        return 0;
    }

    @Override
    public void setBounds(double l, double u) {

    }

    @Override
    public boolean intersectsShapeInTime(Interval ivI, TimeShape in) {
        return false;
    }

    @Override
    public boolean intersects(Interval i) {
        return false;
    }

    @Override
    public boolean containsShapeInTime(TimeShape in) {
        return false;
    }

    @Override
    public boolean intersects(IntervalType type, double start, double end) {
        return false;
    }

    @Override
    public boolean containsShapeInTime(Interval ivI, TimeShape in) {
        return false;
    }

    @Override
    public boolean contains(Interval i) {
        return false;
    }

    @Override
    public IntervalType getType() {
        return null;
    }

    @Override
    public boolean touchesShapeInTime(TimeShape in) {
        return false;
    }

    @Override
    public void reset(IntervalType type, double start, double end) {

    }

    @Override
    public boolean touchesShapeInTime(Interval ivI, TimeShape in) {
        return false;
    }

    @Override
    public double getAreaInTime() {
        return 0;
    }

    @Override
    public double getAreaInTime(Interval ivI) {
        return 0;
    }

    @Override
    public double getIntersectingAreaInTime(TimeShape r) {
        return 0;
    }

    @Override
    public double getIntersectingAreaInTime(Interval ivI, TimeShape r) {
        return 0;
    }
}
