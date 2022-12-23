#ifndef PATHTRACER_ACCEL_STRUCTURE_H
#define PATHTRACER_ACCEL_STRUCTURE_H

#include "triangle.h"
#include "intersection.h"
#include "object.h"

inline vec3 lower(vec3 a, vec3 b) {
	float minX = a.x < b.x ? a.x : b.x;
	float minY = a.y < b.y ? a.y : b.y;
	float minZ = a.z < b.z ? a.z : b.z;
	return { minX, minY, minZ };
}

inline vec3 upper(vec3 a, vec3 b) {
	float maxX = a.x > b.x ? a.x : b.x;
	float maxY = a.y > b.y ? a.y : b.y;
	float maxZ = a.z > b.z ? a.z : b.z;
	return { maxX, maxY, maxZ };
}

struct AABB
{
	vec3 lowerBound;
	vec3 upperBound;
};

class AccelStructure {
public:
	AccelStructure() = default;
	virtual bool intersect(const Ray& ray, Intersection& intersection) = 0;
};

class NaiveAccelStructure: public AccelStructure {
public:
	explicit NaiveAccelStructure(vector<Object*> objects);
	bool intersect(const Ray& ray, Intersection& intersection) override;

private:
	vector<Triangle*> container;
};

class BVHAccelStructure : public AccelStructure {
public:
	explicit BVHAccelStructure(vector<Object*> objects);
	bool intersect(const Ray& ray, Intersection& intersection) override;

private:


};

class SAHAccelStructure : public AccelStructure {
public:
	explicit SAHAccelStructure(vector<Object*> objects);
	bool intersect(const Ray& ray, Intersection& intersection) override;

private:

};


#endif // !PATHTRACER_ACCEL_STRUCTURE_H
