package gtl.geom;

/**
 * Created by hadoop on 17-3-24.
 */
public class Vector2D extends VectorImpl {
    public Vector2D() {
    }

    public Vector2D(double x, double y) {
        super(x, y);
    }

    public Vector2D(double []ca, int beginPosition){
        super(ca,beginPosition,2);
    }
}
