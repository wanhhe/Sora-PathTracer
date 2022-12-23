#ifndef PATHTRACER_TRIANGLE_H
#define PATHTRACER_TRIANGLE_H

#include "common.h"
#include "ray.h"
#include "material.h"

class Triangle {
public:
	Triangle(const vec3& _v0, const vec3& _v1, const vec3& _v2, const vec3& _n0, const vec3& _n1, const vec3& _n2, const vec3& _t0, const vec3& _t1, const vec3& _t2);
	
	bool intersect(const Ray& ray, float& tNear, float& b1, float& b2) const;
	vec3 getNormal(float t, float u, float v) const;
	shared_ptr<Material> getMaterial() const { return material; }
	float getArea() const { return area; }

public:
	vec3 v0, v1, v2;  // vertices
	vec3 n0, n1, n2;  // normal
	vec3 t0, t1, t2;  // texture
	vec3 e1, e2;  // edge
	//vec3 normal;  // total normal
	float area;
	shared_ptr<Material> material;
};

#endif // !PATHTRACER_TRIANGLE_H
