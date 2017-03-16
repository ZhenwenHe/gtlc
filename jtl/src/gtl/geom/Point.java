package gtl.geom;

/**
 * Created by hadoop on 17-3-21.
 */
/**
 * Represents a single point.
 *
 * A <code>Point</code> is topologically valid if and only if:
 * <ul>
 * <li>the coordinate which defines it (if any) is a valid coordinate
 * (i.e. does not have an <code>NaN</code> X or Y ordinate)
 * </ul>
 *
 *@version 1.7
 */
public class Point extends Geometry implements Puntal {
    /**
     *  The <code>Vertex</code> wrapped by this <code>Point</code>.
     */
    private VertexSequence coordinates;


    @Override
    public Object clone() {
        return null;
    }
}


