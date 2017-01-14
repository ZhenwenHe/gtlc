package gtl.stil.impl;

import gtl.stil.Interval;
import gtl.stil.IntervalType;

import java.io.*;
import java.util.DoubleSummaryStatistics;

/**
 * Created by ZhenwenHe on 2016/12/8.
 */
public class IntervalImpl implements Interval {
    private IntervalType type;
    private double low;
    private double high;



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

        if(type != interval.type) return false;

        if (Double.compare(interval.low, low) != 0) return false;
        if (Double.compare(interval.high, high) != 0) return false;

        return true;

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
    public boolean lowerClosed() {
        return !(this.type==IntervalType.IT_OPEN || this.type==IntervalType.IT_LEFTOPEN);
    }

    @Override
    public boolean upperClosed() {
        return !(this.type==IntervalType.IT_OPEN || this.type==IntervalType.IT_RIGHTOPEN);
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

    /**
     * Equals Query: Is = Qs and Ie = Qe.
     * @param q : the input interval for query
     * @return : if equals to q ,return true ,else return false
     */
    @Override
    public boolean equals(Interval q) {
        return equals((Object) q);
    }

    /**
     * Starts Query: Is = Qs and Qs < Ie < Qe; as shown in Fig. 3a.
     * @param q : the input interval for query
     * @return: true or false
     */
    @Override
    public boolean starts(Interval q) {
        //如果左端的开闭状态不一致，直接返回false
        if(q.lowerClosed()!=this.lowerClosed()) return false;

        if(Double.compare(q.getLowerBound(),this.getLowerBound())==0){
            if(q.getLowerBound()<this.getUpperBound() &&
                    this.getUpperBound()<q.getUpperBound())
                return true;
        }
        return false;
    }

    /**
     * StartedBy Query: Is = Qs and Ie > Qe; as shown in Fig. 3b.
     * @param q: the input interval for query
     * @return: true or false
     */
    @Override
    public boolean startedBy(Interval q) {
        //如果左端的开闭状态不一致，直接返回false
        if(q.lowerClosed()!=this.lowerClosed()) return false;

        if(Double.compare(q.getLowerBound(),this.getLowerBound())==0){
            if(q.getUpperBound()<this.getUpperBound())
                return true;
        }
        return false;
    }

    /**
     * Meets Query: Is < Ie = Qs < Qe; as shown in Fig. 3c.
     * @param q: the input interval for query
     * @return: true or false
     */
    @Override
    public boolean meets(Interval q) {

        if(q.lowerClosed()!=this.upperClosed()) return false;

        if(Double.compare(q.getLowerBound(),this.getUpperBound())==0){
            return true;
        }

        return false;
    }

    /**
     * MetBy Query: Qs < Qe = Is < Ie; as shown in Fig. 3d.
     * @param q: the input interval for query
     * @return: true or false
     */
    @Override
    public boolean metBy(Interval q) {

        if(q.upperClosed()!=this.lowerClosed()) return false;

        if(Double.compare(q.getUpperBound(),this.getLowerBound())==0){
            return true;
        }

        return false;
    }

    /**
     *  this is (Is,Ie)
     *  q is the input parameter Inteval （Qs,Qe）
     *Finishes Query: Qs < Is < Qe and Ie = Qe; as shown in Fig. 3e.
     * @param q: the input interval for query
     * @return true or false
     */
    @Override
    public boolean finishes(Interval q) {
        if(this.upperClosed()!=q.upperClosed()) return false;
        if(Double.compare(this.getUpperBound(),q.getUpperBound())==0){
            if(Double.compare(this.getLowerBound(),q.getLowerBound())>0 &&
                    Double.compare(this.getLowerBound(),q.getUpperBound())<0)
                return true;
        }
        return false;
    }

    /**
     *  this is (Is,Ie)
     *  q is the input parameter Inteval （Qs,Qe）
     *FinishedBy Query: Is < Qs and Ie = Qe; as shown in Fig. 3f.
     */
    @Override
    public boolean finishedBy(Interval q) {

        if(this.upperClosed()!=q.upperClosed()) return false;
        if(Double.compare(this.getUpperBound(),q.getUpperBound())==0){
            if(Double.compare(this.getLowerBound(),q.getLowerBound())<0 )
                return true;
        }
        return false;
    }

    /**
     *  this is (Is,Ie)
     *  q is the input parameter Inteval （Qs,Qe）
     *Before Query: Is < Ie < Qs < Qe; as shown in Fig. 3a.
     */
    @Override
    public boolean before(Interval q) {

        int i = Double.compare(q.getLowerBound(),this.getUpperBound());
        if(i>0) return true;
        else if(i==0){
            if(q.lowerClosed() && this.upperClosed())
                return false;
            else
                return true;
        }
        else
            return false;
    }

    /**
     *  this is (Is,Ie)
     *  q is the input parameter Inteval （Qs,Qe）
     *After Query: Qs < Qe < Is < Ie; as shown in Fig. 4b.
     */
    @Override
    public boolean after(Interval q) {

        int i = Double.compare(q.getUpperBound(),this.getLowerBound());

        if(i<0)
            return true;
        else if(i==0){
            if(q.upperClosed() && this.lowerClosed())
                return false;
            else
                return true;
        }
        else
            return false;
    }

    /**
     *  this is (Is,Ie)
     *  q is the input parameter Inteval （Qs,Qe）
     *Overlaps Query: Is < Qs and Qs < Ie < Qe; as shown in Fig. 4c.
     */
    @Override
    public boolean overlaps(Interval q) {
        int i = Double.compare(this.getLowerBound(),q.getLowerBound());
        if(i>0){
            return false;
        }
        if(i==0){
            if(!(this.lowerClosed() && (!q.lowerClosed())))
                return false;
        }
        //Ie < Qe?
        int j =Double.compare(q.getUpperBound(),this.getUpperBound());
        if(j<0){
            return false;
        }
        else if(j==0){
            if(q.upperClosed()&& (!this.upperClosed()))
                return true;
            else
                return false;
        }
        else {
            //Qs < Ie
            int k = Double.compare(this.getUpperBound(),q.getLowerBound());
            if(k>0){
                return true;
            }
            else if(k==0){
                if(this.upperClosed() && (!q.lowerClosed())){
                    return true;
                }
                else
                    return false;
            }
            else
                return false;
        }
    }

    /**
     *  this is (Is,Ie)
     *  q is the input parameter Inteval （Qs,Qe）
     *OverlappedBy Query: Qs < Is < Qe and Ie > Qe; as shown in      Fig. 4d.
     */
    @Override
    public boolean overlappedBy(Interval q) {
        //compare Ie and Qe
        int i = Double.compare(this.getUpperBound(),q.getUpperBound());
        if(i<0) return false;
        if(i==0){
            if(!(this.upperClosed() && (!q.upperClosed())))
                return false;
        }

        //compare Qs and Is
        i = Double.compare(this.getLowerBound(),q.getLowerBound());
        if(i<0) return false;
        if(i==0){
            if(!(this.lowerClosed() && (!q.lowerClosed()))){
                return false;
            }
        }

        //compare Qe and Is
        i=Double.compare(q.getUpperBound(),this.getLowerBound());
        if(i<0) return false;
        if(i==0){
            if(this.lowerClosed()==false && q.upperClosed())
                return true;
            else
                return false;
        }
        return true;
    }

    /**
     *  this is (Is,Ie)
     *  q is the input parameter Inteval （Qs,Qe）
     *During Query: Qs < Is < Ie < Qe; as shown in Fig. 4e.
     */
    @Override
    public boolean during(Interval q) {
        int i=Double.compare(q.getLowerBound(),this.getLowerBound());
        if(i>0)
            return false;
        if(i==0){
            if(!(q.lowerClosed() && this.lowerClosed()==false))
                return false;
        }
        i = Double.compare(q.getUpperBound(), this.getUpperBound());
        if(i<0) return false;
        if(i==0){
            if(q.upperClosed() && this.upperClosed()==false)
                return true;
            else
                return false;
        }
        return true;
    }

    /**
     *  this is (Is,Ie)
     *  q is the input parameter Inteval （Qs,Qe）
     * Covers Query: Is <= Qs < Qe <= Ie; as shown in Fig. 4f.
     */
    @Override
    public boolean covers(Interval q) {
        if (this.high < this.low) return false;
        int l = Double.compare(q.getLowerBound(),this.getLowerBound());
        int u = Double.compare(this.getUpperBound(),q.getUpperBound());
        //if (this.low < q.low && this.high > q.high) return true;
        if(l>0 && u>0 ) return true;
        //if (this.low > q.low || this.high < q.high) return false;
        if(l<0 || u<0 ) return false;

        // 1) l==0 u>0
        // 2) l==0 u==0
        if(l==0){
            boolean b = (q.lowerClosed()==true) && this.lowerClosed()==false;
            if(b)
                return false;
        }
        //1)u==0 l>0
        //2)u==0 l==0
        if(u==0){
            boolean b= this.upperClosed()==false && q.upperClosed()==true;
            if(b)
                return false;
        }
        return true;
    }

    /**
     *  this is (Is,Ie)
     *  q is the input parameter Inteval （Qs,Qe）
     * Covers Query: Qs <= Is < Ie <= Qe; as shown in Fig. 4f.
     */
    @Override
    public boolean coveredBy(Interval q) {
        return q.covers((Interval) this);
    }

    /**
     *  this is (Is,Ie)
     *  q is the input parameter Inteval （Qs,Qe）
     *Contains Query: Is < Qs < Qe < Ie; as shown in Fig. 4f.
     */
    @Override
    public boolean contains(Interval q) {
        if (this.high < this.low) return false;
        int l = Double.compare(q.getLowerBound(),this.getLowerBound());
        int u = Double.compare(this.getUpperBound(),q.getUpperBound());
        //if (this.low < q.low && this.high > q.high) return true;
        if(l>0 && u>0 ) return true;
        //if (this.low > q.low || this.high < q.high) return false;
        if(l<0 || u<0 ) return false;

        // 1) l==0 u>0
        // 2) l==0 u==0
        if(l==0){
            boolean b = (q.lowerClosed()==false) && this.lowerClosed();
            if(!b)
                return false;
        }
        //1)u==0 l>0
        //2)u==0 l==0
        if(u==0){
            boolean b= this.upperClosed() && q.upperClosed()==false;
            if(!b)
                return false;
        }
        return true;
    }


}
