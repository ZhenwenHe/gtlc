package gtl.stil.shape.impl;


import gtl.stil.*;
import gtl.stil.impl.VertexImp;
import gtl.stil.shape.LineSegment;
import gtl.stil.shape.Point;
import gtl.stil.shape.Region;
import gtl.stil.shape.Shape;

import java.io.*;
import java.util.Arrays;

/**
 * Created by ZhenwenHe on 2016/12/8.
 */
public class LineSegmentImpl implements LineSegment {
    double[] startCoordinates;
    double[] endCoordinates;

    public LineSegmentImpl( ) {
        this.startCoordinates = new double [3];
        this.endCoordinates = new double [3];
    }

    public LineSegmentImpl(double[] startPoint, double[] endPoint) {
        reset(startPoint,endPoint);
    }

    public LineSegmentImpl(Vertex startPoint, Vertex endPoint) {
        reset(startPoint.getCoordinates(),endPoint.getCoordinates());
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
    public boolean read(InputStream in) throws IOException {
        int i=0;
        DataInputStream dis =new DataInputStream(in);
        int dims = dis.readInt();
        this.makeDimension(dims);
        for( i=0;i<getDimension();i++) {
            this.startCoordinates[i] = dis.readDouble();
        }
        for( i=0;i<getDimension();i++) {
            this.endCoordinates[i] = dis.readDouble();
        }
        return true;
    }


    @Override
    public boolean write(OutputStream out) throws IOException {
        DataOutputStream dos =new DataOutputStream(out);
        int dims = this.getDimension();
        dos.writeInt(dims);
        for(double d:this.startCoordinates)
            dos.writeDouble(d);
        for(double d:this.endCoordinates)
            dos.writeDouble(d);
        dos.close();
        return true;
    }

    @Override
    public long getByteArraySize() {
        return getDimension()*8*2+4;
    }

    @Override
    public boolean intersectsLineSegment(LineSegment l) {
        assert this.getDimension()==2;

        // use Geometry::intersects
        Point p1 = IndexSuits.createPoint(this.startCoordinates);
        Point p2 = IndexSuits.createPoint(this.endCoordinates);
        Point p3 = IndexSuits.createPoint(l.getStartCoordinates());
        Point p4 = IndexSuits.createPoint(l.getEndCoordinates());
        return LineSegmentImpl.intersects(p1, p2, p3, p4);
    }

    @Override
    public boolean intersectsShape(Shape in) {
        if(in instanceof LineSegment){
            this.intersectsLineSegment((LineSegment)in);
        }
        if(in instanceof Region){
            this.intersectsRegion((Region)in);
        }
        return false;
    }

    @Override
    public boolean intersectsRegion(Region p) {
        assert this.getDimension()==2;
        return p.intersectsLineSegment((this));
    }

    @Override
    public boolean containsShape(Shape in) {
         return false;
    }

    @Override
    public boolean touchesShape(Shape in) {

        return false;
    }

    @Override
    public double getMinimumDistance(Point p) {
        assert this.getDimension()==2;

        if (this.endCoordinates[0] >= this.startCoordinates[0] - IndexSuits.EPSILON &&
                this.endCoordinates[0] <= this.startCoordinates[0] + IndexSuits.EPSILON)
            return Math.abs(p.getCoordinate(0) - this.startCoordinates[0]);

        if (this.endCoordinates[1] >= this.startCoordinates[1] - IndexSuits.EPSILON &&
                this.endCoordinates[1] <= this.startCoordinates[1] + IndexSuits.EPSILON)
            return Math.abs(p.getCoordinate(1) - this.startCoordinates[1]);

        double x1 = this.startCoordinates[0];
        double x2 = this.endCoordinates[0];
        double x0 = p.getCoordinate(0);
        double y1 = this.startCoordinates[1];
        double y2 = this.endCoordinates[1];
        double y0 = p.getCoordinate(1);

        return Math.abs((x2 - x1) * (y1 - y0) - (x1 - x0) * (y2 - y1)) / (Math.sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
    }

    @Override
    public Point getCenter() {
        int dims =this.getDimension();
        double[] coordinates = new double[dims];
        for (int cDim = 0; cDim < dims; ++cDim)
        {
            coordinates[cDim] =
                    (Math.abs(this.startCoordinates[cDim] - this.endCoordinates[cDim]) / 2.0) +
                            Math.min(this.startCoordinates[cDim], this.endCoordinates[cDim]);
        }
        return IndexSuits.createPoint(coordinates);
    }

    @Override
    public double getRelativeMinimumDistance(Point p) {
        assert getDimension()==2;
        
        if (this.endCoordinates[0] >= this.startCoordinates[0] - IndexSuits.EPSILON &&
                this.endCoordinates[0] <= this.startCoordinates[0] + IndexSuits.EPSILON)
        {
            if (this.startCoordinates[1] < this.endCoordinates[1]) return this.startCoordinates[0] - p.getCoordinate(0);
            if (this.startCoordinates[1] >= this.endCoordinates[1]) return p.getCoordinate(0) - this.startCoordinates[0];
        }

        if (this.endCoordinates[1] >= this.startCoordinates[1] - IndexSuits.EPSILON &&
                this.endCoordinates[1] <= this.startCoordinates[1] + IndexSuits.EPSILON)
        {
            if (this.startCoordinates[0] < this.endCoordinates[0]) return p.getCoordinate(1) - this.startCoordinates[1];
            if (this.startCoordinates[0] >= this.endCoordinates[0]) return this.startCoordinates[1] - p.getCoordinate(1);
        }

        double x1 = this.startCoordinates[0];
        double x2 = this.endCoordinates[0];
        double x0 = p.getCoordinate(0);
        double y1 = this.startCoordinates[1];
        double y2 = this.endCoordinates[1];
        double y0 = p.getCoordinate(1);

        return ((x1 - x0) * (y2 - y1) - (x2 - x1) * (y1 - y0)) / (Math.sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
    }



    @Override
    public int getDimension() {
        return Math.min(this.startCoordinates.length,this.endCoordinates.length);
    }

    @Override
    public Envelope getMBR() {
        int dims = this.getDimension();
        double[] low = new double[dims];
        double[] high = new double[dims];
        for (int cDim = 0; cDim < dims; ++cDim){
            low[cDim] = Math.min(this.startCoordinates[cDim], this.endCoordinates[cDim]);
            high[cDim] =  Math.max(this.startCoordinates[cDim], this.endCoordinates[cDim]);
        }

        return IndexSuits.createEnvelope(low,high);
    }

    @Override
    public double getRelativeMaximumDistance(Region r) {
         assert this.getDimension()==2;
        // clockwise.
        double d1 = this.getRelativeMinimumDistance(IndexSuits.createPoint(r.getLowCoordinates()));

        double []  coords=new double [this.getDimension()];
        coords[0] = r.getLowCoordinate(0);
        coords[1] = r.getHighCoordinate(1);
        double d2 = getRelativeMinimumDistance(IndexSuits.createPoint(coords));

        double d3 = getRelativeMinimumDistance(IndexSuits.createPoint(r.getHighCoordinates()));

        coords[0] =r.getHighCoordinate(0);
        coords[1] = r.getLowCoordinates()[1];
        double d4 = getRelativeMinimumDistance(IndexSuits.createPoint(coords));

        return Math.max(d1, Math.max(d2, Math.max(d3, d4)));
    }

    @Override
    public double getArea() {
        return 0;
    }

    @Override
    public double getMinimumDistance(Shape in) {
        if(in instanceof Point) {
            return this.getMinimumDistance((Point)in);
        }
        if(in instanceof Region){
            assert false;

        }
        return Double.MAX_VALUE;
    }

    @Override
    public double getAngleOfPerpendicularRay() {
        assert this.getDimension()==2;
        
        if (this.startCoordinates[0] >= this.endCoordinates[0] - IndexSuits.EPSILON &&
                this.startCoordinates[0] <= this.endCoordinates[0] + IndexSuits.EPSILON) return 0.0;

        if (this.startCoordinates[1] >= this.endCoordinates[1] - IndexSuits.EPSILON &&
                this.startCoordinates[1] <= this.endCoordinates[1] + IndexSuits.EPSILON) return IndexSuits.M_PI_2;

        return Math.atan(-(this.startCoordinates[0] - this.endCoordinates[0]) / (this.startCoordinates[1] - this.endCoordinates[1]));

    }

    @Override
    public Object clone() {
        return new LineSegmentImpl(this.startCoordinates,this.endCoordinates);
    }

    @Override
    public void makeInfinite(int dimension) {
        this.makeDimension(dimension);
        for (int cIndex = 0; cIndex < dimension; ++cIndex) {
            this.startCoordinates[cIndex] = Double.MAX_VALUE;
            this.endCoordinates[cIndex] = Double.MAX_VALUE;
        }
    }

    @Override
    public void makeDimension(int dimension) {
        if(dimension!=this.getDimension()){
            this.startCoordinates = new double[dimension];
            this.endCoordinates = new double[dimension];
        }
    }

    @Override
    public void copyFrom(Object i) {
        if(i instanceof LineSegment){
            this.reset(((LineSegment)i).getStartCoordinates(),((LineSegment)i).getEndCoordinates());
        }
    }

    @Override
    public void copyTo(Object i) {
        if(i instanceof LineSegment){
            ((LineSegment)i).reset(this.startCoordinates,this.endCoordinates);
        }
    }

    @Override
    public double[] getStartCoordinates() {
        return this.startCoordinates;
    }

    @Override
    public double[] getEndCoordinates() {
        return this.endCoordinates;
    }

    @Override
    public double getStartCoordinate(int i) {
        return this.startCoordinates[i];
    }

    @Override
    public double getEndCoordinate(int i) {
        return this.endCoordinates[i];
    }

    @Override
    public void reset(double[] s, double[] e) {
        int dims = Math.min(s.length,e.length);
        if(this.endCoordinates==null || this.startCoordinates==null){
            this.startCoordinates=new double[dims];
            this.endCoordinates=new double[dims];
        }
        else{
            if(this.getDimension()!=dims){
                this.startCoordinates=new double[dims];
                this.endCoordinates=new double[dims];
            }
        }
        System.arraycopy(s,0,this.startCoordinates,0,dims);
        System.arraycopy(e,0,this.endCoordinates,0,dims);
    }

    //some helpers for intersects methods
    protected static double doubleAreaTriangle( Point a,  Point b,  Point c){
		/*
		double *pA, *pB, *pC;
    pA = a.m_pCoords; pB = b.m_pCoords; pC = c.m_pCoords;
    return (((pB[0] - pA[0]) * (pC[1] - pA[1])) - ((pC[0] - pA[0]) * (pB[1] - pA[1])));
	*/
        return 0;
    }
    protected static boolean leftOf( Point a,  Point b,  Point c){
        return (doubleAreaTriangle(a, b, c) > 0);
    }
    protected static boolean collinear( Point a,  Point b,  Point c){
        return (doubleAreaTriangle(a, b, c) == 0);
    }
    protected static boolean between( Point a,  Point b,  Point c){
		/*
		if ( !collinear(a, b, c) ) {
        return false;
    }
    double *pA, *pB, *pC;
    pA = a.m_pCoords; pB = b.m_pCoords; pC = c.m_pCoords;
    if ( pA[0] != pB[0] ) { // a & b are not on the same vertical, compare on x axis
        return  between(pA[0], pB[0], pC[0]);
    } else { // a & b are a vertical segment, we need to compare on y axis
        return between(pA[1], pB[1], pC[1]);
    }
	*/
        return false;
    }
    protected static boolean between(double a, double b, double c){
        return ( ((a <= c) && (c <= b)) || ((a >= c) && (c >= b)) );
    }
    protected static boolean intersectsProper( Point  a,  Point  b,  Point  c,  Point  d){
        /*
        if ( collinear(a, b, c) || collinear(a, b, d) ||
         collinear(c, d, a) || collinear(c, d, b)) {
			return false;
		}

		return ((leftOf(a, b, c) ^ leftOf(a, b, d)) && (leftOf(c, d, a) ^ leftOf(c, d, b)));
		*/
        return false;
    }
    protected static boolean intersects( Point a,  Point b,  Point c,  Point d){
		/*
		if (intersectsProper(a, b, c, d)) {
			return true;
		} 
		else if ( between(a, b, c) || between(a, b, d) ||
				  between(c, d, a) || between(c, d, b) ) { 
			return true;
		}
		else { 
			return false;
		}
		*/        return false;
    }


}
