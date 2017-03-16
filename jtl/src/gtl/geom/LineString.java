package gtl.geom;

/**
 * Created by hadoop on 17-3-21.
 */
/**
 *  Models an OGC-style <code>LineString</code>.
 *  A LineString consists of a sequence of two or more vertices,
 *  along with all points along the linearly-interpolated curves
 *  (line segments) between each
 *  pair of consecutive vertices.
 *  Consecutive vertices may be equal.
 *  The line segments in the line may intersect each other (in other words,
 *  the linestring may "curl back" in itself and self-intersect.
 *  Linestrings with exactly two identical points are invalid.
 *  <p>
 * A linestring must have either 0 or 2 or more points.
 * If these conditions are not met, the constructors throw
 * an {@link IllegalArgumentException}
 */
public class LineString extends Geometry implements Lineal {
    /**
     *  The points of this <code>LineString</code>.
     */
    protected VertexSequence points;

    @Override
    public Object clone() {
        return null;
    }
}
