package gtl.shape.impl;

import gtl.shape.MovingRegion;
import gtl.shape.Region;

/**
 * Created by ZhenwenHe on 2016/12/22.
 */
public class MovingRegionImpl extends  TimeRegionImpl implements MovingRegion {
    @Override
    public Region getVMBR() {
        return null;
    }

    @Override
    public Region getMBRAtTime(double t) {
        return null;
    }
}
