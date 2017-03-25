package gtl.geom;

import gtl.math.MathSuits;
import gtl.math.Float128;

/**
 * Created by hadoop on 17-3-20.
 */
public class Geom2DSuits extends GeomSuits {
    /**
     * A value that indicates an orientation of clockwise, or a right turn.
     */
    public static final int CLOCKWISE = -1;

    /**
     * A value that indicates an orientation of clockwise, or a right turn.
     */
    public static final int RIGHT = CLOCKWISE;

    /**
     * A value that indicates an orientation of counterclockwise, or a left turn.
     */
    public static final int COUNTERCLOCKWISE = 1;

    /**
     * A value that indicates an orientation of counterclockwise, or a left turn.
     */
    public static final int LEFT = COUNTERCLOCKWISE;

    /**
     * A value that indicates an orientation of collinear, or no turn (straight).
     */
    public static final int COLLINEAR = 0;

    /**
     * A value that indicates an orientation of collinear, or no turn (straight).
     */
    public static final int STRAIGHT = COLLINEAR;




    /**
     * Returns the index of the direction of the point <code>q</code> relative to
     * a vector specified by <code>p1-p2</code>.
     *
     * @param p1 the origin point of the vector
     * @param p2 the final point of the vector
     * @param q the point to compute the direction to
     *
     * @return 1 if q is counter-clockwise (left) from p1-p2
     * @return -1 if q is clockwise (right) from p1-p2
     * @return 0 if q is collinear with p1-p2
     */
    public static int orientationIndex(Vertex2D p1, Vertex2D p2, Vertex2D q)
    {
        /**
         * MD - 9 Aug 2010 It seems that the basic algorithm is slightly orientation
         * dependent, when computing the orientation of a point very close to a
         * line. This is possibly due to the arithmetic in the translation to the
         * origin.
         *
         * For instance, the following situation produces identical results in spite
         * of the inverse orientation of the line segment:
         *
         * Vertex2D p0 = Geom2DSuits.createVertex(219.3649559090992, 140.84159161824724);
         * Vertex2D p1 = Geom2DSuits.createVertex(168.9018919682399, -5.713787599646864);
         *
         * Vertex2D p = Geom2DSuits.createVertex(186.80814046338352, 46.28973405831556); int
         * orient = orientationIndex(p0, p1, p); int orientInv =
         * orientationIndex(p1, p0, p);
         *
         * A way to force consistent results is to normalize the orientation of the
         * vector using the following code. However, this may make the results of
         * orientationIndex inconsistent through the triangle of points, so it's not
         * clear this is an appropriate patch.
         *
         */
        return Geom2DSuits.orientationIndex2d(p1, p2, q);
        // testing only
        //return ShewchuksDeterminant.orientationIndex(p1, p2, q);
        // previous implementation - not quite fully robust
        //return RobustDeterminant2D.orientationIndex(p1, p2, q);

    }

    /**
     * Tests whether a point lies inside or on a ring. The ring may be oriented in
     * either direction. A point lying exactly on the ring boundary is considered
     * to be inside the ring.
     * <p>
     * This method does <i>not</i> first check the point against the envelope2D of
     * the ring.
     *
     * @param p
     *          point to check for ring inclusion
     * @param ring
     *          an array of coordinates representing the ring (which must have
     *          first point identical to last point)
     * @return true if p is inside ring
     *
     * @see locatePointInRing
     */
    public static boolean isPointInRing(Vertex2D p, Vertex2D[] ring)
    {
        return locatePointInRing(p, ring) != Location.EXTERIOR;
    }

    /**
     * Determines whether a point lies in the interior, on the boundary, or in the
     * exterior of a ring. The ring may be oriented in either direction.
     * <p>
     * This method does <i>not</i> first check the point against the envelope2D of
     * the ring.
     *
     * @param p
     *          point to check for ring inclusion
     * @param ring
     *          an array of coordinates representing the ring (which must have
     *          first point identical to last point)
     * @return the {@link Location} of p relative to the ring
     */
    public static int locatePointInRing(Vertex2D p, Vertex2D[] ring)
    {
        return RayCrossingCounter2D.locatePointInRing(p, ring);
    }

