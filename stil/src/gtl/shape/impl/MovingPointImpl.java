package gtl.shape.impl;

import gtl.shape.MovingPoint;
import gtl.shape.Region;

/**
 * Created by ZhenwenHe on 2016/12/22.
 */
class MovingPointImpl extends TimePointImpl implements  MovingPoint{
    @Override
    public Region getVMBR() {
        return null;
    }

    @Override
    public Region getMBRAtTime(double t) {
        return null;
    }
}
