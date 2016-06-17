package gtl.stil;

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

    public static StorageManager createDiskStorageManager(String baseName, int pageSize, boolean overWrite) throws IOException {
        DiskStorageManager dsm = new DiskStorageManager(baseName,pageSize,overWrite);
        return (StorageManager)dsm;
    }

    public static Identifier createIdentifier(long v){
        IdentifierImpl i = new IdentifierImpl();
        i.setData(v);
        return i ;
    }

    public static StorageManager createMemoryStorageManager() throws IOException{
        return new MemoryStorageManager();
    }

    public static BufferedStorageManager createBufferedStorageManager(StorageManager storageManager, int capacity, boolean writeThrough){
        return new BufferedStorageManagerImpl(storageManager,  capacity,  writeThrough);
    }
}