    /**
     * Tests whether a point lies on the line segments defined by a list of
     * coordinates.
     *
     * @return true if the point is a vertex of the line or lies in the interior
     *         of a line segment in the linestring
     */
    public static boolean isOnLine(Vertex2D p, Vertex2D[] pt)
    {
        LineIntersector2D lineIntersector2D = new RobustLineIntersector2D();
        for (int i = 1; i < pt.length; i++) {
            Vertex2D p0 = pt[i - 1];
            Vertex2D p1 = pt[i];
            lineIntersector2D.computeIntersection(p, p0, p1);
            if (lineIntersector2D.hasIntersection()) {
                return true;
            }
        }
        return false;
    }

    /**
     * Computes whether a ring defined by an array of {@link Vertex2D}s is
     * oriented counter-clockwise.
     * <ul>
     * <li>The list of points is assumed to have the first and last points equal.
     * <li>This will handle coordinate lists which contain repeated points.
     * </ul>
     * This algorithm is <b>only</b> guaranteed to work with valid rings. If the
     * ring is invalid (e.g. self-crosses or touches), the computed result may not
     * be correct.
     *
     * @param ring
     *          an array of Coordinates forming a ring
     * @return true if the ring is oriented counter-clockwise.
     * @throws IllegalArgumentException
     *           if there are too few points to determine orientation (&lt; 4)
     */
    public static boolean isCCW(Vertex2D[] ring)
    {
        // # of points without closing endpoint
        int nPts = ring.length - 1;
        // sanity check
        if (nPts < 3)
            throw new IllegalArgumentException(
                    "Ring has fewer than 4 points, so orientation cannot be determined");

        // find highest point
        Vertex2D hiPt = ring[0];
        int hiIndex = 0;
        for (int i = 1; i <= nPts; i++) {
            Vertex2D p = ring[i];
            if (p.y > hiPt.y) {
                hiPt = p;
                hiIndex = i;
            }
        }

        // find distinct point before highest point
        int iPrev = hiIndex;
        do {
            iPrev = iPrev - 1;
            if (iPrev < 0)
                iPrev = nPts;
        } while (ring[iPrev].equals(hiPt) && iPrev != hiIndex);

        // find distinct point after highest point
        int iNext = hiIndex;
        do {
            iNext = (iNext + 1) % nPts;
        } while (ring[iNext].equals(hiPt) && iNext != hiIndex);

        Vertex2D prev = ring[iPrev];
        Vertex2D next = ring[iNext];

        /**
         * This check catches cases where the ring contains an A-B-A configuration
         * of points. This can happen if the ring does not contain 3 distinct points
         * (including the case where the input array has fewer than 4 elements), or
         * it contains coincident line segments.
         */
        if (prev.equals(hiPt) || next.equals(hiPt) || prev.equals(next))
            return false;

        int disc = computeOrientation(prev, hiPt, next);

        /**
         * If disc is exactly 0, lines are collinear. There are two possible cases:
         * (1) the lines lie along the x axis in opposite directions (2) the lines
         * lie on top of one another
         *
         * (1) is handled by checking if next is left of prev ==> CCW (2) will never
         * happen if the ring is valid, so don't check for it (Might want to assert
         * this)
         */
        boolean isCCW;
        if (disc == 0) {
            // poly is CCW if prev x is right of next x
            isCCW = (prev.x > next.x);
        }
        else {
            // if area is positive, points are ordered CCW
            isCCW = (disc > 0);
        }
        return isCCW;
    }

