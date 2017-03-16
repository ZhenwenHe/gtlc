package gtl.geom;

/**
 * Created by hadoop on 17-3-20.
 */

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

/**
 *  Defines a rectangular region of the 2D coordinate plane.
 *  It is often used to represent the bounding box of a {@link Geometry},
 *  e.g. the minimum and maximum x and y values of the {@link Vertex}s.
 *  <p>
 *  Envelopes support infinite or half-infinite regions, by using the values of
 *  <code>Double.POSITIVE_INFINITY</code> and <code>Double.NEGATIVE_INFINITY</code>.
 *  Envelope2D objects may have a null value.
 *  <p>
 *  When Envelope2D objects are created or initialized,
 *  the supplies extent values are automatically sorted into the correct order.
 */
public class Envelope2D implements Comparable<Envelope2D>, gtl.io.Serializable
{
    public int hashCode() {
        //Algorithm from Effective Java by Joshua Bloch [Jon Aquino]
        int result = 17;
        result = 37 * result + Vertex.hashCode(minx);
        result = 37 * result + Vertex.hashCode(maxx);
        result = 37 * result + Vertex.hashCode(miny);
        result = 37 * result + Vertex.hashCode(maxy);
        return result;
    }

    /**
     * Test the point q to see whether it intersects the Envelope2D defined by p1-p2
     * @param p1 one extremal point of the envelope2D
     * @param p2 another extremal point of the envelope2D
     * @param q the point to test for intersection
     * @return <code>true</code> if q intersects the envelope2D p1-p2
     */
    public static boolean intersects(Vertex p1, Vertex p2, Vertex q)
    {
        //OptimizeIt shows that Math#min and Math#max here are a bottleneck.
        //Replace with direct comparisons. [Jon Aquino]
        if (((q.x >= (p1.x < p2.x ? p1.x : p2.x)) && (q.x <= (p1.x > p2.x ? p1.x : p2.x))) &&
                ((q.y >= (p1.y < p2.y ? p1.y : p2.y)) && (q.y <= (p1.y > p2.y ? p1.y : p2.y)))) {
            return true;
        }
        return false;
    }

    /**
     * Tests whether the envelope2D defined by p1-p2
     * and the envelope2D defined by q1-q2
     * intersect.
     *
     * @param p1 one extremal point of the envelope2D P
     * @param p2 another extremal point of the envelope2D P
     * @param q1 one extremal point of the envelope2D Q
     * @param q2 another extremal point of the envelope2D Q
     * @return <code>true</code> if Q intersects P
     */
    public static boolean intersects(Vertex p1, Vertex p2, Vertex q1, Vertex q2)
    {
        double minq = Math.min(q1.x, q2.x);
        double maxq = Math.max(q1.x, q2.x);
        double minp = Math.min(p1.x, p2.x);
        double maxp = Math.max(p1.x, p2.x);

        if( minp > maxq )
            return false;
        if( maxp < minq )
            return false;

        minq = Math.min(q1.y, q2.y);
        maxq = Math.max(q1.y, q2.y);
        minp = Math.min(p1.y, p2.y);
        maxp = Math.max(p1.y, p2.y);

        if( minp > maxq )
            return false;
        if( maxp < minq )
            return false;
        return true;
    }

    /**
     *  the minimum x-coordinate
     */
    private double minx;

    /**
     *  the maximum x-coordinate
     */
    private double maxx;

    /**
     *  the minimum y-coordinate
     */
    private double miny;

    /**
     *  the maximum y-coordinate
     */
    private double maxy;

    /**
     *  Creates a null <code>Envelope2D</code>.
     */
    public Envelope2D() {
        init();
    }

    /**
     *  Creates an <code>Envelope2D</code> for a region defined by maximum and minimum values.
     *
     *@param  x1  the first x-value
     *@param  x2  the second x-value
     *@param  y1  the first y-value
     *@param  y2  the second y-value
     */
    public Envelope2D(double x1, double x2, double y1, double y2)
    {
        init(x1, x2, y1, y2);
    }

    /**
     *  Creates an <code>Envelope2D</code> for a region defined by two Coordinates.
     *
     *@param  p1  the first Vertex
     *@param  p2  the second Vertex
     */
    public Envelope2D(Vertex p1, Vertex p2)
    {
        init(p1.x, p2.x, p1.y, p2.y);
    }

    /**
     *  Creates an <code>Envelope2D</code> for a region defined by a single Vertex.
     *
     *@param  p  the Vertex
     */
    public Envelope2D(Vertex p)
    {
        init(p.x, p.x, p.y, p.y);
    }

