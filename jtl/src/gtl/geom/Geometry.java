package gtl.geom;

/**
 * Created by hadoop on 17-3-21.
 */

import gtl.geom.EnvelopeImpl;

import java.io.*;

public abstract class Geometry implements  gtl.io.Serializable
{

    static final int POINT = 0;
    static final int MULTIPOINT = 1;
    static final int LINESTRING = 2;
    static final int LINEARRING = 3;
    static final int MULTILINESTRING = 4;
    static final int POLYGON = 5;
    static final int MULTIPOLYGON = 6;
    static final int GEOMETRYCOLLECTION = 7;


    int geometryType;

    /**
     *  The bounding box of this <code>Geometry</code>.
     */
    protected Envelope envelope;


    /**
     *  The ID of the Spatial Reference System used by this <code>Geometry</code>
     */
    protected int SRID;

    /**
     * An object reference which can be used to carry ancillary data defined
     * by the client.
     * If this object needs to be storage, please implement gtl.io.Serializable
     * otherwise it will not be storage with this geometry.
     */
    protected Object userData;
    /**
     * if userDataSize =0 , the userData does not need to storage
     * else the userDataSize id the bytearray length of userData
     * in this case, the userData must implement the interface gtl.o.Serializable
     */
    protected int userDataSize;


    public Geometry() {
        this.envelope = new EnvelopeImpl();
        this.SRID=8037;
        this.userData=null;
        this.userDataSize=0;
    }

    public Envelope getEnvelope() {
        return this.envelope;
    }

    public void setEnvelope(Envelope envelope) {
        if(envelope!=null)
            this.envelope.copyFrom(envelope);
    }

    public int getSRID() {
        return SRID;
    }

    public void setSRID(int SRID) {
        this.SRID = SRID;
    }

    public Object getUserData() {
        return userData;
    }

    public void setUserData(Object userData) {
        this.userData = userData;
        this.userDataSize=0;
    }

    /**
     *
     * @param userData
     * @param storageFlag if the value is true, the userData will be stored with Geometry
     *                    in this case ,
     */
    public void setUserData(gtl.io.Serializable userData,boolean storageFlag) {
        if(storageFlag && userData!=null) {
            this.userData = userData;
            this.userDataSize =(int) userData.getByteArraySize();
        }
        else {
            this.userDataSize = 0;
            this.userData=userData;
        }

    }

    @Override
    public void copyFrom(Object i) {
        if(i instanceof  Geometry){
            Geometry g = (Geometry)i;
            this.userData=g.userData;
            this.userDataSize=g.userDataSize;
            this.SRID=g.SRID;
            this.envelope.copyFrom(g.envelope);
        }
    }

    @Override
    public boolean load(DataInput in) throws IOException {
        this.geometryType=in.readInt();
        this.SRID=in.readInt();
        this.envelope.load(in);
        this.userDataSize = in.readInt();
        if(this.userDataSize>0) { //this.userData!=null
            gtl.io.Serializable s = (gtl.io.Serializable)(this.userData);
            s.load(in);
        }
        return true;
    }

    @Override
    public boolean store(DataOutput out) throws IOException {
        out.writeInt(this.geometryType);
        out.writeInt(this.SRID);
        this.envelope.store(out);
        out.writeInt(this.userDataSize);
        if(this.userDataSize>0) {
            byte [] tmp = new byte[this.userDataSize];
            gtl.io.Serializable s = (gtl.io.Serializable)(this.userData);
        }
        return true;
    }

    @Override
    public long getByteArraySize() {
        long len = 4+this.envelope.getByteArraySize();
        if(this.userData !=null && this.userData instanceof gtl.io.Serializable){
            gtl.io.Serializable s = (gtl.io.Serializable) this.userData;
            len += s.getByteArraySize();
        }
        return len;
    }

    @Override
    public abstract Object clone();

    public static  Geometry create(int gtype){
        return null;
    }
}
