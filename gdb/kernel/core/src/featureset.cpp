#include "internal_featuresetimpl.h"



begin_gtl_namespace
begin_gdb_namespace


FeatureSetSharedPtr FeatureSet::create(FeatureClassSharedPtr fcls) {
	FeatureSetSharedPtr p(new FeatureSetImpl(fcls));
	p->getFeatureClass()->setLocalFeatureSet(p);
	return p;
}

FeatureSetSharedPtr FeatureSet::create(const FeatureClassRef  fcls ){
	FeatureSetSharedPtr p(new FeatureSetImpl(fcls));
	p->getFeatureClass()->setLocalFeatureSet(p);
	return p;
}




end_gdb_namespace
end_gtl_namespace


