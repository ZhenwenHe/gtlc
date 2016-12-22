package gtl.stil.rtree.impl;

import gtl.stil.Identifier;
import gtl.stil.Node;
import gtl.stil.shape.Region;

import java.util.Stack;

/**
 * Created by ZhenwenHe on 2016/12/19.
 */
public class IndexImpl extends NodeImpl {


    int findLeastEnlargement( Region r) {
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
    void split(byte[] pData, Region mbr, Identifier id, Node left, Node right) {

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
