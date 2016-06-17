package gtl.stil;

/**
 * Created by ZhenwenHe on 2016/12/7.
 */
public interface Node extends Entry {
    int getChildrenCount() ;
    Identifier getChildIdentifier(int index);
    byte[] getChildData(int index );
    Shape getChildShape(int index) ;
    int getLevel() ;
    boolean isIndex() ;
    boolean isLeaf() ;
}
