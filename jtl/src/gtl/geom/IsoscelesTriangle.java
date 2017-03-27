package gtl.geom;

/**
 * Created by ZhenwenHe on 2017/3/13.
 * V0V1==V0V2
 */
public class IsoscelesTriangle extends TriangleImpl {
    public Triangle leftTriangle(){
        // (V1+V2)/2.0
        Vector m = vertices[1].add(vertices[2]).divide(2.0);
        return new TriangleImpl(vertices[0],vertices[1],m);
    }
    public Triangle rightTriangle(){
        // m=(V1+V2)/2.0
        Vector m = vertices[1].add(vertices[2]).divide(2.0);
        return new TriangleImpl(vertices[0],m,vertices[2]);
    }
}