    /**
     *  Create an <code>Envelope2D</code> from an existing Envelope2D.
     *
     *@param  env  the Envelope2D to initialize from
     */
    public Envelope2D(Envelope2D env)
    {
        init(env);
    }

    /**
     *  Initialize to a null <code>Envelope2D</code>.
     */
    public void init()
    {
        setToNull();
    }

    /**
     *  Initialize an <code>Envelope2D</code> for a region defined by maximum and minimum values.
     *
     *@param  x1  the first x-value
     *@param  x2  the second x-value
     *@param  y1  the first y-value
     *@param  y2  the second y-value
     */
    public void init(double x1, double x2, double y1, double y2)
    {
        if (x1 < x2) {
            minx = x1;
            maxx = x2;
        }
        else {
            minx = x2;
            maxx = x1;
        }
        if (y1 < y2) {
            miny = y1;
            maxy = y2;
        }
        else {
            miny = y2;
            maxy = y1;
        }
    }

    /**
     *  Initialize an <code>Envelope2D</code> to a region defined by two Coordinates.
     *
     *@param  p1  the first Vertex
     *@param  p2  the second Vertex
     */
    public void init(Vertex p1, Vertex p2)
    {
        init(p1.x, p2.x, p1.y, p2.y);
    }

    /**
     *  Initialize an <code>Envelope2D</code> to a region defined by a single Vertex.
     *
     *@param  p  the coordinate
     */
    public void init(Vertex p)
    {
        init(p.x, p.x, p.y, p.y);
    }

    /**
     *  Initialize an <code>Envelope2D</code> from an existing Envelope2D.
     *
     *@param  env  the Envelope2D to initialize from
     */
    public void init(Envelope2D env)
    {
        this.minx = env.minx;
        this.maxx = env.maxx;
        this.miny = env.miny;
        this.maxy = env.maxy;
    }


    /**
     *  Makes this <code>Envelope2D</code> a "null" envelope2D, that is, the envelope2D
     *  of the empty geometry.
     */
    public void setToNull() {
        minx = 0;
        maxx = -1;
        miny = 0;
        maxy = -1;
    }

    /**
     *  Returns <code>true</code> if this <code>Envelope2D</code> is a "null"
     *  envelope2D.
     *
     *@return    <code>true</code> if this <code>Envelope2D</code> is uninitialized
     *      or is the envelope2D of the empty geometry.
     */
    public boolean isNull() {
        return maxx < minx;
    }

    /**
     *  Returns the difference between the maximum and minimum x values.
     *
     *@return    max x - min x, or 0 if this is a null <code>Envelope2D</code>
     */
    public double getWidth() {
        if (isNull()) {
            return 0;
        }
        return maxx - minx;
    }

    /**
     *  Returns the difference between the maximum and minimum y values.
     *
     *@return    max y - min y, or 0 if this is a null <code>Envelope2D</code>
     */
    public double getHeight() {
        if (isNull()) {
            return 0;
        }
        return maxy - miny;
    }

    /**
     *  Returns the <code>Envelope2D</code>s minimum x-value. min x &gt; max x
     *  indicates that this is a null <code>Envelope2D</code>.
     *
     *@return    the minimum x-coordinate
     */
    public double getMinX() {
        return minx;
    }

    /**
     *  Returns the <code>Envelope2D</code>s maximum x-value. min x &gt; max x
     *  indicates that this is a null <code>Envelope2D</code>.
     *
     *@return    the maximum x-coordinate
     */
    public double getMaxX() {
        return maxx;
    }

    /**
     *  Returns the <code>Envelope2D</code>s minimum y-value. min y &gt; max y
     *  indicates that this is a null <code>Envelope2D</code>.
     *
     *@return    the minimum y-coordinate
     */
    public double getMinY() {
        return miny;
    }

    /**
     *  Returns the <code>Envelope2D</code>s maximum y-value. min y &gt; max y
     *  indicates that this is a null <code>Envelope2D</code>.
     *
     *@return    the maximum y-coordinate
     */
    public double getMaxY() {
        return maxy;
    }

    /**
     * Gets the area of this envelope2D.
     *
     * @return the area of the envelope2D
     * @return 0.0 if the envelope2D is null
     */
    public double getArea()
    {
        return getWidth() * getHeight();
    }

