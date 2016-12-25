package gtl.stil.shape.impl;

import gtl.stil.Interval;
import gtl.stil.IntervalType;
import gtl.stil.shape.TimePoint;
import gtl.stil.shape.TimeShape;

/**
 * Created by ZhenwenHe on 2016/12/22.
 */
public class TimePointImpl extends PointImpl implements TimePoint{
    protected  double startTime;
    protected double endTime;

    @Override
    public double getLowerBound() {
        return startTime;
    }
    @Override
    public double getUpperBound() {

        return endTime;
    }

    @Override
    public boolean intersectsShapeInTime(TimeShape in) {
        return false;
    }


    @Override
    public void setBounds(double l, double u) {
        this.startTime=l;
        this.endTime=u;
    }

    @Override
    public boolean intersectsShapeInTime(Interval ivI, TimeShape in) {
        return false;
    }

    @Override
    public boolean intersects(Interval i) {
        if(i==null) return false;
        return intersects(i.getType(),i.getLowerBound(),i.getUpperBound());
    }

    @Override
    public boolean containsShapeInTime(TimeShape in) {
        return false;
    }

    @Override
    public boolean intersects(IntervalType type, double start, double end) {
        if (startTime >= end || endTime <= start)
            return false;
        return true;
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
