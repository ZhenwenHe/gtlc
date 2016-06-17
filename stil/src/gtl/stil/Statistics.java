package gtl.stil;


/**
 * Created by ZhenwenHe on 2016/12/6.
 */
public interface Statistics extends Serializable{
     long getReads()   ;
     long getWrites()  ;
     int getNumberOfNodes() ;
     long getNumberOfData();
}
