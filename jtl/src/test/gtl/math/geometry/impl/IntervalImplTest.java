package test.gtl.math.geometry.impl;

import gtl.math.geometry.GeometrySuits;
import gtl.math.geometry.Interval;
import gtl.math.geometry.IntervalType;
import junit.framework.TestCase;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;

/**
 * Created by hadoop on 1/20/17.
 */
public class IntervalImplTest extends TestCase {
    public void setUp() throws Exception {
        super.setUp();

    }

    public void tearDown() throws Exception {

    }

    public void testIsValid() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,11);
        assertTrue(i1.isValid());
        assertTrue(i2.isValid());
        assertTrue(i3.isValid());
    }

    public void testEquals() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.0,20.0);
        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_RIGHTOPEN,10,20);
        Interval i4 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.01,20);
        Interval i5 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20.001);
        assertTrue(i1.equals(i2));
        assertFalse(i1.equals(i3));
        assertFalse(i1.equals(i4));
        assertFalse(i1.equals(i5));

    }

    public void testHashCode() throws Exception {

        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.0,20.0);
        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_RIGHTOPEN,10,20);
        Interval i4 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.01,20);
        Interval i5 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20.001);
        int c1=i1.hashCode();
        int c3=i3.hashCode();

        assertFalse(c1==c3);
    }

    public void testToString() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.0,20.0);
        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_RIGHTOPEN,10,20);
        Interval i4 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.01,20);
        Interval i5 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20.001);

        String s = "IntervalImpl{type=IT_CLOSED, low=10.0, high=20.0}";
        assertTrue(s.equals(i1.toString()));
        assertFalse(s.equals(i3.toString()));
        assertFalse(s.equals(i4.toString()));
        assertFalse(s.equals(i5.toString()));

    }

    public void testGetLowerBound() throws Exception {

        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.0,20.0);
        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_RIGHTOPEN,10,20);
        Interval i4 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.01,20);
        Interval i5 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20.001);

        double low = i1.getLowerBound();
        assertTrue(low==10.0);
    }

    public void testGetUpperBound() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.0,20.0);
        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_RIGHTOPEN,10,20);
        Interval i4 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.01,20);
        Interval i5 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20.001);

        double high = i1.getUpperBound();
        assertTrue(high==20.0);
    }

    public void testSetBounds() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.0,20.0);
        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_RIGHTOPEN,10,20);
        Interval i4 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.01,20);
        Interval i5 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20.001);
        Interval i6 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,100,202.001);

        double low =i1.getLowerBound();
        double high=i1.getUpperBound();

        i6.setBounds(low,high);

        assertTrue(i6.getLowerBound()==10);
        assertTrue(i6.getUpperBound()==20);
    }

    public void testIntersects() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.0,20.0);

        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,30,40);
        assertFalse(i1.intersects(i2));

        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,-10,9);
        assertFalse(i1.intersects(i2));

        Interval i4 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,-10,10);
        assertTrue(i1.intersects(i4));
        Interval i5= GeometrySuits.createInterval(IntervalType.IT_RIGHTOPEN,-10,10);
        assertFalse(i1.intersects(i5));
        Interval i6= GeometrySuits.createInterval(IntervalType.IT_LEFTOPEN,-10,10);
        assertTrue(i1.intersects(i6));
        Interval i7= GeometrySuits.createInterval(IntervalType.IT_OPEN,-10,10);
        assertFalse(i1.intersects(i7));


        Interval i8 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,-10,15);
        assertTrue(i1.intersects(i8));
        Interval i9= GeometrySuits.createInterval(IntervalType.IT_RIGHTOPEN,-10,20);
        assertTrue(i1.intersects(i9));
        Interval i10= GeometrySuits.createInterval(IntervalType.IT_LEFTOPEN,-10,40);
        assertTrue(i1.intersects(i10));


        Interval i11 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,15);
        assertTrue(i1.intersects(i11));
        Interval i12= GeometrySuits.createInterval(IntervalType.IT_RIGHTOPEN,10,20);
        assertTrue(i1.intersects(i12));
        Interval i13= GeometrySuits.createInterval(IntervalType.IT_LEFTOPEN,10,40);
        assertTrue(i1.intersects(i13));

        Interval i14 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,15,18);
        assertTrue(i1.intersects(i14));
        Interval i15= GeometrySuits.createInterval(IntervalType.IT_RIGHTOPEN,15,20);
        assertTrue(i1.intersects(i15));
        Interval i16= GeometrySuits.createInterval(IntervalType.IT_LEFTOPEN,15,30);
        assertTrue(i1.intersects(i16));

        Interval i17 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,20,30);
        assertTrue(i1.intersects(i17));
        Interval i18= GeometrySuits.createInterval(IntervalType.IT_RIGHTOPEN,20,30);
        assertTrue(i1.intersects(i18));
        Interval i19= GeometrySuits.createInterval(IntervalType.IT_LEFTOPEN,20,30);
        assertFalse(i1.intersects(i19));

    }

    public void testIntersects1() throws Exception {
        this.testIntersects();
    }

    public void testLowerClosed() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.0,20.0);
        assertTrue(i1.lowerClosed());
        i1 = GeometrySuits.createInterval(IntervalType.IT_OPEN,10.0,20.0);
        assertFalse(i1.lowerClosed());
        i1 = GeometrySuits.createInterval(IntervalType.IT_LEFTOPEN,10.0,20.0);
        assertFalse(i1.lowerClosed());
        i1 = GeometrySuits.createInterval(IntervalType.IT_RIGHTOPEN,10.0,20.0);
        assertTrue(i1.lowerClosed());

    }

    public void testUpperClosed() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.0,20.0);
        assertTrue(i1.upperClosed());
        i1 = GeometrySuits.createInterval(IntervalType.IT_OPEN,10.0,20.0);
        assertFalse(i1.upperClosed());
        i1 = GeometrySuits.createInterval(IntervalType.IT_LEFTOPEN,10.0,20.0);
        assertTrue(i1.upperClosed());
        i1 = GeometrySuits.createInterval(IntervalType.IT_RIGHTOPEN,10.0,20.0);
        assertFalse(i1.upperClosed());
    }

    public void testGetType() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.0,20.0);
        assertTrue(i1.getType()==IntervalType.IT_CLOSED);
        i1 = GeometrySuits.createInterval(IntervalType.IT_OPEN,10.0,20.0);
        assertTrue(i1.getType()==IntervalType.IT_OPEN);
        i1 = GeometrySuits.createInterval(IntervalType.IT_LEFTOPEN,10.0,20.0);
        assertTrue(i1.getType()==IntervalType.IT_LEFTOPEN);
        i1 = GeometrySuits.createInterval(IntervalType.IT_RIGHTOPEN,10.0,20.0);
        assertTrue(i1.getType()==IntervalType.IT_RIGHTOPEN);
    }

    public void testClone() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.0,20.0);
        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.0,20.0);
        assertTrue(i1.equals(i2));
        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_RIGHTOPEN,10.0,20.0);
        assertFalse(i1.equals(i3));
        Interval i4 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,-10.0,20.0);
        assertFalse(i1.equals(i4));
        Interval i5 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.0,30.0);
        assertFalse(i1.equals(i5));
    }

    public void testCopyFrom() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.0,20.0);

        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_OPEN,-10.0,20.0);

        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_OPEN,-10.0,20.0);

        i3.copyFrom(i1);
        assertTrue(i1.equals(i3));

        i1.copyTo(i2);

        assertTrue(i3.equals(i2));


    }

    public void testRead() throws Exception {

    }

    public void testWrite() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.0,20.0);

        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_OPEN,-10.0,20.0);

        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_OPEN,30.0,40.0);

        ByteArrayOutputStream baos= new ByteArrayOutputStream();
        i1.write(baos);
        i2.write(baos);
        i3.write(baos);
        byte [] dat = baos.toByteArray();

        Interval i4 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,0,1);
        Interval i5 = GeometrySuits.createInterval(IntervalType.IT_OPEN,0,1);
        Interval i6 = GeometrySuits.createInterval(IntervalType.IT_OPEN,0,1);

        ByteArrayInputStream bais = new ByteArrayInputStream(dat);

        i4.read(bais);
        assertTrue(i4.equals(i1));
        i5.read(bais);
        assertTrue(i5.equals(i2));
        i6.read(bais);
        assertTrue(i6.equals(i3));



    }

    public void testGetByteArraySize() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.0,20.0);
        assertTrue(i1.getByteArraySize()==(4+8+8));
    }

    public void testReset() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.0,20.0);

        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_OPEN,-10.0,20.0);

        i2.reset(IntervalType.IT_CLOSED,10,20);

        assertTrue(i2.equals(i1));


    }

    public void testEquals1() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.0,20.0);
        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_RIGHTOPEN,10,20);
        Interval i4 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10.01,20);
        Interval i5 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20.001);
        assertTrue(i1.equals((Object)i2));
        assertFalse(i1.equals((Object)i3));
        assertFalse(i1.equals((Object)i4));
        assertFalse(i1.equals((Object)i5));
    }

    public void testStarts() throws Exception {

        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,18);
        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_OPEN,10,18);

        assertTrue(i2.starts(i1));
        assertFalse(i3.starts(i1));

    }

    public void testStartedBy() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,18);
        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_OPEN,10,18);

        assertTrue(i1.startedBy(i2));
        assertFalse(i1.startedBy(i3));
    }

    public void testMeets() throws Exception {

        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,20,30);
        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_OPEN,20,30);

        assertTrue(i1.meets(i2));
        assertFalse(i1.meets(i3));
    }

    public void testMetBy() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,20,30);
        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_OPEN,20,30);

        assertTrue(i2.metBy(i1));
        assertFalse(i3.metBy(i1));

    }

    public void testFinishes() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,15,20);
        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_OPEN,10,20);

        assertTrue(i2.finishes(i1));
        assertFalse(i3.finishes(i1));

    }

    public void testFinishedBy() throws Exception {

        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,15,20);
        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_OPEN,10,20);

        assertTrue(i1.finishedBy(i2));
        assertFalse(i1.finishedBy(i3));

    }

    public void testBefore() throws Exception {

        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,15,20);
        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_OPEN,-10,10);
        Interval i4 = GeometrySuits.createInterval(IntervalType.IT_OPEN,-10,9);


        assertTrue(i3.before(i1));
        assertFalse(i2.before(i1));
        assertTrue(i4.before(i1));

    }

    public void testAfter() throws Exception {

        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,15,20);
        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_OPEN,-10,10);
        Interval i4 = GeometrySuits.createInterval(IntervalType.IT_OPEN,-10,9);


        assertTrue(i1.after(i3));
        assertFalse(i1.after(i2));
        assertTrue(i1.after(i4));

    }

    public void testOverlaps() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,15,30);
        assertTrue(i1.overlaps(i2));
    }

    public void testOverlappedBy() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,15,30);
        assertTrue(i2.overlappedBy(i1));
    }

    public void testDuring() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i4 = GeometrySuits.createInterval(IntervalType.IT_OPEN,10,20);
        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,15,30);
        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,9,30);

        assertFalse(i2.during(i1));
        assertTrue(i1.during(i3));
        assertTrue(i4.during(i1));

    }

    public void testCovers() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i4 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,15,30);
        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,9,30);

        assertFalse(i2.covers(i1));
        assertTrue(i3.covers(i1));
        assertTrue(i4.covers(i1));
    }

    public void testCoveredBy() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i4 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,15,30);
        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,9,30);

        assertFalse(i2.coveredBy(i1));
        assertTrue(i1.coveredBy(i3));
        assertTrue(i4.coveredBy(i1));
    }

    public void testContains() throws Exception {
        Interval i1 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,10,20);
        Interval i4 = GeometrySuits.createInterval(IntervalType.IT_OPEN,10,20);
        Interval i2 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,15,30);
        Interval i3 = GeometrySuits.createInterval(IntervalType.IT_CLOSED,9,30);

        assertFalse(i1.contains(i2));
        assertTrue(i3.contains(i1));
        assertTrue(i1.contains(i4));
    }

}