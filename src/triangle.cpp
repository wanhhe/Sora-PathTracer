#include "triangle.h"

Triangle::Triangle(const vec3& _v0, const vec3& _v1, const vec3& _v2, const vec3& _n0, const vec3& _n1, const vec3& _n2, const vec3& _t0, const vec3& _t1, const vec3& _t2)
	:v0(_v0), v1(_v1), v2(_v2), n0(normalize(_n0)), n1(normalize(_n1)), n2(normalize(_n2)), t0(_t0), t1(_t1), t2(_t2) {
	e1 = v1 - v0;
	e2 = v2 - v0;
	//normal = normalize(cross(e1, e2));
	area = glm::length(cross(e1, e2)) * 0.5f;
}

float Triangle::getArea() const {
	return area;
}