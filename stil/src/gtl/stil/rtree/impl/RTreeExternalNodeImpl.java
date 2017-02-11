package gtl.stil.rtree.impl;

import gtl.stil.Identifier;
import gtl.stil.Node;
import gtl.stil.shape.Region;

import java.util.Stack;

/**
 * Created by ZhenwenHe on 2017/2/13.
 */
public class RTreeExternalNodeImpl extends RTreeNodeImpl{
    public RTreeExternalNodeImpl(RTreeImpl tree, Identifier identifier, int level, int capacity) {
        super(tree, identifier, level, capacity);
    }

    public RTreeExternalNodeImpl(RTreeImpl tree) {
        super(tree);
    }

    @Override
    public Object clone() {
        return null;
    }

    @Override
    protected Node chooseSubtree(Region mbr, int level, Stack<Identifier> pathBuffer) {
        return null;
    }

    @Override
    protected Node findLeaf(Region mbr, Identifier id, Stack<Identifier> pathBuffer) {
        return null;
    }

    @Override
    protected Node[] split(byte[] pData, Region mbr, Identifier id) {
        return new Node[0];
    }
}
