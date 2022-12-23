#ifndef PATHTRACER_OBJECT_H
#define PATHTRACER_OBJECT_H

#include "material.h"
#include "triangle.h"

class Object {
public:
	Object() = default;
	Object(const string& filename, shared_ptr<Material> _material);

	vector<Triangle*> primitives;
private:
	shared_ptr<Material> material;
};
#endif // !PATHTRACER_OBJECT_H
