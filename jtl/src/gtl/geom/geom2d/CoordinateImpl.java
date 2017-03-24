package gtl.math.geometric.geom2d;
import gtl.math.MathSuits;
import gtl.math.geometric.Vertex;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.Comparator;

/**
 * Created by hadoop on 17-3-16.
 */
class CoordinateImpl extends Coordinate {


    /**
     *  Constructs a <code>Coordinate</code> at (x,y,z).
     *
     *@param  x  the x-value
     *@param  y  the y-value
     *@param  z  the z-value
     */
    public CoordinateImpl(double x, double y, double z) {
        this.x = x;
        this.y = y;
        this.z = z;
    }

    /**
     *  Constructs a <code>Coordinate</code> at (0,0,NaN).
     */
    public CoordinateImpl() {
        this(0.0, 0.0);
    }

    /**
     *  Constructs a <code>Coordinate</code> having the same (x,y,z) values as
     *  <code>other</code>.
     *
     *@param  c  the <code>Coordinate</code> to copy.
     */
    public CoordinateImpl(CoordinateImpl c) {
        this(c.x, c.y, c.z);
    }

    /**
     *  Constructs a <code>Coordinate</code> at (x,y,NaN).
     *
     *@param  x  the x-value
     *@param  y  the y-value
     */
    public CoordinateImpl(double x, double y) {
        this(x, y, NULL_ORDINATE);
    }


    public Object clone() {
        return (Object)new CoordinateImpl(this.x,this.y,this.z);
    }

    /**
     *  Sets this <code>Coordinate</code>s (x,y,z) values to that of <code>other</code>.
     *
     *@param  other  the <code>Coordinate</code> to copy
     */
    public void setCoordinate(Coordinate other) {
        CoordinateImpl c= (CoordinateImpl) other;
        x = c.x;
        y = c.y;
        z = c.z;
    }

    @Override
    public void setCoordinate(Vertex other) {
        x=other.getX();
        y=other.getY();
        z=other.getZ();
    }

    /**
     * Gets the ordinate value for the given index.
     * The supported values for the index are
     * {@link X}, {@link Y}, and {@link Z}.
     *
     * @param ordinateIndex the ordinate index
     * @return the value of the ordinate
     * @throws IllegalArgumentException if the index is not valid
     */
    public double getOrdinate(int ordinateIndex)
    {
        switch (ordinateIndex) {
            case X: return x;
            case Y: return y;
            case Z: return z;
        }
        throw new IllegalArgumentException("Invalid ordinate index: " + ordinateIndex);
    }

    /**
     * Sets the ordinate for the given index
     * to a given value.
     * The supported values for the index are
     * {@link X}, {@link Y}, and {@link Z}.
     *
     * @param ordinateIndex the ordinate index
     * @param value the value to set
     * @throws IllegalArgumentException if the index is not valid
     */
    public void setOrdinate(int ordinateIndex, double value)
    {
        switch (ordinateIndex) {
            case X:
                x = value;
                break;
            case Y:
                y = value;
                break;
            case Z:
                z = value;
                break;
            default:
                throw new IllegalArgumentException("Invalid ordinate index: " + ordinateIndex);
        }
    }

    /**
     *  Returns whether the planar projections of the two <code>Coordinate</code>s
     *  are equal.
     *
     *@param  other  a <code>Coordinate</code> with which to do the 2D comparison.
     *@return        <code>true</code> if the x- and y-coordinates are equal; the
     *      z-coordinates do not have to be equal.
     */
    public boolean equals2D(Coordinate other) {
        CoordinateImpl c= (CoordinateImpl) other;
        if (x != c.x) {
            return false;
        }
        if (y != c.y) {
            return false;
        }
        return true;
    }

    /**
     * Tests if another coordinate has the same values for the X and Y ordinates.
     * The Z ordinate is ignored.
     *
     *@param c a <code>Coordinate</code> with which to do the 2D comparison.
     *@return true if <code>other</code> is a <code>Coordinate</code>
     *      with the same values for X and Y.
     */
    public boolean equals2D(Coordinate c, double tolerance){
        if (! MathSuits.equalsWithTolerance(this.x, c.getOrdinate(X), tolerance)) {
            return false;
        }
        if (! MathSuits.equalsWithTolerance(this.y, c.getOrdinate(Y), tolerance)) {
            return false;
        }
        return true;
    }

    /**
     * Tests if another coordinate has the same values for the X, Y and Z ordinates.
     *
     *@param other a <code>Coordinate</code> with which to do the 3D comparison.
     *@return true if <code>other</code> is a <code>Coordinate</code>
     *      with the same values for X, Y and Z.
     */
    public boolean equals3D(Coordinate other) {
        CoordinateImpl c= (CoordinateImpl) other;
        return (x == c.x) && (y == c.y) &&
                ((z == c.z) ||
                        (Double.isNaN(z) && Double.isNaN(c.z)));
    }

    /**
     * Tests if another coordinate has the same value for Z, within a tolerance.
     *
     * @param c a coordinate
     * @param tolerance the tolerance value
     * @return true if the Z ordinates are within the given tolerance
     */
    public boolean equalInZ(Coordinate c, double tolerance){
        return MathSuits.equalsWithTolerance(this.z, c.getOrdinate(Z), tolerance);
    }

    /**
     *  Returns <code>true</code> if <code>other</code> has the same values for
     *  the x and y ordinates.
     *  Since Coordinates are 2.5D, this routine ignores the z value when making the comparison.
     *
     *@param  other  a <code>Coordinate</code> with which to do the comparison.
     *@return        <code>true</code> if <code>other</code> is a <code>Coordinate</code>
     *      with the same values for the x and y ordinates.
     */
    public boolean equals(Object other) {
        if (!(other instanceof Coordinate)) {
            return false;
        }
        return equals2D((Coordinate) other);
    }





