package gtl.stil;

import gtl.stil.shape.Point;
import gtl.stil.shape.Shape;

/**
 * Created by ZhenwenHe on 2016/12/6.
 */
public interface Indexable {
    void insert(byte[] pData, Shape shape, Identifier shapeIdentifier);
    boolean delete(Shape shape, Identifier shapeIdentifier);
    void contains(Shape query, Visitor  v);
    void intersects(Shape query, Visitor  v);
    void pointLocation(Point query, Visitor v);
    void nearestNeighbor(int k, Shape query, Visitor v, NearestNeighborComparator nnc) ;
    void nearestNeighbor(int k, Shape query, Visitor v);
    void selfJoin(Shape s, Visitor v);
    void queryStrategy(QueryStrategy qs);
    PropertySet getProperties( ) ;
    void addCommand(Command in, CommandType ct);
    boolean isValid();
    Statistics getStatistics( );
}
