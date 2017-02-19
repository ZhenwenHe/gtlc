package gtl.stil;

import gtl.stil.impl.*;
import gtl.stil.shape.LineSegment;
import gtl.stil.shape.Point;
import gtl.stil.shape.Region;
import gtl.stil.shape.impl.LineSegmentImpl;
import gtl.stil.shape.impl.PointImpl;
import gtl.stil.shape.impl.RegionImpl;
import gtl.stil.storage.BufferedStorageManager;
import gtl.stil.storage.StorageManager;
import gtl.stil.storage.impl.DiskStorageManager;
import gtl.stil.storage.impl.MemoryStorageManager;
import gtl.stil.storage.impl.BufferedStorageManagerImpl;

import java.io.*;

/**
 * Created by ZhenwenHe on 2016/12/10.
 */
public class IndexSuits {
    public  static final double EPSILON = 1.0E-308;
    public  static final double M_PI_2 =1.57079632679489661922;

    public static byte [] createByteArray(byte[] ba){
        byte[] r = new byte[ba.length];
        System.arraycopy(ba,0,r,0,r.length);
        return r;
    }
    public static byte [] createByteArray(int size, byte defaultValue){
        byte[] r = new byte[size];
        for(int i=0;i<size;i++)
            r[i]=defaultValue;
        return r;
    }
    public static byte [][] createByteArray(byte[][] baa){
        byte[][] r = new byte[baa.length][];
        for(int i=0;i<baa.length;i++){
            r[i]=new byte[baa[i].length];
            System.arraycopy(baa[i],0,r[i],0,baa[i].length);
        }
        return r;
    }

    public static BufferedStorageManager createBufferedStorageManager(StorageManager storageManager, int capacity, boolean writeThrough){
        return new BufferedStorageManagerImpl(storageManager,  capacity,  writeThrough);
    }
    public static Envelope createEnvelope(){
        return new EnvelopeImpl();
    }
    public static Envelope createEnvelope(double [] low, double [] high){
        return new EnvelopeImpl(low,high);
    }

    public static Identifier createIdentifier(long v){
        return new IdentifierImpl(v);
    }
    public static Identifier[] createIdentifierArray(Identifier[] c ){
        Identifier[] r= new  Identifier[c.length];
        for(int i=0;i<r.length;i++){
            r[i]=(Identifier) c[i].clone();
        }
        return r;
    }

    public static Interval createInterval(IntervalType t, double low, double high ){
        return new IntervalImpl(t,low,high);
    }

    public static PropertySet createPropertySet(){
        return new PropertySetImpl();
    }

    public static LineSegment createLineSegment(Vertex s,Vertex e){
        return new LineSegmentImpl(s,e);
    }
    public static Point createPoint( ){
        return new PointImpl( );
    }
    public static Point createPoint(double[] c ){
        return new PointImpl(c);
    }
    public static Point createPoint(double x,double y){
        return new PointImpl(x,y);
    }
    public static Point[] createPointArray(int size ){
        return new PointImpl[size];
    }

    public static Region createRegion(){
        return new RegionImpl();
    }
    public static Region createRegion(double [] low, double [] high){
        return new RegionImpl(low,high);
    }
    public static Region[] createRegionArray(Region[] c ){
        Region[] r= new  Region[c.length];
        for(int i=0;i<r.length;i++){
            r[i]=(Region) c[i].clone();
        }
        return r;
    }

    public static StorageManager createDiskStorageManager(String baseName, int pageSize, boolean overWrite) throws IOException {
        return new DiskStorageManager(baseName,pageSize,overWrite);
    }

    public static StorageManager createMemoryStorageManager() throws IOException{
        return new MemoryStorageManager();
    }

    public static Vertex createVertex(){
        return new VertexImpl();
    }
    public static Vertex createVertex(double [] v){
        return new VertexImpl(v);
    }
    public static Vertex createVertex(double x,double y){
        return new VertexImpl(x,y);
    }
    public static Vertex createVertex(double x,double y,double z){
        return new VertexImpl(x,y,z);
    }

}
