#pragma once
#include "ray.h"


begin_gtl_namespace
begin_gdb_namespace
class RayImpl : public Ray
{
public:
	RayImpl(void);
	~RayImpl(void);

	Vertex3d origin;
	Vertex3d direction;
	Vertex3d inv_direction;
	int sign[3];

	RayImpl(const Vertex3d& o, const Vertex3d& d) {
		origin = o;
		direction = d;
		inv_direction = Vertex3d(1 / d.x, 1 / d.y, 1 / d.z);
		sign[0] = (inv_direction.x < 0);
		sign[1] = (inv_direction.y < 0);
		sign[2] = (inv_direction.z < 0);
	}

	RayImpl(const RayImpl &r) {
		origin = r.origin;
		direction = r.direction;
		inv_direction = r.inv_direction;
		sign[0] = r.sign[0]; sign[1] = r.sign[1]; sign[2] = r.sign[2];
	}
	virtual Vertex3d getOrigin(){
		return Vertex3d(origin.x, origin.y, origin.z);
	}
	virtual void setOrigin(const Vertex3d& o){
		origin.x = o.x;
		origin.y = o.y;
		origin.z = o.z;
	}
	virtual Vertex3d getDirection(){
		return Vertex3d(direction.x, direction.y, direction.z);
	}
	virtual void setDirection(const Vertex3d& d){
		direction.x = d.x;
		direction.y = d.y;
		direction.z = d.z;

		inv_direction = Vertex3d(1 / d.x, 1 / d.y, 1 / d.z);

		sign[0] = (inv_direction.x < 0);
		sign[1] = (inv_direction.y < 0);
		sign[2] = (inv_direction.z < 0);
	}
	virtual Vertex3d getInverseDirection(){
		return Vertex3d(inv_direction.x, inv_direction.y, inv_direction.z);
	}
	virtual void set(const Vertex3d& o, const Vertex3d& d){
		origin.x = o.x;
		origin.y = o.y;
		origin.z = o.z;

		direction.x = d.x;
		direction.y = d.y;
		direction.z = d.z;

		inv_direction = Vertex3d(1 / d.x, 1 / d.y, 1 / d.z);

		sign[0] = (inv_direction.x < 0);
		sign[1] = (inv_direction.y < 0);
		sign[2] = (inv_direction.z < 0);
	}
	virtual RaySharedPtr clone() {
		return RaySharedPtr((Ray *) new RayImpl(*this));
	}
};
end_gdb_namespace
end_gtl_namespace


