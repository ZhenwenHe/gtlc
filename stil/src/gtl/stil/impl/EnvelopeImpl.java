package gtl.stil.impl;

import gtl.stil.Envelope;
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
        for( i=0;i<getDimension();i++) {
            this.low[i] = dis.readDouble();
        }
        for( i=0;i<getDimension();i++) {
            this.high[i] = dis.readDouble();
        }
        return true;
    }

    @Override
    public boolean write(OutputStream out) throws IOException {
        DataOutputStream dos =new DataOutputStream(out);
        for(double d:this.low)
            dos.writeDouble(d);
        for(double d:this.high)
            dos.writeDouble(d);
        dos.close();
        return false;
    }

    @Override
    public long getByteArraySize(){
        return getDimension()*8*2;
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

            int minDims=Math.min(newdataLow.length,this.low.length);
            for(int i=0;i<minDims;i++){
                newdataLow[i]=this.low[i];
            }
            this.low=newdataLow;

            minDims=Math.min(newdataHigh.length,this.high.length);
            for(int i=0;i<minDims;i++){
                newdataHigh[i]=this.high[i];
            }
            this.high=newdataHigh;
        }
    }

    @Override
    public int getDimension() {
        return this.low.length;
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
                            this.low[i] >= e.getLowCoordinate(i) + Double.POSITIVE_INFINITY
                                    &&
                            this.low[i] <= e.getLowCoordinate(i) - Double.POSITIVE_INFINITY
                    )
                    ||
                    (
                            this.high[i] >= e.getHighCoordinate(i) + Double.POSITIVE_INFINITY
                                    &&
                            this.high[i] <= e.getHighCoordinate(i) - Double.POSITIVE_INFINITY
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
            (this.low[i] >= p.getCoordinate(i) - Double.POSITIVE_INFINITY &&
             this.low[i] <= p.getCoordinate(i) + Double.POSITIVE_INFINITY) ||
            (this.high[i] >= p.getCoordinate(i) - Double.POSITIVE_INFINITY &&
             this.high[i] <= p.getCoordinate(i) + Double.POSITIVE_INFINITY))
            return true;
        }
        return false;
    }
}
