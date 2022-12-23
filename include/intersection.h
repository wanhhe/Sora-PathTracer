#ifndef PATHTRACER_INTERSECTION_H
#define PATHTRACER_INTERSECTION_H

#include "common.h"

struct Intersection {
	Intersection() {
		happended = false;
	}

	bool happended;
	vec3 coords;
	vec3 tcoords;
	vec3 normal;
};

#endif // !PATHTRACER_INTERSECTION_H
