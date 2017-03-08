package gtl.shape.impl;

import gtl.math.geometry.Interval;
import gtl.math.geometry.IntervalType;
import gtl.shape.TimeRegion;
import gtl.shape.TimeShape;

/**
 * Created by ZhenwenHe on 2016/12/22.
 */
public class TimeRegionImpl extends RegionImpl implements TimeRegion {
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
    public boolean isValid() {
        return false;
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

    @Override
    public boolean lowerClosed() {
        return false;
    }

    @Override
    public boolean upperClosed() {
        return false;
    }

    @Override
    public boolean equals(Interval q) {
        return false;
    }

    @Override
    public boolean starts(Interval q) {
        return false;
    }

    @Override
    public boolean startedBy(Interval q) {
        return false;
    }

    @Override
    public boolean meets(Interval q) {
        return false;
    }

    @Override
    public boolean metBy(Interval q) {
        return false;
    }

    @Override
    public boolean finishes(Interval q) {
        return false;
    }

    @Override
    public boolean finishedBy(Interval q) {
        return false;
    }

    @Override
    public boolean before(Interval q) {
        return false;
    }

    @Override
    public boolean after(Interval q) {
        return false;
    }

    @Override
    public boolean overlaps(Interval q) {
        return false;
    }

    @Override
    public boolean overlappedBy(Interval q) {
        return false;
    }

    @Override
    public boolean during(Interval q) {
        return false;
    }

    @Override
    public boolean covers(Interval q) {
        return false;
    }

    @Override
    public boolean coveredBy(Interval q) {
        return false;
    }
}
