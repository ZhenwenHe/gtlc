package gtl.stil;

import gtl.stil.shape.Shape;

import java.util.function.Supplier;

/**
 * Created by ZhenwenHe on 2016/12/7.
 */
public interface Node extends Entry {
    int getChildrenCount() ;
    int getCapacity();
    default  Identifier getChildIdentifier(int index){
        if (index <= this.getChildrenCount())
            return getChildEntry(index).getIdentifier();
        else
            return null;
    }
    default void setChildIdentifier(int index,Identifier id){
        if(id==null) return;
        if (index <= this.getChildrenCount()) {
            Entry e = this.getChildEntry(index);
            assert e!=null;
            e.setIdentifier(id);
        }
    }
    default byte[] getChildData(int index ){
        if (index <= this.getChildrenCount())
            return getChildEntry(index).getData();
        else
            return null;
    }
    default void setChildData(int index,byte[] data){
        if (index <= this.getChildrenCount()) {
            Entry e = this.getChildEntry(index);
            assert e!=null;
            e.setData(data);
        }
    }
    default Shape getChildShape(int index) {
        if (index <= this.getChildrenCount()) {
            Entry e = this.getChildEntry(index);
            assert e!=null;
            return e.getShape();
        }
        return null;
    }
    int getLevel() ;
    default boolean isIndex() {return isInternalNode();}
    default boolean isLeaf() {return  isExternalNode();}
    boolean isInternalNode();
    boolean isExternalNode();

    Entry getChildEntry(int index);
    void insertEntry(Entry e);
    Entry removeEntry(int index);

    //重新计算节点的包围矩形，对于不同的
    Shape recalculateShape();
    /**
     * 对于不同的节点实现，需要重载这个函数生成对应的边界矩形，
     * 例如，如果是RTree的Node，则生产的是Region ;
    */
    Shape newShape( );
}
