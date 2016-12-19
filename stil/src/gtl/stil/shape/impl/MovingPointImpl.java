package gtl.stil.shape.impl;

import gtl.stil.shape.MovingPoint;
import gtl.stil.shape.Region;
import gtl.stil.shape.TimePoint;

/**
 * Created by ZhenwenHe on 2016/12/22.
 */
public class MovingPointImpl extends TimePointImpl implements  MovingPoint{
    @Override
    public Region getVMBR() {
        return null;
    }

    @Override
    public Region getMBRAtTime(double t) {
        return null;
    }
}