    /**
     * Computes the orientation of a point q to the directed line segment p1-p2.
     * The orientation of a point relative to a directed line segment indicates
     * which way you turn to get to q after travelling from p1 to p2.
     *
     * @param p1 the first vertex of the line segment
     * @param p2 the second vertex of the line segment
     * @param q the point to compute the relative orientation of
     * @return 1 if q is counter-clockwise from p1-p2,
     * or -1 if q is clockwise from p1-p2,
     * or 0 if q is collinear with p1-p2
     */
    public static int computeOrientation(Vertex2D p1, Vertex2D p2,
                                         Vertex2D q)
    {
        return orientationIndex(p1, p2, q);
    }

    /**
     * Computes the distance2D from a point p to a line segment AB
     *
     * Note: NON-ROBUST!
     *
     * @param p
     *          the point to compute the distance2D for
     * @param A
     *          one point of the line
     * @param B
     *          another point of the line (must be different to A)
     * @return the distance2D from p to line segment AB
     */
    public static double distancePointLine(Vertex2D p, Vertex2D A,
                                           Vertex2D B)
    {
        // if start = end, then just compute distance2D to one of the endpoints
        if (A.x == B.x && A.y == B.y)
            return p.distance(A);

        // otherwise use comp.graphics.algorithms Frequently Asked Questions method
    /*
     * (1) r = AC dot AB
     *         ---------
     *         ||AB||^2
     *
     * r has the following meaning:
     *   r=0 P = A
     *   r=1 P = B
     *   r<0 P is on the backward extension of AB
     *   r>1 P is on the forward extension of AB
     *   0<r<1 P is interior to AB
     */

        double len2 = (B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y);
        double r = ((p.x - A.x) * (B.x - A.x) + (p.y - A.y) * (B.y - A.y))
                / len2;

        if (r <= 0.0)
            return p.distance(A);
        if (r >= 1.0)
            return p.distance(B);

    /*
     * (2) s = (Ay-Cy)(Bx-Ax)-(Ax-Cx)(By-Ay)
     *         -----------------------------
     *                    L^2
     *
     * Then the distance2D from C to P = |s|*L.
     *
     * This is the same calculation as {@link #distancePointLinePerpendicular}.
     * Unrolled here for performance.
     */
        double s = ((A.y - p.y) * (B.x - A.x) - (A.x - p.x) * (B.y - A.y))
                / len2;
        return Math.abs(s) * Math.sqrt(len2);
    }

    /**
     * Computes the perpendicular distance2D from a point p to the (infinite) line
     * containing the points AB
     *
     * @param p
     *          the point to compute the distance2D for
     * @param A
     *          one point of the line
     * @param B
     *          another point of the line (must be different to A)
     * @return the distance2D from p to line AB
     */
    public static double distancePointLinePerpendicular(Vertex2D p,
                                                        Vertex2D A, Vertex2D B)
    {
        // use comp.graphics.algorithms Frequently Asked Questions method
    /*
     * (2) s = (Ay-Cy)(Bx-Ax)-(Ax-Cx)(By-Ay)
     *         -----------------------------
     *                    L^2
     *
     * Then the distance2D from C to P = |s|*L.
     */
        double len2 = (B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y);
        double s = ((A.y - p.y) * (B.x - A.x) - (A.x - p.x) * (B.y - A.y))
                / len2;

        return Math.abs(s) * Math.sqrt(len2);
    }

    /**
     * Computes the distance2D from a point to a sequence of line segments.
     *
     * @param p
     *          a point
     * @param line
     *          a sequence of contiguous line segments defined by their vertices
     * @return the minimum distance2D between the point and the line segments
     */
    public static double distancePointLine(Vertex2D p, Vertex2D[] line)
    {
        if (line.length == 0)
            throw new IllegalArgumentException(
                    "Line array must contain at least one vertex");
        // this handles the case of length = 1
        double minDistance = p.distance(line[0]);
        for (int i = 0; i < line.length - 1; i++) {
            double dist = distancePointLine(p, line[i], line[i + 1]);
            if (dist < minDistance) {
                minDistance = dist;
            }
        }
        return minDistance;
    }

