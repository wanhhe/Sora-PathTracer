#ifndef PATHTRACER_MODEL_H
#define PATHTRACER_MODEL_H

#include "material.h"
#include "triangle.h"

class Model {
public:
	Model(const string& filename, shared_ptr<Material> _material);
private:
	vector<Triangle*> primitives;
	shared_ptr<Material> material;
};

#endif // !PATHTRACER_MODEL_H
