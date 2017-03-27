package gtl.geom;

import gtl.geom.EnvelopeImpl;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

/**
 * Created by ZhenwenHe on 2017/3/13.
 */
class TriangleImpl implements Triangle {

    Vector vertices[]=null;//逆时针方向存储点

    public TriangleImpl() {
        this.vertices = new Vector[3];
        for(int i=0;i<3;++i){
            this.vertices[i]=new VectorImpl();
        }
    }

    public TriangleImpl(Vector[] vertices) {
        this.vertices = new Vector[3];
        for(int i=0;i<3;++i){
            this.vertices[i]=(Vector) vertices[i].clone();
        }
    }

    public TriangleImpl(Vector v0, Vector v1, Vector v2) {
        this.vertices = new Vector[3];
        this.vertices[0]=(Vector) v0.clone();
        this.vertices[1]=(Vector) v1.clone();
        this.vertices[2]=(Vector) v2.clone();
    }

    @Override
    public Vector getVertex(int i) {
        return this.vertices[i%3];
    }

    @Override
    public Vector[] getVertices() {
        return this.vertices;
    }

    @Override
    public double getAngle(int i) {
        return this.vertices[i].angle(this.vertices[(i+2)%3],this.vertices[(i+1)%3]);
    }

    @Override
    public double getHeight(int v) {
        return Geom3DSuits.perpendicularDistance(this.vertices[(v+2)%3],this.vertices[(v+1)%3],this.vertices[v]);
    }

    @Override
    public double getPerimeter() {
        return this.getEdgeLength(0,1)+this.getEdgeLength(1,2)+this.getEdgeLength(2,0);
    }

    @Override
    public double getArea() {
        double s = getHeight(0);
        s=s*getEdgeLength(1,2)/2.0;
        return s;
    }

    @Override
    public double getEdgeLength(int s, int e) {
        return this.vertices[e%3].subtract(this.vertices[s%3]).length();
    }

    @Override
    public void copyFrom(Object i) {
        if(i instanceof Triangle){
            Vector[] vv= ((Triangle)i).getVertices();
            int k=0;
            for(Vector v: vv){
                this.vertices[k].reset(v.getCoordinates());
                ++k;
            }
        }
    }

    @Override
    public LineSegment getEdge(int s, int e) {
        return new LineSegment(getVertex(s),getVertex(e));
    }

    @Override
    public int getDimension() {
        return this.vertices[0].getDimension();
    }

    @Override
    public Object clone() {
        return new TriangleImpl(this.vertices);
    }

    @Override
    public boolean load(DataInput in) throws IOException {
        for(Vector v: this.vertices){
            v.load(in);
        }
        return true;
    }

    @Override
    public boolean store(DataOutput out) throws IOException {
        for(Vector v: this.vertices)
            v.store(out);
        return true;
    }

    @Override
    public long getByteArraySize() {
        long len=0;
        for(Vector v: this.vertices)
            len+=v.getByteArraySize();
        return len;
    }

    @Override
    public Envelope getEnvelope() {
        int dims=this.getDimension();
        double [] origin=new double[dims];
        double [] topRight=new double[dims];
        for(int i=0;i<dims;++i){
            origin[i]=this.vertices[0].getOrdinate(i);
            topRight[i]=this.vertices[0].getOrdinate(i);
        }
        for(int c=1;c<3;++c){
            for(int i=0;i<dims;++i){
                origin[i]=Math.min(origin[i],this.vertices[c].getOrdinate(i));
                topRight[i]=Math.max(topRight[i],this.vertices[c].getOrdinate(i));
            }
        }
        return new EnvelopeImpl(origin,topRight);
    }

    @Override
    public Vector getCenter() {
        return this.getCentroid();
    }

    @Override
    public Vector getOrthocenter() {
        //垂线
        return null;
    }

    @Override
    public Vector getCentroid() {
        return null;
    }

    @Override
    public Vector getCircumcenter() {
        return null;
    }

    @Override
    public Vector getIncircleCenter() {
        return null;
    }

    @Override
    public boolean intersects(Envelope e) {
        return false;
    }

    @Override
    public boolean intersects(Triangle e) {
        return false;
    }

    @Override
    public boolean contains(Vector p) {
        return false;
    }

    @Override
    public boolean isEquilateralTriangle() {
        return false;
    }

    @Override
    public boolean isIsoscelesTriangle() {
        return false;
    }

    @Override
    public boolean isRightAngledTriangle() {
        return false;
    }

    @Override
    public boolean isIsoscelesRightTriangle() {
        return false;
    }

    @Override
    public boolean isScaleneTriangle() {
        return false;
    }


}
