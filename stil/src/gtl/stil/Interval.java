package gtl.stil;

import java.io.Serializable;

/**
 * Created by ZhenwenHe on 2016/12/7.
 */
public interface Interval extends Serializable {
     double getLowerBound()  ;
     double getUpperBound()  ;
     void setBounds(double l, double u) ;
     boolean intersectsInterval( Interval i)  ;
     boolean intersectsInterval(IntervalType type,  double start,  double end)  ;
     boolean containsInterval( Interval i)  ;
     IntervalType getIntervalType()  ;
}
