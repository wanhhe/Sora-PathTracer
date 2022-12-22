#ifndef PATHTRACER_RAY_H
#define PATHTRACER_RAY_H

#include "common.h"

class Ray {
public:
	Ray() = default;
	Ray(const vec3& _origin, const vec3& _direction) : origin(_origin), direction(_direction) {
		inv_direction = { 1 / direction.x, 1 / direction.y, 1 / direction.z };
	}

private:
	vec3 origin;
	vec3 direction;
	vec3 inv_direction;
};

#endif // !PATHTRACER_RAY_H