    /**
     * Computes the distance2D from a line segment AB to a line segment CD
     *
     * Note: NON-ROBUST!
     *
     * @param A
     *          a point of one line
     * @param B
     *          the second point of (must be different to A)
     * @param C
     *          one point of the line
     * @param D
     *          another point of the line (must be different to A)
     */
    public static double distanceLineLine(Vertex2D A, Vertex2D B,
                                          Vertex2D C, Vertex2D D)
    {
        // check for zero-length segments
        if (A.equals(B))
            return distancePointLine(A, C, D);
        if (C.equals(D))
            return distancePointLine(D, A, B);

        // AB and CD are line segments
        /*
         * from comp.graphics.algo
         *
         * Solving the above for r and s yields
         *
         *     (Ay-Cy)(Dx-Cx)-(Ax-Cx)(Dy-Cy)
         * r = ----------------------------- (eqn 1)
         *     (Bx-Ax)(Dy-Cy)-(By-Ay)(Dx-Cx)
         *
         *     (Ay-Cy)(Bx-Ax)-(Ax-Cx)(By-Ay)
         * s = ----------------------------- (eqn 2)
         *     (Bx-Ax)(Dy-Cy)-(By-Ay)(Dx-Cx)
         *
         * Let P be the position vector of the
         * intersection point, then
         *   P=A+r(B-A) or
         *   Px=Ax+r(Bx-Ax)
         *   Py=Ay+r(By-Ay)
         * By examining the values of r & s, you can also determine some other limiting
         * conditions:
         *   If 0<=r<=1 & 0<=s<=1, intersection exists
         *      r<0 or r>1 or s<0 or s>1 line segments do not intersect
         *   If the denominator in eqn 1 is zero, AB & CD are parallel
         *   If the numerator in eqn 1 is also zero, AB & CD are collinear.
         */

        boolean noIntersection = false;
        if (! Envelope2D.intersects(A, B, C, D)) {
            noIntersection = true;
        }
        else {
            double denom = (B.x - A.x) * (D.y - C.y) - (B.y - A.y) * (D.x - C.x);

            if (denom == 0) {
                noIntersection = true;
            }
            else {
                double r_num = (A.y - C.y) * (D.x - C.x) - (A.x - C.x) * (D.y - C.y);
                double s_num = (A.y - C.y) * (B.x - A.x) - (A.x - C.x) * (B.y - A.y);

                double s = s_num / denom;
                double r = r_num / denom;

                if ((r < 0) || (r > 1) || (s < 0) || (s > 1)) {
                    noIntersection = true;
                }
            }
        }
        if (noIntersection) {
            return MathSuits.min(
                    distancePointLine(A, C, D),
                    distancePointLine(B, C, D),
                    distancePointLine(C, A, B),
                    distancePointLine(D, A, B));
        }
        // segments intersect
        return 0.0;
    }

    /**
     * Computes the signed area for a ring. The signed area is positive if the
     * ring is oriented CW, negative if the ring is oriented CCW, and zero if the
     * ring is degenerate or flat.
     *
     * @param ring
     *          the coordinates forming the ring
     * @return the signed area of the ring
     */
    public static double signedArea(Vertex2D[] ring)
    {
        if (ring.length < 3)
            return 0.0;
        double sum = 0.0;
        /**
         * Based on the Shoelace formula.
         * http://en.wikipedia.org/wiki/Shoelace_formula
         */
        double x0 = ring[0].x;
        for (int i = 1; i < ring.length - 1; i++) {
            double x = ring[i].x - x0;
            double y1 = ring[i + 1].y;
            double y2 = ring[i - 1].y;
            sum += x * (y2 - y1);
        }
        return sum / 2.0;
    }

