#include "accel_structure.h"

AABB unionBound(const AABB& a, const AABB& b) {
	AABB c;
	c.lowerBound = lower(a.lowerBound, b.lowerBound);
	c.upperBound = upper(a.upperBound, b.upperBound);
	return c;
}

NaiveAccelStructure::NaiveAccelStructure(vector<Object*> objects) {
	for (auto object : objects) {
		container.insert(container.end(), object->primitives.begin(), object->primitives.end());
	}
}

bool NaiveAccelStructure::intersect(const Ray& ray, Intersection& intersection) {
	float indexNearset = -1;
	float tNear = std::numeric_limits<float>::max();
	float b1, b2;
	for (int i = 0; i < container.size(); i++) {
		float t, u, v;
		if (container[i]->intersect(ray, t, u, v) && t < tNear && t > 0) {
			tNear = t;
			b1 = u;
			b2 = v;
			indexNearset = i;
		}
	}
	if (indexNearset == -1) return false;

	intersection.happended = true;
	intersection.distance = tNear;
	intersection.coords = ray.origin + tNear * ray.direction;
	intersection.normal = container[indexNearset]->getNormal(1 - b1 - b2, b1, b2);
	intersection.material = container[indexNearset]->getMaterial();
	return true;
}