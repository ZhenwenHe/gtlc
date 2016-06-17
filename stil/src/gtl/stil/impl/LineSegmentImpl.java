package gtl.stil.impl;


import gtl.stil.*;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * Created by ZhenwenHe on 2016/12/8.
 */
public class LineSegmentImpl implements LineSegment {
    VertexImp startPoint;
    VertexImp endPoint;

    public LineSegmentImpl( ) {
        this.startPoint = null;
        this.endPoint = null;
    }

    public LineSegmentImpl(VertexImp startPoint, VertexImp endPoint) {
        this.startPoint = startPoint;
        this.endPoint = endPoint;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        LineSegmentImpl that = (LineSegmentImpl) o;

        if (!getStartPoint().equals(that.getStartPoint())) return false;
        return getEndPoint().equals(that.getEndPoint());

    }

    @Override
    public int hashCode() {
        int result = getStartPoint().hashCode();
        result = 31 * result + getEndPoint().hashCode();
        return result;
    }

    @Override
    public String toString() {
        return "LineSegmentImpl{" +
                "startPoint=" + startPoint +
                ", endPoint=" + endPoint +
                '}';
    }

    @Override
    public boolean intersectsLineSegment(LineSegment l) {
        return false;
    }

    @Override
    public boolean intersectsShape(Shape in) {
        return false;
    }

    @Override
    public boolean read(InputStream in) throws IOException {
        return false;
    }

    @Override
    public boolean intersectsRegion(Region p) {
        return false;
    }

    @Override
    public boolean containsShape(Shape in) {
        return false;
    }

    @Override
    public boolean write(OutputStream out) throws IOException {
        return false;
    }

    @Override
    public boolean touchesShape(Shape in) {
        return false;
    }

    @Override
    public double getMinimumDistance(Point p) {
        return 0;
    }

    @Override
    public Point getCenter() {
        return null;
    }

    @Override
    public double getRelativeMinimumDistance(Point p) {
        return 0;
    }

    @Override
    public long getByteArraySize() {
        return 0;
    }

    @Override
    public int getDimension() {
        return 0;
    }

    @Override
    public Envelope getMBR() {
        return null;
    }

    @Override
    public double getRelativeMaximumDistance(Region r) {
        return 0;
    }

    @Override
    public double getArea() {
        return 0;
    }

    @Override
    public double getMinimumDistance(Shape in) {
        return 0;
    }

    @Override
    public double getAngleOfPerpendicularRay() {
        return 0;
    }

    @Override
    public Object clone() {
        return null;
    }

    @Override
    public void makeInfinite(int dimension) {

    }

    @Override
    public void makeDimension(int dimension) {

    }

    @Override
    public Vertex getStartPoint() {
        return null;
    }

    @Override
    public Vertex getEndPoint() {
        return null;
    }
}
