package gtl.index;


import gtl.io.Serializable;

/**
 * Created by ZhenwenHe on 2016/12/6.
 */
public interface Visitor extends Serializable {
    void visitNode( Node  in);
    void visitData(Entry in)  ;
    void visitData(Entry[] ev);
}
