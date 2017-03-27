package gtl.geom;

/**
 * Created by hadoop on 17-3-21.
 */

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;

/**
 * Models a collection of {@link Geometry}s of
 * arbitrary type and dimension.
 *
 *
 */
public class GeometryCollection extends Geometry implements Collection<Geometry> {
    /**
     *  Internal representation of this <code>GeometryCollection</code>.
     */
    ArrayList<Geometry> geometries;

    public GeometryCollection(ArrayList<Geometry> geometries) {
        this.geometries = new ArrayList<Geometry>(geometries.size());
        int i=0;
        for(Geometry g: geometries){
            this.geometries.set(i,g);
            i++;
        }
        this.geometryType=Geometry.GEOMETRYCOLLECTION;
    }
    public GeometryCollection(Geometry[] geometries) {
        this.geometries = new ArrayList<Geometry>(geometries.length);
        int i=0;
        for(Geometry g: geometries){
            this.geometries.set(i,g);
            i++;
        }
        this.geometryType=Geometry.GEOMETRYCOLLECTION;
    }
    public GeometryCollection() {
        this.geometries = new ArrayList<Geometry>();
        this.geometryType=Geometry.GEOMETRYCOLLECTION;
    }

    @Override
    public int size() {
        return this.geometries.size();
    }

    @Override
    public boolean isEmpty() {
        return this.geometries.isEmpty();
    }

    @Override
    public boolean contains(Object o) {
        return this.geometries.contains(o);
    }

    @Override
    public Iterator<Geometry> iterator() {
        return this.geometries.iterator();
    }

    @Override
    public Object[] toArray() {
        return this.geometries.toArray();
    }

    @Override
    public <T> T[] toArray(T[] a) {
        return this.geometries.toArray(a);
    }

    @Override
    public boolean add(Geometry geometry) {
        return this.geometries.add(geometry);
    }

    @Override
    public boolean remove(Object o) {
        return this.geometries.remove(o);
    }

    @Override
    public boolean containsAll(Collection<?> c) {
        return this.geometries.containsAll(c);
    }

    @Override
    public boolean addAll(Collection<? extends Geometry> c) {
        return this.geometries.addAll(c);
    }

    @Override
    public boolean removeAll(Collection<?> c) {
        return this.geometries.removeAll(c);
    }

    @Override
    public boolean retainAll(Collection<?> c) {
        return this.geometries.retainAll(c);
    }

    @Override
    public void clear() {
        this.geometries.clear();
    }

    @Override
    public void copyFrom(Object i) {
        if(i instanceof  GeometryCollection ){
            GeometryCollection g = (GeometryCollection)i;
            super.copyFrom(i);
            this.geometries.clear();
            for(Geometry g1: g.geometries){
                this.geometries.add((Geometry) g1.clone());
            }
        }
    }

    @Override
    public boolean load(DataInput in) throws IOException {
        super.load(in);
        int c= in.readInt();
        this.geometries.clear();
        for(int i=0;i<c;++i){
            //the first four bytes store geometry type
            int gt = in.readInt();
            Geometry g = Geometry.create(gt);
            g.load(in);
            this.geometries.add(g);
        }
        return true;
    }

    @Override
    public boolean store(DataOutput out) throws IOException {
        super.store(out);
        out.writeInt(this.geometries.size());
        for(Geometry g: this.geometries){
            out.writeInt(g.geometryType);
            g.store(out);
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
        len+=4;
        for(Geometry g: this.geometries){
            len+=4;
            len+=g.getByteArraySize();
        }
        return len;
    }

    @Override
    public Object clone(){
        GeometryCollection gc = new GeometryCollection();
        gc.copyFrom(this);
        return gc;
    }


}

