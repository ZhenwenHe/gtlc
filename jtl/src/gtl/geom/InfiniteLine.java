package gtl.geom;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

/**
 * Created by hadoop on 17-3-26.
 * The primal way to specify a line L is by giving two distinct points, P0 and P1, on it.
 * In fact, this defines a finite line segment S going from P0 to P1 which are the endpoints
 * of S. This is how the Greeks understood straight lines, and it coincides with our natural
 * intuition for the most direct and shortest path between the two endpoints. This line can
 * then be extended indefinitely beyond either endpoint producing infinite rays in both directions.
 * When extended simultaneously beyond both ends, one gets the concept of an infinite line which
 * is how we often think of it today.
 *
 * reference :http://geomalgorithms.com/a02-_lines.html
 * reference :LineSegment
 */
public class InfiniteLine implements gtl.io.Serializable, Comparable<InfiniteLine>{
    Vector startPoint;
    Vector endPoint;

    public InfiniteLine(Vector startPoint, Vector endPoint) {
        this.startPoint = (Vector) startPoint.clone();
        this.endPoint =(Vector)  endPoint.clone();
    }

    public InfiniteLine(double[] startPoint, double[] endPoint) {
        reset(startPoint, endPoint);
    }

    public InfiniteLine( ) {
        this.startPoint = new VectorImpl(0.0,0.0,0.0);
        this.endPoint =new VectorImpl(0.0,0.0,0.0);
    }

    @Override
    public Object clone() {
        return new InfiniteLine(this.startPoint,this.endPoint);
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
        if(i instanceof InfiniteLine){
            this.reset(((InfiniteLine)i).getStartPoint(),((InfiniteLine)i).getEndPoint());
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
    public int compareTo(InfiniteLine o) {
        return 0;
    }
}
