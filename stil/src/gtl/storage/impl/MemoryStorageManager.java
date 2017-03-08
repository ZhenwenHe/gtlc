package gtl.storage.impl;

import gtl.common.Identifier;
import gtl.storage.StorageManager;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Stack;

/**
 * Created by ZhenwenHe on 2016/12/8.
 */
public class MemoryStorageManager implements StorageManager {

    class Entry{
        byte [] data;
        int length;

        Entry(byte[] d) {
            this.length=d.length;
            this.data = new byte[this.length];
            System.arraycopy(d,0,this.data,0,this.length);
        }
    }; // Entry

    /**
     * 存放Entry集合，下标为页面ID
     */
    ArrayList<MemoryStorageManager.Entry>  buffer;
    /**
     * 存放空页面ID
     */
    Stack<Identifier> emptyPages;

    public MemoryStorageManager() {
        this.buffer = new ArrayList<Entry>();
        this.emptyPages =  new Stack<Identifier> ();
    }

    @Override
    public byte[] loadByteArray(Identifier page) {
        int index  = (int)page.longValue();
        MemoryStorageManager.Entry e = this.buffer.get(index);
        if(e==null) return null;
        byte [] r=null;
        r = new byte[e.length];
        System.arraycopy(e.data,0,r,0,e.length);
        return r;
    }

    @Override
    public void storeByteArray(Identifier page, byte[] data) throws IOException{

        if (page.longValue() == StorageManager.NEW_PAGE ) {
            //如果是新建页
           Entry  e = new Entry(data);
            if (emptyPages.empty()) {
                //如果页面栈为空，将e放到buffer中，
                // 则新建的页面的ID为e在buffer中的下标
                buffer.add(e);
                page.reset( buffer.size() - 1);
            }
            else {
                //如果页面栈不为空，则弹出栈中页面,并将页面ID设置为栈顶元素
                // 则新建的页面的ID为e在buffer中的下标
                page.reset(emptyPages.pop().longValue());
                buffer.set(page.intValue(),e);
            }
        }
        else {
            //如果页面ID不是新建,则获取已有的页面及其数据
            Entry  e_old=this.buffer.get(page.intValue());
            if(e_old==null){
                throw new IOException("MemoryStorageManager.storeByteArray: Invalid Page Exception");
            }
            Entry e = new Entry( data);
            this.buffer.set(page.intValue(),e);
        }
    }

    @Override
    public void deleteByteArray(Identifier page) throws IOException{
        int index = (int)(page.longValue());
        MemoryStorageManager.Entry e = this.buffer.get(index);
        if(e==null) return;
        this.buffer.set(index,null);
        this.emptyPages.push(page);
    }

    @Override
    public void flush() throws IOException{

    }

    @Override
    public void close() throws IOException {
        flush();
    }


}
