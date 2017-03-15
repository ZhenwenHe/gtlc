package gtl.math.geometry;


import gtl.storage.Serializable;

/**
 * Created by ZhenwenHe on 2016/12/7.
 */
public interface Interval extends Serializable {
     double getLowerBound()  ;
     double getUpperBound()  ;
     void setBounds(double l, double u) ;
     boolean intersects( Interval i)  ;
     boolean intersects(IntervalType type,  double start,  double end)  ;
     IntervalType getType()  ;
     void reset(IntervalType type,  double start,  double end);
     boolean lowerClosed();
     boolean upperClosed();
     boolean isValid();

     /**
      *  this is (Is,Ie)
      *  q is the input parameter Inteval （Qs,Qe）
      *
      *Equals Query: Is = Qs and Ie = Qe.
      *Starts Query: Is = Qs and Qs < Ie < Qe; as shown in Fig. 3a.
      *StartedBy Query: Is = Qs and Ie > Qe; as shown in Fig. 3b.
      *Meets Query: Is < Ie = Qs < Qe; as shown in Fig. 3c.
      *MetBy Query: Qs < Qe = Is < Ie; as shown in Fig. 3d.
      *Finishes Query: Qs < Is < Qe and Ie = Qe; as shown in Fig. 3e.
      *FinishedBy Query: Is < Qs and Ie = Qe; as shown in Fig. 3f.
      *Before Query: Is < Ie < Qs < Qe; as shown in Fig. 3a.
      *After Query: Qs < Qe < Is < Ie; as shown in Fig. 4b.
      *Overlaps Query: Is < Qs and Qs < Ie < Qe; as shown in Fig. 4c.
      *OverlappedBy Query: Qs < Is < Qe and Ie > Qe; as shown in      Fig. 4d.
      *During Query: Qs < Is < Ie < Qe; as shown in Fig. 4e.
      *Contains Query: Is < Qs < Qe < Ie; as shown in Fig. 4f.
      */
     boolean equals(Interval q);
     boolean starts(Interval q);
     boolean startedBy(Interval q);
     boolean meets(Interval q);
     boolean metBy(Interval q);
     boolean finishes(Interval q);
     boolean finishedBy(Interval q);
     boolean before(Interval q);
     boolean after(Interval q);
     boolean overlaps(Interval q);
     boolean overlappedBy(Interval q);
     boolean during(Interval q);
     boolean contains(Interval q);
     boolean covers(Interval q);
     boolean coveredBy(Interval q);
}
