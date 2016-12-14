package gtl.stil;

import gtl.stil.impl.EnvelopeImpl;
import gtl.stil.impl.VertexImp;
import gtl.stil.shape.Point;
import gtl.stil.shape.impl.PointImpl;
import gtl.stil.storage.BufferedStorageManager;
import gtl.stil.storage.StorageManager;
import gtl.stil.storage.impl.DiskStorageManager;
import gtl.stil.impl.IdentifierImpl;
import gtl.stil.storage.impl.MemoryStorageManager;
import gtl.stil.storage.impl.BufferedStorageManagerImpl;

import java.io.IOException;

/**
 * Created by ZhenwenHe on 2016/12/10.
 */
public class IndexSuits {
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

    public static Point createPoint( ){
        return new PointImpl( );
    }

    public static StorageManager createDiskStorageManager(String baseName, int pageSize, boolean overWrite) throws IOException {
        return new DiskStorageManager(baseName,pageSize,overWrite);
    }

    public static StorageManager createMemoryStorageManager() throws IOException{
        return new MemoryStorageManager();
    }

    public static Vertex createVertex(){
        return new VertexImp();
    }
}
