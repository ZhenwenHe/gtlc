package gtl.geom;

/**
 * Created by hadoop on 17-3-21.
 */

/**
 * Represents a polygon with linear edges, which may include holes.
 * The outer boundary (shell)
 * and inner boundaries (holes) of the polygon are represented by {@link LinearRing}s.
 * The boundary rings of the polygon may have any orientation.
 * Polygons are closed, simple geometries by definition.
 * <p>
 * The polygon model conforms to the assertions specified in the
 * <A HREF="http://www.opengis.org/techno/specs.htm">OpenGIS Simple Features
 * Specification for SQL</A>.
 * <p>
 * A <code>Polygon</code> is topologically valid if and only if:
 * <ul>
 * <li>the coordinates which define it are valid coordinates
 * <li>the linear rings for the shell and holes are valid
 * (i.e. are closed and do not self-intersect)
 * <li>holes touch the shell or another hole at at most one point
 * (which implies that the rings of the shell and holes must not cross)
 * <li>the interior of the polygon is connected,
 * or equivalently no sequence of touching holes
 * makes the interior of the polygon disconnected
 * (i.e. effectively split the polygon into two pieces).
 * </ul>
 */
public class Polygon extends Geometry implements Polygonal {

    /**
     *  The exterior boundary,
     * or <code>null</code> if this <code>Polygon</code>
     *  is empty.
     */
    protected LinearRing shell = null;

    /**
     * The interior boundaries, if any.
     * This instance var is never null.
     * If there are no holes, the array is of zero length.
     */
    protected LinearRing[] holes;


    @Override
    public Object clone() {
        return null;
    }
}


