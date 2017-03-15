package gtl.math.geometry.impl;

import gtl.math.MathSuits;
import gtl.math.geometry.Envelope;
import gtl.math.geometry.Vertex;
import gtl.math.geometry.LineSegment;
import gtl.math.geometry.Triangle;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

/**
 * Created by ZhenwenHe on 2017/3/13.
 */
class TriangleImpl implements Triangle {
    Vertex vertices[]=null;//逆时针方向存储点

    public TriangleImpl() {
        this.vertices = new Vertex[3];
        for(int i=0;i<3;++i){
            this.vertices[i]=new VertexImpl();
        }
    }
    public TriangleImpl(Vertex[] vertices) {
        this.vertices = new Vertex[3];
        for(int i=0;i<3;++i){
            this.vertices[i]=(Vertex) vertices[i].clone();
        }
    }
    public TriangleImpl(Vertex v0, Vertex v1, Vertex v2) {
        this.vertices = new Vertex[3];
        this.vertices[0]=(Vertex) v0.clone();
        this.vertices[1]=(Vertex) v1.clone();
        this.vertices[2]=(Vertex) v2.clone();
    }

    @Override
    public Vertex getVertex(int i) {
        return this.vertices[i%3];
    }

    @Override
    public double getAngle(int i) {
        return this.vertices[i].angle(this.vertices[(i+2)%3],this.vertices[(i+1)%3]);
    }

    @Override
    public double getHeight(int v) {
        return MathSuits.perpendicularDistance(this.vertices[(v+2)%3],this.vertices[(v+1)%3],this.vertices[v]);
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

    }

    @Override
    public LineSegment getEdge(int s, int e) {
        return null;
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
    public int getDimension() {
        return 0;
    }

    @Override
    public boolean isIsoscelesRightTriangle() {
        return false;
    }

    @Override
    public boolean isScaleneTriangle() {
        return false;
    }


    @Override
    public Object clone() {
        return null;
    }

    @Override
    public boolean load(DataInput in) throws IOException {
        return false;
    }

    @Override
    public boolean store(DataOutput out) throws IOException {
        return false;
    }

    @Override
    public long getByteArraySize() {
        return 0;
    }

    @Override
    public Envelope getEnvelope() {
        return null;
    }

    @Override
    public Vertex getCenter() {
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
    public boolean contains(Vertex p) {
        return false;
    }
}
