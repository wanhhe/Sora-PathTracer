#ifndef PATHTRACER_TRIANGLE_H
#define PATHTRACER_TRIANGLE_H

#include "common.h"

class Triangle {
public:
	Triangle(const vec3& _v0, const vec3& _v1, const vec3& _v2, const vec3& _n0, const vec3& _n1, const vec3& _n2, const vec3& _t0, const vec3& _t1, const vec3& _t2);
	
	float getArea() const;

private:
	vec3 v0, v1, v2;  // vertices
	vec3 n0, n1, n2;  // normal
	vec3 t0, t1, t2;  // texture
	vec3 e1, e2;  // edge
	//vec3 normal;  // total normal
	float area;
};

#endif // !PATHTRACER_TRIANGLE_H
