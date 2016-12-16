package gtl.stil.impl;

import gtl.stil.Interval;
import gtl.stil.IntervalType;

import java.io.*;

/**
 * Created by ZhenwenHe on 2016/12/8.
 */
public class IntervalImpl implements Interval {
    IntervalType type;
    double low;
    double high;

    public IntervalImpl(IntervalType type, double low, double high) {
        assert low<high;
        this.type = type;
        this.low = low;
        this.high = high;
    }

    public IntervalImpl(double low, double high) {
        assert low<high;
        this.type=IntervalType.IT_RIGHTOPEN;
        this.low = low;
        this.high = high;
    }

    public IntervalImpl() {
        this.type=IntervalType.IT_RIGHTOPEN;
        this.low=0;
        this.high=1;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof IntervalImpl)) return false;

        IntervalImpl interval = (IntervalImpl) o;

        if (Double.compare(interval.low, low) != 0) return false;
        if (Double.compare(interval.high, high) != 0) return false;
        return type == interval.type;

    }

    @Override
    public int hashCode() {
        int result;
        long temp;
        result = type.hashCode();
        temp = Double.doubleToLongBits(low);
        result = 31 * result + (int) (temp ^ (temp >>> 32));
        temp = Double.doubleToLongBits(high);
        result = 31 * result + (int) (temp ^ (temp >>> 32));
        return result;
    }

    @Override
    public String toString() {
        return "IntervalImpl{" +
                "type=" + type +
                ", low=" + low +
                ", high=" + high +
                '}';
    }

    @Override
    public double getLowerBound() {
        return this.low;
    }

    @Override
    public double getUpperBound() {
        return this.high;
    }

    @Override
    public void setBounds(double l, double u) {
        this.low=l;
        this.high=u;
    }

    @Override
    public boolean intersects(Interval i) {
        return this.intersects(this.type,this.low,this.high);
    }

    @Override
    public boolean intersects(IntervalType type, double low, double high) {
        if (this.high < this.low)
            return false;

        if (this.low > high || this.high < low) return false;
        if ((this.low > low && this.low < high) || (this.high > low && this.high < high)) return true;

        switch (this.type) {
            case IT_CLOSED:
                if (this.low == high) {
                    if (type == IntervalType.IT_CLOSED || type == IntervalType.IT_LEFTOPEN) return true;
                    else return false;
                }
                else if (this.high == low) {
                    if (type == IntervalType.IT_CLOSED || type == IntervalType.IT_RIGHTOPEN) return true;
                    else return false;
                }
                break;
            case IT_OPEN:
                if (this.low == high || this.high == low) return false;
                break;
            case IT_RIGHTOPEN:
                if (this.low == high){
                    if (type == IntervalType.IT_CLOSED || type == IntervalType.IT_LEFTOPEN) return true;
                    else return false;
                }
                else if (this.high == low){
                    return false;
                }
                break;
            case IT_LEFTOPEN:
                if (this.low == high){
                    return false;
                }
                else if (this.high == low){
                    if (type == IntervalType.IT_CLOSED || type == IntervalType.IT_RIGHTOPEN) return true;
                    else return false;
                }
                break;
        }

        return true;
        
    }

    @Override
    public boolean contains(Interval i) {

        if (this.high < this.low) return false;
        double low = i.getLowerBound();
        double high = i.getUpperBound();
        IntervalType type = i.getType();

        if (this.low < low && this.high > high) return true;
        if (this.low > low || this.high < high) return false;

        switch (this.type)
        {
            case IT_CLOSED:
                break;
            case IT_OPEN:
                if ((this.low == low && this.high == high && type != IntervalType.IT_OPEN) ||
                        (this.low == low && (type == IntervalType.IT_CLOSED || type == IntervalType.IT_RIGHTOPEN)) ||
                        (this.high == high && ( type == IntervalType.IT_CLOSED || type == IntervalType.IT_LEFTOPEN)))
                    return false;
                break;
            case IT_RIGHTOPEN:
                if (this.high == high && (type == IntervalType.IT_CLOSED || type == IntervalType.IT_LEFTOPEN))
                    return false;
                break;
            case IT_LEFTOPEN:
                if (this.low == low && (type == IntervalType.IT_CLOSED || type == IntervalType.IT_RIGHTOPEN))
                    return false;
                break;
        }

        return true;
    }

    @Override
    public IntervalType getType() {
        return this.type;
    }

    @Override
    public Object clone() {
        return new IntervalImpl(this.type,this.low,this.high);
    }

    @Override
    public void copyFrom(Object i) {
        if(i instanceof Interval) {
            reset(((Interval)i).getType(),((Interval)i).getLowerBound(),((Interval)i).getUpperBound());
        }
    }

    @Override
    public void copyTo(Object i) {
        if(i instanceof Interval)
            ((Interval)i).reset(this.type,this.low,this.high);
    }

    @Override
    public boolean read(InputStream in) throws IOException {
        DataInputStream dis =new DataInputStream(in);
        int t= dis.readInt();
        this.type=IntervalType.values()[t];
        this.low = dis.readDouble();
        this.high = dis.readDouble();
        return true;
    }

    @Override
    public boolean write(OutputStream out) throws IOException {
        DataOutputStream dos =new DataOutputStream(out);
        dos.writeInt(this.type.ordinal());
        dos.writeDouble(this.low);
        dos.writeDouble(this.high);
        dos.close();
        return true;
    }

    @Override
    public long getByteArraySize() {
        return 8*2+4;
    }

    @Override
    public void reset(IntervalType type, double start, double end) {
        this.type = type;
        this.low = low;
        this.high = high;
    }
}
