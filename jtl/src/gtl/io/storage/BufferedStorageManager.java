package gtl.io.storage;

import java.io.IOException;

/**
 * 带有缓存的存储管理器
 * Created by ZhenwenHe on 2016/12/8.
 */
public interface BufferedStorageManager extends StorageManager {
    long getHits() ;
    void clear() throws IOException;
}
