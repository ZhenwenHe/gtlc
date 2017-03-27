package gtl.geom;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

/**
 * Created by hadoop on 17-3-27.
 * Line and Ray and Segment with defining  points {Point P0, P1;}
 *  (a Line is infinite, Rays and  Segments start at P0)
 *  (a Ray extends beyond P1, but a  Segment ends at P1)
 *  ref:http://geomalgorithms.com/a06-_intersect-2.html
 */
public class Ray implements gtl.io.Serializable, Comparable<Ray>{
    Vector startPoint;
    Vector endPoint;

    public Ray(Vector startPoint, Vector endPoint) {
        this.startPoint = (Vector) startPoint.clone();
        this.endPoint =(Vector)  endPoint.clone();
    }

    public Ray(double[] startPoint, double[] endPoint) {
        reset(startPoint, endPoint);
    }

    public Ray( ) {
        this.startPoint = new VectorImpl(0.0,0.0,0.0);
        this.endPoint =new VectorImpl(0.0,0.0,0.0);
    }

    @Override
    public Object clone() {
        return new Ray(this.startPoint,this.endPoint);
    }


    public void reset(Vector s, Vector e) {
        this.startPoint = (Vector) startPoint.clone();
        this.endPoint =(Vector)  endPoint.clone();
    }


    public void reset(double[] s, double[] e) {
        this.startPoint = new VectorImpl(s);
        this.endPoint =new VectorImpl(e);
    }

    @Override
    public void copyFrom(Object i) {
        if(i == null) return;
        if(i instanceof Ray){
            this.reset(((Ray)i).getStartPoint(),((Ray)i).getEndPoint());
        }
    }


    public Vector getStartPoint() {
        return this.startPoint;
    }


    public Vector getEndPoint() {
        return this.endPoint;
    }

    @Override
    public boolean load(DataInput in) throws IOException {
        this.startPoint.load(in);
        this.endPoint.load(in);
        return true;
    }

    @Override
    public boolean store(DataOutput out) throws IOException {
        this.startPoint.store(out);
        this.endPoint.store(out);
        return true;
    }

    @Override
    public long getByteArraySize() {
        return this.startPoint.getByteArraySize()+this.endPoint.getByteArraySize();
    }

    @Override
    public int compareTo(Ray o) {
        return 0;
    }

    public InfiniteLine extend(){
        return new InfiniteLine(startPoint,endPoint);
    }
}
