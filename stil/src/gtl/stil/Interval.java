package gtl.stil;


/**
 * Created by ZhenwenHe on 2016/12/7.
 */
public interface Interval extends gtl.stil.Serializable {
     double getLowerBound()  ;
     double getUpperBound()  ;
     void setBounds(double l, double u) ;
     boolean intersects( Interval i)  ;
     boolean intersects(IntervalType type,  double start,  double end)  ;
     boolean contains( Interval i)  ;
     IntervalType getType()  ;
     void reset(IntervalType type,  double start,  double end);
}
