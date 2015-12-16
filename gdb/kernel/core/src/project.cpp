#include "internal_projectimpl.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace 


struct DDBProjectImplDeletor{
public:
	void operator()(Project * p){
		DDBProjectImpl * pp = dynamic_cast<DDBProjectImpl*>(p);
		delete pp;
	}
};


ProjectSharedPtr Project::create(int signal,ProjectAdapterSharedPtr prjadapt){
		if(signal == 0)
		{
			return ProjectSharedPtr((Project*)new ProjectImpl());
		}
		else if(1 == signal)
		{
			return ProjectSharedPtr((Project*)new DDBProjectImpl(prjadapt),DDBProjectImplDeletor());
		}
		else
		{
			return ProjectSharedPtr();
		}
} 
//
//
//ProjectSharedPtr Project::create(){
//	return ProjectSharedPtr((Project*)new ProjectImpl());
//}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