    /**
     * Computes the signed area for a ring. The signed area is:
     * <ul>
     * <li>positive if the ring is oriented CW
     * <li>negative if the ring is oriented CCW
     * <li>zero if the ring is degenerate or flat
     * </ul>
     *
     * @param ring
     *          the coordinates forming the ring
     * @return the signed area of the ring
     */
    public static double signedArea(VertexSequence ring)
    {
        int n = ring.size();
        if (n < 3)
            return 0.0;
        /**
         * Based on the Shoelace formula.
         * http://en.wikipedia.org/wiki/Shoelace_formula
         */
        Vertex2D p0 = Geom2DSuits.createVertex2D();
        Vertex2D p1 = Geom2DSuits.createVertex2D();
        Vertex2D p2 = Geom2DSuits.createVertex2D();
        ring.getCoordinate(0, p1);
        ring.getCoordinate(1, p2);
        double x0 = p1.x;
        p2.x -= x0;
        double sum = 0.0;
        for (int i = 1; i < n - 1; i++) {
            p0.y = p1.y;
            p1.x = p2.x;
            p1.y = p2.y;
            ring.getCoordinate(i + 1, p2);
            p2.x -= x0;
            sum += p1.x * (p0.y - p2.y);
        }
        return sum / 2.0;
    }

    /**
     * Computes the length of a linestring specified by a sequence of points.
     *
     * @param pts
     *          the points specifying the linestring
     * @return the length of the linestring
     */
    public static double length(VertexSequence pts)
    {
        // optimized for processing VertexSequences
        int n = pts.size();
        if (n <= 1)
            return 0.0;

        double len = 0.0;

        Vertex2D p = Geom2DSuits.createVertex2D();
        pts.getCoordinate(0, p);
        double x0 = p.x;
        double y0 = p.y;

        for (int i = 1; i < n; i++) {
            pts.getCoordinate(i, p);
            double x1 = p.x;
            double y1 = p.y;
            double dx = x1 - x0;
            double dy = y1 - y0;

            len += Math.sqrt(dx * dx + dy * dy);

            x0 = x1;
            y0 = y1;
        }
        return len;
    }

    /**
     * Returns the index of the direction of the point <code>q</code> relative to
     * a vector specified by <code>p1-p2</code>.
     *
     * @param p1 the origin point of the vector
     * @param p2 the final point of the vector
     * @param q the point to compute the direction to
     *
     * @return 1 if q is counter-clockwise (left) from p1-p2
     * @return -1 if q is clockwise (right) from p1-p2
     * @return 0 if q is collinear with p1-p2
     */
    public static int orientationIndex2d(Vertex2D p1, Vertex2D p2, Vertex2D q)
    {
        // fast filter for orientation index
        // avoids use of slow extended-precision arithmetic in many cases
        int index = orientationIndexFilter2d(p1, p2, q);
        if (index <= 1) return index;

        // normalize coordinates
        Float128 dx1 = Float128.valueOf(p2.x).selfAdd(-p1.x);
        Float128 dy1 = Float128.valueOf(p2.y).selfAdd(-p1.y);
        Float128 dx2 = Float128.valueOf(q.x).selfAdd(-p2.x);
        Float128 dy2 = Float128.valueOf(q.y).selfAdd(-p2.y);

        // sign of determinant - unrolled for performance
        return dx1.selfMultiply(dy2).selfSubtract(dy1.selfMultiply(dx2)).signum();
    }

    /**
     * Computes the sign of the determinant of the 2x2 matrix
     * with the given entries.
     *
     * @return -1 if the determinant is negative,
     * @return  1 if the determinant is positive,
     * @return  0 if the determinant is 0.
     */
    public static int signOfDet2x2(Float128 x1, Float128 y1, Float128 x2, Float128 y2)
    {
        Float128 det = x1.multiply(y2).selfSubtract(y1.multiply(x2));
        return det.signum();
    }

    /**
     * A value which is safely greater than the
     * relative round-off error in double-precision numbers
     */
    private static final double DP_SAFE_EPSILON = 1e-15;

