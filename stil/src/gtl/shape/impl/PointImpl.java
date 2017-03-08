package gtl.shape.impl;


import gtl.math.geometry.Envelope;
import gtl.math.geometry.Vertex;
import gtl.math.geometry.impl.EnvelopeImpl;
import gtl.math.geometry.impl.VertexImpl;
import gtl.shape.Point;
import gtl.shape.Region;
import gtl.shape.Shape;

import java.util.Arrays;

/**
 * Created by ZhenwenHe on 2016/12/7.
 */
public class PointImpl extends VertexImpl implements Point {

    public PointImpl(){
        super();
    }
    public PointImpl(double x ,double y) {
        super(x,y);
    }

    public PointImpl(double x ,double y,double z) {
        super(x,y,z);
    }

    public PointImpl(double[] coordinates) {
        super(coordinates);
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
                "coordinates=" + Arrays.toString(super.getCoordinates()) +
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
        return this;
    }

    @Override
    public int getDimension() {
            return super.getDimension();
    }

    @Override
    public Envelope getMBR() {
        return new EnvelopeImpl(this.getCoordinates(),this.getCoordinates());
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
