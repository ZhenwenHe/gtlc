package gtl.stil.rtree.impl;

import gtl.stil.Identifier;
import gtl.stil.Node;
import gtl.stil.shape.Region;

import java.util.Stack;

/**
 * Created by ZhenwenHe on 2016/12/22.
 */
public class LeafImpl extends NodeImpl {
    @Override
    Node chooseSubtree(Region mbr, int level, Stack<Identifier> pathBuffer) {
        return null;
    }

    @Override
    Node findLeaf(Region mbr, Identifier id, Stack<Identifier> pathBuffer) {
        return null;
    }

    @Override
    void split(byte[] pData, Region mbr, Identifier id, Node left, Node right) {

    }

    void deleteData(Identifier id, Stack<Identifier> pathBuffer){

    }
}