    /**
     * Gets the minimum extent of this envelope2D across both dimensions.
     *
     * @return the minimum extent of this envelope2D
     */
    public double minExtent()
    {
        if (isNull()) return 0.0;
        double w = getWidth();
        double h = getHeight();
        if (w < h) return w;
        return h;
    }

    /**
     * Gets the maximum extent of this envelope2D across both dimensions.
     *
     * @return the maximum extent of this envelope2D
     */
    public double maxExtent()
    {
        if (isNull()) return 0.0;
        double w = getWidth();
        double h = getHeight();
        if (w > h) return w;
        return h;
    }

    /**
     *  Enlarges this <code>Envelope2D</code> so that it contains
     *  the given {@link Vertex}.
     *  Has no effect if the point is already on or within the envelope2D.
     *
     *@param  p  the Vertex to expand to include
     */
    public void expandToInclude(Vertex p)
    {
        expandToInclude(p.x, p.y);
    }

    /**
     * Expands this envelope2D by a given distance2D in all directions.
     * Both positive and negative distances are supported.
     *
     * @param distance the distance2D to expand the envelope2D
     */
    public void expandBy(double distance)
    {
        expandBy(distance, distance);
    }

    /**
     * Expands this envelope2D by a given distance2D in all directions.
     * Both positive and negative distances are supported.
     *
     * @param deltaX the distance2D to expand the envelope2D along the the X axis
     * @param deltaY the distance2D to expand the envelope2D along the the Y axis
     */
    public void expandBy(double deltaX, double deltaY)
    {
        if (isNull()) return;

        minx -= deltaX;
        maxx += deltaX;
        miny -= deltaY;
        maxy += deltaY;

        // check for envelope2D disappearing
        if (minx > maxx || miny > maxy)
            setToNull();
    }

    /**
     *  Enlarges this <code>Envelope2D</code> so that it contains
     *  the given point.
     *  Has no effect if the point is already on or within the envelope2D.
     *
     *@param  x  the value to lower the minimum x to or to raise the maximum x to
     *@param  y  the value to lower the minimum y to or to raise the maximum y to
     */
    public void expandToInclude(double x, double y) {
        if (isNull()) {
            minx = x;
            maxx = x;
            miny = y;
            maxy = y;
        }
        else {
            if (x < minx) {
                minx = x;
            }
            if (x > maxx) {
                maxx = x;
            }
            if (y < miny) {
                miny = y;
            }
            if (y > maxy) {
                maxy = y;
            }
        }
    }

    /**
     *  Enlarges this <code>Envelope2D</code> so that it contains
     *  the <code>other</code> Envelope2D.
     *  Has no effect if <code>other</code> is wholly on or
     *  within the envelope2D.
     *
     *@param  other  the <code>Envelope2D</code> to expand to include
     */
    public void expandToInclude(Envelope2D other) {
        if (other.isNull()) {
            return;
        }
        if (isNull()) {
            minx = other.getMinX();
            maxx = other.getMaxX();
            miny = other.getMinY();
            maxy = other.getMaxY();
        }
        else {
            if (other.minx < minx) {
                minx = other.minx;
            }
            if (other.maxx > maxx) {
                maxx = other.maxx;
            }
            if (other.miny < miny) {
                miny = other.miny;
            }
            if (other.maxy > maxy) {
                maxy = other.maxy;
            }
        }
    }

    /**
     * Translates this envelope2D by given amounts in the X and Y direction.
     *
     * @param transX the amount to translate along the X axis
     * @param transY the amount to translate along the Y axis
     */
    public void translate(double transX, double transY) {
        if (isNull()) {
            return;
        }
        init(getMinX() + transX, getMaxX() + transX,
                getMinY() + transY, getMaxY() + transY);
    }

    /**
     * Computes the coordinate of the centre of this envelope2D (as long as it is non-null
     *
     * @return the centre coordinate of this envelope2D
     * <code>null</code> if the envelope2D is null
     */
    public Vertex centre() {
        if (isNull()) return null;
        return Geom2DSuits.createVertex(
                (getMinX() + getMaxX()) / 2.0,
                (getMinY() + getMaxY()) / 2.0);
    }

    /**
     * Computes the intersection of two {@link Envelope2D}s.
     *
     * @param env the envelope2D to intersect with
     * @return a new Envelope2D representing the intersection of the envelopes (this will be
     * the null envelope2D if either argument is null, or they do not intersect
     */
    public Envelope2D intersection(Envelope2D env)
    {
        if (isNull() || env.isNull() || ! intersects(env)) return new Envelope2D();

        double intMinX = minx > env.minx ? minx : env.minx;
        double intMinY = miny > env.miny ? miny : env.miny;
        double intMaxX = maxx < env.maxx ? maxx : env.maxx;
        double intMaxY = maxy < env.maxy ? maxy : env.maxy;
        return new Envelope2D(intMinX, intMaxX, intMinY, intMaxY);
    }



