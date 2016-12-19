package gtl.stil.impl;

import gtl.stil.Envelope;
import gtl.stil.IndexSuits;
import gtl.stil.Vertex;

import java.io.*;
import java.util.DoubleSummaryStatistics;

/**
 * Created by ZhenwenHe on 2016/12/8.
 */
public class EnvelopeImpl implements Envelope {
    double [] low;
    double [] high;

    public EnvelopeImpl(){
        this.low=null;
        this.high=null;
    }

    public EnvelopeImpl(double [] low, double [] high){
         reset(low,high,Math.min(low.length,high.length));
    }

    @Override
    public boolean read(InputStream in) throws IOException {
        int i=0;
        DataInputStream dis =new DataInputStream(in);
        int dims= dis.readInt();
        this.makeDimension(dims);
        for( i=0;i<dims;i++) {
            this.low[i] = dis.readDouble();
        }
        for( i=0;i<dims;i++) {
            this.high[i] = dis.readDouble();
        }
        return true;
    }

    @Override
    public boolean write(OutputStream out) throws IOException {
        DataOutputStream dos =new DataOutputStream(out);
        int dims = this.getDimension();
        dos.writeInt(dims);
        for(double d:this.low)
            dos.writeDouble(d);
        for(double d:this.high)
            dos.writeDouble(d);
        dos.close();
        return true;
    }

    @Override
    public long getByteArraySize(){
        return getDimension()*8*2+4;
    }

    @Override
    public void makeInfinite(int dimension) {
        makeDimension(dimension);
        for (int cIndex = 0; cIndex < dimension; ++cIndex){
            this.low[cIndex] = Double.MAX_VALUE;
            this.high[cIndex] = -Double.MAX_VALUE;
        }
    }

    @Override
    public void makeDimension(int dimension) {
        if (getDimension() != dimension){
            double [] newdataLow=new double[dimension];
            double [] newdataHigh=new double[dimension];

            int minDims=Math.min(dimension,this.low.length);
            for(int i=0;i<minDims;i++){
                newdataLow[i]=this.low[i];
            }
            this.low=newdataLow;

            minDims=Math.min(dimension,this.high.length);
            for(int i=0;i<minDims;i++){
                newdataHigh[i]=this.high[i];
            }
            this.high=newdataHigh;
        }
    }

    @Override
    public int getDimension() {
        return Math.min(this.low.length,this.high.length);
    }

    @Override
    public double[] getLowCoordinates() {
        return this.low;
    }

    @Override
    public double[] getHighCoordinates() {
        return this.high;
    }

    @Override
    public double getLowCoordinate(int i) {
        return this.low[i];
    }

    @Override
    public double getHighCoordinate(int i) {
        return this.high[i];
    }

    @Override
    public void setLowCoordinate(int i, double d) {
        this.low[i]=d;
    }

    @Override
    public void setHighCoordinate(int i, double d) {
        this.high[i]=d;
    }

    @Override
    public void reset(double[] low, double[] high, int dimension) {
        dimension = Math.min(Math.min(low.length,high.length),dimension);
        this.low=new double [dimension];
        this.high=new double [dimension];
        System.arraycopy(low,0,this.low,0,dimension);
        System.arraycopy(high,0,this.high,0,dimension);
    }

    @Override
    public Object clone() {
        return new EnvelopeImpl(this.low,this.high);
    }

    @Override
    public void copyFrom(Object i) {

        if(i instanceof Envelope){
            Envelope e = (Envelope)i;
            if(this.getDimension()!=e.getDimension()){
                this.reset(e.getLowCoordinates(),e.getHighCoordinates(),e.getDimension());
            }
        }

    }

    @Override
    public void copyTo(Object i) {
        if(i instanceof Envelope){
            Envelope e = (Envelope)i;
            e.reset(this.getLowCoordinates(),
                    this.getHighCoordinates(),
                    this.getDimension());
        }
    }

    @Override
    public void reset(double[] low, double[] high) {
        int dimension = Math.min(low.length,high.length);
        if(getDimension()!=dimension){
            this.low=new double [dimension];
            this.high=new double [dimension];
        }
        System.arraycopy(low,0,this.low,0,dimension);
        System.arraycopy(high,0,this.high,0,dimension);
    }

    public boolean intersects(Envelope e){
        if(e==null) return false;
        int dims = this.getDimension();
        if(dims!=e.getDimension()) return false;

        for (int i = 0; i < dims; ++i) {
            if (this.low[i] > e.getHighCoordinate(i)
                    || this.high[i] < e.getLowCoordinate(i))
                return false;
        }
        return true;
    }
    public boolean contains(Envelope e){
        if(e==null) return false;
        int dims = this.getDimension();
        if(dims!=e.getDimension()) return false;

        for (int i = 0; i < dims; ++i) {
            if (this.low[i] > e.getLowCoordinate(i)
                    || this.high[i] < e.getHighCoordinate(i))
                return false;
        }
        return true;
    }

