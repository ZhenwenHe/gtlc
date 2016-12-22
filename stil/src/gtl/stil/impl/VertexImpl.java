package gtl.stil.impl;

import gtl.stil.Vertex;

import java.io.*;
import java.util.Arrays;

/**
 * Created by ZhenwenHe on 2016/12/8.
 */
public class VertexImpl implements Vertex {

    double  [] coordinates;

    public VertexImpl( ) {
        this.coordinates = new double[3];
    }

    public VertexImpl(double x , double y) {
        this.coordinates = new double[2];
        this.coordinates[0]=x;
        this.coordinates[1]=y;
    }

    public VertexImpl(double x , double y, double z) {
        this.coordinates = new double[3];
        this.coordinates[0]=x;
        this.coordinates[1]=y;
        this.coordinates[2]=z;
    }

    public VertexImpl(double[] coordinates) {
        this.coordinates=new double[coordinates.length];
        System.arraycopy(coordinates,0,this.coordinates,0,coordinates.length);
    }

    public double[] getCoordinates() {
        return coordinates;
    }

    public void setCoordinates(double[] coordinates) {
        this.coordinates = coordinates;
    }

    @Override
    public Object clone() {
        double [] td = new double [this.coordinates.length];
        System.arraycopy(this.coordinates,0,td,0,td.length);
        return new VertexImpl(td);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof VertexImpl)) return false;

        VertexImpl point = (VertexImpl) o;

        return Arrays.equals(getCoordinates(), point.getCoordinates());

    }

    @Override
    public int hashCode() {
        return Arrays.hashCode(getCoordinates());
    }

    @Override
    public String toString() {
        return "VertexImpl{" +
                "coordinates=" + Arrays.toString(coordinates) +
                '}';
    }

    @Override
    public int getDimension() {
        return this.coordinates.length;
    }

    @Override
    public double getX() {
        return this.coordinates[0];
    }

    @Override
    public double getY() {
         return this.coordinates[1];
    }

    @Override
    public double getZ() {
        return this.coordinates[2];
    }

    @Override
    public boolean read(InputStream in) throws IOException {
        DataInputStream dis =new DataInputStream(in);
        int dims = dis.readInt();
        this.makeDimension(dims);
        for(int i=0;i<dims;i++) {
            this.coordinates[i] = dis.readDouble();
        }
        dis.close();
        return true;
    }

    @Override
    public boolean write(OutputStream out) throws IOException {
        DataOutputStream dos =new DataOutputStream(out);
        int dims = this.getDimension();
        dos.writeInt(dims);
        for(double d:this.coordinates)
            dos.writeDouble(d);
        dos.close();
        return true;
    }
    @Override
    public long getByteArraySize(){
        return getDimension()*8+4;
    }


    @Override
    public void setX(double x) {
        this.coordinates[0]=x;
    }

    @Override
    public void setY(double y) {
        this.coordinates[1]=y;
    }

    @Override
    public void setZ(double z) {
        this.coordinates[2]=z;
    }

    @Override
    public void makeInfinite(int dimension) {
        makeDimension(dimension);
        for (int cIndex = 0; cIndex < this.coordinates.length; ++cIndex){
            this.coordinates[cIndex] =Double.MAX_VALUE;
        }
    }

    @Override
    public void makeDimension(int dimension) {
        if (this.getDimension() != dimension){
            double [] newData=new double[dimension];
            int minDims=Math.min(newData.length,this.coordinates.length);
            for(int i=0;i<minDims;i++){
                newData[i]=this.coordinates[i];
            }
            this.coordinates=newData;
        }
    }

    @Override
    public double getCoordinate(int i) {
        return this.coordinates[i];
    }

    @Override
    public void copyFrom(Object i) {
        if(i instanceof Vertex){
            if(((Vertex)i).getCoordinates().length==this.coordinates.length){
                System.arraycopy(
                        ((Vertex)i).getCoordinates(), 0,
                        this.coordinates,0,this.coordinates.length);
            }
            else{
                this.coordinates=new double[((Vertex)i).getCoordinates().length];
                System.arraycopy(
                        ((Vertex)i).getCoordinates(), 0,
                        this.coordinates,0,this.coordinates.length);
            }
        }
    }

    @Override
    public void copyTo(Object i) {
        if(i instanceof Vertex){
            if(((Vertex)i).getCoordinates().length==this.coordinates.length){
                System.arraycopy(
                        this.coordinates, 0,
                        ((Vertex)i).getCoordinates(),0,this.coordinates.length);
            }
            else{
                ((Vertex)i).reset(this.coordinates);
            }
        }
    }

    public void reset(double [] coordinates){
        if(this.coordinates.length==coordinates.length){
            System.arraycopy(this.coordinates,0,
                    coordinates,0,coordinates.length);
        }
        else{
            this.coordinates=new double [coordinates.length];
            System.arraycopy(this.coordinates,0,
                    coordinates,0,coordinates.length);
        }
    }
}
