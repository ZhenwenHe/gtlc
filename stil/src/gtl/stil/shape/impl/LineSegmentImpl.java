package gtl.stil.shape.impl;


import gtl.stil.*;
import gtl.stil.impl.VertexImp;
import gtl.stil.shape.LineSegment;
import gtl.stil.shape.Point;
import gtl.stil.shape.Region;
import gtl.stil.shape.Shape;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Arrays;

/**
 * Created by ZhenwenHe on 2016/12/8.
 */
public class LineSegmentImpl implements LineSegment {
    double[] startCoordinates;
    double[] endCoordinates;

    public LineSegmentImpl( ) {
        this.startCoordinates = null;
        this.endCoordinates = null;
    }

    public LineSegmentImpl(double[] startPoint, double[] endPoint) {

    }

    public LineSegmentImpl(Vertex startPoint, Vertex endPoint) {

    }


    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof LineSegmentImpl)) return false;

        LineSegmentImpl that = (LineSegmentImpl) o;

        if (!Arrays.equals(startCoordinates, that.startCoordinates)) return false;
        return Arrays.equals(endCoordinates, that.endCoordinates);

    }

    @Override
    public int hashCode() {
        int result = Arrays.hashCode(startCoordinates);
        result = 31 * result + Arrays.hashCode(endCoordinates);
        return result;
    }

    @Override
    public String toString() {
        return "LineSegmentImpl{" +
                "startCoordinates=" + Arrays.toString(startCoordinates) +
                ", endCoordinates=" + Arrays.toString(endCoordinates) +
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
    public void copyFrom(Object i) {

    }

    @Override
    public void copyTo(Object i) {

    }

    @Override
    public double[] getStartCoordinates() {
        return new double[0];
    }

    @Override
    public double[] getEndCoordinates() {
        return new double[0];
    }

    @Override
    public Double getStartCoordinate(int i) {
        return null;
    }

    @Override
    public Double getEndCoordinate(int i) {
        return null;
    }
}
