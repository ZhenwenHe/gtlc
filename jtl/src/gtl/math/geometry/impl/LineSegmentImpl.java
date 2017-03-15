package gtl.math.geometry.impl;

import gtl.math.geometry.LineSegment;
import gtl.math.geometry.Vertex;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

/**
 * Created by ZhenwenHe on 2017/3/14.
 */
class LineSegmentImpl implements LineSegment {
    Vertex startVertex;
    Vertex endVertex;

    public LineSegmentImpl(Vertex startVertex, Vertex endVertex) {
        this.startVertex = (Vertex) startVertex.clone();
        this.endVertex =(Vertex)  endVertex.clone();
    }
    public LineSegmentImpl(double[] startVertex, double[] endVertex) {
        reset(startVertex,endVertex);
    }
    public LineSegmentImpl( ) {
        this.startVertex = new VertexImpl(0.0,0.0,0.0);
        this.endVertex =new VertexImpl(0.0,0.0,0.0);
    }

    @Override
    public Object clone() {
        return new LineSegmentImpl(this.startVertex,this.endVertex);
    }

    @Override
    public void reset(Vertex s, Vertex e) {
        this.startVertex = (Vertex) startVertex.clone();
        this.endVertex =(Vertex)  endVertex.clone();
    }

    @Override
    public void reset(double[] s, double[] e) {
        this.startVertex = new VertexImpl(s);
        this.endVertex =new VertexImpl(e);
    }

    @Override
    public void copyFrom(Object i) {
        if(i == null) return;
        if(i instanceof LineSegment){
            this.reset(((LineSegment)i).getStartVertex(),((LineSegment)i).getEndVertex());
        }
    }

    @Override
    public Vertex getStartVertex() {
        return this.startVertex;
    }

    @Override
    public Vertex getEndVertex() {
        return this.endVertex;
    }

    @Override
    public boolean load(DataInput in) throws IOException {
        this.startVertex.load(in);
        this.endVertex.load(in);
        return true;
    }

    @Override
    public boolean store(DataOutput out) throws IOException {
        this.startVertex.store(out);
        this.endVertex.store(out);
        return true;
    }

    @Override
    public long getByteArraySize() {
        return this.startVertex.getByteArraySize()+this.endVertex.getByteArraySize();
    }
}
