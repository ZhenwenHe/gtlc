package gtl.shape.impl;


import gtl.math.geometry.Envelope;
import gtl.math.geometry.GeometrySuits;
import gtl.math.geometry.Vertex;
import gtl.shape.Point;
import gtl.shape.Region;
import gtl.shape.Shape;
import gtl.shape.ShapeSuits;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.Arrays;

/**
 * Created by ZhenwenHe on 2016/12/7.
 */
class PointImpl implements Point {
    Vertex data;
    public PointImpl(){
        this.data=GeometrySuits.createVertex();
    }
    public PointImpl(double x ,double y) {
        this.data=GeometrySuits.createVertex(x,y);
    }
    public PointImpl(double x ,double y,double z) {
        this.data=GeometrySuits.createVertex(x,y,z);
    }

    public PointImpl(double[] coordinates) {
        this.data=GeometrySuits.createVertex(coordinates);
    }

    @Override
    public double[] getCoordinates() {
        return this.data.getCoordinates();
    }

    @Override
    public double getCoordinate(int i) {
        return this.data.getCoordinate(i);
    }

    @Override
    public void copyFrom(Object i) {
        if(i instanceof Point || i instanceof Vertex){
            this.data.copyFrom(i);
        }
    }

    @Override
    public double getX() {
        return this.data.getX();
    }

    @Override
    public double getY() {
        return this.data.getY();
    }

    @Override
    public double getZ() {
        return this.data.getZ();
    }

    @Override
    public void setX(double x) {
        this.data.setX(x);
    }

    @Override
    public void setY(double y) {
        this.data.setY(y);
    }

    @Override
    public void setZ(double z) {
        this.data.setX(z);
    }

    @Override
    public void makeInfinite(int dimension) {
        this.makeInfinite(dimension);
    }

    @Override
    public void makeDimension(int dimension) {
        this.makeInfinite(dimension);
    }

    @Override
    public void reset(double[] coordinates) {
        this.data.reset(coordinates);
    }


    @Override
    public boolean load(DataInput in) throws IOException {
        return this.data.load(in);
    }

    @Override
    public boolean store(DataOutput out) throws IOException {
        return this.data.store(out);
    }

    @Override
    public Object clone() {
        return new PointImpl(this.getCoordinates());
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof PointImpl)) return false;

        PointImpl point = (PointImpl) o;

        return Arrays.equals(getCoordinates(), point.getCoordinates());

    }

    @Override
    public int hashCode() {
        return Arrays.hashCode(getCoordinates());
    }

    @Override
    public String toString() {
        return "PointImpl{" +
                "coordinates=" + Arrays.toString(this.getCoordinates()) +
                '}';
    }

    @Override
    public boolean intersectsShape(Shape in) {
        if(in instanceof Region){
            return in.containsShape(this);
        }
        else
            return false;
    }

    @Override
    public boolean containsShape(Shape in) {
        return false;
    }

    @Override
    public boolean touchesShape(Shape in) {
        if(in instanceof Point){
            return this.equals(in);
        }
        if(in instanceof Region){
            return in.touchesShape(this);
        }

        return false;
    }

    @Override
    public Vertex getCenter() {
        return (Vertex) this.data.clone();
    }

    @Override
    public int getDimension() {
            return this.data.getDimension();
    }

    @Override
    public Envelope getMBR() {
        return GeometrySuits.createEnvelope(this.getCoordinates(),this.getCoordinates());
    }

    @Override
    public double getArea() {
        return 0;
    }

    @Override
    public double getMinimumDistance(Shape in) {
        if(in instanceof Point){
           return getMinimumDistance((Point)(in));
        }
        if(in instanceof Region){
            in.getMinimumDistance(this);
        }
        return 0;
    }

    @Override
    public long getByteArraySize(){
        return getDimension()*8;
    }

    @Override
    public double getMinimumDistance(Point in) {
        double ret=0;
        double [] coords = in.getCoordinates();
        for (int cDim = 0; cDim < this.getCoordinates().length; ++cDim){
            ret += Math.pow(this.getCoordinates()[cDim] - coords[cDim], 2.0);
        }
        return Math.sqrt(ret);
    }
}
