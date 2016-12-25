package gtl.stil;

import gtl.stil.shape.Shape;

/**
 * Created by ZhenwenHe on 2016/12/7.
 */
public interface Node extends Entry {
    int getChildrenCount() ;
    Identifier getChildIdentifier(int index);
    void setChildIdentifier(int index,Identifier id);
    byte[] getChildData(int index );
    void setChildData(int index,byte[] data);
    Shape getChildShape(int index) ;
    int getLevel() ;
    boolean isIndex() ;
    boolean isLeaf() ;
    boolean isInternalNode();
    boolean isExternalNode();
}
