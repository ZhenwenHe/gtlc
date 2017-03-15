package gtl.math.geometry;

import gtl.math.geometry.Vertex;
import gtl.math.geometry.LineSegment;
import gtl.shape.Shape;
import gtl.storage.Serializable;

/**
 * Created by ZhenwenHe on 2017/3/13.
 */
public interface Triangle extends Serializable {
    Vertex getVertex(int i);
    double getAngle(int i);
    LineSegment getEdge(int s, int e);
    double getHeight(int v);//"height" is measured at right angles  from vertex(v) to its base edge
    double getPerimeter();
    public double getArea();
    double getEdgeLength(int s, int e);
    int getDimension();
    Envelope getEnvelope();
    Vertex getCenter();

    boolean intersects(Envelope e);
    boolean intersects(Triangle e);
    boolean contains(Vertex p);

    boolean isEquilateralTriangle();
    boolean isIsoscelesTriangle();
    boolean isRightAngledTriangle();
    boolean isIsoscelesRightTriangle();
    boolean isScaleneTriangle();

}
