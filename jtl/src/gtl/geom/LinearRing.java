package gtl.geom;

/**
 * Created by hadoop on 17-3-21.
 */
/**
 * Models an OGC SFS <code>LinearRing</code>.
 * A <code>LinearRing</code> is a {@link LineString} which is both closed and simple.
 * In other words,
 * the first and last coordinate in the ring must be equal,
 * and the interior of the ring must not self-intersect.
 * Either orientation of the ring is allowed.
 * <p>
 * A ring must have either 0 or 4 or more points.
 * The first and last points must be equal (in 2D).
 * If these conditions are not met, the constructors throw
 * an {@link IllegalArgumentException}
 *
 * @version 1.7
 */
public class LinearRing extends LineString
{
    /**
     * The minimum number of vertices allowed in a valid non-empty ring (= 4).
     * Empty rings with 0 vertices are also valid.
     */
    public static final int MINIMUM_VALID_SIZE = 4;



}
