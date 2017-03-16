package gtl.geom;

/**
 * Created by hadoop on 17-3-21.
 */
/**
 * Models a collection of {@link Polygon}s.
 * <p>
 * As per the OGC SFS specification,
 * the Polygons in a MultiPolygon may not overlap,
 * and may only touch at single points.
 * This allows the topological point-set semantics
 * to be well-defined.
 *
 *
 *@version 1.7
 */
public class MultiPolygon extends GeometryCollection implements Polygonal {
}



