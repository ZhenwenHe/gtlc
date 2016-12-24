package gtl.stil.rtree.impl;

import gtl.stil.Identifier;
import gtl.stil.InternalNode;
import gtl.stil.Node;
import gtl.stil.rtree.RTree;
import gtl.stil.shape.Region;

import java.util.Stack;

/**
 * internal node class
 * Created by ZhenwenHe on 2016/12/19.
 */
public class InternalNodeImpl extends NodeImpl implements InternalNode{

    InternalNodeImpl(RTree pTree, Identifier id, int level){

    }
    @Override
    public Object clone() {
        return null;
    }

    int findLeastEnlargement(Region r) {
        return -1;
    }

    int findLeastOverlap( Region r) {
        return -1;
    }

    void adjustTree(NodeImpl n, Stack<Identifier>s){

    }
    void adjustTree(NodeImpl n1, NodeImpl n2, Stack<Identifier> s, byte[] overflowTable){

    }

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

    class OverlapEntry {
        int m_index;
        double m_enlargement;
        Region m_original;
        Region m_combined;
        double m_oa;
        double m_ca;
    }; // OverlapEntry

    static int compareEntries(Object pv1, Object pv2) {
        OverlapEntry pe1 =  (OverlapEntry) pv1;
        OverlapEntry pe2 =  (OverlapEntry) pv2;

        if (pe1.m_enlargement < pe2.m_enlargement) return -1;
        if (pe1.m_enlargement > pe2.m_enlargement) return 1;
        return 0;
    }
}
