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
}

void Scene::loadSceneBunny(Config* config) {

}

void Scene::buildNaive() {
	accelStructure = new NaiveAccelStructure(objects);
}

void Scene::buildBVH() {
	accelStructure = new BVHAccelStructure(objects);
}

void Scene::buildSAH() {

}