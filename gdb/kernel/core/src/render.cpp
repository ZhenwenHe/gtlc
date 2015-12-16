#include "render.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
 
/////////////////////////////////////////////////////////////////////

Render::Render():_origin(0,0,0){
}
Render2d::Render2d(){
	_dimension=2;
}
Render3d::Render3d(){
	_dimension=3;
}
Render3d * Render::asRender3d(){
	return dynamic_cast<Render3d *>(this);
}

Render2d * Render::asRender2d(){
	return dynamic_cast<Render2d *>(this);
}



end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
