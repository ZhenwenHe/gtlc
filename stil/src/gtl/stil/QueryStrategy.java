package gtl.stil;


/**
 * Created by ZhenwenHe on 2016/12/6.
 */
public interface QueryStrategy extends Serializable{
    void getNextEntry(Entry previouslyFetched, Identifier nextEntryToFetch, boolean bFetchNextEntry) ;
}
