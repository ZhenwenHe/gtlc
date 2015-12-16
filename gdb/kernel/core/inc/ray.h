#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "material.h"
#include "matrix4x4.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
/** @addtogroup Geometry Ray-…‰œﬂ¿‡
*  @{
*/
class GV3DCORE_API Ray{
public:
	virtual Vertex3d getOrigin() = 0;
	virtual void setOrigin(const Vertex3d& d) = 0;
	virtual Vertex3d getDirection() = 0;
	virtual void setDirection(const Vertex3d& d) = 0;
	virtual Vertex3d getInverseDirection() = 0;
	virtual void set(const Vertex3d& o, const Vertex3d& d) = 0;
	virtual std::shared_ptr<Ray> clone() = 0;
	static std::shared_ptr<Ray> create(const Vertex3d& o, const Vertex3d& d);
};
typedef std::shared_ptr<Ray>  RaySharedPtr;
/** @} */

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace


