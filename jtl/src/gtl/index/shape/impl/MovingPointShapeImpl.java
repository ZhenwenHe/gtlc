package gtl.index.shape.impl;

import gtl.index.shape.MovingPointShape;
import gtl.index.shape.RegionShape;

/**
 * Created by ZhenwenHe on 2016/12/22.
 */
class MovingPointShapeImpl extends TimePointShapeImpl implements MovingPointShape {
    @Override
    public RegionShape getVMBR() {
        return null;
    }

    @Override
    public RegionShape getMBRAtTime(double t) {
        return null;
    }
}