    /**
     *  Check if the region defined by <code>other</code>
     *  overlaps (intersects) the region of this <code>Envelope2D</code>.
     *
     *@param  other  the <code>Envelope2D</code> which this <code>Envelope2D</code> is
     *          being checked for overlapping
     *@return        <code>true</code> if the <code>Envelope2D</code>s overlap
     */
    public boolean intersects(Envelope2D other) {
        if (isNull() || other.isNull()) { return false; }
        return !(other.minx > maxx ||
                other.maxx < minx ||
                other.miny > maxy ||
                other.maxy < miny);
    }
    /**
     * @deprecated Use #intersects instead. In the future, #overlaps may be
     * changed to be a true overlap check; that is, whether the intersection is
     * two-dimensional.
     */
    public boolean overlaps(Envelope2D other) {
        return intersects(other);
    }

    /**
     *  Check if the point <code>p</code>
     *  overlaps (lies inside) the region of this <code>Envelope2D</code>.
     *
     *@param  p  the <code>Vertex</code> to be tested
     *@return        <code>true</code> if the point overlaps this <code>Envelope2D</code>
     */
    public boolean intersects(Vertex p) {
        return intersects(p.x, p.y);
    }
    /**
     * @deprecated Use #intersects instead.
     */
    public boolean overlaps(Vertex p) {
        return intersects(p);
    }
    /**
     *  Check if the point <code>(x, y)</code>
     *  overlaps (lies inside) the region of this <code>Envelope2D</code>.
     *
     *@param  x  the x-ordinate of the point
     *@param  y  the y-ordinate of the point
     *@return        <code>true</code> if the point overlaps this <code>Envelope2D</code>
     */
    public boolean intersects(double x, double y) {
        if (isNull()) return false;
        return ! (x > maxx ||
                x < minx ||
                y > maxy ||
                y < miny);
    }
    /**
     * @deprecated Use #intersects instead.
     */
    public boolean overlaps(double x, double y) {
        return intersects(x, y);
    }

    /**
     * Tests if the <code>Envelope2D other</code>
     * lies wholely inside this <code>Envelope2D</code> (inclusive of the boundary).
     * <p>
     * Note that this is <b>not</b> the same definition as the SFS <tt>contains</tt>,
     * which would exclude the envelope boundary.
     *
     *@param  other the <code>Envelope2D</code> to check
     *@return true if <code>other</code> is contained in this <code>Envelope2D</code>
     *
     *@see #covers(Envelope2D)
     */
    public boolean contains(Envelope2D other) {
        return covers(other);
    }

    /**
     * Tests if the given point lies in or on the envelope2D.
     * <p>
     * Note that this is <b>not</b> the same definition as the SFS <tt>contains</tt>,
     * which would exclude the envelope2D boundary.
     *
     *@param  p  the point which this <code>Envelope2D</code> is
     *      being checked for containing
     *@return    <code>true</code> if the point lies in the interior or
     *      on the boundary of this <code>Envelope2D</code>.
     *
     *@see #covers(Vertex)
     */
    public boolean contains(Vertex p) {
        return covers(p);
    }

    /**
     * Tests if the given point lies in or on the envelope2D.
     * <p>
     * Note that this is <b>not</b> the same definition as the SFS <tt>contains</tt>,
     * which would exclude the envelope2D boundary.
     *
     *@param  x  the x-coordinate of the point which this <code>Envelope2D</code> is
     *      being checked for containing
     *@param  y  the y-coordinate of the point which this <code>Envelope2D</code> is
     *      being checked for containing
     *@return    <code>true</code> if <code>(x, y)</code> lies in the interior or
     *      on the boundary of this <code>Envelope2D</code>.
     *
     *@see #covers(double, double)
     */
    public boolean contains(double x, double y) {
        return covers(x, y);
    }

    /**
     * Tests if the given point lies in or on the envelope2D.
     *
     *@param  x  the x-coordinate of the point which this <code>Envelope2D</code> is
     *      being checked for containing
     *@param  y  the y-coordinate of the point which this <code>Envelope2D</code> is
     *      being checked for containing
     *@return    <code>true</code> if <code>(x, y)</code> lies in the interior or
     *      on the boundary of this <code>Envelope2D</code>.
     */
    public boolean covers(double x, double y) {
        if (isNull()) return false;
        return x >= minx &&
                x <= maxx &&
                y >= miny &&
                y <= maxy;
    }

