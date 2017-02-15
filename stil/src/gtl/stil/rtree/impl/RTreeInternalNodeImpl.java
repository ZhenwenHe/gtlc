package gtl.stil.rtree.impl;
import gtl.stil.Entry;
import gtl.stil.Identifier;
import gtl.stil.Node;
import gtl.stil.shape.Region;
import java.util.Stack;

/**
 * Created by ZhenwenHe on 2017/2/13.
 */
public class RTreeInternalNodeImpl extends RTreeNodeImpl  {
    public RTreeInternalNodeImpl(RTreeImpl tree, Identifier identifier, int level) {
        super(tree, identifier, level, tree.indexCapacity);
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
    protected Node[] split(Entry e) {
        return new Node[0];
    }

    public void adjustTree(Node n, Stack<Identifier> pathBuffer){

    }
    public void adjustTree(Node n, Node nn, Stack<Identifier> pathBuffer, byte[] overflowTable){

    }

    protected int findLeastEnlargement(Region r) {
        return -1;
    }
    protected int findLeastOverlap(Region r) {
        return -1;
    }
}
