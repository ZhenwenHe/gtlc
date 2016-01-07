#include "internal_layerimpl.h"



begin_gtl_namespace
begin_gdb_namespace 

LayerSharedPtr Layer::create(){
	return LayerSharedPtr(new LayerImpl());
}
end_gdb_namespace
end_gtl_namespace


