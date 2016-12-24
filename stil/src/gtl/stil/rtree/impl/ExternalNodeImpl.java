package gtl.stil.rtree.impl;

import gtl.stil.ExternalNode;
import gtl.stil.Identifier;
import gtl.stil.Node;
import gtl.stil.shape.Region;

import java.util.Stack;

/**
 * Created by ZhenwenHe on 2016/12/22.
 */
public class ExternalNodeImpl extends NodeImpl implements ExternalNode{
    @Override
    Node chooseSubtree(Region mbr, int level, Stack<Identifier> pathBuffer) {
        return null;
    }

    @Override
    Node findLeaf(Region mbr, Identifier id, Stack<Identifier> pathBuffer) {
        return null;
    }

    @Override
    public Node[] split(byte[] pData, Region mbr, Identifier id ) {
        return null;
    }

    void deleteData(Identifier id, Stack<Identifier> pathBuffer){

    }

    @Override
    public Object clone() {
        return null;
    }
}
