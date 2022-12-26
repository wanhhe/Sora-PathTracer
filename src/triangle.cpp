#include "triangle.h"

Triangle::Triangle(const vec3& _v0, const vec3& _v1, const vec3& _v2, const vec3& _n0, const vec3& _n1, const vec3& _n2, 
	const vec2& _t0, const vec2& _t1, const vec2& _t2, shared_ptr<Material> _material)
	:v0(_v0), v1(_v1), v2(_v2), n0(normalize(_n0)), n1(normalize(_n1)), n2(normalize(_n2)), t0(_t0), t1(_t1), t2(_t2), material(std::move(_material)) {
	e1 = v1 - v0;
	e2 = v2 - v0;
	//normal = normalize(cross(e1, e2));
	area = glm::length(cross(e1, e2)) * 0.5f;
}

bool Triangle::intersect(const Ray& ray, float& tNear, float& b1, float& b2) const {
	vec3 s1 = cross(ray.direction, e2);
	float det = dot(s1, e1);
	if (det <= 0) return false;

	vec3 s = ray.origin - v0;
	b1 = dot(s1, s);
	// 本来是大于1，但这里b1还没有除以系数，所以相当于放大了
	if (b1 < 0 || b1 > det) return false;

	vec3 s2 = cross(s, e1);
	b2 = dot(s2, ray.direction);
	if (b2 < 0 || b1 + b2 > det) return false;

	float invDet = 1.f / det;
	tNear = invDet * dot(e2, s2);
	b1 *= invDet;
	b2 *= invDet;
	return true;
}

vec3 Triangle::getNormal(float t, float u, float v) const {
	return t * n0 + u * n1 + v * n2;
}