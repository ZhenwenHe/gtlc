package gtl.geom;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

/**
 * Created by ZhenwenHe on 2017/3/13.
 */
public class LineSegment implements gtl.io.Serializable, Comparable<LineSegment>{
    Vector startPoint;
    Vector endPoint;

    public LineSegment(Vector startPoint, Vector endPoint) {
        this.startPoint = (Vector) startPoint.clone();
        this.endPoint =(Vector)  endPoint.clone();
    }

    public LineSegment(double[] startPoint, double[] endPoint) {
        reset(startPoint, endPoint);
    }

    public LineSegment( ) {
        this.startPoint = new VectorImpl(0.0,0.0,0.0);
        this.endPoint =new VectorImpl(0.0,0.0,0.0);
    }

    @Override
    public Object clone() {
        return new LineSegment(this.startPoint,this.endPoint);
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
        if(i instanceof LineSegment){
            this.reset(((LineSegment)i).getStartPoint(),((LineSegment)i).getEndPoint());
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
    public int compareTo(LineSegment o) {
        return 0;
    }

    /**
     * extend to line
     * @return
     */
    public InfiniteLine extend(){
        return new InfiniteLine(startPoint,endPoint);
    }
}
