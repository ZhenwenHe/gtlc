package gtl.geom;

/**
 * Created by hadoop on 17-3-20.
 */

import gtl.exception.NotRepresentableException;


class HCoordinate {

    /**
     * Computes the (approximate) intersection point between two line segments
     * using homogeneous coordinates.
     * <p>
     * Note that this algorithm is
     * not numerically stable; i.e. it can produce intersection points which
     * lie outside the envelope2D of the line segments themselves.  In order
     * to increase the precision of the calculation input points should be normalized
     * before passing them to this routine.
     */
    public static Vertex intersection(
            Vertex p1, Vertex p2,
            Vertex q1, Vertex q2)
            throws NotRepresentableException
    {
        // unrolled computation
        double px = p1.y - p2.y;
        double py = p2.x - p1.x;
        double pw = p1.x * p2.y - p2.x * p1.y;

        double qx = q1.y - q2.y;
        double qy = q2.x - q1.x;
        double qw = q1.x * q2.y - q2.x * q1.y;

        double x = py * qw - qy * pw;
        double y = qx * pw - px * qw;
        double w = px * qy - qx * py;

        double xInt = x/w;
        double yInt = y/w;

        if ((Double.isNaN(xInt)) || (Double.isInfinite(xInt)
                || Double.isNaN(yInt)) || (Double.isInfinite(yInt))) {
            throw new NotRepresentableException();
        }

        return Geom2DSuits.createVertex(xInt, yInt);
    }

  /*
  public static Vertex OLDintersection(
      Vertex p1, Vertex p2,
      Vertex q1, Vertex q2)
      throws NotRepresentableException
  {
    HCoordinate l1 = new HCoordinate(p1, p2);
    HCoordinate l2 = new HCoordinate(q1, q2);
    HCoordinate intHCoord = new HCoordinate(l1, l2);
    Vertex intPt = intHCoord.getCoordinate();
    return intPt;
  }
  */

    public double x,y,w;

    public HCoordinate() {
        x = 0.0;
        y = 0.0;
        w = 1.0;
    }

    public HCoordinate(double _x, double _y, double _w) {
        x = _x;
        y = _y;
        w = _w;
    }

    public HCoordinate(double _x, double _y) {
        x = _x;
        y = _y;
        w = 1.0;
    }

    public HCoordinate(Vertex p) {
        x = p.x;
        y = p.y;
        w = 1.0;
    }

    public HCoordinate(HCoordinate p1, HCoordinate p2)
    {
        x = p1.y * p2.w - p2.y * p1.w;
        y = p2.x * p1.w - p1.x * p2.w;
        w = p1.x * p2.y - p2.x * p1.y;
    }

    /**
     * Constructs a homogeneous coordinate which is the intersection of the lines
     * define by the homogenous coordinates represented by two
     * {@link Vertex}s.
     *
     * @param p1
     * @param p2
     */
    public HCoordinate(Vertex p1, Vertex p2)
    {
        // optimization when it is known that w = 1
        x = p1.y - p2.y;
        y = p2.x - p1.x;
        w = p1.x * p2.y - p2.x * p1.y;
    }

    public HCoordinate(Vertex p1, Vertex p2, Vertex q1, Vertex q2)
    {
        // unrolled computation
        double px = p1.y - p2.y;
        double py = p2.x - p1.x;
        double pw = p1.x * p2.y - p2.x * p1.y;

        double qx = q1.y - q2.y;
        double qy = q2.x - q1.x;
        double qw = q1.x * q2.y - q2.x * q1.y;

        x = py * qw - qy * pw;
        y = qx * pw - px * qw;
        w = px * qy - qx * py;
    }

    public double getX() throws NotRepresentableException {
        double a = x/w;
        if ((Double.isNaN(a)) || (Double.isInfinite(a))) {
            throw new NotRepresentableException();
        }
        return a;
    }

    public double getY() throws NotRepresentableException {
        double a = y/w;
        if  ((Double.isNaN(a)) || (Double.isInfinite(a))) {
            throw new NotRepresentableException();
        }
        return a;
    }

    public Vertex getCoordinate() throws NotRepresentableException {
        Vertex p = Geom2DSuits.createVertex();
        p.x = getX();
        p.y = getY();
        return p;
    }
}