    /**
     * Computes the 2-dimensional Euclidean distance to another location.
     * The Z-ordinate is ignored.
     *
     * @param c a point
     * @return the 2-dimensional Euclidean distance between the locations
     */
    public double distance(Coordinate c) {
        double dx = x - c.getOrdinate(X);
        double dy = y - c.getOrdinate(Y);
        return Math.sqrt(dx * dx + dy * dy);
    }

    /**
     * Computes the 3-dimensional Euclidean distance to another location.
     *
     * @param c a coordinate
     * @return the 3-dimensional Euclidean distance between the locations
     */
    public double distance3D(Coordinate cp) {
        CoordinateImpl c= (CoordinateImpl)cp;
        double dx = x - c.x;
        double dy = y - c.y;
        double dz = z - c.z;
        return Math.sqrt(dx * dx + dy * dy + dz * dz);
    }



    @Override
    public void copyFrom(Object i) {
        if(i instanceof Coordinate){
            Coordinate c = (Coordinate)i;
            this.x=c.getOrdinate(X);
            this.y=c.getOrdinate(Y);
            this.z=c.getOrdinate(Z);
        }
    }

    @Override
    public boolean load(DataInput in) throws IOException {
        this.x=in.readDouble();
        this.y=in.readDouble();
        this.z=in.readDouble();
        return true;
    }

    @Override
    public boolean store(DataOutput out) throws IOException {
        out.writeDouble(this.x);
        out.writeDouble(this.y);
        out.writeDouble(this.z);
        return true;
    }

    @Override
    public long getByteArraySize() {
        return 24;
    }

    /**
     *  Compares this {@link Coordinate} with the specified {@link Coordinate} for order.
     *  This method ignores the z value when making the comparison.
     *  Returns:
     *  <UL>
     *    <LI> -1 : this.x &lt; other.x || ((this.x == other.x) &amp;&amp; (this.y &lt; other.y))
     *    <LI> 0 : this.x == other.x &amp;&amp; this.y = other.y
     *    <LI> 1 : this.x &gt; other.x || ((this.x == other.x) &amp;&amp; (this.y &gt; other.y))
     *
     *  </UL>
     *  Note: This method assumes that ordinate values
     * are valid numbers.  NaN values are not handled correctly.
     *
     *@param  o  the <code>Coordinate</code> with which this <code>Coordinate</code>
     *      is being compared
     *@return    -1, zero, or 1 as this <code>Coordinate</code>
     *      is less than, equal to, or greater than the specified <code>Coordinate</code>
     */
    @Override
    public int compareTo(Coordinate other) {
        if (x < other.x) return -1;
        if (x > other.x) return 1;
        if (y < other.y) return -1;
        if (y > other.y) return 1;
        return 0;
    }

    /**
     * Gets a hashcode for this coordinate.
     *
     * @return a hashcode for this coordinate
     */
    @Override
    public  int hashCode() {
        //Algorithm from Effective Java by Joshua Bloch [Jon Aquino]
        int result = 17;
        result = 37 * result + hashCode(x);
        result = 37 * result + hashCode(y);
        return result;
    }


    /**
     * Compares two {@link Coordinate}s, allowing for either a 2-dimensional
     * or 3-dimensional comparison, and handling NaN values correctly.
     */
    public static class DimensionalComparator
            implements Comparator
    {
        /**
         * Compare two <code>double</code>s, allowing for NaN values.
         * NaN is treated as being less than any valid numeric.
         *
         * @param a a <code>double</code>
         * @param b a <code>double</code>
         * @return -1, 0, or 1 depending on whether a is less than, equal to or greater than b
         */
        public static int compare(double a, double b)
        {
            if (a < b) return -1;
            if (a > b) return 1;

            if (Double.isNaN(a)) {
                if (Double.isNaN(b)) return 0;
                return -1;
            }

            if (Double.isNaN(b)) return 1;
            return 0;
        }

        private int dimensionsToTest = 2;

        /**
         * Creates a comparator for 2 dimensional coordinates.
         */
        public DimensionalComparator()
        {
            this(2);
        }

        /**
         * Creates a comparator for 2 or 3 dimensional coordinates, depending
         * on the value provided.
         *
         * @param dimensionsToTest the numeric of dimensions to test
         */
        public DimensionalComparator(int dimensionsToTest)
        {
            if (dimensionsToTest != 2 && dimensionsToTest != 3)
                throw new IllegalArgumentException("only 2 or 3 dimensions may be specified");
            this.dimensionsToTest = dimensionsToTest;
        }

        /**
         * Compares two {@link Coordinate}s along to the numeric of
         * dimensions specified.
         *
         * @param o1 a {@link Coordinate}
         * @param o2 a {link Coordinate}
         * @return -1, 0, or 1 depending on whether o1 is less than,
         * equal to, or greater than 02
         *
         */
        public int compare(Object o1, Object o2)
        {
            CoordinateImpl c1 = (CoordinateImpl) o1;
            CoordinateImpl c2 = (CoordinateImpl) o2;

            int compX = compare(c1.x, c2.x);
            if (compX != 0) return compX;

            int compY = compare(c1.y, c2.y);
            if (compY != 0) return compY;

            if (dimensionsToTest <= 2) return 0;

            int compZ = compare(c1.z, c2.z);
            return compZ;
        }
    }
}