    /**
     * A filter for computing the orientation index of three coordinates.
     * <p>
     * If the orientation can be computed safely using standard DP
     * arithmetic, this routine returns the orientation index.
     * Otherwise, a value i > 1 is returned.
     * In this case the orientation index must
     * be computed using some other more robust method.
     * The filter is fast to compute, so can be used to
     * avoid the use of slower robust methods except when they are really needed,
     * thus providing better average performance.
     * <p>
     * Uses an approach due to Jonathan Shewchuk, which is in the public domain.
     *
     * @param pa a coordinate
     * @param pb a coordinate
     * @param pc a coordinate
     * @return the orientation index if it can be computed safely
     * @return i > 1 if the orientation index cannot be computed safely
     */
    private static int orientationIndexFilter2d(Vertex2D pa, Vertex2D pb, Vertex2D pc)
    {
        double detsum;

        double detleft = (pa.x - pc.x) * (pb.y - pc.y);
        double detright = (pa.y - pc.y) * (pb.x - pc.x);
        double det = detleft - detright;

        if (detleft > 0.0) {
            if (detright <= 0.0) {
                return signum(det);
            }
            else {
                detsum = detleft + detright;
            }
        }
        else if (detleft < 0.0) {
            if (detright >= 0.0) {
                return signum(det);
            }
            else {
                detsum = -detleft - detright;
            }
        }
        else {
            return signum(det);
        }

        double errbound = DP_SAFE_EPSILON * detsum;
        if ((det >= errbound) || (-det >= errbound)) {
            return signum(det);
        }

        return 2;
    }

    private static int signum(double x)
    {
        if (x > 0) return 1;
        if (x < 0) return -1;
        return 0;
    }

    /**
     * Computes an intersection point between two lines
     * using Float128 arithmetic.
     * Currently does not handle case of parallel lines.
     *
     * @param p1
     * @param p2
     * @param q1
     * @param q2
     * @return
     */
    public static Vertex2D intersection2d(
            Vertex2D p1, Vertex2D p2,
            Vertex2D q1, Vertex2D q2)
    {
        Float128 denom1 = Float128.valueOf(q2.y).selfSubtract(q1.y)
                .selfMultiply(Float128.valueOf(p2.x).selfSubtract(p1.x));
        Float128 denom2 = Float128.valueOf(q2.x).selfSubtract(q1.x)
                .selfMultiply(Float128.valueOf(p2.y).selfSubtract(p1.y));
        Float128 denom = denom1.subtract(denom2);

        /**
         * Cases:
         * - denom is 0 if lines are parallel
         * - intersection point lies within line segment p if fracP is between 0 and 1
         * - intersection point lies within line segment q if fracQ is between 0 and 1
         */

        Float128 numx1 = Float128.valueOf(q2.x).selfSubtract(q1.x)
                .selfMultiply(Float128.valueOf(p1.y).selfSubtract(q1.y));
        Float128 numx2 = Float128.valueOf(q2.y).selfSubtract(q1.y)
                .selfMultiply(Float128.valueOf(p1.x).selfSubtract(q1.x));
        Float128 numx = numx1.subtract(numx2);
        double fracP = numx.selfDivide(denom).doubleValue();

        double x = Float128.valueOf(p1.x).selfAdd(Float128.valueOf(p2.x).selfSubtract(p1.x).selfMultiply(fracP)).doubleValue();

        Float128 numy1 = Float128.valueOf(p2.x).selfSubtract(p1.x)
                .selfMultiply(Float128.valueOf(p1.y).selfSubtract(q1.y));
        Float128 numy2 = Float128.valueOf(p2.y).selfSubtract(p1.y)
                .selfMultiply(Float128.valueOf(p1.x).selfSubtract(q1.x));
        Float128 numy = numy1.subtract(numy2);
        double fracQ = numy.selfDivide(denom).doubleValue();

        double y = Float128.valueOf(q1.y).selfAdd(Float128.valueOf(q2.y).selfSubtract(q1.y).selfMultiply(fracQ)).doubleValue();

        return Geom2DSuits.createVertex2D(x,y);
    }
}
