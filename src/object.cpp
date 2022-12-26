#include "object.h"
#include "obj_loader.h"

Object::Object(const string& filename, shared_ptr<Material> _material) {
	objl::Loader loader;
	if (!loader.LoadFile(filename)) {
		throw std::runtime_error("Load file error!");
	}

	for (int i = 0; i < loader.LoadedMeshes.size(); i++) {
		auto mesh = loader.LoadedMeshes[i];
		for (int j = 0; j < mesh.Vertices.size(); j += 3) {
			Triangle currentTriangle(
				vec3(mesh.Vertices[i].Position.X, mesh.Vertices[i].Position.Y, mesh.Vertices[i].Position.Z),
				vec3(mesh.Vertices[i + 1].Position.X, mesh.Vertices[i + 1].Position.Y, mesh.Vertices[i + 1].Position.Z),
				vec3(mesh.Vertices[i + 2].Position.X, mesh.Vertices[i + 2].Position.Y, mesh.Vertices[i + 2].Position.Z),
				vec3(mesh.Vertices[i].Normal.X, mesh.Vertices[i].Normal.Y, mesh.Vertices[i].Normal.Z),
				vec3(mesh.Vertices[i + 1].Normal.X, mesh.Vertices[i + 1].Normal.Y, mesh.Vertices[i + 1].Normal.Z),
				vec3(mesh.Vertices[i + 2].Normal.X, mesh.Vertices[i + 2].Normal.Y, mesh.Vertices[i + 2].Normal.Z),
				vec2(mesh.Vertices[i].TextureCoordinate.X, mesh.Vertices[i].TextureCoordinate.Y),
				vec2(mesh.Vertices[i + 1].TextureCoordinate.X, mesh.Vertices[i + 1].TextureCoordinate.Y),
				vec2(mesh.Vertices[i + 2].TextureCoordinate.X, mesh.Vertices[i + 2].TextureCoordinate.Y),
				_material
			);
			primitives.emplace_back(currentTriangle);
		}
	}

	material = std::move(_material);
}