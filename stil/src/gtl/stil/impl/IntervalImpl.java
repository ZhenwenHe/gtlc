package gtl.stil.impl;

import gtl.stil.Interval;
import gtl.stil.IntervalType;

/**
 * Created by ZhenwenHe on 2016/12/8.
 */
public class IntervalImpl implements Interval {
    @Override
    public double getLowerBound() {
        return 0;
    }

    @Override
    public double getUpperBound() {
        return 0;
    }

    @Override
    public void setBounds(double l, double u) {

    }

    @Override
    public boolean intersectsInterval(Interval i) {
        return false;
    }

    @Override
    public boolean intersectsInterval(IntervalType type, double start, double end) {
        return false;
    }

    @Override
    public boolean containsInterval(Interval i) {
        return false;
    }

    @Override
    public IntervalType getIntervalType() {
        return null;
    }
}
