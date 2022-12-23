#ifndef PATHTRACER_SCENE_H
#define PATHTRACER_SCENE_H

#include "common.h"
#include "object.h"
#include "accel_structure.h"

class Scene {
public:
	Scene() = default;

	void buildNaive();
	void buildBVH();
	void buildSAH();

	vec3 castRay(const Ray& ray);

private:
	vector<Object*> objects;
	AccelStructure* accelStructure;
};
#endif // !PATHTRACER_SCENE_H
