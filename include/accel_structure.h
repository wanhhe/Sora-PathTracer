#ifndef PATHTRACER_ACCEL_STRUCTURE_H
#define PATHTRACER_ACCEL_STRUCTURE_H

#include "ray.h"
#include "intersection.h"

class AccelStructure {
public:
	AccelStructure() = default;
	virtual bool intersect(const Ray& ray, Intersection& intersection) = 0;
};

class NaiveAccelStructure: public AccelStructure {
public:
	explicit NaiveAccelStructure();
	bool intersect(const Ray& ray, Intersection& intersection) override;

private:

};

class BVHAccelStructure : public AccelStructure {
public:
	explicit BVHAccelStructure();
	bool intersect(const Ray& ray, Intersection& intersection) override;

private:


};

class SAHAccelStructure : public AccelStructure {
public:
	explicit SAHAccelStructure();
	bool intersect(const Ray& ray, Intersection& intersection) override;

private:

};


#endif // !PATHTRACER_ACCEL_STRUCTURE_H
