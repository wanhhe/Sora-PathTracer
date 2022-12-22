#ifndef PATHTRACER_SCENE_H
#define PATHTRACER_SCENE_H

#include "common.h"

class Scene {
public:
	Scene() = default;

	void buildNaive();
	void buildBVH();
	void buildSAH();

private:
	
};
#endif // !PATHTRACER_SCENE_H
