package gtl.index.shape.impl;

import gtl.index.shape.MovingRegionShape;
import gtl.index.shape.RegionShape;

/**
 * Created by ZhenwenHe on 2016/12/22.
 */
class MovingRegionShapeImpl extends TimeRegionShapeImpl implements MovingRegionShape {
    @Override
    public RegionShape getVMBR() {
        return null;
    }

    @Override
    public RegionShape getMBRAtTime(double t) {
        return null;
    }
}
