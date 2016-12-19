package gtl.stil.shape;

import gtl.stil.shape.Region;

/**
 * Created by ZhenwenHe on 2016/12/8.
 */
public interface EvolvingShape {
    Region getVMBR();
    Region getMBRAtTime(double t) ;
}
