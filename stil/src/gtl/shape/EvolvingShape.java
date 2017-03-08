package gtl.shape;

/**
 * Created by ZhenwenHe on 2016/12/8.
 */
public interface EvolvingShape {
    Region getVMBR();
    Region getMBRAtTime(double t) ;
}
