package gtl.storage;

import gtl.common.Identifier;

import java.io.Closeable;
import java.io.IOException;

/**
 * 存储管理器的接口
 * 其子接口为BufferedStorageManager
 * 其子类有：
 *        DiskStorageManager
 *        MemoryStorageManager
 * Created by ZhenwenHe on 2016/12/8.
 */
public interface StorageManager extends Closeable ,AutoCloseable{
    static final long EMPTY_PAGE= -0x1;
    static final long NEW_PAGE=-0x1;

    byte[] loadByteArray(Identifier page) throws IOException;
    void storeByteArray(Identifier page,byte[] data) throws IOException;
    void deleteByteArray(Identifier page) throws IOException;
    void flush() throws IOException;
}
