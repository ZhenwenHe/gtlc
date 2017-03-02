package gtl.stil;

import gtl.stil.impl.*;
import gtl.stil.rtree.RTree;
import gtl.stil.rtree.impl.RTreeImpl;
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
import java.util.Random;

/**
 * Created by ZhenwenHe on 2016/12/10.
 */
public class IndexSuits {
    public  static final double EPSILON = 1.0E-308;
    public  static final double M_PI_2 =1.57079632679489661922;

    public static BufferedStorageManager createBufferedStorageManager(StorageManager storageManager, int capacity, boolean writeThrough){
        return new BufferedStorageManagerImpl(storageManager,  capacity,  writeThrough);
    }

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

    public static StorageManager createDiskStorageManager(String baseName, int pageSize, boolean overWrite) throws IOException {
        return new DiskStorageManager(baseName,pageSize,overWrite);
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

    public static LineSegment createLineSegment(Vertex s,Vertex e){
        return new LineSegmentImpl(s,e);
    }

    public static StorageManager createMemoryStorageManager() throws IOException{
        return new MemoryStorageManager();
    }

    public static PropertySet createPropertySet(){
        return new PropertySetImpl();
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
    public static RTree createRTree(StorageManager sm , PropertySet ps){
        return new RTreeImpl(sm,ps);
    }


    public static Variant createVariant(){
        return new Variant();
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

    public static RTree loadRTree(StorageManager sm ,Identifier indexIdentifier){
        Variant var=new Variant(indexIdentifier.longValue());
        PropertySet ps=createPropertySet() ;
        ps.put("IndexIdentifier", var);
        return createRTree(sm, ps);
    }

    public static void generateRTreeTestData(){
        //

    }

    /**
     * each interval's range is [0,1.0)
     * @param numb number of the generated intervals
     * @return
     */
    public static Interval[] generateRandomIntervals(int numb){
        double v1, v2,v;
        Interval [] ivs = new Interval[numb];
        Random r = new Random(System.currentTimeMillis());
        for(int i=0;i<numb;i++) {
            v1 = r.nextDouble();
            v2 = r.nextDouble();
            while(v1==v2){
                v2=r.nextDouble();
            }
            if(v1>v2)
                ivs[i]=createInterval(IntervalType.IT_CLOSED,v2,v1);
            else
                ivs[i]=createInterval(IntervalType.IT_CLOSED,v1,v2);
        }
        return ivs;
    }

    /**
     * all vertices generated by this function are in the range [0,1.0)
     * @param numb vertex number
     * @param dim  vertex dimension
     * @return
     */
    public static Vertex[] generateRandomVertices(int numb, int dim){
        double v1, v2,v;
        Vertex [] ivs = new Vertex[numb];
        Random r = new Random(System.currentTimeMillis());
        for(int i=0;i<numb;i++) {
            ivs[i]=createVertex();
            ivs[i].makeDimension(dim);
            for(int k=0;k<dim;k++){
                ivs[i].getCoordinates()[k]=r.nextDouble();
            }
        }
        return ivs;
    }

    /**
     * all envelopes generated by this function are in the range [0,1.0)
     * @param numb
     * @param dim
     * @param minEdgeLength
     * @param maxEdgeLength
     * @return
     */
    public static Envelope[] generateRandomEnvelopes(int numb, int dim, double minEdgeLength,double maxEdgeLength){
        double v1, v2,v;
        Envelope [] ivs = new Envelope[numb];
        double [] originalVertex=new double[dim];
        double [] highVertex=new double[dim];
        double [] delta=new double[dim];//[minEdgeLength,maxEdgeLength]
        Random r = new Random(System.currentTimeMillis());
        for(int i=0;i<numb;i++) {
            for(int k=0;k<dim;k++){
                originalVertex[k]=r.nextDouble();
                delta[k]=r.nextDouble();
                while(delta[k]>maxEdgeLength || delta[k]<minEdgeLength)
                    delta[k]=r.nextDouble();
                highVertex[k]=originalVertex[k]+delta[k];
            }
            ivs[i]=createEnvelope(originalVertex,highVertex);
        }
        return ivs;
    }

    public static boolean writeEnvelopeFile(Envelope[] envelopes, String envelopeFileName){
        try {
            DataOutputStream dos = new DataOutputStream(new FileOutputStream(envelopeFileName));
            dos.writeInt(envelopes.length);
            for(Envelope e: envelopes)
                e.store(dos);
            dos.close();
        }
        catch (Exception e){
            e.printStackTrace();
        }
        return true;
    }
    public static Envelope[] readEnvelopeFile(String envelopeFileName){
        try {
            DataInputStream dis = new DataInputStream(new FileInputStream(envelopeFileName));
            int len = dis.readInt();
            if(len==0) return null;
            Envelope [] envelopes = new Envelope[len];
            for(int i=0;i<len;++i){
                envelopes[i]=createEnvelope();
                envelopes[i].load(dis);
            }
            return envelopes;
        }
        catch (Exception e){
            e.printStackTrace();
            return null;
        }
    }

}
