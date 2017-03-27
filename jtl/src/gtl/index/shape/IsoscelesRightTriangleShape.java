package gtl.index.shape;

import gtl.geom.Envelope;
import gtl.geom.IsoscelesRightTriangle;

/**
 * Created by ZhenwenHe on 2017/3/27.
 */
public class IsoscelesRightTriangleShape extends IsoscelesRightTriangle implements Shape {
    @Override
    public boolean intersectsShape(Shape in) {
        return false;
    }

    @Override
    public boolean containsShape(Shape in) {
        return false;
    }

    @Override
    public boolean touchesShape(Shape in) {
        return false;
    }

    @Override
    public Envelope getMBR() {
        return null;
    }

    @Override
    public double getMinimumDistance(Shape in) {
        return 0;
    }
}
