package gtl.stil.rtree;

import gtl.stil.Identifier;
import gtl.stil.Indexable;
import gtl.stil.PropertySet;
import gtl.stil.storage.StorageManager;

import java.util.Map;

/**
 * Created by ZhenwenHe on 2016/12/22.
 */
public interface RTree extends Indexable{
    // String                   Value     Description
    // ----------------------------------------------
    // IndexIdentifier         Identifier   If specified an existing index will be openened from the supplied
    //                          storage manager with the given index id. Behaviour is unspecified
    //                          if the index id or the storage manager are incorrect.
    // Dimension                Integer  Dimensionality of the data that will be inserted.
    // IndexCapacity            Integer  The index node capacity. Default is 100.
    // LeafCapacity             Integer  The leaf node capacity. Default is 100.
    // FillFactor               Double The fill factor. Default is 70%
    // TreeVariant              Integer   Can be one of Linear, Quadratic or Rstar. Default is Rstar
    // NearMinimumOverlapFactor Integer  Default is 32.
    // SplitDistributionFactor  Double Default is 0.4
    // ReinsertFactor           Double Default is 0.3
    // EnsureTightMBRs          Boolean   Default is true
    void reset(StorageManager storageManager, PropertySet propSet);
    void reset(StorageManager storageManager,
            Identifier indexIdentifier,
            int dimension,
            int indexCapacity,int leafCapacity,
            double fillFactor,
            RTreeVariant treeVariant,
            int nearMinimumOverlapFactor,
            double splitDistributionFactor,
            double reinsertFactor,
            boolean ensureTightMBRs );



}
