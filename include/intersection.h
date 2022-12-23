#ifndef PATHTRACER_INTERSECTION_H
#define PATHTRACER_INTERSECTION_H

#include "common.h"

class Material;

struct Intersection {
	Intersection() {
		happended = false;
		distance = std::numeric_limits<float>::max();
	}

	bool happended;
	vec3 coords;
	vec3 tcoords;
	vec3 normal;
	float distance;
	shared_ptr<Material> material;
};

#endif // !PATHTRACER_INTERSECTION_H