    /**
     * Tests if the given point lies in or on the envelope2D.
     *
     *@param  p  the point which this <code>Envelope2D</code> is
     *      being checked for containing
     *@return    <code>true</code> if the point lies in the interior or
     *      on the boundary of this <code>Envelope2D</code>.
     */
    public boolean covers(Vertex p) {
        return covers(p.x, p.y);
    }

    /**
     * Tests if the <code>Envelope2D other</code>
     * lies wholely inside this <code>Envelope2D</code> (inclusive of the boundary).
     *
     *@param  other the <code>Envelope2D</code> to check
     *@return true if this <code>Envelope2D</code> covers the <code>other</code>
     */
    public boolean covers(Envelope2D other) {
        if (isNull() || other.isNull()) { return false; }
        return other.getMinX() >= minx &&
                other.getMaxX() <= maxx &&
                other.getMinY() >= miny &&
                other.getMaxY() <= maxy;
    }

    /**
     * Computes the distance2D between this and another
     * <code>Envelope2D</code>.
     * The distance2D between overlapping Envelopes is 0.  Otherwise, the
     * distance2D is the Euclidean distance2D between the closest points.
     */
    public double distance(Envelope2D env)
    {
        if (intersects(env)) return 0;

        double dx = 0.0;
        if (maxx < env.minx)
            dx = env.minx - maxx;
        else if (minx > env.maxx)
            dx = minx - env.maxx;

        double dy = 0.0;
        if (maxy < env.miny)
            dy = env.miny - maxy;
        else if (miny > env.maxy) dy = miny - env.maxy;

        // if either is zero, the envelopes overlap either vertically or horizontally
        if (dx == 0.0) return dy;
        if (dy == 0.0) return dx;
        return Math.sqrt(dx * dx + dy * dy);
    }

    public boolean equals(Object other) {
        if (!(other instanceof Envelope2D)) {
            return false;
        }
        Envelope2D otherEnvelope2D = (Envelope2D) other;
        if (isNull()) {
            return otherEnvelope2D.isNull();
        }
        return maxx == otherEnvelope2D.getMaxX() &&
                maxy == otherEnvelope2D.getMaxY() &&
                minx == otherEnvelope2D.getMinX() &&
                miny == otherEnvelope2D.getMinY();
    }

    public String toString()
    {
        return "Env[" + minx + " : " + maxx + ", " + miny + " : " + maxy + "]";
    }

    /**
     * Compares two envelopes using lexicographic ordering.
     * The ordering comparison is based on the usual numerical
     * comparison between the sequence of ordinates.
     * Null envelopes are less than all non-null envelopes.
     *
     * @param o an Envelope2D object
     */
    public int compareTo (Envelope2D o) {
        Envelope2D env = (Envelope2D) o;
        // compare nulls if present
        if (isNull()) {
            if (env.isNull()) return 0;
            return -1;
        }
        else {
            if (env.isNull()) return 1;
        }
        // compare based on numerical ordering of ordinates
        if (minx < env.minx) return -1;
        if (minx > env.minx) return 1;
        if (miny < env.miny) return -1;
        if (miny > env.miny) return 1;
        if (maxx < env.maxx) return -1;
        if (maxx > env.maxx) return 1;
        if (maxy < env.maxy) return -1;
        if (maxy > env.maxy) return 1;
        return 0;


    }

    @Override
    public Object clone() {
        return new Envelope2D(this.minx,this.maxx,this.miny,this.maxy);
    }

    @Override
    public void copyFrom(Object i) {
        if(i instanceof Envelope2D){
            Envelope2D e=(Envelope2D)i;
            this.init(e.minx,e.maxx,e.miny,e.maxy);
        }
    }

    @Override
    public boolean load(DataInput in) throws IOException {
        this.minx=in.readDouble();
        this.maxx=in.readDouble();
        this.miny=in.readDouble();
        this.maxy=in.readDouble();
        return true;
    }

    @Override
    public boolean store(DataOutput out) throws IOException {
        out.writeDouble(this.minx);
        out.writeDouble(this.maxx);
        out.writeDouble(this.miny);
        out.writeDouble(this.maxy);
        return true;
    }

    @Override
    public long getByteArraySize() {
        return 8*4;
    }


}
