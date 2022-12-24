#include "scene.h"

void Scene::loadSceneBox(Config* config) {
	auto redDiffuseMaterial = make_shared<DiffuseMaterial>(vec3(0.63, 0.065, 0.05));
	auto greenDiffuseMaterial = make_shared<DiffuseMaterial>(vec3(0.14, 0.45, 0.091));
	auto whiteDiffuseMaterial = make_shared<DiffuseMaterial>(vec3(0.725, 0.71, 0.68));
	auto lightMaterial = make_shared<DiffuseLightMaterial>(vec3(23.918, 19.283, 15.541));

	shared_ptr<Material> tallBoxMaterial;
	switch (config->material)
	{
	case DIFFUSE: tallBoxMaterial = whiteDiffuseMaterial; break;
	case MIRROR: tallBoxMaterial = make_shared<MirrorMaterial>(vec3(0.725, 0.71, 0.68)); break;
	case MICROFACET: tallBoxMaterial = make_shared<MicrofacetMaterial>(vec3(0.725, 0.71, 0.68), config->roughness); break;
	default:
		throw std::runtime_error("Material not supported");
	}

	Object* floor = new Object("../models/cornellbox/floor.obj", whiteDiffuseMaterial);
	Object* left = new Object("../models/cornellbox/left.obj", redDiffuseMaterial);
	Object* right = new Object("../models/cornellbox/right.obj", greenDiffuseMaterial);
	Object* shortBox = new Object("../models/cornellbox/shortbox.obj", whiteDiffuseMaterial);
	Object* tallBox = new Object("../models/cornellbox/tallbox.obj", tallBoxMaterial);
	Object* light = new Object("../models/cornellbox/light.obj", lightMaterial);

	objects.emplace_back(floor);
	objects.emplace_back(left);
	objects.emplace_back(right);
	objects.emplace_back(shortBox);
	objects.emplace_back(tallBox);
	objects.emplace_back(light);


}

void Scene::loadSceneBunny(Config* config) {
	auto redDiffuseMaterial = make_shared<DiffuseMaterial>(vec3(0.63, 0.065, 0.05));
	auto greenDiffuseMaterial = make_shared<DiffuseMaterial>(vec3(0.14, 0.45, 0.091));
	auto whiteDiffuseMaterial = make_shared<DiffuseMaterial>(vec3(0.725, 0.71, 0.68));
	auto lightMaterial = make_shared<DiffuseLightMaterial>(vec3(23.918, 19.283, 15.541));

	shared_ptr<Material> bunnyMaterial;
	switch (config->material)
	{
	case DIFFUSE: bunnyMaterial = whiteDiffuseMaterial; break;
	case MIRROR: bunnyMaterial = make_shared<MirrorMaterial>(vec3(0.725, 0.71, 0.68)); break;
	case MICROFACET: bunnyMaterial = make_shared<MicrofacetMaterial>(vec3(0.725, 0.71, 0.68), config->roughness);
	default:
		throw std::runtime_error("Material not supported");
	}

	auto floor = new Object("../models/cornellbox/floor.obj", whiteDiffuseMaterial);
	auto left = new Object("../models/cornellbox/left.obj", redDiffuseMaterial);
	auto right = new Object("../models/cornellbox/right.obj", greenDiffuseMaterial);
	auto light = new Object("../models/cornellbox/light.obj", lightMaterial);
	auto bunny = new Object("../models/bunny/bunny.obj", bunnyMaterial);

	objects.emplace_back(floor);
	objects.emplace_back(left);
	objects.emplace_back(right);
	objects.emplace_back(light);
	objects.emplace_back(bunny);
}

void Scene::buildNaive() {
	accelStructure = new NaiveAccelStructure(objects);
}

void Scene::buildBVH() {
	accelStructure = new BVHAccelStructure(objects);
}

void Scene::buildSAH() {
	accelStructure = new SAHAccelStructure(objects);
}