    /**
     * @param e
     * @return
     */
    public boolean touches(Envelope e){
        if(e==null) return false;
        int dims = this.getDimension();
        if(dims!=e.getDimension()) return false;

        for (int i = 0; i < dims; ++i) {
            if (
                    (
                            this.low[i] >= e.getLowCoordinate(i) + IndexSuits.EPSILON
                                    &&
                            this.low[i] <= e.getLowCoordinate(i) - IndexSuits.EPSILON
                    )
                    ||
                    (
                            this.high[i] >= e.getHighCoordinate(i) + IndexSuits.EPSILON
                                    &&
                            this.high[i] <= e.getHighCoordinate(i) - IndexSuits.EPSILON
                    )
            )
            return false;
        }
        return true;
    }
    public boolean contains(Vertex p){
        if(p==null) return false;
        int dims = this.getDimension();
        if(dims!=p.getDimension()) return false;

        for (int i = 0; i < dims; ++i) {
            if (this.low[i] > p.getCoordinate(i) || this.high[i] < p.getCoordinate(i))
                return false;
        }
        return true;
    }
    public boolean touches(Vertex p){
        if(p==null) return false;
        int dims = this.getDimension();
        if(dims!=p.getDimension()) return false;

        for (int i = 0; i < dims; ++i){
            if (
            (this.low[i] >= p.getCoordinate(i) - IndexSuits.EPSILON &&
             this.low[i] <= p.getCoordinate(i) + IndexSuits.EPSILON) ||
            (this.high[i] >= p.getCoordinate(i) - IndexSuits.EPSILON &&
             this.high[i] <= p.getCoordinate(i) + IndexSuits.EPSILON ))
            return true;
        }
        return false;
    }

    @Override
    public Envelope getIntersectingEnvelope(Envelope e) {
        if(e==null) return null;
        int dims = this.getDimension();
        if(dims!=e.getDimension()) return null;

        EnvelopeImpl ret = new EnvelopeImpl();
        ret.makeInfinite(dims);

        // check for intersection.
        for (int cDim = 0; cDim < dims; ++cDim) {
            if (this.low[cDim] > e.getHighCoordinate(cDim) || this.high[cDim] < e.getLowCoordinate(cDim))
                return ret;
        }

        for (int cDim = 0; cDim < dims; ++cDim) {
            ret.low[cDim] = Math.max(this.low[cDim], e.getLowCoordinate(cDim));
            ret.high[cDim] = Math.min(this.high[cDim], e.getHighCoordinate(cDim));
        }

        return ret;
    }

    @Override
    public double getIntersectingArea(Envelope e) {
        if(e==null) return 0.0;
        int dims = this.getDimension();
        if(dims!=e.getDimension()) return 0.0;

        double ret = 1.0;
        double f1, f2;

        for (int cDim = 0; cDim < dims; ++cDim) {
            if (this.low[cDim] > e.getHighCoordinate(cDim) || this.high[cDim] < e.getLowCoordinate(cDim)) return 0.0;

            f1 = Math.max(this.low[cDim], e.getLowCoordinate(cDim));
            f2 = Math.min(this.high[cDim], e.getHighCoordinate(cDim));
            ret *= f2 - f1;
        }

        return ret;
    }

    /*
     * Returns the margin of a region. It is calculated as the sum of  2^(d-1) * width, in each dimension.
     * It is actually the sum of all edges, no matter what the dimensionality is.
    */
    @Override
    public double getMargin() {
        int dims = this.getDimension();
        double mul = Math.pow(2.0, dims - 1.0);
        double margin = 0.0;

        for (int i = 0; i < dims; ++i) {
            margin += (this.high[i] - this.low[i]) * mul;
        }

        return margin;
    }

    @Override
    public void combine(Envelope e) {
        int dims  = this.getDimension();
        if(e.getDimension()!=dims)
            return ;

        for (int cDim = 0; cDim < dims; ++cDim) {
            this.low[cDim] = Math.min(this.low[cDim], e.getLowCoordinate(cDim));
            this.high[cDim] = Math.max(this.high[cDim], e.getHighCoordinate(cDim));
        }
    }

    @Override
    public void combine(Vertex v) {
        int dims  = this.getDimension();
        if(v.getDimension()!=dims)
            return ;

        for (int cDim = 0; cDim < dims; ++cDim) {
            this.low[cDim] = Math.min(this.low[cDim], v.getCoordinate(cDim));
            this.high[cDim] = Math.max(this.high[cDim], v.getCoordinate(cDim));
        }
    }

    @Override
    public Envelope getCombinedEnvelope(Envelope e) {
        EnvelopeImpl r = (EnvelopeImpl) this.clone();
        r.combine(e);
        return r;
    }
}
