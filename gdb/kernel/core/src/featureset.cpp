#include "internal_featuresetimpl.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
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
end_cug_namespace
end_edu_namespace
end_cn_namespace
