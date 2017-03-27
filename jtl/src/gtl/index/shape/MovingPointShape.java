package gtl.index.shape;

/**
 * Created by ZhenwenHe on 2016/12/22.
 */
public class MovingPointShape extends TimePointShape implements EvolvingShape {
    @Override
    public RegionShape getVMBR() {
        return null;
    }

    @Override
    public RegionShape getMBRAtTime(double t) {
        return null;
    }
}