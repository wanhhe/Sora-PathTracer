#include "model.h"
#include "obj_loader.h"

Model::Model(const string& filename, shared_ptr<Material> _material) {
	objl::Loader loader;
	if (!loader.LoadFile(filename)) {
		throw std::runtime_error("Load file error!");
	}

	for (int i = 0; i < loader.LoadedMeshes.size(); i++) {
		auto mesh = loader.LoadedMeshes[i];
		for (int j = 0; j < mesh.Vertices.size(); j += 3) {

		}
	}
}