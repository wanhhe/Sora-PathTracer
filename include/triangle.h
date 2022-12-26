#ifndef PATHTRACER_TRIANGLE_H
#define PATHTRACER_TRIANGLE_H

#include "common.h"
#include "ray.h"
#include "material.h"

class Triangle {
public:
	Triangle(const vec3& _v0, const vec3& _v1, const vec3& _v2, const vec3& _n0, const vec3& _n1, const vec3& _n2, 
		const vec2& _t0, const vec2& _t1, const vec2& _t2, shared_ptr<Material> _material);
	
	bool intersect(const Ray& ray, float& tNear, float& b1, float& b2) const;
	vec3 getNormal(float t, float u, float v) const;
	shared_ptr<Material> getMaterial() const { return material; }
	float getArea() const { return area; }

	float getBarycenterX() const { return (v0.x + v1.x + v2.x) / 3.0f; }
	float getBarycenterY() const { return (v0.y + v1.y + v2.y) / 3.0f; }
	float getBarycenterZ() const { return (v0.z + v1.z + v2.z) / 3.0f; }

public:
	vec3 v0, v1, v2;  // vertices
	vec3 n0, n1, n2;  // normal
	vec2 t0, t1, t2;  // texture
	vec3 e1, e2;  // edge
	//vec3 normal;  // total normal
	float area;
	shared_ptr<Material> material;
};

#endif // !PATHTRACER_TRIANGLE_H
